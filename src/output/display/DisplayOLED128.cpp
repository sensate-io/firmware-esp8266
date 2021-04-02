/**************************************************************************/
/*!
    @file     DisplayOLED128.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP8266 firmware is used to connect ESP8266 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v41 - Renamed Display Class to support more types, New Display Mode
    v40 - New Display Structure to enable Display Rotation, different Styles etc.
    v33 - Added Digital Sensor Switch Support
    v30 - Added Support for SSD1306 Displays
    v29 - First Public Release
*/
/**************************************************************************/

#include "DisplayOLED128.h"

extern bool isResetting;
extern VisualisationHelper* vHelper;

extern int displayMode;
extern boolean displayEnabled;
extern bool firstSensorData;

DisplayOLED128::DisplayOLED128(int width, int height, bool flip, int _type, String address, uint8_t PortSDA, uint8_t PortSCL) : Display(_type) {

  if(!isResetting)
  {
    OLEDDISPLAY_GEOMETRY g;
    
    displayWidth = width;
    displayHeight = height;

    if(displayWidth==128 && displayHeight==32)
    {
      g = GEOMETRY_128_32;
      visibleDataCount = 2;
    }
    else
    {
      g = GEOMETRY_128_64;
      visibleDataCount = 4;
    }  

    firstSensorData=true;

    if(type==2)
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

void DisplayOLED128::clear(boolean update) {
  
  if(!isResetting)
  {
    display->clear();
    if(update)
      display->display();
  }
}

void DisplayOLED128::flip(int rotation) {
  display->resetDisplay();
  display->setContrast(255);

  if(rotation==180)
    display->flipScreenVertically();
  else
    display->resetOrientation();
}

void DisplayOLED128::drawProductLogo() {
  
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    // https://www.online-utility.org/image_converter.jsp --> XBM Mono
    
    if(!isResetting && displayEnabled)
    {
      display->drawXbm(0, 0, Product_Logo_width, Product_Logo_height, Product_Logo);
      display->display();
    }
}

void DisplayOLED128::drawString(int16_t x, int16_t y, String text) {

  if(!isResetting && displayEnabled)
  {
    display->setColor(BLACK);
    display->fillRect(x, y, display->getStringWidth(text), 16);
    display->setColor(WHITE);
    display->drawString(x, y, text);
    display->display();
  }
    
}

void DisplayOLED128::drawArrow() {
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

void DisplayOLED128::blinkArrow(int count) {

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

void DisplayOLED128::drawDisconnected(bool update) {

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

void DisplayOLED128::drawConnected(bool update) {

  if(!isResetting && displayEnabled)
  {
    display->setColor(BLACK);
    display->fillRect(112, 48, 16, 16);
    if(update)
        display->display();
    }
}

void DisplayOLED128::drawData(unsigned long currentMillis) {

  for(int i=0;i<visibleDataCount;i++)
  {
    DisplayValueData* displayValueData = vHelper->getDataForPosition(currentMillis, i);
    if(displayValueData!=NULL)
      drawValue(i, displayValueData->getName(), displayValueData->getShortName(), displayValueData->getValue(), displayValueData->getUnit());
    else
      clearValue(i);
  }

}

void DisplayOLED128::clearValue(int position) {

  switch(displayMode)
  {
    case 1:
      clearValueQuad(position);
      break;
    default:
      clearValueClassic(position);
      break;
  }

}

void DisplayOLED128::drawValue(int position, String name, String shortName, String value, String unit) {

  if(firstSensorData)
  {
    clear(true);
    firstSensorData=false;
  }

  String valueString = value + " " + unit;

  switch(displayMode)
  {
    case 1:
      drawValueQuad(position, name, shortName, valueString);
      break;
    case 2:
    	drawValueClassicRA(position, name, shortName, valueString);
    	break;
    default:
      drawValueClassic(position, name, shortName, valueString);
      break;
  }

}

//void DisplayOLED128::drawValue(int position, String name, String shortName, float value, String unit) {
//
//  if(firstSensorData)
//  {
//    clear(true);
//    firstSensorData=false;
//  }
//
//  String valueString = String(value) + " " + unit;
//
//  switch(displayMode)
//  {
//    case 1:
//      drawValueQuad(position, name, shortName, valueString);
//      break;
//    default:
//      drawValueClassic(position, name, shortName, valueString);
//      break;
//  }
//
//}
//
//void DisplayOLED128::drawValue(int position, String name, String shortName, bool value, String onString, String offString) {
//
//  if(firstSensorData)
//  {
//    clear(true);
//    firstSensorData=false;
//  }
//
//  String boolString;
//    if(value)
//      boolString = onString;
//    else
//      boolString = offString;
//
//  switch(displayMode)
//  {
//    case 1:
//      drawValueQuad(position, name, shortName, boolString);
//      break;
//    default:
//      drawValueClassic(position, name, shortName, boolString);
//      break;
//  }
//
//}

void DisplayOLED128::drawValueClassic(int position, String name, String shortName, String valueString) {

  if(!isResetting && displayEnabled)
  {
    String text = name+": "+valueString;
    if(display->getStringWidth(text)>=128)
      text = shortName+": "+valueString;

    display->setColor(BLACK);
    display->fillRect(0, position*16, 128, 16);
    display->setColor(WHITE);
    display->drawString(0, position*16, text);
    display->display();
  }
  
}

void DisplayOLED128::drawValueClassicRA(int position, String name, String shortName, String valueString) {

  if(!isResetting && displayEnabled)
  {
	display->setColor(BLACK);
	display->fillRect(0, position*16, 128, 16);
	display->setColor(WHITE);

	int valueWidth = display->getStringWidth(valueString);
	int valuePosition = displayWidth-valueWidth;

	display->drawString(valuePosition, position*16, valueString);

	String text = name+":";
	if(display->getStringWidth(text)>=valuePosition)
	{
		text = shortName+":";
	}

    display->drawString(0, position*16, text);
    display->display();
  }

}

void DisplayOLED128::drawValueQuad(int position, String name, String shortName, String valueString) {

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

    String text = valueString;
    x = x+(w/2)-(display->getStringWidth(text)/2);

    display->drawString(x,y+16,text);
    
    display->drawLine(0, h, 2*w-1, h);
    display->drawLine(w, 0, w, 2*h-1);

    display->display();
  }

}

void DisplayOLED128::clearValueClassic(int position) {

  if(!isResetting && displayEnabled)
  {
    display->setColor(BLACK);
    display->fillRect(0, position*16, 128, 16);
  }
  
}

void DisplayOLED128::clearValueQuad(int position) {

  if(!isResetting && displayEnabled)
  {
    int rectX=0;
    int rectY=0;
    int w = 63;
    int h = 31;

    switch(position)
    {
      case 0:
        break;
      case 1:
        rectX = w+2;
        break;
      case 2:
        rectY = h+2;
        break;
      case 3:
        rectX = w+2;
        rectY = h+2;
        break;
    }

    display->setColor(BLACK);
    display->fillRect(rectX, rectY, w, h);

    display->display();
  }

}

int DisplayOLED128::getSimultanValueCount() {
	return visibleDataCount;
}
