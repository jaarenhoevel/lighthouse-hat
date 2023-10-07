#include <Arduino.h>

#include <PixelMatrix.h>
#include <Effect.h>

#include <BlinderEffect.cpp>
#include <RainbowEffect.cpp>
#include <RotatingBeamEffect.cpp>
#include <StrobeEffect.cpp>

#define FRAME_INTERVAL  25
#define EFFECT_INTERVAL 10000

class Controller {
    public:
        Controller();

        enum Effects {
            BLINDER,
            RAINBOW,
            ROTATING_BEAM,
            STROBE,
            EFFECT_MAX
        };

        void update();

    private:
        PixelMatrix *pm;

        BlinderEffect *blinderEffect;
        RainbowEffect *rainbowEffect;
        RotatingBeamEffect *rotatingBeamEffect;
        StrobeEffect *strobeEffect;

        Effect *currentEffect = NULL;

        bool isMaster = true;

        uint32_t lastFrame, lastEffectChange = 0;
        int32_t millisOffset = 0;

        uint8_t effectIndex = 0;

        uint32_t getMillis();
        void setEffect(Effect *effect, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0);
        void nextEffect();
};