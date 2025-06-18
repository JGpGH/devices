#define UNIT_TEST
#include "../libs/SerialRpc/serial_mock.hpp"
#include "../libs/SerialRpc/serialrpc.hpp"
#include "console_util.cpp"

// Provide the global Serial instance for mocking
SerialMock Serial;

// Simple echo procedure: copies input to output, returns 0
int echo_proc(const uint8_t* data, uint16_t data_len, uint8_t* resp_buf, uint16_t resp_buf_size, uint16_t* resp_len) {
    if (data_len > resp_buf_size) return SERIALRPC_ERR_BADDATA;
    memcpy(resp_buf, data, data_len);
    *resp_len = data_len;
    return 0;
}

// Helper: build a call packet (meta, proc_idx, [len_lo, len_hi, data...])
void build_call_packet(std::vector<uint8_t>& pkt, uint8_t proc_idx, const uint8_t* data, uint16_t data_len) {
    MetaBlock mb = {0};
    mb.is_call = true;
    mb.has_error = false;
    mb.has_data = (data_len > 0);
    mb.parity = false;
    mb.error_code = 0;
    pkt.push_back(metabyte_from_block(&mb));
    pkt.push_back(proc_idx);
    if (data_len > 0) {
        pkt.push_back(data_len & 0xFF);
        pkt.push_back((data_len >> 8) & 0xFF);
        for (uint16_t i = 0; i < data_len; ++i) pkt.push_back(data[i]);
    }
}

int test_echo(ConsoleState* state) {
    Serial.clear();
    // Setup SerialRpc with one procedure (echo)
    SerialRpcProcedure procs[1] = { echo_proc };
    ProcedureSet ps = { procs, 1 };
    SerialRpc rpc = { ps };
    // Build call packet for echo (proc_idx=0, data="hi")
    std::vector<uint8_t> pkt;
    const char* msg = "hi";
    build_call_packet(pkt, 0, (const uint8_t*)msg, 2);
    Serial.push_bytes(pkt.data(), pkt.size());
    // Run tick
    serial_rpc_tick(&rpc);
    // Check response: meta, proc_idx, len_lo, len_hi, data
    if (Serial.written.size() < 6) {
        print_above(state, COLOR_RED, " echo: response too short");
        return 1;
    }
    uint8_t meta = Serial.written[0];
    MetaBlock mb;
    metabyte_to_block(meta, &mb);
    if (mb.is_call || mb.has_error || !mb.has_data) {
        print_above(state, COLOR_RED, " echo: bad meta byte");
        return 2;
    }
    if (Serial.written[1] != 0) {
        print_above(state, COLOR_RED, " echo: wrong proc_idx");
        return 3;
    }
    uint16_t len = Serial.written[2] | (Serial.written[3] << 8);
    if (len != 2) {
        print_above(state, COLOR_RED, " echo: wrong length");
        return 4;
    }
    if (memcmp(&Serial.written[4], msg, 2) != 0) {
        print_above(state, COLOR_RED, " echo: wrong data");
        return 5;
    }
    print_above(state, COLOR_GREEN, " echo test passed");
    return 0;
}

int test_notfound(ConsoleState* state) {
    Serial.clear();
    // Setup SerialRpc with no procedures
    SerialRpcProcedure procs[1] = { echo_proc };
    ProcedureSet ps = { procs, 1 };
    SerialRpc rpc = { ps };
    // Build call packet for invalid proc_idx=1
    std::vector<uint8_t> pkt;
    build_call_packet(pkt, 1, nullptr, 0);
    Serial.push_bytes(pkt.data(), pkt.size());
    serial_rpc_tick(&rpc);
    // Should respond with error meta, proc_idx=1
    if (Serial.written.size() < 2) {
        print_above(state, COLOR_RED, " notfound: response too short");
        return 1;
    }
    uint8_t meta = Serial.written[0];
    MetaBlock mb;
    metabyte_to_block(meta, &mb);
    if (mb.is_call || !mb.has_error) {
        print_above(state, COLOR_RED, " notfound: bad meta");
        return 2;
    }
    if (Serial.written[1] != 1) {
        print_above(state, COLOR_RED, " notfound: wrong proc_idx");
        return 3;
    }
    print_above(state, COLOR_GREEN, " notfound test passed");
    return 0;
}

int main() {
    typedef int (*TestFunc)(ConsoleState*);
    TestFunc tests[] = {
        test_echo,
        test_notfound
    };
    int total = sizeof(tests) / sizeof(tests[0]);
    int failed = 0;
    ConsoleState state = {""};
    for (int ran = 0; ran < total; ++ran) {
        int res = tests[ran](&state);
        if (res != 0) failed++;
        print_update(&state, "Tests run: %d/%d", ran + 1, total);
    }
    print_update(&state, "Tests run: %d/%d\n", total, total);
    if (failed == 0) {
        print_above(&state, COLOR_CYAN, "All serialrpc tests passed!");
    } else {
        print_above(&state, COLOR_RED, "%d test(s) failed!", failed);
    }
    return failed;
}
