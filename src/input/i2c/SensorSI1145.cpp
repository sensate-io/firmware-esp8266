/**************************************************************************/
/*!
    @file     SensorSI1145.cpp
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

#include "SensorSI1145.h"

extern boolean isResetting;
extern int powerMode;

// Adafruit_SI1145* SensorSI1145::si1145;
SI1145_WE* SensorSI1145::si1145;
int SensorSI1145::lastCycleId = -1;
boolean SensorSI1145::failedInit = false;

SensorSI1145::SensorSI1145 (long id, String category, String shortName, String name, String PortSDA, String PortSCL, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {

  if(si1145==NULL)
    si1145 = new SI1145_WE();

  si1145->init();

  si1145->enableHighSignalVisRange(); // Gain divided by 14.5
  si1145->enableHighSignalIrRange(); // Gain divided by 14.5
  si1145->enableMeasurements(PSALSUV_TYPE, AUTO);
}

void SensorSI1145::preCycle(int cycleId)
{
  if(cycleId!=lastCycleId)
  {
    if(failedInit)
    {
      Serial.println("Trying to re-init SI1145...");
      si1145->init();
      si1145->enableHighSignalVisRange(); // Gain divided by 14.5
      si1145->enableHighSignalIrRange(); // Gain divided by 14.5
      si1145->enableMeasurements(PSALSUV_TYPE, AUTO);
      failedInit=false;
    }
    lastCycleId=cycleId;
  }
  
}

Data* SensorSI1145::read(bool shouldPostData)
{  
  if(!isResetting && !failedInit)
  {
    if(_calculation->getValueType()=="a")
    {
      float visLight = si1145->getAlsVisData();

      if (visLight==65535.0) {
          Serial.println("NAN VisLight!");
          failedInit=true;
      }
      else {
          shouldPostData = smartSensorCheck(visLight, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, visLight, shouldPostData);
      }
    }
    else if(_calculation->getValueType()=="b")
    {
      float irLight = si1145->getAlsIrData();
      
      if (irLight==65535.0f) {
          Serial.println("NAN IrLight!");
          failedInit=true;
      }
      else {
         shouldPostData = smartSensorCheck(irLight, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, irLight, shouldPostData);
      }
    }
    else if(_calculation->getValueType()=="c")
    {
      float prox = si1145->getPsData();
       
      if (prox==65535.0f) {
          Serial.println("NAN Proximiy!");
          failedInit=true;
      }
      else {
        shouldPostData = smartSensorCheck(prox, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, prox, shouldPostData);
      }
    }
    else if(_calculation->getValueType()=="raw")
    {
      float index = si1145->getUvIndex();
      
      if (index==655.35f) {
          Serial.println("NAN UV-Index!");
          failedInit=true;
      }
      else {
        shouldPostData = smartSensorCheck(index, _smartValueThreshold, shouldPostData);
        return _calculation->calculate(this, index, shouldPostData);
      }
    }
  }
  return NULL;
}

boolean SensorSI1145::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
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