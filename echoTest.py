#!/usr/bin/env python3

import serial
import time

# Configuration parameters for the serial port.
serial_port = '/dev/ttyUSB0'  # Example for Linux
baud_rate = 115200
timeout = 5  # Seconds

def write_and_read(serial_connection, message):
    # Write the message to the serial port.
    serial_connection.write(message.encode())

    # Wait for the data to be transmitted and received.
    time.sleep(1)

    # Try to read the response from the serial port.
    incoming_data = serial_connection.read(size=len(message))
    return incoming_data.decode()

def test_serial_connection(ser):
    # The message to send. Change it as needed.
    message = 'Hello, Serial Port!'
    print(f"Sending: {message}")

    # Write to the serial port and read the response.
    response = write_and_read(ser, message)
    if response == message:
        print(f"loopback test passed: {response}")
    else:
        print(f"loopback test failed: {response}")

def main():
    # Initialize the serial connection.
    with serial.Serial(serial_port, baud_rate, timeout=timeout) as ser:
        try:
            while True:
                test_serial_connection(ser)
                time.sleep(2)
        except KeyboardInterrupt:
            print("Exiting...")
        

if __name__ == "__main__":
    main()