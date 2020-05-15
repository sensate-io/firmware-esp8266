/**************************************************************************/
/*!
    @file     DisplaySSD1306.cpp
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

#include "DisplaySSD1306.h"

extern bool isResetting;

int displayMode;
boolean displayEnabled;
int displayType;
int displayHeight;
int displayWidth;
int displayRotation;
bool firstSensorData;

Display::Display(bool flip, String type, String address, uint8_t PortSDA, uint8_t PortSCL) {

  if(!isResetting)
  {
    OLEDDISPLAY_GEOMETRY g;
    
    if(displayWidth==128 && displayHeight==32)
      g = GEOMETRY_128_32;
    else
      g = GEOMETRY_128_64;

    firstSensorData=true;

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

//void drawLines() {
//  for (int16_t i=0; i<display.getWidth(); i+=4) {
//    display.drawLine(0, 0, i, display.getHeight()-1);
//    display.display();
//    delay(10);
//  }
//  for (int16_t i=0; i<display.getHeight(); i+=4) {
//    display.drawLine(0, 0, display.getWidth()-1, i);
//    display.display();
//    delay(10);
//  }
//  delay(250);
//
//  display.clear();
//  for (int16_t i=0; i<display.getWidth(); i+=4) {
//    display.drawLine(0, display.getHeight()-1, i, 0);
//    display.display();
//    delay(10);
//  }
//  for (int16_t i=display.getHeight()-1; i>=0; i-=4) {
//    display.drawLine(0, display.getHeight()-1, display.getWidth()-1, i);
//    display.display();
//    delay(10);
//  }
//  delay(250);
//
//  display.clear();
//  for (int16_t i=display.getWidth()-1; i>=0; i-=4) {
//    display.drawLine(display.getWidth()-1, display.getHeight()-1, i, 0);
//    display.display();
//    delay(10);
//  }
//  for (int16_t i=display.getHeight()-1; i>=0; i-=4) {
//    display.drawLine(display.getWidth()-1, display.getHeight()-1, 0, i);
//    display.display();
//    delay(10);
//  }
//  delay(250);
//  display.clear();
//  for (int16_t i=0; i<display.getHeight(); i+=4) {
//    display.drawLine(display.getWidth()-1, 0, 0, i);
//    display.display();
//    delay(10);
//  }
//  for (int16_t i=0; i<display.getWidth(); i+=4) {
//    display.drawLine(display.getWidth()-1, 0, i, display.getHeight()-1);
//    display.display();
//    delay(10);
//  }
//  delay(250);
//}
//
//// Adapted from Adafruit_SSD1306
//void drawRect(void) {
//  for (int16_t i=0; i<display.getHeight()/2; i+=2) {
//    display.drawRect(i, i, display.getWidth()-2*i, display.getHeight()-2*i);
//    display.display();
//    delay(10);
//  }
//}
//
//// Adapted from Adafruit_SSD1306
//void fillRect(void) {
//  uint8_t color = 1;
//  for (int16_t i=0; i<display.getHeight()/2; i+=3) {
//    display.setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
//    display.fillRect(i, i, display.getWidth() - i*2, display.getHeight() - i*2);
//    display.display();
//    delay(10);
//    color++;
//  }
//  // Reset back to WHITE
//  display.setColor(WHITE);
//}
//
//// Adapted from Adafruit_SSD1306
//void drawCircle(void) {
//  for (int16_t i=0; i<display.getHeight(); i+=2) {
//    display.drawCircle(display.getWidth()/2, display.getHeight()/2, i);
//    display.display();
//    delay(10);
//  }
//  delay(1000);
//  display.clear();
//
//  // This will draw the part of the circel in quadrant 1
//  // Quadrants are numberd like this:
//  //   0010 | 0001
//  //  ------|-----
//  //   0100 | 1000
//  //
//  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000001);
//  display.display();
//  delay(200);
//  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000011);
//  display.display();
//  delay(200);
//  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000111);
//  display.display();
//  delay(200);
//  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00001111);
//  display.display();
//}
//
//void printBuffer(void) {
//  // Initialize the log buffer
//  // allocate memory to store 8 lines of text and 30 chars per line.
//  display.setLogBuffer(5, 30);
//
//  // Some test data
//  const char* test[] = {
//      "Hello",
//      "World" ,
//      "----",
//      "Show off",
//      "how",
//      "the log buffer",
//      "is",
//      "working.",
//      "Even",
//      "scrolling is",
//      "working"
//  };
//
//  for (uint8_t i = 0; i < 11; i++) {
//    display.clear();
//    // Print to the screen
//    display.println(test[i]);
//    // Draw it to the internal screen buffer
//    display.drawLogBuffer(0, 0);
//    // Display it on the screen
//    display.display();
//    delay(500);
//  }
//}