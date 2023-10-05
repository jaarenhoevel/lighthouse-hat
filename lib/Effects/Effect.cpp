#include <Effect.h>

Effect::Effect(PixelMatrix *pm) {
    this->pm = pm;
}

void Effect::init(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) {
    this->param1 = param1;
    this->param2 = param2;
    this->param3 = param3;
    this->param4 = param4;
}

void Effect::update(uint32_t millis) {
    return;
}