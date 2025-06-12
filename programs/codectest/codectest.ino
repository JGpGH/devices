#include <codec.hpp>
#include "config.h"

class TestResult {
public:
    const char* name;
    bool success;
    const char* reason;

    void fail(const char* reason) {
        success = false;
        this->reason = reason;
    }

    void pass() {
        success = true;
    }

    // Use a static buffer to avoid dynamic allocation
    const char* toString() {
        static char buf[48];
        snprintf(buf, sizeof(buf), "%s: %s%s%s", name, success ? "PASSED" : "FAILED", success ? "" : " - ", success ? "" : reason);
        return buf;
    }
};

// Shared static arrays for all tests to reduce stack usage
static const int TEST_ARR_LEN = 4;
static unsigned char shared_u8Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static unsigned char shared_decoded_u8Arr[TEST_ARR_LEN] = {0};
static unsigned int shared_u16Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static unsigned int shared_decoded_u16Arr[TEST_ARR_LEN] = {0};
static unsigned long shared_u32Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static unsigned long shared_decoded_u32Arr[TEST_ARR_LEN] = {0};
static unsigned long long shared_u64Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static unsigned long long shared_decoded_u64Arr[TEST_ARR_LEN] = {0};
static char shared_i8Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static char shared_decoded_i8Arr[TEST_ARR_LEN] = {0};
static int shared_i16Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static int shared_decoded_i16Arr[TEST_ARR_LEN] = {0};
static long shared_i32Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static long shared_decoded_i32Arr[TEST_ARR_LEN] = {0};
static long long shared_i64Arr[TEST_ARR_LEN] = {0, 1, 2, 3};
static long long shared_decoded_i64Arr[TEST_ARR_LEN] = {0};
static float shared_f32Arr[TEST_ARR_LEN] = {0.0f, 1.1f, 2.2f, 3.3f};
static float shared_decoded_f32Arr[TEST_ARR_LEN] = {0.0f};
static double shared_f64Arr[TEST_ARR_LEN] = {0.0, 1.1, 2.2, 3.3};
static double shared_decoded_f64Arr[TEST_ARR_LEN] = {0.0};

TestResult testu8Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_u8Arr(shared_u8Arr, TEST_ARR_LEN);
    buffer->decode_u8Arr(shared_decoded_u8Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_u8Arr[i] != shared_u8Arr[i]) {
            result.success = false;
            result.reason = "u8Arr failed";
            break;
        }
    }
    result.name = "u8Arr";
    return result;
}
TestResult testu16Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_u16Arr(shared_u16Arr, TEST_ARR_LEN);
    buffer->decode_u16Arr(shared_decoded_u16Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_u16Arr[i] != shared_u16Arr[i]) {
            result.success = false;
            result.reason = "u16Arr failed";
            break;
        }
    }
    result.name = "u16Arr";
    return result;
}
TestResult testu32Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_u32Arr(shared_u32Arr, TEST_ARR_LEN);
    buffer->decode_u32Arr(shared_decoded_u32Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_u32Arr[i] != shared_u32Arr[i]) {
            result.success = false;
            result.reason = "u32Arr failed";
            break;
        }
    }
    result.name = "u32Arr";
    return result;
}
TestResult testu64Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_u64Arr(shared_u64Arr, TEST_ARR_LEN);
    buffer->decode_u64Arr(shared_decoded_u64Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_u64Arr[i] != shared_u64Arr[i]) {
            result.success = false;
            result.reason = "u64Arr failed";
            break;
        }
    }
    result.name = "u64Arr";
    return result;
}

TestResult testi8Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_i8Arr(shared_i8Arr, TEST_ARR_LEN);
    buffer->decode_i8Arr(shared_decoded_i8Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_i8Arr[i] != shared_i8Arr[i]) {
            result.success = false;
            result.reason = "i8Arr failed";
            break;
        }
    }
    result.name = "i8Arr";
    return result;
}
TestResult testi16Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_i16Arr(shared_i16Arr, TEST_ARR_LEN);
    buffer->decode_i16Arr(shared_decoded_i16Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_i16Arr[i] != shared_i16Arr[i]) {
            result.success = false;
            result.reason = "i16Arr failed";
            break;
        }
    }
    result.name = "i16Arr";
    return result;
}
TestResult testi32Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_i32Arr(shared_i32Arr, TEST_ARR_LEN);
    buffer->decode_i32Arr(shared_decoded_i32Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_i32Arr[i] != shared_i32Arr[i]) {
            result.success = false;
            result.reason = "i32Arr failed";
            break;
        }
    }
    result.name = "i32Arr";
    return result;
}
TestResult testi64Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_i64Arr(shared_i64Arr, TEST_ARR_LEN);
    buffer->decode_i64Arr(shared_decoded_i64Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (shared_decoded_i64Arr[i] != shared_i64Arr[i]) {
            result.success = false;
            result.reason = "i64Arr failed";
            break;
        }
    }
    result.name = "i64Arr";
    return result;
}
TestResult testf32Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_f32Arr(shared_f32Arr, TEST_ARR_LEN);
    buffer->decode_f32Arr(shared_decoded_f32Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (abs(shared_decoded_f32Arr[i] - shared_f32Arr[i]) > 1e-5f) {
            result.success = false;
            result.reason = "f32Arr failed";
            break;
        }
    }
    result.name = "f32Arr";
    return result;
}
TestResult testf64Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    buffer->encode_f64Arr(shared_f64Arr, TEST_ARR_LEN);
    buffer->decode_f64Arr(shared_decoded_f64Arr, &reslen, TEST_ARR_LEN);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        if (abs(shared_decoded_f64Arr[i] - shared_f64Arr[i]) > 1e-9) {
            result.success = false;
            result.reason = "f64Arr failed";
            break;
        }
    }
    result.name = "f64Arr";
    return result;
}

