#include <wifi.h>
#include <EEPROM.h>
/**************************************************************************/
/*!
  @brief    Configure the soft access point’s network interface.
  @param    local_ip                IPAddress: IP address of the AP
  @param    gateway                 IPAddress: gateway address of the AP
  @param    subnet                  IPAddress: subnet address of the AP
  @return   1 / 0                   uint8_t: 1) successfully, 2) failed
*/
/**************************************************************************/
uint8_t setupSoftApConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet){
    if (Serial){
        Serial.println("Class wifi, function: setupSoftApConfig");
    }
    if (WiFi.softAPConfig(local_ip, gateway, subnet)){
        return 1;
    } else {
        return 0;  
    }
}

/**************************************************************************/
/*!
  @brief    Set up a soft access point to establish a Wi-Fi network.
  @param    SSID                    String: SSID for the AP
  @param    PSW                     String: Password for the AP
  @return   1 / 0                   uint8_t: 1) successfully, 2) failed
*/
/**************************************************************************/
uint8_t setupSoftAp(String SSID, String PSW){
    if (Serial){
        Serial.println("Class wifi, function: setupSoftAp");
    }
    if (WiFi.softAP(SSID, PSW)){
        return 1;
    } else {
        return 0;  
    }
}

/**************************************************************************/
/*!
  @brief    Set up A piece of the flash in the EEPROM
  @param    nmrOfBytes              uint8_t: total number of bytes of the EEPROM
*/
/**************************************************************************/
void initEeprom(uint8_t nmrOfBytes){
    if (Serial){
        Serial.println("Class wifi, function: initEeprom");
    }
    EEPROM.begin(nmrOfBytes);
}

/**************************************************************************/
/*!
  @brief    Write SSID and PSW to new AP to the EEPROM
  @param    apSsid                  String: SSID name, max 32 bytes
  @param    ApPsw                   String: PSW name, max 32 bytes
  @return   1 / 2 / 3               uint8_t: 
                                    0) Booth successfull,
                                    1) failed write SSID, 
                                    2) booth failed, 
                                    3) failed write PS
*/
/**************************************************************************/
uint8_t saveApToEeprom(String apSsid, String ApPsw){
    if (Serial){
        Serial.println("Class wifi, function: saveApToEeprom");
    }
    uint8_t res = 0;
    // clear EEPROM data
    for (int i = 0; i < STANDARD_EEPROM_SIZE; i++) {
        EEPROM.write(i, 0);
    }

    // Write SSID
    if ((uint8_t) apSsid.length() < (STANDARD_EEPROM_SIZE / 2) + 1){
        for(uint8_t i = 0; i < apSsid.length(); i ++ ) {
            EEPROM.write(i, (uint8_t)apSsid[i]);
        }   
    } else{
        res = res + 1;
    }
    Serial.println("lengt apPsw:" + (STANDARD_EEPROM_SIZE / 2) + ApPsw.length());
    // Offset half of the flash size and write PSW
    if ((uint8_t) ApPsw.length() < (STANDARD_EEPROM_SIZE / 2) + 1){
        for(uint8_t i = (STANDARD_EEPROM_SIZE / 2); i < ((STANDARD_EEPROM_SIZE / 2) + ApPsw.length()); i ++ ) {
            EEPROM.write(i, (uint8_t)ApPsw[(i - (STANDARD_EEPROM_SIZE / 2))]);
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
//  TODO reconstrut uint8_t to chars
uint8_t getConfigForEeprom(uint8_t nmrOfBytes){
    uint8_t value = 0;
    if (Serial){
        Serial.println("Class wifi, function: getConfigForEeprom");
    }
    for(uint8_t i = 0; i < (STANDARD_EEPROM_SIZE / 2); i ++ ) {
        uint8_t value = EEPROM.read(i);
        Serial.print(value, DEC);
    }
    Serial.print("\n");

    for(uint8_t i = (STANDARD_EEPROM_SIZE / 2); i < STANDARD_EEPROM_SIZE; i ++ ) {
        value = EEPROM.read(i);
        if (value > 0){
            Serial.print(value, DEC);   
        }
    }  

    Serial.print("\n"); 

    return 0;
}
