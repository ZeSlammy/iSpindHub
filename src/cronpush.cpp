#include "cronpush.h"
// Recurring Jobs Functions
void pushBrewFather()
{ // Check if we have a BF Key
    if (config.brewfather.key != "")
    {

        // create payload
        StaticJsonDocument<400> payload;
        FSInfo fs_info;
        LittleFS.info(fs_info);
        Dir dir = LittleFS.openDir("/data");
        //const char *bf_id = "SYuAmrrHxyGMsx";
        String bf_id = config.brewfather.key;
        String bf_server = "http://log.brewfather.net/ispindel?id=";
        String bf_url;
        bf_url = bf_server + bf_id;
        Serial.println(bf_url);
        WiFiClient client;
        HTTPClient http;
       
        while (dir.next())
        {
            // PrOcessing One file at at time
             http.begin(client, bf_url);
            http.addHeader("Content-Type", "application/json");
            http.addHeader("Cache-Control", "no-cache");
            String json;
            String f_name = dir.fileName();
            f_name.remove(f_name.length() - 4);
            // JsonObject f_name_json = payload.createNestedObject(f_name);
            if (dir.fileSize())
            {
                File file = dir.openFile("r");
                String temp = file.readStringUntil('\r');
                int line_len = temp.length() + 1;
                //int file_size = file.size();
                //float num_line = file_size / line_len;
                file.seek(line_len, SeekEnd);
                String lastData = file.readString();
                file.close();
                // Store Last Readings
                int str_len = lastData.length() + 1;
                int count = 0;
                int idx;
                int mov_idx = 0;
                String array_data[10] = {};
                for (idx = 0; idx <= str_len; idx++)
                {
                    if (lastData[idx] == ',')
                    { // splitData[count] = lastData.substring(mov_idx,idx-1);
                        array_data[count] = lastData.substring(mov_idx, idx);
                        mov_idx = idx + 1;
                        count++;
                    }
                }
                /* array_data : 2021-05-02T12:12:22Z,iSpindel032,63.3900,18.26,4.2000,1.0560,900.0,-57.0,,
                1 : Date created
                2 : name
                3 : Angle
                4 : Temp
                5 : Battery
                6 : Gravity
                7 : Frequency
                8 : RSSI
                9 : Temp Unit
                */
                /*f_name_json["name"] = array_data[1];
                f_name_json["ID"] = array_data[1];
                f_name_json["angle"] = array_data[2];
                f_name_json["temperature"] = array_data[3];
                f_name_json["battery"] = array_data[4];
                f_name_json["gravity"] = array_data[5];
                f_name_json["interval"] = array_data[6];
                f_name_json["RSSI"] = array_data[7];
                */
                payload["name"] = array_data[1];
                payload["ID"] = array_data[1];
                payload["angle"] = array_data[2];
                payload["temperature"] = array_data[3];
                payload["battery"] = array_data[4];
                payload["gravity"] = array_data[5];
                payload["interval"] = array_data[6];
                payload["RSSI"] = array_data[7];
                char buffer[900];
                serializeJsonPretty(payload, buffer);
                Serial.println(buffer);
                serializeJson(payload, json);
                http.POST(json);
                // Read response
                Serial.print(http.getString());
                delay(5000);
                http.end();
            }
        }

        // Disconnect
        
    }
    else{
        Serial.println("No BF Key, no Push");
    }
}