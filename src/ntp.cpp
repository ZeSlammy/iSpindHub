
#include "ntp.h"

// Maps IANA timezone name → POSIX TZ string (handles DST automatically).
static const char* ianaToPosx(const char* iana)
{
    struct { const char* iana; const char* posix; } table[] = {
        {"UTC",                  "UTC0"},
        {"Europe/London",        "GMT0BST,M3.5.0/1,M10.5.0"},
        {"Europe/Dublin",        "IST-1GMT0,M10.5.0,M3.5.0/1"},
        {"Europe/Lisbon",        "WET0WEST,M3.5.0/1,M10.5.0"},
        {"Europe/Paris",         "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Berlin",        "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Amsterdam",     "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Brussels",      "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Madrid",        "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Rome",          "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Zurich",        "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Vienna",        "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Stockholm",     "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Copenhagen",    "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Oslo",          "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Warsaw",        "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Prague",        "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Budapest",      "CET-1CEST,M3.5.0,M10.5.0/3"},
        {"Europe/Helsinki",      "EET-2EEST,M3.5.0/3,M10.5.0/4"},
        {"Europe/Athens",        "EET-2EEST,M3.5.0/3,M10.5.0/4"},
        {"Europe/Bucharest",     "EET-2EEST,M3.5.0/3,M10.5.0/4"},
        {"Europe/Kyiv",          "EET-2EEST,M3.5.0/3,M10.5.0/4"},
        {"Europe/Moscow",        "MSK-3"},
        {"Europe/Istanbul",      "<+03>-3"},
        {"America/New_York",     "EST5EDT,M3.2.0,M11.1.0"},
        {"America/Toronto",      "EST5EDT,M3.2.0,M11.1.0"},
        {"America/Chicago",      "CST6CDT,M3.2.0,M11.1.0"},
        {"America/Denver",       "MST7MDT,M3.2.0,M11.1.0"},
        {"America/Phoenix",      "MST7"},
        {"America/Los_Angeles",  "PST8PDT,M3.2.0,M11.1.0"},
        {"America/Vancouver",    "PST8PDT,M3.2.0,M11.1.0"},
        {"America/Anchorage",    "AKST9AKDT,M3.2.0,M11.1.0"},
        {"America/Honolulu",     "HST10"},
        {"America/Sao_Paulo",    "<-03>3<-02>,M10.3.0,M2.3.0"},
        {"America/Argentina/Buenos_Aires", "<-03>3"},
        {"America/Santiago",     "<-04>4<-03>,M10.2.6/24,M3.2.6/24"},
        {"Australia/Sydney",     "AEST-10AEDT,M10.1.0,M4.1.0/3"},
        {"Australia/Melbourne",  "AEST-10AEDT,M10.1.0,M4.1.0/3"},
        {"Australia/Brisbane",   "AEST-10"},
        {"Australia/Adelaide",   "ACST-9:30ACDT,M10.1.0,M4.1.0/3"},
        {"Australia/Perth",      "AWST-8"},
        {"Asia/Tokyo",           "JST-9"},
        {"Asia/Seoul",           "KST-9"},
        {"Asia/Shanghai",        "CST-8"},
        {"Asia/Hong_Kong",       "HKT-8"},
        {"Asia/Singapore",       "SGT-8"},
        {"Asia/Taipei",          "CST-8"},
        {"Asia/Bangkok",         "<+07>-7"},
        {"Asia/Jakarta",         "WIB-7"},
        {"Asia/Kolkata",         "IST-5:30"},
        {"Asia/Karachi",         "PKT-5"},
        {"Asia/Dubai",           "<+04>-4"},
        {"Asia/Riyadh",          "<+03>-3"},
        {"Asia/Baghdad",         "<+03>-3"},
        {"Asia/Tehran",          "<+0330>-3:30<+0430>,80/0,264/0"},
        {"Asia/Yekaterinburg",   "<+05>-5"},
        {"Asia/Almaty",          "<+06>-6"},
        {"Africa/Johannesburg",  "SAST-2"},
        {"Africa/Cairo",         "EET-2"},
        {"Africa/Lagos",         "WAT-1"},
        {"Africa/Nairobi",       "EAT-3"},
        {"Pacific/Auckland",     "NZST-12NZDT,M9.5.0,M4.1.0/3"},
        {"Pacific/Fiji",         "<+12>-12"},
        {"Pacific/Honolulu",     "HST10"},
        {NULL, NULL}
    };
    for (int i = 0; table[i].iana != NULL; i++)
    {
        if (strcmp(iana, table[i].iana) == 0)
            return table[i].posix;
    }
    return NULL;
}

