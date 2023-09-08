import os

def createBaseFiles(projectName):
    os.mkdir(os.path.join('programs', projectName))
    with open(os.path.join('programs', projectName, projectName + '.ino'), 'w') as f:
        f.write('#include "config.h"\n')
        f.write('\n')
        f.write('void setup() {\n\n}\n\n')
        f.write('void loop() {\n\n}')
    with open(os.path.join('programs', projectName, 'config-schema.json'), 'w') as f:
        f.write('{\n}')

if __name__ == "__main__":
    projectName = input('Enter project name: ')
    createBaseFiles(projectName)