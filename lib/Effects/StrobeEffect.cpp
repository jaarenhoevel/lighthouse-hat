#include <Effect.h>

class StrobeEffect: public Effect {
    using Effect::Effect;
    
    /**
     * param1: interval
     * param2: flash count
     * param3: color
     * param4: total amount of repeats before exiting
    */
    public: 
        void init(bool isMaster, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0) {        
            Effect::init(isMaster, param1, param2, param3, param4);
            this->last = 0;
            this->on = false;
            this->flashes = 0; 
            this->resets = 0;

            this->param2 = param2 * 2;
            
            this->color = CRGB::White;
            if (param3 != 0) {
                this->color = param3;
            }

            fill_solid(pm->filter, PIXEL_COUNT, CRGB::White);

            pm->setRotation(0, false);   
        };

        bool update(uint32_t millis) {
            if (millis - (millis % this->param1) != this->last) {
                fill_solid(pm->pixels, PIXEL_COUNT, this->on ? CRGB::Black : this->color);
                this->on = !this->on;

                this->flashes ++;
                
                if (this->flashes == this->param2) {
                    this->last = millis - (millis % this->param1);
                    this->flashes = 0;
                    this->resets ++;
                    if (this->param4 != 0 && this->param4 == this->resets) return false;
                }
            }

            return true;
        }

    private:
        uint32_t last = 0;
        bool on = false;
        uint8_t flashes = 0;
        uint8_t resets = 0;
        CRGB color;
};