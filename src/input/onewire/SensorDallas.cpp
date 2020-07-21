/**************************************************************************/
/*!
    @file     SensorDallas.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v33 - Changes for Digital Sensor Switch Support
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include "SensorDallas.h"

boolean SensorDallas::initDallas0 = false;
boolean SensorDallas::initDallas1 = false;
boolean SensorDallas::initDallas2 = false;
boolean SensorDallas::initDallas3 = false;
boolean SensorDallas::initDallas4 = false;
boolean SensorDallas::initDallas5 = false;
boolean SensorDallas::initDallas6 = false;
boolean SensorDallas::initDallas7 = false;
boolean SensorDallas::initDallas8 = false;
boolean SensorDallas::initDallas9 = false;
boolean SensorDallas::initDallas10 = false;
boolean SensorDallas::initDallas11 = false;
boolean SensorDallas::initDallas12 = false;
boolean SensorDallas::initDallas13 = false;
boolean SensorDallas::initDallas14 = false;
boolean SensorDallas::initDallas15 = false;
boolean SensorDallas::initDallas16 = false;
OneWire* SensorDallas::oneWire0  = NULL;
OneWire* SensorDallas::oneWire1  = NULL;
OneWire* SensorDallas::oneWire2  = NULL;
OneWire* SensorDallas::oneWire3  = NULL;
OneWire* SensorDallas::oneWire4  = NULL;
OneWire* SensorDallas::oneWire5  = NULL;
OneWire* SensorDallas::oneWire6  = NULL;
OneWire* SensorDallas::oneWire7  = NULL;
OneWire* SensorDallas::oneWire8  = NULL;
OneWire* SensorDallas::oneWire9  = NULL;
OneWire* SensorDallas::oneWire10  = NULL;
OneWire* SensorDallas::oneWire11  = NULL;
OneWire* SensorDallas::oneWire12  = NULL;
OneWire* SensorDallas::oneWire13  = NULL;
OneWire* SensorDallas::oneWire14  = NULL;
OneWire* SensorDallas::oneWire15  = NULL;
OneWire* SensorDallas::oneWire16  = NULL;
DallasTemperature* SensorDallas::dallasTemperature0 = NULL;
DallasTemperature* SensorDallas::dallasTemperature1 = NULL;
DallasTemperature* SensorDallas::dallasTemperature2 = NULL;
DallasTemperature* SensorDallas::dallasTemperature3 = NULL;
DallasTemperature* SensorDallas::dallasTemperature4 = NULL;
DallasTemperature* SensorDallas::dallasTemperature5 = NULL;
DallasTemperature* SensorDallas::dallasTemperature6 = NULL;
DallasTemperature* SensorDallas::dallasTemperature7 = NULL;
DallasTemperature* SensorDallas::dallasTemperature8 = NULL;
DallasTemperature* SensorDallas::dallasTemperature9 = NULL;
DallasTemperature* SensorDallas::dallasTemperature10 = NULL;
DallasTemperature* SensorDallas::dallasTemperature11 = NULL;
DallasTemperature* SensorDallas::dallasTemperature12 = NULL;
DallasTemperature* SensorDallas::dallasTemperature13 = NULL;
DallasTemperature* SensorDallas::dallasTemperature14 = NULL;
DallasTemperature* SensorDallas::dallasTemperature15 = NULL;
DallasTemperature* SensorDallas::dallasTemperature16 = NULL;

extern boolean isResetting;
extern int powerMode;

SensorDallas::SensorDallas (long id, String category, String shortName, String name, uint8_t port, int channel, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation, false) {

  _channel = channel;

  if(port==0)
  {
    if(!initDallas0)
    {
      initDallas0=true;
      oneWire0 = new OneWire(0);
      dallasTemperature0 = new DallasTemperature(oneWire0);
      dallasTemperature0->begin();
    }
    dallasTemperature = dallasTemperature0;
  }
  else if(port==1)
  {
    if(!initDallas1)
    {
      initDallas1=true;
      oneWire1 = new OneWire(1);
      dallasTemperature1 = new DallasTemperature(oneWire1);
      dallasTemperature1->begin();
    }
    dallasTemperature = dallasTemperature1;
  }
  else if(port==2)
  {
    if(!initDallas2)
    {
      initDallas2=true;
      oneWire2 = new OneWire(2);
      dallasTemperature2 = new DallasTemperature(oneWire2);
      dallasTemperature2->begin();
    }
    dallasTemperature = dallasTemperature2;
  }
  else if(port==3)
  {
    if(!initDallas3)
    {
      initDallas3=true;
      oneWire3 = new OneWire(3);
      dallasTemperature3 = new DallasTemperature(oneWire3);
      dallasTemperature3->begin();
    }
    dallasTemperature = dallasTemperature3;
  }
  else if(port==4)
  {
    if(!initDallas4)
    {
      initDallas4=true;
      oneWire4 = new OneWire(4);
      dallasTemperature4 = new DallasTemperature(oneWire4);
      dallasTemperature4->begin();
    }
    dallasTemperature = dallasTemperature4;
  }
  else if(port==5)
  {
    if(!initDallas5)
    {
      initDallas5=true;
      oneWire5 = new OneWire(5);
      dallasTemperature5 = new DallasTemperature(oneWire5);
      dallasTemperature5->begin();
    }
    dallasTemperature = dallasTemperature5;
  }
  else if(port==6)
  {
    if(!initDallas6)
    {
      initDallas6=true;
      oneWire6 = new OneWire(6);
      dallasTemperature6 = new DallasTemperature(oneWire6);
      dallasTemperature6->begin();
    }
    dallasTemperature = dallasTemperature6;
  }
  else if(port==7)
  {
    if(!initDallas7)
    {
      initDallas7=true;
      oneWire7 = new OneWire(7);
      dallasTemperature7 = new DallasTemperature(oneWire7);
      dallasTemperature7->begin();
    }
    dallasTemperature = dallasTemperature7;
  }
  else if(port==8)
  {
    if(!initDallas8)
    {
      initDallas8=true;
      oneWire8 = new OneWire(8);
      dallasTemperature8 = new DallasTemperature(oneWire8);
      dallasTemperature8->begin();
    }
    dallasTemperature = dallasTemperature8;
  }
  else if(port==9)
  {
    if(!initDallas9)
    {
      initDallas9=true;
      oneWire9 = new OneWire(9);
      dallasTemperature9 = new DallasTemperature(oneWire9);
      dallasTemperature9->begin();
    }
    dallasTemperature = dallasTemperature9;
  }
  else if(port==10)
  {
    if(!initDallas10)
    {
      initDallas10=true;
      oneWire10 = new OneWire(10);
      dallasTemperature10 = new DallasTemperature(oneWire10);
      dallasTemperature10->begin();
    }
    dallasTemperature = dallasTemperature10;
  }
  else if(port==11)
  {
    if(!initDallas11)
    {
      initDallas11=true;
      oneWire11 = new OneWire(11);
      dallasTemperature11 = new DallasTemperature(oneWire11);
      dallasTemperature11->begin();
    }
    dallasTemperature = dallasTemperature11;
  }
  else if(port==12)
  {
    if(!initDallas12)
    {
      initDallas12=true;
      oneWire12 = new OneWire(12);
      dallasTemperature12 = new DallasTemperature(oneWire12);
      dallasTemperature12->begin();
    }
    dallasTemperature = dallasTemperature12;
  }
  else if(port==13)
  {
    if(!initDallas13)
    {
      initDallas13=true;
      oneWire13 = new OneWire(13);
      dallasTemperature13 = new DallasTemperature(oneWire13);
      dallasTemperature13->begin();
    }
    dallasTemperature = dallasTemperature13;
  }
  else if(port==14)
  {
    if(!initDallas14)
    {
      initDallas14=true;
      oneWire14 = new OneWire(14);
      dallasTemperature14 = new DallasTemperature(oneWire14);
      dallasTemperature14->begin();
    }
    dallasTemperature = dallasTemperature14;
  }
  else if(port==15)
  {
    if(!initDallas15)
    {
      initDallas15=true;
      oneWire15 = new OneWire(15);
      dallasTemperature15 = new DallasTemperature(oneWire15);
      dallasTemperature15->begin();
    }
    dallasTemperature = dallasTemperature15;
  }
  else if(port==16)
  {
    if(!initDallas16)
    {
      initDallas16=true;
      oneWire16 = new OneWire(16);
      dallasTemperature16 = new DallasTemperature(oneWire16);
      dallasTemperature16->begin();
    }
    dallasTemperature = dallasTemperature16;
  }
  
  numberOfDevices = dallasTemperature->getDeviceCount();

  if(_channel<numberOfDevices)
    dallasTemperature->getAddress(_address, channel);

}

void SensorDallas::preCycle(int cycleId)
{
}

Data* SensorDallas::read(bool shouldPostData)
{  
  if(!isResetting && numberOfDevices>0 && _channel<numberOfDevices)
  {    
    dallasTemperature->requestTemperaturesByAddress(_address);
    float tempC = dallasTemperature->getTempC(_address);

    if(tempC==-127.00 || tempC==85.00)
    {
      for(int i=0;i<10;i++)
      {
        dallasTemperature->requestTemperaturesByAddress(_address);
        delay(50);
        yield();
        tempC = dallasTemperature->getTempC(_address);
        delay(100);
        yield();

        if(tempC!=-127.00 && tempC!=85.00)
          break;
      }
    }
    
    if(tempC!=-127.00 && tempC!=85.00) {
      shouldPostData = smartSensorCheck(tempC, 0.3f, shouldPostData);
      return _calculation->calculate(this, tempC, shouldPostData);
    }
  }

  return NULL;

}

boolean SensorDallas::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
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