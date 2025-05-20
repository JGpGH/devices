import os
import subprocess
from color import Error, Warning, Info, Success

def libraryExists(library):
    return os.path.isdir(f"libs/{library}")

clipath = "bin/arduino-cli"

def install_required_tools():
    if os.system("which apt") != 0:
        print(Error("can only run on apt based systems"))
        raise SystemExit(1)
    packages = []
    if os.system("which unzip") != 0:
        packages.append("unzip")
    if os.system("which git") != 0:
        packages.append("git")
    if os.system("which curl") != 0:
        packages.append("curl")
    if len(packages) > 0:
        print(Info(f"Installing required packages: {', '.join(packages)}"))
        os.system(f"sudo apt install {' '.join(packages)}")

def coreInstalled(core):
    result = subprocess.run([clipath, "core", "list"], capture_output=True, text=True)
    return core in result.stdout

def arduinoCliIsInstalled():
    if not os.path.isfile(clipath):
        print(Warning("Installing arduino-cli"))
        subprocess.run("curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh", shell=True)

def download_file(url, dest):
    """Download a file from a URL to a destination path using pure Python."""
    import urllib.request
    try:
        with urllib.request.urlopen(url) as response, open(dest, 'wb') as out_file:
            out_file.write(response.read())
        return True
    except Exception as e:
        print(Error(f"Failed to download {url}: {e}"))
        return False

if __name__ == "__main__":
    install_required_tools()
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
        zip_path = "libs/Vector-1.2.2.zip"
        url = "https://downloads.arduino.cc/libraries/github.com/janelia-arduino/Vector-1.2.2.zip"
        if download_file(url, zip_path):
            os.system(f"unzip -d libs {zip_path} && rm {zip_path}")
        else:
            print(Error("Failed to download Vector-1.2.2.zip"))

    if not libraryExists("IRemote"):
        os.system("git clone https://github.com/z3t0/Arduino-IRremote.git libs/IRemote")
    else:
        print(Success("IRemote is already installed"))
