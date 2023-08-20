#include <IRremote.hpp>
#include "config.h"
#define DECODE_ONKYO
const uint_fast8_t IR_RECEIVE_PIN = 2;

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
    Serial.println("boot");
}

void broadcast(decode_type_t p, uint32_t address, uint8_t command) {
    for (uint_fast8_t i = 0; i < (sizeof(IR_SEND_PINS) / sizeof(IR_SEND_PINS[0])); i++) {
        retransmit(IR_SEND_PINS[i], p, address, command);
    }
}

void retransmit(uint_fast8_t pin, decode_type_t p, uint32_t address, uint8_t command) {
    switch(p) {
        case NEC:
            IrSender.setSendPin(pin);
            IrSender.sendNEC(address, command, 1);
            break;
    }
}

void loop() {
    if (IrReceiver.decode()) {
        broadcast(IrReceiver.decodedIRData.protocol, IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
        IrReceiver.resume(); // Enable receiving of the next value
    }
}