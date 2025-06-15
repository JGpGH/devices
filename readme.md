# Install
```
python3 -m venv .
./bin/pip3 install -r ./requirements.txt
./bin/python3 ./ensureInstalled.py
```

## Windows
idk if buildAndUpload commands to arduinocli is going to work

# Upload 
```
./bin/python3 ./buildAndUpload.py [config name] [serial port]
```
example:
```
./bin/python3 ./buildAndUpload.py nano-codec-test /dev/ttyUSB0
```