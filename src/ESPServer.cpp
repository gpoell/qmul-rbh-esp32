#include <iostream>
#include "ESPServer.h"
#include "secrets.h"

using namespace std;

void set_ipaddress() {
    IPAddress local_IP(192, 168, 0, 11);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    if (!WiFi.config(local_IP, gateway, subnet)) {
        cout << "Assignment of static IP address failed..." << endl;
    };
};

void connect_network() {
    int status {WL_IDLE_STATUS};
    wifi_creds w;
    while (status != WL_CONNECTED) {
        cout  << "Attempting to connect to network: " << w.ssid << endl;
        status = WiFi.begin(w.ssid, w.passwd);
        delay(3000);
    };
};

void start_server(WiFiServer& server) {
    cout << "Connected to wifi...starting server" << endl;
    server.begin();
};

void esp_server_init(WiFiServer& server) {
    set_ipaddress();        // Set Static IP Address
    connect_network();      // Connect to Network
    start_server(server);   // Start Server
};