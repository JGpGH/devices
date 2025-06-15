#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import json
import getopt
from generateConfigHeader import ConfigGenerator
from ensureInstalled import CLIPATH
from generateConfigHeader import Procedure

def verboseCommand(command):
    print(command)
    return os.system(command)

def build(config):
    program = config['program']
    fbqn = config['fbqn']
    print(f'generating config variables header for {config["program"]}')
    generateConfigHeader(config['vars'], config['program'])
    programPath = os.path.join(os.getcwd(), 'programs', program)
    libPath = os.path.join(os.getcwd(), 'libs')
    command = f"{CLIPATH} compile --fqbn {fbqn} --libraries {libPath} {programPath}"
    return verboseCommand(command)

def upload(config, port):
    program = config['program']
    fbqn = config['fbqn']
    programPath = os.path.join(os.getcwd(), 'programs', program)
    command = f"{CLIPATH} upload --fqbn {fbqn} -p {port} {programPath}"
    return verboseCommand(command)

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

def loadConfigSchema(programeName):
    with open(os.path.join(os.getcwd(), 'programs', programeName, 'config-schema.json')) as json_file:
        data = json.load(json_file)
        return data

def generateConfigHeader(configVariables, programName):
    configHeaderPath = os.path.join(os.getcwd(), 'programs', programName, 'config.h')
    schema = loadConfigSchema(programName)
    if 'procedures' not in schema:
        schema['procedures'] = []
    if 'config' not in schema:
        schema['config'] = {}

    procedures = []
    for procedure in schema['procedures']:
        procedures.append(Procedure.fromstr(procedure))
    ConfigGenerator().generate(configHeaderPath, schema['config'], configVariables, procedures)

def configExists(configId):
    return os.path.exists(os.path.join(os.getcwd(), 'configs', f'{configId}.json'))

def buildAndUpload():
    configId = None
    port = None
    buildOnly = False
    try:
        opts, args = getopt.getopt(sys.argv[1:], "c:p:b", ["config=", "port=", "build"])
        for opt, arg in opts:
            if opt in ("-c", "--config"):
                configId = arg
            elif opt in ("-p", "--port"):
                port = arg
            elif opt in ("-b", "--build"):
                buildOnly = True
        for arg in args:
            if configId is None:
                configId = arg
            elif port is None:
                port = arg
    except getopt.GetoptError:
        print('buildAndRun.py -c <configId> -p <port>')
        sys.exit(2)

    if not configExists(configId):
        print("Invalid config id")
        return
    config = loadConfiguration(configId)
    if config is None:
        print("Invalid configuration")
        return
    if port is None:
        print("Port is not set")
        return
    print(f"Building {configId}")
    if build(config) != 0:
        print(f"Build failed for {configId}")
        return
    if not buildOnly:
        print(f"running {configId} on {port}")
        upload(config, port)

if __name__ == "__main__":
    buildAndUpload()