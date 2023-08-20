import os

def libraryExists(library):
    return os.path.isdir(f"lib/{library}")

os.system("arduino-cli core install atmel-avr-xminis:avr")

if not libraryExists("IRemote"):
    os.system("git clone https://github.com/z3t0/Arduino-IRremote.git lib/IRemote")