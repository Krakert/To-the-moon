#include <main.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  initEeprom();
  display.init();

  // String ssid = "stefan";  
  // String psw = "dekraker";
  // String ssid = "Huisdekraker";
  // String psw = "FAGHPTB438FQ";
  String ssid = "pvstsdkjdk";
  String psw = "KKCA6TBV8TA7";

  saveApToEeprom(ssid, psw);
  // TODO make reset fucntion, if GPIO IS high clear eeprom
  // clearEeprom();

  /* GET DATA OF THE EEPROM AND DUMP IT VIA SERIAL */
  uint8_t** valuesOutEeprom = getConfigFormEeprom();
  dumbDataEeprom(valuesOutEeprom);
  /* IF FOUND TWO PARAMETERS, SETUP STATION ELSE AP */
  if (checkIfNotEmpty(valuesOutEeprom) == 2){
    if (Serial){
      Serial.println("Start the STA mode, data valid in the eeprom");
    }
    // ! Start the STA mode, data valid in the eeprom
    /* GET THE PARAMETERS IN SEPAERATE VARIABLES */
    char *ssidFromEeprom = extractParameter(valuesOutEeprom, 0);
    char *pswFromEeprom = extractParameter(valuesOutEeprom, 1);
    /* SETUP STATION MODE */
    WiFi.mode(WIFI_STA);
    /* USE DATA OUT THE EEPROM */
    WiFi.begin(ssidFromEeprom, pswFromEeprom);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed!");
    } else {
      Serial.print("\r\nIP Address: ");
      Serial.println(WiFi.localIP());
      STATION = true;

      server.on("/", HTTP_GET, handleOnHomeSTA);
      server.onNotFound(handleNotFound);
      server.begin(); 
    }
    
    display.boot(); 
  } 
  
  if (STATION == false) {
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

    server.on("/", HTTP_GET, handleOnHomeAP);
    server.on("/get", HTTP_GET, handleGetDataAP);
    server.onNotFound(handleNotFound);
    server.begin();  
    display.boot(); 
  }      
        
  if (STATION) {
    // ? Display something the boot screen if the station mode is enabled
  } else {
    display.showInitScreenAp();
  }   
  Serial.println(STATION);       
}

void loop() {
  if (STATION) {
    requestHandler httpHandler;
    Serial.println(infoCoin.idCoin);
    Serial.println(infoCoin.priceCoin);
    httpHandler.requestFromApi("ADA", &infoCoin, false);
  }
  delay(5000);
}


/*
1. check if coin

*/

