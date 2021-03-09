#ifndef BABYFSM_H
#define BABYFSM_H

#include <cstdint>

#include "core/rendering/rendersystem.h"

const uint8_t maxStates = 9;

class FSM {
    void (*updates[maxStates])(FSM&);
    void (*draws[maxStates])(RenderSystem*);

    uint8_t m_current;

    bool m_initialized = false;

public:
    FSM() {
        instance = this;
    }

    FSM &add(uint8_t s, void (*upd)(FSM&) = nullptr, void (*drw)(RenderSystem*) = nullptr) {
        updates[s] = upd;
        draws[s] = drw;

        if (!m_initialized) {
            m_current = s;
            m_initialized = true;
        }
        return *this;
    }

    void go(uint8_t s) {
        if (s == m_current) {
            return;
        }
        m_current = s;
    }

    void update() {
        if (updates[m_current] != nullptr) {
            updates[m_current](*this);
        }
    }

    void draw(RenderSystem * renderer) {
        if (draws[m_current] != nullptr) {
            draws[m_current](renderer);
        }
    }

    static FSM * instance;
};

#endif // BABYFSM_H
