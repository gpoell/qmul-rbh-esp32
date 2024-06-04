#ifndef ESPServer_h
#define ESPServer_h

#include <WiFi.h>

void esp_server_init(WiFiServer& server, const char* ssid, const char* passwd);

#endif