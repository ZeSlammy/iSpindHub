#include "wifi.h"
AsyncWiFiManager wm;
void doWiFi() {
    doWiFi(false);
}
void doWiFi(bool ignore) {
    // AsyncWiFiManager Callbacks
    //WiFi.enableSTA(true);
    //WiFi.setSleepMode(WIFI_NONE_SLEEP);
    
    wm.setCleanConnect(true); // Always disconnect before connecting
    wm.setAPCallback(apCallback);                       // Called after AP has started
    wm.setDebugOutput(true); 
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
    wm.setMenu(_wfmPortalMenu); // Set menu items
    wm.setShowStaticFields(true); // Force show static ip fields
    wm.setShowDnsFields(true);
    wm.startConfigPortal("iSpindHubConfig");
    wm.setConnectTimeout(30);
    wm.setConfigPortalTimeout(120);
    if (!wm.autoConnect(config.apconfig.ssid, config.apconfig.passphrase))
        {
            Log.warning(F("Failed to connect and/or hit timeout." CR));
            Log.warning(F("Restarting." CR));
            _delay(100);
            ESP.restart();
        }
        else
        {
            
        }
}

void resetWifi()
{ // Wipe WiFi settings and reset controller
    wm.resetSettings();
    Log.notice(F("Restarting after clearing WiFi settings." CR));
    ESP.restart();
}

void apCallback(AsyncWiFiManager *asyncWiFiManager)
{ // Entered Access Point mode
    Log.verbose(F("[CALLBACK]: setAPCallback fired." CR));
    Log.notice(F("Entered portal mode; name: %s, IP: %s." CR),
               asyncWiFiManager->getConfigPortalSSID().c_str(),
               WiFi.localIP().toString().c_str());
}