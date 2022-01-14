// importing libraries

#include "FS.h"
#include <WiFi.h>
#include "SPIFFS.h"
#include "webpages_defination.h"
#include <DNSServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <driver/rtc_io.h>
#include <ESPAsyncWebServer.h>

//Logging
#define LOG true // comment this line to stop logging

#ifdef LOG
  #include "TimeLib.h"
  void logData();
  String log_line = "";
#endif

// Globals
DynamicJsonDocument doc(33768);
DynamicJsonDocument coins(100);
DynamicJsonDocument creds(200);
DynamicJsonDocument triggers(100);
DynamicJsonDocument status_doc(100);
File file;

#define DEBUG true // comment this line to stop debugging

// landing page status
String VibrationChecked = "false";
String landing = "false";
bool UpdateStatus = false;

// functions definations
void ServerRoutes();
void ReadModeChange();
void load_status();
void loadCreds();
void loadCoins();
void loadTirggers();

String COINS_LIST = "[{\"id\":\"BTC\",\"name\":\"Bitcion\"},{\"id\":\"ETH\",\"name\":\"Ethereum\"},{\"id\":\"BIT\",\"name\":\"BitDAO\"},{\"id\":\"ADA\",\"name\":\"Cardano\"},{\"id\":\"BNB\",\"name\":\"Binance Coin\"},{\"id\":\"DOT\",\"name\":\"Polkadot\"},{\"id\":\"DOGE\",\"name\":\"Dogecoin\"},{\"id\":\"LTC\",\"name\":\"Litecoin\"},{\"id\":\"SOL\",\"name\":\"Solana\"},{\"id\":\"XRP\",\"name\":\"XRP\"}]";


//Battery
#define vsense 35
#define led GPIO_NUM_15
uint8_t voltageThreshold = 4; //below this voltage battery LED will light up

//Sleep
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  60  //sleep time in seconds
#define M1_BITMASK 0x5000
#define M3_BITMASK 0x8001000

//Vibrator Motors
const uint8_t vibrator_pins[4] = {4,16,17,5};
bool doVibrate = false;

//Buzzer
#define buzzer 13

//Mode
uint8_t Mode_1_SWH = 27;
uint8_t Mode_2_SWH = 14;
uint8_t Mode_3_SWH = 12;
RTC_DATA_ATTR uint8_t Mode = 0;

/*
Mode 1 Sleep
Mode 2 Monitoring
Mode 3 Configuration
*/

unsigned long CurrentTime = 0;

const uint8_t MAX = 3;
String Coins[MAX];
uint8_t Triggers[MAX];

bool doSaveCoins = false;
bool doSaveTrigs = false;

RTC_DATA_ATTR bool lowVoltage = false;

RTC_DATA_ATTR uint8_t M_timer[MAX] = {0,0,0}; //store minute value of each coin for 5 minute timer
RTC_DATA_ATTR uint8_t H_timer[MAX] = {0,0,0}; //store minute value of each coin for 1 hour timer

RTC_DATA_ATTR float minute_last_prices[5][MAX]; //save 5-minute prices, doesnot affect by high or low price
RTC_DATA_ATTR float mh[5][MAX]; //5 minute high prices
RTC_DATA_ATTR float ml[5][MAX]; //5 minute low prices

RTC_DATA_ATTR float hour_last_prices[60][MAX]; //save 1 hour prices, doesnot affect by high or low price
RTC_DATA_ATTR float HH[60][MAX]; //1 hour high prices
RTC_DATA_ATTR float HL[60][MAX]; //1 hour high prices

RTC_DATA_ATTR uint8_t count1 = 0;
RTC_DATA_ATTR uint8_t count2 = 0;


// Wi-Fi
String wifi_ssid = "";
String wifi_password =  "";
bool WiFiconnected = false;
uint8_t WIFI_RETRY_AFTER = 10; //10sec
unsigned long LastRun = 0;
bool doConnect = false;
uint8_t ReasonID = 0;
String Reason = "";
bool manual = false;
bool isWiFi = false;

// Access Point
const char *ap_ssid = "Moon Pillow";
const char *ap_password = "12345678";

