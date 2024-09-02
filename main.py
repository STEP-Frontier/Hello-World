import serial
import time
import os


# Get USB port name by running: ls /dev/tty.*
port = '/dev/tty.usbmodem11403'
baudrate = 115200
ser = serial.Serial(port=port, baudrate=baudrate, timeout=1)

try:    
    while True:
        line = ser.readline()
        if not line: 
            continue
        
        contents = line.decode('utf-8').strip()
        print(contents)
        

except KeyboardInterrupt:
    print("Finished")

finally:
    ser.close()
