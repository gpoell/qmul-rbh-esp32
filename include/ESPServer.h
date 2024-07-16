/*
 * ESP Server manages the WiFi, Tactile Sensor, and Motor components, and facilitates 
 * the processing of commands sent by the GUI.
 *      
 * Attributes:
 *    
 *      server <WiFiServer>:     creates a server that listens for incoming connections on
 *                               a specified port.
 *      guiClient <WiFiClient>:  persists the client for continuously sending tactile data
 *                               to the GUI and unblocking incoming GUI commands.
 *      prompt <CommandPrompt>:  displays important information to the user through the
 *                               Serial Monitor.
 *      sensor <TactileSensor>:  collects magnetic flux density recordings from the
 *                               Hall Effect sensors
 *      motor <L9110HMotor>:     performs motor operations
 *      connected <bool>:        flag for continuously sending tactile data to GUI
 * 
 * Methods:
 * 
 *     init:                     initializes sensors and communication protocols
 *     process_command:          executes gripper functionality based on GUI commands
 *     send_tactile_data:        sends tactile data to GUI with standardized buffer message
 *     create_buffer_message:    resizes the message to match the GUI buffer request
 *     is_connected:             returns the connection status for reading tactile data
 *     get_server:               returns client connection from GUI
 *     get_client:               returns the stored client for continuously sending data to GUI
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

using namespace std;

class ESPServer {
    private:
        WiFiServer server {5000};
        WiFiClient guiClient;
        CommandPrompt prompt;
        TactileSensor sensor{1};
        L9110HMotor motor;
        bool connected {false};

    public:
        ESPServer(const int port) : server(port) {};
        ~ESPServer() {};
        void init();
        void process_command(const String& cmd, WiFiClient& client);
        void send_tactile_data(WiFiClient& client);
        const char* create_buffer_message(string& message, int size);
        bool is_connected() { return connected; };
        WiFiClient get_server() { return server.available(); };
        WiFiClient get_client() { return guiClient; };
};

#endif