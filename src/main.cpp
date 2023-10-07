#include <ESP8266WiFi.h>
#include <espnow.h>

#include <Controller.h>

uint8_t broadcast_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

Controller *controller;

void recv(uint8_t *mac, uint8_t *data, uint8_t len) {
    controller->handleBeacon(mac, data, len);
}

void setup() {
    Serial.begin(921600);
    delay(500);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        while (1);
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_add_peer(broadcast_address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    
    controller = new Controller();

    esp_now_register_recv_cb(recv);
}

void loop() {
    controller->update();
    //delay(10);
}
