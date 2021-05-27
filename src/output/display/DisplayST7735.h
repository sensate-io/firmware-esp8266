/**************************************************************************/
/*!
    @file     DisplayST7735.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP8266 firmware is used to connect ESP8266 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v42 - Fixed Umlaut Handling
    v41 - New Display Type ST7735, New Display Mode
*/
/**************************************************************************/

#ifndef _DisplayST7735_h_
#define _DisplayST7735_h_

#include "Display.h"
#include "Adafruit_ST7735.h"
#include <WString.h>

class DisplayST7735 : public Display {
  private:
	Adafruit_ST7735 *display;
    int type;
    String replaceUmlauts(String original);
  public:
    DisplayST7735 (bool, int);
    void drawProductLogo();
    void clear(boolean update);
//    void drawString(int16_t x, int16_t y, String text);
//    void drawArrow();
//    void blinkArrow(int count);
    void drawDisconnected(bool update);
    void drawConnected(bool update);
    void flip(int rotation);
    void drawData(unsigned long currentMillis);
    void drawString(int16_t x, int16_t y, String text);
    void drawValue(int position, String name, String shortName, String value, String unit);
    void clearValue(int position);
    int getSimultanValueCount();
    void drawValueClassic(int position, String name, String shortName, String valueString);
    void drawValueClassicRA(int position, String name, String shortName, String valueString);
    void clearValueClassic(int position);
    void clearValueQuad(int position);
    int getStringWidth(String text, int);
    int getType();
};

#endif
