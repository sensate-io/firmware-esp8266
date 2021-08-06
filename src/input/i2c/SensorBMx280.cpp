/**************************************************************************/
/*!
    @file     SensorBMx280.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP8266 firmware is used to connect ESP8266 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v45 - Fixed Pressure Measurement for BME280 Sensors
    v33 - Changes for Digital Sensor Switch Support
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include "SensorBMx280.h"
#include "Wire.h"

extern boolean isResetting;
extern int powerMode;

BME280I2C* SensorBMx280::bme76 = NULL;
BME280I2C* SensorBMx280::bme77 = NULL;

SensorBMx280::SensorBMx280 (long id, String category, String shortName, String name, String i2cAdress, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);
  
  if(i2cAdress=="0x77")
  {
    if(bme77 == NULL)
    {
    	BME280I2C::Settings settings(
			   BME280::OSR_X1,
			   BME280::OSR_X1,
			   BME280::OSR_X1,
			   BME280::Mode_Forced,
			   BME280::StandbyTime_1000ms,
			   BME280::Filter_16,
			   BME280::SpiEnable_False,
			   BME280I2C::I2CAddr_0x77
		  );
      bme77 = new BME280I2C(settings);
    }
    bme = bme77;
  }
  else
  {
    if(bme76 == NULL)
    {
      bme76 = new BME280I2C();
    }
    bme = bme76;
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
}

void SensorBMx280::preCycle(int cycleId)
{
}

Data* SensorBMx280::read(bool shouldPostData)
{  
  if(!isResetting && bme != NULL)
  {
    if(_calculation->getValueType()=="humidity")
    {
      float humidity = bme->hum();
      shouldPostData = smartSensorCheck(humidity, _smartValueThreshold, shouldPostData);
      return _calculation->calculate(this, humidity, shouldPostData);
    }
    else if(_calculation->getValueType()=="temperature")
    {          
       float tempC = bme->temp();
       shouldPostData = smartSensorCheck(tempC, _smartValueThreshold, shouldPostData);
       return _calculation->calculate(this, tempC, shouldPostData);       
    }
    else if(_calculation->getValueType()=="pressure")
    {    
       float pressure = bme->pres(BME280::PresUnit_hPa);
       shouldPostData = smartSensorCheck(pressure, _smartValueThreshold, shouldPostData);
       return _calculation->calculate(this, pressure, shouldPostData);  
    }
    else if(_calculation->getValueType()=="altitude")
    {     
        float pressure = bme->pres(BME280::PresUnit_hPa);
        shouldPostData = smartSensorCheck(pressure, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, pressure, shouldPostData);
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
