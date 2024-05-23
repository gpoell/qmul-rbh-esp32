#include "Arduino.h"
#include "MLX90393.h"
#include "TactileSensor.h"
#include "Wire.h"
#include "CommandPrompt.h"
#include "Wifi.h"
#include "RBHServer.h"

// Global Variables
TactileSensor sensor(1);
vector3Double data;
bool read_flag = false;
String cmd;
CommandPrompt prompt;
RBHServer rbhserver;

void setup() {
    // Initialize Serial
    Serial.begin(9600);

    // Initialize I2C communication and serial bus
    Wire.begin();

    // Initilialize Hall Effect Sensor
    sensor.init();

    // Initialize RBH Server
    String ssid = "";
    String passwd = "";
    rbhserver.init(ssid, passwd);   // Start WiFi connection
    rbhserver.start_server();       // Start Server

    // Wait for Serial Bus to open
    delay(1000);
    prompt.prompt();
};

void loop() {
    WiFiClient client = rbhserver.server.available();
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
            client.print('/n');
            // Send data
            for (int i = 0; i < sample; i++) 
            {
                data = sensor.readData();
                Serial.println(String(data.x) + "," + String(data.y) + "," + String(data.z));
                client.print(String(data.x) + "," + String(data.y) + "," + String(data.z) + ",");
            };
            // Send terminating byte
            client.print('n');
        }
        else {
            Serial.println("Not command collect...");
        }
        
        prompt.prompt();
    }
}