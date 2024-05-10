import time

import leather
import matplotlib.pyplot as plt
import seaborn as sns
from bokeh.plotting import figure, show
import pygal
# from plotnine import ggplot, aes, geom_line
import pandas as pd

data = [
    {'x': 0, 'q': {'y': [3]}},
    {'x': 4, 'q': {'y': [5]}},
    {'x': 7, 'q': {'y': [9]}},
    {'x': 8, 'q': {'y': [4]}},
]


def x(row, index):
    return row['x']


def y(row, index):
    return row['q']['y'][0]

start_time_leather = time.time()

chart = leather.Chart('Line')
chart.add_line(data, x=x, y=y)
chart.to_svg('examples/charts/custom_data.svg')

print("Leather took", time.time() - start_time_leather, "to run")
 # Leather usually takes between 1-2 ms

start_time_mat_plot_lib = time.time()
 # Extract x and y values from data
x_values = [x(row, i) for i, row in enumerate(data)]
y_values = [y(row, i) for i, row in enumerate(data)]

# Plot the data points
plt.plot(x_values, y_values, '-')

# Set labels and title
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Data Points')

# Display the plot
# plt.show()
print("Matplotlib took", time.time() - start_time_mat_plot_lib, "to run")
# Matplotlib takes about 0.12 seconds (100x slower than leather)

start_time_seaborn = time.time()
df = pd.DataFrame(data)

# Plot
sns.lineplot(data=df, x='x', y=df.apply(lambda row: row['q']['y'][0], axis=1))

print("Seaborn took", time.time() - start_time_seaborn, "to run")

# About 10 times slower than leather

# start_time_plotnine = time.time()

# # Define aesthetic mappings
# aesthetics = aes(x='x', y='q_y')

# # Create ggplot object and add layers
# # plot = ggplot(df, aesthetics) + geom_line()

# print("plotnine took", time.time() - start_time_plotnine, "to run")

start_time_bokeh = time.time()

p = figure(title='Data Points', x_axis_label='X', y_axis_label='Y')

# Plot data using circle glyph
p.circle(x=x_values, y=y_values, size=10, color='navy', alpha=0.5)

print("Bokeh took", time.time() - start_time_bokeh, "to run")


start_time_pygal = time.time()

x_values_pygal = [point['x'] for point in data]
y_values_pygal = [point['q']['y'][0] for point in data]

line_chart = pygal.Line()
line_chart.title = 'Data Points'
line_chart.x_labels = x_values_pygal
line_chart.add('Data', y_values_pygal)

print("Pygal took", time.time() - start_time_pygal, "to run")
