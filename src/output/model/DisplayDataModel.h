/**************************************************************************/
/*!
    @file     DisplayDataModel.h
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
#include "DisplayValueData.h"

#define maxSensorCount 25

#ifndef _DISPLAYDATAMODEL_h_
#define _DISPLAYDATAMODEL_h_

class DisplayDataModel {
  private:
    int _count;
    DisplayValueData* _displayValueData[maxSensorCount];
  public:
    DisplayDataModel ();
    int getCount();
    void setData(int, DisplayValueData*);
    void updateData(int, float, String);
    void updateData(int, boolean, String);
    void updateData(int, String, String);
    DisplayValueData* getData(int);
};

#endif