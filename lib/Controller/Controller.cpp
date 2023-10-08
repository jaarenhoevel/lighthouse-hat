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
    if (!this->isMaster && millis() - this->lastBeaconReceived > BEACON_INTERVAL * 10) this->isMaster = true; // No beacon received for a long time -> we are master now :))
    if (this->isMaster && millis() - this->lastBeaconSent > BEACON_INTERVAL) this->broadcastStatus();

    if (this->getMillis() - this->lastFrame < FRAME_INTERVAL) return;

    // Serial.printf("\nFrame");

    this->lastFrame = this->getMillis();

    if (this->isMaster && this->getMillis() - this->lastEffectChange > EFFECT_INTERVAL) {
        this->nextEffect();
        this->lastEffectChange = this->getMillis();
    }

    if (this->currentEffect != NULL && !this->currentEffect->update(this->getMillis()) && this->isMaster) this->nextEffect(); // Update effect when set and switch to next effect when effect returns false
    this->pm->update();
}

void Controller::setEffect(Effect *effect, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) {
    effect->init(this->isMaster, param1, param2, param3, param4);
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
        return NULL;
    }   
}

void Controller::nextEffect() {
    Serial.printf("\nNext effect %u", this->effectIndex);
    this->currentEffectId = this->effectIndex; // update current effect index
    
    switch (this->effectIndex) {
    case Controller::Effects::BLINDER:
        this->setEffect(this->blinderEffect, 750);
        break;

    case Controller::Effects::RAINBOW:
        this->setEffect(this->rainbowEffect, 5000);
        break;

    case Controller::Effects::ROTATING_BEAM: {       
        uint8_t hue = random8(255);
        CRGB color = CHSV(hue, 255, 255);
        CRGB color2 = CHSV(hue + 32, 255, 255);

        uint32_t param2 = 0;
        uint32_t param3 = 0;
        memcpy(&param2, &color, sizeof(CRGB));
        memcpy(&param3, &color2, sizeof(CRGB));
        
        this->setEffect(this->rotatingBeamEffect, 2000, param2, param3);
        break;
    }

    case Controller::Effects::STROBE:
        this->setEffect(this->strobeEffect, 2500, 4, CRGB::White, 5);
        break;
    
    default:
        this->setEffect(this->getEffectById(this->effectIndex));
        break;
    }

    this->effectIndex ++;
    if (this->effectIndex == Controller::Effects::EFFECT_MAX) this->effectIndex = 0;
}

uint32_t Controller::getMillis() {
    return millis() + this->millisOffset;
}

void Controller::broadcastStatus() {
    Beacon status;

    status.magic = MAGIC;
    status.millis = millis();
    status.effect = this->currentEffectId;
    status.param1 = this->effectParams[0];
    status.param2 = this->effectParams[1];
    status.param3 = this->effectParams[2];
    status.param4 = this->effectParams[3];

    uint8_t addr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    esp_now_send(addr, (uint8_t *) &status, sizeof(status));

    // Serial.printf("\nSent beacon");
    this->lastBeaconSent = millis();
}

void Controller::handleBeacon(uint8_t *mac, uint8_t *data, int len) {
    // Serial.printf("\nReceived something... let's see");
    
    if (len != sizeof(Beacon)) {
        Serial.printf("\nWrong len!");
        return;   
    }
    
    Beacon status;

    memcpy(&status, data, len);

    if (status.magic != MAGIC) {
        Serial.printf("\nMagic bytes did not match!");
        return;
    }

    this->lastBeaconReceived = millis();

    if (status.millis < millis()) {
        this->isMaster = true;
        this->millisOffset = 0;
        this->pm->setMillisOffset(this->millisOffset);
        this->pm->setRotationDirection(true);

        // Serial.printf("\nI am the master :)");
        return;
    }

    this->isMaster = false;
    this->millisOffset = status.millis - millis();
    this->pm->setMillisOffset(this->millisOffset);
    this->pm->setRotationDirection(false); // Set default rotation direction to ccw

    // Serial.printf("\nReceived master millis :( adjusting offset");

    // Serial.printf("\n\n\nBEACON DEBUG: millis %u, effect %u, param1 %u, param2 %u, param3 %u, param4 %u", 
    //     status.millis, status.effect, status.param1, status.param2, status.param3, status.param4);

    // Check if effect has changed...
    if (status.effect == this->currentEffectId && 
        status.param1 == this->effectParams[0] && 
        status.param2 == this->effectParams[1] && 
        status.param3 == this->effectParams[2] && 
        status.param4 == this->effectParams[3]) return;

    Serial.printf("\nReceived new effect %u!", status.effect);

    this->setEffect(this->getEffectById(status.effect), status.param1, status.param2, status.param3, status.param4);
    this->currentEffectId = status.effect;
}