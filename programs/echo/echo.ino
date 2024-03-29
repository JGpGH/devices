#include "config.h"

void setup() {
    Serial.begin(BAUDRATE);
    Serial.println("Echo program started");
}

void loop() {
    if (Serial.available()) {
        Serial.write(Serial.read());
    }
    delay(10);
}