/**************************************************************************/
/*!
    @file     SensorDigitalSwitch.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v33 - Added Digital Sensor Switch
*/
/**************************************************************************/

#include "SensorDigitalSwitch.h"

extern boolean isResetting;
extern int powerMode;

SensorDigitalSwitch::SensorDigitalSwitch (long id, String category, String shortName, String name, uint8_t port, int refreshInterval, int postDataInterval, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, 0.5, calculation, true) {

    pinMode(port, INPUT);
    _port = port;

}

void SensorDigitalSwitch::preCycle(int cycleId)
{
}

Data* SensorDigitalSwitch::read(bool shouldPostData)
{
  if(!isResetting)
  {
    bool portState = digitalRead(_port);  
    if(lastPostedValue!=portState)
        shouldPostData = true;
    
    Data *data = _calculation->calculate(this, portState, shouldPostData);
    
    if(shouldPostData)
        lastPostedValue = portState;
    
    return data;
  }

  return NULL;

}

boolean SensorDigitalSwitch::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
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