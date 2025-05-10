#ifndef _OBJECT_POOL_HPP_
#define _OBJECT_POOL_HPP_

#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <chrono>
#include <mutex>

#include <SDL.h>

template <typename T>
class ObjectPool
{
private:
    //自定义删除器，用于将对象释放回池中
    struct PoolDeleter
    {
        ObjectPool* pool;
        PoolDeleter(ObjectPool* _pool) : pool(_pool) {}
        void operator()(T* _ptr)
        {
            pool->ReleaseObject(std::unique_ptr<T>(_ptr, *this));
        }
    };

private:
    //此处使用vector可能导致vector自身的频繁内存分配，可考虑使用list等替换
    std::vector<std::unique_ptr<T>> activeObjects;         //活跃对象列表
    std::vector<std::unique_ptr<T>> inactiveObjects;       //非活跃对象列表

    size_t initialSize;                                    //初始容量上限
    size_t maxSize;                                        //极限容量上限（0表无限制）
    size_t expansionSize;                                  //每次扩容的幅度大小

    mutable std::mutex poolMutex;                          //线程安全锁
    std::chrono::steady_clock::time_point lastShrinkTime;  //上次缩容时间

    //缩容策略函数，默认当活跃对象超过总容量50%时缩容
    std::function<bool(const ObjectPool<T>&)> shrinkPolicy =
        [](const ObjectPool<T>& _pool)
        {
            return _pool.GetInactiveCount() > (_pool.GetActiveCount() + _pool.GetInactiveCount()) / 2;
        };

public:
    ObjectPool(size_t = 100, size_t = 0, size_t = 100);    //默认容量、极限容量、扩容幅度
    ~ObjectPool();
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    void OnUpdate(double);
    void OnRender(SDL_Renderer*);

    std::unique_ptr<T> Acquire();                          //从池中获取对象

    size_t GetTotalCount() const;
    size_t GetActiveCount() const;
    size_t GetInactiveCount() const;
    size_t GetMaxSize() const;

private:
    std::unique_ptr<T> CreateObject();                     //创建新对象
    void ReleaseObject(std::unique_ptr<T> _object);        //释放对象回池

    bool CheckExpand(size_t);                              //对象池扩容
    void CheckShrink();                                    //对象池缩容
};

template <typename T>
ObjectPool<T>::ObjectPool(size_t _initialSize, size_t _maxSize, size_t _expansionSize) :
    initialSize(_initialSize),
    maxSize(_maxSize),
    expansionSize(_expansionSize),
    lastShrinkTime(std::chrono::steady_clock::now())
{
    if (_initialSize == 0)
        throw std::runtime_error("ObjectPool initial size cannot be zero");

    //初始化非活跃对象池
    for (size_t i = 0; i < initialSize; i++)
        inactiveObjects.push_back(CreateObject());
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
    std::lock_guard<std::mutex> lock(poolMutex);
    activeObjects.clear();
    inactiveObjects.clear();
}

template <typename T>
void ObjectPool<T>::OnUpdate(double _delta)
{
    if constexpr (std::is_member_function_pointer_v<decltype(&T::OnUpdate)>)
        _obj->OnUpdate(_delta);
}

template <typename T>
void ObjectPool<T>::OnRender(SDL_Renderer* _renderer)
{
    if constexpr (std::is_member_function_pointer_v<decltype(&T::OnRender)>)
        _obj->OnRender(_renderer);
}

template <typename T>
std::unique_ptr<T> ObjectPool<T>::Acquire()
{
    std::lock_guard<std::mutex> lock(poolMutex);

    //如果非活跃对象为空且池未满，自动扩容
    if (inactiveObjects.empty())
    {
        bool _flag = CheckExpand(expansionSize);
		if (!_flag)
            throw std::runtime_error("ObjectPool reached maximum size (" + std::to_string(maxSize) + ")\n");
    }

    //获取非活跃对象
    std::unique_ptr<T> _obj = inactiveObjects.back();
    inactiveObjects.pop_back();
    activeObjects.push_back(_obj);

    ////重置对象状态（要求T类有Reset方法）
    //if constexpr (std::is_member_function_pointer_v<decltype(&T::Reset)>)
    //    _obj->Reset();

    //返回带有自定义删除器的std:unique_ptr
    return std::unique_ptr<T>(_obj.get(), PoolDeleter(this));
}

template <typename T>
size_t ObjectPool<T>::GetTotalCount() const
{
    std::lock_guard<std::mutex> lock(poolMutex);
    return activeObjects.size() + inactiveObjects.size();
}

template <typename T>
size_t ObjectPool<T>::GetActiveCount() const
{
    std::lock_guard<std::mutex> lock(poolMutex);
    return activeObjects.size();
}

template <typename T>
size_t ObjectPool<T>::GetInactiveCount() const
{
    std::lock_guard<std::mutex> lock(poolMutex);
    return inactiveObjects.size();
}

template <typename T>
size_t ObjectPool<T>::GetMaxSize() const
{
    return maxSize;
}

template <typename T>
std::unique_ptr<T> ObjectPool<T>::CreateObject()
{
    return std::make_unique<T>();
}

template <typename T>
void ObjectPool<T>::ReleaseObject(std::unique_ptr<T> _object)
{
    if (!_object) return;
    std::lock_guard<std::mutex> lock(poolMutex);

    //防止迭代器失效（使用find_if而不是remove_if，后者适用于批量操作），注意Lambda内比较的是get()的底层指针
    auto it = std::find_if(activeObjects.begin(), activeObjects.end(),
        [&_object](const auto& ptr) { return ptr.get() == _object.get(); });
    if (it == activeObjects.end()) return;

    //转移所有权，避免重复构造
    inactiveObjects.push_back(std::move(*it));
    activeObjects.erase(it);

    //检查是否需要自动缩容
    CheckShrink();
}

template <typename T>
bool ObjectPool<T>::CheckExpand(size_t _amount)
{
    //检查是否达到最大容量限制
	size_t _currentTotal = activeObjects.size() + inactiveObjects.size();
    if (_currentTotal >= maxSize)
        return false;

    //若还可扩容，则扩容对应大小，不超过极限容量大小（maxSize==0表示无上限）
	size_t _actualAmount = _amount;
    if (maxSize > 0)
        _actualAmount = std::min(_amount, maxSize - _currentTotal);
    //在非活跃对象列表上进行扩容
    for (size_t i = 0; i < _actualAmount; i++)
        inactiveObjects.push_back(CreateObject());
    return true;
}

template <typename T>
void ObjectPool<T>::CheckShrink()
{
    //避免频繁缩容，至少间隔60秒
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - lastShrinkTime).count() < 60)
        return;

    //检查缩容策略，若满足条件则执行缩容
    if (shrinkPolicy(*this))
    {
        //期望缩容到当前容量的一半，但保留至少initialSize个对象
        size_t _targetSize = std::max(initialSize, (activeObjects.size() + expansionSize) / 2);
        size_t _shrinkAmount = (activeObjects.size() + expansionSize) / 2;
		//从非活跃对象列表中删除多余的对象
        if (inactiveObjects.size() >= _shrinkAmount)
        {
            //直接从末尾erase防止迭代器失效，也可inactiveObjects.resize(inactiveObjects.size() - _shrinkAmount);
            inactiveObjects.erase(inactiveObjects.end() - _shrinkAmount, inactiveObjects.end());
            
			//更新上次缩容时间
            lastShrinkTime = now;
        }
    }
}

#endif