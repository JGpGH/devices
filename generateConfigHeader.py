class ConfigGenerator:
    wholeNumberTypes = ['int', 'uint8_t', 'uint16_t', 'uint32_t', 'uint64_t', 'byte', 'uint_fast8_t']
    decimalNumberTypes = ['float', 'double']
    stringTypes = ['String', 'char*']

    def __init__(self):
        self.variables = {}

    def variableToString(self, name, type, value):
        if isinstance(value, list):
            return f"{type} {name}[] = {{{','.join(value)}}};\n"
        else:
            return f"{type} {name} = {value};\n"

    def validateConfig(self, configVariables, configSchema):
        # check for variables that are not present in schema
        for variableName in configVariables:
            if variableName not in configSchema:
                print(f"config variable {variableName} is not in the config schema")
                return False
        # check that all required variables are present
        for variableName in configSchema:
            if variableName not in configVariables and configSchema[variableName]['required']:
                print(f"Missing required config variable {variableName}")
                return False
        # check that all variables are of the correct type
        for variableName in configSchema:
            if isinstance(configSchema[variableName]['default'], list) and not isinstance(configVariables[variableName], list):
                print(f"config variable {variableName} is not a list")
                return False
            variableType = configSchema[variableName]['type']
            if variableType in self.wholeNumberTypes:
                if not isinstance(configVariables[variableName], int):
                    print(f"config variable {variableName} is not an integer")
                    return False
            elif variableType in self.decimalNumberTypes:
                if not isinstance(configVariables[variableName], float):
                    print(f"config variable {variableName} is not a decimal number")
                    return False
            elif variableType in self.stringTypes:
                if not isinstance(configVariables[variableName], str):
                    print(f"config variable {variableName} is not a string")
                    return False
        return True

    def generate(self, outputPath, schema, configVariables):
        if not self.validateConfig(configVariables, schema):
            print("config schema validation failed - aborting generation")
            return
        with open(outputPath, 'w') as f:
            f.write("// This file is auto-generated\n")
            f.write("#ifndef CONFIG_H\n")
            f.write("#define CONFIG_H\n")
            [f.write(self.variableToString(variable, schema[variable]['type'], configVariables[variable])) for variable in configVariables]
            f.write("#endif\n")
    