#pragma once

#include <FastLED.h>

#define LED_COUNT   22
#define LED_TURNS   3.25
#define INIT_TURN   0.95

#define LED_TYPE    WS2812B
#define LED_COLOR   GRB
#define LED_PIN     D3

#define PIXEL_COUNT 100

class PixelMatrix {
    public:
        PixelMatrix();

        void update();

        CRGB* pixels;
        CRGB* filter;

        void setRotation(uint16_t speed = 0, bool enabled = true);

    private:
        CRGB* leds;

        float rotation = 0.f;
        bool rotationEnabled = false;
        uint16_t rotationSpeed = 2000; // ms per rotation
};