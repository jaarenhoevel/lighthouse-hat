#include <Effect.h>

Effect::Effect(PixelMatrix *pm) {
    this->pm = pm;
}

void Effect::init(bool isMaster, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) {
    this->isMaster = isMaster;
    this->param1 = param1;
    this->param2 = param2;
    this->param3 = param3;
    this->param4 = param4;
}

bool Effect::update(uint32_t millis) {
    return true;
}