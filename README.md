# Queen Mary University of London
## MSc Advanced Robotics
### Soft Robotic Gripper - ESP32 Server

## Overview
The soft robotic gripper contains an ESP32 MCU that listens for remote commands from the [Graphical User Interface](https://github.com/gpoell/qmul-rbh-gui) to operate the motor and read tactile data from the Hall Effect sensors attached to the gripper's fingertips. This application is developed using [PlatformIO](https://docs.platformio.org/en/latest/what-is-platformio.html) which enables the combined use of the Arduino framework and the C++ STL, providing a development environment familiar to most students. This README provides an outline of the entire application, including detailed instructions on how to clone and run the application, an overview of the application architecture, information on how the repository is organized, and a list of additional resources to expand upon topics covered below.

## Table of Contents
1. [Project Background](#project-background)
1. [Installation and Dependencies](#installation-and-dependencies)
2. [Running the Application](#running-the-application)
3. [Application Architecture](#application-architecture)
4. [Repository Folder Structure](#repository-folder-structure)
5. [Helpful Articles](#helpful-articles)

## Project Background
My dissertation is focused on developing a tactile data collection and object recognition system using the soft robotic gripper and magnetic sensors which are important for robotic perception and interaction. The modules contained in this repository are used to configure a server to receive commands from a remote interface, transmit tactile data from the magnetic sensors, and actuate the motor to operate the gripper. The axial tactile data (x, y, z) collected from the sensors while grasping an object is used to train a classification model for recognizing objects.

## Installation and Dependencies
[Visual Studio Code](https://code.visualstudio.com/) is a powerful, lightweight editor that provides extensions for a variety of programming languages and frameworks. It contains a [PlatformIO extension](https://docs.platformio.org/en/latest/integration/ide/vscode.html) that can easily be installed to use with VS Code. I would recommend following the [Platform IDE for VSCode](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode) configuration instructions and tutorial to test your workspace before proceeding with working on this application.

## Running the Application
Below are instructions to clone the repository using SSH and running the application.

<details>
<summary>Cloning the repository with SSH</summary>

1. Install the latest version of [Git.](https://git-scm.com/downloads)
2. Connect to your GitHub account with SSH: [Connecting to GitHub with SSH](https://docs.github.com/en/authentication/connecting-to-github-with-ssh "Connecting to GitHub with SSH"). Specifically use the instructions below
    1. <https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent>
    2. <https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account>
3. Create a directory called PlatformIO. PlatformIO requires this folder to detect the workspace.
4. Clone the repository under the PlatformIO directory:
```
git clone git@github.com:gpoell/qmul-rbh-esp32.git
```
</details>

<details>
<summary>Running the application</summary>

Open the repository with VS Code once PlatformIO is installed. PlatformIO should automatically detect the platformio.ini file at the root level of the repository and configure your PlatformIO workspace. PlatformIO provides similar features to Arduino and follows the same process of building, uploading, and opening the Serial Monitor. I would recommend starting with the [PlatformIO Quick Start](https://docs.platformio.org/en/latest/integration/ide/vscode.html) example to ensure everything is configured correctly.

1. Build the workspace
2. Upload the executable to the ESP32
3. Open the Serial Monitor

<b>IMPORTANT</b>  
The Wi-Fi connection details are automatically read from a header file called secrets.h inside of the includes/ directory. This file is included in the .gitignore so it should remain local to your workspace, and it should contain your connection details in the following format:  
<b>secrets.h</b>

```
#ifndef secrets_h
#define secrets_h

struct wifi_creds {
    const char ssid[5] = "SSID";
    const char passwd[8] = "password";
};

#endif
```
</details>

## Application Architecture
The application running on the ESP32 leverages the Arduino framework and design pattern. The [main](src/main.cpp) process starts by running a one-time setup followed by a continuous process that runs a majority of the logic for the application. Typical setup processes are used to assign values to global variables, initialize classes, and start class methods like activating serial communication so users can enter and display messages. The setup for this application begins the [Serial](https://www.arduino.cc/reference/en/language/functions/communication/serial/), [I2C](https://www.arduino.cc/reference/en/language/functions/communication/wire/), and [Wi-Fi](https://www.arduino.cc/reference/en/libraries/wifi/) communication methods along with initializing the [motor](include/L9110HMotor.h) and [tactile sensors](include/TactileSensor.h) through the [Gripper](include/Gripper.h). The Gripper utlizes a Finite State Machine (FSM) design pattern which generates unique state objects associated with a command (i.e. the command CONNECT generates a CONNECTED state object) to perform tasks such as managing client connections, reading tactile data, and operating the gripper.

The ESP32 contains a dual-core processor that is used to simultaneously operate the gripper while reading data from the tactile sensors. Core 1 actively listens for incoming remote [client connections](https://reference.arduino.cc/reference/en/libraries/wifi/wificlient/) from the GUI and processes the client commands through the ESP Server. It performs various tasks such as managing client connections, calibrating the sensors, and operating the motor through its [GPIO](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/) pins. Core 0 is primarily used to read data from the tactile sensors using the [I2C](https://www.arduino.cc/reference/en/language/functions/communication/wire/) protocol and sending the data to the GUI so it does not block operations performed by Core 1. When the GUI requests to read tactile data, Core 1 enables a setting in the ESP Server which notifies Core 0 to begin reading tactile data and sending it back to the GUI through its remote client connection.

The diagram below illustrates an overview of how the ESP32 interfaces with the [Graphical User Interface](https://github.com/gpoell/qmul-rbh-gui). It follows a Client-Server model where frontend applications, like GUIs, are considered Clients and backend applications like this one are considered Servers. The GUI sends a client command to the server which is ingested by Core 1 and processed by the ESP Server to perform the appropriate operations. If the client command requests to read tactile data, Core 1 enables a setting in the ESP Server that notifies Core 0 to read data from the tactile sensors and send it back to the GUI through its remote client connection.

<b>ESP32 Server Architecture Diagram</b>
<picture>
    <img src='docs/esp_architecture.png'>
</pictuer>

<details>
<summary>Arduino Wi-Fi</summary>

The [Arduino WiFi](https://www.arduino.cc/reference/en/libraries/wifi/) library is used to create the server running on the ESP32. During setup, the [ESP Server](include/ESPServer.h) is initialized which sets the IP configuration, connects to the network defined in secrets.h, and starts the [server](https://www.arduino.cc/reference/en/libraries/wifi/server.begin). Core1 actively listens and processes incoming [WiFiClient](https://www.arduino.cc/reference/en/libraries/wifi/wificlient) connections and sends data back to the GUI using the [WiFiClient methods](https://www.arduino.cc/reference/en/libraries/wifi/client.print) method.
</details>

<details>
<summary>Tactile Sensor</summary>

The [Tactile Sensor](include/TactileSensor.h) is composed of 4 [MLX90393](include/MLX90393.h) [Hall Effect](https://en.wikipedia.org/wiki/Hall_effect) sensors that record magnetic flux density measurements in 3 dimensions. Each Hall Effect sensor has a magnet suspended slightly above it in a soft silicon material, and these sensors are attached to the fingertips of the gripper. When the gripper grasps an object, it pushes the suspended magnets closer to the Hall Effect sensors which increases the magnetic flux density recordings. This change in magnetic flux density is synonymous to the pressure being applied to a grasped object as a result of its stiffness, which for this project, is used to represent the ripeness stages of strawberries. These recordings are captured using the I2C protocol and are extracted from the memory addresses of the Hall Effect sensors found in its [datasheet](docs/MLX90393-Datasheet-Melexis.PDF). 
</details>

<details>
<summary>L9110H Motor</summary>

The actuator for the soft robotic gripper is a [RS PRO Brushed Geared DC Geared Motor](https://my.rs-online.com/web/p/dc-motors/4130600) connected to a [L9110H H-Bridge Motor Driver](https://www.adafruit.com/product/4489). The motor terminals can be connected to the ESP32 GPIO pins 18 and 19 where the direction of the motor is controlled by supplying power to one of the pins using Arduino's digitalWrite method. The [L9110H Motor](include/L9110HMotor.h) is a simple module containing methods for initializing the pins, and opening and closing the gripper.
</details>

## Repository Folder Structure
📁docs/         -- documents for the README  
📁include/      -- project header files  
📁lib/          -- project specific libraries  
📁src/          -- source code  
📁test/         -- project tests

## Helpful Articles
<b>Wi-Fi</b>  

[Arduino WiFi](https://www.arduino.cc/reference/en/libraries/wifi/)
[ESP32 Useful WiFi Library Functions](https://randomnerdtutorials.com/esp32-useful-WiFi-functions-arduino/)

<b>PlatformIO</b>  

[PlatformIO extension](https://docs.platformio.org/en/latest/integration/ide/vscode.html)

<b>DC-Motor: Arduino</b>  

[Arduino DC-Motor](https://arduinogetstarted.com/tutorials/arduino-dc-motor?utm_content=cmp-true)