/*
 * ESP Server manages the WiFi, Tactile Sensor, and Motor components, and facilitates 
 * the processing of commands sent by the GUI.
 *      Components
 *          - Wi-Fi Server:     creates a server that listens for incoming connections on
 *                              a specified port.
 * 
 *          - Command Prompt:   displays important information to the user through the
 *                              Serial monitor.
 * 
 *          - Tactile Sensor:   collects magnetic flux density recordings from the Hall
 *                              Effect sensors.
 *  
 *          - L9110H Motor:     controls the direction of the motor for opening and closing
 *                              the gripper.
 * 
 *      Parameters
 *          - port:             port for server to listen to incoming requests (default is 80)
 */

#ifndef ESPServer_h
#define ESPServer_h

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <iostream>
#include "CommandPrompt.h"
#include "TactileSensor.h"
#include "L9110HMotor.h"


class ESPServer {
    private:
        WiFiServer server {80};
        CommandPrompt prompt;
        TactileSensor sensor{1};
        L9110HMotor motor;
        vector3Double data;
        enum CommandOption { help, clear, calibrate, read, stop, collect, open, close };
        CommandOption command;
    public:
        ESPServer(const int port) : server(port) {};
        ~ESPServer() {};
        void init();
        WiFiClient client_available();
        void set_cmd_option(const String& cmd);
        void process_command(const String& cmd, WiFiClient& client);
};

#endif