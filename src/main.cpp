#include <Arduino.h>
#include "ESPServer.h"

ESPServer espserver {5000};

void setup() {
    espserver.init();
};

void loop() {

    // Process GUI or Serial Monitor Commands
    WiFiClient client = espserver.get_server();
    if (client) {
        const String cmd = client.readStringUntil('\0');
        espserver.process_command(cmd, client);
    };

    // Continuously send tactile data while GUI is connected
    if (espserver.is_connected()) {
        WiFiClient guiClient = espserver.get_client(); 
        espserver.send_tactile_data(guiClient);
    }
}