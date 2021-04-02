/**************************************************************************/
/*!
    @file     WiFiManager.h
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

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include "Hash.h"

#ifndef _WiFiManager_h_
#define _WiFiManager_h_

#include "../output/display/Display.h"
#include "../controller/StateHelper.h"
#include "../controller/Bridge.h"

void connectToNetwork();
void connectToAP();
bool startUpLocalAP();
void setupBridgeConfig();
void resetBridgeConfig();
void setupBridgeConfigManual();
void shutDownWiFi();
void startDNS();
void loopDNS();
void checkWiFiStatus();

#endif
