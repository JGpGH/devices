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

## test
unit tests can be ran with 
```
make -C tests run
```
for integration tests, you need to buildAndUpload the corresponding test to the device first
you can run a given test with
```
make -C tests integrationtest DEV=[serial port] TEST=[test name]
```