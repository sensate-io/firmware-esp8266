/**************************************************************************/
/*!
    @file     SensorBME680.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include <Adafruit_BME680.h>
// #include <Wire.h>

#ifndef _SensorBME680_h_
#define _SensorBME680_h_

#include "../Sensor.h"

class SensorBME680 : public Sensor {
  private:
    static Adafruit_BME680* bme;
    static int lastReadCycleId;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorBME680 (long, String, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif