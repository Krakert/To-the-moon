#include <wifi.h>
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
