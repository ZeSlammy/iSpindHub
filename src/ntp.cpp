
#include "ntp.h"

void setClock() {
    Log.notice(F("Entering blocking loop to get NTP time."));
    configTime(GMT, 0, "pool.ntp.org", "time.nist.gov");
    time_t nowSecs = time(nullptr);
    time_t startSecs = time(nullptr);
    int cycle = 0;
    while (nowSecs < EPOCH_1_1_2019) {
        if (nowSecs - startSecs > 9) {
            if (cycle > 9) {
                Log.warning(F("Unable to get time hack from %s, rebooting." CR));
                ESP.restart();
            }
#ifdef LOG_LEVEL
            Serial.println();
#endif
            Log.verbose(F("Re-requesting time hack."));
            configTime(GMT, 0, "pool.ntp.org", "time.nist.gov");
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