
/**************************************************************************/
/*!
    @file     Data.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v33 - Added Digital Sensor Switch Support
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include "Data.h"

Data::Data(Sensor *sensor, float value, String unit) {
  _type = 1;
  _valueFloat = value;
  _sensor = sensor;
  _unit = unit;
}

Data::Data(Sensor *sensor, int value, String unit) {
  _type = 2;
  _valueInt = value;
  _sensor = sensor;
  _unit = unit;
}

Data::Data(Sensor *sensor, boolean value, String unit) {
  _type = 3;
  _valueBoolean = value;
  _sensor = sensor;
  _unit = unit;
}

String Data::getRequestString() {

  String returnString = String(_sensor->getId()) +",";
  
  if(_type==1)
    returnString = returnString + String(_valueFloat);
  else if(_type==2)
    returnString = returnString + String(_valueInt);
  else if(_type==3)
  {
    if(_valueBoolean)
      returnString = returnString + "true";
    else
      returnString = returnString + "false";
      
  }
  
  return returnString+","+_unit;
  
}

String Data::getValueString() {

  if(_type==1)
    return String(_valueFloat);
  else if(_type==2)
    return String(_valueInt);
  else if(_type==3)
    return String(_valueBoolean);  

  return "";
}

Sensor* Data::getSensor() {
  return _sensor;
} 