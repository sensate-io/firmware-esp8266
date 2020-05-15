/**************************************************************************/
/*!
    @file     Sensor.cpp
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

#include "Sensor.h"

extern unsigned long nextSensorDue;

Sensor::Sensor (long id, String shortName, String name, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) {
  lastTick = -1;
  lastPost = -1;
  _refreshInterval = refreshInterval;
  _postDataInterval = postDataInterval;
  _name = name;
  _shortName = shortName;
  _id = id;
  _calculation = calculation;
  _smartValueThreshold = smartValueThreshold;
}

int Sensor::getRefreshInterval() {
  return _refreshInterval;
}

int Sensor::getPostDataInterval() {
  return _postDataInterval;
}

String Sensor::getShortName() {
  return _shortName;
}

String Sensor::getName() {
  return _name;
}

long Sensor::getId() {
  return _id;
}

void Sensor::preCycle(int cycleId)
{
  Serial.println("Base Sensor Pre Cycle");
}

Data* Sensor::read(bool shouldPostData) {
  Serial.println("Base Sensor Read: " + getName());
}

void Sensor::postCycle(int cycleId)
{
  Serial.println("Base Sensor Post Cycle");
}

Data* Sensor::trySensorRead(unsigned long currentTimeMillis, int cycleId) {

  if (lastTick==-1 || currentTimeMillis < lastTick)
      return forceSensorRead(currentTimeMillis, cycleId);
  else
  {
    if (lastTick + _refreshInterval <= currentTimeMillis)
      return forceSensorRead(currentTimeMillis, cycleId);      
  }
  return NULL;
}

Data* Sensor::forceSensorRead(unsigned long currentTimeMillis, int cycleId) {

  bool shouldPostData = false;
  if(lastPost==-1 || (lastPost + _postDataInterval <= currentTimeMillis))
  {
    shouldPostData = true;
    lastPost = currentTimeMillis;
  }

  preCycle(cycleId);
  Data *d =  read(shouldPostData);
  lastTick = currentTimeMillis;
  postCycle(cycleId);

  if(nextSensorDue==-1 || nextSensorDue < currentTimeMillis || (nextSensorDue > currentTimeMillis+_postDataInterval))
  {
    nextSensorDue = currentTimeMillis+_postDataInterval;
  }
    

  return d;

}