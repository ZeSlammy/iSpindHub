#include "wifi.h"
void doWiFi() {
    doWiFi(false);
}
void doWiFi(bool ignore = false) {
    AsyncWiFiManager wm;
    wm.setDebugOutput(true); 
    if (ignore){
        wm.startConfigPortal("iSpindHubConfig");
    }
    else{
        
    }
}
