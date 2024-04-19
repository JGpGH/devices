import os
import subprocess
from color import Error, Warning, Info, Success

def libraryExists(library):
    return os.path.isdir(f"libs/{library}")

clipath = "bin/arduino-cli"

def coreInstalled(core):
    result = subprocess.run([clipath, "core", "list"], capture_output=True, text=True)
    return core in result.stdout

def arduinoCliIsInstalled():
    if not os.path.isfile(clipath):
        print(Warning("Installing arduino-cli"))
        subprocess.run("curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh", shell=True)

arduinoCliIsInstalled()

if not coreInstalled("arduino:avr"):
    print(Warning("Installing arduino:avr"))
    os.system(f"{clipath} core install arduino:avr")
else:
    print(Success("arduino:avr is already installed"))

if not coreInstalled("atmel-avr-xminis:avr"):
    print(Warning("Installing atmel-avr-xminis:avr"))
    os.system(f"{clipath} core install atmel-avr-xminis:avr")
else:
    print(Success("atmel-avr-xminis:avr is already installed"))

if not libraryExists("Vector-1.2.2"):
    print(Warning("Installing Vector-1.2.2"))
    os.system("wget -qO ~/devices/libs/Vector-1.2.2.zip https://downloads.arduino.cc/libraries/github.com/janelia-arduino/Vector-1.2.2.zip && unzip -d ~/devices/libs ~/devices/libs/Vector-1.2.2.zip && rm ~/devices/libs/Vector-1.2.2.zip")

if not libraryExists("IRemote"):
    os.system("git clone https://github.com/z3t0/Arduino-IRremote.git libs/IRemote")
else:
    print(Success("IRemote is already installed"))
