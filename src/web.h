#include <ArduinoLog.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "screen.h"
#include <WiFiUdp.h>
#include "ntp.h"
#include <time.h>
#include <StreamUtils.h>
#include "wifi.h"
#ifndef USE_LITTLEFS
#define USE_LITTLEFS
#endif
void initWebServer();
void setRegPageAliases();
void setActionPageHandlers();
void setJsonHandlers();
void setSettingsAliases();


bool handleURLTargetPost(AsyncWebServerRequest *request);
bool handleBrewfatherTargetPost(AsyncWebServerRequest *request);

#define LOG_LEVEL LOG_LEVEL_VERBOSE



bool saveConfig();
bool saveFile();
bool serializeConfig(Print &);
bool deserializeConfig(Stream &);