#include <main.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  initEeprom();
  display.init();
  LittleFS.begin();

  // String ssid = "stefan";  
  // String psw = "dekraker";
  // String ssid = "POCO F2 Pro";  
  // String psw = "KTMDuke69018@";
  // String ssid = "Huisdekraker";
  // String psw = "FAGHPTB438FQ";
  // String ssid = "pvstsdkjdk";
  // String psw = "KKCA6TBV8TA7";

  // saveApToEeprom(ssid, psw);
  // TODO make reset function, if GPIO IS high clear eeprom
  // clearEeprom();

  /* GET DATA OF THE EEPROM AND DUMP IT VIA SERIAL */
  uint8_t** valuesOutEeprom = getConfigFormEeprom();
  // dumbDataEeprom();
  /* IF FOUND TWO PARAMETERS, SETUP STATION ELSE AP */
  if (checkIfNotEmpty(valuesOutEeprom) == 2){
    if (Serial){
      Serial.println("Start the STA mode, data valid in the eeprom");
    }
    // ! Start the STA mode, data valid in the eeprom
    /* GET THE PARAMETERS IN SEPAERATE VARIABLES */
    char* ssidFromEeprom = extractParameter(valuesOutEeprom, 0);
    char* pswFromEeprom = extractParameter(valuesOutEeprom, 1);
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
      server.on("/server.css", HTTP_GET, handleOnHomeSTACSS);
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
    // TODO Display something the boot screen if the station mode is enabled
  } else {
    display.showInitScreenAp();
  }   
     
}

void loop() {
  if (STATION) {
    requestHandler httpHandler;
    // 1D array
    uint8_t* arrayCoins = httpHandler.requestGetCoinsDatabase(false);
    saveCoinsToEeprom(arrayCoins, true);
    dumbDataEeprom();
    // 2D array
    uint8_t** arrayOfCoins = httpHandler.fillArrayWithCoinIds(arrayCoins);
    listCoins(arrayOfCoins);

    for (uint8_t j = 0; j < MAX_NUMBER_OF_COINS; j++){
      char coinName[4];
      for (uint8_t k = 0; k < BYTES_PER_ID_COIN; k++){
        if (arrayOfCoins[j][k] != '\0'){
          coinName[k] = arrayOfCoins[j][k];
        }
      }
      if (strlen(coinName) > 0){
        httpHandler.requestFromApi(coinName, &infoCoin, true);
        httpHandler.requestInsertDataCoin(&infoCoin, true);
      }
    }
  }
  delay(5000);
}


/*
1. check if coin

*/