TestResult test_u8(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned char u8 = 0x01;
    bool success = false;
    buffer->encode_u8(u8);
    buffer->decode_u8(&success) == 0x01 ? result.pass() : result.fail("decoding failed");
    result.name = "u8";
    result.success = success;
    return result;
}

TestResult test_u16(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int u16 = 0x0203;
    bool success = false;
    buffer->encode_u16(u16);
    buffer->decode_u16(&success) == 0x0203 ? result.pass() : result.fail("decoding failed");
    result.name = "u16";
    return result;
}

TestResult test_u32(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned long u32 = 0x04050607;
    bool success = false;
    buffer->encode_u32(u32);
    buffer->decode_u32(&success) == 0x04050607 ? result.pass() : result.fail("decoding failed");
    result.name = "u32";
    return result;
}

TestResult test_u64(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned long long u64 = 0x08090a0b0c0d0e0f;
    bool success = false;
    buffer->encode_u64(u64);
    buffer->decode_u64(&success) == 0x08090a0b0c0d0e0f ? result.pass() : result.fail("decoding failed");
    result.name = "u64";
    return result;
}

TestResult test_i8(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    char i8 = 0x01;
    bool success = false;
    buffer->encode_i8(i8);
    buffer->decode_i8(&success) == 0x01 ? result.pass() : result.fail("decoding failed");
    result.name = "i8";
    return result;
}

TestResult test_i16(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    int i16 = 0x0203;
    bool success = false;
    buffer->encode_i16(i16);
    buffer->decode_i16(&success) == 0x0203 ? result.pass() : result.fail("decoding failed");
    result.name = "i16";
    return result;
}

TestResult test_i32(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    long i32 = 0x04050607;
    bool success = false;
    buffer->encode_i32(i32);
    buffer->decode_i32(&success) == 0x04050607 ? result.pass() : result.fail("decoding failed");
    result.name = "i32";
    return result;
}

TestResult test_i64(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    long long i64 = 0x08090a0b0c0d0e0f;
    bool success = false;
    buffer->encode_i64(i64);
    buffer->decode_i64(&success) == 0x08090a0b0c0d0e0f ? result.pass() : result.fail("decoding failed");
    result.name = "i64";
    return result;
}

TestResult test_bool(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    bool b = true;
    bool success = false;
    buffer->encode_bool(b);
    buffer->decode_bool(&success) ? result.pass() : result.fail("decoding failed");
    result.name = "bool";
    return result;
}

TestResult test_f32(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    float f32 = 3.14159f;
    bool success = false;
    buffer->encode_f32(f32);
    buffer->decode_f32(&success) == 3.14159f ? result.pass() : result.fail("decoding failed");
    result.name = "f32";
    return result;
}

TestResult test_f64(Buffer* buffer)  {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    double f64 = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
    bool success = false;
    buffer->encode_f64(f64);
    buffer->decode_f64(&success) == 3.141592653589793238462643383279502884197169399375105820974944592307816406286 ? result.pass() : result.fail("decoding failed");
    result.name = "f64";
    return result;
}

void testCodecs() {
    Buffer buffer = Buffer(128);
    // Run and print each test one by one to save memory
    TestResult result;
    // Define an array of function pointers and their names
    typedef TestResult (*TestFunc)(Buffer*);
    TestFunc tests[] = {
        test_u8, test_u16, test_u32, test_u64, test_bool, test_f32, test_f64,
        testu8Array, testu16Array, testu32Array, testu64Array,
        testi8Array, testi16Array, testi32Array, testi64Array,
        testf32Array, testf64Array
    };
    const int numTests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < numTests; ++i) {
        result = tests[i](&buffer);
        Serial.write(result.toString(), strlen(result.toString()));
        Serial.write("\r\n", 2);
    }
}

char data_buffer[128] = {};
void setup() {
    Serial.begin(BAUDRATE);
    memset(data_buffer, 0, 128);
}

int loop_count = 0;
void loop() {
    if(Serial.available() > 0) {
        Serial.write("Running tests...");
        int bytesToRead = min(Serial.available(), 128);
        Serial.readBytes(data_buffer, bytesToRead);
        testCodecs();
    }
    delay(10);
}