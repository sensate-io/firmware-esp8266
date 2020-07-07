/**************************************************************************/
/*!
    @file     Bridge.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include "Bridge.h"

extern State state;
extern bool isResetting;
extern StaticJsonBuffer<10000> jsonBuffer;
extern String apiVersion;
extern int currentVersion;
extern char firmwareType[];

extern int powerMode;
extern Display* display;
extern int displayType;
extern boolean displayEnabled;
extern int displayMode;
extern int displayHeight;
extern int displayWidth;
extern int displayRotation;

extern struct rst_info resetInfo;

extern String name;
extern String board;
extern String type;

extern long powerOnDelay;
extern String powerSavePort;
extern boolean invPowerSavePort;

extern boolean enableMQTT;
extern MQTT* mqtt;

uint8_t i2cSDAPort;
uint8_t i2cSCLPort;

unsigned long nextSensorDue = -1;

char pwdHash[41] = "";

#define ssl_fingerprint_prod "E9 A9 3D E2 AD AB C2 96 FA 3C A7 A8 57 DE 8E A0 95 59 9B 7A" //hub.sensate.cloud
#define ssl_fingerprint_test "E3 19 E5 F3 DA 1A BD 01 EB BC 08 E6 49 18 7D 54 66 BD 2B 12" //test.sensate.cloud
#define maxSensorCount 25

String bridgeURL;

Sensor *sensors[maxSensorCount];
int sensorCount = 0;

bool wasDisconnected = false;
bool serverError = false;

int registerRetry = 0;
int configRetry = 0;
int sensorCycle = 1;

bool registerBridge()
{
  if(display!=NULL)
  {
    display->clear(false);
    display->drawProductLogo();
    display->drawString(0, 10, "Signing in...");
  }
    
  String uuid = getUUID();
  String networkIP = WiFi.localIP().toString();

  if (uuid.length() > 0 && networkIP.length() > 0)
  {
    Serial.println("Registering Bridge " + uuid + " to Bridge located at " + bridgeURL);
    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient httpClient;

      String urlString = bridgeURL + "/" + apiVersion + "/bridge/";

      if(urlString.startsWith("https://hub"))
        httpClient.begin(urlString, ssl_fingerprint_prod);
      else if(urlString.startsWith("https://test"))
        httpClient.begin(urlString, ssl_fingerprint_test);
      else
        httpClient.begin(urlString);
        
      httpClient.addHeader("Content-Type", "application/json");
      httpClient.setTimeout(5000);

      String pwdHashString = "";

      if(pwdHash[0]!=0xff)
      {
        pwdHashString = String(pwdHashString);
      }
      
      String message = "{\"uuid\":\"" + uuid + "\",\"networkIP\":\"" + networkIP + "\",\"name\":\"" + name + "\",\"vendor\":\"" + board + "\",\"type\":\"" + type + "\",\"firmwareVersion\":" + currentVersion + ",\"secPassword\":\"" + pwdHashString + "\"}";

      int httpCode = httpClient.POST(message);

      if (httpCode == HTTP_CODE_OK)
      {
        String payload = httpClient.getString();
        if (payload == uuid)
        {
          registerRetry=0;
          state = Init_Configuration;
          httpClient.end();
          return true;
        }          
      }
      else if (httpCode == HTTP_CODE_UPGRADE_REQUIRED)
      {
        httpClient.end();
        restart();
        return false;
      }
      else
      {
        registerRetry++;
        Serial.println("Register failed..? - HTTP:" + String(httpCode));
        if(registerRetry>=10)
        {
          if(powerMode==2)
          {
            httpClient.end();
            Serial.println("Registering not possible, going back to Deep Sleep for 5 minutes.");
            trySleep(300000000);
            return false;
          }
          else if (registerRetry>=25)
          {
            httpClient.end();
            restart();
            return false;
          }
        }
      }
      
      httpClient.end();
      return false;

    }
    else
    {
      Serial.println(WiFi.status());
      Serial.println("Register failed..? - WIFI:" + WiFi.status());
      Serial.println("Trying to reconnect... " + WiFi.status());
      WiFi.reconnect();
      int conRes = WiFi.waitForConnectResult();
    }
    
  }
  else
  {
    Serial.println("Failed to register Bridge");
    return false;
  }

}

void restoreBridgeConfig() {
  
  EEPROM.begin(314);

  int val20 = EEPROM.read(20);
  int val21 = EEPROM.read(21);
  int val22 = EEPROM.read(22);
  int val23 = EEPROM.read(23);

  if (val20 == 11 && val21 == 21 && val22 == 31 && val23 == 41)
  {
    char urlChars[100];
    EEPROM.get(121, urlChars);
    bridgeURL = String(urlChars);
    Serial.println("Registered Bridge URL: " + bridgeURL);
  }

  int val221 = EEPROM.read(221);
  int val222 = EEPROM.read(222);

  if (val221 == 221 && val222 == 222)
  {
    char nameChars[25];
    EEPROM.get(223, nameChars);
    String tmpName = String(nameChars);
    if(!tmpName.equals(("")))
        name = tmpName;
      
    EEPROM.get(248, pwdHash);
  }

  int val290 = EEPROM.read(290);
  int val291 = EEPROM.read(291);

  // Display Config

  if (val290 == 90 && val291 == 91)
  {
    Serial.println("Restore Display Mode!");
    displayType = EEPROM.read(292);
    
    int dE = EEPROM.read(293);
    if(dE==0)
      displayEnabled = false;
    else
      displayEnabled = true;

    int dM = EEPROM.read(306);
    if(dM==1)
      displayMode=1;
    else
      displayMode=0;  

    int dR = EEPROM.read(307);
    if(dR==180)
      displayRotation = 180;
    else
      displayRotation = 0;
  }
  else
  {
    displayType = 1;
    displayEnabled = true;
  }

  // Power Config

  int val294 = EEPROM.read(294);
  int val295 = EEPROM.read(295);

  if (val294 == 94 && val295 == 95)
  {
    Serial.println("Restore Power Mode!");

    powerMode = EEPROM.read(296);

    char pspChars[4];
    EEPROM.get(297, pspChars);
    powerSavePort = String(pspChars);
    EEPROM.get(301, powerOnDelay);

    int iPSP = EEPROM.read(305);

    if(iPSP==0)
      invPowerSavePort = false;
    else
      invPowerSavePort = true;
  }
  else
  {
    powerMode = 0;
    powerSavePort = "";
    powerOnDelay = 0;
    invPowerSavePort = false;
  }

  int val308 = EEPROM.read(308);
  int val309 = EEPROM.read(309);

  if (val308 == 108 && val309 == 109)
  {
    Serial.println("Restore I2C And Display Size");

    i2cSDAPort = EEPROM.read(310);
    i2cSCLPort = EEPROM.read(311);

    displayWidth = EEPROM.read(312);
    displayHeight = EEPROM.read(313);
  }
  else
  {
    i2cSDAPort = 4;
    i2cSCLPort = 5;
    displayWidth = 128;
    displayHeight = 64;
  }

  Serial.println("I2C and Display Config: "+String(i2cSDAPort)+"/"+String(i2cSCLPort)+" - "+String(displayWidth)+"x"+String(displayHeight)+" ");

  EEPROM.end();
}

void restart() {

  Serial.println("restarting..");

  isResetting = true;

  shutDownWiFi();

  Serial.end();

  wdt_reset(); 
  ESP.restart(); 
  while(1)
    wdt_reset();
}

bool getBridgeConfig() {

  Serial.println("Getting Bridge Config from " + bridgeURL);

  HTTPClient httpClient;

  if(display!=NULL)
  {
    display->clear(false);
    display->drawProductLogo();
    display->drawString(0, 10, "Waiting for config...");
  }

  String urlString = bridgeURL + "/" + apiVersion + "/bridge/" + getUUID();

  if(urlString.startsWith("https://hub"))
    httpClient.begin(urlString, ssl_fingerprint_prod);
  else if(urlString.startsWith("https://test"))
    httpClient.begin(urlString, ssl_fingerprint_test);
  else
    httpClient.begin(urlString);

  httpClient.addHeader("Content-Type", "application/json");
    
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK)
  {
    String payload = httpClient.getString();

    int portNumber = 0;

    bool foundPorts = false;
    configRetry=0;

    if (payload != NULL && payload != "")
    {
      JsonObject& bridgeConfig = jsonBuffer.parseObject(payload);

      if(bridgeConfig.containsKey("p"))
      {
        configureBridge(bridgeConfig);
        yield();

        JsonArray& configArray = bridgeConfig["p"];

        for (JsonObject& configEntry : configArray) {
          
          int portRow = portNumber;

          if (configEntry.containsKey("sod") && !configEntry["sod"])
          {
              portRow = -1;
          }
          else
          {
            portNumber++;
          }
      
          if (configEntry.containsKey("et"))
            configureExpansionPort(portRow, configEntry);
          else
            configurePort(portRow, configEntry);

          foundPorts = true;
          yield();
        }

        if(display!=NULL)
        {
          display->clear(false);
          display->drawProductLogo();
          display->drawString(0, 10, "Waiting for sensors...");
        }

      }

      jsonBuffer.clear();

      httpClient.end();

      if(foundPorts)
        state = Operating;

      return true;
    }
  }

  httpClient.end();

  configRetry++;
  if(configRetry>=10 && powerMode==2)
  {
    Serial.println("Fetching configuration not possible, going back to Deep Sleep for 5 minutes.");
    trySleep(300000000);
  }
  return false;

}

void configureBridge(JsonObject& bridgeConfig) {

  powerMode = bridgeConfig["pm"];
  displayType = bridgeConfig["dt"];

  powerOnDelay =  bridgeConfig["pod"];
  powerSavePort = bridgeConfig["psp"].asString();
  invPowerSavePort = bridgeConfig["ipsp"];

  uint8_t oldSCL = i2cSCLPort;
  uint8_t oldSDA = i2cSDAPort;

  i2cSCLPort = bridgeConfig["i2c"];
  i2cSDAPort = bridgeConfig["i2d"];

  boolean updateRotation = false;

  int oldDisplayWidth = displayWidth;
  int oldDisplayHeight = displayHeight;

  if(displayType!=0)
  {
    displayMode = bridgeConfig["dm"];
    displayEnabled = bridgeConfig["de"];

    displayHeight = bridgeConfig["dh"];
    displayWidth = bridgeConfig["dw"];

    int newRotation = bridgeConfig["dr"];

    enableMQTT = bridgeConfig["me"];

    if(displayRotation!=newRotation)
    {
        displayRotation = newRotation;
        updateRotation = true;
    }

  }
  else
  {
    displayEnabled = false;
  }

  if(displayType!=0 && (oldSCL != i2cSCLPort || oldSDA != i2cSDAPort || oldDisplayWidth != displayWidth || oldDisplayHeight != displayHeight) || (display!=NULL && display->getType()!=displayType))
  {
    Serial.println("CREATE TEMP DISPLAY for new I2C Bus!");
    boolean rotateDisplay = (displayRotation == 180);
    display = new Display(rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);
  }
  else
  {
    if(display!=NULL && !displayEnabled)
    {
      display->clear(true);
    }
    else if(display==NULL && displayType!=0)
    {
      Serial.println("CREATE TEMP DISPLAY!");
      boolean rotateDisplay = (displayRotation == 180);
      display = new Display(rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);
    }
    else if(updateRotation && display!=NULL)
    {
      Serial.println("FLIP!");
      display->flip(displayRotation);
    }
    else if(displayType==0)
    {
      Serial.println("Init I2C Bus: SDA-"+String(i2cSDAPort)+", SCL-"+String(i2cSCLPort));
      Wire.begin(i2cSDAPort, i2cSCLPort);
    }
  }

  tryInitMQTT();
  
  storeDisplayAndPowerConfig(true);

  doPowerSavingInit(false);
}

void storeDisplayAndPowerConfig(boolean withPowerSettings) {

  EEPROM.begin(314);

// Display Config
  EEPROM.write(290, 90);
  EEPROM.write(291, 91);

  EEPROM.write(292, displayType);

  if(displayEnabled)
    EEPROM.write(293, 1);
  else
    EEPROM.write(293, 0);

  EEPROM.write(306, displayMode);
  EEPROM.write(307, displayRotation);

// Power Config

  if(withPowerSettings)
  {
    EEPROM.write(294, 94);
    EEPROM.write(295, 95);

    EEPROM.write(296, powerMode);

    char pspChars[4];
    powerSavePort.toCharArray(pspChars, 4);
    
    EEPROM.put(297, pspChars);
    EEPROM.put(301, powerOnDelay);

    if(invPowerSavePort)
      EEPROM.write(305, 1);
    else
      EEPROM.write(305, 0);
  }

  //I2C Config and More

  EEPROM.write(308, 108);
  EEPROM.write(309, 109);

  EEPROM.write(310, i2cSDAPort);
  EEPROM.write(311, i2cSCLPort);
  EEPROM.write(312, displayWidth);
  EEPROM.write(313, displayHeight);

  EEPROM.end();

  yield();
}

void tryInitMQTT() {

  if(!enableMQTT)
    return;
  
  Serial.println("Trying to init MQTT client...");

  EEPROM.begin(398);

  int val314 = EEPROM.read(314);
  int val315 = EEPROM.read(315);

  if(val314 == 114 && val315 == 115)
  {
    char brokerUrl[36];
    EEPROM.get(316, brokerUrl);
    long brokerPort;
    EEPROM.get(352, brokerPort);

    int val356 = EEPROM.read(356);
    int val357 = EEPROM.read(357);

    if(val356 == 156 && val357 == 157)
    {
      Serial.println("Starting Authenticated MQTT Broker connection...");

      char username[20];
      EEPROM.get(358, username);
      char password[20];
      EEPROM.get(378, password);

      mqtt = new MQTT(brokerUrl, brokerPort, String(username), String(password));
    }
    else
    {
      Serial.println(val356);
      Serial.println(val357);
      Serial.println("Starting Anonymous MQTT Broker connection...");
      mqtt = new MQTT(brokerUrl, brokerPort);
    }

    mqtt->connect();
  }
  else
  {
    Serial.println("No MQTT Broker configured.");
  }

  EEPROM.end();

  yield();

}


void configureExpansionPort(int portNumber, JsonObject& portConfig) {
  Serial.println("Configure Expansion Port: ");
  
  SensorCalculation* calc = NULL;

  if (portConfig["s"]["cf"] == "APPROX_QUAD")
    calc = new SensorCalculationApproxQuad(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portConfig["s"]["cv3"], portConfig["s"]["cv4"], portNumber);
  else if (portConfig["s"]["cf"] == "PT1001000")
    calc = new SensorCalculationPT1001000(portConfig["s"]["cv1"], portNumber);
  else if (portConfig["s"]["cf"] == "DUAL_NTC")
    calc = new SensorCalculationDualNtc(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KELVIN")
    calc = new SensorCalculationDirectKelvin(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_CELSIUS")
    calc = new SensorCalculationDirectCelsius(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_PERCENT")
    calc = new SensorCalculationDirectPercent(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_HEKTOPASCAL")
    calc = new SensorCalculationDirectHektoPascal(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_METER")
    calc = new SensorCalculationDirectMeter(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUX")
    calc = new SensorCalculationDirectLux(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUMEN")
    calc = new SensorCalculationDirectLumen(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_OHM")
    calc = new SensorCalculationDirectOhm(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KOHM")
    calc = new SensorCalculationDirectKOhm(portNumber);  
  else if (portConfig["s"]["cf"] == "DIRECT_PPM")
    calc = new SensorCalculationDirectPPM(portNumber);  
  else if (portConfig["s"]["cf"] == "DIRECT_NONE")
    calc = new SensorCalculationDirectNone(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_METER")
    calc = new SensorCalculationCalcAltitude(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_RAW_PERCENT")
    calc = new SensorCalculationRawToPercent(portConfig["c1"], portConfig["c2"], portNumber);
  else if (portConfig["s"]["cf"] == "RAW")
    calc = new SensorCalculationRaw(portNumber);

  if(calc==NULL)
  {
    Serial.println("Unsupported Calculation Formula, please check Firmware Version.");
    return;
  }

  int refreshInterval = portConfig["ri"];
  if (refreshInterval == 0)
  {
    Serial.println("No Sensor Refresh Update interval set, falling back to default (10sec).");
    refreshInterval = 10000;
  }

  int postDataInterval = portConfig["pdi"];
  if (postDataInterval == 0)
  {
    Serial.println("No Sensor Post Data Update interval set, falling back to default (60sec).");
    postDataInterval = 60000;
  }

  Serial.print(portConfig["n"].asString());
  Serial.print(" of type: ");
  Serial.println(portConfig["et"].asString());

  if (portConfig["et"] == "ADS1015" || portConfig["et"] == "ADS1115")
  {
    addSensor(new Ads1x15 (portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["et"], portConfig["ec1"], portConfig["ch"], portConfig["c1"], portConfig["c2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "DALLAS" || portConfig["et"] == "DS18B20")
  {
    uint8_t port = translateGPIOPort(portConfig["ec1"]);
    addSensor(new SensorDallas(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], port, portConfig["ch"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "DHT11" || portConfig["et"] == "DHT21" || portConfig["et"] == "DHT22")
  {
    uint8_t port = translateGPIOPort(portConfig["ec1"]);
    if(port>=0)
    {
      addSensor(new SensorDHT(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["et"], port, refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
    }
  }
  else if (portConfig["et"] == "BME280" || portConfig["et"] == "BMP280")
  {    
    addSensor(new SensorBMx280(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "BME680")
  {    
    addSensor(new SensorBME680(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "MAX44009")
  {    
    addSensor(new SensorMax44009(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  else if (portConfig["et"] == "BH1750")
  {    
    addSensor(new SensorBH1750(portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["ec1"], portConfig["ec2"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
  }
  
}

void configurePort(int portNumber, JsonObject& portConfig) {
  
  String port = portConfig["p"];
  
  Serial.println("Configure Onboard Port:" + port);

  // portConfig.prettyPrintTo(Serial);

  SensorCalculation* calc = NULL;

  if (portConfig["s"]["cf"] == "APPROX_QUAD")
    calc = new SensorCalculationApproxQuad(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portConfig["s"]["cv3"], portConfig["s"]["cv4"], portNumber);
  else if (portConfig["s"]["cf"] == "PT1001000")
    calc = new SensorCalculationPT1001000(portConfig["s"]["cv1"], portNumber);
  else if (portConfig["s"]["cf"] == "DUAL_NTC")
    calc = new SensorCalculationDualNtc(portConfig["s"]["cv1"], portConfig["s"]["cv2"], portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KELVIN")
    calc = new SensorCalculationDirectKelvin(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_CELSIUS")
    calc = new SensorCalculationDirectCelsius(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_PERCENT")
    calc = new SensorCalculationDirectPercent(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_HEKTOPASCAL")
    calc = new SensorCalculationDirectHektoPascal(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_METER")
    calc = new SensorCalculationDirectMeter(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUX")
    calc = new SensorCalculationDirectLux(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_LUMEN")
    calc = new SensorCalculationDirectLumen(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_OHM")
    calc = new SensorCalculationDirectOhm(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_KOHM")
    calc = new SensorCalculationDirectKOhm(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_PPM")
    calc = new SensorCalculationDirectPPM(portNumber);
  else if (portConfig["s"]["cf"] == "DIRECT_NONE")
    calc = new SensorCalculationDirectNone(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_METER")
    calc = new SensorCalculationCalcAltitude(portNumber);
  else if (portConfig["s"]["cf"] == "CALC_RAW_PERCENT")
    calc = new SensorCalculationRawToPercent(portConfig["c1"], portConfig["c2"], portNumber);
  else if (portConfig["s"]["cf"] == "RAW")
    calc = new SensorCalculationRaw(portNumber);
    
  if(calc==NULL)
  {
    Serial.println("Unsupported Calculation Formula, please check Firmware Version.");
    return;
  }

  int refreshInterval = portConfig["ri"];
  if (refreshInterval == 0)
  {
    Serial.println("No Sensor Refresh Update interval set, falling back to default (10sec).");
    refreshInterval = 10000;
  }

  int postDataInterval = portConfig["pdi"];
  if (postDataInterval == 0)
  {
    Serial.println("No Sensor Post Data Update interval set, falling back to default (60sec).");
    postDataInterval = 60000;
  }
  
  if (port.equals("ADC"))
  {
    if(portConfig["c1"]!=0 && portConfig["c2"]==0)
    {
      addSensor(new SensorAnalogue (portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], portConfig["c1"], refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
    }
    else
    {
      addSensor(new SensorAnalogue (portConfig["id"], portConfig["c"], portConfig["sn"], portConfig["n"], 0, refreshInterval, postDataInterval, portConfig["s"]["svt"], calc));
    }
  }
}

void addSensor(Sensor *sensor)
{
  if (sensorCount <= maxSensorCount)
  {
    sensors[sensorCount] = sensor;
    sensorCount++;
  }
}

void loopSensor(int currentTimeMs) {

  Data *data[maxSensorCount];
  int dataCount = 0;

  if(wasDisconnected)
  {
    if(WiFi.status() == WL_CONNECTED)
    {
      if(display!=NULL)
        display->drawConnected(true);
      wasDisconnected = false;
    }
  }

  for (int i = 0; i < sensorCount; i++)
  {
    Data* newData = sensors[i]->trySensorRead(currentTimeMs, sensorCycle);
    yield();
    if (newData != NULL)
    {
       data[dataCount] = newData;
       dataCount++;
    }
    yield();
  }

  sensorCycle++;
  if(sensorCycle==1000)
    sensorCycle = 10; //Cycles 1->9 only run once at startup!

  boolean submitSuccess = false;
  int retry = 1;

  while(retry < 10 && !submitSuccess && dataCount>0)
  {
    boolean abortDelay = false;
    yield();

    if(WiFi.status() == WL_CONNECTED)
    {
      submitSuccess = postSensorData(data, dataCount);
      abortDelay = submitSuccess;
      yield();
      if(mqtt!=NULL)
      {
        mqtt->publishSensorData(data, dataCount);
      }
    }
    else
    {
      wasDisconnected = true;
      if(display!=NULL)
        display->drawDisconnected(true);

      if(WiFi.status()==WL_DISCONNECTED)
      {
        Serial.println("Not Connected - Trying to reconnect to WiFi..");
        WiFi.reconnect();
        int connRes = WiFi.waitForConnectResult();
          if(connRes==WL_CONNECTED)
          {
            Serial.println("Reconnect success!");
            abortDelay = true;
          }
      } 
      yield();
    }

    if(!abortDelay)
    {
      int delayTime=100*retry;  
      Serial.println("Communication Issues - Retrying in "+String(delayTime)+"ms ");
      yield();
      delay(delayTime);
    }
    retry++;
  }

// If dataCount = 0 this is only a display refresh cycle - the disconnected symbol has to be drawn again, otherwise it looks like the device has a connection
  if(dataCount==0)
  {
      if((serverError || (WiFi.status()!=WL_CONNECTED)) && display!=NULL)
      {
        wasDisconnected=true;
        display->drawDisconnected(true);
      }
        
      yield();
  }
  else  // dealloc the data array from memory
  {
    for (int i = 0; i < dataCount; i++)
      {
        delete data[i];
        data[i] = NULL;
      }
  }

  long time = nextSensorDue-millis();

  if(time>10000)
  {
    trySleep((time-3000)*1000);
  }
}

void trySleep(long microseconds)
{
    // Serial.println("Power Mode... "+String(powerMode));

    if(powerMode==2 && microseconds>0)
    {
      doPowerSaving();
      Serial.println("Going to deep sleep for "+String(microseconds));
      ESP.deepSleep(microseconds);
    }
}

void doPowerSaving() {

    if(powerSavePort!=NULL && !powerSavePort.length()>0)
    {
        Serial.println("Switching Power Save Port: "+powerSavePort);

        int port = translateGPIOPort(powerSavePort);

        if(port>=0)
        {
          pinMode(port, OUTPUT);
          digitalWrite(port, invPowerSavePort);
        }

    }

    yield();
}

void doPowerSavingInit(boolean doDelay) {

  if(powerSavePort!="")
  {
    Serial.println("Init Power Save Port: "+powerSavePort);

    int port = translateGPIOPort(powerSavePort);

    if(port>=0)
    {
      pinMode(port, OUTPUT);
      digitalWrite(port, !invPowerSavePort);
    }
    else
    {
      Serial.println("No valid Power Save Port defined: "+powerSavePort);
    }
  }
  else
  {
    Serial.println("No Power Save Port defined!");
  }

  yield();

  if(doDelay && powerOnDelay>0)
  {
    Serial.println("Delaying Power on for: "+String(powerOnDelay));
    delay(powerOnDelay);
  }

}

uint8_t translateGPIOPort(String gpioPort)
{
  if(gpioPort=="0")
    return 0;
  if(gpioPort=="1")
    return 1;
  if(gpioPort=="2")
    return 2;
  if(gpioPort=="3")
    return 3;
  if(gpioPort=="4")
    return 4;
  if(gpioPort=="5")
    return 5;
  if(gpioPort=="6")
    return 6;
  if(gpioPort=="7")
    return 7;
  if(gpioPort=="8")
    return 8;
  if(gpioPort=="9")
    return 9;
  if(gpioPort=="10")
    return 10;
  if(gpioPort=="11")
    return 11;
  if(gpioPort=="12")
    return 12;
  if(gpioPort=="13")
    return 13;
  if(gpioPort=="14")
    return 14;
  if(gpioPort=="15")
    return 15;
  if(gpioPort=="16")
    return 16;  
  
  return -1;
}

boolean postSensorData(Data* data[], int dataCount)
{
  HTTPClient httpClient;

  String requestDataString = "";

  for (int i = 0; i < dataCount; i++)
  {
    if (i == 0)
      requestDataString = data[i]->getRequestString();
    else
      requestDataString = requestDataString + "," + data[i]->getRequestString();
  }

  String urlString = bridgeURL + "/" + apiVersion + "/data/" + getUUID() + "/" + requestDataString;

  if(urlString.startsWith("https://hub"))
    httpClient.begin(urlString, ssl_fingerprint_prod);
  else if(urlString.startsWith("https://test"))
    httpClient.begin(urlString, ssl_fingerprint_test);
  else
    httpClient.begin(urlString);
  
  int httpCode = httpClient.PUT("");

  if (httpCode == HTTP_CODE_UPGRADE_REQUIRED)
  {
    httpClient.end();
    restart();
    return true;
  }
  else if (httpCode == HTTP_CODE_OK)
  {
    if(serverError || wasDisconnected)
    {
      serverError = false;
      wasDisconnected = false;
      if(display!=NULL)
        display->drawConnected(true);
    }

    String payload = httpClient.getString();

    if(payload!=NULL && displayType!=0)
    {
      if(payload.equals("ed") && displayEnabled==false)
      {
        Serial.println("Enabling display!");
        displayEnabled=true;
        boolean rotateDisplay = (displayRotation == 180);
        if(display==NULL)
          display = new Display(rotateDisplay, displayType,"",i2cSDAPort,i2cSCLPort);
        storeDisplayAndPowerConfig(false);
      }
      else if(payload.equals("dd") && displayEnabled==true)
      {
        Serial.println("Disabling display!");
        displayEnabled=false;
        if(display!=NULL)
          display->clear(true);
        storeDisplayAndPowerConfig(false);
      }
    }

    httpClient.end();
    return true;
  }
  else
  {
    if(display!=NULL)
    {
      wasDisconnected=true;
      display->drawDisconnected(true);
    }
      
    serverError = true;
    Serial.println("Server Error: "+String(httpCode));

    Serial.println("Server Error: "+httpClient.errorToString(httpCode));
    Serial.println(WiFi.status());

    httpClient.end();
  }
  return false;
}

void checkStatus() 
{
  if(WiFi.status() != WL_CONNECTED)
  {
    wasDisconnected=true;
    if(display!=NULL)
      display->drawDisconnected(true);
    Serial.println("Not Connected");
  }
}