#define WM_ASYNC // Turn on Async mode
#define WEBSERVER_H
#include "AsyncWiFiManager.h"
#include <ArduinoLog.h>
#include <ArduinoJson.h>
#include "tools.h"
#include <ESP8266WiFi.h>
#include "config.h"
#include "jsonconfig.h"
void doWiFi();
void doWiFi(bool);
void resetWifi();
extern struct Config config;
// WiFiManager Callbacks
void apCallback(AsyncWiFiManager *myWiFiManager);
