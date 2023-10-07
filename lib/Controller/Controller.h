#include <Arduino.h>

#include <PixelMatrix.h>
#include <Effect.h>

#include <BlinderEffect.cpp>
#include <RainbowEffect.cpp>
#include <RotatingBeamEffect.cpp>
#include <StrobeEffect.cpp>

#define FRAME_INTERVAL  25
#define EFFECT_INTERVAL 10000

#define MAGIC           0xBEAC07

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

        typedef struct beacon {
            uint32_t magic;
            uint32_t millis;
            uint16_t effect;
            uint32_t param1;
            uint32_t param2;
            uint32_t param3;
            uint32_t param4;
        };

    private:
        PixelMatrix *pm;

        BlinderEffect *blinderEffect;
        RainbowEffect *rainbowEffect;
        RotatingBeamEffect *rotatingBeamEffect;
        StrobeEffect *strobeEffect;

        Effect *currentEffect = NULL;

        uint32_t effectParams = [0, 0, 0, 0];

        bool isMaster = true;

        uint32_t lastFrame, lastEffectChange = 0;
        int32_t millisOffset = 0;

        uint8_t effectIndex = 0;

        uint32_t getMillis();
        void setEffect(Effect *effect, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0);
        void nextEffect();

        Effect* getEffectById(uint8_t id);

        void broadcastStatus();
        void handleBeacon(const uint8_t * mac, const uint8_t *data, int len);
};