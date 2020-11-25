#include "ispindel.h"
extern Adafruit_ST7735 tft;
//extern int delay_loop;

int handle_spindel_data(String iSpinData,int delay_loop){
    int str_len = iSpinData.length() +1;
    char charData[str_len];
    int count = 0;
    int idx;
    int mov_idx = 0;
    Serial.println("delay loop en entrant");
    Serial.println(delay_loop);
    iSpinData.toCharArray(charData,str_len);
    //Split by the \r char
    char* splitData = strtok(charData,"\r");
    // get length of a line
    int line_len = strlen(splitData);
    // Get to the LAST record
    String lastData = iSpinData.substring(str_len-line_len,str_len);
    Serial.println(lastData);
    // Do we have an interval field ?
    // Let's count commas
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
  
    if (count > 5) // We have more than 5 entries so we have a "delay"
    {
        delay_loop = array_data[6].toInt()*1000;
        
    }
    //int i;
    //for (i=0; i <= count;i++){
    //    Serial.println(array_data[i]);
    //    //tft.print(array_data[i]);
    //   //tft.setCursor(2,10*(i+2));
    //}
    Serial.println("delay loop en sortant");
    Serial.println(delay_loop);
    displaydata(array_data);
    return(delay_loop);
}