#define WM_ASYNC // Turn on Async mode
#include "AsyncWiFiManager.h"
#include <ArduinoLog.h>
#include <ArduinoJson.h>
#include "tools.h"
#include <ESP8266WiFi.h>
void doWiFi();
void doWiFi(bool);
void resetWifi();
