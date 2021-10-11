#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define REQUEST_ADDRESS_API     "http://api.iot.hva-robots.nl/crypto/"

typedef struct {
    String idCoin;
    String priceCoin;
} namePriceStruct;


class requestHandler{
    
    public:
        void requestFromApi(String coinId, namePriceStruct *namePriceStruct, uint8_t debug);

    private:
        WiFiClient wifiClient;    
        HTTPClient http; 
        StaticJsonDocument<48> filter;
        StaticJsonDocument<128> doc;
};
#endif // REQUESTHANDLER_H