#include <vector>
#include "config.h"
#include "procedure.hpp"

TestResult testu8Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    unsigned char u8Arr[13];
    unsigned int reslen = 0;
    buffer.encode_u8Arr((unsigned char*) "Hello, World!", 13);
    buffer.decode_u8Arr(u8Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u8Arr[i] != (unsigned char) "Hello, World!"[i]) {
            result.success = false;
            result.message = "u8Arr failed;";
            break;
        }
    }
    result.name = "u8Arr";
    return result;
}

TestResult testu16Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    unsigned int u16Arr[13];
    unsigned int reslen = 0;
    buffer.encode_u16Arr((unsigned int*) "Hello, World!", 13);
    buffer.decode_u16Arr(u16Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u16Arr[i] != (unsigned int) "Hello, World!"[i]) {
            result.success = false;
            result.message = "u16Arr failed;";
            break;
        }
    }
    result.name = "u16Arr";
    return result;
}

TestResult testu32Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    unsigned long u32Arr[13];
    unsigned int reslen = 0;
    buffer.encode_u32Arr((unsigned long*) "Hello, World!", 13);
    buffer.decode_u32Arr(u32Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u32Arr[i] != (unsigned long) "Hello, World!"[i]) {
            result.success = false;
            result.message = "u32Arr failed;";
            break;
        }
    }
    result.name = "u32Arr";
    return result;
}

TestResult testu64Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    unsigned long long u64Arr[13];
    unsigned int reslen = 0;
    buffer.encode_u64Arr((unsigned long long*) "Hello, World!", 13);
    buffer.decode_u64Arr(u64Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (u64Arr[i] != (unsigned long long) "Hello, World!"[i]) {
            result.success = false;
            result.message = "u64Arr failed;";
            break;
        }
    }
    result.name = "u64Arr";
    return result;
}

TestResult testi8Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    char i8Arr[13];
    unsigned int reslen = 0;
    buffer.encode_i8Arr((char*) "Hello, World!", 13);
    buffer.decode_i8Arr(i8Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i8Arr[i] != (char) "Hello, World!"[i]) {
            result.success = false;
            result.message = "i8Arr failed;";
            break;
        }
    }
    result.name = "i8Arr";
    return result;
}

TestResult testi16Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    int i16Arr[13];
    unsigned int reslen = 0;
    buffer.encode_i16Arr((int*) "Hello, World!", 13);
    buffer.decode_i16Arr(i16Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i16Arr[i] != (int) "Hello, World!"[i]) {
            result.success = false;
            result.message = "i16Arr failed;";
            break;
        }
    }
    result.name = "i16Arr";
    return result;
}

TestResult testi32Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    long i32Arr[13];
    unsigned int reslen = 0;
    buffer.encode_i32Arr((long*) "Hello, World!", 13);
    buffer.decode_i32Arr(i32Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i32Arr[i] != (long) "Hello, World!"[i]) {
            result.success = false;
            result.message = "i32Arr failed;";
            break;
        }
    }
    result.name = "i32Arr";
    return result;
}

TestResult testi64Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    long long i64Arr[13];
    unsigned int reslen = 0;
    buffer.encode_i64Arr((long long*) "Hello, World!", 13);
    buffer.decode_i64Arr(i64Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (i64Arr[i] != (long long) "Hello, World!"[i]) {
            result.success = false;
            result.message = "i64Arr failed;";
            break;
        }
    }
    result.name = "i64Arr";
    return result;
}

TestResult testf32Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    float f32Arr[13];
    unsigned int reslen = 0;
    buffer.encode_f32Arr((float*) "Hello, World!", 13);
    buffer.decode_f32Arr(f32Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (f32Arr[i] != (float) "Hello, World!"[i]) {
            result.success = false;
            result.message = "f32Arr failed;";
            break;
        }
    }
    result.name = "f32Arr";
    return result;
}

TestResult testf64Array() {
    TestResult result;
    result.success = true;
    result.message = "";
    Buffer buffer = Buffer(128);
    double f64Arr[13];
    unsigned int reslen = 0;
    buffer.encode_f64Arr((double*) "Hello, World!", 13);
    buffer.decode_f64Arr(f64Arr, &reslen, 13);
    for (int i = 0; i < 13; i++) {
        if (f64Arr[i] != (double) "Hello, World!"[i]) {
            result.success = false;
            result.message = "f64Arr failed;";
            break;
        }
    }
    result.name = "f64Arr";
    return result;
}

TestResult test_u8(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    unsigned char u8 = 0x01;
    buffer.encode_u8(u8);
    buffer.decode_u8(&u8) == 0x01 ? result.message += "u8 passed;" : result.message += "u8 failed;";
    result.name = "u8";
    return result;
}

TestResult test_u16(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    unsigned int u16 = 0x0203;
    buffer.encode_u16(u16);
    buffer.decode_u16(&u16) == 0x0203 ? result.message += "u16 passed;" : result.message += "u16 failed;";
    result.name = "u16";
    return result;
}

TestResult test_u32(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    unsigned long u32 = 0x04050607;
    buffer.encode_u32(u32);
    buffer.decode_u32(&u32) == 0x04050607 ? result.message += "u32 passed;" : result.message += "u32 failed;";
    result.name = "u32";
    return result;
}

TestResult test_u64(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    unsigned long long u64 = 0x08090a0b0c0d0e0f;
    buffer.encode_u64(u64);
    buffer.decode_u64(&u64) == 0x08090a0b0c0d0e0f ? result.message += "u64 passed;" : result.message += "u64 failed;";
    result.name = "u64";
    return result;
}

TestResult test_i8(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    char i8 = 0x01;
    buffer.encode_i8(i8);
    buffer.decode_i8(&i8) == 0x01 ? result.message += "i8 passed;" : result.message += "i8 failed;";
    result.name = "i8";
    return result;
}

TestResult test_i16(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    int i16 = 0x0203;
    buffer.encode_i16(i16);
    buffer.decode_i16(&i16) == 0x0203 ? result.message += "i16 passed;" : result.message += "i16 failed;";
    result.name = "i16";
    return result;
}

TestResult test_i32(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    long i32 = 0x04050607;
    buffer.encode_i32(i32);
    buffer.decode_i32(&i32) == 0x04050607 ? result.message += "i32 passed;" : result.message += "i32 failed;";
    result.name = "i32";
    return result;
}

TestResult test_i64(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    long long i64 = 0x08090a0b0c0d0e0f;
    buffer.encode_i64(i64);
    buffer.decode_i64(&i64) == 0x08090a0b0c0d0e0f ? result.message += "i64 passed;" : result.message += "i64 failed;";
    result.name = "i64";
    return result;
}

TestResult test_bool(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    bool b = true;
    buffer.encode_bool(b);
    buffer.decode_bool(&b) ? result.message += "bool passed;" : result.message += "bool failed;";
    result.name = "bool";
    return result;
}

TestResult test_f32(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    float f32 = 3.14159f;
    buffer.encode_f32(f32);
    buffer.decode_f32(&f32) == 3.14159f ? result.message += "f32 passed;" : result.message += "f32 failed;";
    result.name = "f32";
    return result;
}

TestResult test_f64(&Buffer buffer)  {
    TestResult result;
    result.success = true;
    result.message = "";
    double f64 = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;
    buffer.encode_f64(f64);
    buffer.decode_f64(&f64) == 3.141592653589793238462643383279502884197169399375105820974944592307816406286 ? result.message += "f64 passed;" : result.message += "f64 failed;";
    result.name = "f64";
    return result;
}

class TestResult {
public:
    String name;
    bool success;
    String messages;
};

std::vector<TestResult> testCodecs() {
    std::vector<TestResult> result = {};
    bool success = false;
    Buffer buffer = Buffer(128);
    result.push_back(test_u8(buffer));
    result.push_back(test_u16(buffer));
    result.push_back(test_u32(buffer));
    result.push_back(test_u64(buffer));
    result.push_back(test_bool(buffer));
    result.push_back(test_f32(buffer));
    result.push_back(test_f64(buffer));
    result.push_back(testu8Array());
    result.push_back(testu16Array());
    result.push_back(testu32Array());
    result.push_back(testu64Array());
    result.push_back(testi8Array());
    result.push_back(testi16Array());
    result.push_back(testi32Array());
    result.push_back(testi64Array());
    result.push_back(testf32Array());
    result.push_back(testf64Array());
    return result;
}

void setup() {
    Serial.begin(115200);
}

char data_buffer[1024] = {};
int loop_count = 0;
char baudrate_str[10] = {};
void loop() {
    if(Serial.available() > 0) {
        Serial.write("BAUDRATE: 101", 10);
        Serial.write("Running tests...", 16);
        int bytesToRead = min(Serial.available(), 1024);
        Serial.readBytes(data_buffer, bytesToRead);
        Serial.write(data_buffer, bytesToRead);
        String result = testCodecs();
        Serial.write(result.c_str(), result.length());
        // Clear the serial input buffer
        while (Serial.available() > 0) {
            Serial.read();
        }
    }
    delay(10);
}