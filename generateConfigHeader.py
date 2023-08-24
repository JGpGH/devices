class ConfigGenerator:
    wholeNumberTypes = ['int', 'uint8_t', 'uint16_t', 'uint32_t', 'uint64_t', 'byte', 'uint_fast8_t']
    decimalNumberTypes = ['float', 'double']
    stringTypes = ['String', 'char*']

    def __init__(self):
        self.variables = {}

    def variableToString(self, name, type, value):
        if isinstance(value, list):
            stringValue = ','.join([str(v) for v in value])
            return f"const {type.strip('[]')} {name}[] = {{{stringValue}}};\n"
        else:
            return f"const {type} {name} = {value};\n"

    def validateNonArrayType(self, variableType, variableName,  value):
            if variableType in self.wholeNumberTypes:
                if not isinstance(value, int):
                    print(f"config variable {variableName} must be an integer")
                    return False
            elif variableType in self.decimalNumberTypes:
                if not isinstance(value, float):
                    print(f"config variable {variableName} must be a decimal number")
                    return False
            elif variableType in self.stringTypes:
                if not isinstance(value, str):
                    print(f"config variable {variableName} must be a string")
                    return False
            return True
                
    def validateConfig(self, configVariables, configSchema):
        # check for variables that are not present in schema
        for variableName in configVariables:
            if variableName not in configSchema:
                print(f"config variable {variableName} is not in the config schema")
                return False
        # check that all variables are present
        for variableName in configSchema:
            if variableName not in configVariables:
                print(f"Missing config variable {variableName}")
                return False
        # check that all variables are of the correct type
        for variableName in configSchema:
            if '[]' in configSchema[variableName]['type']:
                if not isinstance(configVariables[variableName], list):
                    print(f"config variable {variableName} must be an array")
                    return False
                else:
                    variableType = configSchema[variableName]['type'].strip('[]')
                    for value in configVariables[variableName]:
                        if not self.validateNonArrayType(variableType, variableName, value):
                            return False
            else:
                variableType = configSchema[variableName]['type']
                if not self.validateNonArrayType(variableType, variableName, configVariables[variableName]):
                    return False
        return True

    def generate(self, outputPath, schema, configVariables):
        if not self.validateConfig(configVariables, schema):
            print("config schema validation failed - aborting generation")
            exit(1)
        with open(outputPath, 'w') as f:
            f.write("// This file is auto-generated\n")
            f.write("#ifndef CONFIG_H\n")
            f.write("#define CONFIG_H\n")
            [f.write(self.variableToString(variable, schema[variable]['type'], configVariables[variable])) for variable in configVariables]
            f.write("#endif\n")
    