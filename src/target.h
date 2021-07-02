
#ifndef _URLTARGET_H
#define _URLTARGET_H

#include "pushtarget.h"
#include "pushhelper.h"
#include "jsonconfig.h"
#include <LCBUrl.h>
#include <ArduinoLog.h>
#include <Arduino.h>

class Target
{
private:
    // Singleton Declarations
    Target() {}
    static Target *single;
    // External Declarations
    PushTarget *target;
    // Private Methods

    // Private Properties

    /////////////////////////////////////////////////////////////////////
    //  Configure Target - Below are configuration items per target type
    /////////////////////////////////////////////////////////////////////

    // Enable target and target name
    const bool target_enabled = true;
    String target_name = "URL Target";
    //
    // Check return body for success
    const bool checkbody_enabled = false;
    String checkbody_name = "";
    //
    // Turn JSON points on/off and provide JSON field name per target type
    //
    const bool apiname_enabled = true;
    String apiname_name = "api_name";
    //
    const bool bubname_enabled = true;
    String bubname_name = "name";
    //
    const bool bpm_enabled = true;
    String bpm_name = "bpm";
    //
    const bool ambienttemp_enabled = true;
    String ambienttemp_name = "ambient";
    //
    const bool vesseltemp_enabled = true;
    String vesseltemp_name = "temp";
    //
    const bool tempformat_enabled = true;
    String tempformat_name = "temp_unit";
    //
    // Main URL for endpoint
    String targeturl = "";
    //
    const bool apikey_enabled = true;
    String apikey_name = ""; // Will pick this up from config

    /////////////////////////////////////////////////////////////////////
    //  Configure Target - Above are configuration items per target type
    /////////////////////////////////////////////////////////////////////

public:
    // Singleton Declarations
    static Target *getInstance();
    ~Target() { single = NULL; }
    // External Declarations

    // Public Methods
    bool push();
    // Public Properties
};

extern struct Config config;

#endif // _URLTARGET_H
