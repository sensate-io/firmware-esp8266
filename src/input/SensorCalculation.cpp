/**************************************************************************/
/*!
    @file     SensorCalculation.cpp
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

#include "SensorCalculation.h"

extern Display* display;

#define SEALEVELPRESSURE_HPA (1013.25)

SensorCalculationApproxQuad::SensorCalculationApproxQuad(double calcValue1, double calcValue2, double calcValue3, double calcValue4, int portNumber)
{
  _portNumber = portNumber;
  _calcValue1 = calcValue1;
  _calcValue2 = calcValue2;
  _calcValue3 = calcValue3;
  _calcValue4 = calcValue4;
}

SensorCalculationPT1001000::SensorCalculationPT1001000(double r0, int portNumber)
{
  a = 0.0039083;
  b = -0.0000005775;
  _portNumber = portNumber;
  _r0 = r0;
}

SensorCalculationDualNtc::SensorCalculationDualNtc(double calcValue1, double calcValue2, int portNumber)
{
  _portNumber = portNumber;
  _calcValue1 = calcValue1;
  _calcValue2 = calcValue2;
}

SensorCalculationDirectKelvin::SensorCalculationDirectKelvin(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectCelsius::SensorCalculationDirectCelsius(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectPercent::SensorCalculationDirectPercent(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectHektoPascal::SensorCalculationDirectHektoPascal(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectMeter::SensorCalculationDirectMeter(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectLux::SensorCalculationDirectLux(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectLumen::SensorCalculationDirectLumen(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectOhm::SensorCalculationDirectOhm(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectKOhm::SensorCalculationDirectKOhm(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectPPM::SensorCalculationDirectPPM(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationDirectNone::SensorCalculationDirectNone(int portNumber)
{
  _portNumber = portNumber;
}


SensorCalculationCalcAltitude::SensorCalculationCalcAltitude(int portNumber)
{
  _portNumber = portNumber;
}

SensorCalculationRawToPercent::SensorCalculationRawToPercent(float calcValue1, float calcValue2, int portNumber)
{
  _portNumber = portNumber;
  _calcValue1 = calcValue1;
  _calcValue2 = calcValue2;
}

SensorCalculationRaw::SensorCalculationRaw(int portNumber)
{
  _portNumber = portNumber;
}


Data* SensorCalculationApproxQuad::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  double v = log( (double)rawValue / _calcValue4);
  double z = _calcValue1 + (_calcValue2 * v) + (_calcValue3 * v * v);
  float tempInK = (float) (1 / z);

  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, tempInK, "K");
  
  if(!postData)
    return NULL;
  return new Data (id, tempInK, "KELVIN");
}

Data* SensorCalculationPT1001000::calculate(long id, String name, String shortName, float rT, bool postData)
{
  double v2 = _r0*_r0*a*a;
  double v3 = 4*_r0*b*(_r0-rT);
  double v4 = sqrt(v2-v3);

  double v5 = 2*_r0*b;
  double tempInC = (-_r0*a+v4)/v5;
  float tempInK = (float) (tempInC + 273.15);

  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, tempInC, "°C");
  if(!postData)
    return NULL;
  return new Data (id, tempInK, "KELVIN");
}

Data* SensorCalculationDualNtc::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  double x = 0.003354016;
  double y = log(rawValue / _calcValue2);
  double z = y / _calcValue1;
  float tempInK = (float) (1.0 / (z + x));

  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, tempInK, "K");
  if(!postData)
    return NULL;
  return new Data (id, tempInK, "KELVIN");
}

Data* SensorCalculationDirectKelvin::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "K");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "KELVIN");
}

Data* SensorCalculationDirectCelsius::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "°C");
  float tempInK = rawValue+273.15;
  if(!postData)
    return NULL;
  return new Data (id, tempInK, "KELVIN");
}

Data* SensorCalculationDirectPercent::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "%");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "PERCENT");
}

Data* SensorCalculationDirectHektoPascal::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "hPa");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "HEKTOPASCAL");
}

Data* SensorCalculationDirectMeter::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "m");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "METER");
}

Data* SensorCalculationDirectLux::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "lx");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "LUX");
}

Data* SensorCalculationDirectLumen::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "lm");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "LUMEN");
}

Data* SensorCalculationDirectOhm::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "Ohm");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "OHM");
}

Data* SensorCalculationDirectKOhm::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "kOhm");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "KOHM");
}

Data* SensorCalculationDirectPPM::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "ppm");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "PPM");
}

Data* SensorCalculationDirectNone::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "NONE");
}

Data* SensorCalculationCalcAltitude::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  float altitude = 44330.0 * (1.0 - pow(rawValue / SEALEVELPRESSURE_HPA, 0.1903));
  
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, altitude, "m");
  if(!postData)
    return NULL;
  return new Data (id, altitude, "METER");
}

Data* SensorCalculationRawToPercent::calculate(long id, String name, String shortName, float rawValue, bool postData)
{
  float min = _calcValue1;
  float max = _calcValue2;

  float rawDiff = rawValue - min;
  float percent = rawDiff * 100 / (max - min);

  if(percent<0) percent = 0.0;
  if(percent>100) percent = 100.0;
  
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, percent, "%");
  if(!postData)
    return NULL;
  return new Data (id, percent, "PERCENT");
}

Data* SensorCalculationRaw::calculate(long id, String name, String shortName, float rawValue, bool postData)
{  
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, name, shortName, rawValue, "(raw)");
  if(!postData)
    return NULL;
  return new Data (id, rawValue, "UNKNOWN");
}