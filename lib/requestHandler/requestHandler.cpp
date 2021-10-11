#include <requestHandler.h>

/**************************************************************************/
/*!
  @brief    Asks the API for the data of a coin and place it in a Struct
  @param    coinId                  String: SSID name, max 32 bytes
  @param    namePriceStruct         Struct*: pointer to a Struct with 2 Strings
*/
/**************************************************************************/
void requestHandler::requestFromApi(String coinId, namePriceStruct *namePriceStruct, uint8_t debug){
    
    if (Serial){
        Serial.println("Class requestHandler, function: requestFromApi");
    }
    String completPath = REQUEST_ADDRESS_API + coinId;
    String payload;
    http.begin(wifiClient, completPath); 
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

    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));

    if (error) {
        if (Serial){
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
    } else{
    namePriceStruct->idCoin = doc["data"]["id"].as<String>();
    namePriceStruct->priceCoin =  doc["data"]["price"].as<String>();
    }
}
