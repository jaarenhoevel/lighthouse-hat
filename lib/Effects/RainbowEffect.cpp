#include <Effect.h>

class RainbowEffect: public Effect {
    using Effect::Effect;
    
    public: void init(uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
        Effect::init(param1, param2, param3, param4);
        
        fill_solid(pm->pixels, PIXEL_COUNT, CRGB::White);
        fill_rainbow_circular(pm->filter, PIXEL_COUNT, 0);

        pm->setRotation(param1, param1 != 0);    
    };
};