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