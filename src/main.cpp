#include "main.h"
// extern Adafruit_ST7735 tft;
extern TFT_eSPI tft;

// DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 22);
IPAddress subnet(255, 255, 255, 0);

int delay_loop = 30000;
// String version = "0.0.6";
String vers = String(version());

// Let's put Global variable because why not
String SG; 
String Voltage;
String Temp;
String Angle;
String deviceName;
String RSSI;
String IP;
String LastSeen;

// Recurring jobs setup
TickTwo BFa_timer(pushBrewFather, 900);

void setup()
{
  // tft.initR(INITR_144GREENTAB); // initialise ST7735S chip, green tab
  tft.init();
  tft.setTextWrap(false); // Allow text to run off right edge
  // tft.setRotation(1);
  //  Display Boot
  // tft.fillScreen(ST7735_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  // tft.drawRect(0,0,128,128,ST7735_WHITE);
  tft.drawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
  tft.setTextColor(ST7735_AQUA);
  centerString("iSpind Hub " + vers, (TFT_WIDTH / 2), 10);
  // tft.setTextColor(ST7735_YELLOW);
  tft.setTextColor(TFT_YELLOW);
  centerString("by Slammy", (TFT_WIDTH / 2), 25);
  // File System
  LittleFS.begin();
  // LittleFS.format();
  Serial.begin(115200);
  wdt_enable(WDTO_8S);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  if (loadConfig())
    {Log.notice(F("Configuration loaded." CR));
    printConfig();
    }
  else
    {Log.error(F("Unable to load configuration." CR));}
  // WiFi.mode(WIFI_STA);
  // bool rst = drd.detect(); // Check for double-reset
  bool rst = false;
  bool isconnected;
  bool isdeployed;
  if (rst == true)
  {
    // Serial.println("DRD Triggered, launch Config Portal");
    doWiFi(true);
  }
  else
  {
    doWiFi();
  }
  // AsyncWiFiManager wm;

  // isconnected = wm.autoConnect("iSpindHubConfig");
  isconnected = WiFi.isConnected();
  if (!isconnected)
  {
    tft.setCursor(2, 40);
    // tft.setTextColor(ST7735_RED);
    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.print("Failed to connect :(");
    // Serial.println("Failed to connect.");
  }
  else
  {
    tft.setCursor(2, 40);
    // tft.setTextColor(ST7735_GREEN);
    tft.setTextColor(TFT_GREEN);
    tft.setTextSize(1);
    tft.print("Connected !");
    // Serial.println("Connected");
    // Serial.println("Setting soft-AP configuration ... ");
    // Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    setClock(); // Set NTP Time
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    //isdeployed = WiFi.softAP("iSpindHub");
    if (strlen(config.ispindhub.name) == 0)
    {
        isdeployed = WiFi.softAP(APNAME);
    }
    else{
      isdeployed = WiFi.softAP(config.ispindhub.name);
    }
    
    if (isdeployed)
    {
      // tft.setTextColor(ST7735_BLUE);
      tft.setTextColor(TFT_BLUE);
      tft.setTextSize(1);
      tft.setCursor(2, 50);
      tft.print("SubNetWork deployed ! ");
      tft.setCursor(2, 80);
      tft.print("Soft-AP IP address = ");
      tft.setCursor(2, 110);
      tft.print(WiFi.localIP().toString());
      // Serial.print("Soft-AP IP address = ");
      // Serial.println(WiFi.softAPIP());
      initWebServer();
      // Serial.print("Web Server Launched");
      BFa_timer.start();
    }
    else
    {
      // tft.setTextColor(ST7735_RED);
      tft.setTextColor(TFT_RED);
      tft.setTextSize(1);
      tft.setCursor(2, 50);
      tft.print("Failed to Deploy SubNetwork ");
    }
  }
}


void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("Début du Loop");
  BFa_timer.update();
  FSInfo fs_info;
  LittleFS.info(fs_info);
  Dir dir = LittleFS.openDir("/data");
  Serial.println("On a ouvert le Dir");
  // or Dir dir = LittleFS.openDir("/data");
  // while(true){
  while (dir.next())
  {
    // Open iSpindel Data file
    // Serial.println("File Name : " + dir.fileName());
    if (dir.fileSize())
    {
      wdt_reset();
      File f = dir.openFile("r");
      /*Serial.println("File Name :");
      Serial.println(f.name());
      Serial.println("File Size :");
      Serial.println(f.size());
      */
      time_t now = time(nullptr);
      String temp = f.readStringUntil('\r');
      int line_len = temp.length() + 1;
      f.seek(line_len, SeekEnd);
      String iSpinData = f.readString();
      // Serial.println(iSpinData);
      delay_loop = handle_spindel_data(iSpinData, delay_loop, now - f.getLastWrite());
      f.close();
      //printConfig();
      BFa_timer.interval(config.brewfather.freq * 60 * 1000);
      // free(iSpinData);
      //Serial.println("Avant delay");
      //Serial.println(BFa_timer.elapsed());
      //Serial.println(BFa_timer.state());
      delay(delay_loop);
      //Serial.println("Après delay");
    }
  }
  dir.rewind();
  //}
  // return;
}
