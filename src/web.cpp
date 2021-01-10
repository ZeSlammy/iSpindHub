#include "web.h"
AsyncWebServer server(80);
extern Adafruit_ST7735 tft;
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
void setSettingsAliases()
{
    server.on("/settings/controller/", HTTP_POST, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing post to /settings/controller/." CR));
        if (handleControllerPost(request))
        {
            request->send(200, F("text/plain"), F("Ok"));
        }
        else
        {
            request->send(500, F("text/plain"), F("Unable to process data"));
        }
    });

    server.on("/settings/controller/", HTTP_ANY, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Invalid method to /settings/tapcontrol/." CR));
        request->send(405, F("text/plain"), F("Method not allowed."));
    });

    server.on("/settings/temperature/", HTTP_POST, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing post to /settings/tempcontrol/." CR));
        if (handleTemperaturePost(request))
        {
            request->send(200, F("text/plain"), F("Ok"));
        }
        else
        {
            request->send(500, F("text/plain"), F("Unable to process data"));
        }
    });

    server.on("/settings/temperature/", HTTP_ANY, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Invalid method to /settings/tapcontrol/." CR));
        request->send(405, F("text/plain"), F("Method not allowed."));
    });

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

    server.on("/settings/brewersfriendtarget/", HTTP_POST, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing post to /settings/brewersfriendtarget/." CR));
        if (handleBrewersFriendTargetPost(request))
        {
            request->send(200, F("text/plain"), F("Ok"));
        }
        else
        {
            request->send(500, F("text/plain"), F("Unable to process data"));
        }
    });

    server.on("/settings/brewersfriendtarget/", HTTP_ANY, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Invalid method to /settings/brewersfriendtarget/." CR));
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

    server.on("/settings/thingspeaktarget/", HTTP_POST, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing post to /settings/thingspeaktarget/." CR));
        if (handleThingSpeakTargetPost(request))
        {
            request->send(200, F("text/plain"), F("Ok"));
        }
        else
        {
            request->send(500, F("text/plain"), F("Unable to process data"));
        }
    });

    server.on("/settings/thingspeaktarget/", HTTP_ANY, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Invalid method to /settings/thingspeaktarget/." CR));
        request->send(405, F("text/plain"), F("Method not allowed."));
    });
}

}