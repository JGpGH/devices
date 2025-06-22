#ifndef CODEC_HPP
#define CODEC_HPP

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// --- Structs for parsed data types ---
typedef struct {
    uint8_t* data;
    uint32_t len;
} ByteArray;

typedef struct {
    char* data;
    uint32_t len;
} RpcString;

// --- Parsing functions ---

// Helper: read little-endian unsigned integers from buffer
typedef struct {
    const uint8_t* buf;
    uint16_t size;
    uint16_t pos;
} BufferView;

static inline bool bv_read_u8(BufferView* bv, uint8_t* out) {
    if (bv->pos + 1 > bv->size) return false;
    *out = bv->buf[bv->pos++];
    return true;
}

static inline bool bv_read_u16(BufferView* bv, uint16_t* out) {
    if (bv->pos + 2 > bv->size) return false;
    *out = bv->buf[bv->pos] | (bv->buf[bv->pos+1] << 8);
    bv->pos += 2;
    return true;
}

static inline bool bv_read_u32(BufferView* bv, uint32_t* out) {
    if (bv->pos + 4 > bv->size) return false;
    *out = bv->buf[bv->pos] | (bv->buf[bv->pos+1] << 8) | (bv->buf[bv->pos+2] << 16) | (bv->buf[bv->pos+3] << 24);
    bv->pos += 4;
    return true;
}

static inline bool bv_read_u64(BufferView* bv, uint64_t* out) {
    if (bv->pos + 8 > bv->size) return false;
    *out = 0;
    for (int i = 0; i < 8; ++i) {
        *out |= ((uint64_t)bv->buf[bv->pos + i]) << (8 * i);
    }
    bv->pos += 8;
    return true;
}

static inline bool bv_read_i8(BufferView* bv, int8_t* out) {
    return bv_read_u8(bv, (uint8_t*)out);
}

static inline bool bv_read_i16(BufferView* bv, int16_t* out) {
    return bv_read_u16(bv, (uint16_t*)out);
}

static inline bool bv_read_i32(BufferView* bv, int32_t* out) {
    return bv_read_u32(bv, (uint32_t*)out);
}

static inline bool bv_read_i64(BufferView* bv, int64_t* out) {
    return bv_read_u64(bv, (uint64_t*)out);
}

static inline bool bv_read_f32(BufferView* bv, float* out) {
    uint32_t tmp;
    if (!bv_read_u32(bv, &tmp)) return false;
    *out = *((float*)&tmp);
    return true;
}

static inline bool bv_read_f64(BufferView* bv, double* out) {
    uint64_t tmp;
    if (!bv_read_u64(bv, &tmp)) return false;
    *out = *((double*)&tmp);
    return true;
}

static inline bool bv_read_bool(BufferView* bv, bool* out) {
    uint8_t tmp;
    if (!bv_read_u8(bv, &tmp)) return false;
    *out = tmp ? true : false;
    return true;
}

// RpcString: u32 length + utf-8 bytes
static inline bool bv_read_string(BufferView* bv, RpcString* out) {
    uint32_t len;
    if (!bv_read_u32(bv, &len)) return false;
    if (bv->pos + len > bv->size) return false;
    out->data = (char*)&bv->buf[bv->pos];
    out->len = len;
    bv->pos += len;
    return true;
}

// Array: u32 length + elements
static inline bool bv_read_u8_array(BufferView* bv, ByteArray* out) {
    uint32_t len;
    if (!bv_read_u32(bv, &len)) return false;
    if (bv->pos + len > bv->size) return false;
    out->data = (uint8_t*)&bv->buf[bv->pos];
    out->len = len;
    bv->pos += len;
    return true;
}

// Enum: decoded from u8
static inline bool bv_read_enum(BufferView* bv, uint8_t* out) {
    return bv_read_u8(bv, out);
}

// --- Write functions ---
static inline bool bv_write_u8(uint8_t* buf, uint16_t buf_size, uint16_t* pos, uint8_t val) {
    if (*pos + 1 > buf_size) return false;
    buf[(*pos)++] = val;
    return true;
}

static inline bool bv_write_u16(uint8_t* buf, uint16_t buf_size, uint16_t* pos, uint16_t val) {
    if (*pos + 2 > buf_size) return false;
    buf[(*pos)++] = val & 0xFF;
    buf[(*pos)++] = (val >> 8) & 0xFF;
    return true;
}

