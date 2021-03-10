#ifndef __CONTROLS
#define __CONTROLS

#include <cstdint>

class Controls;
class Button {
    uint16_t downCount = 0;

    friend Controls;
public:
    void update(bool held) { downCount = held ? downCount + 1 : 0; }
    bool pressed() const { return downCount == 1; }
    bool held() const { return downCount > 0; }
    bool downEvery(int initial, int thereafter) {
        return initial == downCount || (downCount > initial && (downCount - initial) % thereafter == 0);
    }
};

struct ControlStatus {
  float x = 0.0f, y = 0.0f;
  Button up, down, left, right, a, b, c;
};

class Controls {
    ControlStatus m_stats;
public:
    const ControlStatus getStatus(bool normalize_dir = false);
};

#endif
