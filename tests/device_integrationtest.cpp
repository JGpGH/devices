#include "serialrpc_client.hpp"
#include <iostream>
#include <cassert>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <serial device>" << std::endl;
        return 1;
    }
    std::string device = argv[1];
    SerialRpcClient client(device);
    // Test echo: send bytes {0xDE, 0xAD, 0xBE, 0xEF}
    std::vector<uint8_t> test_data = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t meta = 0, proc_idx = 0;
    auto resp = client.call(/*proc_idx=*/0, test_data, meta, proc_idx);
    // Check response
    if (resp != test_data) {
        std::cerr << "Echo test failed! Sent: ";
        for (auto b : test_data) std::cerr << std::hex << (int)b << " ";
        std::cerr << " Got: ";
        for (auto b : resp) std::cerr << std::hex << (int)b << " ";
        std::cerr << std::endl;
        return 2;
    }
    std::cout << "Echo test passed!" << std::endl;
    return 0;
}
