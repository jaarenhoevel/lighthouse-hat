#include <Effect.h>

class RainbowEffect: public Effect {
    using Effect::Effect;
    
    /**
     * param1: Rotation speed
    */
    public: void init(bool isMaster, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
        Effect::init(isMaster, param1, param2, param3, param4);
        
        fill_rainbow_circular(this->pm->filter, PIXEL_COUNT, 0);
        fill_gradient_RGB(this->pm->pixels, PIXEL_COUNT / 2, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black);
        fill_gradient_RGB(&this->pm->pixels[PIXEL_COUNT / 2], PIXEL_COUNT / 2, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black);

        pm->setRotation(param1, param1 != 0);    
    };
};