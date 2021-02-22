/**************************************************************************/
/*!
    @file     DisplayDataModel.cpp
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

#include "DisplayDataModel.h"

DisplayDataModel::DisplayDataModel ()
{
    _count = 0;
}

int DisplayDataModel::getCount()
{
    return _count;
}

void DisplayDataModel::setData(int position, DisplayValueData* newValueData)
{
    if(_count<=position)
    {
        _count=position+1;
        _displayValueData[position] = newValueData;
    }
}

void DisplayDataModel::updateData(int position, float value, String unit)
{
    if(position<_count)
        _displayValueData[position]->update(value, unit);
}

void DisplayDataModel::updateData(int position, boolean value, String unit)
{
    if(position<_count)
        _displayValueData[position]->update(value, unit);
}

void DisplayDataModel::updateData(int position, String value, String unit)
{
    if(position<_count)
        _displayValueData[position]->update(value, unit);
}

DisplayValueData* DisplayDataModel::getData(int position)
{
    if(position<_count)
        return _displayValueData[position];
    else
        return NULL;
}
