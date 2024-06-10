#include "ESPServer.h"
#include "secrets.h"

using namespace std;

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

WiFiClient ESPServer::client_available() {
    return server.available();
};

void ESPServer::set_cmd_option(const String& cmd) {
    if (cmd.equals("clear")) { command = clear; }
    else if (cmd.equals("open")) { command = open; }
    else if (cmd.equals("close")) { command = close; }
    else if (cmd.startsWith("calibrate")) { command = calibrate; }
    else if (cmd.equals("read")) { command = read; }
    else if (cmd.equals("stop")) { command = stop; }
    else if (cmd.startsWith("collect")) { command = collect; }
    else { command = help; }
};

void ESPServer::process_command(const String& cmd, WiFiClient& client) {
    set_cmd_option(cmd); // smart pointer?
    switch (command) {
        case open:
            motor.open();
            client.print('1');
            client.print(cmd);
            break;
        case close:
            motor.close();
            client.print('1');
            client.print(cmd);
            break;
        case calibrate:
            break;
        case collect:
            {
                const int sample = 3;
                // Send initial byte for client to prepare reading
                client.print('1');
                for(int i=0; i<sample; i++){
                    data = sensor.readData();
                    Serial.println(String(data.x) + "," + String(data.y) + "," + String(data.z));
                    client.print(String(data.x) + "," + String(data.y) + "," + String(data.z));
                };
            }
            break;
        case read:
            break;
        case stop:
            break;
        case help:
            prompt.help();
            break;
        default:
            prompt.invalid();
            prompt.help();
            break;
    };
    prompt.prompt();
};

/*
 * Static functions for the ESP Server that configures the Wi-Fi server's
 * static IP address, connects to the network, and starts it.
 */

static void start_server(WiFiServer& server) {
    set_ipaddress();        // Set Static IP Address
    connect_network();      // Connect to Network
    server.begin();         // Start Server
    cout << "Connected to wifi...starting server" << endl;
};

static void set_ipaddress() {
    IPAddress local_IP(192, 168, 0, 11);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    // Output an error message if the static IP address assignment fails
    if (!WiFi.config(local_IP, gateway, subnet)) {
        cout << "Assignment of static IP address failed..." << endl;
    };
};

static void connect_network() {
    // Set initial status to Idle
    int status {WL_IDLE_STATUS};
    
    // Read Wi-Fi credentials from configuration
    wifi_creds w;

    // Attempt to connect to network
    while (status != WL_CONNECTED) {
        cout  << "Attempting to connect to network: " << w.ssid << endl;
        status = WiFi.begin(w.ssid, w.passwd);
        delay(3000);
    };
};