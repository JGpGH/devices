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
## find your serial port
./bin/arduino-cli board list
## build & upload
```
./bin/python3 ./buildAndUpload.py [config name] [serial port]
```
example:
```
./bin/python3 ./buildAndUpload.py nano-codec-test /dev/ttyUSB0
```
```
python3 .\buildAndUpload.py -c blinky-uno -p COM10
```