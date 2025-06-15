# Install
### Windows
run on powershell, regular user
```
install-windows.ps1
```
### Ubuntu
```
install-ubuntu.sh
```
# Upload 
```
./bin/python3 ./buildAndUpload.py [config name] [serial port]
```
example:
```
./bin/python3 ./buildAndUpload.py nano-codec-test /dev/ttyUSB0
```