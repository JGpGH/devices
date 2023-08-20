#include "config.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(BLINK_SPEED);
    digitalWrite(LED_BUILTIN, LOW);
    delay(BLINK_SPEED);
}
