#include "MLX90393.h"
#include "TactileSensor.h"
#include "Wire.h"
#include "CommandPrompt.h"

// Instantiate Classes
TactileSensor sensor(1);
CommandPrompt prompt;
vector3Double data;
bool readFlag = false;
String cmd;

void setup() {
    //Initialize I2C communication, serial bus, and sensor
    Wire.begin();
    Serial.begin(9600);
    sensor.init();

    // Wait for Serial Bus to open
    delay(1000);
    prompt.prompt();
}

void loop() {
    if (Serial.available()) {
        
        // Wait for user to enter command
        cmd = Serial.readStringUntil('\n');
        cmd.trim();
        Serial.println(cmd.length());
        prompt.setOption(cmd);
        int cmd_option = prompt.getOption();
        // readFlag = false;
        
        // Execute command option -- see CommandPrompt.h for options list
        switch (cmd_option) {
            case 0: // help
                prompt.help();
                prompt.prompt();
                break;
            case 1: // clear
                prompt.clear();
                prompt.prompt();
                break;
            case 2: // calibrate
                {
                    int sample = 50;
                    if (cmd.length() > 8 && cmd.substring(9).toInt() > 0) {
                        // calibrate with specified number
                        sample = cmd.substring(9).toInt();
                    };
                    sensor.calibrate(sample);
                    prompt.prompt();
                    break;
                };
            case 3: // read
                {
                    readFlag = true;
                    break;
                };
            case 4: // stop
            {
                readFlag = false;
                prompt.prompt();
                break;
            };
            case 5: // collect
                {
                    int sample = 50;
                    if (cmd.length() > 7 && cmd.substring(8).toInt() > 0) {
                        sample = cmd.substring(8).toInt();
                    };
                    for(int i=0; i<sample; i++){
                        data = sensor.readData();
                        Serial.println(String(data.x) + "," + String(data.y) + "," + String(data.z));
                    };
                    prompt.prompt();
                    break; 
                };
            default:
                prompt.invalid();
                prompt.prompt();
                break;
        };
    };
    if (readFlag) {
        data = sensor.readData();
        Serial.println(String(data.x) + " " + String(data.y) + " " + String(data.z));
    };

}
