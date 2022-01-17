#include "jsonconfig.h"
const char *filename = "/config.json";
Config config;

extern const size_t capacitySerial = 1536;
extern const size_t capacityDeserial = 768;

bool deleteConfigFile()
{
    if (!LittleFS.begin())
    {
        return false;
    }
    return LittleFS.remove(filename);
}

bool loadConfig()
{
    // Manage loading the configuration
    if (!loadFile())
    {
        return false;
    }
    else
    {
        saveFile();
        return true;
    }
}

bool loadFile()
{
    if (!LittleFS.begin())
    {
        return false;
    }
    // Loads the configuration from a file on File System
    File file = LittleFS.open(filename, "r");
    if (!LittleFS.exists(filename) || !file)
    {
        // Unable to open the file
        file.close();
        File file = LittleFS.open(filename, "w");
        if (!LittleFS.exists(filename) || !file)
        {
            // Still could not create a file
            return false;
        }
        else
        {
            file.close();
            file = LittleFS.open(filename, "r");
            if (!LittleFS.exists(filename) || !file)
            {
                // Unable to open a file we created in the previous block
                return false;
            }
        }
    }

    if (!deserializeConfig(file))
    {
        file.close();
        return false;
    }
    else
    {
        file.close();
        return true;
    }
}

bool saveConfig()
{
    return saveFile();
}

