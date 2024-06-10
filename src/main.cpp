#include <Arduino.h>
#include "ESPServer.h"

ESPServer espserver {80};

void setup() {
    // Initialize ESP Server
    espserver.init();
};

void loop() {
    // Wait for Client (GUI) to send command
    WiFiClient client = espserver.client_available();
    if (client) {
        
        // Read command
        Serial.println("Client connected..");
        const String cmd = client.readStringUntil('\0');
        Serial.println(cmd);
        
        // Process command
        espserver.process_command(cmd, client);
    }
}