#!/usr/bin/env python3
import serial
import sys
import getopt
import threading

#defines dataclass for serial connection options
class SerialOptions:
    def __init__(self, port: str, baudrate: int, timeout: int):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout

#methods to load serial options from command line arguments
def load_serial_options():
    port = None
    baudrate = 115200
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
    print(f"Port: {port}, Baudrate: {baudrate}, Timeout: {timeout}")
    return SerialOptions(port, baudrate, timeout)

def openFromOptions(options: SerialOptions):
    return serial.Serial(port=options.port, baudrate=options.baudrate, timeout=options.timeout)

def read_serial(ser: serial.Serial):
    while True:
        try:
            if ser.in_waiting > 0:
                data = ser.read_all().decode(errors='replace').strip()
                print(data, end='')
        except serial.SerialException as e:
            print("Serial error:", e)
            break

def send_input(ser):
    while True:
        try:
            user_input = input("")
            if user_input.lower() == 'exit':
                break
            ser.write(user_input.encode())
        except serial.SerialException as e:
            print("Serial error:", e)
            break

if __name__ == '__main__':
    # Open serial port
    ser = openFromOptions(load_serial_options())
    ser.flush()
    try:
        print("Serial port opened successfully.")
        
        # Creating threads for reading serial data and sending input
        read_thread = threading.Thread(target=read_serial, args=(ser,), daemon=True)
        send_thread = threading.Thread(target=send_input, args=(ser,), daemon=True)
        
        # Starting threads
        read_thread.start()
        send_thread.start()
        
        # Joining threads to keep the main program running until threads are done
        read_thread.join()
        send_thread.join()
        
        # Closing serial port when threads are done
        ser.close()
        print("Serial port closed.")
    except serial.SerialException as e:
        print("Error opening serial port:", e)
