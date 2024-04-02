#ifndef SERIALRPC_PROCEDURE_HPP
#define SERIALRPC_PROCEDURE_HPP

#include "codec.hpp"

//struct representing the result of a procedure call, contains the return value as a buffer of bytes and the error code as an int
struct ProcedureResult {
    Buffer buffer;
    int error;
};

struct Procedure {
    ProcedureResult (*procedurePtr)(Buffer);
};

enum error {
    NONE = 0,
    INVALID_PROCEDURE_ID = 1,
    INVALID_INPUT = 2,
    PROCEDURE_ERROR = 3
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

//calls a procedure with a buffer of bytes as input and returns a buffer of bytes as output
ProcedureResult callProcedure(ProcedureSet procedure_set, Buffer input_with_header) {
    bool success = true;
    unsigned char procedure_id = input_with_header.decode_u8(&success);
    if (!success) {
        return {Buffer(0), error::INVALID_INPUT};
    }
    if (procedure_id >= procedure_set.len) {
        return {Buffer(0), error::INVALID_PROCEDURE_ID};
    }
    Procedure procedure = procedure_set.procedures[procedure_id];

    return procedure.procedurePtr(input_with_header);
}

#endif