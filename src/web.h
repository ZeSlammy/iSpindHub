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
void setSettingsAliases();
extern struct Config config;

bool handleURLTargetPost(AsyncWebServerRequest *request);
bool handleBrewfatherTargetPost(AsyncWebServerRequest *request);

#define LOG_LEVEL LOG_LEVEL_VERBOSE

struct URLTarget
{
    // Stores URL Target configuration
    char url[128];
    int freq;
    bool update;

    void load(JsonObjectConst);
    void save(JsonObject) const;
};

struct KeyTarget
{
    // Stores Key Target configurations
    char key[64];
    int channel;
    int freq;
    bool update;

    void load(JsonObjectConst);
    void save(JsonObject) const;
};

struct Config
{
    // Stores the complete configuration
    URLTarget urltarget;
    KeyTarget brewfather;
    void load(JsonObjectConst);
    void save(JsonObject) const;
};

bool saveConfig();
bool saveFile();
bool serializeConfig(Print &);
bool deserializeConfig(Stream &);