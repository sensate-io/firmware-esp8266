
/**************************************************************************/
/*!
    @file     Data.cpp
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

#include "Data.h"

Data::Data(long id, float value, String unit) {
  _type = 1;
  _valueFloat = value;
  _id = id;
  _unit = unit;
}

Data::Data(long id, int value, String unit) {
  _type = 2;
  _valueInt = value;
  _id = id;
  _unit = unit;
}

Data::Data(long id, boolean value, String unit) {
  _type = 3;
  _valueBoolean = value;
  _id = id;
  _unit = unit;
}

String Data::getRequestString() {

  String returnString = String(_id) +",";
  
  if(_type==1)
    returnString = returnString + String(_valueFloat);
  else if(_type==2)
    returnString = returnString + String(_valueInt);
  else if(_type==3)
    returnString = returnString + String(_valueBoolean);  
  
  return returnString+","+_unit;
  
}