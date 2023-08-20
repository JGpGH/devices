import os
import sys
import json
from generateConfigHeader import ConfigGenerator

def buildAndRun(program, fbqn, port):
    programPath = os.path.join(os.getcwd(), 'programs', program)
    os.system(f"arduino-cli compile --fqbn {fbqn} {programPath}")
    os.system(f"arduino-cli upload -p {port} --fqbn {fbqn} {programPath}")

def loadConfiguration(configId):
    with open(os.path.join(os.getcwd(), 'configs', f'{configId}.json')) as json_file:
        data = json.load(json_file)
        if data['fbqn'] is None:
            print("fbqn is not set")
            return
        if data['program'] is None:
            print("program is not set")
            return
        
        if not data['program'] in os.listdir(os.path.join(os.getcwd(), 'programs')):
            print(f"program {data['program']} does not exist")
            return
        return data

def loadConfigProgramSchema(programeName):
    with open(os.path.join(os.getcwd(), 'programs', programeName, 'config-schema.json')) as json_file:
        data = json.load(json_file)
        return data
    
def generateConfigHeader(configVariables, programName):
    configHeaderPath = os.path.join(os.getcwd(), 'programs', programName, 'config.h')
    schema = loadConfigProgramSchema(programName)
    ConfigGenerator().generate(configHeaderPath, schema, configVariables)

def configExists(configId):
    return os.path.exists(os.path.join(os.getcwd(), 'configs', f'{configId}.json'))

def main():
    if len(sys.argv) < 3:
        print("Please provide a config id and a port")
        return
    configId = sys.argv[1]
    port = sys.argv[2]

    if not configExists(configId):
        print("Invalid config id")
        return
    config = loadConfiguration(configId)
    print(f'generating config variables header for {config["program"]}')
    generateConfigHeader(config['vars'], config['program'])
    print(f"Building and running {configId} on {port}")
    buildAndRun(config['program'], config['fbqn'], port)

if __name__ == "__main__":
    main()