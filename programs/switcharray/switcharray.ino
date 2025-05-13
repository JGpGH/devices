#include "config.h"

void setup() {
    Serial.begin(BAUDRATE);
    for(int i = 0; i < sizeof(SWITCH_PINS); i++) {
        pinMode(SWITCH_PINS[i], OUTPUT); 
    }
}
unsigned char buffer[5];
unsigned char ptr = 0;
unsigned char curr = 0;

void interpretBuffer() {
  if(buffer[0] == 'r') {
    unsigned char pin = (buffer[1] - 0x30) * 10 + (buffer[2] - 0x30);
    unsigned char state = buffer[3] == 'H' || buffer[3] == 'h' ? HIGH : LOW;
    for(int i = 0; i < sizeof(SWITCH_PINS); i++) {
      if(SWITCH_PINS[i] == pin) {
        digitalWrite(pin, state);
        readConfirm();
        return;
      }
    }
  }
  readError();
}

void readError() {
  Serial.write('N');
}

void readConfirm() {
  Serial.write('Y');
}

void loop() {
  if (Serial.available() > 0) {
    curr = Serial.read();
    Serial.write(curr);
    if(curr == 'X') {
      ptr = 0;
    } else if(ptr > 4) {
      readError();
    } else {
      buffer[ptr] = curr;
      ptr++;
    }
    if(ptr == 4) {
      interpretBuffer();
      ptr = 0;
    }
  }
  delay(50); 
} 