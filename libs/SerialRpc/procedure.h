struct Buffer {
    byte* data;
    unsigned long len;
    unsigned int readIndex;
    unsigned int writeIndex;
};

//A procedure is a function that takes a buffer of bytes and returns a buffer of bytes
typedef Buffer (*Procedure)(Buffer);

//struct representing the result of a procedure call, contains the return value as a buffer of bytes and the error code as an int
struct ProcedureResult {
    Buffer buffer;
    int error;
};

//struct representing an aggregation of procedures, contains an array of procedures and the total length of the array
struct ProcedureSet {
    Procedure* procedures;
    unsigned int len;
};

ProcedureSet procedureSetFromArray(Procedure* procedures, unsigned int len) {
    ProcedureSet set;
    set.procedures = procedures;
    set.len = len;
    return set;
}


