#include <eeprom_handler.h>
/**************************************************************************/
/*!
  @brief    Set up A piece of the flash in the EEPROM
  @param    nmrOfBytes              uint8_t: total number of bytes of the EEPROM to init
*/
/**************************************************************************/
void initEeprom(uint8_t nmrOfBytes){
    if (Serial){
        Serial.println("Class eeprom_handler, function: initEeprom");
    }
    EEPROM.begin(nmrOfBytes);
}

/**************************************************************************/
/*!
  @brief    Write SSID and PSW to new AP to the EEPROM
  @param    apSsid                  String: SSID name, max 32 bytes
  @param    ApPsw                   String: PSW name, max 32 bytes
  @return   res                     uint8_t: 
                                    0) both successfull,
                                    1) failed write SSID, 
                                    2) both failed, 
                                    3) failed write PS
*/
/**************************************************************************/
uint8_t saveApToEeprom(String apSsid, String ApPsw){
    if (Serial){
        Serial.println("Class eeprom_handler, function: saveApToEeprom");
    }
    uint8_t res = 0;
    
    // Clear EEPROM
    clearEeprom(BYTES_NEEDED_FROM_EEPROM);

    // Write SSID
    if ((uint8_t) apSsid.length() < PARAMETER_SIZE_IN_BYTES + 1){
        for(uint8_t i = 0; i < apSsid.length(); i ++ ) {
            EEPROM.write(i, (uint8_t)apSsid[i]);
        }   
    } else{
        res = res + 1;
    }

    // Offset half of the flash size and write PSW
    if ((uint8_t) ApPsw.length() < PARAMETER_SIZE_IN_BYTES + 1){
        for(uint8_t i = PARAMETER_SIZE_IN_BYTES; i < (PARAMETER_SIZE_IN_BYTES + ApPsw.length()); i ++ ) {
            EEPROM.write(i, (uint8_t)ApPsw[(i - PARAMETER_SIZE_IN_BYTES)]);
        }   
    } else{
        if (res == 1){
            res = res + 1;
        } else {
            res = 3;
        }
    }
    EEPROM.commit();
    return res;
}

/**************************************************************************/
/*!
  @brief    Read data from the EEPROM
  @param    nmrOfBytes              uint8_t: total number of bytes of the EEPROM to read
  @return   valueEeprom             uint8_t[][]: 
                                    [0-32][]) 32 bytes, should contain SSID
                                    [][0-32]) 32 bytes, should contain SSID
*/
/**************************************************************************/
//  TODO reconstruct uint8_t to chars
uint8_t** getConfigForEeprom(uint8_t nmrOfBytes){
    if (Serial){
        Serial.println("Class eeprom_handler, function: getConfigForEeprom");
    }

    // init the 2D array
    uint8_t** valueEeprom = new uint8_t* [PARAMETERS_EEPROM];

    for (uint8_t m = 0; m < PARAMETERS_EEPROM; m++){
        valueEeprom[m] = new uint8_t[BYTES_NEEDED_FROM_EEPROM];
    }

    for(uint8_t i = 0; i < PARAMETER_SIZE_IN_BYTES; i ++ ) {
        valueEeprom[0][i] = EEPROM.read(i);
    }
    for(uint8_t j = PARAMETER_SIZE_IN_BYTES; j < BYTES_NEEDED_FROM_EEPROM; j ++ ) {
        valueEeprom[1][j - PARAMETER_SIZE_IN_BYTES] = EEPROM.read(j);
    }  

    return valueEeprom;
}

/**************************************************************************/
/*!
  @brief    Serial print the data read from the EEPROM
  @param    valuesOutEeprom         uint8_t**: pointer to a uint8_t 2D array
  @param    raw                     uint8_t: If true print the uint8_t data (ascii)
*/
/**************************************************************************/
void dumbDataEeprom(uint8_t** valuesOutEeprom, uint8_t raw){
    char intToChar[32] = { 0 };
    if (Serial){
        Serial.println("Class eeprom_handler, function: dumbDataEeprom");
        for (uint8_t i = 0; i < PARAMETERS_EEPROM; i++){
            for (uint8_t j = 0; j < PARAMETER_SIZE_IN_BYTES; j++){
                if (raw){
                    Serial.print(valuesOutEeprom[i][j]);
                } else {
                    intToChar[j] = valuesOutEeprom[i][j];
                    Serial.print(intToChar[j]);
                }
            }
            Serial.println();
        }   
    }   
}
/**************************************************************************/
/*!
  @brief    Set a the bytes to '\0'
  @param    nmrOfBytes              uint8_t: total number of bytes of the EEPROM to clear
*/
/**************************************************************************/
void clearEeprom(uint8_t nmrOfBytes){
    if (Serial){
        Serial.println("Class eeprom_handler, function: clearEeprom");
    }
    // clear EEPROM data
    for (int i = 0; i < nmrOfBytes; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

/**************************************************************************/
/*!
  @brief    Check if the data is not empty
  @param    valuesOutEeprom         uint8_t**: pointer to a uint8_t 2D array
  @return   res                     uint8_t: 
                                    0) both strings a empty
                                    1) only one of the two is not empty, 
                                    2) both have data,                   
*/
/**************************************************************************/
uint8_t checkIfNotEmpty(uint8_t** valuesOutEeprom){
    char intToChar[32] = { 0 };
    uint8_t res = 0;
    if (Serial){
        Serial.println("Class eeprom_handler, function: checkIfNotEmpty");
    } 
    for (uint8_t i = 0; i < PARAMETERS_EEPROM; i++){
        for (uint8_t j = 0; j < PARAMETER_SIZE_IN_BYTES; j++){
            intToChar[j] = valuesOutEeprom[i][j];
        }
        if (strlen(intToChar) > 0){
            res = res + 1;
        }
    }
    Serial.println(res);  
    return res;
}