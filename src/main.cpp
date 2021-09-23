#include <ESP8266WiFi.h>
#include <wifi.h>

#define STANDARD_CONFIG_AP   true 

/* 
  optional IP stettings can be used to override the default settings,
  set STANDARD_CONFIG_AP to false to use. 
*/
#if(!STANDARD_CONFIG_AP)
  #define WIFI_AP_SSID  "Own SSID"
  #define WIFI_AP_PSW   "OWn PSW"
  IPAddress LOCAL_IP(192,168,1,1);
  IPAddress GATEWAY(192,168,1,1);
  IPAddress SUBNET(255,255,255,0);
  #define WIFI_AP_PORT  80
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // WiFi.begin("pvstsdkjdk","KKCA6TBV8TA7");

  // Serial.print("Connecting");
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println();

  // Serial.print("Connected, IP address: ");
  // Serial.println(WiFi.localIP());

  initEeprom();
  String ssid = "test";
  String psw = "tset";
  saveApToEeprom(ssid, psw);
  delay(1000);
  getConfigForEeprom();
  
  #if(STANDARD_CONFIG_AP)
    if(setupSoftApConfig()){
      if (setupSoftAp()){
        ESP8266WebServer server(WIFI_AP_STANDARD_PORT);
      }
    }
  #else 
    if(setupSoftApConfig(LOCAL_IP, GATEWAY, SUBNET)){
      if (setupSoftAp(WIFI_AP_SSID, WIFI_AP_PSW)){
        ESP8266WebServer server(WIFI_AP_PORT);
      }
    }
  #endif
}

void loop() {
  
}