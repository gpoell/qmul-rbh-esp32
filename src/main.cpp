#include "Arduino.h"
#include "MLX90393.h"
#include "TactileSensor.h"
#include "Wire.h"
#include "CommandPrompt.h"
#include "Wifi.h"

// Global Variables
TactileSensor sensor(1);
vector3Double data;
bool read_flag = false;
String cmd;
CommandPrompt prompt;

// WiFi Configuration -- need to move to config file
IPAddress local_IP(192, 168, 0, 11);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

String ssid = "";
String passwd = "";
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
    // Initialize Serial
    Serial.begin(9600);

    // Initialize I2C communication and serial bus
    Wire.begin();

    // Initilialize Hall Effect Sensor
    sensor.init();

    // Initialize WiFi
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }

    Serial.print("Attempting to connect to WPA network, SSID: ");
    Serial.print(ssid);
    Serial.println();
    status = WiFi.begin(ssid, passwd);

    while (status != WL_CONNECTED) {
        delay(3000);
        Serial.print("Attempting to connect to WPA network, SSID: ");
        Serial.print(ssid);
        Serial.println();
        status = WiFi.begin(ssid, passwd);
    }
    
    // Set Static IP Address
    IPAddress myAddress = WiFi.localIP();
    Serial.print("Server Address: ");
    Serial.print(myAddress);
    Serial.println();

    // Start Server
    Serial.println("Connected to wifi...starting server");
    server.begin();

    // Wait for Serial Bus to open
    delay(1000);
    prompt.prompt();
};

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Client connected..");
        cmd = client.readString();
        Serial.println(cmd);
        client.println(cmd);
        prompt.setOption(cmd);
        int cmd_option = prompt.getOption();
        if (cmd_option == 5){
            int sample = 50;
            for(int i = 0; i < sample; i++) {
                data = sensor.readData();
                Serial.println(String(data.x) + "," + String(data.y) + "," + String(data.z));
            };
        }
        else {
            Serial.println("Not command collect...");
        }
        
        prompt.prompt();
    }
}