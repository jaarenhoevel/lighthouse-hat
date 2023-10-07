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

    this->effectParams[0] = param1;
    this->effectParams[1] = param2;
    this->effectParams[2] = param3;
    this->effectParams[3] = param4;

    if (this->isMaster) this->broadcastStatus();
}

Effect* Controller::getEffectById(uint16_t id) {
    switch (id) {
    case Controller::Effects::BLINDER:
        return this->blinderEffect;

    case Controller::Effects::RAINBOW:
        return this->rainbowEffect;

    case Controller::Effects::ROTATING_BEAM:
        return this->rotatingBeamEffect;

    case Controller::Effects::STROBE:
        return this->strobeEffect;
    
    default:
        break;
    }   
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
        this->setEffect(this->getEffectById(this->effectIndex));
        break;
    }

    Serial.printf(" ... done");

    this->effectIndex ++;
    if (this->effectIndex == Controller::Effects::EFFECT_MAX) this->effectIndex = 0;
}

uint32_t Controller::getMillis() {
    return millis() + this->millisOffset;
}

void Controller::broadcastStatus() {
    beacon status;

    status.magic = MAGIC;
    status.millis = millis();
    status.effect = this->effectIndex;
    status.param1 = this->effectParams[0];
    status.param2 = this->effectParams[1];
    status.param3 = this->effectParams[2];
    status.param4 = this->effectParams[3];
}

void Controller::handleBeacon(const uint8_t * mac, const uint8_t *data, int len) {
    beacon* status =(beacon*) data;

    if (status->magic != MAGIC) {
        Serial.printf("Magic bytes did not match!");
        return;
    }

    if (status->millis < millis()) {
        this->isMaster = true;
        this->millisOffset = 0;

        Serial.printf("I am the master :)");
        return;
    }

    this->isMaster = false;
    this->millisOffset = status->millis - millis();
    Serial.printf("Received master millis :( adjusting offset");

    this->setEffect(this->getEffectById(status->effect), status->param1, status->param2, status->param3, status->param4);
}