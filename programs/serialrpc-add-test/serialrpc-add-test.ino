#include "config.h"
#include <codec.hpp>
#include <serialrpc.hpp>

int rpc_add(const uint8_t* data, uint16_t data_len, uint8_t* resp_buf, uint16_t resp_buf_size, uint16_t* resp_len) {
    if (data_len != 8) {
        return SERIALRPC_ERR_BADDATA; // Expecting 8 bytes (two uint32_t)
    }
    BufferView bv = { data, data_len, 0 };
    uint32_t a = 0, b = 0;
    if (!bv_read_u32(&bv, &a)) return SERIALRPC_ERR_BADDATA;
    if (!bv_read_u32(&bv, &b)) return SERIALRPC_ERR_BADDATA;
    uint32_t sum = a + b;
    size_t pos = 0;
    if (!bv_write_u32(resp_buf, resp_buf_size, &pos, sum)) return SERIALRPC_ERR_PROCERR;
    *resp_len = pos;
    return 0; // Success
}

SerialRpcProcedure procedures[] = { rpc_add };
SerialRpc serial_rpc(procedures, 1);

void setup() {
    Serial.begin(BAUDRATE);
    pinMode(LED_BUILTIN, OUTPUT);
    #if defined(USBCON)
    while (!Serial) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
    }
    #endif
}

void loop() {
    serial_rpc.tick();
    delay(10); // Prevent flooding the serial port
}