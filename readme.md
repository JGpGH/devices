# Install
```
python3 -m venv .
./bin/pip3 install -r ./requirements.txt
./bin/python3 ./ensureInstalled.py
```

# Upload 
```
./bin/python3 ./buildAndUpload.py [config name] [serial port]
```
example:
```
./bin/python3 ./buildAndUpload.py nano-codec-test /dev/ttyUSB0
```