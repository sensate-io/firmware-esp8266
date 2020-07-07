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
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include "SensorCalculation.h"

extern Display* display;

SensorCalculation::SensorCalculation()
{

}

String SensorCalculation::getValueType()
{
  return _valueType;
}

String SensorCalculation::getValueUnit()
{
  return _valueUnit;
}

SensorCalculationApproxQuad::SensorCalculationApproxQuad(double calcValue1, double calcValue2, double calcValue3, double calcValue4, int portNumber) : SensorCalculation()
{
  _valueType = "temperature";
  _valueUnit = "°C";

  _portNumber = portNumber;
  _calcValue1 = calcValue1;
  _calcValue2 = calcValue2;
  _calcValue3 = calcValue3;
  _calcValue4 = calcValue4;
}

SensorCalculationPT1001000::SensorCalculationPT1001000(double r0, int portNumber) : SensorCalculation()
{
  _valueType = "temperature";
  _valueUnit = "°C";

  a = 0.0039083;
  b = -0.0000005775;
  _portNumber = portNumber;
  _r0 = r0;
}

SensorCalculationDualNtc::SensorCalculationDualNtc(double calcValue1, double calcValue2, int portNumber) : SensorCalculation()
{
  _valueType = "temperature";
  _valueUnit = "°C";

  _portNumber = portNumber;
  _calcValue1 = calcValue1;
  _calcValue2 = calcValue2;
}

SensorCalculationDirectKelvin::SensorCalculationDirectKelvin(int portNumber) : SensorCalculation()
{
  _valueType = "temperature";
  _valueUnit = "K";

  _portNumber = portNumber;
}

SensorCalculationDirectCelsius::SensorCalculationDirectCelsius(int portNumber) : SensorCalculation()
{
  _valueType = "temperature";
  _valueUnit = "°C";
  _portNumber = portNumber;
}

SensorCalculationDirectPercent::SensorCalculationDirectPercent(int portNumber) : SensorCalculation()
{
  _valueType = "humidity";
  _valueUnit = "%";
  _portNumber = portNumber;
}

SensorCalculationDirectHektoPascal::SensorCalculationDirectHektoPascal(int portNumber) : SensorCalculation()
{
  _valueType = "pressure";
  _valueUnit = "hPa";
  _portNumber = portNumber;
}

SensorCalculationDirectMeter::SensorCalculationDirectMeter(int portNumber) : SensorCalculation()
{
  _valueType = "";
  _valueUnit = "m";
  _portNumber = portNumber;
}

SensorCalculationDirectLux::SensorCalculationDirectLux(int portNumber) : SensorCalculation()
{
  _valueType = "illuminance";
  _valueUnit = "lx";
  _portNumber = portNumber;
}

SensorCalculationDirectLumen::SensorCalculationDirectLumen(int portNumber) : SensorCalculation()
{
  _valueType = "flux";
  _valueUnit = "lm";
  _portNumber = portNumber;
}

SensorCalculationDirectOhm::SensorCalculationDirectOhm(int portNumber) : SensorCalculation()
{
  _valueType = "resistance";
  _valueUnit = "Ohm";
  _portNumber = portNumber;
}

SensorCalculationDirectKOhm::SensorCalculationDirectKOhm(int portNumber) : SensorCalculation()
{
  _valueType = "resistance";
  _valueUnit = "kOhm";
  _portNumber = portNumber;
}

SensorCalculationDirectPPM::SensorCalculationDirectPPM(int portNumber) : SensorCalculation()
{
  _valueType = "concentration";
  _valueUnit = "ppm";
  _portNumber = portNumber;
}

SensorCalculationDirectNone::SensorCalculationDirectNone(int portNumber) : SensorCalculation()
{
  _valueType = "unknown";
  _valueUnit = "";
  _portNumber = portNumber;
}

