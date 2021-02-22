/**************************************************************************/
/*!
    @file     DisplayValueData.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP32 firmware is used to connect ESP32 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v40 - New Display Structure to enable Display Rotation, different Styles etc.
*/
/**************************************************************************/

#include <Arduino.h>
#include "valuetype.h"

#ifndef _DISPLAYVALUEDATA_h_
#define _DISPLAYVALUEDATA_h_

class DisplayValueData {
  private:
    int _position;
    String _name;
    String _shortName;
    valuetype _valueType;
    float _floatValue;
    boolean _boolValue;
    String _stringValue;
    String _unit;
  public:
    DisplayValueData (int, String, String, String);
    void update(float, String);
    void update(boolean, String);
    void update(String, String);
    int getPosition();
    String getName();
    String getShortName();
    String getValue();
    String getUnit();
};

#endif