// Legacy fallback: maps old abbreviation-style TZ values to UTC offset in seconds.
static long tzAbbrevToOffsetSeconds(const char* tz)
{
    struct { const char* name; long offset; } table[] = {
        {"GMT", 0}, {"UTC", 0},
        {"ECT", 3600}, {"CET", 3600}, {"CEST", 7200},
        {"EET", 7200}, {"EEST", 10800},
        {"ART", 7200},
        {"EAT", 10800},
        {"MET", 12600},
        {"NET", 14400},
        {"PLT", 18000},
        {"IST", 19800},
        {"BST", 21600},
        {"VST", 25200},
        {"CTT", 28800},
        {"JST", 32400},
        {"ACT", 34200},
        {"AET", 36000},
        {"SST", 39600},
        {"NST", 43200},
        {"MIT", -39600},
        {"HST", -36000},
        {"AST", -32400},
        {"PST", -28800},
        {"PDT", -25200},
        {"PNT", -25200}, {"MST", -25200},
        {"MDT", -21600},
        {"CST", -21600},
        {"CDT", -18000},
        {"EST", -18000}, {"IET", -18000},
        {"EDT", -14400},
        {"PRT", -14400},
        {"CNT", -12600},
        {"AGT", -10800}, {"BET", -10800},
        {"CAT", -3600},
        {NULL, 0}
    };
    for (int i = 0; table[i].name != NULL; i++)
    {
        if (strcmp(tz, table[i].name) == 0)
            return table[i].offset;
    }
    return 0;
}

void setClock() {
    Log.notice(F("Entering blocking loop to get NTP time."));
    const char* TMZ = config.ispindhub.TZ;
    if (strlen(TMZ) == 0) TMZ = "Europe/London";
    Log.notice(F("Time Zone: %s" CR), TMZ);

    const char* posix = ianaToPosx(TMZ);
    if (posix)
    {
        // IANA timezone name — use POSIX string for correct automatic DST
        configTime(posix, "pool.ntp.org", "time.nist.gov");
    }
    else
    {
        // Legacy abbreviation (old config) — fall back to offset-based
        configTime(tzAbbrevToOffsetSeconds(TMZ),
                   (long)config.ispindhub.dst_offset * 3600,
                   "pool.ntp.org", "time.nist.gov");
    }

    time_t nowSecs = time(nullptr);
    time_t startSecs = time(nullptr);
    int cycle = 0;
    while (nowSecs < EPOCH_1_1_2019) {
        if (nowSecs - startSecs > 9) {
            if (cycle > 9) {
                Log.warning(F("Unable to get time hack, rebooting." CR));
                ESP.restart();
            }
#ifdef LOG_LEVEL
            Serial.println();
#endif
            Log.verbose(F("Re-requesting time hack."));
            if (posix)
                configTime(posix, "pool.ntp.org", "time.nist.gov");
            else
                configTime(tzAbbrevToOffsetSeconds(TMZ),
                           (long)config.ispindhub.dst_offset * 3600,
                           "pool.ntp.org", "time.nist.gov");
            startSecs = time(nullptr);
            cycle++;
        }
#ifdef LOG_LEVEL
        Serial.print(F("."));
#endif
        delay(1000);
        yield();
        nowSecs = time(nullptr);
    }
#ifdef LOG_LEVEL
    Serial.println();
#endif
    Log.notice(F("NTP time set." CR));
    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);
}

String getDTS() {
    // Returns JSON-type string = 2019-12-20T13:59:39Z
    /// Also:
    // sprintf(dts, "%04u-%02u-%02uT%02u:%02u:%02uZ", getYear(), getMonth(), getDate(), getHour(), getMinute(), getSecond());
    time_t now;
    time_t rawtime = time(&now);
    struct tm ts;
    ts = *localtime(&rawtime);
    char dta[21] = {'\0'};
    strftime(dta, sizeof(dta), "%FT%TZ", &ts);
    String dateTimeString = String(dta);
    return dateTimeString;
}

int getYear() {
    // tm_year = years since 1900
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int year = 1900 + ts->tm_year;
    return year;
}

int getMonth() {
    // tm_mon = months since January (0-11)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int month = ts->tm_mon;
    return month;
}

int getDate() {
    // tm_mday = day of the month (1-31)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int day = ts->tm_mday;
    return day;
}

int getWday() {
    // tm_wday = days since Sunday (0-6)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int wday = 1 + ts->tm_wday;
    return wday;
}

int getHour() {
    // tm_hour = hours since midnight (0-23)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int hour = ts->tm_hour;
    return hour;
}

int getMinute() {
    // tm_min = minutes after the hour (0-59)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int min = ts->tm_min;
    return min;
}

int getSecond() {
    // tm_sec = seconds after the minute (0-60)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int sec = ts->tm_sec;
    return sec;
}

int getYDay() {
    // tm_yday = days since January 1 (0-365)
    time_t rawtime;
    struct tm * ts;
    time ( &rawtime );
    ts = gmtime ( &rawtime );
    int yday = ts->tm_yday;
    return yday;
}