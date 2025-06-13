import re
from color import Error, Warning, Info, Success

class Procedure:
    def __init__(self, name: str, returnType: str, args: list[tuple[str, str]]):
        self.name = name
        self.returnType = returnType
        self.args = args

    def __str__(self):
        return f"{self.returnType} {self.name}({', '.join([f'{argType} {argName}' for argType, argName in self.args])});"
    
    @classmethod
    def fromstr(cls, string: str):
        # Regular expression to match the procedure format
        pattern = re.compile(r'(\w+)\s+(\w+)\((.*)\)')
        match = pattern.match(string)
        if not match:
            raise ValueError("String does not match the expected format")

        returnType, name, args_str = match.groups()
        # Split the arguments string by comma and strip spaces
        args = []
        if args_str:  # Check if the args string is not empty
            for arg in args_str.split(','):
                argType, argName = arg.strip().split()
                args.append((argType, argName))

        return cls(name, returnType, args)
    
    def validate(self):
        if not isTypeValid(self.returnType):
            raise ValueError(f"invalid return type {self.returnType}")
        for argType, _ in self.args:
            if not isTypeValid(argType):
                raise ValueError(f"invalid argument type {argType}")
        return True

def isTypeValid(type: str):
    return type in ConfigGenerator.wholeNumberTypes

class ConfigGenerator:
    wholeNumberTypes = ['u8', 'u16', 'u32', 'u64', 'f32', 'f64', 'bool', 'i8', 'i16', 'i32', 'i64', 'bool']
    c_types = {
        'u8': 'uint8_t',
        'u16': 'uint16_t',
        'u32': 'uint32_t',
        'u64': 'uint64_t',
        'f32': 'float',
        'f64': 'double',
        'bool': 'bool',
        'i8': 'int8_t',
        'i16': 'int16_t',
        'i32': 'int32_t',
        'i64': 'int64_t',
        'bool': 'bool'
    }
    def __init__(self):
        self.variables = {}

    def to_c_type(self, t):
        return self.c_types[t]

    def variableToString(self, name, type, value):
        if isinstance(value, list):
            stringValue = ','.join([str(v) for v in value])
            return f"const {self.to_c_type(type.strip('[]'))} {name}[] = {{{stringValue}}};\n"
        else:
            return f"const {self.to_c_type(type)} {name} = {value};\n"

    def validateNonArrayType(self, variableType, variableName,  value):
            if variableType in self.wholeNumberTypes:
                if not isinstance(value, int):
                    print(Error(f"config variable {variableName} must be an integer"))
                    return False
            return True
                
    def validateConfig(self, configVariables, configSchema, prodecures: list[Procedure]):
        for procedure in prodecures:
            procedure.validate()
        # check for variables that are not present in schema
        for variableName in configVariables:
            if variableName not in configSchema:
                raise ValueError(f"config variable {variableName} is not in the config schema")
        # check that all variables are present
        for configName in configSchema:
            if configName not in configVariables:
                if configSchema[configName]['required']:
                    raise ValueError(f"Missing config variable {configName}")
                configVariables[configName] = configSchema[configName]['default']
        # check that all variables are of the correct type
        for variableName in configSchema:
            if '[]' in configSchema[variableName]['type']:
                if not isinstance(configVariables[variableName], list):
                    raise ValueError(f"config variable {variableName} must be an array")
                else:
                    variableType = configSchema[variableName]['type'].strip('[]')
                    for value in configVariables[variableName]:
                        if not self.validateNonArrayType(variableType, variableName, value):
                            raise ValueError(f"config variable {variableName} must be of type {variableType}")
            else:
                variableType = configSchema[variableName]['type']
                if not self.validateNonArrayType(variableType, variableName, configVariables[variableName]):
                    raise ValueError(f"config variable {variableName} must be of type {variableType}")
        return True

    def generate(self, outputPath, configSchema, configVariables, procedures: list[Procedure]):
        if not self.validateConfig(configVariables, configSchema, procedures):
            raise ValueError("config schema validation failed - aborting generation")
        with open(outputPath, 'w') as f:
            f.write("// This file is auto-generated\n")
            f.write("#ifndef CONFIG_H\n")
            f.write("#define CONFIG_H\n")
            if len(procedures) > 0:
                f.write("#include \"procedure.hpp\"\n")

            [f.write(self.variableToString(variable, configSchema[variable]['type'], configVariables[variable])) for variable in configVariables]

            for procedure in procedures:
                f.write(f"{self.to_c_type(procedure.returnType)} {procedure.name}({', '.join([f'{self.to_c_type(argType)} {argName}' for argType, argName in procedure.args])});\n")
                f.write(f"ProcedureResult rpc_{procedure.name}(Buffer buff) {{\n")
                f.write(f"    {self.to_c_type(procedure.returnType)} result;\n")
                f.write("    bool success = true;\n")
                for argType, argName in procedure.args:
                    f.write(f"    {self.to_c_type(argType)} {argName} = buff.decode_{argType}(&success);\n")
                    f.write("    if (!success) {\n")
                    f.write(f"        return {{Buffer(0), error::INVALID_INPUT}};\n")
                    f.write("    }\n")
                f.write(f"    result = {procedure.name}({', '.join([arg[1] for arg in procedure.args])});\n")
                f.write("    return {Buffer::from(result), error::NONE};\n")
                f.write("}\n")
            f.write("#endif\n")
    