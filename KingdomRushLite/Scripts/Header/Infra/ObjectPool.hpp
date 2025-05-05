#ifndef _OBJECT_POOL_HPP_
#define _OBJECT_POOL_HPP_

//�����https://github.com/simdsoft/yasio/blob/dev/yasio/impl/ObjectPool.hpp

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
            ChunkLinkNode* next;                     //�ڵ����ʱ���ڹ����������
            char padding[sizeof(std::max_align_t)];  //ȷ������Ҫ���Լ��ݲ�ͬƽ̨
        };
        char data[0];                                //�洢ʵ�ʶ������ݵ���������
	}*ChunkLink;                                     //�ڴ�����ͣ�ÿ���ڴ������������

private:
    void* freeHead;                                  //ָ����������������п��ж���ͷ��
    ChunkLink chunkHead;                             //ָ���ڴ�������������з�����ڴ�飩ͷ��
    const size_t elementSize;                        //�洢��ÿ������Ĵ�С
    const size_t elementCount;                       //ÿ���ڴ��洢�Ķ�������

public:
	ObjectPool(size_t, size_t);                      //����洢����ߴ���������Ԥ����һ���ڴ��
	ObjectPool(size_t, size_t, std::false_type);     //����ͬ�ϣ���������Ԥ����
    ObjectPool(const ObjectPool&) = delete;          //ɾ���������캯��
    void operator=(const ObjectPool&) = delete;      //ɾ���������������
    virtual ~ObjectPool();

    void* Get();                                     //ʹ��static_cast<MyObj*>(objPool.Get())����ǿת
    void Release(void*);                             //�ͷŶ���س���
    void Purge();                                    //�ͷ������ڴ��
    void Clean();                                    //���ö���أ������ж�����Ϊ����

private:
	void* AllocateFromChunk(void*);                  //�ӿ��������з������
    static void* FirstOf(ChunkLink);                 //
    static void*& NextOf(void* const);               //
    void* AllocateFromProcessHeap();                 //�������ڴ��
    void* TidyChunk(ChunkLink);                      //�ڴ���ʼ����������������
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
	//�����������Ϊ�գ���ӿ��������з���һ�����󣬷�������µ��ڴ�鲢���������׸�����
    return (freeHead != nullptr) ? AllocateFromChunk(freeHead) : AllocateFromProcessHeap();
}

void ObjectPool::Release(void* _ptr)
{
    //�����ͷŵĶ�������������ͷ������ʵ���ͷ��ڴ棬������������
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
	//���¿�������ͷ��ָ�룬ָ����һ������
    freeHead = NextOf(_current);
	//������������ԭ�ȵ�ͷ������
    return _current;
}

void* ObjectPool::FirstOf(ChunkLink _chunkHead)
{
    return _chunkHead->data;
}

void*& ObjectPool::NextOf(void* const _ptr)
{
    //������ָ��_ptrǿת����Ϊָ��ָ���ָ��void**��Ȼ�����������ȡ��ָ�������void*&
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
//	size_t maxSize;                    //����ص�����С
//	std::vector<T*> pool;              //�洢������е����ж���
//	std::stack<T*> unused;             //�洢������е����ö����Ż�����ѯʱ��
//
//public:
//	ObjectPool(size_t);
//	~ObjectPool();
//
//	void OnUpdate(double);
//	void OnRender(SDL_Renderer*);
//
//	T* GetObject();                    //�Ӷ�����л�ȡһ������
//};
//
//template<typename T>
//ObjectPool<T>::ObjectPool(size_t _maxSize) : maxSize(_maxSize)
//{
//	//�ڶ����������ж���
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