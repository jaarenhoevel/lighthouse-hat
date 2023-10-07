#include <Effect.h>

class StrobeEffect: public Effect {
    using Effect::Effect;
    
    /**
     * param1: interval
     * param2: flash count
     * param3: color
    */
    public: 
        void init(uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
            Effect::init(param1, param2, param3, param4);
            this->last = 0;
            this->on = false;
            this->flashes = 0; 

            this->param2 = param2 * 2;
            
            this->color = CRGB::White;
            if (param3 != 0) {
                this->color = param2;
            }

            fill_solid(pm->filter, PIXEL_COUNT, CRGB::White);

            pm->setRotation(0, false);   
        };

        void update(uint32_t millis) {
            if (millis - this->last > this->param1) {
                fill_solid(pm->pixels, PIXEL_COUNT, this->on ? CRGB::Black : this->color);
                this->on = !this->on;

                this->flashes ++;
                
                if (this->flashes == this->param2) {
                    this->last = millis;
                    this->flashes = 0;
                }
            }
        }

    private:
        uint32_t last = 0;
        bool on = false;
        uint8_t flashes = 0;
        CRGB color;
};