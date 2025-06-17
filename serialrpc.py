import serial
import struct
import json
import re

# --- SerialRpc constants (from specs.md) ---
SERIALRPC_ERR_NOTFOUND = 0
SERIALRPC_ERR_BADDATA = 1
SERIALRPC_ERR_PROCERR = 2
SERIALRPC_ERR_BADGATEWAY = 3
SERIALRPC_ERR_CORRUPT = 4

# Meta byte flags
METABYTE_CALL = 0x01
METABYTE_ERROR = 0x02
METABYTE_DATA = 0x04
METABYTE_PARITY = 0x08
METABYTE_ERRCODE_MASK = 0xF0
METABYTE_ERRCODE_SHIFT = 4

# --- Meta byte helpers ---
def metabyte_from_block(is_call, has_error, has_data, parity, error_code):
    meta = 0
    if is_call: meta |= METABYTE_CALL
    if has_error: meta |= METABYTE_ERROR
    if has_data: meta |= METABYTE_DATA
    if parity: meta |= METABYTE_PARITY
    meta |= ((error_code & 0x0F) << METABYTE_ERRCODE_SHIFT)
    return meta

def metabyte_to_block(meta):
    return {
        'is_call': bool(meta & METABYTE_CALL),
        'has_error': bool(meta & METABYTE_ERROR),
        'has_data': bool(meta & METABYTE_DATA),
        'parity': bool(meta & METABYTE_PARITY),
        'error_code': (meta & METABYTE_ERRCODE_MASK) >> METABYTE_ERRCODE_SHIFT
    }

# --- Encoding helpers ---
def encode_u8(val): return struct.pack('<B', val)
def encode_u16(val): return struct.pack('<H', val)
def encode_u32(val): return struct.pack('<I', val)
def encode_u64(val): return struct.pack('<Q', val)
def encode_bool(val): return struct.pack('<B', 1 if val else 0)
def encode_string(s):
    b = s.encode('utf-8')
    return encode_u32(len(b)) + b
def encode_array(arr, encode_elem):
    return encode_u32(len(arr)) + b''.join(encode_elem(x) for x in arr)

def decode_u8(b, o=0): return struct.unpack_from('<B', b, o)[0], o+1
def decode_u16(b, o=0): return struct.unpack_from('<H', b, o)[0], o+2
def decode_u32(b, o=0): return struct.unpack_from('<I', b, o)[0], o+4
def decode_u64(b, o=0): return struct.unpack_from('<Q', b, o)[0], o+8
def decode_bool(b, o=0): v, o = decode_u8(b, o); return bool(v), o
def decode_string(b, o=0):
    l, o = decode_u32(b, o)
    s = b[o:o+l].decode('utf-8')
    return s, o+l
def decode_array(b, o, decode_elem):
    l, o = decode_u32(b, o)
    arr = []
    for _ in range(l):
        v, o = decode_elem(b, o)
        arr.append(v)
    return arr, o

# --- SerialRpc class ---
class SerialRpc:
    def __init__(self, port, baudrate=115200, timeout=1):
        self.ser = serial.Serial(port, baudrate, timeout=timeout)

    def call(self, proc_idx, args_bytes):
        # Build payload
        meta = metabyte_from_block(True, False, True, False, 0)
        payload = bytes([meta]) + encode_u8(proc_idx) + encode_u16(len(args_bytes)) + args_bytes
        self.ser.write(payload)
        # Read response meta and proc_idx
        meta = self.ser.read(1)
        if not meta:
            raise TimeoutError('No response meta byte')
        meta = meta[0]
        mb = metabyte_to_block(meta)
        idx = self.ser.read(1)
        if not idx:
            raise TimeoutError('No response proc_idx')
        idx = idx[0]
        if mb['has_data']:
            len_bytes = self.ser.read(2)
            if len(len_bytes) < 2:
                raise TimeoutError('No response data length')
            data_len = struct.unpack('<H', len_bytes)[0]
            data = self.ser.read(data_len)
            if len(data) < data_len:
                raise TimeoutError('Incomplete response data')
        else:
            data = b''
        return mb, idx, data

    def close(self):
        self.ser.close()

def parse_procedures_from_schema(schema_path):
    with open(schema_path) as f:
        schema = json.load(f)
    proc_map = {}
    for idx, proc in enumerate(schema.get("procedures", [])):
        # Example: "u32 add(u32 a, u32 b)"
        m = re.match(r"(\w+)\s+(\w+)\((.*)\)", proc)
        if not m:
            continue
        ret_type, name, args = m.groups()
        arg_types = []
        if args.strip():
            for arg in args.split(','):
                t, n = arg.strip().split()[:2]
                arg_types.append(t)
        proc_map[name] = {'idx': idx, 'arg_types': arg_types, 'ret_type': ret_type}
    return proc_map

def encode_arg(val, typ):
    if typ == 'u8': return encode_u8(int(val))
    if typ == 'u16': return encode_u16(int(val))
    if typ == 'u32': return encode_u32(int(val))
    if typ == 'u64': return encode_u64(int(val))
    if typ == 'i8': return encode_u8(int(val))
    if typ == 'i16': return encode_u16(int(val))
    if typ == 'i32': return encode_u32(int(val))
    if typ == 'i64': return encode_u64(int(val))
    if typ == 'bool': return encode_bool(bool(int(val)))
    if typ == 'string': return encode_string(val)
    raise ValueError(f"Unknown type: {typ}")

# Example usage:
# rpc = SerialRpc('/dev/ttyUSB0')
# args = encode_u32(1) + encode_u32(2)
# mb, idx, data = rpc.call(0, args)
# if not mb['has_error']:
#     result, _ = decode_u32(data)
#     print('Result:', result)
# else:
#     print('Error code:', mb['error_code'])

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="SerialRpc client")
    parser.add_argument("port", help="Serial port, e.g. /dev/ttyUSB0 or COM3")
    parser.add_argument("schema", help="Path to config-schema.json")
    parser.add_argument("proc", help="Procedure name to call")
    parser.add_argument("args", nargs="*", help="Arguments as strings")
    parser.add_argument("--baud", type=int, default=115200, help="Baudrate (default: 115200)")
    args = parser.parse_args()

    proc_map = parse_procedures_from_schema(args.schema)
    if args.proc not in proc_map:
        print(f"Procedure {args.proc} not found in schema.")
        exit(1)
    proc_info = proc_map[args.proc]
    if len(args.args) != len(proc_info['arg_types']):
        print(f"Procedure {args.proc} expects {len(proc_info['arg_types'])} arguments: {proc_info['arg_types']}")
        exit(1)
    arg_bytes = b''.join(encode_arg(val, typ) for val, typ in zip(args.args, proc_info['arg_types']))
    rpc = SerialRpc(args.port, baudrate=args.baud)
    try:
        mb, idx, data = rpc.call(proc_info['idx'], arg_bytes)
        print(f"Response meta: {mb}, proc_idx: {idx}")
        if not mb['has_error']:
            if data:
                # Only handle u32 return for now
                if proc_info['ret_type'] == 'u32':
                    result, _ = decode_u32(data)
                    print(f"Result: {result}")
                else:
                    print(f"Raw data: {data.hex()}")
            else:
                print("No data returned.")
        else:
            print(f"Error code: {mb['error_code']}")
    finally:
        rpc.close()





