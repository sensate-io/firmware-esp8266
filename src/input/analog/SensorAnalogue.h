/**************************************************************************/
/*!
    @file     SensorAnalogue.h
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

#ifndef _SensorAnalogue_h_
#define _SensorAnalogue_h_

#include "../Sensor.h"

class SensorAnalogue : public Sensor {
  private:
    int numberOfSamples;
    int _rSplit;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorAnalogue (long, String, String, String, int, int, float, SensorCalculation*);
    SensorAnalogue (long, String, String, String, int, int, int, float, SensorCalculation*);
};

#endif