#pragma once

#include <PixelMatrix.h>
#include <FastLED.h>

class Effect {
    public:
        Effect(PixelMatrix *pm);
        
        virtual void init(uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0);
        virtual void update(uint32_t millis);
    
    protected:
        PixelMatrix *pm;
        uint32_t param1, param2, param3, param4;
};