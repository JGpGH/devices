#ifndef SERIALRPC_HPP
#define SERIALRPC_HPP

#ifdef UNIT_TEST
#include "serial_mock.hpp"
#endif
#include "codec.hpp"
#include <string.h>

// Error code constants
#define SERIALRPC_ERR_NOTFOUND 0
#define SERIALRPC_ERR_BADDATA 1
#define SERIALRPC_ERR_PROCERR 2
#define SERIALRPC_ERR_BADGATEWAY 3
#define SERIALRPC_ERR_CORRUPT 4

// Buffer size constants
#define SERIALRPC_MAX_DATA_LEN 256
#define SERIALRPC_MAX_RESP_LEN 256

// Procedure function pointer type
// Arguments: data pointer, data length, response buffer, response buffer size
// Returns error code (0=OK, 1=BadData, ...)
typedef int (*SerialRpcProcedure)(const uint8_t* data, uint16_t data_len, uint8_t* resp_buf, uint16_t resp_buf_size, uint16_t* resp_len);

struct ProcedureSet {
    SerialRpcProcedure* procedures; // Array of procedure pointers
    uint16_t count; // Number of procedures in the set
};

struct SerialRpc {
    ProcedureSet procedure_set;
};

// Helper: respond with error
static void serialrpc_respond_error(uint8_t proc_idx, uint8_t error_code) {
    uint8_t resp[3];
    MetaBlock mb = {0};
    mb.is_call = false;
    mb.has_error = true;
    mb.has_data = false;
    mb.parity = false;
    mb.error_code = error_code;
    resp[0] = metabyte_from_block(&mb);
    resp[1] = proc_idx;
    resp[2] = 0; // No data
    Serial.write(resp, 2); // Only meta and proc idx
}

// Main tick function: parse payload, call procedure, respond
void serial_rpc_tick(SerialRpc* serial_rpc) {
    if (!Serial.available()) return;
    // Read meta byte
    uint8_t meta = 0;
    Serial.readBytes(&meta, 1);
    MetaBlock mb;
    metabyte_to_block(meta, &mb);
    // Read procedure index
    uint8_t proc_idx = 0;
    if (Serial.readBytes(&proc_idx, 1) != 1) return;
    if (mb.is_call == false) {
        // If not a call, respond with error
        serialrpc_respond_error(proc_idx, SERIALRPC_ERR_BADGATEWAY);
        return;
    }

    uint8_t data[SERIALRPC_MAX_DATA_LEN];
    memset(data, 0, SERIALRPC_MAX_DATA_LEN); // Clear data buffer
    uint8_t len_bytes[2];
    memset(len_bytes, 0, sizeof(len_bytes)); // Clear length bytes
    uint16_t data_len = 0; // Ensure data_len is always defined
    if (mb.has_data) {
        // Read data length (u16 LE)
        if (Serial.readBytes(len_bytes, 2) != 2) return;
        data_len = len_bytes[0] | (len_bytes[1] << 8);
        if (data_len > SERIALRPC_MAX_DATA_LEN) {
            serialrpc_respond_error(proc_idx, SERIALRPC_ERR_BADDATA);
            return;
        }
        if (Serial.readBytes(data, data_len) != data_len) {
            serialrpc_respond_error(proc_idx, SERIALRPC_ERR_BADDATA);
            return;
        }
    }

    // Call procedure if valid
    if (proc_idx >= serial_rpc->procedure_set.count) {
        serialrpc_respond_error(proc_idx, SERIALRPC_ERR_NOTFOUND);
        return;
    }
    uint8_t resp_buf[SERIALRPC_MAX_RESP_LEN];
    memset(resp_buf, 0, SERIALRPC_MAX_RESP_LEN); // Clear response buffer
    uint16_t resp_len = 0; // Will be set by procedure if needed
    int err = serial_rpc->procedure_set.procedures[proc_idx](data, data_len, resp_buf, sizeof(resp_buf), &resp_len);
    // Respond
    MetaBlock resp_mb = {0};
    resp_mb.is_call = false;
    resp_mb.has_error = (err != 0);
    resp_mb.has_data = (resp_len > 0);
    resp_mb.parity = false;
    resp_mb.error_code = (err != 0) ? err : 0;
    uint8_t header[4];
    header[0] = metabyte_from_block(&resp_mb);
    header[1] = proc_idx;
    header[2] = resp_len & 0xFF;
    header[3] = (resp_len >> 8) & 0xFF;
    Serial.write(header, 4);
    if (resp_len > 0) {
        Serial.write(resp_buf, resp_len); // Write response data from start of buffer
    }
}

#endif // SERIALRPC_HPP