IPAddress apIP(8,8,8,8);
DNSServer dnsServer;
AsyncWebServer server(80);


class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler(){
    ServerRoutes();
  }
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request){
    if(request->url() == "/connecttest.txt" || request->url() == "/redirect" || request->url() == "/generate_204" || request->url() == "/fwlink")
      request->redirect("http://8.8.8.8/");
    else
      request->send(200, "text/html", HTML);
  }
};


void SetupWiFi(){
  #ifdef DEBUG
    Serial.println("[WiFi STA] Initiallizing");
  #endif
  if(WiFiconnected)
    WiFi.disconnect(true);
  if(wifi_ssid != ""){
    isWiFi = true;
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    #ifdef DEBUG
      Serial.println("[WiFi STA] Connecting");
    #endif
    while (WiFi.status() != WL_CONNECTED){
      ReadModeChange();
      if(Mode == 1 || !isWiFi){
        WiFi.disconnect(true);
        break;
      }
    }
  }
}

void WiFiEvent(WiFiEvent_t event){
  switch (event){
    case SYSTEM_EVENT_STA_CONNECTED:
      #ifdef DEBUG
        Serial.println("[WiFi STA] Connected");
      #endif
      break;
     
    case SYSTEM_EVENT_STA_GOT_IP:
      WiFiconnected = true;
      Reason = "";
      ReasonID = 0;
      #ifdef DEBUG
        Serial.print("[WiFi STA] IP Address: ");
        Serial.println(WiFi.localIP());
      #endif
      break;

     case SYSTEM_EVENT_AP_STACONNECTED:
      #ifdef DEBUG
        Serial.println("[WiFi AP] Client connected");
      #endif
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      #ifdef DEBUG
        Serial.println("[WiFi AP] Client disconnected");
      #endif
      break;
   }
}


void setup(){
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("[Main] Setup started");
    Serial.println("[SPIFFS] Mounting");
  #endif

  if(!SPIFFS.begin()){
    #ifdef DEBUG
      Serial.println("[SPIFFS] Mounting failed");
    #endif
    return;
  }
  else{
    #ifdef DEBUG
      Serial.println("[SPIFFS] Mounting complete");
    #endif
  }
    
  pinMode(Mode_1_SWH, INPUT);
  pinMode(Mode_2_SWH, INPUT);
  pinMode(Mode_3_SWH, INPUT);

  for(uint8_t i=0; i<4; i++)
    pinMode(vibrator_pins[i], OUTPUT);
  
  rtc_gpio_init(led);
  rtc_gpio_set_direction(led, RTC_GPIO_MODE_OUTPUT_ONLY);
  
  #ifdef DEBUG
    Serial.println("[Wi-Fi] Initializing");
  #endif
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(50);
  WiFi.mode(WIFI_MODE_APSTA);
  delay(50);
  WiFi.onEvent(WiFiEvent);
  WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
      uint8_t res = info.disconnected.reason;
      WiFiconnected = false;
      if(res == 15 || res == 202){
        isWiFi = false;
        Reason = "Authentication Failed - Invalid Password";
        ReasonID = 202;
      }
      else if(res == 201){
        isWiFi = false;
        ReasonID = 201;
        Reason = "WiFi is not in range";
      }
      else if(res == 3){
        isWiFi = false;
        ReasonID = 3;
        Reason = "WiFi is not answering";
      }
      else
        Reason = "Unknown Reason";
  }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  ledcSetup(0, 1000, 8);
  ledcAttachPin(buzzer, 0);
  ledcWrite(0, 0);

  ReadModeChange();

  if(Mode == 3)
    WiFi.scanNetworks();
  if(Mode != 1){
    load_status();
    loadCreds();
    loadCoins();
    loadTirggers();
  }
  #ifdef DEBUG
    Serial.println("[Main] Setup complete");
  #endif
}



