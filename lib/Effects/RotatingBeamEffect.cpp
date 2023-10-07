#include <Effect.h>

class RotatingBeamEffect: public Effect {
    using Effect::Effect;
    
    /**
     * param1: rotation speed,
     * param2: beamcolor
     * param3: beamcolor 2
    */
    public: void init(uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
        Effect::init(param1, param2, param3, param4);

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
        
        fill_gradient_RGB(pm->pixels, PIXEL_COUNT, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black); // Beam pattern
        fill_gradient_RGB(pm->filter, PIXEL_COUNT / 2, colorA, colorB); // Filter from color a to b
        fill_gradient_RGB(&pm->filter[PIXEL_COUNT / 2], PIXEL_COUNT / 2, colorB, colorA); // Filter back from color a to color b

        pm->setRotation(param1, param1 != 0);    
    };
};