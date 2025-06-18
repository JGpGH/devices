#include <stdio.h>
#include <assert.h>
#include "../libs/SerialRpc/codec.hpp"
#include "console_util.cpp"

// Use print_above for test result output so progress stays on the last line
int test_u8_roundtrip(ConsoleState* state) {
    uint8_t buf[1]; size_t pos = 0;
    uint8_t val = 0xAB;
    if (!bv_write_u8(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 1, 0 };
    uint8_t out = 0;
    if (!bv_read_u8(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_u8_roundtrip passed");
    return 0;
}

int test_u16_roundtrip(ConsoleState* state) {
    uint8_t buf[2]; size_t pos = 0;
    uint16_t val = 0xBEEF;
    if (!bv_write_u16(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 2, 0 };
    uint16_t out = 0;
    if (!bv_read_u16(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_u16_roundtrip passed");
    return 0;
}

int test_u32_roundtrip(ConsoleState* state) {
    uint8_t buf[4]; size_t pos = 0;
    uint32_t val = 0x12345678;
    if (!bv_write_u32(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 4, 0 };
    uint32_t out = 0;
    if (!bv_read_u32(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_u32_roundtrip passed");
    return 0;
}

int test_u64_roundtrip(ConsoleState* state) {
    uint8_t buf[8]; size_t pos = 0;
    uint64_t val = 0x1122334455667788ULL;
    if (!bv_write_u64(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 8, 0 };
    uint64_t out = 0;
    if (!bv_read_u64(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_u64_roundtrip passed");
    return 0;
}

int test_i32_roundtrip(ConsoleState* state) {
    uint8_t buf[4]; size_t pos = 0;
    int32_t val = -12345678;
    if (!bv_write_i32(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 4, 0 };
    int32_t out = 0;
    if (!bv_read_i32(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_i32_roundtrip passed");
    return 0;
}

int test_f32_roundtrip(ConsoleState* state) {
    uint8_t buf[4]; size_t pos = 0;
    float val = 3.14159f;
    if (!bv_write_f32(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 4, 0 };
    float out = 0;
    if (!bv_read_f32(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_f32_roundtrip passed");
    return 0;
}

int test_f64_roundtrip(ConsoleState* state) {
    uint8_t buf[8]; size_t pos = 0;
    double val = 2.718281828459;
    if (!bv_write_f64(buf, sizeof(buf), &pos, val)) return 1;
    BufferView bv = { buf, 8, 0 };
    double out = 0;
    if (!bv_read_f64(&bv, &out)) return 2;
    if (out != val) return 3;
    print_above(state, COLOR_GREEN, " test_f64_roundtrip passed");
    return 0;
}

int test_bool_roundtrip(ConsoleState* state) {
    uint8_t buf[1]; size_t pos = 0;
    if (!bv_write_bool(buf, sizeof(buf), &pos, true)) return 1;
    BufferView bv = { buf, 1, 0 };
    bool out = false;
    if (!bv_read_bool(&bv, &out)) return 2;
    if (out != true) return 3;
    print_above(state, COLOR_GREEN, " test_bool_roundtrip passed");
    return 0;
}

int test_string_roundtrip(ConsoleState* state) {
    uint8_t buf[64]; size_t pos = 0;
    const char* str = "hello world";
    uint32_t len = (uint32_t)strlen(str);
    if (!bv_write_string(buf, sizeof(buf), &pos, str, len)) return 1;
    BufferView bv = { buf, pos, 0 };
    RpcString out;
    if (!bv_read_string(&bv, &out)) return 2;
    if (out.len != len) return 3;
    if (strncmp(out.data, str, len) != 0) return 4;
    print_above(state, COLOR_GREEN, " test_string_roundtrip passed");
    return 0;
}

int main() {
    typedef int (*TestFunc)(ConsoleState*);
    TestFunc tests[] = {
        test_u8_roundtrip,
        test_u16_roundtrip,
        test_u32_roundtrip,
        test_u64_roundtrip,
        test_i32_roundtrip,
        test_f32_roundtrip,
        test_f64_roundtrip,
        test_bool_roundtrip,
        test_string_roundtrip
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
        print_above(&state, COLOR_CYAN, "All codec tests passed!");
    } else {
        print_above(&state, COLOR_RED, "%d test(s) failed!", failed);
    }
    return failed;
}