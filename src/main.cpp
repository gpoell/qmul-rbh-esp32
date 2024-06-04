#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "MLX90393.h"
#include "TactileSensor.h"
#include "CommandPrompt.h"
#include "ESPServer.h"

// Global Variables
TactileSensor sensor(1);
vector3Double data;
bool read_flag = false;
String cmd;
CommandPrompt prompt;
WiFiServer server(80);

void setup() {
    // Initialize Serial
    Serial.begin(9600);

    // Initialize I2C communication and serial bus
    Wire.begin();

    // Initilialize Hall Effect Sensor
    sensor.init();

    // Initialize RBH Server
    const char ssid[] = "";
    const char passwd[] = "";
    esp_server_init(server, ssid, passwd);

    // Wait for Serial Bus to open
    delay(1000);
    prompt.prompt();
};

void loop() {
    WiFiClient client = server.available();
    if (client) 
    {
        Serial.println("Client connected..");
        // Read command
        cmd = client.readString();
        Serial.println(cmd);
        // Set command option
        prompt.setOption(cmd);  // this method should return integer
        int cmd_option = prompt.getOption();
        if (cmd_option == 5) 
        {
            int sample = 10;
            // Send initial byte for client to prepare reading
            client.print('1');
            // Send data
            for (int i = 0; i < sample; i++) 
            {
                data = sensor.readData();
                Serial.println(String(data.x) + "," + String(data.y) + "," + String(data.z));
                client.print(String(data.x) + "," + String(data.y) + "," + String(data.z));
            };
        }
        else {
            Serial.println("Not command collect...");
        }
        
        prompt.prompt();
    }
}