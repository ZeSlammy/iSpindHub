/* Copyright (C) 2019-2021 Lee C. Bussy (@LBussy)

This file is part of Lee Bussy's Brew Bubbles (brew-bubbles).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#ifndef _PUSHTARGET_H
#define _PUSHTARGET_H

#include <ESP8266WiFi.h>

struct pushPoint
{
    bool enabled;   // Whether to send or not
    char name[129]; // What to call the data point
};

class PushTarget
{
private:
public:
    PushTarget(){};
    pushPoint target;      // Target enabled and name
    pushPoint checkBody;   // Check return body for success
    char url[129];         // URL of target
    pushPoint key;         // API Key
    IPAddress ip;          // Resolved address of target
    pushPoint apiName;     // i.e. "Brew Bubbles"
    pushPoint bubName;     // mDNS name i.e. "brewbubbles"
    pushPoint lastTime;    // Time of last send
    pushPoint tempFormat;  // F or C
    pushPoint ambientTemp; // Room or chamber temp
    pushPoint vesselTemp;  // Brew temp
    pushPoint bpm;         // Bubbles per minute
};

#endif // _PUSHTARGET_H
