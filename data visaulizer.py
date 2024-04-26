import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

fig = plt.figure(figsize=(6, 3))

data_size = 160

arduino = serial.Serial('COM6', 115200)

freq = [0] * data_size
start = 0
capacity = [0] * data_size

ln, = plt.plot(freq, capacity, '-')

while True:
    freq[start] = start
    
    start = start+1
    if start == data_size:
        break
    

def read():
    data = arduino.readline()
    decoded = str(data[0:len(data)].decode("utf-8"))
    splitData = decoded.split(":")
    clean_data = [line.rstrip() for line in splitData]
    capacity[int(clean_data[0])] = float(clean_data[1])

def update_chart(frame):
    try:
        read()
    except:
        print("out of range somehow")
    ln.set_data(freq, capacity)
    fig.gca().relim()
    fig.gca().autoscale_view()
    return ln,

animation = FuncAnimation(fig, update_chart, interval=0.01)
plt.show()
