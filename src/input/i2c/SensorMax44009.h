/**************************************************************************/
/*!
    @file     SensorMax44009.h
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

#include <MAX44009.h>
#include <Wire.h>

#ifndef _SensorMax44009_h_
#define _SensorMax44009_h_

#include "../Sensor.h"

class SensorMax44009 : public Sensor {
  private:
    static MAX44009 *max44009;
    String _calcType;
    boolean failedInit;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    void postCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorMax44009 (long, String, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif