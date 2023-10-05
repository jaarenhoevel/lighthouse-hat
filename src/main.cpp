#include <PixelMatrix.h>
#include <RotatingBeamEffect.cpp>

PixelMatrix *pm;

void setup() {
    Serial.begin(921600);
    delay(250);
    
    pm = new PixelMatrix();

    RotatingBeamEffect *rbe = new RotatingBeamEffect(pm);
    rbe->init(2000, CRGB::Blue, CRGB::Green);
}

void loop() {
    delay(25);
    pm->update();
}