static inline bool bv_write_u32(uint8_t* buf, uint16_t buf_size, uint16_t* pos, uint32_t val) {
    if (*pos + 4 > buf_size) return false;
    buf[(*pos)++] = val & 0xFF;
    buf[(*pos)++] = (val >> 8) & 0xFF;
    buf[(*pos)++] = (val >> 16) & 0xFF;
    buf[(*pos)++] = (val >> 24) & 0xFF;
    return true;
}

static inline bool bv_write_u64(uint8_t* buf, uint16_t buf_size, uint16_t* pos, uint64_t val) {
    if (*pos + 8 > buf_size) return false;
    for (int i = 0; i < 8; ++i) {
        buf[(*pos)++] = (val >> (8 * i)) & 0xFF;
    }
    return true;
}

static inline bool bv_write_i8(uint8_t* buf, uint16_t buf_size, uint16_t* pos, int8_t val) {
    return bv_write_u8(buf, buf_size, pos, (uint8_t)val);
}

static inline bool bv_write_i16(uint8_t* buf, uint16_t buf_size, uint16_t* pos, int16_t val) {
    return bv_write_u16(buf, buf_size, pos, (uint16_t)val);
}

static inline bool bv_write_i32(uint8_t* buf, uint16_t buf_size, uint16_t* pos, int32_t val) {
    return bv_write_u32(buf, buf_size, pos, (uint32_t)val);
}

static inline bool bv_write_i64(uint8_t* buf, uint16_t buf_size, uint16_t* pos, int64_t val) {
    return bv_write_u64(buf, buf_size, pos, (uint64_t)val);
}

static inline bool bv_write_f32(uint8_t* buf, uint16_t buf_size, uint16_t* pos, float val) {
    uint32_t tmp;
    memcpy(&tmp, &val, sizeof(float));
    return bv_write_u32(buf, buf_size, pos, tmp);
}

static inline bool bv_write_f64(uint8_t* buf, uint16_t buf_size, uint16_t* pos, double val) {
    uint64_t tmp;
    memcpy(&tmp, &val, sizeof(double));
    return bv_write_u64(buf, buf_size, pos, tmp);
}

static inline bool bv_write_bool(uint8_t* buf, uint16_t buf_size, uint16_t* pos, bool val) {
    return bv_write_u8(buf, buf_size, pos, val ? 1 : 0);
}

static inline bool bv_write_bytes(uint8_t* buf, uint16_t buf_size, uint16_t* pos, const uint8_t* data, uint16_t data_len) {
    if (*pos + data_len > buf_size) return false;
    memcpy(&buf[*pos], data, data_len);
    *pos += data_len;
    return true;
}

static inline bool bv_write_string(uint8_t* buf, uint16_t buf_size, uint16_t* pos, const char* str, uint32_t len) {
    if (!bv_write_u32(buf, buf_size, pos, len)) return false;
    return bv_write_bytes(buf, buf_size, pos, (const uint8_t*)str, len);
}

// --- Meta byte block helpers ---
#define METABYTE_CALL      0x01
#define METABYTE_ERROR     0x02
#define METABYTE_DATA      0x04
#define METABYTE_PARITY    0x08
#define METABYTE_ERRCODE_MASK 0xF0
#define METABYTE_ERRCODE_SHIFT 4

typedef struct {
    bool is_call;
    bool has_error;
    bool has_data;
    bool parity;
    uint8_t error_code; // 0-15
} MetaBlock;

static inline uint8_t metabyte_from_block(const MetaBlock* mb) {
    uint8_t meta = 0;
    if (mb->is_call) meta |= METABYTE_CALL;
    if (mb->has_error) meta |= METABYTE_ERROR;
    if (mb->has_data) meta |= METABYTE_DATA;
    if (mb->parity) meta |= METABYTE_PARITY;
    meta |= ((mb->error_code & 0x0F) << METABYTE_ERRCODE_SHIFT);
    return meta;
}

static inline void metabyte_to_block(uint8_t meta, MetaBlock* mb) {
    mb->is_call = (meta & METABYTE_CALL) != 0;
    mb->has_error = (meta & METABYTE_ERROR) != 0;
    mb->has_data = (meta & METABYTE_DATA) != 0;
    mb->parity = (meta & METABYTE_PARITY) != 0;
    mb->error_code = (meta & METABYTE_ERRCODE_MASK) >> METABYTE_ERRCODE_SHIFT;
}

#endif // CODEC_HPP
