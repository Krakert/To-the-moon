#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <String.h>
#include <eepromHandler.h>

/* PARAMETERS STANDARD AP*/ 
const static String WIFI_AP_STANDARD_SSID =        "To the Moon Setup";
const static String WIFI_AP_STANDARD_PSW =         "12345678";
const static uint8_t WIFI_AP_STANDARD_IP[] =       {192,168,1,1};
const static uint8_t WIFI_AP_STANDARD_GATEWAY[] =  {192,168,1,1};
const static uint8_t WIFI_AP_STANDARD_SUBNET[] =   {255,255,255,0};
const static uint8_t WIFI_AP_STANDARD_PORT =       80;


/* FUNCTIONS SOFTAP */
uint8_t setupSoftApConfig(IPAddress local_ip = IPAddress(WIFI_AP_STANDARD_IP), 
                          IPAddress gateway = IPAddress(WIFI_AP_STANDARD_GATEWAY),
                          IPAddress subnet = IPAddress(WIFI_AP_STANDARD_SUBNET));
uint8_t setupSoftAp(String SSID = WIFI_AP_STANDARD_SSID, String PSW = WIFI_AP_STANDARD_PSW);

/* IF NAVIGATED TO A UNKNOW PAGE */
void handleNotFound(AsyncWebServerRequest *request);

/* FUNCTIONS SERVER REQUEST, STA MODE */
void handleOnHomeSTA(AsyncWebServerRequest *request);
void handleOnHomeSTACSS(AsyncWebServerRequest *request);

/* FUNCTIONS SERVER REQUEST, AP MODE */
void handleOnHomeAP(AsyncWebServerRequest *request);
void handleGetDataAP(AsyncWebServerRequest *request);

const char INDEX_HTML_AP[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" /> 
</head>
<style>
    body {background-color: #E7E9EB; font-family: roboto,sans-serif; color: black;}
    h1 {color: #1967d2; font-size: 200%;}
    .center {text-align: center;}
    .form {padding: 5px;}
</style>
<div class="center">
    <h1>Connect to your home network</h1>
        <form action="/get" class="form">
            <div class="form">
                Network name: <input type="text" name="ssidFromAp">
            </div>
            <div class="form">
                Password: <input type="text" name="passwordFromAp">
            </div>
            <input type="submit" value="Submit">
        </form> 
</div>
</body>
</html>
)=====";

const char INDEX_HTML_AP_SUCCES[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" /> 
</head>
<style>
    body {background-color: #E7E9EB; font-family: roboto,sans-serif; color: black;}
    h1 {color: #1967d2; font-size: 200%;}
    .center {text-align: center;}
    .form {padding: 5px;}
</style>
<div class="center">
    <h1>All data is saved!</h1>
    <p>The device will try to connect to the network you entered</p>
</div>
</body>
</html>
)=====";


#endif // WIFIHANDLER_H