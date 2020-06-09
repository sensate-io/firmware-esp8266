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
    v29 - First Public Release
*/
/**************************************************************************/

#include <Arduino.h>
#include <math.h>

#ifndef _SensorCalculation_h_
#define __SensorCalculation_h_

#include "../communication/Data.h"
#include "../output/display/DisplayOLED128.h"

class SensorCalculation {
  protected:
    int _portNumber;
  public:
    virtual Data* calculate(long, String, String, float, bool);
};

class SensorCalculationApproxQuad : public SensorCalculation  {
  private:
    double _calcValue1,_calcValue2,_calcValue3,_calcValue4;
  public:
    SensorCalculationApproxQuad(double, double, double, double, int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationPT1001000 : public SensorCalculation  {
  private:
    double _r0, a, b;
  public:
    SensorCalculationPT1001000(double, int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDualNtc : public SensorCalculation  {
  private:
    double _calcValue1,_calcValue2;
  public:
    SensorCalculationDualNtc(double, double, int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectKelvin : public SensorCalculation  {
  public:
    SensorCalculationDirectKelvin(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectCelsius : public SensorCalculation  {
  public:
    SensorCalculationDirectCelsius(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectPercent : public SensorCalculation  {
  public:
    SensorCalculationDirectPercent(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectHektoPascal : public SensorCalculation  {
  public:
    SensorCalculationDirectHektoPascal(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectMeter : public SensorCalculation  {
  public:
    SensorCalculationDirectMeter(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectLux : public SensorCalculation  {
  public:
    SensorCalculationDirectLux(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectLumen : public SensorCalculation  {
  public:
    SensorCalculationDirectLumen(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectNone : public SensorCalculation  {
  public:
    SensorCalculationDirectNone(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectOhm : public SensorCalculation  {
  public:
    SensorCalculationDirectOhm(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectKOhm : public SensorCalculation  {
  public:
    SensorCalculationDirectKOhm(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationDirectPPM : public SensorCalculation  {
  public:
    SensorCalculationDirectPPM(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationCalcAltitude : public SensorCalculation  {
  public:
    SensorCalculationCalcAltitude(int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationRawToPercent : public SensorCalculation  {
  private:
    float _calcValue1,_calcValue2;
  public:
    SensorCalculationRawToPercent(float, float, int);
    Data* calculate(long, String, String, float, bool);
};

class SensorCalculationRaw : public SensorCalculation  {
  public:
    SensorCalculationRaw(int);
    Data* calculate(long, String, String, float, bool);
};

#endif