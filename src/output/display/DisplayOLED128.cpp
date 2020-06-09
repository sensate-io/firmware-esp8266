/**************************************************************************/
/*!
    @file     DisplayOLED128.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v30 - Added Support for SSD1306 Displays
    v29 - First Public Release
*/
/**************************************************************************/

#include "DisplayOLED128.h"

extern bool isResetting;

int displayMode;
boolean displayEnabled;
int displayType;
int displayHeight;
int displayWidth;
int displayRotation;
bool firstSensorData;

Display::Display(bool flip, int _type, String address, uint8_t PortSDA, uint8_t PortSCL) {

  if(!isResetting)
  {
    OLEDDISPLAY_GEOMETRY g;
    
    if(displayWidth==128 && displayHeight==32)
      g = GEOMETRY_128_32;
    else
      g = GEOMETRY_128_64;

    firstSensorData=true;

    type = _type;
    if(_type==2)
      display = new SSD1306Wire(0x3c, PortSDA, PortSCL, g);
    else
      display = new SH1106Wire(0x3c, PortSDA, PortSCL, g);

    display->init();
    if(flip)
      display->flipScreenVertically();
    display->setContrast(255);
    display->clear();

  }
}

void Display::clear(boolean update) {
  
  if(!isResetting)
  {
    display->clear();
    if(update)
      display->display();
  }
}

void Display::flip(int rotation) {
  display->resetDisplay();
  display->setContrast(255);

  if(rotation==180)
    display->flipScreenVertically();
  else
    display->resetOrientation();
}

int Display::getType()
{
  return type;
}

void Display::drawProductLogo() {
  
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    // https://www.online-utility.org/image_converter.jsp --> XBM Mono
    
    if(!isResetting && displayEnabled)
    {
      display->drawXbm(0, 0, Product_Logo_width, Product_Logo_height, Product_Logo);
      display->display();
    }
}

void Display::drawString(int16_t x, int16_t y, String text) {

  if(!isResetting && displayEnabled)
  {
    display->setColor(BLACK);
    display->fillRect(x, y, display->getStringWidth(text), 16);
    display->setColor(WHITE);
    display->drawString(x, y, text);
    display->display();
  }
    
}

void Display::drawArrow() {
  if(!isResetting && displayEnabled)
  {
   display->drawLine(0, 0, 30, display->getHeight()/2);
   display->drawLine(4, 0, 34, display->getHeight()/2);
   display->drawLine(30, display->getHeight()/2, 0, display->getHeight()-1);
   display->drawLine(34, display->getHeight()/2, 4, display->getHeight()-1);
   display->drawLine(display->getWidth()-1, 0, display->getWidth()-30, display->getHeight()/2);
   display->drawLine(display->getWidth()-5, 0, display->getWidth()-34, display->getHeight()/2);
   display->drawLine(display->getWidth()-30, display->getHeight()/2, display->getWidth()-1, display->getHeight()-1);
   display->drawLine(display->getWidth()-34, display->getHeight()/2, display->getWidth()-5, display->getHeight()-1);
    display->display();
  }
    
}

void Display::blinkArrow(int count) {

  for(int i=0;i<count;i++)
  {
    clear(true);
    delay(500);
    drawArrow();
    if(i<count-1)
      delay(500);
  }
  clear(true);
}

void Display::drawDisconnected(bool update) {

  if(!isResetting && displayEnabled)
  {
    display->setColor(BLACK);
    display->fillRect(112, 48, 16, 16);
    display->setColor(WHITE);
    
    display->drawLine(112, 53, 118, 53);
    display->drawLine(112, 59, 116, 59);
    display->drawLine(116, 63, 119, 49);
    display->drawLine(120, 63, 123, 49);

    display->drawLine(122, 53, 127, 53);
    display->drawLine(121, 59, 127, 59);

    if(update)
        display->display();
    }
}

void Display::drawConnected(bool update) {

  if(!isResetting && displayEnabled)
  {
    display->setColor(BLACK);
    display->fillRect(112, 48, 16, 16);
    if(update)
        display->display();
    }
}

void Display::drawValue(int position, String name, String shortName, float value, String unit) {

  if(firstSensorData)
  {
    clear(true);
    firstSensorData=false;
  }

  switch(displayMode)
  {
    case 1:
      drawValueQuad(position, name, shortName, value, unit);
      break;
    default:
      drawValueClassic(position, name, shortName, value, unit);
      break;
  }

}

void Display::drawValueClassic(int position, String name, String shortName, float value, String unit) {

  if(!isResetting && displayEnabled)
  {
    String text = name+": "+value + " "+unit;
    if(display->getStringWidth(text)>=128)
      text = shortName+": "+value + " "+unit;

    display->setColor(BLACK);
    display->fillRect(0, position*16, 128, 16);
    display->setColor(WHITE);
    display->drawString(0, position*16, text);
    display->display();
  }
  
}

void Display::drawValueQuad(int position, String name, String shortName, float value, String unit) {

  if(!isResetting && displayEnabled)
  {
    int x=0;
    int rectX=0;
    int y=0;
    int rectY=0;
    int w = 64;
    int h = 32;

    switch(position)
    {
      case 0:
        break;
      case 1:
        x = w+3;
        rectX = w+1;
        break;
      case 2:
        y = h+3;
        rectY = h+1;
        break;
      case 3:
        x = w+3;
        rectX = w+1;
        y = h+3;
        rectY = h+1;
        break;
    }

    display->setColor(BLACK);
    display->fillRect(rectX, rectY, w, h);
    display->setColor(WHITE);

    String label = name + ":";
    if(display->getStringWidth(label)>62)
      label = shortName + ":";

    display->drawString(x, y, label);

    String text = String(value)+" "+unit;
    x = x+(w/2)-(display->getStringWidth(text)/2);

    display->drawString(x,y+16,text);
    
    display->drawLine(0, h, 2*w-1, h);
    display->drawLine(w, 0, w, 2*h-1);

    display->display();
  }

}