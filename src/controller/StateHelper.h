/**************************************************************************/
/*!
    @file     StateHelper.h
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

#include <EEPROM.h>

#ifndef _StateHelper_h_
#define _StateHelper_h_

typedef enum {
    Boot,    // First State, do everything thats required to start
    Connect_WiFi, // Try to connect to a specified Wireless Network
    Check_Firmware, // On Connection Success, try to get Firmware Update
    Connected_WiFi, // Connection to WiFi Successful and Firmware Up to Date
    Register, // Register Bridge at the Cloud Service
    Init_Configuration, // Request the Bridge Configuration from the Sensate Cloud and initialize the received Configuration
    Operating,  // Normal State after Setup
    Init_AP,   // If connection cannot be established, enter Setup AP Mode
    Init_Setup, // Start everything needed for Setup Mode
    Setup   // Operating in Setup Mode
} State;

bool isWiFiConfigured();

#endif
