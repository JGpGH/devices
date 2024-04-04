#ifndef _SERIAL_RPC_CODEC_HPP_
#define _SERIAL_RPC_CODEC_HPP_

class Buffer {
    unsigned int len;
    unsigned int readIndex; // new member variable to keep track of read index
    unsigned int writeIndex; // new member variable to keep track of write index
    unsigned char* data;

    public:
        Buffer(unsigned char* data, unsigned int len) : data(data), len(len), readIndex(0), writeIndex(0) {}
        Buffer(unsigned int len) : data(new unsigned char[len]), len(len), readIndex(0), writeIndex(0) {}

        ~Buffer() {
            delete[] data;
        }

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

        void append(unsigned char* data, unsigned int len) {
            memcpy(this->data + writeIndex, data, len);
            writeIndex += len;
        }

        void append(unsigned char data) {
            this->data[writeIndex] = data;
            writeIndex += 1;
        }

        unsigned char decode_u8(bool* success) {
            if (available() < 1) {
                *success = false;
                return 0;
            }
            unsigned char value = data[readIndex];
            readIndex += 1;
            *success = true;
            return value;
        }

        unsigned int decode_u16(bool* success) {
            if (available() < 2) {
                *success = false;
                return 0;
            }
            unsigned int value = data[readIndex] | (data[readIndex + 1] << 8);
            readIndex += 2;
            *success = true;
            return value;
        }

        unsigned long decode_u32(bool* success) {
            if (available() < 4) {
                *success = false;
                return 0;
            }
            unsigned long value = data[readIndex] | (data[readIndex + 1] << 8) |
                              (data[readIndex + 2] << 16) | (data[readIndex + 3] << 24);
            readIndex += 4;
            *success = true;
            return value;
        }

        unsigned long long decode_u64(bool* success) {
            if (available() < 8) {
                *success = false;
                return 0;
            }
            unsigned long long value = (unsigned long long)data[readIndex] | ((unsigned long long)data[readIndex + 1] << 8) |
                               ((unsigned long long)data[readIndex + 2] << 16) | ((unsigned long long)data[readIndex + 3] << 24) |
                               ((unsigned long long)data[readIndex + 4] << 32) | ((unsigned long long)data[readIndex + 5] << 40) |
                               ((unsigned long long)data[readIndex + 6] << 48) | ((unsigned long long)data[readIndex + 7] << 56);
            readIndex += 8;
            *success = true;
            return value;
        }

        bool decode_bool(bool* success) {
            if (available() < 1) {
                *success = false;
                return false;
            }
            bool value = data[readIndex] != 0;
            readIndex += 1;
            *success = true;
            return value;
        }

        float decode_f32(bool* success) {
            if (available() < 4) {
                *success = false;
                return 0;
            }
            float value;
            memcpy(&value, data + readIndex, 4);
            readIndex += 4;
            *success = true;
            return value;
        }

        double decode_f64(bool* success) {
            if (available() < 8) {
                *success = false;
                return 0;
            }
            double value;
            memcpy(&value, data + readIndex, 8);
            readIndex += 8;
            *success = true;
            return value;
        }

        char decode_i8 (bool* success) {
            return (char)decode_u8(success);
        }

        int decode_i16(bool* success) {
            return (int)decode_u16(success);
        }

        long decode_i32(bool* success) {
            return (long)decode_u32(success);
        }

        long long decode_i64(bool* success) {
            return (long long)decode_u64(success);
        }

