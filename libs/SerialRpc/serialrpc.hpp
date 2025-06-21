#ifndef SERIALRPC_HPP
#define SERIALRPC_HPP

#ifdef UNIT_TEST
#include "serial_mock.hpp"
#endif
#include "codec.hpp"
#include <string.h>

// Error code constants
enum SerialRpcError {
    SERIALRPC_ERR_NOTFOUND = 0,
    SERIALRPC_ERR_BADDATA = 1,
    SERIALRPC_ERR_PROCERR = 2,
    SERIALRPC_ERR_BADGATEWAY = 3,
    SERIALRPC_ERR_CORRUPT = 4
};

// Buffer size constants
constexpr uint16_t SERIALRPC_MAX_DATA_LEN = 256;
constexpr uint16_t SERIALRPC_MAX_RESP_LEN = 256;

// Procedure function pointer type
// Arguments: data pointer, data length, response buffer, response buffer size
// Returns error code (0=OK, 1=BadData, ...)
typedef int (*SerialRpcProcedure)(const uint8_t* data, uint16_t data_len, uint8_t* resp_buf, uint16_t resp_buf_size, uint16_t* resp_len);

class ProcedureSet {
public:
    ProcedureSet(SerialRpcProcedure* procs, uint16_t count) : procedures(procs), count_(count) {}
    uint16_t count() const { return count_; }
    SerialRpcProcedure get(uint16_t idx) const { return procedures[idx]; }
private:
    SerialRpcProcedure* procedures;
    uint16_t count_;
};

class SerialRpc {
public:
    SerialRpc(SerialRpcProcedure* procs, uint16_t count) : procedure_set(procs, count) {}

    void tick() {
        if (!Serial.available()) return;
        // Read meta byte
        uint8_t meta = 0;
        Serial.readBytes(&meta, 1);
        MetaBlock mb;
        metabyte_to_block(meta, &mb);
        // Read procedure index
        uint8_t proc_idx = 0;
        if (Serial.readBytes(&proc_idx, 1) != 1) return;
        if (!mb.is_call) {
            respond_error(proc_idx, SERIALRPC_ERR_BADGATEWAY);
            return;
        }
        uint8_t data[SERIALRPC_MAX_DATA_LEN] = {0};
        uint16_t data_len = 0;
        if (mb.has_data) {
            uint8_t len_bytes[2] = {0, 0};
            if (Serial.readBytes(len_bytes, 2) != 2) return;
            data_len = len_bytes[0] | (len_bytes[1] << 8);
            if (data_len > SERIALRPC_MAX_DATA_LEN) {
                respond_error(proc_idx, SERIALRPC_ERR_BADDATA);
                return;
            }
            if (Serial.readBytes(data, data_len) != data_len) {
                respond_error(proc_idx, SERIALRPC_ERR_BADDATA);
                return;
            }
        }
        if (proc_idx >= procedure_set.count()) {
            respond_error(proc_idx, SERIALRPC_ERR_NOTFOUND);
            return;
        }
        uint8_t resp_buf[SERIALRPC_MAX_RESP_LEN] = {0};
        uint16_t resp_len = 0;
        int err = procedure_set.get(proc_idx)(data, data_len, resp_buf, SERIALRPC_MAX_RESP_LEN, &resp_len);
        respond(proc_idx, err, resp_buf, resp_len);
    }

private:
    ProcedureSet procedure_set;

    void respond_error(uint8_t proc_idx, uint8_t error_code) {
        uint8_t resp[2];
        MetaBlock mb = {0};
        mb.is_call = false;
        mb.has_error = true;
        mb.has_data = false;
        mb.parity = false;
        mb.error_code = error_code;
        resp[0] = metabyte_from_block(&mb);
        resp[1] = proc_idx;
        Serial.write(resp, 2);
    }

    void respond(uint8_t proc_idx, int err, const uint8_t* resp_buf, uint16_t resp_len) {
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
            Serial.write(resp_buf, resp_len);
        }
    }
};

#endif // SERIALRPC_HPP