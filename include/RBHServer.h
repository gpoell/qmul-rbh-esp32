#ifndef RBHServer_h
#define RBHServer_h
#include <WiFi.h>

class RBHServer {
    private:

    public:
        WiFiServer server;
        WiFiClient client;
        void init(String ssid, String passwd);
        void set_ipaddress();
        void start_server();
};

#endif