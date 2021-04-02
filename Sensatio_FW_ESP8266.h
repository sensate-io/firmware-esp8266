/**************************************************************************/
/*!
    @file     Sensatio_FW_ESP8266.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP8266 firmware is used to connect ESP8266 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v41 - Changed IDE, Sensatio
*/
/**************************************************************************/

// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Sensatio_FW_ESP8266_H_
#define _Sensatio_FW_ESP8266_H_
#include "Arduino.h"

//add your includes for the project Sensatio_FW_ESP8266 here
#include "src/controller/Bridge.h"
#include "src/output/display/Display.h"
#include "src/communication/WiFiManager.h"
#include "src/controller/OTA.h"
#include "src/communication/RestServer.h"
#include "src/output/VisualisationHelper.h"
#include <user_interface.h>
//end of add your includes here


//add your function definitions for the project Sensatio_FW_ESP8266 here

void loop();
void runTick();
void runDelayed();
void runLoop();
void initSensate();
void setup();


//Do not add code below this line
#endif /* _Sensatio_FW_ESP8266_H_ */
