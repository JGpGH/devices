#include "config.h"
#include "procedure.hpp"

class TestResult {
public:
    char* name;
    bool success;
    char* reason;

    void fail(char* reason) {
        success = false;
        this->reason = reason;
    }

    void pass() {
        success = true;
    }

    String toString() {
        String res = "";
        res += name;
        res += ": ";
        res += success ? "PASSED\0" : "FAILED\0";
        res += success ? "" : " - ";
        if(!success) {
            res += reason;
        }
        return res;
    }
};

TestResult testu8Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    unsigned char u8Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_u8Arr(u8Arr, 13);
    buffer->decode_u8Arr(u8Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u8Arr[i] != i) {
            result.success = false;
            result.reason = "u8Arr failed;\0";
            break;
        }
    }
    result.name = "u8Arr\0";
    return result;
}

TestResult testu16Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    unsigned int u16Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_u16Arr(u16Arr, 13);
    buffer->decode_u16Arr(u16Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u16Arr[i] != i) {
            result.success = false;
            result.reason = "u16Arr failed;\0";
            break;
        }
    }
    result.name = "u16Arr\0";
    return result;
}

TestResult testu32Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    unsigned long u32Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_u32Arr(u32Arr, 13);
    buffer->decode_u32Arr(u32Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u32Arr[i] != i) {
            result.success = false;
            result.reason = "u32Arr failed\0";
            break;
        }
    }
    result.name = "u32Arr\0";
    return result;
}

TestResult testu64Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    unsigned long long u64Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_u64Arr(u64Arr, 13);
    buffer->decode_u64Arr(u64Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u64Arr[i] != i) {
            result.success = false;
            result.reason = "u64Arr failed\0";
            break;
        }
    }
    result.name = "u64Arr\0";
    return result;
}

TestResult testi8Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    char i8Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_i8Arr(i8Arr, 13);
    buffer->decode_i8Arr(i8Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i8Arr[i] != i) {
            result.success = false;
            result.reason = "i8Arr failed\0";
            break;
        }
    }
    result.name = "i8Arr\0";
    return result;
}

TestResult testi16Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    int i16Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_i16Arr(i16Arr, 13);
    buffer->decode_i16Arr(i16Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i16Arr[i] != i) {
            result.success = false;
            result.reason = "i16Arr failed\0";
            break;
        }
    }
    result.name = "i16Arr\0";
    return result;
}

TestResult testi32Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    long i32Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_i32Arr(i32Arr, 13);
    buffer->decode_i32Arr(i32Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i32Arr[i] != i) {
            result.success = false;
            result.reason = "i32Arr failed\0";
            break;
        }
    }
    result.name = "i32Arr\0";
    return result;
}

TestResult testi64Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    long long i64Arr[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    buffer->encode_i64Arr(i64Arr, 13);
    buffer->decode_i64Arr(i64Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i64Arr[i] != i) {
            result.success = false;
            result.reason = "i64Arr failed\0";
            break;
        }
    }
    result.name = "i64Arr\0";
    return result;
}

TestResult testf32Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    float f32Arr[13] = { 0.0f, 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.10f, 11.11f, 12.12f };
    buffer->encode_f32Arr(f32Arr, 13);
    buffer->decode_f32Arr(f32Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (f32Arr[i] != i + 0.1f) {
            result.success = false;
            result.reason = "f32Arr failed\0";
            break;
        }
    }
    result.name = "f32Arr\0";
    return result;
}

TestResult testf64Array(Buffer* buffer) {
    TestResult result;
    buffer->reset();
    result.success = true;
    result.reason = "";
    unsigned int reslen = 0;
    double f64Arr[13] = { 0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10, 11.11, 12.12 };
    buffer->encode_f64Arr(f64Arr, 13);
    buffer->decode_f64Arr(f64Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (f64Arr[i] != i + 0.1) {
            result.success = false;
            result.reason = "f64Arr failed\0";
            break;
        }
    }
    result.name = "f64Arr\0";
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
    buffer->decode_u8(&success) == 0x01 ? result.pass() : result.fail("decoding failed\0");
    result.name = "u8\0";
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
    buffer->decode_u16(&success) == 0x0203 ? result.pass() : result.fail("decoding failed\0");
    result.name = "u16\0";
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
    buffer->decode_u32(&success) == 0x04050607 ? result.pass() : result.fail("decoding failed\0");
    result.name = "u32\0";
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
    buffer->decode_u64(&success) == 0x08090a0b0c0d0e0f ? result.pass() : result.fail("decoding failed\0");
    result.name = "u64\0";
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
    buffer->decode_i8(&success) == 0x01 ? result.pass() : result.fail("decoding failed\0");
    result.name = "i8\0";
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
    buffer->decode_i16(&success) == 0x0203 ? result.pass() : result.fail("decoding failed\0");
    result.name = "i16\0";
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
    buffer->decode_i32(&success) == 0x04050607 ? result.pass() : result.fail("decoding failed\0");
    result.name = "i32\0";
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
    buffer->decode_i64(&success) == 0x08090a0b0c0d0e0f ? result.pass() : result.fail("decoding failed\0");
    result.name = "i64\0";
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
    buffer->decode_bool(&success) ? result.pass() : result.fail("decoding failed\0");
    result.name = "bool\0";
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
    buffer->decode_f32(&success) == 3.14159f ? result.pass() : result.fail("decoding failed\0");
    result.name = "f32\0";
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
    buffer->decode_f64(&success) == 3.141592653589793238462643383279502884197169399375105820974944592307816406286 ? result.pass() : result.fail("decoding failed\0");
    result.name = "f64\0";
    return result;
}

const int TEST_AMOUNT = 17;
TestResult test_result_data[TEST_AMOUNT];
void testCodecs() {
    bool success = false;
    Buffer buffer = Buffer(128);
    test_result_data[0] = test_u8(&buffer);
    test_result_data[1] = test_u16(&buffer);
    test_result_data[2] = test_u32(&buffer);
    test_result_data[3] = test_u64(&buffer);
    test_result_data[4] = test_bool(&buffer);
    test_result_data[5] = test_f32(&buffer);
    test_result_data[6] = test_f64(&buffer);
    /*test_result_data[7] = testu8Array(&buffer);
    test_result_data[8] = testu16Array(&buffer);
    test_result_data[9] = testu32Array(&buffer);
    test_result_data[10] = testu64Array(&buffer);
    test_result_data[11] = testi8Array(&buffer);
    test_result_data[12] = testi16Array(&buffer);
    test_result_data[13] = testi32Array(&buffer);
    test_result_data[14] = testi64Array(&buffer);
    test_result_data[15] = testf32Array(&buffer);
    test_result_data[16] = testf64Array(&buffer);*/
}

char data_buffer[1024] = {};
void setup() {
    Serial.begin(115200);
    memset(data_buffer, 0, 1024);
}

int loop_count = 0;
char baudrate_str[10] = {};
void loop() {
    if(Serial.available() > 0) {
        Serial.write("BAUDRATE: 101\0");
        Serial.write("Running tests...\0");
        int bytesToRead = min(Serial.available(), 1024);
        Serial.readBytes(data_buffer, bytesToRead);
        Serial.write(data_buffer, bytesToRead);
        testCodecs();
        for (int i = 0; i < TEST_AMOUNT; i++) {
            Serial.write(test_result_data[i].toString().c_str(), test_result_data[i].toString().length());
        }
    }
    delay(10);
}