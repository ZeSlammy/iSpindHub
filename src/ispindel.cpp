#include "ispindel.h"
extern Adafruit_ST7735 tft;
//extern int delay_loop;

int handle_spindel_data(String iSpinData,int delay_loop,int last_seen_ms){
    Serial.println("delay loop en entrant");
    Serial.println(delay_loop);
    String lastData = get_last_value(iSpinData);
    Serial.println(lastData);
    int str_len = lastData.length() +1;
    int count = 0;
    int idx;
    int mov_idx = 0;
    String array_data[10] = {};
    for (idx = 0; idx <= str_len; idx++)
    {
        if (lastData[idx] == ',')
        {   //splitData[count] = lastData.substring(mov_idx,idx-1);
        array_data[count] = lastData.substring(mov_idx,idx);
        mov_idx = idx+1;
        count++;
        }
    }
    Serial.println("delay loop en sortant");
    Serial.println(delay_loop);
    displaydata(array_data,last_seen_ms);
    return(delay_loop);
}