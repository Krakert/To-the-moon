/// @file  
#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <eepromHandler.h>
#include <displayHandler.h>

#define REQUEST_ADDRESS_API                 "http://api.iot.hva-robots.nl/crypto/"
#define DATABASE_PORT_NUMBER                8112
#define REQUEST_ADDRESS_DATABASE_BASE       "http://oege.ie.hva.nl:"
#define REQUEST_DATABASE_INSERT_DATA        "/insert/data"
#define REQUEST_DATABASE_LIST_COINS         "/get/coins"
#define REQUEST_DATABASE_LIST_DATA_COIN     "/get/"

typedef struct {
    String idCoin;
    String priceCoin;
    String timeStamp;
} namePriceTimeStruct;



class requestHandler{
    
    public:
        requestHandler();
        void requestFromApi(String coinId, namePriceTimeStruct *namePriceTimeStruct, uint8_t debug);
        uint8_t requestInsertDataCoin(namePriceTimeStruct *namePriceTimeStruct, uint8_t debug);
        uint8_t* requestGetCoinsDatabase(uint8_t debug);
        uint8_t** fillArrayWithCoinIds(uint8_t* arrayOfTheCoins);
        void requestGetListDataCoin(uint8_t** arrayOfCoins, uint8_t index, uint8_t amountOfPoints, dataGraphStruct *dataForGraph, uint8_t debug);
    private:
        WiFiClient clientHttpCall;    
        StaticJsonDocument<64> filter;
        char fullHttpCall[128];
};
#endif // REQUESTHANDLER_H
