#ifndef eeprom_handler_H
#define eeprom_handler_H

#include <HardwareSerial.h>
#include <String.h>
#include <EEPROM.h>

/* PARAMETERS EEPROM*/
#define PARAMETER_SIZE_IN_BYTES          32
#define PARAMETERS_EEPROM                2
#define BYTES_NEEDED_FROM_EEPROM         (PARAMETER_SIZE_IN_BYTES*PARAMETERS_EEPROM)

/* FUNCTIONS EEPROM */
void initEeprom(uint8_t nmrOfBytes = BYTES_NEEDED_FROM_EEPROM);
uint8_t saveApToEeprom(String apSsid, String ApPsw, uint8_t debug = false);
uint8_t** getConfigForEeprom(uint8_t nmrOfBytes = BYTES_NEEDED_FROM_EEPROM);
void dumbDataEeprom(uint8_t** valuesOutEeprom, uint8_t raw = false);
void clearEeprom(uint8_t nmrOfBytes = BYTES_NEEDED_FROM_EEPROM);
uint8_t checkIfNotEmpty(uint8_t** valuesOutEeprom);
char * extractParameter(uint8_t** valuesOutEeprom, uint8_t parameter);
#endif // eeprom_handler_H