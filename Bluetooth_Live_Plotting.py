import serial
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import struct

# Serial setup
ser = serial.Serial('COM10', 115200)  # Replace 'COM10' with your actual COM port

# Data storage for plotting
times = []
accel_xs, velocity_xs, position_xs = [], [], []  # Only displaying 'x' values for simplicity

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

    # Clear out older data in buffer to avoid overflow
    if ser.in_waiting > 1200:  # Adjust this threshold based on data rate and timing
        ser.read(ser.in_waiting - 12)  # Keep only the last 12 bytes (latest packet)

    # Read from serial if enough data is available
    if ser.in_waiting >= 12:
        data = ser.read(12)
        try:
            x, y, z = struct.unpack('fff', data)
            # print(x)

            # Thresholding to filter out small errors in acceleration data
            if abs(x) < 0.2:
                x = 0

            # Calculate time difference
            current_time = time.time()
            dt = current_time - previous_time
            previous_time = current_time

            # Update velocity and position
            velocity_x += x * dt
            position_x += velocity_x * dt + 0.5 * x * dt**2

            # Append data to lists
            times.append(current_time - start_time)
            accel_xs.append(x)
            velocity_xs.append(velocity_x)
            position_xs.append(position_x)

            # Limit lists to 50 items for performance
            max_points = 50
            if len(times) > max_points:
                times.pop(0)
                accel_xs.pop(0)
                velocity_xs.pop(0)
                position_xs.pop(0)

            # Update plots
            ax1.clear()
            ax2.clear()
            ax3.clear()
            ax1.plot(times, accel_xs, label='Acc X')
            ax2.plot(times, velocity_xs, label='Vel X')
            ax3.plot(times, position_xs, label='Pos X')

            # Set labels and legends
            ax1.set_ylabel('Acceleration X (m/s^2)')
            ax2.set_ylabel('Velocity X (m/s)')
            ax3.set_ylabel('Position X (m)')
            ax3.set_xlabel('Time (s)')
            ax1.legend()
            ax2.legend()
            ax3.legend()

        except struct.error:
            print("Received invalid data:", data)

# Create live animation
ani = FuncAnimation(fig, update_plot, interval=500)  # Update every 500 ms
plt.tight_layout()
plt.show()