bool saveFile()
{
    // Saves the configuration to a file on File System
    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        file.close();
        return false;
    }

    // Serialize JSON to file
    if (!serializeConfig(file))
    {
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool deserializeConfig(Stream &src)
{
    // Deserialize configuration
    DynamicJsonDocument doc(capacityDeserial);

    // Parse the JSON object in the file
    DeserializationError err = deserializeJson(doc, src);

    if (err)
    {
        // We really don;t care if there's an err, the file should be created anyway
        config.load(doc.as<JsonObject>());
        return true;
    }
    else
    {
        config.load(doc.as<JsonObject>());
        return true;
    }
}

bool serializeConfig(Print &dst)
{
    // Serialize configuration
    DynamicJsonDocument doc(capacitySerial);

    // Create an object at the root
    JsonObject root = doc.to<JsonObject>();

    // Fill the object
    config.save(root);

    // Serialize JSON to file
    return serializeJsonPretty(doc, dst) > 0;
}

bool printFile()
{
    // Prints the content of a file to the Serial
    File file = LittleFS.open(filename, "r");
    if (!file)
        return false;

    while (file.available())
        Serial.print((char)file.read());

    Serial.println();
    file.close();
    return true;
}

bool printConfig()
{
    // Serialize configuration
    DynamicJsonDocument doc(capacitySerial);

    // Create an object at the root
    JsonObject root = doc.to<JsonObject>();

    // Fill the object
    config.save(root);

    // Serialize JSON to file
    bool retval = serializeJsonPretty(doc, Serial) > 0;
    Serial.println();
    return retval;
}
bool mergeJsonString(String newJson)
{
    // Serialize configuration
    DynamicJsonDocument doc(capacityDeserial);

    // Parse directly from file
    DeserializationError err = deserializeJson(doc, newJson);
    if (err)
        Serial.println(err.c_str());

    return mergeJsonObject(doc);
}

bool mergeJsonObject(JsonVariantConst src)
{
    // Serialize configuration
    DynamicJsonDocument doc(capacityDeserial);

    // Create an object at the root
    JsonObject root = doc.to<JsonObject>();

    // Fill the object
    config.save(root);

    // Merge in the configuration
    if (merge(root, src))
    {
        // Move new object to config
        config.load(root);
        saveFile();
        return true;
    }

    return false;
}

bool merge(JsonVariant dst, JsonVariantConst src)
{
    if (src.is<JsonObject>())
    {
        for (auto kvp : src.as<JsonObjectConst>())
        {
            merge(dst.getOrAddMember(kvp.key()), kvp.value());
        }
    }
    else
    {
        dst.set(src);
    }
    return true;
}

void ApConfig::save(JsonObject obj) const
{
    obj["ssid"] = ssid;
    obj["passphrase"] = passphrase;
}

void ApConfig::load(JsonObjectConst obj)
{
    // Load Access Point configuration
    //
    if (obj["ssid"].isNull())
    {
        strlcpy(ssid, APNAME, sizeof(ssid));
    }
    else
    {
        const char *sd = obj["ssid"];
        strlcpy(ssid, sd, sizeof(ssid));
    }

    if (obj["passphrase"].isNull())
    {
        strlcpy(passphrase, APPWD, sizeof(passphrase));
    }
    else
    {
        const char *ps = obj["passphrase"];
        strlcpy(passphrase, ps, sizeof(passphrase));
    }
}

void URLTarget::save(JsonObject obj) const
{
    obj["url"] = url;
    obj["freq"] = freq;
    obj["update"] = update;
}

void URLTarget::load(JsonObjectConst obj)
{
    // Load URL Target configuration
    //
    if (obj["url"].isNull())
    {
        strlcpy(url, "", sizeof(url));
    }
    else
    {
        const char *tu = obj["url"];
        strlcpy(url, tu, sizeof(url));
    }

    if (obj["freq"].isNull())
    {
        freq = 2;
    }
    else
    {
        int f = obj["freq"];
        freq = f;
    }

    if (obj["update"].isNull())
    {
        update = false;
    }
    else
    {
        bool u = obj["update"];
        update = u;
    }
}

void KeyTarget::save(JsonObject obj) const
{
    obj["channel"] = channel;
    obj["key"] = key;
    obj["freq"] = freq;
    obj["update"] = update;
}

void KeyTarget::load(JsonObjectConst obj)
{
    // Load Key-type configuration
    //
    if (obj["channel"].isNull())
    {
        channel = 0;
    }
    else
    {
        int c = obj["channel"];
        channel = c;
    }

    if (obj["key"].isNull())
    {
        strlcpy(key, "", sizeof(key));
    }
    else
    {
        const char *k = obj["key"];
        strlcpy(key, k, sizeof(key));
    }

    if (obj["freq"].isNull())
    {
        freq = 15;
    }
    else
    {
        int f = obj["freq"];
        freq = f;
    }

    if (obj["update"].isNull())
    {
        update = false;
    }
    else
    {
        bool u = obj["update"];
        update = u;
    }
}
void iSpindHub::save(JsonObject obj) const
{
    obj["name"] = name;
    obj["TMZ"] = TMZ;
}

void iSpindHub::load(JsonObjectConst obj)
{
    // Load iSpindHub configuration
    //

    if (obj["name"].isNull())
    {
        strlcpy(name, APNAME, sizeof(name));
    }
    else
    {
        const char *nm = obj["name"];
        strlcpy(name, nm, sizeof(name));
    }

    if (obj["TMZ"].isNull())
    {
        strlcpy(TMZ, "CET", sizeof(TMZ));
    }
    else
    {
        const char *tm = obj["TMZ"];
        strlcpy(TMZ,tm, sizeof(TMZ));
    }
}

void Config::load(JsonObjectConst obj)
{
    // Load all config objects
    //

    apconfig.load(obj["apconfig"]);
    ispindhub.load(obj["ispindhub"]);
    urltarget.load(obj["urltarget"]);
    brewersfriend.load(obj["brewersfriend"]);
    brewfather.load(obj["brewfather"]);
}

void Config::save(JsonObject obj) const
{
    // Add Access Point object
    apconfig.save(obj.createNestedObject("apconfig"));
    // Add iSpindHub object
    ispindhub.save(obj.createNestedObject("ispindhub"));
    // Add Target object
    urltarget.save(obj.createNestedObject("urltarget"));
    // Add BrewPiLess object
    bpiless.save(obj.createNestedObject("bpiless"));
    // Add Brewer's Friend object
    brewersfriend.save(obj.createNestedObject("brewersfriend"));
    // Add Brewfather object
    brewfather.save(obj.createNestedObject("brewfather"));
}


