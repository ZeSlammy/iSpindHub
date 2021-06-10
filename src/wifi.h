#ifndef _WIFI_H
#define _WIFI_H
#define WM_ASYNC // Turn on Async mode

#include <ArduinoLog.h>
//#include <ArduinoJson.h>
#include "tools.h"
#include <ESP8266WiFi.h>
#include "config.h"
#include "jsonconfig.h"
#define WEBSERVER_H
#include <AsyncWiFiManager.h>
void doWiFi();
void doWiFi(bool dontUseStoredCreds);
void resetWifi();
extern struct Config config;
// WiFiManager Callbacks
void apCallback(AsyncWiFiManager *myWiFiManager);
void saveConfigCallback();
void saveParamsCallback();
void WiFiEvent(WiFiEvent_t event);


#endif // _WIFI_H