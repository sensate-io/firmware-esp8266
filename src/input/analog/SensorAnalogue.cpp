/**************************************************************************/
/*!
    @file     SensorAnalogue.cpp
    @author   M. Fegerl (Sensate Digital Technologies GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v29 - First Public Release
*/
/**************************************************************************/

#include "SensorAnalogue.h"

extern boolean isResetting;
extern int powerMode;

SensorAnalogue::SensorAnalogue (long id, String shortName, String name, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation) {

}

SensorAnalogue::SensorAnalogue (long id, String shortName, String name, int rSplit, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation) {

_rSplit = rSplit;
 
}

void SensorAnalogue::preCycle(int cycleId)
{
}

Data* SensorAnalogue::read(bool shouldPostData)
{
  if(!isResetting)
  {
    float adc = 0;

    for (int i = 0; i < 10; i++)
    {
        adc = adc + (float) analogRead(0); // ADC = A0
    }

    adc = adc / (float)10;
    
    if(!std::isinf(adc))
    {
      if(_rSplit!=0)
      {
        double rT = ((double) adc)*_rSplit/(1024-adc);
        shouldPostData = smartSensorCheck(rT, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(_id, _name, _shortName, rT, shouldPostData);
      }
      else
      {
        shouldPostData = smartSensorCheck(adc, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(_id, _name, _shortName, adc, shouldPostData);
      }
    }
  }

  return NULL;

}

boolean SensorAnalogue::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
{
  if(powerMode == 3)
  {
    if(!shouldPostData)
    {
      if(!isnan(lastPostedValue))
      {
          if(lastPostedValue-currentRawValue>threshhold|| lastPostedValue-currentRawValue<-threshhold)
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

void SensorAnalogue::postCycle(int cycleId)
{
}