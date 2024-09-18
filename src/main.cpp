#include <Arduino.h>
#include <Wire.h>
#include "ESPServer.h"
#include "secrets.h"
#include "Gripper.h"

enum GRIPPER_CONF {
    SERVER_PORT = 5000,
    WIRE_SDA = 33,
    WIRE_SCL = 32,
    SERIAL_BAUD = 9600
};


// ESPServer espserver {5000};
WiFiServer server {SERVER_PORT};
Gripper gripper;


void start_server(WiFiServer& server);
void set_ipaddress();
void connect_network();


void setup() {
    Serial.begin(SERIAL_BAUD);              // Initialize Serial
    Wire.setPins(WIRE_SDA, WIRE_SCL);       // Set SDA and SCL pins on MCU
    Wire.begin();                           // Initialize I2C Communication
    gripper.init();                         // Initialize Gripper
    start_server(server);                   // Configure and start Wi-Fi Server
    CommandPrompt::prompt();                // Display prompt
};


void loop() {

    // Process GUI Commands
    WiFiClient client = server.available();
    if (client) {
        
        // Send Acknowledge bit
        client.print('1');

        // Convert Arduino string and validate command
        const String ardCmd = client.readStringUntil('\0');
        const string cmd(ardCmd.c_str(), ardCmd.length());

        // Begin command processing and state machine transitions
        gripper.processCommand(cmd, client);
        client.stop();

    };

    // Continuously send tactile data while GUI is connected
    if (gripper.getState() == STT_CONNECT) {
        gripper.sendTactileData();
    };

};

/*
 * Server functions to set up Wi-Fi configuration.
 *  1. Sets the static IP address
 *  2. Connects to the network
 *  3. Starts server.
 */

void set_ipaddress() {
    IPAddress local_IP(192, 168, 1, 10);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    // Output an error message if the static IP address assignment fails
    if (!WiFi.config(local_IP, gateway, subnet)) {
        cout << "[WARNING] Assignment of static IP address failed..." << endl;
    };
};

void connect_network() {
    // Set initial status to Idle
    int status {WL_IDLE_STATUS};
    
    // Read Wi-Fi credentials from configuration
    wifi_creds w;

    // Attempt to connect to network
    while (status != WL_CONNECTED) {
        cout  << "[INFO] Attempting to connect to network: " << w.ssid << endl;
        status = WiFi.begin(w.ssid, w.passwd);
        delay(3000);
    };
};

void start_server(WiFiServer& server) {
    set_ipaddress();
    connect_network();
    server.begin();
    cout << "[INFO] Connected to wifi...starting server" << endl;
};
