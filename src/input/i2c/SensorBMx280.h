/**************************************************************************/
/*!
    @file     SensorBMx280.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v45 - Fixed Pressure Measurement for BME280 Sensors
    v33 - Changes for Digital Sensor Switch Support
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include <BME280I2C.h>
// #include <Wire.h>

#ifndef _SensorBMx280_h_
#define _SensorBMx280_h_

#include "../Sensor.h"

class SensorBMx280 : public Sensor {
  private:
    static BME280I2C* bme76;
    static BME280I2C* bme77;
    BME280I2C* bme;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorBMx280 (long, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif
