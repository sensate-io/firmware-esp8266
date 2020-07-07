/**************************************************************************/
/*!
    @file     SensorDHT.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v32 - Added MQTT Support!
    v31 - Fixed an issue with DHT11 Sensors
    v29 - First Public Release
*/
/**************************************************************************/

#include "SensorDHT.h"

boolean SensorDHT::initDHT0 = false;
boolean SensorDHT::initDHT1 = false;
boolean SensorDHT::initDHT2 = false;
boolean SensorDHT::initDHT3 = false;
boolean SensorDHT::initDHT4 = false;
boolean SensorDHT::initDHT5 = false;
boolean SensorDHT::initDHT6 = false;
boolean SensorDHT::initDHT7 = false;
boolean SensorDHT::initDHT8 = false;
boolean SensorDHT::initDHT9 = false;
boolean SensorDHT::initDHT10 = false;
boolean SensorDHT::initDHT11 = false;
boolean SensorDHT::initDHT12 = false;
boolean SensorDHT::initDHT13 = false;
boolean SensorDHT::initDHT14 = false;
boolean SensorDHT::initDHT15 = false;
boolean SensorDHT::initDHT16 = false;

DHT_Unified* SensorDHT::dht0 = NULL;
DHT_Unified* SensorDHT::dht1 = NULL;
DHT_Unified* SensorDHT::dht2 = NULL;
DHT_Unified* SensorDHT::dht3 = NULL;
DHT_Unified* SensorDHT::dht4 = NULL;
DHT_Unified* SensorDHT::dht5 = NULL;
DHT_Unified* SensorDHT::dht6 = NULL;
DHT_Unified* SensorDHT::dht7 = NULL;
DHT_Unified* SensorDHT::dht8 = NULL;
DHT_Unified* SensorDHT::dht9 = NULL;
DHT_Unified* SensorDHT::dht10 = NULL;
DHT_Unified* SensorDHT::dht11 = NULL;
DHT_Unified* SensorDHT::dht12 = NULL;
DHT_Unified* SensorDHT::dht13 = NULL;
DHT_Unified* SensorDHT::dht14 = NULL;
DHT_Unified* SensorDHT::dht15 = NULL;
DHT_Unified* SensorDHT::dht16 = NULL;

extern boolean isResetting;
extern int powerMode;

