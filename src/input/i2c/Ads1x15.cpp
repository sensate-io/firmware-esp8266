/**************************************************************************/
/*!
    @file     Ads1x15.cpp
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

#include "Ads1x15.h"

boolean Ads1x15::init48 = false;
boolean Ads1x15::init49 = false;
boolean Ads1x15::init4A = false;
boolean Ads1x15::init4B = false;
Adafruit_ADS1015* Ads1x15::ads1x15_48 = NULL;
Adafruit_ADS1015* Ads1x15::ads1x15_49 = NULL;
Adafruit_ADS1015* Ads1x15::ads1x15_4A = NULL;
Adafruit_ADS1015* Ads1x15::ads1x15_4B = NULL;

extern int powerMode;

Ads1x15::Ads1x15 (long id, String category, String shortName, String name, String type, String addressString, int channel, int preResistor, int postResistor, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation) {

  _channel = channel;
  numberOfSamples = 10;
  _preResistor = (float) preResistor;
  _postResistor = (float) postResistor;

  if (addressString == NULL | addressString == "" || addressString == "0x48") {
    if (!init48)
    {
      init48 = true;
      ads1x15_48 = new Adafruit_ADS1015(0x48);
      ads1x15_48->setGain(GAIN_ONE);
      ads1x15_48->begin();
    }
    ads1x15 = ads1x15_48;
  }
  else if (addressString == "0x49") {
    if (!init49)
    {
      init49 = true;
      ads1x15_49 = new Adafruit_ADS1015(0x49);
      ads1x15_49->setGain(GAIN_ONE);
      ads1x15_49->begin();
    }
    ads1x15 = ads1x15_49;
  }

  else if (addressString == "0x4A") {
    if (!init4A)
    {
      init4A = true;
      ads1x15_4A = new Adafruit_ADS1015(0x4A);
      ads1x15_4A->setGain(GAIN_ONE);
      ads1x15_4A->begin();
    }
    ads1x15 = ads1x15_4A;
  }
  else if (addressString == "0x4B") {
    if (!init4B)
    {
      init4B = true;
      ads1x15_4B = new Adafruit_ADS1015(0x4B);
      ads1x15_4B->setGain(GAIN_ONE);
      ads1x15_4B->begin();
    }
    ads1x15 = ads1x15_4B;
  }

}

void Ads1x15::preCycle(int cycleId)
{
}

Data* Ads1x15::read(bool shouldPostData)
{
  float adcMax = 0;
  float adc = 0;

  for (int i = 0; i < numberOfSamples; i++)
  {
    adcMax = adcMax + (float) ads1x15->readADC_SingleEnded(0);
    
    if (_channel == 1)
      adc = adc + (float) ads1x15->readADC_SingleEnded(1);
    if (_channel == 2)
      adc = adc + (float) ads1x15->readADC_SingleEnded(2);
    if (_channel == 3)
      adc = adc + (float) ads1x15->readADC_SingleEnded(3);
  }

  adcMax = adcMax / (float)numberOfSamples;
  adc = adc / (float)numberOfSamples;

  if (adc >= adcMax-20)
    adc = adcMax;

  if(_preResistor!=0 && _postResistor!=0)
  {
    adc = adc/adcMax*1024;
    shouldPostData = smartSensorCheck(adc, _smartValueThreshold, shouldPostData);
    return _calculation->calculate(this, adc, shouldPostData);
  }
  else
  {
    float res = NULL;

    if(_preResistor>0)
      res = _preResistor / (adcMax / adc - 1.0);
    else if(_postResistor>0)
      res = _postResistor * (adcMax / adc - 1.0);

    if(res!=NULL && !std::isinf(res))
    {
      // Serial.printf("Corrected res: %f\n\r",res);
      shouldPostData = smartSensorCheck(res, _smartValueThreshold, shouldPostData);
      return _calculation->calculate(this, res, shouldPostData);
    }
  }

  return NULL;

}

boolean Ads1x15::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
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