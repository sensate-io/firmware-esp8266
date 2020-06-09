/**************************************************************************/
/*!
    @file     Bridge.h
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

#include <Esp.h>
#include <EEPROM.h>

#ifndef _Bridge_h_
#define _Bridge_h_

#include "StateHelper.h"
#include "UUID.h"
#include "../communication/WiFiManager.h"
#include "../input/Sensor.h"
#include "../input/analog/SensorAnalogue.h"
#include "../input/i2c/Ads1x15.h"
#include "../input/i2c/SensorBMx280.h"
#include "../input/i2c/SensorBME680.h"
#include "../input/i2c/SensorMax44009.h"
#include "../input/i2c/SensorBH1750.h"
#include "../input/onewire/SensorDHT.h"
#include "../input/onewire/SensorDallas.h"
#include "../output/display/DisplayOLED128.h"

bool registerBridge();
void restoreBridgeConfig();
void restart();

bool getBridgeConfig();
void configureBridge(JsonObject& bridgeConfig);
void configureExpansionPort(int portNumber, JsonObject& portConfig);
void configurePort(int portNumber, JsonObject& portConfig);
void addSensor(Sensor *sensor);
void loopSensor(int currentTimeMs);
boolean postSensorData(Data* data[], int dataCount);
void checkStatus();
void trySleep(long microseconds);
void storeDisplayAndPowerConfig(boolean withPowerSettings);
void doPowerSaving();
void doPowerSavingInit(boolean doDelay);
uint8_t translateGPIOPort(String gpioPort);

#endif