void checkVoltage(){
  int raw = analogRead(vsense);
  float sensedVoltage = (raw * 3.3) / 4096;
  float actualVoltage = sensedVoltage / (10000/(10000+10000));
  if(actualVoltage <= voltageThreshold && !lowVoltage){
    rtc_gpio_hold_dis(led);
    rtc_gpio_set_level(led, HIGH);
    rtc_gpio_hold_en(led);
    lowVoltage = true;
  }
  else if(actualVoltage > voltageThreshold && lowVoltage){
    rtc_gpio_hold_dis(led);
    rtc_gpio_set_level(led, LOW);
    rtc_gpio_hold_en(led);
    lowVoltage = false;
  }
}


void StartServer(){
  WiFi.softAPdisconnect();
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_ssid,ap_password);
  vTaskDelay(500);
  #ifdef DEBUG
    Serial.print("[AP] IP: ");
    Serial.println(WiFi.softAPIP());
  #endif
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
  ServerRoutes();
  server.begin();
  #ifdef DEBUG
    Serial.println("[AP] Server started");
  #endif
}

void StopServer(){
  WiFi.softAPdisconnect(true);
  server.end();
  dnsServer.stop();
  #ifdef DEBUG
    Serial.println("[AP] Server stopped");
  #endif
}

void Buzz(uint8_t times){
  for(uint8_t i=0; i<times; i++){
    ledcWrite(0, 125);  // start beep
    vTaskDelay(400);
    ledcWrite(0, 0);  //stop beep
    if(times == 2 && i==0)
      vTaskDelay(400);
  }
}

void Vibrate(){
  // Start vibration
  for(uint8_t i=0; i<4; i++)
    digitalWrite(vibrator_pins[i], HIGH);
  vTaskDelay(500);

  //Stop vibration
  for(uint8_t i=0; i<4; i++)
    digitalWrite(vibrator_pins[i], LOW);
}


bool CheckTrigger(float price, int index, int blocktype){
  float hpercent;
  float lpercent;
  uint8_t percent = 0;
  if(blocktype == 1){
    hpercent = ((price - mh[M_timer[index] - 1][index])/mh[M_timer[index] - 1][index]) * 100;
    lpercent = ((price - ml[M_timer[index] - 1][index])/ml[M_timer[index] - 1][index]) * 100;
    percent = 3;
  }
  else{
    hpercent = ((price - HH[H_timer[index] - 1][index])/HH[H_timer[index] - 1][index]) * 100;
    lpercent = ((price - HL[H_timer[index] - 1][index])/HL[H_timer[index] - 1][index]) * 100;
    percent = 5;
  }
  if(hpercent <= (percent*-1)){
    #ifdef LOG
      log_line += " Triggered at more than " + String(percent) + "% by " + Coins[index];
    #endif
    #ifdef DEBUG
      Serial.println("Triggered at more than " + String(percent) + "% by " + Coins[index]);
    #endif
    return true;
  }
  else if(lpercent >= percent){
    #ifdef LOG
      log_line += " Triggered at less than " + String(percent) + "% by " + Coins[index];
    #endif
    #ifdef DEBUG
      Serial.println("Triggered at less than " + String(percent) + "% by " + Coins[index]);
    #endif
    return true;
  }
  return false;
}

void swap(int blocktype, int index){
  if(blocktype == 1){
    for(int i=0; i<4; i++){
      mh[i][index] =  mh[i+1][index];
      ml[i][index] =  ml[i+1][index];
      minute_last_prices[i][index] =  minute_last_prices[i+1][index];
    }
  }
  else{
    for(int i=0; i<59; i++){
      HH[i][index] =  HH[i+1][index];
      HL[i][index] =  HL[i+1][index];
      hour_last_prices[i][index] =  hour_last_prices[i+1][index];
    }
  }
}

