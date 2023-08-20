#include <IRremote.hpp>
#define DECODE_ONKYO
const uint_fast8_t IR_RECEIVE_PIN = 2;
int[] IR_SEND_PINS = {3, 4, 5};

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
    Serial.println("boot");
}

void retransmit(IRData::protocol p, uint32_t address, uint32_t command) {
    switch p {
        case NEC:
            IrReceiver.sendNEC(address, command, 1);
            break;
    }
}

void loop() {
    if (IrReceiver.decode()) {
        retransmit(IrReceiver.decodedIRData.protocol, IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
        IrReceiver.resume(); // Enable receiving of the next value
    }
}