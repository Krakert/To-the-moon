#ifndef WIFI_H_
#define WIFI_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <String.h>
#include <EEPROM.h>

/* PARAMETERS STANDARD AP*/ 
#define WIFI_AP_STANDARD_SSID           "To the Moon Setup"
#define WIFI_AP_STANDARD_PSW            "12345678"
#define WIFI_AP_STANDARD_IP              192,168,1,1
#define WIFI_AP_STANDARD_GATEWAY         192,168,1,1
#define WIFI_AP_STANDARD_SUBNET          255,255,255,0
#define WIFI_AP_STANDARD_PORT            80

/* PARAMETERS EEPROM*/
#define STANDARD_EEPROM_SIZE             (32*2)

/* FUNCTIONS SOFTAP */
uint8_t setupSoftApConfig(IPAddress local_ip = IPAddress(WIFI_AP_STANDARD_IP), 
                          IPAddress gateway = IPAddress(WIFI_AP_STANDARD_GATEWAY),
                          IPAddress subnet = IPAddress(WIFI_AP_STANDARD_SUBNET));
uint8_t setupSoftAp(String SSID = WIFI_AP_STANDARD_SSID, String PSW = WIFI_AP_STANDARD_PSW);

/* FUNCTIONS EEPROM */
void initEeprom(uint8_t nmrOfBytes = STANDARD_EEPROM_SIZE);
uint8_t saveApToEeprom(String apSsid, String ApPsw);
uint8_t getConfigForEeprom(uint8_t nmrOfBytes = STANDARD_EEPROM_SIZE);

#endif