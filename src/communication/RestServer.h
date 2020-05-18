/**************************************************************************/
/*!
    @file     RestServer.h
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

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#ifndef _RestServer_h_
#define _RestServer_h_

#include "../controller/StateHelper.h"
#include "../controller/UUID.h"
#include "../output/display/DisplayOLED128.h"
#include "../communication/WiFiManager.h"

void startRestServer();
void loopRestserver();
void configRestServerRouting();
void androidCaptiveResponse();
void captiveResponse();
void tryIdentify();
void restGetUuid();
void handleNotFound();
bool captivePortal();
void handleRoot();
void returnNetworkList();
void presentWiFiSetupScreen();
void tryRestart();
void restUpdateName();
void handleAppleCaptivePortal();

String toStringIp(IPAddress ip);
bool isIp(String str);

#endif