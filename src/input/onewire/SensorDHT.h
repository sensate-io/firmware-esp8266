/**************************************************************************/
/*!
    @file     SensorDHT.h
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

#include <DHT.h>
#include <DHT_U.h>

#ifndef _SensorDHT_h_
#define _SensorDHT_h_

#include "../Sensor.h"

class SensorDHT : public Sensor {
  private:
    static boolean initDHT0;
    static boolean initDHT1;
    static boolean initDHT2;
    static boolean initDHT3;
    static boolean initDHT4;
    static boolean initDHT5;
    static boolean initDHT6;
    static boolean initDHT7;
    static boolean initDHT8;
    static boolean initDHT9;
    static boolean initDHT10;
    static boolean initDHT11;
    static boolean initDHT12;
    static boolean initDHT13;
    static boolean initDHT14;
    static boolean initDHT15;
    static boolean initDHT16;
    static DHT_Unified* dht0;
    static DHT_Unified* dht1;
    static DHT_Unified* dht2;
    static DHT_Unified* dht3;
    static DHT_Unified* dht4;
    static DHT_Unified* dht5;
    static DHT_Unified* dht6;
    static DHT_Unified* dht7;
    static DHT_Unified* dht8;
    static DHT_Unified* dht9;
    static DHT_Unified* dht10;
    static DHT_Unified* dht11;
    static DHT_Unified* dht12;
    static DHT_Unified* dht13;
    static DHT_Unified* dht14;
    static DHT_Unified* dht15;
    static DHT_Unified* dht16;
    DHT_Unified* dht;
    float lastPostedValue = NAN;
    static void portFix(uint8_t);
  protected:
    Data* read(bool);
    void preCycle(int);
    boolean smartSensorCheck(float, float, boolean);
  public:
    SensorDHT (long, String, String, String,String, uint8_t, int, int, float, SensorCalculation*);
};

#endif