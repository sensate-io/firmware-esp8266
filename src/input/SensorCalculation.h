/**************************************************************************/
/*!
    @file     SensorCalculation.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v40 - New Display Structure to enable Display Rotation, different Styles etc.
    v35 - Added Support for VEML6075 and SI1145 UVI Sensors
    v34 - Added Generic Analog Sensor Support
    v33 - Added Digital Sensor Switch Support
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include <Arduino.h>
#include <math.h>

#ifndef _SensorCalculation_h_
#define _SensorCalculation_h_

#include "Sensor.h"
#include "../output/VisualisationHelper.h"

class Data;
class Sensor;

class SensorCalculation {
  protected:
    int _portNumber;
    String _valueType;
    String _valueUnit;
  public:
    String getValueType();
    String getValueUnit();
    SensorCalculation();
    virtual Data* calculate(Sensor* sensor, float, bool);
    virtual Data* calculate(Sensor* sensor, bool, bool);
};

class SensorCalculationApproxQuad : public SensorCalculation  {
  private:
    double _calcValue1,_calcValue2,_calcValue3,_calcValue4;
  public:
    SensorCalculationApproxQuad(double, double, double, double, int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationPT1001000 : public SensorCalculation  {
  private:
    double _r0, a, b;
  public:
    SensorCalculationPT1001000(double, int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDualNtc : public SensorCalculation  {
  private:
    double _calcValue1,_calcValue2;
  public:
    SensorCalculationDualNtc(double, double, int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectKelvin : public SensorCalculation  {
  public:
    SensorCalculationDirectKelvin(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectCelsius : public SensorCalculation  {
  public:
    SensorCalculationDirectCelsius(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectPercent : public SensorCalculation  {
  public:
    SensorCalculationDirectPercent(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectHektoPascal : public SensorCalculation  {
  public:
    SensorCalculationDirectHektoPascal(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectMeter : public SensorCalculation  {
  public:
    SensorCalculationDirectMeter(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectLux : public SensorCalculation  {
  public:
    SensorCalculationDirectLux(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectLumen : public SensorCalculation  {
  public:
    SensorCalculationDirectLumen(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectNone : public SensorCalculation  {
  public:
    SensorCalculationDirectNone(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectOhm : public SensorCalculation  {
  public:
    SensorCalculationDirectOhm(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectKOhm : public SensorCalculation  {
  public:
    SensorCalculationDirectKOhm(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectPPM : public SensorCalculation  {
  public:
    SensorCalculationDirectPPM(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationDirectWpm2 : public SensorCalculation  {
  public:
    SensorCalculationDirectWpm2(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationCalcAltitude : public SensorCalculation  {
  public:
    SensorCalculationCalcAltitude(int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationRawToPercent : public SensorCalculation  {
  private:
    float _calcValue1,_calcValue2;
  public:
    SensorCalculationRawToPercent(float, float, int);
    Data* calculate(Sensor* sensor, float, bool);
};

class SensorCalculationRaw : public SensorCalculation  {
  public:
    SensorCalculationRaw(int);
    SensorCalculationRaw(int, String);
    Data* calculate(Sensor* sensor, float, bool);
    Data* calculate(Sensor* sensor, bool, bool);
};

class SensorCalculationRawToVoltage : public SensorCalculation  {
  private:
    float _calcValue1,_calcValue2;
  public:
    SensorCalculationRawToVoltage(float, float, int);
    Data* calculate(Sensor* sensor, float, bool);
};


#endif