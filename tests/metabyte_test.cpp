#include <stdio.h>
#include <assert.h>
#include "../libs/SerialRpc/codec.hpp"
#include "console_util.cpp"

int test_metabyte_roundtrip(ConsoleState* state) {
    MetaBlock mb = {true, true, true, true, 0xA};
    uint8_t meta = metabyte_from_block(&mb);
    MetaBlock out = {0};
    metabyte_to_block(meta, &out);
    if (!(out.is_call && out.has_error && out.has_data && out.parity && out.error_code == 0xA)) {
        print_above(state, COLOR_RED, " metabyte roundtrip failed");
        return 1;
    }
    print_above(state, COLOR_GREEN, " metabyte roundtrip passed");
    return 0;
}

int test_metabyte_flags(ConsoleState* state) {
    MetaBlock mb = {false, true, false, true, 0x5};
    uint8_t meta = metabyte_from_block(&mb);
    MetaBlock out = {0};
    metabyte_to_block(meta, &out);
    if (!(out.is_call == false && out.has_error && out.has_data == false && out.parity && out.error_code == 0x5)) {
        print_above(state, COLOR_RED, " metabyte flags test failed");
        return 1;
    }
    print_above(state, COLOR_GREEN, " metabyte flags test passed");
    return 0;
}

int main() {
    typedef int (*TestFunc)(ConsoleState*);
    TestFunc tests[] = {
        test_metabyte_roundtrip,
        test_metabyte_flags
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
        print_above(&state, COLOR_CYAN, "All metabyte tests passed!");
    } else {
        print_above(&state, COLOR_RED, "%d test(s) failed!", failed);
    }
    return failed;
}
