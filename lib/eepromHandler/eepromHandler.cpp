#include <eepromHandler.h>
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
  @param    debug                   uint8_t: 1) execute dumbDataEeprom()
  @return   res                     uint8_t: 
                                    0) both successfull,
                                    1) failed write SSID, 
                                    2) both failed, 
                                    3) failed write PS
*/
/**************************************************************************/
uint8_t saveApToEeprom(String apSsid, String ApPsw, uint8_t debug){
    if (Serial){
        Serial.println("Class eeprom_handler, function: saveApToEeprom");
    }
    uint8_t res = 0;
    
    // Clear EEPROM
    clearEeprom(FULL_AMOUNT_OF_BYTES_EEPROM);

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
    if (debug){
        dumbDataEeprom();
    }
    return res;
}

/**************************************************************************/
/*!
  @brief    Read data from the EEPROM
  @param    nmrOfBytes              uint8_t: total number of bytes of the EEPROM to read
  @return   valueEeprom             uint8_t[][]: 
                                    [0-32][]) 32 bytes, should contain SSID
                                    [][0-32]) 32 bytes, should contain PSW
*/
/**************************************************************************/
uint8_t** getConfigFormEeprom(uint8_t nmrOfBytes){
    if (Serial){
        Serial.println("Class eeprom_handler, function: getConfigFormEeprom");
    }

    // init the 2D array
    uint8_t** valueEeprom = new uint8_t* [PARAMETERS_EEPROM];

    for (uint8_t i = 0; i < PARAMETERS_EEPROM; i++){
        valueEeprom[i] = new uint8_t[BYTES_NEEDED_FROM_EEPROM_WIFI];
    }

    for(uint8_t j = 0; j < PARAMETER_SIZE_IN_BYTES; j ++ ) {
        valueEeprom[0][j] = EEPROM.read(j);
    }
    for(uint8_t k = PARAMETER_SIZE_IN_BYTES; k < BYTES_NEEDED_FROM_EEPROM_WIFI; k ++ ) {
        valueEeprom[1][k - PARAMETER_SIZE_IN_BYTES] = EEPROM.read(k);
    }  

    return valueEeprom;
}

/**************************************************************************/
/*!
  @brief    Serial print the data read from the EEPROM, in char form.
*/
/**************************************************************************/
void dumbDataEeprom(){
    char intToChar[FULL_AMOUNT_OF_BYTES_EEPROM] = { 0 };
    if (Serial){
        Serial.println("Class eepromHandler, function: dumbData");
        for (uint8_t i  = 0; i < BYTES_NEEDED_FROM_EEPROM_WIFI + BYTES_NEEDED_FROM_EEPROM_COINS; i++){
            intToChar[i] = EEPROM.read(i);
                Serial.print(intToChar[i]);
        } 
        Serial.println();
    }   
}
/**************************************************************************/
/*!
  @brief    Clear the EEPROM data by writing '\0' to all the bytes
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
    char intToChar[PARAMETER_SIZE_IN_BYTES] = { 0 };
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
    return res;
}

/**************************************************************************/
/*!
  @brief    Extract a parameter from the 2D array that contain the data of 
            the EEPROM and return it.
  @param    valuesOutEeprom         uint8_t**: pointer to a uint8_t 2D array
  @param    parameter               uint8_t**: 0) first, 1) second parameter of  
  @return   intToCharSsid /         char[32]: 32 char, only filled if not null
            intToCharPsw            char[32]: 32 char, only filled if not null
*/
/**************************************************************************/
char * extractParameter(uint8_t** valuesOutEeprom, uint8_t parameter){
    static char intToCharSsid[PARAMETER_SIZE_IN_BYTES] = { 0 };
    static char intToCharPsw[PARAMETER_SIZE_IN_BYTES] = { 0 };
    if (Serial){
        Serial.println("Class eeprom_handler, function: extractParameter");
    }
    if (parameter == 0) {
        for (uint8_t i = 0; i < PARAMETER_SIZE_IN_BYTES; i++){
            if (valuesOutEeprom[0][i] != '\0'){
                intToCharSsid[i] = valuesOutEeprom[0][i];;
            }
        }
    return intToCharSsid;
    } else if (parameter == 1){
        for (uint8_t j = 0; j < PARAMETER_SIZE_IN_BYTES; j++){
            if (valuesOutEeprom[1][j] != '\0'){
                intToCharPsw[j] = valuesOutEeprom[1][j];;
            }
        }
    return intToCharPsw;    
    }
    return 0;
}
/**************************************************************************/
/*!
  @brief    Save all the coin ID to the EEPROM, compare data form API call to the EEPROM
            Only rewrite the data that is different than in the EEPROM.
  @param    arrayCoins              uint8_t**: pointer to a uint8_t 2D array
  @param    debug                   uint8_t: If true print changes that are being made  
*/
/**************************************************************************/
void saveCoinsToEeprom(uint8_t* arrayCoins, uint8_t debug){
    if (Serial){
        Serial.println("Class eepromHandler, function: saveCoinsToEeprom");
    }
    static char dataInEeprom[BYTES_NEEDED_FROM_EEPROM_COINS];

    // get data from EEPROM
    for (uint8_t i = 0; i < BYTES_NEEDED_FROM_EEPROM_COINS; i ++){
        dataInEeprom[i] = EEPROM.read(i + BYTES_NEEDED_FROM_EEPROM_WIFI);
    }
    // compare EEPROM with data from call, if diff, write to
    for (uint8_t j = 0; j < BYTES_NEEDED_FROM_EEPROM_COINS; j++){
        if (dataInEeprom[j] != arrayCoins[j]){
            if (Serial && debug){
                Serial.println("Found diff");
                char buffer[32];
                sprintf(buffer, "In eeprom: %c, Byte: %d", char(dataInEeprom[j]), j + BYTES_NEEDED_FROM_EEPROM_WIFI);
                Serial.println(buffer);
                sprintf(buffer, "From call: %c, Byte: %d", char(arrayCoins[j]), j + BYTES_NEEDED_FROM_EEPROM_WIFI);
                Serial.println(buffer);
            }
            EEPROM.write(j + BYTES_NEEDED_FROM_EEPROM_WIFI, arrayCoins[j]);
        }
    }
    EEPROM.commit();
}

/**************************************************************************/
/*!
  @brief    Print the 2D array and list the coins.
  @param    arrayCoins              uint8_t**: pointer to a uint8_t 2D array 
*/
/**************************************************************************/
void listCoins(uint8_t** arrayOfCoins){
    if (Serial){
        Serial.println("Class eepromHandler, function: listCoins");
        for (uint8_t j = 0; j < MAX_NUMBER_OF_COINS; j++){
            static char coinName[4];
            for (uint8_t k = 0; k < BYTES_PER_ID_COIN; k++){
                if (arrayOfCoins[j][k] != '\0'){    
                coinName[k] = arrayOfCoins[j][k];
                } else {
                    coinName[k] = '\0';
                }
            }
            if (strlen(coinName) >= BYTES_PER_ID_COIN - 2){
                Serial.print(coinName);
                Serial.println();
            }
        }
    }
}