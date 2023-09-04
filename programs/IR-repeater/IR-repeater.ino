#include <IRremote.hpp>
#include "config.h"
#define DECODE_ONKYO

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
}

void broadcast(decode_type_t p, uint32_t address, uint8_t command) {
    retransmit(IR_SEND_PIN, p, address, command);
}

void retransmit(uint_fast8_t pin, decode_type_t p, uint32_t address, uint8_t command) {
    switch(p) {
        case NEC:
            IrSender.setSendPin(pin);
            IrSender.sendNEC(address, command, 10);
            break;
    }
}

void loop() {
    if (IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);
        broadcast(IrReceiver.decodedIRData.protocol, IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
        IrReceiver.resume(); // Enable receiving of the next value
    }
}