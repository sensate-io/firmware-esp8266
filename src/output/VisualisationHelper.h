/**************************************************************************/
/*!
    @file     VisualisationHelper.h
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
#include "model/DisplayDataModel.h"

#ifndef _VISUALISATIONHELPER_h_
#define _VISUALISATIONHELPER_h_

class VisualisationHelper {
  private:
    DisplayDataModel* _displayDataModel;
    boolean _enableCycle = false;
    unsigned long _displayCycleInterval;
    int _simultanValueCount;
    unsigned long _lastDisplayCycleUpdate;
    int _currentCyclePosition = 0;
    int _maxCyclePosition = 0;
  public:
    VisualisationHelper();
    DisplayDataModel* getDisplayDataModel();
    void enableDisplayCycle(unsigned long, int, unsigned long);
    void disableDisplayCycle();
    void updateSimultanValueCount(int);
    DisplayValueData* getDataForPosition(unsigned long, int);
};

#endif
