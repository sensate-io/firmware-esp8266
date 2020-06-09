/**************************************************************************/
/*!
    @file     SensorBMx280.cpp
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

#include "SensorBMx280.h"

extern boolean isResetting;
extern int powerMode;

BME280I2C* SensorBMx280::bme = NULL;

SensorBMx280::SensorBMx280 (long id, String shortName, String name, String PortSDA, String PortSCL, String calcType, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation) {

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);
  
  if(bme == NULL)
  {
    bme = new BME280I2C();
  }
    

  int i=0;

  while(!bme->begin())
  {
    Serial.println("Trying to find BME280 sensor!");
    delay(500);

    if(i==5)
    {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      bme = NULL;
      break;
    }

    i++;
  }
  
  _calcType = calcType;

}

void SensorBMx280::preCycle(int cycleId)
{
}

Data* SensorBMx280::read(bool shouldPostData)
{  
  if(!isResetting && bme != NULL)
  {
    if(_calcType=="DIRECT_PERCENT")
    {
      float humidity = bme->hum();
      shouldPostData = smartSensorCheck(humidity, _smartValueThreshold, shouldPostData);
      return _calculation->calculate(_id, _name,  _shortName, humidity, shouldPostData);
    }
    else if(_calcType=="DIRECT_CELSIUS")
    {           
       float tempC = bme->temp();
       shouldPostData = smartSensorCheck(tempC, _smartValueThreshold, shouldPostData);
       return _calculation->calculate(_id, _name,  _shortName, tempC, shouldPostData);       
    }
    else if(_calcType=="DIRECT_HEKTOPASCAL")
    {     
       float pressure = bme->pres() / 100.0F;
       shouldPostData = smartSensorCheck(pressure, _smartValueThreshold, shouldPostData);
       return _calculation->calculate(_id, _name,  _shortName, pressure, shouldPostData);  
    }
    else if(_calcType=="CALC_METER")
    {      
        float pressure = bme->pres() / 100.0F;
        shouldPostData = smartSensorCheck(pressure, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(_id, _name,  _shortName, pressure, shouldPostData);
    }
  }
 
  return NULL;
}

boolean SensorBMx280::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
{
  if(powerMode == 3)
  {
    if(!shouldPostData)
    {
      if(!isnan(lastPostedValue))
      {
          if(lastPostedValue-currentRawValue>threshhold || lastPostedValue-currentRawValue<-threshhold)
          {
            shouldPostData = true;
          }
      }
    }

    if(shouldPostData)
      lastPostedValue = currentRawValue;
  }

  return shouldPostData;
  
}

void SensorBMx280::postCycle(int cycleId)
{
  
}