SensorDHT::SensorDHT (long id, String category, String shortName, String name, String dhtType, uint8_t port, int refreshInterval, int postDataInterval, float smartValueThreshold, SensorCalculation* calculation) : Sensor (id, category, shortName, name, refreshInterval, postDataInterval, smartValueThreshold, calculation) {

  uint8_t type;

  if(dhtType=="DHT11")
    type = DHT11;
  else if(dhtType=="DHT21")
    type = DHT21;
  else if(dhtType=="DHT22")
  {
    type = DHT22;
  }

  if(port==0)
  {
    if(!initDHT0)
    {
      if(type!=DHT11)
        portFix(port);  
      initDHT0=true;
      dht0 = new DHT_Unified(0, type);
      dht0->begin();
    }
    dht = dht0;
  }
  else if(port==1)
  {
    if(!initDHT1)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT1=true;
      dht1 = new DHT_Unified(1, type);
      dht1->begin();
    }
    dht = dht1;
  }
  else if(port==2)
  {
    if(!initDHT2)
    {
      if(type!=DHT11)
        portFix(port);
      initDHT2=true;
      dht2 = new DHT_Unified(2, type);
      dht2->begin();
    }
    dht = dht2;
  }
  else if(port==3)
  {
    if(!initDHT3)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT3=true;
      dht3 = new DHT_Unified(3, type);
      dht3->begin();
    }
    else
    {
      dht = dht3;
    }
  }
  else if(port==4)
  {
    if(!initDHT4)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT4=true;
      dht4 = new DHT_Unified(4, type);
      dht4->begin();
    }
    dht = dht4;
  }
  else if(port==5)
  {
    if(!initDHT5)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT5=true;
      dht5 = new DHT_Unified(5, type);
      dht5->begin();
    }
    else
    {
      dht = dht5;
    }
  }
  else if(port==6)
  {
    if(!initDHT6)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT6=true;
      dht6 = new DHT_Unified(6, type);
      dht6->begin();
    }
    dht = dht6;
  }
  else if(port==7)
  {
    if(!initDHT7)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT7=true;
      dht7 = new DHT_Unified(7, type);
      dht7->begin();
    }
    dht = dht7;
  }
  else if(port==8)
  {
    if(!initDHT8)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT8=true;
      dht8 = new DHT_Unified(8, type);
      dht8->begin();
    }
    dht = dht8;
  }
  else if(port==9)
  {
    if(!initDHT9)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT9=true;
      dht9 = new DHT_Unified(9, type);
      dht9->begin();
    }
    else
    {
      dht = dht9;
    }
  }
  else if(port==10)
  {
    if(!initDHT10)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT10=true;
      dht10 = new DHT_Unified(10, type);
      dht10->begin();
    }
    dht = dht10;
  }
  else if(port==11)
  {
    if(!initDHT11)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT11=true;
      dht11 = new DHT_Unified(11, type);
      dht11->begin();
    }
    dht = dht11;
  }
  else if(port==12)
  {
    if(!initDHT12)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT12=true;
      dht12 = new DHT_Unified(12, type);
      dht12->begin();
    }
    dht = dht12;
  }
  else if(port==13)
  {
    if(!initDHT13)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT13=true;
      dht13 = new DHT_Unified(13, type);
      dht13->begin();
    }
    dht = dht13;
  }
  else if(port==14)
  {
    if(!initDHT14)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT14=true;
      dht14 = new DHT_Unified(14, type);
      dht14->begin();
    }
    dht = dht14;
  }
  else if(port==15)
  {
    if(!initDHT15)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT15=true;
      dht15 = new DHT_Unified(15, type);
      dht15->begin();
    }
    dht = dht15;
  }
  else if(port==16)
  {
    if(!initDHT16)
    {    
      if(type!=DHT11)
        portFix(port);
      initDHT16=true;
      dht16 = new DHT_Unified(16, type);
      dht16->begin();
    }
    dht = dht16;
  }
}

void SensorDHT::portFix(uint8_t port)
{
  pinMode(port, OUTPUT);
  digitalWrite(port, LOW);
  delay(20);
  pinMode(port, INPUT);
}

void SensorDHT::preCycle(int cycleId)
{
}

Data* SensorDHT::read(bool shouldPostData)
{  
  if(!isResetting)
  {
    sensors_event_t event;

    if(_calculation->getValueType()=="humidity")
    {
      dht->humidity().getEvent(&event);

        if (isnan(event.relative_humidity)) {
          Serial.println("NAN Humidity!");
        }
        else {
          shouldPostData = smartSensorCheck(event.relative_humidity, _smartValueThreshold, shouldPostData);
          return _calculation->calculate(this, event.relative_humidity, shouldPostData);
        }
    }
    else if(_calculation->getValueType()=="temperature")
    {
      dht->temperature().getEvent(&event);
       
       if (isnan(event.temperature)) {
          Serial.println("NAN Temperature!");
        }
        else {
          shouldPostData = smartSensorCheck(event.temperature, _smartValueThreshold, shouldPostData);
          return _calculation->calculate(this, event.temperature, shouldPostData);
        }
    }
  }
 
  return NULL;
}

boolean SensorDHT::smartSensorCheck(float currentRawValue, float threshhold, boolean shouldPostData)
{
  if(powerMode == 3)
  {
    if(!shouldPostData)
    {
      if(!isnan(lastPostedValue))
      {
          if(lastPostedValue-currentRawValue>threshhold || lastPostedValue-currentRawValue<-threshhold)
          {
            Serial.println("Overwrite shouldPostData!");
            shouldPostData = true;
          }
          
      }
    }

    if(shouldPostData)
      lastPostedValue = currentRawValue;
  }

  return shouldPostData;
  
}