#include <Arduino.h>
#include "ESPServer.h"

ESPServer espserver {5000};

void setup() {
    espserver.init();
};

void loop() {

    // Process GUI or Serial Monitor Commands
    WiFiClient client = espserver.client_available();
    if (client || Serial.available()) {
        const String cmd = client.readStringUntil('\0');
        espserver.process_command(cmd, client);
    };

    // Send Tactile Data when GUI is connected
    if (espserver.is_connected()) {
        espserver.get_tactile_data();
    }
}