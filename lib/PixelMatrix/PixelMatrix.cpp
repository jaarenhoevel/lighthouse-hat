#include <PixelMatrix.h>

PixelMatrix::PixelMatrix() {
    this->leds = new CRGB[LED_COUNT];
    this->pixels = new CRGB[PIXEL_COUNT];
    this->filter = new CRGB[PIXEL_COUNT];
    
    FastLED.addLeds<LED_TYPE, LED_PIN, LED_COLOR>(this->leds, LED_COUNT).setCorrection(TypicalPixelString);
    FastLED.setBrightness(255);

    fill_solid(this->pixels, PIXEL_COUNT, CRGB::Black);
    fill_solid(this->filter, PIXEL_COUNT, CRGB::White);

    this->update();
}

void PixelMatrix::setMillisOffset(uint32_t offset) {
    this->millisOffset = offset;
}

void PixelMatrix::update() {
    if (this->rotationEnabled) {
        this->rotation = ((millis() + this->millisOffset) % this->rotationSpeed) / (1.f * this->rotationSpeed);
    } else {
        this->rotation = 0.f;
    }
    
    // fill_rainbow_circular(this->pixels, PIXEL_COUNT, 0);
    // fill_gradient_RGB(this->pixels, PIXEL_COUNT, CRGB::Black, CRGB(255, 255, 150), CRGB::Black, CRGB::Black);
    
    for (uint8_t i = 0; i < LED_COUNT; i ++) {
        // Serial.printf("\nCalculating position for led %u", i);

        float pos = ((i / ((LED_COUNT - 1) * 1.f)) * LED_TURNS) + INIT_TURN;
        float posRotated = pos + this->rotation;
        while (pos >= 1) pos -= 1;
        while (posRotated >= 1) posRotated -= 1;

        uint8_t mappedPixel = pos * (PIXEL_COUNT - 1);
        uint8_t mappedPixelRotated = posRotated * (PIXEL_COUNT - 1);

        // Serial.printf(" %f -> mapped to pixel no %u", pos, mappedPixel);

        CRGB color[1] = { this->pixels[mappedPixelRotated] };
        fadeUsingColor(color, 1, this->filter[mappedPixel]);

        this->leds[i] = color[0];
    }
    
    FastLED.show();
}

void PixelMatrix::setRotation(uint16_t speed, bool enabled) {
    this->rotationEnabled = enabled;
    if (speed != 0) {
        this->rotationSpeed = speed;
    }
}