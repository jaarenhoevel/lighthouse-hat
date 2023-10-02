#include <PixelMatrix.h>

PixelMatrix* pm;

void setup() {
    Serial.begin(921600);
    delay(250);
    
    pm = new PixelMatrix();

    fill_gradient_RGB(pm->pixels, PIXEL_COUNT, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black);
    fill_rainbow_circular(pm->filter, PIXEL_COUNT, 0);

    pm->setRotation(1500);
}

void loop() {
    delay(25);
    pm->update();
}