#ifndef WEB_H
#define WEB_H
#include <ArduinoLog.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#define WEBSERVER_H
#include <ESPAsyncWebServer.h>
#include "screen.h"
#include <WiFiUdp.h>
#include "ntp.h"
#include <time.h>
#include <StreamUtils.h>
#include "wifi.h"
#include "uptime.h"
#ifndef USE_LITTLEFS
#define USE_LITTLEFS
#endif
void initWebServer();
void setRegPageAliases();
void setActionPageHandlers();
void setJsonHandlers();
void setSettingsAliases();

bool handleiSpindHubPost(AsyncWebServerRequest *request);
bool handleURLTargetPost(AsyncWebServerRequest *request);
bool handleBrewfatherTargetPost(AsyncWebServerRequest *request);
bool handleBPiLessPost(AsyncWebServerRequest *request);

#define LOG_LEVEL LOG_LEVEL_VERBOSE
extern struct Config config;
extern const size_t capacityDeserial;
extern const size_t capacitySerial;
extern const char *resetReason[7];
extern const char *resetDescription[7];


bool saveConfig();
bool saveFile();
bool serializeConfig(Print &);
bool deserializeConfig(Stream &);

#endif //