/**************************************************************************/
/*!
    @file     SensorBH1750.cpp
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

#include "SensorBH1750.h"

extern boolean isResetting;
extern int powerMode;

BH1750* SensorBH1750::bh1750;

SensorBH1750::SensorBH1750 (long id, String shortName, String name, String PortSDA, String PortSCL, String calcType, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation) {

  if(bh1750==NULL)
  {
    bh1750 = new BH1750();

    bh1750->begin();
  }
  
  _calcType = calcType;

}

void SensorBH1750::preCycle(int cycleId)
{
}

Data* SensorBH1750::read(bool shouldPostData)
{  
  if(!isResetting)
  {
    if(_calcType=="DIRECT_LUX")
    {
      float illuminance = bh1750->readLightLevel();

      if(illuminance>=0)
      {
        shouldPostData = smartSensorCheck(illuminance, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(_id, _name,  _shortName, illuminance, shouldPostData);
      }
      else
        Serial.println("NAN Illuminance!");
    }
  }
 
  return NULL;
}

boolean SensorBH1750::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
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

void SensorBH1750::postCycle(int cycleId)
{
  
}