void check_price_block(uint8_t blocktype, uint8_t index){
  bool high_value_in_block = false;
  bool low_value_in_block = false;
  float Low,High;
  float BL, BH;
  if(blocktype == 1){ // for 5-minute block
    Low = ml[4][index];
    High = mh[4][index];
    BH = BL = minute_last_prices[0][index]; //assuming 0-minute price is High and Low
    for(uint8_t i=0; i<5; i++){
      if(minute_last_prices[i][index] == High)  //check highest is in block or not
        high_value_in_block = true;
        
      if(minute_last_prices[i][index] == Low) //check lowest is in block or not
        low_value_in_block = true;
        
      if(i != 0){ // finding highest and lowest in block
        if(minute_last_prices[i][index] > BH)
          BH = minute_last_prices[i][index];
        if(minute_last_prices[i][index] < BL)
          BL = minute_last_prices[i][index];
      }
    }
    if(!high_value_in_block)//replace current high price if current high price is not in block
       mh[4][index] = BH;
 
    if(!low_value_in_block) //replace current low price if current low price is not in block
      ml[4][index] = BL;
  }
  else{ //for 1-hour block
    Low = HL[59][index];
    High = HH[59][index];
    BH = BL = hour_last_prices[0][index]; //assuming 0-minute price is High and Low
    for(uint8_t i=0; i<60; i++){ //check highest and lowest is in block or not
      if(hour_last_prices[i][index] == High)
        high_value_in_block = true;
        
      if(hour_last_prices[i][index] == Low)
        low_value_in_block = true;
        
      if(i != 0){ // finding highest and lowest in block
        if(hour_last_prices[i][index] > BH)
          BH = hour_last_prices[i][index];
        if(hour_last_prices[i][index] < BL)
          BL = hour_last_prices[i][index];
      }
    }
    if(!high_value_in_block) //replace current high price if current high price is not in block
      HH[59][index] = BH;
      
    if(!low_value_in_block) //replace current low price if current low price is not in block
      HL[59][index] = BL;
  }
}

String LoadFile(const char *filename){
  file = SPIFFS.open(filename, FILE_READ);
  if(file){
    if(file.size()>10){
      String ReadData = "";
      while (file.available())
        ReadData += file.readString();
      file.close();
      return ReadData;
    }
    else
      return "";
  }
  else
    return "";
}

JsonObject StringtoJson(DynamicJsonDocument document, String file_data){
  deserializeJson(document, file_data);
  JsonObject obj = document.as<JsonObject>();
  return obj;
}

void SaveJsonData(DynamicJsonDocument filedata, const char *filename){
  file = SPIFFS.open(filename, FILE_WRITE);
  serializeJson(filedata, file);
  file.close();
}

void loadCoins(){
  coins.clear(); //free extra used memory
  String file_data = LoadFile("/coins.json");
  if(file_data != ""){
    JsonObject obj = StringtoJson(coins, file_data);
    for(int i=0; i<MAX; i++)
      Coins[i] = obj[String(String(i))].as<String>();
  }
  else{
    for(int i=0; i<MAX; i++)
      coins[String(i)] = "";
    SaveJsonData(coins, "/coins.json");
  }
}

void loadTirggers(){
  triggers.clear(); //free extra used memory
  String file_data = LoadFile("/triggers.json");
  if(file_data != ""){
    JsonObject obj = StringtoJson(triggers, file_data);
    for(int i=0; i<MAX; i++)
      Triggers[i] = obj["T"+String(i)].as<uint8_t>();
  }
  else{
    for(int i=0; i<MAX; i++)
      triggers["T"+String(i)] = "";
    SaveJsonData(triggers, "/triggers.json");
  }
}

void loadCreds(){
  creds.clear(); //free extra used memory
  String file_data = LoadFile("/creds.json");
  if(file_data != ""){
    JsonObject obj = StringtoJson(creds, file_data);
    wifi_ssid = obj["ssid"].as<String>();
    wifi_password = obj["pass"].as<String>();
  }
  else{
    creds["ssid"] = "";
    creds["pass"] = "";
    SaveJsonData(creds, "/creds.json");
  }
}

void load_status(){
  status_doc.clear(); //free extra used memory
  String file_data = LoadFile("/status.json");
  if(file_data != ""){
    JsonObject obj = StringtoJson(status_doc, file_data);
    VibrationChecked = obj["vib"].as<String>(); 
    landing = obj["land"].as<String>();
  }
  else{
    status_doc["vib"] = VibrationChecked = "false";
    status_doc["land"] = landing = "false";
    SaveJsonData(status_doc, "/status.json");
  }
}

void savecoins(){
  coins.clear(); //free extra used memory
  for(int i=0; i<MAX; i++)
    coins[String(i)] = String(Coins[i]);
  SaveJsonData(coins, "/coins.json");
  doSaveCoins = false;
}

