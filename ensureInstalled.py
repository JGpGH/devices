import os
import subprocess
from sys import platform
from color import Error, Warning, Info
import tempfile
import zipfile
import urllib.request


def libraryExists(library):
    return os.path.isdir(f"libs/{library}")

def has_binary(binary):
    return os.system(f"which {binary} > /dev/null 2>&1") == 0

clipath = "bin/arduino-cli"

def install_required_tools():
    if not has_binary("apt"):
        print(Error("can only run on apt based systems"))
        raise SystemExit(1)
    packages = []
    if not has_binary("unzip"):
        packages.append("unzip > /dev/null 2>&1")
    if not has_binary("git"):
        packages.append("git")
    if not has_binary("curl"):
        packages.append("curl")
    if len(packages) > 0:
        print(Info(f"Installing required packages: {', '.join(packages)}"))
        os.system(f"sudo apt install {' '.join(packages)}")

def coreInstalled(core):
    result = subprocess.run([clipath, "core", "list"], capture_output=True, text=True)
    return core in result.stdout

def download_file(url, dest, unzip=False):
    try:
        with urllib.request.urlopen(url) as response:
            if unzip:
                with tempfile.TemporaryFile() as temp, zipfile.ZipFile(temp, 'r') as zip_ref:
                    zip_ref.extractall(dest)
            else:
                with open(dest, 'wb') as out_file:
                    out_file.write(response.read())
        return True
    except Exception as e:
        print(Error(f"Failed to download {url}: {e}"))
        return False

def arduinoCliIsInstalled():
    if not os.path.isfile(clipath):
        print(Warning("Installing arduino-cli"))
        if platform == 'win32':
            download_file("https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip", clipath, unzip=True)
        subprocess.run("curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh", shell=True)

def ensureAll():
    install_required_tools()
    arduinoCliIsInstalled()

    if not coreInstalled("arduino:avr"):
        print(Warning("Installing arduino:avr"))
        os.system(f"{clipath} core install arduino:avr")

    if not coreInstalled("atmel-avr-xminis:avr"):
        print(Warning("Installing atmel-avr-xminis:avr"))
        os.system(f"{clipath} core install atmel-avr-xminis:avr")

    if not libraryExists("Vector-1.2.2"):
        print(Warning("Installing Vector-1.2.2"))
        zip_path = "libs/Vector-1.2.2.zip"
        url = "https://downloads.arduino.cc/libraries/github.com/janelia-arduino/Vector-1.2.2.zip"
        download_file(url, zip_path, unzip=True)

    if not libraryExists("IRemote"):
        os.system("git clone https://github.com/z3t0/Arduino-IRremote.git libs/IRemote")


if __name__ == "__main__":
    ensureAll()
