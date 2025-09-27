import serial
import time

class NanoSerial:
    def __init__(self, port, baudrate=115200, timeout=1):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.ser = None

    def open(self):
        try:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=self.timeout)
            print(f"Connected to {self.port}")
            return True
        except Exception as e:
            print(f"Serial connection failed: {str(e)}")
            return False

    def send(self, command):
        if self.ser and self.ser.is_open:
            try:
                command_bytes = command.encode() # 将字符串编码为字节流
                self.ser.write(command_bytes) # 发送字节流
                print(f"Sent: {command}")
                return True
            except Exception as e:
                print(f"Serial error: {str(e)}")
                return False
        return False

    def close(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            print(f"Disconnected from {self.port}")

    def is_open(self):
        return self.ser.is_open if self.ser else False