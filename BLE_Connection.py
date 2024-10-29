import asyncio
from bleak import BleakClient


ESP32_MAC_ADDRESS = "e8:9f:6d:55:15:7e"  
CHAR_UUID_X = "beb5483e-36e1-4688-b7f5-ea07361b26a8"  # X-axis characteristic UUID
CHAR_UUID_Y = "0e8f1722-1b3e-4d9b-a8ca-cdf0350b7c1d"  # Y-axis characteristic UUID
CHAR_UUID_Z = "6e400001-b5a3-f393-e0a9-e50e24dcca9e"  # Z-axis characteristic UUID

async def get_acceleration_data():
    async with BleakClient(ESP32_MAC_ADDRESS) as client:
        print("Connected to ESP32!")

        while True:
            try:
                x_data = await client.read_gatt_char(CHAR_UUID_X)
                y_data = await client.read_gatt_char(CHAR_UUID_Y)
                z_data = await client.read_gatt_char(CHAR_UUID_Z)

                # Decode data assuming ESP32 sends it as strings
                x = float(x_data.decode("utf-8"))
                y = float(y_data.decode("utf-8"))
                z = float(z_data.decode("utf-8"))

                print(f"Acceleration - X: {x}, Y: {y}, Z: {z}")
            except Exception as e:
                print(f"Error reading data: {e}")
                break
            
            await asyncio.sleep(0.5)  # Delay between reads

asyncio.run(get_acceleration_data())
