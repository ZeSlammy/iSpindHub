#include "filemgr.h"

JsonDocument get_files_info(){
FSInfo fs_info;
LittleFS.info(fs_info);

Dir dir = LittleFS.openDir("/data");
// or Dir dir = LittleFS.openDir("/data");
while (dir.next()) {
    //Open iSpindel Data file
    Serial.println("File Name : " + dir.fileName());
}

}