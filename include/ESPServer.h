/*
 * ESP Server manages the WiFi, Tactile Sensor, and Motor components, and facilitates 
 * the processing of commands sent by the GUI.
 *      
 * Attributes:
 *    
 *      server <WiFiServer>:     creates a server that listens for incoming connections on
 *                               a specified port.
 *      guiClient <WiFiClient>:  persists the client for sending tactile data
 *      prompt <CommandPrompt>:  displays important information to the user through the
 *                               Serial Monitor.
 *      sensor <TactileSensor>:  collects magnetic flux density recordings from the
 *                               Hall Effect sensors
 *      motor <L9110HMotor>:     opens and closes the gripper
 *      data <vector3Double>:    structure of integers for capturing sensor recordings
 *      connected <bool>:        indicator for sending tactile data to GUI
 * 
 * Methods:
 * 
 *     process_command:          executes gripper functionality based on commands
 *     is_connected:             returns the connection status for reading tactile data
 *     get_tactile_data:         sends tactile data recordings to the GUI
 *     create_buffer_message:    resizes the message to match the GUI buffer request
 * 
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
        WiFiServer server {5000};
        WiFiClient guiClient;
        CommandPrompt prompt;
        TactileSensor sensor{1};
        L9110HMotor motor;
        vector3Double data;
        bool connected {false};

    public:
        ESPServer(const int port) : server(port) {};
        ~ESPServer() {};
        void init();
        WiFiClient client_available();
        void process_command(const String& cmd, WiFiClient& client);
        bool is_connected() {return connected;};
        void get_tactile_data();
        const char* create_buffer_message(std::string& message, int size);
};

#endif