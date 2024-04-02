#include "procedure.hpp"

struct SerialRpc {
    ProcedureSet procedure_set;
    Buffer input_buffer;
};

void readBuffer(Buffer* buffer, bool* success) {
    buffer->reset();
    uint8_t lenBuffer[4];
    for (int i = 0; i < 4; i++) {
        lenBuffer[i] = Serial.read();
    }
    buffer->append(lenBuffer, 4);
    uint32_t packageLength = buffer->decode_u32(success);
    if (!*success) {
        return;
    }
    for (int i = 0; i < packageLength; i++) {
        buffer->append(Serial.read());
    }
    *success = true;
}

void serial_rpc_tick(SerialRpc* serial_rpc) {
    bool success = false;
    if(Serial.available()) {
        readBuffer(&serial_rpc->input_buffer, &success);
        if (!success) {
            return;
        }
        Serial.println("Test started");
        ProcedureResult result = callProcedure(serial_rpc->procedure_set, serial_rpc->input_buffer);
        Serial.write(result.buffer.getData(), result.buffer.getLen());
    }
}