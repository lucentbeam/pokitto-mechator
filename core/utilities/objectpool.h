#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <cstdint>
#include <algorithm>

template <class Obj, int Count>
class ObjectPool
{
    Obj m_objects[Count];

    uint8_t m_activeCount;

public:

    Obj * activateNext();

    void deactivate(Obj*);

    void deactivate(uint8_t);

    void clear();

    void iterate(void* data, void (*)(void*,Obj*,bool&));

    Obj * objects() { return m_objects; }

    uint8_t objectCount() { return m_activeCount; }
};

//#include "objectpool.cpp"


template<class Obj, int Count>
Obj *ObjectPool<Obj,Count>::activateNext()
{
    if (m_activeCount == Count) {
        return nullptr;
    }
    m_activeCount++;
    return m_objects + m_activeCount - 1;
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::deactivate(Obj * target)
{
    if (m_activeCount == 0) {
        return;
    }
    int choice = -1;
    for(int i = 0; i < Count; ++i) {
        if (target == (m_objects + i)) {
            choice = i;
        }
    }
    if (choice >= 0) {
        deactivate(choice);
    }
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::deactivate(uint8_t idx)
{
    std::swap(m_objects[idx], m_objects[m_activeCount - 1]);
    --m_activeCount;
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::clear()
{
    m_activeCount = 0;
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::iterate(void *data, void (*func)(void *, Obj *, bool&))
{
    for(int i = m_activeCount-1; i >= 0; --i) {
        bool destroy = false;
        func(data, m_objects+i, destroy);
        if (destroy) {
            deactivate(i);
        }
    }
}

#endif // OBJECTPOOL_H
