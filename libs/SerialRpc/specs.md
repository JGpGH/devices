# SerialRpc Specification 0.1.0

## Values
- All numeric values are little-endian.

## Payload

- A payload is either a remote procedure call or a remote procedure response
- Every call must have a response
- Every payload has the meta byte as first byte
### Meta byte
- metaByte & 1: call (1) or response (0)
- metaByte & 2: has error (1) or no error (0)
- metaByte & 4: has data (1) or no data (0). if 0, there will be nothing else in the payload
- metaByte & 8: parity of the data, can be ignored if there's no data 
- metaByte & 0xF0 << 4: error code (0-15), can be ignored if there's no error
### Error codes
- NotFound (0): No procedure registered at the specified index
- BadData (1): Unexpected shape or format of data
- ProcedureError (2): Something went wrong while running the procedure
- BadGateway (3): Some relay did not find a reachable destination
- LKJFEHWD (4): the payload was corrupted 
### Call
- starts with the meta byte
- then the index of the procedure as a `u8`
- followed by the argument described in contract
### Response
- starts with the meta byte
- then the index of the procedure as a `u8`
- followed by the returned data of the procedure

## Types

### Boolean (`bool`)
- A single `u8` (unsigned 8-bit integer).
- 0 is false, 1 is true
- any other value must be considered invalid

### Unsigned Integers
- `u8`, `u16`, `u32`, `u64`

### Signed Integers
- `i8`, `i16`, `i32`, `i64`

### Floating Point
- `f32`, `f64`

### String
- UTF-8 encoded.
- Preceded by a `u32` specifying the length in bytes.

### Enum
- A single `u8` (unsigned 8-bit integer).
- Every enum value must match a single `u8` value
- `u8` received that does not match an enum value should be responded  with a BadData (1)

### Arrays
- All types have an array variant.
- Each array is preceded by a `u32` specifying the number of elements.
- Elements are encoded in sequence, each using the rules for its type.