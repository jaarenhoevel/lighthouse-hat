#include <Effect.h>

class StaticFilterEffect: public Effect {
    using Effect::Effect;
    
    /**
     * param1: rotation speed,
     * param2: color a
     * param3: color b
    */
    public: void init(bool isMaster, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
        Effect::init(isMaster, param1, param2, param3, param4);

        CRGB colorA = CRGB::White;
        if (param2 != 0) {
            colorA = param2;
        }

        CRGB colorB;
        if (param3 != 0) {
            colorB = param3;
        } else {
            memcpy(&colorB, &colorA, sizeof(CRGB)); // use color a if not defined
        }
        
        fill_gradient_RGB(this->pm->pixels, PIXEL_COUNT, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black); // Beam pattern
        fill_solid(this->pm->filter, PIXEL_COUNT / 2, colorA);
        fill_solid(&this->pm->filter[PIXEL_COUNT / 2], PIXEL_COUNT / 2, colorB);

        pm->setRotation(param1, param1 != 0);    
    };
};