/// @file  
#include <main.h>

void updateAll();

void setup()
{
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Serial.println();
  initEeprom();
  display.init();
  LittleFS.begin();

  buttonState = digitalRead(buttonPin);
  if (buttonState){
    clearEeprom();
  }

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
    display.boot(); 
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
    updateAll();
  } else {
    display.showInitScreenAp();
  }   
  display.setToDefault();
  display.clear(ILI9341_BLACK);
  for (uint8_t i = 0; i < 8; i++){
    display.placeBoxtInCenter(X_CENTER, Y_CENTER, 220 + i, 80 + i, ILI9341_WHITE);
  }
  display.placeBoxtInCenter(X_CENTER, Y_CENTER, 220, 80, ILI9341_BLUE, true);
  display.placeTextInCenter(WiFi.localIP().toString(), X_CENTER, Y_CENTER,2, ILI9341_WHITE);
  delay(3000);
  
  display.drawTriangleButton(10, Y_CENTER, 30, (Y_CENTER  + Y_CENTER/3), 30, (Y_CENTER  - Y_CENTER/3), &buttonLeft, ILI9341_BLUE);
  display.drawTriangleButton((DISPLAY_X_MAX - 10), Y_CENTER, (DISPLAY_X_MAX - 30), (Y_CENTER  + Y_CENTER/3), (DISPLAY_X_MAX - 30), (Y_CENTER  - Y_CENTER/3), &buttonRight, ILI9341_BLUE);
  display.drawRectButton(40, 60, 238, 160, &buttonGraph);
}

void loop() {

  if (STATION) {
    if (display.ts.touched()){
      delay(25);
      if (display.ts.touched()){
        tsPressed = true;
      }
    }  

    if (!display.ts.touched() && tsPressed && !tsReleased){
        tsReleased = true;
    }

    if (!display.ts.touched() && tsPressed && tsReleased){
      TS_Point p = display.ts.getPoint();
      p.x = map(p.x, TS_MAXX, TS_MINX, display.tft.width(), 0);
      p.y = map(p.y, TS_MAXY, TS_MINY, display.tft.height(), 0);
      if(display.checkIfPressed(&buttonLeft, p.x, p.y)){
        if (indexGraph == 0){
          indexGraph = amountOfCoins - 1;
        } else {
          indexGraph--;
        }
      } else if(display.checkIfPressed(&buttonRight, p.x, p.y)){
        if (indexGraph == amountOfCoins - 1){
          indexGraph = 0;
        } else {
          indexGraph++;
        }
      } else if(display.checkIfPressed(&buttonGraph, p.x, p.y)) {
        showData = !showData;
      }
      tsPressed = false;
      tsReleased = false;
      flagUpdateScreen = true;
    }

    if (flagUpdateScreen){
      // Show pop up menu and intel
      if (showData == false){
        // get last 8 points out db.
        httpHandler.requestGetListDataCoin(arrayOfCoins, indexGraph, DATAPOINTS_GRAPH, &dataForGraph, true);
        // flush area 
        display.tft.fillRect(0, 0, DISPLAY_X_MAX, 60, ILI9341_BLACK);
        display.tft.fillRect(40, 60, 240, 160, ILI9341_BLACK);
        display.drawGraph(&dataForGraph, 51, 220, 217, 160, 10);
        // Plot coin name
        display.tft.setCursor(10, 15);
        char coinName[4];
        for (uint8_t i = 0; i < 4; i++){
            coinName[i] = arrayOfCoins[indexGraph][i];
        }
        display.tft.setTextSize(5);  
        display.tft.print(coinName);
        // plot last price know in DB
        display.tft.setCursor(140, 29);
        display.tft.setTextSize(3);
        memset(bufferDisplay, 0, 32);
        sprintf(bufferDisplay, "%0.4f", dataForGraph.rawXAxis[7]); 
        display.tft.print(bufferDisplay);
      } else {
        for (uint8_t i = 0; i < 8; i++){
          display.placeBoxtInCenter(X_CENTER, Y_CENTER + 10, 220 + i, 80 + i, ILI9341_WHITE);
        }
        display.placeBoxtInCenter(X_CENTER, Y_CENTER + 10, 220, 80, ILI9341_BLUE, true);
        sprintf(bufferDisplay, "Max: %0.4f", dataForGraph.maxMin[0]); 
        display.placeTextInCenter(bufferDisplay, X_CENTER, Y_CENTER - 10, 2, ILI9341_WHITE);
        sprintf(bufferDisplay, "Min: %0.4f", dataForGraph.maxMin[1]); 
        display.placeTextInCenter(bufferDisplay, X_CENTER, Y_CENTER + 10, 2, ILI9341_WHITE);
        sprintf(bufferDisplay, "Procent: %0.4f", dataForGraph.procent); 
        display.placeTextInCenter(bufferDisplay, X_CENTER, Y_CENTER + 30, 2, ILI9341_WHITE);
      }
      flagUpdateScreen = false;
    }


    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL_UPDATE) {
      previousMillis = currentMillis;
      if (Serial){
        Serial.println("void loop(), time over, time to update");
      }
      updateAll();
    }
  }
}

void updateAll(){
  // 1D array
  uint8_t* arrayCoins = httpHandler.requestGetCoinsDatabase(false);
  saveCoinsToEeprom(arrayCoins, true);
  dumbDataEeprom();
  // 2D array
  arrayOfCoins = httpHandler.fillArrayWithCoinIds(arrayCoins);
  listCoins(arrayOfCoins);

  for (uint8_t j = 0; j < MAX_NUMBER_OF_COINS; j++){
    char coinName[4];
    for (uint8_t k = 0; k < BYTES_PER_ID_COIN; k++){
      if (arrayOfCoins[j][k] != '\0'){
        coinName[k] = arrayOfCoins[j][k];
      } else {
        coinName[k] = '\0';
      }
    }
    if (strlen(coinName) >= BYTES_PER_ID_COIN - 2){
      amountOfCoins++;
      httpHandler.requestFromApi(coinName, &infoCoin, true);
      httpHandler.requestInsertDataCoin(&infoCoin, true);
    }
  }
}