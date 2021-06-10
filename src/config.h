#ifndef CONFIG_H
#define CONFIG_H
#ifndef APNAME
#define APNAME "iSpindHub"
#endif
#ifndef APPWD
#define APPWD ""
#endif
#ifndef APCONFNAME
#define APCONFNAME "iSpindHubConfig"
#endif
#ifndef APCONFPWD
#define APCONFPWD ""
#endif

#ifndef WIFI_CHAN
#define WIFI_CHAN 1
#endif // WIFI_CHAN
#ifndef HOSTNAME
#define HOSTNAME "iSpindHub"
#endif // HOSTNAME
#ifndef TIMESERVER
#define TIMESERVER "pool.ntp.org", "time.nist.gov"
#define THISTZ TZ_Etc_GMT
#endif // TIMESERVER

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_VERBOSE
#endif // LOG_LEVEL


#endif