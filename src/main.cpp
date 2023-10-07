#include <Controller.h>

Controller *controller;

void setup() {
    Serial.begin(921600);
    delay(500);
    
    controller = new Controller();
}

void loop() {
    controller->update();
    //delay(10);
}