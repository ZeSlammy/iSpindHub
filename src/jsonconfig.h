#ifndef _JSONCONFIG_H
#define _JSONCONFIG_H
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include <string.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <LittleFS.h>
#include "config.h"

struct ApConfig
{
    // Stores Access Point configuration
    char ssid[32];
    char passphrase[64];

    void load(JsonObjectConst);
    void save(JsonObject) const;
};

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
    ApConfig apconfig;
    URLTarget urltarget;
    KeyTarget brewersfriend;
    KeyTarget brewfather;
    bool nodrd;

    void load(JsonObjectConst);
    void save(JsonObject) const;
};

bool deleteConfigFile();
bool loadConfig();
bool saveConfig();
bool loadFile();
bool saveFile();
bool printConfig();
bool printFile();
bool serializeConfig(Print &);
bool deserializeConfig(Stream &);

#endif // _JSONCONFIG_H