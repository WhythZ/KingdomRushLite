#ifndef _OBJECT_POOL_HPP_
#define _OBJECT_POOL_HPP_

//借鉴自https://github.com/simdsoft/yasio/blob/dev/yasio/impl/ObjectPool.hpp

#include <assert.h>
#include <stdlib.h>
#include <cstddef>
#include <memory>
#include <mutex>
#include <type_traits>

class ObjectPool
{
private:
    typedef struct ChunkLinkNode
    {
        union
        {
            ChunkLinkNode* next;                     //节点空闲时用于构造空闲链表
            char padding[sizeof(std::max_align_t)];  //确保对齐要求以兼容不同平台
        };
        char data[0];                                //存储实际对象数据的柔性数组
	}*ChunkLink;                                     //内存块类型，每个内存块包含多个对象

private:
    void* freeHead;                                  //指向空闲链表（串联所有空闲对象）头部
    ChunkLink chunkHead;                             //指向内存块链表（串联所有分配的内存块）头部
    const size_t elementSize;                        //存储的每个对象的大小
    const size_t elementCount;                       //每个内存块存储的对象数量

public:
	ObjectPool(size_t, size_t);                      //传入存储对象尺寸与数量，预分配一个内存块
	ObjectPool(size_t, size_t, std::false_type);     //传参同上，但不进行预分配
    ObjectPool(const ObjectPool&) = delete;          //删除拷贝构造函数
    void operator=(const ObjectPool&) = delete;      //删除拷贝构造运算符
    virtual ~ObjectPool();

    void* Get();                                     //使用static_cast<MyObj*>(objPool.Get())进行强转
    void Release(void*);                             //释放对象回池中
    void Purge();                                    //释放所有内存块
    void Clean();                                    //重置对象池，将所有对象标记为空闲

private:
	void* AllocateFromChunk(void*);                  //从空闲链表中分配对象
    static void* FirstOf(ChunkLink);                 //
    static void*& NextOf(void* const);               //
    void* AllocateFromProcessHeap();                 //分配新内存块
    void* TidyChunk(ChunkLink);                      //内存块初始化，构建空闲链表
};

ObjectPool::ObjectPool(size_t _elementSize, size_t _elementCount)
    :freeHead(nullptr), chunkHead(nullptr), elementSize(_elementSize), elementCount(_elementCount)
{
    Release(AllocateFromProcessHeap());
}

ObjectPool::ObjectPool(size_t _elementSize, size_t _elementCount, std::false_type)
    :freeHead(nullptr), chunkHead(nullptr), elementSize(_elementSize), elementCount(_elementCount)
{
}

ObjectPool::~ObjectPool()
{
    this->Purge();
}

void* ObjectPool::Get()
{
	//如果空闲链表不为空，则从空闲链表中分配一个对象，否则分配新的内存块并返回其中首个对象
    return (freeHead != nullptr) ? AllocateFromChunk(freeHead) : AllocateFromProcessHeap();
}

void ObjectPool::Release(void* _ptr)
{
    //将被释放的对象插入空闲链表头部，不实际释放内存，留作后续重用
    NextOf(_ptr) = freeHead;
    freeHead = _ptr;
}

void ObjectPool::Purge()
{
    if (this->chunkHead == nullptr)
        return;

    ChunkLinkNode* p, ** q = &this->chunkHead;
    while ((p = *q) != nullptr)
    {
        *q = p->next;
        delete[](uint8_t*)(p);
    }

    freeHead = nullptr;
}

void ObjectPool::Clean()
{
    if (this->chunkHead == nullptr)
        return;

    ChunkLinkNode* chunkHead = this->chunkHead;
    void* last = this->TidyChunk(chunkHead);

    while ((chunkHead = chunkHead->next) != nullptr)
    {
        NextOf(last) = FirstOf(chunkHead);
        last = this->TidyChunk(chunkHead);
    }

    NextOf(last) = nullptr;
    freeHead = FirstOf(this->chunkHead);
}

void* ObjectPool::AllocateFromChunk(void* _current)
{
	//更新空闲链表头部指针，指向下一个对象
    freeHead = NextOf(_current);
	//返回闲置链表原先的头部对象
    return _current;
}

void* ObjectPool::FirstOf(ChunkLink _chunkHead)
{
    return _chunkHead->data;
}

void*& ObjectPool::NextOf(void* const _ptr)
{
    //将传入指针_ptr强转解释为指向指针的指针void**，然后解引用它获取对指针的引用void*&
    return *(static_cast<void**>(_ptr));
}

void* ObjectPool::AllocateFromProcessHeap()
{
    ChunkLink new_chunk = (ChunkLink) new uint8_t[sizeof(ChunkLinkNode) + elementSize * elementCount];
    NextOf(TidyChunk(new_chunk)) = nullptr;

    //link the new_chunk
    new_chunk->next = this->chunkHead;
    this->chunkHead = new_chunk;

    //allocate 1 object
    return AllocateFromChunk(FirstOf(new_chunk));
}

void* ObjectPool::TidyChunk(ChunkLink _chunkHead)
{
    char* last = _chunkHead->data + (elementCount - 1) * elementSize;
    for (char* ptr = _chunkHead->data; ptr < last; ptr += elementSize)
        NextOf(ptr) = (ptr + elementSize);
    return last;
}

//#include <vector>
//#include <stack>
//#include <SDL.h>
//
//template <typename T>
//class ObjectPool
//{
//private:
//	size_t maxSize;                    //对象池的最大大小
//	std::vector<T*> pool;              //存储对象池中的所有对象
//	std::stack<T*> unused;             //存储对象池中的闲置对象，优化掉查询时间
//
//public:
//	ObjectPool(size_t);
//	~ObjectPool();
//
//	void OnUpdate(double);
//	void OnRender(SDL_Renderer*);
//
//	T* GetObject();                    //从对象池中获取一个对象
//};
//
//template<typename T>
//ObjectPool<T>::ObjectPool(size_t _maxSize) : maxSize(_maxSize)
//{
//	//在堆区开辟所有对象
//	pool.reserve(maxSize);
//}
//
//template<typename T>
//ObjectPool<T>::~ObjectPool()
//{
//	for (auto _obj : pool)
//		delete _obj;
//}

#endif