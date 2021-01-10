#include "main.h"
extern Adafruit_ST7735 tft;


DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,22);
IPAddress subnet(255,255,255,0);

int delay_loop = 10000;
String version = "1.0.0";


void setup() {
  tft.initR(INITR_144GREENTAB); // initialise ST7735S chip, green tab
  tft.setTextWrap(false); // Allow text to run off right edge
  //tft.setRotation(1);
  // Display Boot 
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0); 
  tft.drawRect(0,0,128,128,ST7735_WHITE);
  tft.setTextColor(ST7735_AQUA);
  centerString("iSpind Hub " + version,64,10);
  tft.setTextColor(ST7735_YELLOW);
  centerString("by Slammy",64,25);
  // File System
  LittleFS.begin();
  LittleFS.format();
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  WiFi.mode(WIFI_STA);
  bool rst = drd.detect(); // Check for double-reset
  bool isconnected;
  bool isdeployed;
  if (rst == true){
    Serial.println("DRD Triggered, launch Config Portal");
    doWiFi(true);
  }
  else{
    doWiFi();
  }
  AsyncWiFiManager wm;
  
  
  isconnected = wm.autoConnect("iSpindHubConfig");
  if(!isconnected){
    tft.setCursor(2,40);
    tft.setTextColor(ST7735_RED);
    tft.setTextSize(1);
    tft.print("Failed to connect :(");
    Serial.println("Failed to connect.");
  }
  else{
    tft.setCursor(2,40);
    tft.setTextColor(ST7735_GREEN);
    tft.setTextSize(1);
    tft.print("Connected !");
    Serial.println("Connected");
    Serial.println("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    setClock();             // Set NTP Time
    isdeployed = WiFi.softAP("iSpindHub");
    if(isdeployed){
      tft.setTextColor(ST7735_BLUE);
      tft.setTextSize(1);
      tft.setCursor(2,50);
      tft.print("SubNetWork deployed ! ");
      tft.setCursor(2,80);
      tft.print("Soft-AP IP address = ");
      tft.setCursor(2,110);
      tft.print(WiFi.localIP().toString());
      Serial.print("Soft-AP IP address = ");
      Serial.println(WiFi.softAPIP());
      initWebServer();
    }
    else{
      tft.setTextColor(ST7735_RED);
      tft.setTextSize(1);
      tft.setCursor(2,50);
      tft.print("Failed to Deploy SubNetwork ");
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
FSInfo fs_info;
LittleFS.info(fs_info);

Dir dir = LittleFS.openDir("/data");
// or Dir dir = LittleFS.openDir("/data");
while (dir.next()) {
    //Open iSpindel Data file
    Serial.println("File Name : " + dir.fileName());
    if(dir.fileSize()) {
        File f = dir.openFile("r");
        //Serial.println("FS Total Size : ");
        //Serial.println(fs_info.totalBytes);
        //Serial.println(fs_info.usedBytes);
        //Serial.println("File Size :");
        //Serial.println(f.size());
        // When was the last time the iSpindel Data file updated ?
        //Serial.println(f.getLastWrite());
        time_t now = time(nullptr);
        //Serial.println(now);
        //pretty_time(now-f.getLastWrite());
        // Open file as a string
        String iSpinData = f.readString();
        //Serial.println(iSpinData);
        delay_loop = handle_spindel_data(iSpinData, delay_loop,now-f.getLastWrite());
        f.close();
        Serial.println("Delay 1:");
        Serial.println(delay_loop);
    delay(delay_loop);
    }


}
Serial.println("Delay 2:");
Serial.println(delay_loop);
delay(delay_loop);
return;
}