#include <iostream>
#include "ESPServer.h"

using namespace std;

void set_ipaddress() {
    IPAddress local_IP(192, 168, 0, 11);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    if (!WiFi.config(local_IP, gateway, subnet)) {
        cout << "Assignment of static IP address failed..." << endl;
    };
};

void connect_network(const char* ssid, const char* passwd) {
    int status {WL_IDLE_STATUS};
    while (status != WL_CONNECTED) {
        cout  << "Attempting to connect to network: " << ssid << endl;
        status = WiFi.begin(ssid, passwd);
        delay(3000);
    };
};

void start_server(WiFiServer& server) {
    cout << "Connected to wifi...starting server" << endl;
    server.begin();
};

void esp_server_init(WiFiServer& server, const char* ssid, const char* passwd) {
    set_ipaddress();                // Set Static IP Address
    connect_network(ssid, passwd);  // Connect to Network
    start_server(server);           // Start Server
};