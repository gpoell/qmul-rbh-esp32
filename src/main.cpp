#include <Arduino.h>
#include <Wire.h>
#include "ESPServer.h"
#include "secrets.h"
#include "Gripper.h"


// ESPServer espserver {5000};
WiFiServer server {5000};
Gripper gripper;


void start_server(WiFiServer& server);
void set_ipaddress();
void connect_network();


void setup() {
    Serial.begin(9600);         // Initialize Serial
    Wire.begin();               // Initialize I2C Communication
    start_server(server);       // Configure and start Wi-Fi Server
    CommandPrompt::prompt();    // Display prompt
    gripper.init();             // Initialize Gripper
};


void loop() {

    // Process GUI Commands
    WiFiClient client = server.available();
    if (client) {

        // Convert Arduino string
        const String ardCmd = client.readStringUntil('\0');
        const string cmd(ardCmd.c_str(), ardCmd.length());

        // Begin command processing and state machine transitions
        gripper.processCommand(cmd, client);
    
    };

    // Continuously send tactile data while GUI is connected
    if (gripper.getState() == STT_CONNECT) {
        gripper.sendTactileData();
        // WiFiClient guiClient = espserver.get_client(); 
        // espserver.send_tactile_data(guiClient);
    }
};

/*
 * Server functions to set up Wi-Fi configuration.
 *  1. Sets the static IP address
 *  2. Connects to the network
 *  3. Starts server.
 */

void set_ipaddress() {
    IPAddress local_IP(192, 168, 1, 11);
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
