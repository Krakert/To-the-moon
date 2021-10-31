#include <wifiHandler.h>
#include <LittleFS.h>
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
  @brief    Functions that return 404, when navigate to a unknown page
*/
/**************************************************************************/
void handleNotFound(AsyncWebServerRequest *request){
    if (Serial){
        Serial.println("function: handleNotFound");
    }
    request->send(404, "text/plain", "404: Not found");
}

/**************************************************************************/
/*!
  @brief    Show the home page, when in STA mode 
*/
/**************************************************************************/
void handleOnHomeSTA(AsyncWebServerRequest *request){
    if (Serial){
        Serial.println("function: handleOnHomeSTA");
    }
    request->send(LittleFS, "/serverMain.html", String());
}

void handleOnHomeSTACSS(AsyncWebServerRequest *request){
      if (Serial){
        Serial.println("function: handleOnHomeSTACSS");
    }
    request->send(LittleFS, "/server.css", "text/css");
}
 
/**************************************************************************/
/*!
  @brief    Show the home page, when in AP mode 
*/
/**************************************************************************/
void handleOnHomeAP(AsyncWebServerRequest *request){
    if (Serial){
        Serial.println("function: handleOnHomeAP");
    }
    request->send_P(200, "text/html", INDEX_HTML_AP);
}

/**************************************************************************/
/*!
  @brief    Functions saves the parameter of the webserver to the EEPROM 
*/
/**************************************************************************/
void handleGetDataAP(AsyncWebServerRequest *request){
    if (Serial){
        Serial.println("function: handleGetDataAP");
      }
      String ssidToSave, pswToSave;
      if (request->hasParam("ssidFromAp")) {
        ssidToSave = request->getParam("ssidFromAp")->value();
      } if (request->hasParam("passwordFromAp")) {
        pswToSave = request->getParam("passwordFromAp")->value();
      }
      if (saveApToEeprom(ssidToSave, pswToSave, true) == 0){
        request->send(200, "text/html", INDEX_HTML_AP_SUCCES);
      } else {
          if (Serial){
                Serial.println("Something went wrong");
          }
        request->send(200, "text/plain", "something went wrong, please restart and retry");
      }
}