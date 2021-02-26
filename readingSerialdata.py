import serial
import time
import cv2

arduino = serial.Serial("COM6")
arduino.baudrate = 9600

arduino.close()
arduino.open()
stuff = arduino.readline()

def test():
    arduino.write(b'testing connection\n')
    
    stuff = arduino.readline()

    if stuff == b'Recieved\n':
        return "Communication Established"
    else:
        test()

print("Validating communication")
arduino.write(b'Simple Sweep\n')

        
dist = []

count = 0

while True:
    x = cv2.waitKqey()
    while arduino.in_waiting:
        

        inte = arduino.readline().decode("utf-8") 
        if inte[0] == 'D':
            if int(inte[1:inte.index("A")]) < 400 and int(inte[1:inte.index("A")]) > 0:
                print(inte[1:inte.index("A")])
                print(inte[inte.index("A")+1:-2])
                dist.append([inte[1:inte.index("A")],inte[inte.index("A")+1:-1]])

        
    if x ==ord("q"):
        arduino.write(b'sleep')
        exit()
    elif x == "SM":
        arduino.write(b'Simple Sweep\n')
        dist = []
    print(dist)

arduino.close()
