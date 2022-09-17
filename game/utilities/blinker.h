#ifndef BLINKER_H
#define BLINKER_H

class Blinker {
    float counter = 0.0f;
    float total, on_length;
public:
    Blinker(float tot, float on);
    void update();
    bool active(float offset = 0) const;
};

#endif // BLINKER_H
