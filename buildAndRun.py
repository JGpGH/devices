import os
import sys
import json

def buildAndRun(program, fbqn, port):
    programPath = os.path.join(os.getcwd(), 'programs', program)
    os.system(f"arduino-cli compile --fqbn {fbqn} {programPath}")
    os.system(f"arduino-cli upload -p {port} --fqbn {fbqn} {programPath}")

def loadConfigurations():
    with open('configs.json') as json_file:
        data = json.load(json_file)
        return data

def main():
    configs = loadConfigurations()
    if len(sys.argv) < 3:
        print("Please provide a config id and a port")
        return
    configId = sys.argv[1]
    port = sys.argv[2]
    if configId not in configs:
        print("Invalid config id")
        return
    config = configs[configId]
    print(f"Building and running {config['name']} on {port}")
    buildAndRun(config['program'], config['fbqn'], port)

if __name__ == "__main__":
    main()