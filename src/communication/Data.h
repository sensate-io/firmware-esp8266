/**************************************************************************/
/*!
    @file     Data.h
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

#include <Arduino.h>

#ifndef _Data_h_
#define _Data_h_

class Data {
  private:
    long _id;
    float _valueFloat;
    int _valueInt;
    boolean _valueBoolean;
    String _unit;
    int _type;
  public:
    Data(long, float, String);
    Data(long, int, String);
    Data(long, boolean, String);
    String getRequestString();
};

#endif