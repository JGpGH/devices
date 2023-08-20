#include <IRremote.hpp>
#define DECODE_ONKYO
const uint_fast8_t IR_RECEIVE_PIN = 2;

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
    Serial.println("boot");
}

void loop() {
    if (IrReceiver.decode()) {
        if(IrReceiver.decodedIRData.protocol == NEC) {
            IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
        }
        if (IrReceiver.decodedIRData.address == 0x0 && IrReceiver.decodedIRData.command == 0x58) {
            delay(1000);
            Serial.write("secret code detected");
        }
        IrReceiver.resume(); // Enable receiving of the next value
    }
}