        bool decode_u8Arr(unsigned char* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_u8(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        bool decode_i8Arr(char* result, unsigned int* result_len, unsigned int max_result) {
            return decode_u8Arr((unsigned char*)result, result_len, max_result);
        }

        bool decode_u16Arr(unsigned int* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_u16(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        bool decode_i16Arr(int* result, unsigned int* result_len, unsigned int max_result) {
            return decode_u16Arr((unsigned int*)result, result_len, max_result);
        }

        bool decode_u32Arr(unsigned long* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_u32(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        bool decode_i32Arr(long* result, unsigned int* result_len, unsigned int max_result) {
            return decode_u32Arr((unsigned long*)result, result_len, max_result);
        }

        bool decode_u64Arr(unsigned long long* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_u64(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        bool decode_i64Arr(long long* result, unsigned int* result_len, unsigned int max_result) {
            return decode_u64Arr((unsigned long long*)result, result_len, max_result);
        }

        bool decode_boolArr(bool* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_bool(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        bool decode_f32Arr(float* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_f32(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        bool decode_f64Arr(double* result, unsigned int* result_len, unsigned int max_result) {
            bool success = false;
            unsigned int arrlen = decode_u32(&success);
            if (!success) {
                return false;
            }
            result_len[0] = arrlen;
            for(unsigned int i = 0; i < arrlen; i++) {
                if (i >= max_result) {
                    return false;
                }
                result[i] = decode_f64(&success);
                if (!success) {
                    return false;
                }
            }
            return true;
        }

        void encode_u8(unsigned char value) {
            data[writeIndex] = value;
            writeIndex += 1;
        }

        void encode_u16(unsigned int value) {
            data[writeIndex] = value & 0xFF;
            data[writeIndex + 1] = (value >> 8) & 0xFF;
            writeIndex += 2;
        }

        void encode_u32(unsigned long value) {
            data[writeIndex] = value & 0xFF;
            data[writeIndex + 1] = (value >> 8) & 0xFF;
            data[writeIndex + 2] = (value >> 16) & 0xFF;
            data[writeIndex + 3] = (value >> 24) & 0xFF;
            writeIndex += 4;
        }

        void encode_u64(unsigned long long value) {
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

        void encode_i8(char value) {
            encode_u8((unsigned char)value);
        }

        void encode_i16(int value) {
            encode_u16((unsigned int)value);
        }

        void encode_i32(long value) {
            encode_u32((unsigned long)value);
        }

        void encode_i64(long long value) {
            encode_u64((unsigned long long)value);
        }

        void encode_u8Arr(unsigned char* value, unsigned int len) {
            encode_u32(len);
            memcpy(data + writeIndex, value, len);
            writeIndex += len;
        }

        void encode_i8Arr(char* value, unsigned int len) {
            encode_u8Arr((unsigned char*)value, len);
        }

        void encode_u16Arr(unsigned int* value, unsigned int len) {
            encode_u32(len);
            for (unsigned int i = 0; i < len; i++) {
                encode_u16(value[i]);
            }
        }

        void encode_i16Arr(int* value, unsigned int len) {
            encode_u16Arr((unsigned int*)value, len);
        }

        void encode_u32Arr(unsigned long* value, unsigned int len) {
            encode_u32(len);
            for (unsigned int i = 0; i < len; i++) {
                encode_u32(value[i]);
            }
        }

        void encode_i32Arr(long* value, unsigned int len) {
            encode_u32Arr((unsigned long*)value, len);
        }

        void encode_u64Arr(unsigned long long* value, unsigned int len) {
            encode_u32(len);
            for (unsigned int i = 0; i < len; i++) {
                encode_u64(value[i]);
            }
        }

        void encode_i64Arr(long long* value, unsigned int len) {
            encode_u64Arr((unsigned long long*)value, len);
        }

        void encode_boolArr(bool* value, unsigned int len) {
            encode_u32(len);
            for (unsigned int i = 0; i < len; i++) {
                encode_bool(value[i]);
            }
        }

        void encode_f32Arr(float* value, unsigned int len) {
            encode_u32(len);
            for (unsigned int i = 0; i < len; i++) {
                encode_f32(value[i]);
            }
        }

        void encode_f64Arr(double* value, unsigned int len) {
            encode_u32(len);
            for (unsigned int i = 0; i < len; i++) {
                encode_f64(value[i]);
            }
        }

        static Buffer from(unsigned char value) {
            Buffer buffer(1);
            buffer.encode_u8(value);
            return buffer;
        }

        static Buffer from(unsigned int value) {
            Buffer buffer(2);
            buffer.encode_u16(value);
            return buffer;
        }

        static Buffer from(unsigned long value) {
            Buffer buffer(4);
            buffer.encode_u32(value);
            return buffer;
        }

        static Buffer from(unsigned long long value) {
            Buffer buffer(8);
            buffer.encode_u64(value);
            return buffer;
        }

        static Buffer from(bool value) {
            Buffer buffer(1);
            buffer.encode_bool(value);
            return buffer;
        }

        static Buffer from(float value) {
            Buffer buffer(4);
            buffer.encode_f32(value);
            return buffer;
        }

        static Buffer from(double value) {
            Buffer buffer(8);
            buffer.encode_f64(value);
            return buffer;
        }

        static Buffer from(char value) {
            Buffer buffer(1);
            buffer.encode_i8(value);
            return buffer;
        }

        static Buffer from(int value) {
            Buffer buffer(2);
            buffer.encode_i16(value);
            return buffer;
        }

        static Buffer from(long value) {
            Buffer buffer(4);
            buffer.encode_i32(value);
            return buffer;
        }

        static Buffer from(long long value) {
            Buffer buffer(8);
            buffer.encode_i64(value);
            return buffer;
        }

        unsigned char* getData() {
            return data;
        }

        unsigned int getMaxLen() {
            return len;
        }

        unsigned int getLen() {
            return writeIndex;
        }
};

#endif