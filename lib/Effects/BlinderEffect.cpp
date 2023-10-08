#include <Effect.h>

class BlinderEffect: public Effect {
    using Effect::Effect;
    
    /**
     * param1: interval
     * param2: color
    */
    public: 
        void init(bool isMaster, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
            Effect::init(isMaster, param1, param2, param3, param4);
            this->last = 0;
            
            this->color = ColorTemperature::Candle;
            if (param2 != 0) {
                this->color = param2;
            }

            fill_solid(pm->filter, PIXEL_COUNT, CRGB::White);

            pm->setRotation(0, false);
        };

        bool update(uint32_t millis) {
            fadeUsingColor(pm->pixels, PIXEL_COUNT, CRGB(220, 200, 200));
            
            if ((millis + this->isMaster * (this->param1 / 2)) % this->param1 < 100) {
                fill_solid(pm->pixels, PIXEL_COUNT, this->color);
                this->last = millis;
            }

            return true;
        }

    private:
        uint32_t last = 0;
        CRGB color;
};