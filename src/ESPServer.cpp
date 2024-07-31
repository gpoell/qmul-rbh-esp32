#include "ESPServer.h"
#include "secrets.h"

static void start_server(WiFiServer& server);
static void set_ipaddress();
static void connect_network();

void ESPServer::init() {
    Serial.begin(9600);         // Initialize Serial
    Wire.begin();               // Initialize I2C Communication
    sensor.init();              // Initialize Hall Effect Sensor
    motor.init();               // Initialize Motor
    start_server(server);       // Configure and start Wi-Fi Server
    prompt.prompt();            // Display prompt
};

void ESPServer::process_command(const String& cmd, WiFiClient& client) {

    const int motor_duration = 500;
    const int calibrate_sample = 20;
    const int collect_sample = 10;
    
    client.print('1');

    if (cmd.startsWith("connect")) {
        connected = true;
        guiClient = client;
    }
    else if (cmd.equals("disconnect")) {
        connected = false;
        guiClient.stop();
    }
    else if (cmd.equals("open")) { 
        motor.open_gripper();
        delay(motor_duration);
        motor.stop_gripper(); 
    }
    else if (cmd.equals("close")) {
        motor.close_gripper();
        delay(motor_duration);
        motor.stop_gripper();
    }
    else if (cmd.startsWith("calibrate")) {
        sensor.calibrate(calibrate_sample);
    }
    else if (cmd.startsWith("collect")) {
        motor.close_gripper();
        delay(200);
        for (int i=0; i<collect_sample; i++) {
            send_tactile_data(client);
        };
        motor.stop_gripper();
    }
    else if (cmd.startsWith("help")) {
        prompt.help();
    }
    else {
        prompt.invalid();
    }

    client.stop();
    prompt.prompt();
};

void ESPServer::send_tactile_data(WiFiClient& client) {
    vector3Float data = sensor.readDataMaxZ();
    string message = to_string(data.x) + "," + to_string(data.y) + "," + to_string(data.z) + ",";
    const char* result = create_buffer_message(message, 65);
    client.print(result);
};

const char* ESPServer::create_buffer_message(string& message, int size) {
    int remainder = size - message.length() - 1;
    string filler(remainder, '0');
    message.append(filler);
    const char* p = message.c_str();
    return p;
};

/*
 * Static functions for the ESP Server that configures the Wi-Fi server's
 * static IP address, connects to the network, and starts it.
 */

static void start_server(WiFiServer& server) {
    set_ipaddress();
    connect_network();
    server.begin();
    cout << "[INFO] Connected to wifi...starting server" << endl;
};

static void set_ipaddress() {
    IPAddress local_IP(192, 168, 0, 11);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    // IPAddress local_IP(192, 168, 1, 11);
    // IPAddress gateway(192, 168, 1, 1);
    // IPAddress subnet(255, 255, 255, 0);

    // Output an error message if the static IP address assignment fails
    if (!WiFi.config(local_IP, gateway, subnet)) {
        cout << "[WARNING] Assignment of static IP address failed..." << endl;
    };
};

static void connect_network() {
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