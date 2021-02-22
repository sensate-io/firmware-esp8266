/**************************************************************************/
/*!
    @file     DisplayValueData.cpp
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

#include "DisplayValueData.h"

DisplayValueData::DisplayValueData (int position, String name, String shortName, String initValue)
{
    _position = position;
    _name = name;
    _shortName = shortName;
    _stringValue = initValue;
    _valueType = STRING;
}

void DisplayValueData::update(float value, String unit)
{
    _valueType = FLOAT;
    _floatValue = value;
    _unit = unit;
}

void DisplayValueData::update(boolean value, String unit)
{
    _valueType = BOOL;
    _boolValue = value;
    _unit = unit;
}

void DisplayValueData::update(String value, String unit)
{
    _valueType = STRING;
    _stringValue = value;
    _unit = unit;
}

int DisplayValueData::getPosition()
{
    return _position;
}

String DisplayValueData::getName() 
{
    return _name;
}

String DisplayValueData::getShortName() 
{
    return _shortName;
}

String DisplayValueData::getValue() 
{
    switch (_valueType)
    {
        case FLOAT:
            return String(_floatValue);
        case BOOL:
        {
            if(_boolValue) 
                return "ON";
            else
                return "OFF";
        }
        case STRING:
            return _stringValue;
    }
    return "";
}

String DisplayValueData::getUnit() 
{
    return _unit;
}