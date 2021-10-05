#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <wire.h>

#include <wifi.h>
#include <eeprom_handler.h>

const char* PARAM_INPUT_1_AP = "ssidFromAp";
const char* PARAM_INPUT_2_AP = "passwordFromAp";

AsyncWebServer server(WIFI_AP_STANDARD_PORT);

void handleNotFound(AsyncWebServerRequest *request){
    if (Serial){
        Serial.println("function: handleNotFound");
    }
    request->send(404, "text/plain", "404: Not found");
}

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
  initEeprom();

  // TODO in setup read EEPROM for SSID and PSW, if empty setup AP
  // String ssid = "test";
  // String psw = "tset";
  // * use 'saveApToEeprom' for 'server.on("/saveApData", handleSaveApData)'
  // saveApToEeprom(ssid, psw);
  // TODO make reset fucntion, if GPIO IS high clear eeprom
  // clearEeprom();

  uint8_t** valuesOutEeprom = getConfigForEeprom();
  // * Use me to show data of the eeprom `dumbDataEeprom(valuesOutEeprom);`
  dumbDataEeprom(valuesOutEeprom);
  if (checkIfNotEmpty(valuesOutEeprom) == 2){
    if (Serial){
      Serial.println("Start the STA mode, data valid in the eeprom");
    }
    #if(STANDARD_CONFIG_AP)
      AsyncWebServer server(WIFI_AP_STANDARD_PORT);
    #else
      AsyncWebServer server(WIFI_AP_PORT);
    #endif
    // ! Start the STA mode, data valid in the eeprom
    char *ssidFromEeprom = extractParameter(valuesOutEeprom, 0);
    char *pswFromEeprom = extractParameter(valuesOutEeprom, 1);
    Serial.println(ssidFromEeprom);
    Serial.println(pswFromEeprom);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidFromEeprom, pswFromEeprom);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed!");
      return;
    }
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    if (Serial){
      Serial.println("Setup AP, no data in eeprom");
    }
    // ! Setup AP, no data in eeprom
    #if(STANDARD_CONFIG_AP)
      
      setupSoftApConfig();
      setupSoftAp();   
    #else
      AsyncWebServer server(WIFI_AP_PORT); 
      setupSoftApConfig(LOCAL_IP, GATEWAY, SUBNET);
      setupSoftAp(WIFI_AP_SSID, WIFI_AP_P SW);

    #endif

    delay(100);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if (Serial){
        Serial.println("function: serverAp.on(\"/\"");
    }
      request->send_P(200, "text/html", INDEX_HTML_AP);
    });

    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      if (Serial){
        Serial.println("function: serverAp.on(\"/get\"");
      }
      String ssidToSave, pswToSave;
      if (request->hasParam(PARAM_INPUT_1_AP)) {
        ssidToSave = request->getParam(PARAM_INPUT_1_AP)->value();
      } if (request->hasParam(PARAM_INPUT_2_AP)) {
        pswToSave = request->getParam(PARAM_INPUT_2_AP)->value();
      }
      if (saveApToEeprom(ssidToSave, pswToSave, true) == 0){
        request->send(200, "text/html", INDEX_HTML_AP_SUCCES);
      } else {
        Serial.println("Something went wrong");
      }
    });

    server.onNotFound(handleNotFound);
    server.begin();    
  }                       
}

void loop() {
}
