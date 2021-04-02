/**************************************************************************/
/*!
    @file     DisplayST7735.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP8266 firmware is used to connect ESP8266 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v41 - New Display Type ST7735, New Display Mode
*/
/**************************************************************************/

#include "DisplayST7735.h"

extern bool isResetting;
extern VisualisationHelper* vHelper;

extern int displayMode;
extern boolean displayEnabled;
extern bool firstSensorData;

DisplayST7735::DisplayST7735(bool flip, int _type) : Display(_type)  {

  if(!isResetting)
  {
	  display = new Adafruit_ST7735(15, 0, 2);
	  display->initR(INITR_BLACKTAB);

	  if(flip)
		  display->setRotation(3);
	  else
		  display->setRotation(1);

	  displayWidth = display->width(); // @suppress("Method cannot be resolved")
	  displayHeight = display->height(); // @suppress("Method cannot be resolved")

	  Serial.println("displayWidth "+String(displayWidth));
	  Serial.println("displayHeight "+String(displayHeight));

	  visibleDataCount = displayHeight/16;

	  Serial.println("visibleDataCount "+String(visibleDataCount));

	  display->fillScreen(ST7735_BLACK); // @suppress("Method cannot be resolved")

	  firstSensorData=true;

  }

}

void DisplayST7735::clear(boolean update) {
  if(!isResetting)
  {
    display->fillScreen(ST7735_BLACK); // @suppress("Method cannot be resolved")
  }
}

void DisplayST7735::drawProductLogo() {

	int posX = (displayWidth - Product_Logo_width) / 2;
	int posY = (displayHeight - Product_Logo_height) / 2;

	display->drawXBitmap(posX, posY, Product_Logo, Product_Logo_width, Product_Logo_height, ST7735_WHITE); // @suppress("Method cannot be resolved")

}

void DisplayST7735::drawData(unsigned long currentMillis) {

	for(int i=0;i<visibleDataCount;i++)
	  {
	    DisplayValueData* displayValueData = vHelper->getDataForPosition(currentMillis, i);
	    if(displayValueData!=NULL)
	      drawValue(i, displayValueData->getName(), displayValueData->getShortName(), displayValueData->getValue(), displayValueData->getUnit());
	    else
	      clearValue(i);
	  }

}

void DisplayST7735::drawValue(int position, String name, String shortName, String value, String unit) {

  if(firstSensorData)
  {
    clear(true);
    firstSensorData=false;
  }

  String valueString = value + " " + unit;

  switch(displayMode)
  {
  	  case 2:
  		drawValueClassicRA(position, name, shortName, valueString);
  		break;
  	  default:
  		  drawValueClassic(position, name, shortName, valueString);
  		  break;
  }

}

void DisplayST7735::drawValueClassic(int position, String name, String shortName, String valueString) {

  if(!isResetting && displayEnabled)
  {
	name.replace("ä", "ae");
	name.replace("ü", "ue");
	name.replace("ö", "oe");
	name.replace("ß", "sz");
	name.replace("Ä", "Ae");
	name.replace("Ü", "Ue");
	name.replace("Ö", "Oe");

    String text = name+": "+valueString;
    if(getStringWidth(text,8)>=displayWidth)
    {
    	shortName.replace("ä", "ae");
    	shortName.replace("ü", "ue");
    	shortName.replace("ö", "oe");
    	shortName.replace("ß", "sz");
    	shortName.replace("Ä", "Ae");
    	shortName.replace("Ü", "Ue");
    	shortName.replace("Ö", "Oe");
    	text = shortName+": "+valueString;
    }

    display->setCursor(0, position*16); // @suppress("Method cannot be resolved")
	display->setTextColor(ST7735_WHITE, ST7735_BLACK); // @suppress("Method cannot be resolved")
	display->print(text); // @suppress("Method cannot be resolved")


	int diffWidth = displayWidth - getStringWidth(text,8);
	if(diffWidth>0)
	{
		display->fillRect(displayWidth-diffWidth, position*16, diffWidth, 16, ST7735_BLACK); // @suppress("Method cannot be resolved")
	}

  }

}

void DisplayST7735::drawValueClassicRA(int position, String name, String shortName, String valueString) {

  if(!isResetting && displayEnabled)
  {
	display->setTextColor(ST7735_WHITE, ST7735_BLACK); // @suppress("Method cannot be resolved")

	int valueWidth = getStringWidth(valueString,8);
	int valuePosition = displayWidth-valueWidth;

	display->setCursor(valuePosition, position*16); // @suppress("Method cannot be resolved")
	display->print(valueString); // @suppress("Method cannot be resolved")

	name.replace("ä", "ae");
	name.replace("ü", "ue");
	name.replace("ö", "oe");
	name.replace("ß", "sz");
	name.replace("Ä", "Ae");
	name.replace("Ü", "Ue");
	name.replace("Ö", "Oe");

    String text = name+":";
    if(getStringWidth(text,8)>=valuePosition)
    {
    	shortName.replace("ä", "ae");
    	shortName.replace("ü", "ue");
    	shortName.replace("ö", "oe");
    	shortName.replace("ß", "sz");
    	shortName.replace("Ä", "Ae");
    	shortName.replace("Ü", "Ue");
    	shortName.replace("Ö", "Oe");
    	text = shortName+":";
    }

    display->setCursor(0, position*16); // @suppress("Method cannot be resolved")
	display->print(text); // @suppress("Method cannot be resolved")

	int nameWidth = getStringWidth(text,8);

	int diffWidth = displayWidth - nameWidth - valueWidth;
	if(diffWidth>0)
	{
		display->fillRect(nameWidth, position*16, diffWidth, 16, ST7735_BLACK); // @suppress("Method cannot be resolved")
	}

  }

}

void DisplayST7735::clearValue(int position) {

  switch(displayMode)
  {
    default:
      clearValueClassic(position);
      break;
  }

}

void DisplayST7735::clearValueClassic(int position) {

  if(!isResetting && displayEnabled)
  {
    display->fillRect(0, position*16, displayWidth, 16, ST7735_BLACK); // @suppress("Method cannot be resolved")
  }

}

void DisplayST7735::drawString(int16_t x, int16_t y, String text) {

  if(!isResetting && displayEnabled)
  {
	  display->setCursor(x, y); // @suppress("Method cannot be resolved")
	  display->setTextColor(ST77XX_CYAN, ST7735_BLACK); // @suppress("Method cannot be resolved")
	  display->print(text); // @suppress("Method cannot be resolved")
  }

}

void DisplayST7735::drawDisconnected(bool update) {

}

void DisplayST7735::drawConnected(bool update) {

}

int DisplayST7735::getSimultanValueCount() {
	return visibleDataCount;
}

void DisplayST7735::flip(int rotation) {
	if(rotation==0)
		  display->setRotation(1);
	  else
		  display->setRotation(3);
	clear(true);
}

int DisplayST7735::getStringWidth(String text, int textHeight) {

	int16_t x,y;
	uint16_t w,h;

	display->getTextBounds(text, 0, 0, &x, &y, &w, &h); // @suppress("Method cannot be resolved")

	if(h>textHeight)
		w = w*h/textHeight;

	return w;

}
