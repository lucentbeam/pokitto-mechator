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

    static void foreach(void (*)(Obj*));

    static void testRemove(bool (*)(Obj*));

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
    for(int i = 0; i < Count; ++i) {
        if (target == &s_objects[i]) {
            s_objects[i] = *s_objects[s_activeCount-1];
            s_objects[s_activeCount-1] = *target;
            --s_activeCount;
            return;
        }
    }
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::deactivate(uint8_t idx)
{
//    std::swap(s_objects[idx], s_objects[s_activeCount - 1]);
    Obj * end = s_objects + s_activeCount - 1;
    Obj * target = s_objects + idx;
    s_objects[idx] = *end;
    s_objects[s_activeCount - 1] = *target;
    --s_activeCount;
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::foreach(void (*func)(Obj *))
{
    for(int i = 0; i < s_activeCount; ++i) {
        func(&s_objects[i]);
    }
}

template<class Obj, int Count>
void ObjectPool<Obj,Count>::testRemove(bool (*func)(Obj *))
{
    int i = 0;
    while(i < s_activeCount) {
        if (func(&s_objects[i])) {
            deactivate(i);
        } else {
            ++i;
        }
    }
}
