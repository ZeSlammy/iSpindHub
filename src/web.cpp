#include "web.h"
AsyncWebServer server(80);
extern Adafruit_ST7735 tft;
const char *filename = "/config.json";
Config config;

extern const size_t capacitySerial = 3 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 3 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(11);
extern const size_t capacityDeserial = capacitySerial + 810;

void initWebServer()
{
    setActionPageHandlers();
    setJsonHandlers();
    setSettingsAliases();
    server.onNotFound([](AsyncWebServerRequest *request) {
        Log.verbose(F("Serving 404." CR));
        request->send(404, F("text/plain"), F("404: File not found."));
    });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.begin();

    Log.notice(F("Async HTTP server started on port 80" CR));
}
void setActionPageHandlers()
{   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Log.notice("Connected on /");
        request->send(200, F("text/plain"), "Hello World");
    });

}
void setJsonHandlers()
{
        server.on("/ispindel", HTTP_POST, [](AsyncWebServerRequest *request) {
        // Used to handle the json coming from iSpindel
         }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
            for (size_t i = 0; i < len; i++) {
                Serial.write(data[i]);
            }

            StaticJsonDocument<300> jdoc;
            //ReadLoggingStream loggingStream(data, Serial);
            //DeserializationError error = deserializeJson(jdoc, loggingStream);
            DeserializationError error = deserializeJson(jdoc, (const char*)data);
            Log.verbose(("Parsing json from ispindel.\n"));
            if (!error) {
                //tft.fillRect(0,0,128,20,ST7735_BLACK);
                // Get Data from JSON 
                float data_size = jdoc.size();
                const char* name = jdoc["name"];
                //long id = jdoc["ID"];
                float angle = jdoc["angle"];
                const char* t_unit = jdoc["temp_units"];
                float temp = jdoc["temperature"];
                float battery = jdoc["battery"];
                // Output iSpindel Name
                //Serial.println("name of file");
                //Serial.println(name);
                //Serial.println(temp);
                
                // Build and open file
                String fname = String("/data/") + String(name) + String(".csv");
                //Serial.println(fname);
                File iSpindLog = LittleFS.open(fname, "a");    
                iSpindLog.print(getDTS());
                iSpindLog.print(",");
                iSpindLog.print(name);
                iSpindLog.print(",");
                iSpindLog.print(angle,4);
                iSpindLog.print(",");
                iSpindLog.print(temp,2);
                iSpindLog.print(",");
                iSpindLog.print(battery,4);
                
                // Check latest data Vs current TODO
                if (data_size > 5){
                    float gravity = jdoc["gravity"];
                    iSpindLog.print(",");
                    iSpindLog.print(gravity,4);
                    float interval = jdoc["interval"];
                    iSpindLog.print(",");
                    iSpindLog.print(interval,4);
                    float rssi = jdoc["RSSI"];
                    iSpindLog.print(",");
                    iSpindLog.print(rssi,4);
                    iSpindLog.print(",");
                    iSpindLog.write(t_unit);
                    iSpindLog.print(",");
                    iSpindLog.print("\r");
                    iSpindLog.close();
                }
                else{
                    iSpindLog.print(",");
                    iSpindLog.print(",");
                    iSpindLog.print(",");
                    iSpindLog.print(",");
                    iSpindLog.write(t_unit);
                    iSpindLog.print(",");
                    iSpindLog.print("\r");
                    iSpindLog.close();
                }
                
                request->send(200, "text/plain", "");
            } else {
            Log.verbose(F("deserializeJson() failed: "));
            Log.verbose(error.f_str());
            request->send(404, "text/plain", "");
            }
    });
};


