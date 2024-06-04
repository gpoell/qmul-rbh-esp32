#include <Wifi.h>
#include <iostream>
#include "RBHServer.h"

using namespace std;

void RBHServer::init(String ssid, String passwd) {
    set_ipaddress();
    cout  << "Attempting to connect to network: " << ssid << endl;
    int status = WL_IDLE_STATUS;
    status = WiFi.begin(ssid, passwd);
    while (status != WL_CONNECTED) 
    {
        delay(3000);
        cout  << "Attempting to connect to network: " << ssid << endl;
        status = WiFi.begin(ssid, passwd);
    }
}

void RBHServer::set_ipaddress() {
    IPAddress local_IP(192, 168, 0, 11);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    if (!WiFi.config(local_IP, gateway, subnet))
    {
        cout << "Assignment of static IP address failed..." << endl;
    }
}

void RBHServer::start_server() {
    cout << "Connected to wifi...starting server" << endl;
    server.begin();
}