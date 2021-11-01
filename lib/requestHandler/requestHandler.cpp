#include <requestHandler.h>

/**************************************************************************/
/*!
  @brief    Asks the API for the data of a coin and place it in a Struct
  @param    coinId                      String: SSID name, max 32 bytes
  @param    namePriceTimeStruct         Struct*: pointer to a Struct with 3 Strings
  @param    debug                       uint8_t: True, false, when true print more data of the call
*/
/**************************************************************************/
void requestHandler::requestFromApi(String coinId, namePriceTimeStruct *namePriceTimeStruct, uint8_t debug){
    if (Serial){
        Serial.println("Class requestHandler, function: requestFromApi");
    }
    String completPath = REQUEST_ADDRESS_API + coinId;
    static char buffer[64];
    if (Serial && debug){
        sprintf(buffer,"Making request to %s", completPath.c_str());
        Serial.println(buffer); 
    }
    String payload;
    HTTPClient http;
    http.begin(clientHttpCall, completPath); 
    int httpCode = http.GET(); 
    if (httpCode > 0) { 
      payload = http.getString();
      if (Serial && debug){
          Serial.println(payload); 
      }
    }
    http.end();


    JsonObject filter_data = filter.createNestedObject("data");
    filter_data["id"] = true;
    filter_data["price"] = true;
    filter_data["price_timestamp"] = true;

    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));

    if (error) {
        if (Serial){
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
    } else{
        namePriceTimeStruct->idCoin = doc["data"]["id"].as<String>();
        namePriceTimeStruct->priceCoin =  doc["data"]["price"].as<String>();
        String correctDate = doc["data"]["price_timestamp"].as<String>();
        correctDate.replace("Z", "");
        namePriceTimeStruct->timeStamp = correctDate;
    }
}

/**************************************************************************/
/*!
  @brief    Insert data of a coin of the API into the Database
  @param    namePriceTimeStruct         Struct*: Pointer to a Struct with 3 Strings
  @param    debug                       uint8_t: True, false, when true print more data of the call
  @return   0 / 1 / 2                   uint8_t: 0, Call unsuccessful
                                                 1, Call successful
                                                 2, Error (404)
*/
/**************************************************************************/
uint8_t requestHandler::requestInsertDataCoin(namePriceTimeStruct *namePriceTimeStruct, uint8_t debug){
    if (Serial){
        Serial.println("Class requestHandler, function: requestInsertDataCoin");
    }
    String payload;
    sprintf(fullHttpCall, "%s%d%s/%s/%s/%s", REQUEST_ADDRESS_DATABASE_BASE, DATABASE_PORT_NUMBER, REQUEST_DATABASE_INSERT_DATA,
                                             namePriceTimeStruct->idCoin.c_str(), 
                                             namePriceTimeStruct->timeStamp.c_str(), 
                                             namePriceTimeStruct->priceCoin.c_str());
    if (debug){
        char buffer[256];
        sprintf(buffer, "Making call to URL: %s", fullHttpCall);
        Serial.println(buffer);
    }
    HTTPClient http;
    http.begin(clientHttpCall, fullHttpCall); 
    int httpCode = http.GET(); 
    if (httpCode > 0) { 
      payload = http.getString();
      if (Serial && debug){
          Serial.println(payload); 
      }
    }
    http.end();
    if (payload == "true"){
        return 1;
    } else if (payload == "false"){
        return 0;
    } else {
        return 2;
    }
}
/**************************************************************************/
/*!
  @brief    Asks the API (DB) for all the coins thats needs to be tracked
  @param    debug                       uint8_t: True, false, when true print more data of the call
  @return   arrayCoins                  uint8_t[32]: Array of the data from the call in uint8_t
*/
/**************************************************************************/
uint8_t* requestHandler::requestGetCoinsDatabase(uint8_t debug){
    if (Serial){
        Serial.println("Class requestHandler, function: requestGetCoinsDatabase");
    }
    String payload;
    sprintf(fullHttpCall, "%s%d%s", REQUEST_ADDRESS_DATABASE_BASE, 
                                    DATABASE_PORT_NUMBER, 
                                    REQUEST_DATABASE_LIST_COINS);
    if (Serial && debug){
        char buffer[256];
        sprintf(buffer, "Making call to URL: %s", fullHttpCall);
        Serial.println(buffer);
    }
    HTTPClient http;
    http.begin(clientHttpCall, fullHttpCall); 
    int httpCode = http.GET(); 
    if (httpCode > 0) { 
      payload = http.getString();
      if (Serial && debug){
          Serial.println(payload); 
      }
    }
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        if (Serial && debug){
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
    }
    static uint8_t arrayCoins[BYTES_NEEDED_FROM_EEPROM_COINS];
    uint8_t index = 0;
    for (JsonObject item : doc.as<JsonArray>()) {        
        String coin = item["coin_id"].as<String>();
        for (uint8_t i  = 0; i < coin.length(); i++){
            if (Serial && debug){
                Serial.println(coin[i]);
                Serial.print(arrayCoins[index + i]);    
                Serial.println();   
            }
            arrayCoins[index + i] = (uint8_t)coin[i];           
        }
        index = index + BYTES_PER_ID_COIN;
    }
    return arrayCoins;
}

/**************************************************************************/
/*!
  @brief    From 1D array to 2D array 
  @param    arrayOfTheCoins             uint8_t*: Pointer to the 1D array
  @return   arrayToReturn               uint8_t[8][4]: Array of the data from the 1D array
                                                       placed in a 2D array
*/
/**************************************************************************/
uint8_t** requestHandler::fillArrayWithCoinIds(uint8_t* arrayOfTheCoins){
    if (Serial){
        Serial.println("Class requestHandler, function: fillArrayWithCoinIds");
    }
    // init the 2D array
    uint8_t** arrayToReturn = new uint8_t* [MAX_NUMBER_OF_COINS];

    for (uint8_t i = 0; i < MAX_NUMBER_OF_COINS; i++){
        arrayToReturn[i] = new uint8_t[BYTES_PER_ID_COIN];
    }
    uint8_t index = 0;
    for (uint8_t j = 0; j < MAX_NUMBER_OF_COINS; j++){
        for (uint8_t k = 0; k < BYTES_PER_ID_COIN; k++){
            arrayToReturn[j][k] = arrayOfTheCoins[index];
            index++;
        }
    }
    return arrayToReturn;
}