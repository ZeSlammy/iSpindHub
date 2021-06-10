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

#include "uptime.h"

static int refresh = UPTIME_REFRESH * 1000;
static unsigned long uptimeNow;
static int days;
static int hours;
static int minutes;
static int seconds;
static int mills;

void getNow()
{
    // Set the uptime values if refresh time is expired
    if ((int)(millis() - uptimeNow) > refresh)
    {
        setValues();
    }
    // Reset timer for another period to avoid a really unlikely situation
    // where the timer expires in between grabbing two parts
    uptimeNow = millis();
}

void setValues()
{
    // Call this only by getNow()
    // Using refr = true forces recalculation
    uptimeNow = millis();
    days = uptimeDays(true);
    hours = uptimeHours(true);
    minutes = uptimeMinutes(true);
    seconds = uptimeSeconds(true);
    mills = uptimeMillis(true);
}

const int uptimeDays(bool refr)
{
    getNow(); // Make sure we are current
    if (refr)
    {
        // Calculate full days from uptime
        days = (int)floor(uptimeNow / DAY_MILLIS);
    }
    return days;
}

const int uptimeHours(bool refr)
{
    getNow(); // Make sure we are current
    if (refr)
    {
        // Refresh values:
        // Subtract millis value for any full days via modulo
        // Calculate full hours from remainder
        hours = (int)floor((uptimeNow % DAY_MILLIS) / HOUR_MILLIS);
    }
    return hours;
}

const int uptimeMinutes(bool refr)
{
    getNow(); // Make sure we are current
    if (refr)
    {
        // Refresh values:
        // Subtract millis value for any full hours via modulo
        // Calculate full minutes from remainder
        minutes = (int)floor((uptimeNow % HOUR_MILLIS) / MIN_MILLIS);
    }
    return minutes;
}

const int uptimeSeconds(bool refr)
{
    getNow(); // Make sure we are current
    if (refr)
    {
        // Refresh values:
        // Subtract millis value for any full minutes via modulo
        // Calculate full seconds from remainder
        seconds = (int)floor((uptimeNow % MIN_MILLIS) / SEC_MILLIS);
    }
    return seconds;
}

const int uptimeMillis(bool refr)
{
    getNow(); // Make sure we are current
    if (refr)
    {
        // Refresh values:
        // Subtract millis value for any full seconds via modulo
        // Return remainder millis
        mills = (int)floor((uptimeNow % SEC_MILLIS));
    }
    return mills;
}
