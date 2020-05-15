/**************************************************************************/
/*!
    @file     SensorBH1750.h
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

#include <Wire.h>

#ifndef _SensorBH1750_h_
#define _SensorBH1750_h_

#include "../Sensor.h"
#include "thirdparty/BH1750.h"

class SensorBH1750 : public Sensor {
  private:
    static BH1750 *bh1750;
    String _calcType;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    void postCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorBH1750 (long, String, String, String, String, String, int, int, float, SensorCalculation*);
};

#endif