/**************************************************************************/
/*!
    @file     SensorSI1145.h
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

#ifndef _SensorSI1145_h_
#define _SensorSI1145_h_

#include "../Sensor.h"
// #include <Adafruit_SI1145.h>
#include <SI1145_WE.h>

class SensorSI1145 : public Sensor {
  private:
    // static Adafruit_SI1145 *si1145;
    static SI1145_WE *si1145;
    static boolean failedInit;
    static int lastCycleId;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
    float calcLux(uint16_t vis, uint16_t ir);
  public:
    SensorSI1145 (long, String, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif