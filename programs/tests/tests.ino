#include "config.h"
#include "procedure.hpp"

String testCodecs() {
    String result = "";
    Buffer buffer = Buffer(128);
    buffer.encode_u8((unsigned char) 0x01);
    buffer.decode_u8() == 0x01 ? result += "u8 passed\n" : result += "u8 failed\n";
    buffer.encode_u16((unsigned int) 0x0203);
    buffer.decode_u16() == 0x0203 ? result += "u16 passed\n" : result += "u16 failed\n";
    buffer.encode_u32((unsigned long) 0x04050607);
    buffer.decode_u32() == (unsigned long) 0x04050607 ? result += "u32 passed\n" : result += "u32 failed\n";
    buffer.encode_u64((unsigned long long) 0x08090a0b0c0d0e0f);
    buffer.decode_u64() == 0x08090a0b0c0d0e0f ? result += "u64 passed\n" : result += "u64 failed\n";
    buffer.encode_bool(true);
    buffer.decode_bool() ? result += "bool passed\n" : result += "bool failed\n";
    buffer.encode_bool(false);
    !buffer.decode_bool() ? result += "bool_2 passed\n" : result += "bool_2 failed\n";
    buffer.encode_f32(3.14159f);
    buffer.decode_f32() == 3.14159f ? result += "f32 passed\n" : result += "f32 failed\n";
    buffer.encode_f64(3.141592653589793238462643383279502884197169399375105820974944592307816406286);
    buffer.decode_f64() == 3.141592653589793238462643383279502884197169399375105820974944592307816406286 ? result += "f64 passed\n" : result += "f64 failed\n";
    buffer.encode_i8(-13);
    buffer.decode_i8() == -13 ? result += "i8 passed\n" : result += "i8 failed\n";
    buffer.encode_i16(-22);
    buffer.decode_i16() == -22 ? result += "i16 passed\n" : result += "i16 failed\n";
    buffer.encode_i16(200);
    buffer.decode_i16() == 200 ? result += "i16_2 passed\n" : result += "i16_2 failed\n";
    buffer.encode_i32((long) -0x04050607);
    buffer.decode_i32() == -0x04050607 ? result += "i32 passed\n" : result += "i32 failed\n";
    buffer.encode_i64((long long) -42);
    buffer.decode_i64() == -42 ? result += "i64 passed\n" : result += "i64 failed\n";
    buffer.encode_i64((long long) -4000002);
    buffer.decode_i64() == -4000002 ? result += "i64_2 passed\n" : result += "i64_2 failed\n";
    return result;
}

void setup() {
    Serial.begin(9600);
    Serial.println("Test started");
    String result = testCodecs();
    Serial.println(result);
}

void loop() {
    delay(1000);
}