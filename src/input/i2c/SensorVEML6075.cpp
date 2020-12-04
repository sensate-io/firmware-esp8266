/**************************************************************************/
/*!
    @file     SensorVEML6075.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v35 - Added Support for VEML6075 and SI1145 UVI Sensors
*/
/**************************************************************************/

#include "SensorVEML6075.h"

extern boolean isResetting;
extern int powerMode;

Adafruit_VEML6075* SensorVEML6075::veml6075;

SensorVEML6075::SensorVEML6075 (long id, String category, String shortName, String name, String PortSDA, String PortSCL, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {

  int i=0;
  failedInit = false;

  if(veml6075==NULL)
    veml6075 = new Adafruit_VEML6075();

  while(!veml6075->begin())
  {
    Serial.println("Trying to find VEML6075 sensor!");
    delay(500);

    if(i==5)
    {
      Serial.println("Could not find a valid VEML6075 sensor, check wiring!");
      failedInit=true;
      break;
    }

    i++;
  }

  veml6075->setIntegrationTime(VEML6075_100MS);
  veml6075->setHighDynamic(true);
  veml6075->setForcedMode(false);
  veml6075->setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                     2.95, 1.74,  // UVB_C and UVB_D coefficients
                     0.001461, 0.002591); // UVA and UVB responses

}

void SensorVEML6075::preCycle(int cycleId)
{
}

Data* SensorVEML6075::read(bool shouldPostData)
{  
  if(!isResetting && !failedInit)
  {
    if(_calculation->getValueType()=="a")
    {
      float irradiance_a = veml6075->readUVA();
      shouldPostData = smartSensorCheck(irradiance_a, _smartValueThreshold, shouldPostData);
      return _calculation->calculate(this, irradiance_a, shouldPostData);
    }
    else if(_calculation->getValueType()=="b")
    {
      float irradiance_b = veml6075->readUVB();
      shouldPostData = smartSensorCheck(irradiance_b, _smartValueThreshold, shouldPostData);
      return _calculation->calculate(this, irradiance_b, shouldPostData);
    }
    else if(_calculation->getValueType()=="raw")
    {
      float index = veml6075->readUVI();
      shouldPostData = smartSensorCheck(index, _smartValueThreshold, shouldPostData);
      return _calculation->calculate(this, index, shouldPostData);
    }
  }
  return NULL;
}

boolean SensorVEML6075::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
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