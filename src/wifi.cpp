#include "wifi.h"
bool shouldSaveConfig = false;
volatile bool wifiLost = false;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
AsyncDNSServer dnsServer;

// Use the single AsyncWebServer instance from web.cpp to avoid having two
// listeners on port 80 (which causes HTTP 500 on all our routes).
extern AsyncWebServer server;
ESPAsync_WiFiManager wm(&server, &dnsServer, "iSpindHub");

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
        // ESPAsync_WiFiManager v1.15.1 crashes (Exception 28) when the stored
        // WiFi password is shorter than WPA minimum (8 chars) instead of
        // falling back to the config portal.  Pre-check here and erase bad
        // credentials so autoConnect opens a fresh portal instead of crashing.
        if (!WiFi.SSID().isEmpty() && WiFi.psk().length() > 0 && WiFi.psk().length() < 8) {
            Log.warning(F("Stored WiFi password too short (%d chars) — clearing to prevent WiFiManager crash." CR),
                        (int)WiFi.psk().length());
            WiFi.persistent(true);
            WiFi.disconnect(true); // erase stored SSID/PSK
        }
        // WiFi.disconnect() on ESP8266 with persistent=true (the default) calls
        // wifi_station_set_config() with a zeroed struct, which writes blank
        // credentials to flash.  ESPAsync_WiFiManager calls WiFi.disconnect()
        // internally before every connection attempt, so without this guard it
        // erases credentials on every boot.  Setting persistent=false makes all
        // internal disconnects only modify the in-memory config, leaving flash
        // untouched.  We restore persistent=true explicitly when we need to
        // write (new portal credentials, or the short-password erase above).
        WiFi.persistent(false);
        if (!wm.autoConnect(APNAME, APPWD))
        {
            Log.warning(F("Failed to connect and/or hit timeout." CR));
            Log.warning(F("Restarting." CR));
            ESP.restart();
        }
        else
        {
            if (shouldSaveConfig)
            {
                // New credentials were entered in the config portal.
                // Persist them to flash now that we know they work.
                WiFi.persistent(true);
                WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
                WiFi.persistent(false);
                Log.notice(F("New WiFi credentials saved to flash." CR));
            }

            Log.notice(F("Get In Set HostName" CR));
            if (strlen(config.ispindhub.name) == 0)
            {
                Log.notice(F(HOSTNAME CR));
                WiFi.hostname(HOSTNAME);
                WiFi.softAP(HOSTNAME, config.apconfig.passphrase);
            }
            else
            {
                Log.notice("We use the information from the config file");
                Log.notice(F(CR));
                Log.notice(config.ispindhub.name);
                Log.notice(F(CR));
                WiFi.hostname(config.ispindhub.name);
                WiFi.softAP(config.ispindhub.name, config.apconfig.passphrase);
            }

            Log.notice(F("Get Out Set HostName" CR));
            saveConfig();
        }
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

// Called in ESP8266 system task context (ctx: sys) — keep it minimal.
// Blocking operations here (delay, WiFi.begin, malloc) cause Exception 9.
// Set a flag and let handleWifiReconnect() do the real work from loop().
void WiFiEvent(WiFiEvent_t event)
{
    if (!WiFi.isConnected())
    {
        wifiLost = true;
    }
}

void handleWifiReconnect()
{
    if (!wifiLost) return;
    wifiLost = false;
    if (WiFi.isConnected()) return; // auto-reconnected before loop() ran

    Log.warning(F("WiFi lost connection, attempting reconnect." CR));
    WiFi.begin();

    int WLcount = 0;
    while (!WiFi.isConnected() && WLcount < 190)
    {
        delay(100);
        Serial.print(".");
        ++WLcount;
    }
    Serial.println();

    if (!WiFi.isConnected())
    {
        Log.error(F("Unable to reconnect WiFi, restarting." CR));
        delay(1000);
        ESP.restart();
    }
    else
    {
        bool isdeployed;
        if (strlen(config.ispindhub.name) == 0)
        {
            isdeployed = WiFi.softAP(APNAME, config.apconfig.passphrase);
        }
        else
        {
            isdeployed = WiFi.softAP(config.ispindhub.name, config.apconfig.passphrase);
        }

        if (!isdeployed)
        {
            Log.error(F("Unable to start softAP, restarting." CR));
            delay(1000);
            ESP.restart();
        }
    }
}