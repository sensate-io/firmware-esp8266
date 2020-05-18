/**************************************************************************/
/*!
    @file     OTA.h
    @author   M. Fegerl (Sensate Digital Technologies GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v29 - First Public Release
*/
/**************************************************************************/

#include <ESP8266httpUpdate.h>
#include "StateHelper.h"
#include "../output/display/DisplayOLED128.h"

#ifndef _OTA_h_
#define _OTA_h_

#include "Bridge.h"

void tryFirmwareUpdate();

#endif