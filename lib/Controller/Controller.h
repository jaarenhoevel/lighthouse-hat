#include <Arduino.h>
#include <espnow.h>

#include <PixelMatrix.h>
#include <Effect.h>

#include <BlinderEffect.cpp>
#include <RainbowEffect.cpp>
#include <RotatingBeamEffect.cpp>
#include <StrobeEffect.cpp>
#include <StaticFilterEffect.cpp>

#define FRAME_INTERVAL  25
#define EFFECT_INTERVAL 60000
#define BEACON_INTERVAL 1000

#define MAGIC           0xBEAC07

class Controller {
    public:
        Controller();

        enum Effects {
            BLINDER,
            RAINBOW,
            ROTATING_BEAM,
            STROBE,
            STATIC_FILTER,
            EFFECT_MAX
        };

        void update();

        struct Beacon {
            uint32_t magic;
            uint32_t millis;
            uint16_t effect;
            uint32_t param1;
            uint32_t param2;
            uint32_t param3;
            uint32_t param4;
        };

        void handleBeacon(uint8_t * mac, uint8_t *data, int len);

    private:
        PixelMatrix *pm;

        BlinderEffect *blinderEffect;
        RainbowEffect *rainbowEffect;
        RotatingBeamEffect *rotatingBeamEffect;
        StrobeEffect *strobeEffect;
        StaticFilterEffect *staticFilterEffect;

        Effect *currentEffect = NULL;
        uint16_t currentEffectId = 0;
        uint16_t iterations = 0;

        uint32_t effectParams[4] = {0, 0, 0, 0};

        bool isMaster = true;

        uint32_t lastFrame, lastEffectChange, lastBeaconReceived, lastBeaconSent = 0;
        int32_t millisOffset = 0;

        uint8_t effectIndex = STATIC_FILTER;

        uint32_t getMillis();
        void setEffect(Effect *effect, uint32_t param1 = 0, uint32_t param2 = 0, uint32_t param3 = 0, uint32_t param4 = 0);
        void nextEffect();

        Effect* getEffectById(uint16_t id);

        void broadcastStatus();
};