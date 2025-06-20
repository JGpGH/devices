#ifndef SERIALRPC_CLIENT_HPP
#define SERIALRPC_CLIENT_HPP

#include "serial.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>

struct SerialRpcClient {
    SerialClient serial;
    SerialRpcClient(const std::string& device, int baudrate = B115200)
        : serial(device, baudrate) {}

    // Call a remote procedure
    // args: argument bytes (encoded as per contract)
    // Returns: response bytes (raw), meta byte, proc_idx
    std::vector<uint8_t> call(uint8_t proc_idx, const std::vector<uint8_t>& args,
                             uint8_t& out_meta, uint8_t& out_proc_idx) {
        // Build payload: meta, proc_idx, [args...]
        bool has_data = !args.empty();
        uint8_t meta = 0x01; // is_call=1
        if (has_data) meta |= 0x04; // has_data=1
        std::vector<uint8_t> payload = {meta, proc_idx};
        if (has_data) payload.insert(payload.end(), args.begin(), args.end());
        serial.write_bytes(payload);
        // Read response: meta, proc_idx, [data...]
        auto resp_header = serial.read_bytes(2);
        if (resp_header.size() < 2) throw std::runtime_error("No response from device");
        out_meta = resp_header[0];
        out_proc_idx = resp_header[1];
        bool resp_has_data = (out_meta & 0x04) != 0;
        std::vector<uint8_t> resp_data;
        if (resp_has_data) {
            // For most types, read up to 8 bytes (max for u64); user must decode as needed
            resp_data = serial.read_bytes(8);
        }
        return resp_data;
    }
};

#endif // SERIALRPC_CLIENT_HPP
