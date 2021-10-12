#ifndef MAIN_H
#define MAIN_H
#include "LittleFS.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


/* ADDED HEADER FILES SELFMADE */
#include <wifi.h>
#include <displayHandler.h>
#include <eepromHandler.h>
#include <requestHandler.h>

uint8_t STATION = false;

displayHandler display;
namePriceStruct infoCoin;

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