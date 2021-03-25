/**************************************************************************/
/*!
    @file     UUID.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v29 - First Public Release
*/
/**************************************************************************/

#include <ESP8266TrueRandom.h>
#include <EEPROM.h>
#include <WString.h>

#ifndef _uuid_h_
#define _uuid_h_

String getUUID();
void restoreUUID();
void generateUUID();

#endif