void setSettingsAliases()
{
        server.on("/settings/urltarget/", HTTP_POST, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing post to /settings/urltarget/." CR));
        if (handleURLTargetPost(request))
        {
            request->send(200, F("text/plain"), F("Ok"));
        }
        else
        {
            request->send(500, F("text/plain"), F("Unable to process data"));
        }
    });

    server.on("/settings/urltarget/", HTTP_ANY, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Invalid method to /settings/tapcontrol/." CR));
        request->send(405, F("text/plain"), F("Method not allowed."));
    });


    server.on("/settings/brewfathertarget/", HTTP_POST, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing post to /settings/brewersfriendtarget/." CR));
        if (handleBrewfatherTargetPost(request))
        {
            request->send(200, F("text/plain"), F("Ok"));
        }
        else
        {
            request->send(500, F("text/plain"), F("Unable to process data"));
        }
    });

    server.on("/settings/brewfathertarget/", HTTP_ANY, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Invalid method to /settings/brewersfriendtarget/." CR));
        request->send(405, F("text/plain"), F("Method not allowed."));
    });

}
bool handleURLTargetPost(AsyncWebServerRequest *request) // Handle URL Target Post
{
    // Loop through all parameters
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isPost())
        {
            // Process any p->name().c_str() / p->value().c_str() pairs
            const char *name = p->name().c_str();
            const char *value = p->value().c_str();
            Log.verbose(F("Processing [%s]:(%s) pair." CR), name, value);

            // URL Target settings
            //
            if (strcmp(name, "urltargeturl") == 0) // Change Target URL
            {
                if (strlen(value) == 0)
                {
                    Log.notice(F("Settings update, [%s]:(%s) applied.  Disabling Url Target." CR), name, value);
                    strlcpy(config.urltarget.url, value, sizeof(config.urltarget.url));
                }
                else if ((strlen(value) < 3) || (strlen(value) > 128))
                {
                    Log.warning(F("Settings update error, [%s]:(%s) not applied." CR), name, value);
                }
                else
                {
                    Log.notice(F("Settings update, [%s]:(%s) applied." CR), name, value);
                    strlcpy(config.urltarget.url, value, sizeof(config.urltarget.url));
                }
            }
            if (strcmp(name, "urlfreq") == 0) // Change Target URL frequency
            {
                if ((atoi(value) < 1) || (atoi(value) > 60))
                {
                    Log.warning(F("Settings update error, [%s]:(%s) not applied." CR), name, value);
                }
                else
                {
                    Log.notice(F("Settings update, [%s]:(%s) applied." CR), name, value);
                    config.urltarget.freq = atoi(value);
                    config.urltarget.update = true;
                }
            }
        }
    }
    if (saveConfig())
    {
        return true;
    }
    else
    {
        Log.error(F("Error: Unable to save tap configuration data." CR));
        return false;
    }
}

bool handleBrewfatherTargetPost(AsyncWebServerRequest *request) // Handle Brewfather Target Post
{
    // Loop through all parameters
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isPost())
        {
            // Process any p->name().c_str() / p->value().c_str() pairs
            const char *name = p->name().c_str();
            const char *value = p->value().c_str();
            Log.verbose(F("Processing [%s]:(%s) pair." CR), name, value);

            // Brewer's Friend target settings
            //
            if (strcmp(name, "brewfatherkey") == 0) // Change Brewfather key
            {
                if (strlen(value) == 0)
                {
                    Log.notice(F("Settings update, [%s]:(%s) applied. Disabling Brewfather target." CR), name, value);
                    strlcpy(config.brewfather.key, value, sizeof(config.brewfather.key));
                }
                else if ((strlen(value) < 10) || (strlen(value) > 64))
                {
                    Log.warning(F("Settings update error, [%s]:(%s) not applied." CR), name, value);
                }
                else
                {
                    Log.notice(F("Settings update, [%s]:(%s) applied." CR), name, value);
                    strlcpy(config.brewfather.key, value, sizeof(config.brewfather.key));
                }
            }
            if (strcmp(name, "brewfatherfreq") == 0) // Change Brewfather frequency
            {
                if ((atoi(value) < 15) || (atoi(value) > 120))
                {
                    Log.warning(F("Settings update error, [%s]:(%s) not applied." CR), name, value);
                }
                else
                {
                    Log.notice(F("Settings update, [%s]:(%s) applied." CR), name, value);
                    config.brewfather.freq = atoi(value);
                    config.brewfather.update = true;
                }
            }
        }
    }
    if (saveConfig())
    {
        return true;
    }
    else
    {
        Log.error(F("Error: Unable to save tap configuration data." CR));
        return false;
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

void Config::save(JsonObject obj) const
{
    // Add Target object
    urltarget.save(obj.createNestedObject("urltarget"));
    // Add Brewfather object
    brewfather.save(obj.createNestedObject("brewfather"));
}

void Config::load(JsonObjectConst obj)
{
    // Load all config objects
    urltarget.load(obj["urltarget"]);
    brewfather.load(obj["brewfather"]);
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
