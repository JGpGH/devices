#include "serialrpc_client.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include "console_util.cpp"

int test_echo(ConsoleState* state, SerialRpcClient& client) {
    std::vector<uint8_t> test_data = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t meta = 0, proc_idx = 0;
    auto resp = client.call(/*proc_idx=*/17, test_data, meta, proc_idx);
    print_above(state, COLOR_BLUE, " Testing echo procedure with data: %02X %02X %02X %02X", 
        test_data[0], test_data[1], test_data[2], test_data[3]);
    print_above(state, COLOR_BLUE, " Received response: %02X %02X %02X %02X", 
        resp.size() > 0 ? resp[0] : 0, resp.size() > 1 ? resp[1] : 0,
        resp.size() > 2 ? resp[2] : 0, resp.size() > 3 ? resp[3] : 0);
    if (resp != test_data) {
        print_above(state, COLOR_RED, " Echo test failed! Sent: %02X %02X %02X %02X Got: %02X %02X %02X %02X",
            test_data[0], test_data[1], test_data[2], test_data[3],
            resp.size() > 0 ? resp[0] : 0, resp.size() > 1 ? resp[1] : 0,
            resp.size() > 2 ? resp[2] : 0, resp.size() > 3 ? resp[3] : 0);
        return 1;
    }

    if (meta != 0x05) {
        print_above(state, COLOR_RED, " Echo test returned unexpected meta: expected 0x05, got %02X", meta);
        return 2;
    }

    if (proc_idx != 17) {
        print_above(state, COLOR_RED, " Echo test returned unexpected proc_idx: expected 0x17, got %02X", proc_idx);
        return 2;
    }

    print_above(state, COLOR_GREEN, " Echo test passed");
    return 0;
}

void print_meta(ConsoleState* state, uint8_t meta) {
    MetaBlock mb;
    metabyte_to_block(meta, &mb);
    print_above(state, COLOR_CYAN, "Meta: is_call=%d, has_error=%d, has_data=%d, parity=%d, error_code=0x%02X",
        mb.is_call, mb.has_error, mb.has_data, mb.parity, mb.error_code);
}

int test_add(ConsoleState* state, SerialRpcClient& client) {
    // Prepare two uint32_t values
    uint32_t a = 0x12221122, b = 0x11334455;
    std::vector<uint8_t> test_data(0);
    SerialRpcClient::write(test_data, a);
    SerialRpcClient::write(test_data, b);
    print_above(state, COLOR_BLUE, "data payload: %02X %02X %02X %02X %02X %02X %02X %02X",
        test_data[0], test_data[1], test_data[2], test_data[3],
        test_data[4], test_data[5], test_data[6], test_data[7]);
    uint8_t meta = 0, proc_idx = 0;
    print_above(state, COLOR_BLUE, " Testing add procedure with: %08X + %08X", a, b);
    auto resp = client.call(0, test_data, meta, proc_idx);
    if (resp.size() != 4) {
        print_above(state, COLOR_RED, " Add test failed: no or incomplete response");
        print_meta(state, meta);
        return 1;
    }
    uint32_t sum = 0;
    int readPtr = 0;
    SerialRpcClient::read(resp, readPtr, &sum);
    if (meta != 0x04) {
        print_above(state, COLOR_RED, " Add test returned unexpected meta: expected 0x04, got %02X", meta);
        print_meta(state, meta);
        return 3;
    }
    if (proc_idx != 0) {
        print_above(state, COLOR_RED, " Add test returned unexpected proc_idx: expected 0x00, got %02X", proc_idx);
        print_meta(state, meta);
        return 4;
    }
    if (sum != a + b) {
        print_above(state, COLOR_RED, " Add test failed: expected %08X, got %08X", a + b, sum);
        print_meta(state, meta);
        return 2;
    }
    print_above(state, COLOR_GREEN, " Add test passed");
    return 0;
}



int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <serial device> [test_name]" << std::endl;
        return 1;
    }
    std::string device = argv[1];
    std::string test_name = (argc >= 3) ? argv[2] : "";
    SerialRpcClient client(device);
    typedef int (*TestFunc)(ConsoleState*, SerialRpcClient&);
    struct TestEntry { const char* name; TestFunc func; };
    TestEntry tests[] = {
        {"echo", test_echo},
        {"add", test_add},
    };
    int total = sizeof(tests) / sizeof(tests[0]);
    int failed = 0;
    int ran = 0;
    ConsoleState state = {""};
    for (int i = 0; i < total; ++i) {
        if (!test_name.empty() && strcasecmp(test_name.c_str(), tests[i].name) != 0) continue;
        int res = tests[i].func(&state, client);
        ran++;
        if (res != 0) failed++;
        print_update(&state, "Tests run: %d/%d", ran, test_name.empty() ? total : 1);
    }
    print_update(&state, "Tests run: %d/%d\n", ran, test_name.empty() ? total : 1);
    if (ran == 0) {
        print_above(&state, COLOR_RED, "No test matched: %s", test_name.c_str());
        return 2;
    }
    if (failed == 0) {
        print_above(&state, COLOR_CYAN, "All device integration tests passed!");
    } else {
        print_above(&state, COLOR_RED, "%d test(s) failed!", failed);
    }
    return failed;
}
