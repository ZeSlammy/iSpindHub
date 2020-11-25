#include <ArduinoLog.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "screen.h"
#include <WiFiUdp.h>
#include <LittleFS.h>
#include "ntp.h"
#include <StreamUtils.h>
void initWebServer();
void setActionPageHandlers();
void setJsonHandlers();
#define LOG_LEVEL LOG_LEVEL_VERBOSE