void savetriggers(){
  triggers.clear(); //free extra used memory
  for(int i=0; i<MAX; i++)
    triggers["T"+String(i)] = String(Triggers[i]);
  SaveJsonData(triggers, "/triggers.json"); //{"T0":"1","T1":"3","T3":"",}
  doSaveTrigs = false;
}

void saveCreds(){
  creds.clear(); //free extra used memory
  creds["ssid"] = wifi_ssid;
  creds["pass"] = wifi_password;
  SaveJsonData(creds, "/creds.json"); //{"ssid":"wifi name","pass":"wifi password"}
  doConnect = false;
}

void savestatus(){
  status_doc.clear(); //free extra used memory
  status_doc["vib"] = VibrationChecked;
  status_doc["land"] = landing;
  SaveJsonData(status_doc, "/status.json");
  UpdateStatus = false;
}

void Block5min(float price, int index){
  uint8_t shiftIndex = 1;
  if(M_timer[index] != 0){
    bool trig = CheckTrigger(price, index, 1);
    if(trig){
      ml[0][index] = price;
      mh[0][index] = price;
      M_timer[index] = 0;
      minute_last_prices[0][index] = price;
      Vibrate();
    }
    else{
      minute_last_prices[M_timer[index]][index] = price;
      if(M_timer[index] == 5){
        swap(1, index);
        shiftIndex = 2;
        minute_last_prices[M_timer[index]-1][index] = price;
      }
      else
        minute_last_prices[M_timer[index]][index] = price;
        
      if(price > mh[M_timer[index]-shiftIndex][index])
        mh[M_timer[index] - (shiftIndex-1)][index] = price;
      else
        mh[M_timer[index] - (shiftIndex-1)][index] = mh[M_timer[index] - shiftIndex][index];
  
      if(price < ml[M_timer[index]-shiftIndex][index])
        ml[M_timer[index] - (shiftIndex-1)][index] = price;
      else
        ml[M_timer[index] - (shiftIndex-1)][index] = ml[M_timer[index] - shiftIndex][index];

      if(shiftIndex == 2)
        check_price_block(1,index);
    }
  }
  else{
    ml[0][index] = price;
    mh[0][index] = price;
    minute_last_prices[0][index] = price;
  }
}

void Block1hr(float price, int index){
  uint8_t shiftIndex = 1;
  if(H_timer[index] != 0){
    bool trig = CheckTrigger(price, index, 2);
    if(trig){
      HH[0][index] = price;
      HL[0][index] = price;
      H_timer[index] = 0;
      hour_last_prices[0][index] = price;
      Vibrate();
    }
    else{
      hour_last_prices[M_timer[index]][index] = price;
      if(H_timer[index] == 60){
        swap(2, index);
        shiftIndex = 2;
        hour_last_prices[M_timer[index]-1][index] = price;
      }
      if(price > HH[H_timer[index]-shiftIndex][index])
        HH[H_timer[index] - (shiftIndex-1)][index] = price;
      else
        HH[H_timer[index] - (shiftIndex-1)][index] = HH[H_timer[index] - shiftIndex][index];
          
      if(price < HL[H_timer[index]-shiftIndex][index])
        HL[H_timer[index] - (shiftIndex-1)][index] = price;
      else
        HL[H_timer[index] - (shiftIndex-1)][index] = HL[H_timer[index] - shiftIndex][index];

      if(shiftIndex == 2)
        check_price_block(2,index);
    }
  }
  else{
    HH[0][index] = price;
    HL[0][index] = price;
    hour_last_prices[0][index] = price;
  }
}

