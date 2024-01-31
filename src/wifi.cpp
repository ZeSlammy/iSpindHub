#include "wifi.h"
bool shouldSaveConfig = false;
const byte DNS_PORT = 53;
#define HTTP_PORT 80
IPAddress apIP(192, 168, 4, 1);
AsyncWebServer webServer(HTTP_PORT);
AsyncDNSServer dnsServer;

ESPAsync_WiFiManager wm(&webServer, &dnsServer, "iSpindHub");

void doWiFi()
{
    doWiFi(false);
}
void doWiFi(bool dontUseStoredCreds)
{
    // wm.setConfigPortalBlocking(false);
    // wm.setConfigPortalBlocking(true);
    WiFi.mode(WIFI_AP_STA); // explicitly set mode, esp defaults to STA+AP
    WiFi.setAutoReconnect(true);
    // AsyncWiFiManager Callbacks
    wm.setAPCallback(apCallback); // Called after AP has started
    // wm.setConfigResetCallback(configResetCallback); // Called after settings are reset
    // wm.setPreSaveConfigCallback(preSaveConfigCallback); // Called before saving wifi creds
    wm.setSaveConfigCallback(saveConfigCallback); //  Called only if wifi is saved/changed, or setBreakAfterConfig(true)
    // wm.setSaveParamsCallback(saveParamsCallback); // Called after parameters are saved via params menu or wifi config
    // wm.setWebServerCallback(webServerCallback); // Called after webserver is setup
    wm.setDebugOutput(true); // Verbose debug is enabled by default
    std::vector<const char *> _wfmPortalMenu = {
        "wifi",
        "wifinoscan",
        "sep",
        "info",
        //"param",
        //"close",
        "erase",
        "restart",
        "exit"};
    // wm.setMenu(_wfmPortalMenu); // Set menu items
    // wm.setCountry(WIFI_COUNTRY);    // Setting wifi country seems to improve OSX soft ap connectivity
    Log.notice(F("Set COUNTRY OK" CR));
    // wm.setWiFiAPChannel(WIFI_CHAN); // Set WiFi channel
    // wm.setShowStaticFields(true); // Force show static ip fields
    // wm.setShowDnsFields(true);
    if (dontUseStoredCreds)
    {
        // Voluntary portal
        // blinker.attach_ms(APBLINK, wifiBlinker);
        wm.setConfigPortalTimeout(120);

        if (wm.startConfigPortal(APCONFNAME, APCONFPWD))
        {
            // We finished with portal, do we need this?
        }
        else
        {
            // Hit timeout on voluntary portal
            delay(3000);
            Log.notice(F("Hit timeout for on-demand portal, exiting." CR));
            // ESP.restart();
        }
    }
    else
    { // Normal WiFi connection attempt
        // blinker.attach_ms(STABLINK, wifiBlinker);
        wm.setConnectTimeout(30);
        wm.setConfigPortalTimeout(120);
        if (!wm.autoConnect(APNAME, APPWD))
        {
            Log.warning(F("Failed to connect and/or hit timeout." CR));
            Log.warning(F("Restarting." CR));
            ESP.restart();
        }
        else
        {
            // We finished with portal (We were configured)
            Log.notice(F("Get In Set HostName" CR));
            if (strlen(config.ispindhub.name) == 0)
            {
                Log.notice(F(HOSTNAME CR));
                // WiFi.setHostname(HOSTNAME);
                WiFi.hostname(HOSTNAME);
                WiFi.softAP(HOSTNAME);
            }
            else
            {
                Log.notice("We use the information from the config file");
                Log.notice(F(CR));
                Log.notice(config.ispindhub.name);
                Log.notice(F(CR));
                WiFi.hostname(config.ispindhub.name);
                WiFi.softAP(config.ispindhub.name);
            }

            Log.notice(F("Get Out Set HostName" CR));
            saveConfig();
        }
    }
    if (shouldSaveConfig)
    { // Save configuration
      // Save configuration
    }

    Log.notice(F("Connected. IP address: %s." CR), WiFi.localIP().toString().c_str());
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    Log.verbose(F("Soft AP started, IP: %s" CR), WiFi.softAPIP().toString().c_str());
    WiFi.onEvent(WiFiEvent);
}

void resetWifi()
{ // Wipe WiFi settings and reset controller
    WiFi.disconnect();
    wm.resetSettings();
    Log.notice(F("Restarting after clearing wifi settings." CR));
    saveConfig();
    _delay(100);
    ESP.restart();
}

void apCallback(ESPAsync_WiFiManager *asyncWiFiManager)
{ // Entered Access Point mode
    Log.verbose(F("[CALLBACK]: setAPCallback fired." CR));
    Log.notice(F("Entered portal mode; name: %s, IP: %s." CR),
               asyncWiFiManager->getConfigPortalSSID().c_str(),
               WiFi.localIP().toString().c_str());
}

void saveConfigCallback()
{
    Log.verbose(F("[CALLBACK]: setSaveConfigCallback fired." CR));
    shouldSaveConfig = true;
}

void saveParamsCallback()
{
    Log.verbose(F("[CALLBACK]: setSaveParamsCallback fired." CR));
}

// void webServerCallback() {
//     Log.verbose(F("[CALLBACK]: setWebServerCallback fired." CR));
// }

void WiFiEvent(WiFiEvent_t event)
{
    Log.notice(F("[WiFi-event] event: %d" CR), event);
    if (!WiFi.isConnected())
    {
        Log.warning(F("WiFi lost connection.."));
        WiFi.begin();

        int WLcount = 0;
        while (!WiFi.isConnected() && WLcount < 190)
        {
            delay(100);
            Serial.print(".");
            ++WLcount;
        }

        if (!WiFi.isConnected())
        {
            // We failed to reconnect.
            Log.error(F("Unable to reconnect WiFI, restarting." CR));
            delay(1000);
            ESP.restart();
        }
        else
        {
            bool isdeployed;
            if (strlen(config.ispindhub.name) == 0)
            {
                isdeployed = WiFi.softAP(APNAME);
            }
            else
            {
                isdeployed = WiFi.softAP(config.ispindhub.name);
            }

            if (!isdeployed)
            {
                Log.error(F("Unable to start softAP, restarting." CR));
                delay(1000);
                ESP.restart();
            }
        }
        Serial.println();
    }
}