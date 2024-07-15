![image](https://github.com/user-attachments/assets/63fac62e-1e70-4d96-854a-6a568bbca8f7)# ESP32 LED Control with Qt GUI via Socket TCP 

## Overview
This project demonstrates how to control an LED connected to an ESP32 using a Qt GUI. The ESP32 receives data from the Qt GUI to control the LED state (ON/OFF) and sends back the current LED status. Additionally, the ESP32 communicates with the Qt client GUI to verify the functionality of TCP communication by sending and receiving data.

## Features
- Control an LED connected to the ESP32 from a Qt GUI.
- Send the LED state back to the Qt GUI.
- Verify TCP communication between the Qt client GUI and the ESP32.

## Components
- ESP32 development board
- LED connected to one of the ESP32's GPIO pins
- Qt GUI for controlling the LED and checking TCP communication

## Setup
1. **Hardware Setup:**
   - Connect the LED to one of the GPIO pins on the ESP32 (e.g., GPIO 2) with an appropriate resistor.

2. **Software Setup:**
   - Install the Arduino IDE and the ESP32 board support package.
   - Install Qt Creator for creating the Qt GUI.

3. **ESP32 Code:**
   - Write and upload the ESP32 code to handle TCP communication and control the LED.

4. **Qt GUI:**
   - Create a Qt GUI with buttons for controlling the LED and displaying its status.
   - Implement TCP communication in the Qt GUI to interact with the ESP32.

## Demo
Below are some images of the demo:



1. ![LED OFF State](![image](https://github.com/user-attachments/assets/f104f327-db15-4bce-9425-206aa737cdf1))
   - LED turned OFF from the Qt GUI.


1. ![LED ON State](![image](https://github.com/user-attachments/assets/80c54497-7d8d-497b-bbd2-d65663baac6e))
   - LED turned ON from the Qt GUI.


## Usage
1. Open the Qt GUI application on your computer.
2. Connect to the ESP32 via TCP.
3. Use the GUI buttons to control the LED state.
4. Observe the LED status updates and TCP communication feedback on the Qt GUI.

## Conclusion
This project showcases a simple yet effective way to control an LED using an ESP32 and a Qt GUI, demonstrating the integration of hardware control and software communication through TCP.
