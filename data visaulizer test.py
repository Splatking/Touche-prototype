import serial
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

sns.set(style="whitegrid")  # Set Seaborn style

fig, ax = plt.subplots(figsize=(6, 3))

data_size = 160

arduino = serial.Serial('COM6', 115200)

freq = np.arange(data_size)
capacity = np.zeros(data_size)

line, = ax.plot(freq, capacity)

def read():
    data = arduino.readline()
    decoded = data.decode("utf-8").strip()
    if ":" in decoded:
        index, value = map(float, decoded.split(":"))
        if 0 <= index < data_size:
            capacity[int(index)] = value

def update_chart(frame):
    try:
        read()
    except Exception as e:
        print("Error:", e)
    line.set_ydata(capacity)
    ax.set_ylim(0, max(capacity) + 10)  # Adjust y-axis limits if needed
    return line,

animation = FuncAnimation(fig, update_chart, interval=10)
plt.show()