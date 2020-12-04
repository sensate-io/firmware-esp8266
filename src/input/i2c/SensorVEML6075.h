/**************************************************************************/
/*!
    @file     SensorVEML6075.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v35 - Added Support for VEML6075 and SI1145 UVI Sensors
*/
/**************************************************************************/

#ifndef _SensorVEML6075_h_
#define _SensorVEML6075_h_

#include "../Sensor.h"
#include <Adafruit_VEML6075.h>

class SensorVEML6075 : public Sensor {
  private:
    static Adafruit_VEML6075 *veml6075;
    boolean failedInit;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorVEML6075 (long, String, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif