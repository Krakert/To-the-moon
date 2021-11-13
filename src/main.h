/// @file  
#ifndef MAIN_H
#define MAIN_H
#include <LittleFS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

/* ADDED HEADER FILES SELFMADE */
#include <wifiHandler.h>
#include <displayHandler.h>
#include <eepromHandler.h>
#include <requestHandler.h>

unsigned long previousMillis = 0;
const int buttonPin = 5;   
uint8_t buttonState = 0;

#define INTERVAL_UPDATE 60*1000

uint8_t STATION = false;
uint8_t FIRSTRUN = true;
uint8_t tsPressed = false;
uint8_t tsReleased = false;
uint8_t indexGraph = 0;
uint8_t amountOfCoins = 0;
uint8_t flagUpdateScreen = true;
uint8_t showData = false;

requestHandler httpHandler;
displayHandler display;
namePriceTimeStruct infoCoin;
sizeButtonStruct buttonLeft;
sizeButtonStruct buttonRight;
sizeButtonStruct buttonGraph;
dataGraphStruct dataForGraph;
uint8_t** arrayOfCoins;
static char bufferDisplay[32];

#define STANDARD_CONFIG_AP   true 

/*
  ! optional IP stettings can be used to override the default settings,
  ! set STANDARD_CONFIG_AP to false to use. 
*/
#if(!STANDARD_CONFIG_AP)
  #define WIFI_AP_SSID  "Own SSID"
  #define WIFI_AP_PSW   "OWn PSW"
  IPAddress LOCAL_IP(192,168,1,1);
  IPAddress GATEWAY(192,168,1,1);
  IPAddress SUBNET(255,255,255,0);
  #define WIFI_AP_PORT  80
  AsyncWebServer server(WIFI_AP_PORT);
#else
AsyncWebServer server(WIFI_AP_STANDARD_PORT);
#endif

#endif // MAIN_H