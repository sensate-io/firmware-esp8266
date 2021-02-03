/**************************************************************************/
/*!
    @file     firmware-esp8266.ino
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v39 - ReAdded Support for VEML6075 and SI1145 UVI Sensors, added auto-reinit if sensor fails
    v38 - Changed automatic Update to only if required Update, removed VEML6075 and SI1145 UV Sensors
    v36 - Greatly improved reliability of connectivity
    v35 - Added Support for VEML6075 and SI1145 UVI Sensors
    v34 - Added Generic Analog Sensor Support
    v33 - Added Digital Sensor Switch Support, Improved MQTT Setup Routine
    v32 - Added MQTT Support!
    v31 - Fixed an issue with DHT11 Sensors
    v30 - Added support for SSD1306 I2C Displays
    v29 - First Public Release
*/
/**************************************************************************/

#include "src/controller/Bridge.h"
#include "src/output/display/DisplayOLED128.h"
#include "src/communication/WiFiManager.h"
#include "src/controller/OTA.h"
#include "src/communication/RestServer.h"

Display* display = NULL;

int currentVersion = 39; 
boolean printMemory = false;

String board = "Generic";
char firmwareType[] = "ESP8266";
// char firmwareType[] = "ESP8266-1M";

// String board = "NodeMCU";
// char firmwareType[] = "ESP8266-NodeMCU-pp";

// String board = "ESP12s";
// char firmwareType[] = "ESP8266-ESP12s";

// String board = "ESP07";
// char firmwareType[] = "ESP8266-ESP07";

// String board = "D1Mini";
// char firmwareType[] = "ESP8266-D1Mini";

extern String name = "Bridge";
extern String ucType = "ESP8266";

String variant = "SensateV"+String(currentVersion)+board;

String apiVersion = "v1";

int powerMode;
long powerOnDelay = 0;
String powerSavePort;
boolean invPowerSavePort = false;

bool isResetting = false;

unsigned long previousDelayMillis = 0; 
unsigned long previousTickerMillis = 0; 
unsigned long currentMillis;
State state = Boot;

extern struct rst_info resetInfo;
extern int displayType;
extern boolean displayEnabled;
extern int displayRotation;
extern int displayMode;

extern uint8_t i2cSDAPort;
extern uint8_t i2cSCLPort;
extern MQTT* mqtt;

#define tickerInterval 250
#define delayInterval 10000

void initSensate();

void setup()
{
	Serial.begin(9600);
  Serial.println("---------------------------");
  Serial.println(variant);
  Serial.println("---------------------------");
  Serial.println("Startup: ");
  Serial.println(getUUID());

  restoreBridgeConfig();

  doPowerSavingInit(true);

  Serial.println("Display Type:"+String(displayType));
  Serial.println("Display Enabled:"+String(displayEnabled));
  Serial.println("Display Rotation:"+String(displayRotation));
  Serial.println("Display Mode:"+String(displayMode));

  Serial.println("Power Mode:"+String(powerMode));
  Serial.println("Power On Delay:"+String(powerOnDelay));
  Serial.println("Power Save Port:"+String(powerSavePort));
  Serial.println("Power Save Port Inv:"+String(invPowerSavePort));
  

  if(displayType!=0)
  {
    boolean rotateDisplay = (displayRotation == 180);

    display = new Display(rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);

    if(!displayEnabled)
      display->clear(true);
  }

  if(display!=NULL)
    display->drawProductLogo();

  state = Connect_WiFi;
  
  initSensate();
}

void loop()
{
  if(!isResetting)
  {
    //First: Do whatever should be done in every loop!
    runLoop();

    //Then: Do short delayed actions (once every #tickerInterval miliseconds)
    currentMillis = millis();
    if(currentMillis - previousTickerMillis >= tickerInterval)
    {
      previousTickerMillis = currentMillis;
      runTick();
    }

    //Then: Do long delayed actions (once every #delayInterval miliseconds)
    currentMillis = millis();
    if(currentMillis - previousDelayMillis >= delayInterval)
    {
      previousDelayMillis = currentMillis;
      runDelayed();
    }
  }
}

void runLoop() {
  switch(state)
  {
    case Setup:
      loopDNS();
      checkWiFiStatus();
    case Connected_WiFi:
    case Init_Configuration:
    case Operating:
      loopRestserver();
      if(mqtt!=NULL)
        mqtt->loop();
      break;
  }
}

void runTick() {
  // Serial.print("-T-");
  switch(state)
  {
    case Operating:
      loopSensor(currentMillis);
      break;
  }

  if(printMemory)
  {
    Serial.print("HEAP: ");
    Serial.println(ESP.getFreeHeap());
  }
    
  if(ESP.getFreeHeap()<1000)
  {
    Serial.println("RUNNING OUT OF HEAP - RESTART");
    restart();
  }
}

void runDelayed() {
  // Serial.print("----D----");
  if(state!=Operating)
    initSensate();
  else
    checkStatus();
}

void initSensate() {

  if(state==Connect_WiFi)
  {
    Serial.println("STATE: Connect_WiFi");
    connectToNetwork();
  }
  if(state==Check_Firmware)
  {
    if(resetInfo.reason != REASON_DEEP_SLEEP_AWAKE)
    {
      Serial.println("STATE: Check_Firmware");
      tryFirmwareUpdate("");
    }
    else
    {
      Serial.println("Skip Check_Firmware (Wake from Sleep)");
      state=Connected_WiFi;
    }
  }
  if(state==Connected_WiFi)
  {
    Serial.println("STATE: Connected_WiFi");
    startRestServer();
  }
  if(state==Init_Setup)
  {
    Serial.println("STATE: Init_Setup");
    startRestServer();
    startDNS();
  }
  if(state==Register)
  {
    if(resetInfo.reason != REASON_DEEP_SLEEP_AWAKE)
    {
      Serial.println("STATE: Register_Bridge");
      initSSL();
      registerBridge();
    }
    else 
    {
      Serial.println("Skip Register_Bridge (Wake from Sleep)");
      initSSL();
      state=Init_Configuration;
    }
  }
  if(state==Init_Configuration)
  {
    Serial.println("STATE: Get_Configuration");
    getBridgeConfig();
  }

}