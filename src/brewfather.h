#ifndef _BREWFTARGET_H
#define _BREWFTARGET_H

#include "pushtarget.h"
#include "pushhelper.h"
#include "jsonconfig.h"
#include <LCBUrl.h>
#include <ArduinoLog.h>
#include <Arduino.h>

class BrewfTarget
{
private:
    // Singleton Declarations
    BrewfTarget() {}
    static BrewfTarget *single;
    // External Declarations
    PushTarget *target;
    // Private Methods

    // Private Properties

    /////////////////////////////////////////////////////////////////////
    //  Configure Target - Below are configuration items per target type
    /////////////////////////////////////////////////////////////////////

    // Enable target and target name
    const bool target_enabled = true;
    String target_name = "Brewfather";
    //
    // Check return body for success
    const bool checkbody_enabled = true;
    String checkbody_name = "200";
    //
    // Turn JSON points on/off and provide JSON field name per target type
    //
    const bool apiname_enabled = false;
    String apiname_name = "";
    //
    const bool bubname_enabled = true;
    String bubname_name = "name";
    //
    const bool bpm_enabled = true;
    String bpm_name = "bpm";
    //
    const bool ambienttemp_enabled = true;
    String ambienttemp_name = "aux_temp";
    //
    const bool vesseltemp_enabled = true;
    String vesseltemp_name = "temp";
    //
    const bool tempformat_enabled = true;
    String tempformat_name = "temp_unit";
    //
    // Main URL for endpoint
    String targeturl = "http://log.brewfather.net/stream?id=";
    //
    const bool apikey_enabled = true;
    String apikey_name = ""; // Will pick this up from config

    /////////////////////////////////////////////////////////////////////
    //  Configure Target - Above are configuration items per target type
    /////////////////////////////////////////////////////////////////////

public:
    // Singleton Declarations
    static BrewfTarget *getInstance();
    ~BrewfTarget() { single = NULL; }
    // External Declarations

    // Public Methods
    bool push();
    // Public Properties
};

extern struct Config config;

#endif // _BREWFTARGET_H