SensorCalculationCalcAltitude::SensorCalculationCalcAltitude(int portNumber) : SensorCalculation()
{
  _valueType = "altitude";
  _valueUnit = "m";
  _portNumber = portNumber;
}

SensorCalculationRawToPercent::SensorCalculationRawToPercent(float calcValue1, float calcValue2, int portNumber) : SensorCalculation()
{
  _valueType = "humidity";
  _valueUnit = "%";
  _portNumber = portNumber;
  _calcValue1 = calcValue1;
  _calcValue2 = calcValue2;
}

SensorCalculationRaw::SensorCalculationRaw(int portNumber) : SensorCalculation()
{
  _valueType = "raw";
  _valueUnit = "(raw))";
  _portNumber = portNumber;
}

Data* SensorCalculation::calculate(Sensor* sensor, float rawValue, bool postData)
{
  return NULL;
}

Data* SensorCalculationApproxQuad::calculate(Sensor* sensor, float rawValue, bool postData)
{
  double v = log( (double)rawValue / _calcValue4);
  double z = _calcValue1 + (_calcValue2 * v) + (_calcValue3 * v * v);
  float tempInC = (float) ((float) (1 / z) - 273.15);

  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), tempInC, _valueUnit);
  
  if(!postData)
    return NULL;
  return new Data (sensor, tempInC, "CELSIUS");
}

Data* SensorCalculationPT1001000::calculate(Sensor* sensor, float rT, bool postData)
{
  double v2 = _r0*_r0*a*a;
  double v3 = 4*_r0*b*(_r0-rT);
  double v4 = sqrt(v2-v3);

  double v5 = 2*_r0*b;
  float tempInC = (float)(-_r0*a+v4)/v5;

  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), tempInC, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, tempInC, "CELSIUS");
}

Data* SensorCalculationDualNtc::calculate(Sensor* sensor, float rawValue, bool postData)
{
  double x = 0.003354016;
  double y = log(rawValue / _calcValue2);
  double z = y / _calcValue1;
  float tempInC = (float) ((1.0 / (z + x))- 273.15);

  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), tempInC, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, tempInC, "CELSIUS");
}

Data* SensorCalculationDirectKelvin::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "KELVIN");
}

Data* SensorCalculationDirectCelsius::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "CELSIUS");
}

Data* SensorCalculationDirectPercent::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "PERCENT");
}

Data* SensorCalculationDirectHektoPascal::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "HEKTOPASCAL");
}

Data* SensorCalculationDirectMeter::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "METER");
}

Data* SensorCalculationDirectLux::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "LUX");
}

Data* SensorCalculationDirectLumen::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "LUMEN");
}

Data* SensorCalculationDirectOhm::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "OHM");
}

Data* SensorCalculationDirectKOhm::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "KOHM");
}

Data* SensorCalculationDirectPPM::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "PPM");
}

Data* SensorCalculationDirectNone::calculate(Sensor* sensor, float rawValue, bool postData)
{
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "NONE");
}

Data* SensorCalculationCalcAltitude::calculate(Sensor* sensor, float rawValue, bool postData)
{
  float altitude = 44330.0 * (1.0 - pow(rawValue / 1013.25, 0.1903));
  
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), altitude, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, altitude, "METER");
}

Data* SensorCalculationRawToPercent::calculate(Sensor* sensor, float rawValue, bool postData)
{
  float min = _calcValue1;
  float max = _calcValue2;

  float rawDiff = rawValue - min;
  float percent = rawDiff * 100 / (max - min);

  if(percent<0) percent = 0.0;
  if(percent>100) percent = 100.0;
  
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), percent, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, percent, "PERCENT");
}

Data* SensorCalculationRaw::calculate(Sensor* sensor, float rawValue, bool postData)
{  
  if(display!=NULL && _portNumber>=0)
    display->drawValue(_portNumber, sensor->getName(), sensor->getShortName(), rawValue, _valueUnit);
  if(!postData)
    return NULL;
  return new Data (sensor, rawValue, "UNKNOWN");
}