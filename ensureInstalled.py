import os
import subprocess
from sys import platform
from color import Error, Warning, Info
import tempfile
import zipfile
import urllib.request

WINDOWS_ARDUINO_CLI_PATH = "bin\\arduino-cli.exe"
LINUX_ARDUINO_CLI_PATH = "bin/arduino-cli"
class Installer:
    def __init__(self):
        if self.is_windows():
            os.makedirs("bin", exist_ok=True)
            self.clipath = WINDOWS_ARDUINO_CLI_PATH
        elif self.is_ubuntu():
            self.clipath = LINUX_ARDUINO_CLI_PATH
        else:
            raise NotImplementedError("Unsupported platform")

    def is_windows(self):
        return platform == 'win32' or platform == 'cygwin'

    def is_ubuntu(self):
        if not platform.startswith('linux'):
            return False
        if not os.path.exists('/etc/lsb-release'):
            return False
        with open('/etc/lsb-release') as f:
            content = f.read()
            if 'DISTRIB_ID=Ubuntu' not in content:
                return False
        return True
    
    def has_package(self, package):
        if self.is_windows():
            return os.system(f"where {package} >nul 2>&1") == 0
        elif self.is_ubuntu():
            return os.system(f"dpkg -l | grep -q {package}") == 0
        else:
            raise NotImplementedError("Unsupported platform")
        
    def library_exists(self, library):
        return os.path.isdir(f"libs/{library}")

    def install_required_tools(self):
        if not self.has_package("git"):
            raise Exception("Git is not installed. Please install Git to continue.")
        if self.is_ubuntu() and not self.has_package("curl"):
            os.system(f"sudo apt install curl")

    def ensure_core_installed(self, core):
        result = subprocess.run([self.clipath, "core", "list"], capture_output=True, text=True)
        if not core in result.stdout:
            print(Warning(f"Core {core} is not installed. Installing..."))
            os.system(f"{self.clipath} core install {core}")
            return True

    def download_file(self, url, dest, unzip=False):
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

    def arduinoCliIsInstalled(self):
        if not os.path.isfile(self.clipath):
            print(Warning("Installing arduino-cli"))
            if platform == 'win32':
                self.download_file("https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip", self.clipath, unzip=True)
            else:
                subprocess.run("curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh", shell=True)

    def ensureAll(self):
        self.install_required_tools()
        self.arduinoCliIsInstalled()

        self.ensure_core_installed("arduino:avr")

        self.ensure_core_installed("atmel-avr-xminis:avr")

        if not self.library_exists("Vector-1.2.2"):
            print(Warning("Installing Vector-1.2.2"))
            zip_path = "libs/Vector-1.2.2.zip"
            url = "https://downloads.arduino.cc/libraries/github.com/janelia-arduino/Vector-1.2.2.zip"
            self.download_file(url, zip_path, unzip=True)

        if not self.library_exists("IRemote"):
            os.system("git clone https://github.com/z3t0/Arduino-IRremote.git libs/IRemote")
CLIPATH = Installer().clipath

if __name__ == "__main__":
    Installer().ensureAll()
