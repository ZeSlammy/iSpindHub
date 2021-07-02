#include "pushhelper.h"

IPAddress resolveHost(const char *hostname)
{
    Log.verbose(F("Host lookup: %s." CR), hostname);
    IPAddress returnIP = INADDR_NONE;
    if (WiFi.hostByName(hostname, returnIP, 10000) == 0)
    {
        Log.error(F("Host lookup error." CR));
        returnIP = INADDR_NONE;
    }
    return returnIP;
}

bool pushToTarget(PushTarget *target, IPAddress targetIP, int port)
{
    LCBUrl lcburl;
    lcburl.setUrl(String(target->url) + String(target->key.name));

    Log.notice(F("Posting to: %s" CR), lcburl.getHost().c_str());

    const size_t capacity = JSON_OBJECT_SIZE(8) + 210;
    StaticJsonDocument<capacity> doc;

    String json;
    serializeJson(doc, json);

    // Use the IP address we resolved (necessary for mDNS)
    Log.verbose(F("Connecting to: %s at %s on port %l" CR),
                lcburl.getHost().c_str(),
                targetIP.toString().c_str(),
                port);

    WiFiClient client;
    //  1 = SUCCESS
    //  0 = FAILED
    // -1 = TIMED_OUT
    // -2 = INVALID_SERVER
    // -3 = TRUNCATED
    // -4 = INVALID_RESPONSE
    client.setNoDelay(true);
    client.setTimeout(10000);
    if (client.connect(targetIP, port))
    {
        Log.notice(F("Connected to: %s." CR), target->target.name);

        // Open POST connection
        if (lcburl.getAfterPath().length() > 0)
        {
            Log.verbose(F("POST /%s%s HTTP/1.1" CR),
                        lcburl.getPath().c_str(),
                        lcburl.getAfterPath().c_str());
        }
        else
        {
            Log.verbose(F("POST /%s HTTP/1.1" CR), lcburl.getPath().c_str());
        }
        client.print(F("POST /"));
        client.print(lcburl.getPath().c_str());
        if (lcburl.getAfterPath().length() > 0)
        {
            client.print(lcburl.getAfterPath().c_str());
        }
        client.println(F(" HTTP/1.1"));

        // Begin headers
        //
        // Host
        Log.verbose(F("Host: %s" CR), lcburl.getHost().c_str());
        client.print(F("Host: "));
        client.println(lcburl.getHost().c_str());
        //
        Log.verbose(F("Connection: close" CR));
        client.println(F("Connection: close"));
        // Content
        Log.verbose(F("Content-Length: %l" CR), json.length());
        client.print(F("Content-Length: "));
        client.println(json.length());
        // Content Type
        Log.verbose(F("Content-Type: application/json" CR));
        client.println(F("Content-Type: application/json"));
        // Terminate headers with a blank line
        Log.verbose(F("End headers." CR));
        client.println();
        //
        // End Headers

        // Post JSON
        client.println(json);
        // Check the HTTP status (should be "HTTP/1.1 200 OK")
        char status[32] = {0};
        client.readBytesUntil('\r', status, sizeof(status));
        client.stop();
        Log.verbose(F("Status: %s" CR), status);
        if (strcmp(status + 9, "200 OK") == 0)
        {
            if (target->checkBody.enabled == true)
            {
                // Check body
                String response = String(status);
                if (response.indexOf(target->checkBody.name) >= 0)
                {
                    Log.verbose(F("Response body ok." CR));
                    return true;
                }
                else
                {
                    Log.error(F("Unexpected body content: %s" CR), response.c_str());
                    return false;
                }
            }
            else
            {
                return true;
            }
        }
        else
        {
            Log.error(F("Unexpected status: %s" CR), status);
            return false;
        }
    }
    else
    {
        Log.warning(F("Connection failed, Host: %s, Port: %l (Err: %d)" CR),
                    lcburl.getHost().c_str(), port, client.connected());
        return false;
    }
}
