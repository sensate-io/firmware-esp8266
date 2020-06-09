/**************************************************************************/
/*!
    @file     Ads1x15.h
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

#ifndef _Ads1x15_h_
#define _Ads1x15_h_

#include "../Sensor.h"
#include "thirdparty/Adafruit_ADS1015.h"

class Ads1x15 : public Sensor {
  private:
    static boolean init48;
    static boolean init49;
    static boolean init4A;
    static boolean init4B;
    static Adafruit_ADS1015* ads1x15_48;
    static Adafruit_ADS1015* ads1x15_49;
    static Adafruit_ADS1015* ads1x15_4A;
    static Adafruit_ADS1015* ads1x15_4B;
    Adafruit_ADS1015* ads1x15;
    int _channel;
    int numberOfSamples;
    float _preResistor;
    float _postResistor;
    float lastPostedValue = NAN;
  protected:
    Data* read(bool);
    void preCycle(int);
    void postCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    Ads1x15 (long, String, String, String, String, int, int, int, int, int, float, SensorCalculation*);
};

#endif