void ReadModeChange(){
  if(digitalRead(Mode_1_SWH) && Mode != 1){ //Not Working 27
    Mode = 1;
    StopServer();
    #ifdef DEBUG
      Serial.println("[Mode] 1 (Sleep Mode) Selected");
    #endif
    esp_sleep_enable_ext1_wakeup(M1_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);  //wake esp if mode changes from not working to configuration/monitoring
    esp_deep_sleep_start();
  }
  else if(digitalRead(Mode_2_SWH) && Mode != 2){ //Monitoring 12
    Mode = 2;
    StopServer();
    #ifdef DEBUG
      Serial.println("[Mode] 2 (Monitoring Mode) Selected");
    #endif
    count1 = count2 = 0;
    for(int i=0; i<MAX; i++){
      M_timer[i] = 0;
      H_timer[i] = 0;
    }
    if(!WiFiconnected){
      loadCreds();
      SetupWiFi();
    }
    loadCoins();
    loadTirggers();
    Buzz(1); //beep 1 time
  }
  else if(digitalRead(Mode_3_SWH) && Mode != 3){ //Configuration 14
    Mode = 3;
    StartServer();
    #ifdef DEBUG
      Serial.println("[Mode] 3 (Configuration Mode) Selected");
    #endif
    load_status();
    loadCreds();
    loadCoins();
    loadTirggers();
    Buzz(2); //beep 2 time
  }
}

void ReadCoins(){
  HTTPClient http;
  vTaskDelay(1000);
  if(count1 != count2){
    count1 = count2 = 0;
    for(int i=0; i<MAX; i++){
      M_timer[i] = 0;
      H_timer[i] = 0;
    }
  }
  count1 = count1 + 1;
  for(int y=0; y<MAX; y++){
    if(Coins[y] != ""){
      http.begin("https://api.bybit.com/v2/public/tickers?symbol="+Coins[y]+"USDT");
      int httpResponseCode = http.GET();
       if(httpResponseCode == 200){
          count2 = count2 + 1;
        String api_payload = http.getString();
        
        deserializeJson(doc, api_payload);
        JsonObject obj = doc.as<JsonObject>();
        
        float last_price = obj["result"][0]["last_price"];

        #ifdef LOG
          int t = obj["time_now"];
          log_line = String(day(t)) + "/" + String(month(t)) + "/" + String(year(t)) + " " + String(hour(t)) + ":" + String(minute(t)) + ":" + String(second(t)) + " -> ";
          log_line += "Coin: " + Coins[y] + ", Price: " + String(last_price);
        #endif
        #ifdef DEBUG
          /*Serial.print("API data: ");
          Serial.println(api_payload);*/
          Serial.println("Coin: " + Coins[y]);
          Serial.println("Price: " + String(last_price));
          Serial.println("__________________________");
        #endif

        if(Triggers[y] == 2 || Triggers[y] == 3)
          Block1hr(last_price,y);
        
        if(Triggers[y] == 1 || Triggers[y] == 3)
          Block5min(last_price,y);

        if(M_timer[y]<5)
          M_timer[y] = M_timer[y] + 1;
        if(H_timer[y]<60)
          H_timer[y] = H_timer[y] + 1;
      }
      http.end();
      #ifdef LOG
        logData();
      #endif
    }
  }
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_enable_ext1_wakeup(M3_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH); //wake esp if mode changes from monitoring to configuration/not working
  esp_deep_sleep_start();
}

void loop(){
  CurrentTime = millis()/1000;
  checkVoltage();
  ReadModeChange();
  if(doVibrate){
    Vibrate();
    doVibrate = false;
  }
  if(Mode == 3){
    dnsServer.processNextRequest();
    if(UpdateStatus)
      savestatus();
    if(doSaveCoins)
      savecoins();
    if(doSaveTrigs)
      savetriggers();
    if(doConnect){
      saveCreds();
      SetupWiFi();
    }
  }
  if(Mode != 1 && !WiFiconnected && (ReasonID != 202) && !manual && wifi_ssid != ""){
    if(CurrentTime - LastRun > WIFI_RETRY_AFTER){
      SetupWiFi();
      LastRun = millis()/1000;
    }
  }
  if(WiFiconnected && Mode == 2){
    ReadCoins();
  }
}

#ifdef LOG
  void logData(){
    File log_file;
    if(SPIFFS.exists("/log.txt"))
      log_file = SPIFFS.open("/log.txt", FILE_APPEND);
  
    else
      log_file = SPIFFS.open("/log.txt", FILE_WRITE);
  
    log_file.println(log_line);
    #ifdef DEBUG
      Serial.println("Data logged");
    #endif
    log_file.close();
  }
#endif
