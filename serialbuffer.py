import struct
from enum import Enum

class Type(Enum):
    U8 = 0
    U16 = 1
    U32 = 2
    U64 = 3
    I8 = 4
    I16 = 5
    I32 = 6
    I64 = 7
    F32 = 8
    F64 = 9
    BOOL = 10
    U8ARR = 11
    U16ARR = 12
    U32ARR = 13
    U64ARR = 14
    I8ARR = 15
    I16ARR = 16
    I32ARR = 17
    I64ARR = 18
    F32ARR = 19
    F64ARR = 20

class Buffer:
    def __init__(self, data, len):
        self.len = len
        self.readIndex = 0
        self.writeIndex = 0
        self.data = bytearray(data) if isinstance(data, bytes) else bytearray(len)

    def reset(self):
        self.readIndex = 0
        self.writeIndex = 0

    def available(self):
        return self.writeIndex - self.readIndex

    def skip(self, length):
        self.readIndex += length

    def append(self, data, length):
        self.data[self.writeIndex:self.writeIndex + length] = data
        self.writeIndex += length

    def append_byte(self, data):
        self.data[self.writeIndex] = data
        self.writeIndex += 1

    def decode_u8(self):
        if self.available() < 1:
            return None, False
        value = self.data[self.readIndex]
        self.readIndex += 1
        return value, True

    def decode_u16(self):
        if self.available() < 2:
            return None, False
        value = self.data[self.readIndex] | (self.data[self.readIndex + 1] << 8)
        self.readIndex += 2
        return value, True

    def decode_u32(self):
        if self.available() < 4:
            return None, False
        value = self.data[self.readIndex] | (self.data[self.readIndex + 1] << 8) | \
                (self.data[self.readIndex + 2] << 16) | (self.data[self.readIndex + 3] << 24)
        self.readIndex += 4
        return value, True

    def decode_u64(self):
        if self.available() < 8:
            return None, False
        value = int.from_bytes(self.data[self.readIndex:self.readIndex + 8], byteorder='little')
        self.readIndex += 8
        return value, True

    def decode_bool(self):
        if self.available() < 1:
            return None, False
        value = self.data[self.readIndex] != 0
        self.readIndex += 1
        return value, True

    def decode_f32(self):
        if self.available() < 4:
            return None, False
        value = struct.unpack('<f', self.data[self.readIndex:self.readIndex + 4])[0]
        self.readIndex += 4
        return value, True

    def decode_f64(self):
        if self.available() < 8:
            return None, False
        value = struct.unpack('<d', self.data[self.readIndex:self.readIndex + 8])[0]
        self.readIndex += 8
        return value, True

    def decode_i8(self):
        value, success = self.decode_u8()
        return value if success else None, success

    def decode_i16(self):
        value, success = self.decode_u16()
        return value if success else None, success

    def decode_i32(self):
        value, success = self.decode_u32()
        return value if success else None, success

    def decode_i64(self):
        value, success = self.decode_u64()
        return value if success else None, success

    def encode_u8(self, value):
        self.data[self.writeIndex] = value
        self.writeIndex += 1

    def encode_u16(self, value):
        self.data[self.writeIndex] = value & 0xFF
        self.data[self.writeIndex + 1] = (value >> 8) & 0xFF
        self.writeIndex += 2

    def encode_u32(self, value):
        self.data[self.writeIndex] = value & 0xFF
        self.data[self.writeIndex + 1] = (value >> 8) & 0xFF
        self.data[self.writeIndex + 2] = (value >> 16) & 0xFF
        self.data[self.writeIndex + 3] = (value >> 24) & 0xFF
        self.writeIndex += 4

    def encode_u64(self, value):
        self.data[self.writeIndex:self.writeIndex + 8] = value.to_bytes(8, byteorder='little')
        self.writeIndex += 8

    def encode_bool(self, value):
        self.data[self.writeIndex] = 1 if value else 0
        self.writeIndex += 1

    def encode(self, value, type):
        if type == Type.U8:
            self.encode_u8(value)
        elif type == Type.U16:
            self.encode_u16(value)
        elif type == Type.U32:
            self.encode_u32(value)
        elif type == Type.U64:
            self.encode_u64(value)
        elif type == Type.BOOL:
            self.encode_bool(value)
        elif type == Type.F32:
            self.encode_f32(value)
        elif type == Type.F64:
            self.encode_f64(value)
        elif type == Type.I8:
            self.encode_i8(value)
        elif type == Type.I16:
            self.encode_i16(value)
        elif type == Type.I32:
            self.encode_i32(value)
        elif type == Type.I64:
            self.encode_i64(value)

    def decode(self, type):
        if type == Type.U8:
            return self.decode_u8()
        elif type == Type.U16:
            return self.decode_u16()
        elif type == Type.U32:
            return self.decode_u32()
        elif type == Type.U64:
            return self.decode_u64()
        elif type == Type.BOOL:
            return self.decode_bool()
        elif type == Type.F32:
            return self.decode_f32()
        elif type == Type.F64:
            return self.decode_f64()
        elif type == Type.I8:
            return self.decode_i8()
        elif type == Type.I16:
            return self.decode_i16()
        elif type == Type.I32:
            return self.decode_i32()
        elif type == Type.I64:
            return self.decode_i64()

    def encode_f32(self, value):
        self.data[self.writeIndex:self.writeIndex + 4] = struct.pack('<f', value)
        self.writeIndex += 4

    def encode_f64(self, value):
        self.data[self.writeIndex:self.writeIndex + 8] = struct.pack('<d', value)
        self.writeIndex += 8

    def encode_i8(self, value):
        self.encode_u8(value)

    def encode_i16(self, value):
        self.encode_u16(value)

    def encode_i32(self, value):
        self.encode_u32(value)

    def encode_i64(self, value):
        self.encode_u64(value)

    @staticmethod
    def from_byte(value):
        buffer = Buffer(b'', 1)
        buffer.encode_u8(value)
        return buffer

    @staticmethod
    def from_u16(value):
        buffer = Buffer(b'', 2)
        buffer.encode_u16(value)
        return buffer

    @staticmethod
    def from_u32(value):
        buffer = Buffer(b'', 4)
        buffer.encode_u32(value)
        return buffer

    @staticmethod
    def from_u64(value):
        buffer = Buffer(b'', 8)
        buffer.encode_u64(value)
        return buffer

    @staticmethod
    def from_bool(value):
        buffer = Buffer(b'', 1)
        buffer.encode_bool(value)
        return buffer

    @staticmethod
    def from_f32(value):
        buffer = Buffer(b'', 4)
        buffer.encode_f32(value)
        return buffer

    @staticmethod
    def from_f64(value):
        buffer = Buffer(b'', 8)
        buffer.encode_f64(value)
        return buffer

    @staticmethod
    def from_i8(value):
        return Buffer.from_byte(value)

    @staticmethod
    def from_i16(value):
        return Buffer.from_u16(value)

    @staticmethod
    def from_i32(value):
        return Buffer.from_u32(value)

    @staticmethod
    def from_i64(value):
        return Buffer.from_u64(value)

    def get_data(self):
        return self.data

    def get_max_len(self):
        return self.len

    def get_len(self):
        return self.writeIndex