#ifndef SERIALRPC_CLIENT_HPP
#define SERIALRPC_CLIENT_HPP

#include "../libs/SerialRpc/codec.hpp"
#include "serial.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <chrono>
#include <thread>

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
        MetaBlock mb = {};
        mb.is_call = true;
        mb.has_error = false;
        mb.has_data = !args.empty();
        mb.parity = false;
        mb.error_code = 0;
        uint8_t meta = metabyte_from_block(&mb);
        std::vector<uint8_t> payload = {meta, proc_idx};
        if (mb.has_data) {
            uint16_t len = static_cast<uint16_t>(args.size());
            SerialRpcClient::write(payload, len);
            payload.insert(payload.end(), args.begin(), args.end());
        }
        serial.write_bytes(payload);

        // Wait up to 10 seconds for response
        auto start = std::chrono::steady_clock::now();
        std::vector<uint8_t> resp_header;
        while (true) {
            resp_header = serial.read_bytes(2);
            if (resp_header.size() == 2) break;
            if (std::chrono::steady_clock::now() - start > std::chrono::seconds(10))
                throw std::runtime_error("No response from device (timeout)");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        out_meta = resp_header[0];
        out_proc_idx = resp_header[1];
        bool resp_has_data = (out_meta & 0x04) != 0;
        std::vector<uint8_t> resp_data;
        if (resp_has_data) {
            std::vector<uint8_t> len_bytes;
            start = std::chrono::steady_clock::now();
            while (true) {
                len_bytes = serial.read_bytes(2);
                if (len_bytes.size() == 2) break;
                if (std::chrono::steady_clock::now() - start > std::chrono::seconds(10))
                    throw std::runtime_error("Failed to read response length (timeout)");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            uint16_t data_len = 0;
            int readPtr = 0;
            SerialRpcClient::read(len_bytes, readPtr, &data_len);
            if (data_len > 0) {
                start = std::chrono::steady_clock::now();
                while (true) {
                    resp_data = serial.read_bytes(data_len);
                    if (resp_data.size() == data_len) break;
                    if (std::chrono::steady_clock::now() - start > std::chrono::seconds(10))
                        throw std::runtime_error("Incomplete response data (timeout)");
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }
        return resp_data;
    }

    // --- Simple write helpers ---
    static void write(std::vector<uint8_t>& buf, uint8_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 1);
        bv_write_u8(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, uint16_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 2);
        bv_write_u16(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, uint32_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 4);
        bv_write_u32(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, uint64_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 8);
        bv_write_u64(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, int8_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 1);
        bv_write_i8(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, int16_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 2);
        bv_write_i16(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, int32_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 4);
        bv_write_i32(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, int64_t val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 8);
        bv_write_i64(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, float val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 4);
        bv_write_f32(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, double val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 8);
        bv_write_f64(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, bool val) {
        uint16_t pos = buf.size();
        buf.resize(pos + 1);
        bv_write_bool(buf.data(), buf.size(), &pos, val);
    }
    static void write(std::vector<uint8_t>& buf, const std::string& str) {
        uint16_t pos = buf.size();
        buf.resize(pos + 4 + str.size());
        bv_write_string(buf.data(), buf.size(), &pos, str.data(), str.size());
    }
    static void write(std::vector<uint8_t>& buf, const std::vector<uint8_t>& arr) {
        uint16_t pos = buf.size();
        buf.resize(pos + 4 + arr.size());
        bv_write_u32(buf.data(), buf.size(), &pos, arr.size());
        bv_write_bytes(buf.data(), buf.size(), &pos, arr.data(), arr.size());
    }
    // --- Simple read helpers ---
    static bool read(const std::vector<uint8_t>& buf, int& pos, uint8_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_u8(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, uint16_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_u16(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, uint32_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_u32(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, uint64_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_u64(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, int8_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_i8(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, int16_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_i16(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, int32_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_i32(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, int64_t* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_i64(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, float* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_f32(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, double* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_f64(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, bool* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        bool ok = bv_read_bool(&bv, out);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, std::string* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        RpcString s;
        bool ok = bv_read_string(&bv, &s);
        if (ok) out->assign(s.data, s.len);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
    static bool read(const std::vector<uint8_t>& buf, int& pos, std::vector<uint8_t>* out) {
        BufferView bv = {buf.data(), buf.size(), static_cast<size_t>(pos)};
        ByteArray arr;
        bool ok = bv_read_u8_array(&bv, &arr);
        if (ok) out->assign(arr.data, arr.data + arr.len);
        pos = static_cast<int>(bv.pos);
        return ok;
    }
};

#endif // SERIALRPC_CLIENT_HPP
