import serial
import time
import sys
import getopt

#defines dataclass for serial connection options
class SerialOptions:
    def __init__(self, port, baudrate, timeout):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout

#methods to load serial options from command line arguments
def load_serial_options():
    port = None
    baudrate = 9600
    timeout = 2000

    try:
        opts, args = getopt.getopt(sys.argv[1:], 'p:b:t:', ['port=', 'baudrate=', 'timeout='])
    except getopt.GetoptError:
        print('monitor.py -p <port> -b <baudrate> -t <timeout>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-p', '--port'):
            port = arg
        elif opt in ('-b', '--baudrate'):
            baudrate = arg
        elif opt in ('-t', '--timeout'):
            timeout = arg

    return SerialOptions(port, baudrate, timeout)

def openFromOptions(options):
    return serial.Serial(options.port, options.baudrate, timeout=options.timeout)

def loopInOut():
    # Read serial port
    while True:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
                print(f'{timestamp}->{line}')
        except KeyboardInterrupt:
            print('Exiting...')
            sys.exit()

if __name__ == '__main__':
    # Open serial port
    ser = openFromOptions(load_serial_options())
    ser.flush()
    loopInOut()