#include "procedure.h"

class Buffer {
    byte* data;
    unsigned int len;
    unsigned int readIndex; // new member variable to keep track of read index
    unsigned int writeIndex; // new member variable to keep track of write index
    public:
        Buffer(byte* data, unsigned int len) : data(data), len(len), readIndex(0), writeIndex(0) {}

        void reset() {
            readIndex = 0;
            writeIndex = 0;
        }

        unsigned int available() {
            return writeIndex - readIndex;
        }

        void skip(unsigned int len) {
            readIndex += len;
        }

        void append(byte* data, unsigned int len) {
            memcpy(this->data + writeIndex, data, len);
            writeIndex += len;
        }

        byte decode_u8() {
            if (available() < 1) {
                return 0;
            }
            byte value = data[readIndex];
            readIndex += 1;
            return value;
        }

        unsigned int decode_u16() {
            if (available() < 2) {
                return 0;
            }
            unsigned int value = data[readIndex] | (data[readIndex + 1] << 8);
            readIndex += 2;
            return value;
        }

        unsigned int decode_u32() {
            if (available() < 4) {
                return 0;
            }
            unsigned int value = data[readIndex] | (data[readIndex + 1] << 8) |
                              (data[readIndex + 2] << 16) | (data[readIndex + 3] << 24);
            readIndex += 4;
            return value;
        }

        unsigned long decode_u64() {
            if (available() < 8) {
                return 0;
            }
            unsigned long value = data[readIndex] | (data[readIndex + 1] << 8) |
                               (data[readIndex + 2] << 16) | (data[readIndex + 3] << 24) |
                               (data[readIndex + 4] << 32) | (data[readIndex + 5] << 40) |
                               (data[readIndex + 6] << 48) | (data[readIndex + 7] << 56);
            readIndex += 8;
            return value;
        }

        bool decode_bool() {
            if (available() < 1) {
                return false;
            }
            bool value = data[readIndex] != 0;
            readIndex += 1;
            return value;
        }

        float decode_f32() {
            if (available() < 4) {
                return 0;
            }
            float value;
            memcpy(&value, data + readIndex, 4);
            readIndex += 4;
            return value;
        }

        double decode_f64() {
            if (available() < 8) {
                return 0;
            }
            double value;
            memcpy(&value, data + readIndex, 8);
            readIndex += 8;
            return value;
        }

        String decode_string() {
            unsigned int strLen = decode_u16();
            if (available() < strLen) {
                return "";
            }
            String value((char*)(data + readIndex), strLen);
            readIndex += strLen;
            return value;
        }

        char decode_i8 () {
            return (char)decode_u8();
        }

        int decode_i16() {
            return (int)decode_u16();
        }

        long decode_i32() {
            return (long)decode_u32();
        }

        long long decode_i64() {
            return (long long)decode_u64();
        }

        void encode_u8(byte value) {
            data[writeIndex] = value;
            writeIndex += 1;
        }

        void encode_u16(unsigned int value) {
            data[writeIndex] = value & 0xFF;
            data[writeIndex + 1] = (value >> 8) & 0xFF;
            writeIndex += 2;
        }

        void encode_u32(unsigned int value) {
            data[writeIndex] = value & 0xFF;
            data[writeIndex + 1] = (value >> 8) & 0xFF;
            data[writeIndex + 2] = (value >> 16) & 0xFF;
            data[writeIndex + 3] = (value >> 24) & 0xFF;
            writeIndex += 4;
        }

        void encode_u64(unsigned long value) {
            data[writeIndex] = value & 0xFF;
            data[writeIndex + 1] = (value >> 8) & 0xFF;
            data[writeIndex + 2] = (value >> 16) & 0xFF;
            data[writeIndex + 3] = (value >> 24) & 0xFF;
            data[writeIndex + 4] = (value >> 32) & 0xFF;
            data[writeIndex + 5] = (value >> 40) & 0xFF;
            data[writeIndex + 6] = (value >> 48) & 0xFF;
            data[writeIndex + 7] = (value >> 56) & 0xFF;
            writeIndex += 8;
        }

        void encode_bool(bool value) {
            data[writeIndex] = value ? 1 : 0;
            writeIndex += 1;
        }

        void encode_f32(float value) {
            memcpy(data + writeIndex, &value, 4);
            writeIndex += 4;
        }

        void encode_f64(double value) {
            memcpy(data + writeIndex, &value, 8);
            writeIndex += 8;
        }

        void encode_string(String value) {
            unsigned int strLen = value.length();
            encode_u16(strLen);
            memcpy(data + writeIndex, value.c_str(), strLen);
            writeIndex += strLen;
        }

        void encode_i8(char value) {
            encode_u8((byte)value);
        }

        void encode_i16(int value) {
            encode_u16((unsigned int)value);
        }

        void encode_i32(long value) {
            encode_u32((unsigned long)value);
        }

        void encode_i64(long long value) {
            encode_u64((unsigned long)value);
        }

        //TODO: add array codec functions
};