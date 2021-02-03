/**************************************************************************/
/*!
    @file     OTA.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v38 - Changed automatic Update to only if required Update
    v29 - First Public Release
*/
/**************************************************************************/

#include "OTA.h"

extern State state;
extern Display* display;
extern bool isResetting;

extern char firmwareType[];
extern int currentVersion; 

void tryFirmwareUpdate(String fwUpdateToken) {

  t_httpUpdate_return ret;
  ESPhttpUpdate.rebootOnUpdate(false);

  Serial.println("Trying firmware Update...");
  
  if(display!=NULL)
  {
    display->clear(true);
    display->drawProductLogo();
    display->drawString(10, 5, "Trying Firmware");
    display->drawString(20, 21, "Update...");
  }

  String updatePath = "/v1/bridge/firmware/"+fwUpdateToken+"?version="+String(currentVersion)+"&type="+firmwareType;

  Serial.println(updatePath);
  
  ret = ESPhttpUpdate.update("hub.sensate.cloud", 80, updatePath);
  switch(ret) {
    case HTTP_UPDATE_FAILED:
        if(display!=NULL)
        {
          display->clear(false); 
          display->drawProductLogo();
        }
        Serial.println("[update] Update failed.");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        if(display!=NULL)
        {
          display->clear(false); 
          display->drawProductLogo();
        }
        Serial.println("[update] Update no Update.");
        break;
    case HTTP_UPDATE_OK:
        if(display!=NULL)
        {
          display->clear(true); 
        }
        restart();
        Serial.println("[update] Update ok."); // may not called we reboot the ESP
        break;
  }

  if(state==Check_Firmware)
    state = Connected_WiFi;

}