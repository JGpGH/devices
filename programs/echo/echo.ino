#include "config.h"

void setup() {
    Serial.begin(BAUDRATE);
}
int ledc = 0;

void loop() {
    if (Serial.available()) {
        Serial.write(Serial.read());
        digitalWrite(LED_BUILTIN, ++ledc % 2 == 0 ? HIGH : LOW);
    }
}