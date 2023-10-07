#include <Controller.h>

Controller::Controller() {
    this->pm = new PixelMatrix();

    this->blinderEffect = new BlinderEffect(pm);
    this->rainbowEffect = new RainbowEffect(pm);
    this->rotatingBeamEffect = new RotatingBeamEffect(pm);
    this->strobeEffect = new StrobeEffect(pm);

    Serial.printf("\nInit complete");

    this->nextEffect();
}

void Controller::update() {
    if (this->getMillis() - this->lastFrame < FRAME_INTERVAL) return;

    // Serial.printf("\nFrame");

    this->lastFrame = this->getMillis();

    if (this->isMaster && this->getMillis() - this->lastEffectChange > EFFECT_INTERVAL) {
        this->nextEffect();
        this->lastEffectChange = this->getMillis();
    }

    if (this->currentEffect != NULL) this->currentEffect->update(this->getMillis());
    this->pm->update();
}

void Controller::setEffect(Effect *effect, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) {
    effect->init(param1, param2, param3, param4);
    this->currentEffect = effect;
}

void Controller::nextEffect() {
    Serial.printf("\nNext effect %u", this->effectIndex);
    
    switch (this->effectIndex) {
    case Controller::Effects::BLINDER:
        this->setEffect(this->blinderEffect, 1000);
        break;

    case Controller::Effects::RAINBOW:
        this->setEffect(this->rainbowEffect, 5000);
        break;

    case Controller::Effects::ROTATING_BEAM:
        this->setEffect(this->rotatingBeamEffect, 2000, CRGB::Red, CRGB::Orange);
        break;

    case Controller::Effects::STROBE:
        this->setEffect(this->strobeEffect, 2500, 4);
        break;
    
    default:
        break;
    }

    Serial.printf(" ... done");

    this->effectIndex ++;
    if (this->effectIndex == Controller::Effects::EFFECT_MAX) this->effectIndex = 0;
}

uint32_t Controller::getMillis() {
    return millis() + this->millisOffset;
}