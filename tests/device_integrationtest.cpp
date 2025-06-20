#include "serialrpc_client.hpp"
#include <iostream>
#include <vector>
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

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <serial device>" << std::endl;
        return 1;
    }
    std::string device = argv[1];
    SerialRpcClient client(device);
    typedef int (*TestFunc)(ConsoleState*, SerialRpcClient&);
    TestFunc tests[] = { test_echo };
    int total = sizeof(tests) / sizeof(tests[0]);
    int failed = 0;
    ConsoleState state = {""};
    for (int ran = 0; ran < total; ++ran) {
        int res = tests[ran](&state, client);
        if (res != 0) failed++;
        print_update(&state, "Tests run: %d/%d", ran + 1, total);
    }
    print_update(&state, "Tests run: %d/%d\n", total, total);
    if (failed == 0) {
        print_above(&state, COLOR_CYAN, "All device integration tests passed!");
    } else {
        print_above(&state, COLOR_RED, "%d test(s) failed!", failed);
    }
    return failed;
}
