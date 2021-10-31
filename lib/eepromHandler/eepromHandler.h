#ifndef EEPROMHANDLER_H
#define EEPROMHANDLER_H

#include <HardwareSerial.h>
#include <cstdio>
#include <String.h>
#include <EEPROM.h>

/* PARAMETERS EEPROM*/
#define PARAMETER_SIZE_IN_BYTES          32
#define PARAMETERS_EEPROM                2
#define BYTES_NEEDED_FROM_EEPROM_WIFI    (PARAMETER_SIZE_IN_BYTES*PARAMETERS_EEPROM)

#define BYTES_PER_ID_COIN                4
#define MAX_NUMBER_OF_COINS              8
#define BYTES_NEEDED_FROM_EEPROM_COINS   (BYTES_PER_ID_COIN*MAX_NUMBER_OF_COINS)
#define FULL_AMOUNT_OF_BYTES_EEPROM      ((BYTES_PER_ID_COIN*MAX_NUMBER_OF_COINS)+(PARAMETER_SIZE_IN_BYTES*PARAMETERS_EEPROM))

/* FUNCTIONS EEPROM */
void initEeprom(uint8_t nmrOfBytes = FULL_AMOUNT_OF_BYTES_EEPROM);
uint8_t saveApToEeprom(String apSsid, String ApPsw, uint8_t debug = false);
uint8_t** getConfigFormEeprom(uint8_t nmrOfBytes = BYTES_NEEDED_FROM_EEPROM_WIFI);
void dumbDataEeprom();
void clearEeprom(uint8_t nmrOfBytes = BYTES_NEEDED_FROM_EEPROM_WIFI);
uint8_t checkIfNotEmpty(uint8_t** valuesOutEeprom);
char * extractParameter(uint8_t** valuesOutEeprom, uint8_t parameter);

void saveCoinsToEeprom(uint8_t* arrayCoins, uint8_t debug = false);
void listCoins(uint8_t** arrayOfCoins);


#endif // EEPROMHANDLER_H