# QMUL MSc Advanced Robotics
# Soft Robotic Gripper: ESP32 Server

## Overview
The soft robotic gripper contains an ESP32 MCU that is responsible for its actuation and reading tactile data from the Hall Effect sensors attached to the tips of the fingers. It can receive wireless commands using the [Graphical User Interface](https://github.com/gpoell/qmul-rbh-gui) designed to simplify the collection and visualization of tactile data. 

This application is developed using [PlatformIO](https://docs.platformio.org/en/latest/what-is-platformio.html) which enables the use of the Arduino framework and the C++ STL to provide a development environment familiar to most students. The ESP32 contains 2 cores that allow for processes to run concurrently; one process actively listens for commands from the GUI to operate the motor while the other process continuously sends real-time tactile data to the GUI.  

## Table of Contents
1. [Installation and Dependencies](https://github.com/gpoell/qmul-rbh-gui/tree/feature-update-readme?tab=readme-ov-file#installation-and-dependencies)
2. [Running the Application](https://github.com/gpoell/qmul-rbh-gui/tree/feature-update-readme?tab=readme-ov-file#running-the-application)
3. [Application Architecture](https://github.com/gpoell/qmul-rbh-gui/tree/feature-update-readme?tab=readme-ov-file#application-architecture)
4. [PyQt Components](https://github.com/gpoell/qmul-rbh-gui/tree/feature-update-readme?tab=readme-ov-file#pyqt-components)
5. [Repository Folder Structure](https://github.com/gpoell/qmul-rbh-gui/tree/feature-update-readme?tab=readme-ov-file#repository-folder-structure)
6. [Helpful Articles](https://github.com/gpoell/qmul-rbh-gui/tree/feature-update-readme?tab=readme-ov-file#helpful-articles)

## Installation and Dependencies
[Visual Studio Code](https://code.visualstudio.com/) is a powerful, lightweight editor that provides extensions for a variety of programming languages and frameworks. There is a [PlatformIO extension](https://docs.platformio.org/en/latest/integration/ide/vscode.html) that can easily be installed to use with VS Code.

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

<b>Note:</b>  
The Wi-Fi connection details are automatically read from a header file called secrets.h inside of the includes/ directory. This file should contain your connection details in the following format:  
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
