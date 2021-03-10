#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <cstdint>
#include <algorithm>

template <class Obj, int Count>
class ObjectPool
{
    static Obj s_objects[Count];

    static uint8_t s_activeCount;

public:

    static Obj * activateNext();

    static void deactivate(Obj*);

    static void deactivate(uint8_t);

    static void iterate(void* data, void (*)(void*,Obj*,bool&));

    static Obj * objects() { return s_objects; }

    static uint8_t objectCount() { return s_activeCount; }
};

#endif // OBJECTPOOL_H

template <class Obj, int Count>
Obj ObjectPool<Obj,Count>::s_objects[Count];

template <class Obj, int Count>
uint8_t ObjectPool<Obj,Count>::s_activeCount = 0;

template<class Obj, int Count>
Obj *ObjectPool<Obj,Count>::activateNext()
{
    if (s_activeCount == Count) {
        return nullptr;
    }
    s_activeCount++;
    return s_objects + s_activeCount - 1;
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::deactivate(Obj * target)
{
    if (s_activeCount == 0) {
        return;
    }
    int choice = -1;
    for(int i = 0; i < Count; ++i) {
        if (target == (s_objects + i)) {
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
    std::swap(s_objects[idx], s_objects[s_activeCount - 1]);
    --s_activeCount;
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::iterate(void *data, void (*func)(void *, Obj *, bool&))
{
    for(int i = s_activeCount-1; i >= 0; --i) {
        bool destroy = false;
        func(data, s_objects+i, destroy);
        if (destroy) {
            deactivate(i);
        }
    }
}
