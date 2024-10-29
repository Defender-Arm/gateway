import serial
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Serial setup
ser = serial.Serial('COM10', 115200)  # Replace 'COM10' with your actual COM port

# Data storage for plotting
times = []
accel_xs, velocity_xs, position_xs = [], [], [] # Only displaying 'x' values for simplicity

# Initialize variables
start_time = time.time()
previous_time = start_time
velocity_x = 0
position_x = 0

# Setup live plotting
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 8))
fig.suptitle('Live IMU X-Axis Data: Acceleration, Velocity, and Position')

def update_plot(frame):
    global velocity_x, position_x, previous_time

    # Read data from serial
    data = ser.readline().decode().strip()
    accel_data = data.split(',')

    try:
        # Parse acceleration data (x-axis only)
        accel_x = float(accel_data[0])
        if abs(accel_x) < 0.2:          # threshold error filtering
            accel_x = 0

        # Calculate time difference
        current_time = time.time()
        dt = current_time - previous_time
        previous_time = current_time

        # Update velocity (v = u + at) for x-axis
        velocity_x += accel_x * dt

        # Update position (s = ut + 0.5at^2) for x-axis
        position_x += velocity_x * dt + 0.5 * accel_x * dt**2

        # Append data to lists
        times.append(time.time() - start_time)
        accel_xs.append(accel_x)
        velocity_xs.append(velocity_x)
        position_xs.append(position_x)

        # Limit lists to 50 items for performance
        max_points = 50
        if len(times) > max_points:
            times.pop(0)
            accel_xs.pop(0)
            velocity_xs.pop(0)
            position_xs.pop(0)
        print("Position: ", position_x)

        # Update plots
        ax1.clear()
        ax2.clear()
        ax3.clear()
        ax1.plot(times, accel_xs, label='Acc X')
        ax2.plot(times, velocity_xs, label='Vel X')
        ax3.plot(times, position_xs, label='Pos X')
        # print(times)
        # print(position_xs)
        # print(velocity_xs)
        # print(accel_xs)

        # Set labels and legends
        ax1.set_ylabel('Acceleration X (m/s^2)')
        ax2.set_ylabel('Velocity X (m/s)')
        ax3.set_ylabel('Position X (m)')
        ax3.set_xlabel('Time (s)')
        ax1.legend()
        ax2.legend()
        ax3.legend()

    except ValueError:
        print("Received invalid data:", data)

# Create live animation
ani = FuncAnimation(fig, update_plot, interval=500)  # Update every 500 ms
plt.tight_layout()
plt.show()
