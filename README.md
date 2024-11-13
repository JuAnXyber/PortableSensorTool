# PortableSensorTool
This tool is designed to read data and manage slave IDs, as well as write new slave IDs for sensors using the Modbus communication protocol. The device offers two sensor options: temperature and humidity.

Temperature Sensor Connection
The temperature sensor utilizes a direct connection method via the slave host (ID 254). This direct connection allows seamless integration between the tool and the Modbus sensor. Once connected, the tool automatically displays the temperature data and the sensor's slave ID. Additionally, there's a menu to write a new slave ID, allowing easy modification of the sensor's ID.

Humidity Sensor Connection
For the humidity sensor, the tool uses an Auto Scan method. It scans each slave ID from 1 to 247 (the maximum ID for Modbus sensors) to establish a connection. Alternatively, it offers a manual input method for immediate connection without waiting for the scan, which takes approximately 2 to 3 seconds per ID. Once connected, the tool displays the humidity data (as a percentage) and the sensor’s slave ID, and also provides an option to write a new slave ID for the humidity sensor.

Error Messaging
This tool is equipped with an error messaging feature during the writing of the slave ID. If there is an error while writing/flashing the new slave ID to the Modbus sensor, the tool detects the error and displays an error message on the screen.

Components and Customization
The system is powered by an ESP32, with RS485 to TTL used to convert data from Modbus to the ESP32 serial. It features a 16x2 character LCD with I2C for display. Additionally, it includes a step-up module from 5V to 24V for sensor power needs. The tool is customizable to meet various requirements.
