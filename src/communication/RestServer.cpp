/**************************************************************************/
/*!
    @file     RestServer.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensatio ESP8266 firmware is used to connect ESP8266 based hardware
    with the Sensatio Cloud and the Sensatio apps.

    ----> https://www.sensatio.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v41 - Renamed Display Class to support more types
    v33 - Improved MQTT Setup Routine
    v32 - Added MQTT Support!
    v29 - First Public Release
*/
/**************************************************************************/

#include "RestServer.h"

ESP8266WebServer http_rest_server(80);

extern State state;
extern IPAddress apIP;
extern IPAddress netMsk;
extern const byte DNS_PORT;
extern const char *myHostname;
extern DisplayOLED128* display;
extern String bridgeURL;
extern MQTT* mqtt;
extern boolean enableMQTT;

void startRestServer() {
  Serial.println("startRestServer");
  configRestServerRouting();
  http_rest_server.begin();
}

void loopRestserver() {
  http_rest_server.handleClient();
}

void configRestServerRouting() {
  Serial.println("configRestServerRouting");
  if(state==Connected_WiFi)
  {
    Serial.println("Connected_WiFi");
    http_rest_server.on("/resetwifi", HTTP_POST, resetBridgeConfig);            //Legacy!
    http_rest_server.on("/resetbridge", HTTP_GET, resetBridgeConfig);           //Remove SSID and Password and restart Device to use SoftAP
    http_rest_server.on("/uuid", HTTP_GET, restGetUuid);              //Welcome and return Device UUID if Hint is correct
    http_rest_server.on("/updateName", HTTP_PUT, restUpdateName);
    http_rest_server.on("/initMqtt", HTTP_PUT, initMqtt);
    http_rest_server.on("/restart", HTTP_GET,tryRestart);
    http_rest_server.on("/identify", HTTP_GET,tryIdentify);
    state=Register;
  }
  if(state==Init_Setup)
  {
    Serial.println("Setup");
    http_rest_server.on("/networks", HTTP_GET, returnNetworkList);    //Return list of available Networks
    http_rest_server.on("/", HTTP_GET, presentWiFiSetupScreen);       //Present Wifi Setup Screen
    http_rest_server.on("/wifi", HTTP_GET, presentWiFiSetupScreen);   //Present Wifi Setup Screen
    http_rest_server.on("/setupwifi", HTTP_POST, setupBridgeConfig);             //Legacy! 
    http_rest_server.on("/setupbridge", HTTP_POST, setupBridgeConfig);           //Set SSID and Password and restart Device to use it
    http_rest_server.on("/manualsetup", HTTP_POST, setupBridgeConfigManual);
    http_rest_server.on("/restart", HTTP_GET,tryRestart);
    http_rest_server.on("/identify", HTTP_GET,tryIdentify);
    http_rest_server.on("/generate_204", HTTP_GET, androidCaptiveResponse);
    http_rest_server.on("/hotspot-detect.html", HTTP_GET, handleAppleCaptivePortal);
    http_rest_server.onNotFound(handleNotFound);
  }
}

void handleAppleCaptivePortal() {
    String Page = F("<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>");

    http_rest_server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http_rest_server.sendHeader("Pragma", "no-cache");
    http_rest_server.sendHeader("Expires", "-1");
    http_rest_server.send(200, "text/html", Page);
    return;
}

void androidCaptiveResponse() {

  Serial.println("REST GET /generate_204 - Android Captive Response");
  
  http_rest_server.send(204);
  http_rest_server.client().stop();  
}

void captiveResponse() {

  Serial.println("REST GET /captiveResponse - Captive Response");
  
  http_rest_server.send(200);
  http_rest_server.client().stop();  
}

void tryIdentify() {

  String requestUuid = http_rest_server.arg("uuid");

  Serial.println("REST GET /identify for uuid: " + requestUuid);

  if (requestUuid == "")
  {
    Serial.println("identify failed");
    http_rest_server.send(400);
  }
  else
  {
    if (getUUID()==requestUuid)
    {
      http_rest_server.send(200);
      if(display!=NULL)
        display->blinkArrow(5);
    }
    else
    {
      Serial.println("identify failed");
      http_rest_server.send(404);
    }
      
  }
  
}

void restGetUuid() {
  String hint = http_rest_server.arg("hint");

  Serial.println("REST GET /uuid for hint: " + hint);

  if (hint == "")
    http_rest_server.send(400);
  else
  {
    String uuid = getUUID();
    if (uuid.startsWith(hint + "-"))
      http_rest_server.send(200, "text/plain", getUUID());
    else
      http_rest_server.send(404);
  }
}

void restUpdateName() {

  String requestUUID = http_rest_server.arg("uuid");
  String newName = http_rest_server.arg("plain");

  if(requestUUID!=NULL && requestUUID.equals(getUUID()) && newName!=NULL && newName.length()>0)
  {
    Serial.println("Setting new bridge name: "+newName);

    EEPROM.begin(289);
    EEPROM.write(221, 221);
    EEPROM.write(222, 222);

    char nameChars[25];
    newName.toCharArray(nameChars, 25);
    EEPROM.put(223, nameChars);
    EEPROM.end();

    http_rest_server.send(200);
    http_rest_server.client().stop();

    yield();
  }
  else
  {
    http_rest_server.send(400);
  }
}

void initMqtt() {

  Serial.println("initMqtt");

  String requestUUID = http_rest_server.arg("uuid");

  if(requestUUID!=NULL && requestUUID.equals(getUUID()))
  {
      String mqttBrokerUrl = http_rest_server.arg("brokerUrl");
      String mqttBrokerPortString = http_rest_server.arg("brokerPort");
      String mqttUsername = http_rest_server.arg("username");
      String mqttPassword = http_rest_server.arg("password");

      EEPROM.begin(398);

      if(mqttBrokerUrl!=NULL && mqttBrokerUrl.length()>0)
      {
        Serial.println("Setting MQTT Broker Url: "+mqttBrokerUrl+" and port: "+mqttBrokerPortString);

        EEPROM.write(314, 114);
        EEPROM.write(315, 115);

        yield();

        char mqttBrokerUrlChars[36];
        mqttBrokerUrl.toCharArray(mqttBrokerUrlChars, 36);
        EEPROM.put(316, mqttBrokerUrlChars);

        if(mqttBrokerPortString==NULL || mqttBrokerPortString.length()==0)
        {
          mqttBrokerPortString = "1883";
        }

        char mqttBrokerPortChars[5];
        mqttBrokerPortString.toCharArray(mqttBrokerPortChars,5);
        long mqttBrokerPort = atol(mqttBrokerPortChars);
        EEPROM.put(352, mqttBrokerPort);

        if(mqttUsername!=NULL && mqttUsername.length()>0 && mqttPassword!=NULL && mqttPassword.length()>0)
        {
          Serial.println("Setting mqttAuth User: "+mqttUsername + " and password.");

          EEPROM.write(356, 156);
          EEPROM.write(357, 157);

          yield();

          char mqttUsernameChars[20];
          mqttUsername.toCharArray(mqttUsernameChars, 20);
          EEPROM.put(358, mqttUsernameChars);

          char mqttPasswordChars[20];
          mqttPassword.toCharArray(mqttPasswordChars, 20);

          EEPROM.put(378, mqttPasswordChars);

          yield();
        }
        else
        {
          Serial.println("Disabling MQTT Auth.");

          EEPROM.write(356, 0);
          EEPROM.write(357, 0);

          mqtt = NULL;

          yield();
        }
        enableMQTT=true;

      }
      else
      {
        Serial.println("Disabling MQTT.");

        EEPROM.write(314, 0);
        EEPROM.write(315, 0);
        EEPROM.write(356, 0);
        EEPROM.write(357, 0);

        mqtt = NULL;
        enableMQTT=false;

        yield();
      }

      EEPROM.end();

      yield();

      tryInitMQTT();

      yield();

      http_rest_server.send(200);
      http_rest_server.client().stop();

      return;
  }

  Serial.println("Invalid/missing Bridge Identifier!");
  http_rest_server.send(401);
  http_rest_server.client().stop();

}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  http_rest_server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  http_rest_server.sendHeader("Pragma", "no-cache");
  http_rest_server.sendHeader("Expires", "-1");
  http_rest_server.send(200, "text/plain", "");
  http_rest_server.client().stop();
}

bool captivePortal() {
  if (!isIp(http_rest_server.hostHeader()) && http_rest_server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    http_rest_server.sendHeader("Location", String("http://") + toStringIp(http_rest_server.client().localIP()), true);
    http_rest_server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    http_rest_server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleRoot() {

  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  
  http_rest_server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  http_rest_server.sendHeader("Pragma", "no-cache");
  http_rest_server.sendHeader("Expires", "-1");

  String Page;
  Page += F(
            "<html><head></head><body>"
            "<h1>HELLO WORLD!!</h1>");
  if (http_rest_server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + "Setup10ideen" + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + "test" + F("</p>");
  }
  Page += F(
            "<p>You may want to <a href='/wifi'>config the wifi connection</a>.</p>"
            "</body></html>");

  http_rest_server.send(200, "text/html", Page);
}

void returnNetworkList() {
  Serial.println("REST GET /networks");

  int numberOfNetworks = WiFi.scanNetworks();

  String networks[numberOfNetworks];

  String wifiResponse = "[";

  StaticJsonBuffer<1000> jsonBuffer;
  JsonArray& root = jsonBuffer.createArray();

  for (int i = 0; i < numberOfNetworks; i++) {

    JsonObject& network = jsonBuffer.createObject();
    network["name"] = WiFi.SSID(i); // @suppress("Invalid arguments")
    network["enc"] =  (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? false : true;
    network["rssi"] = WiFi.RSSI(i); // @suppress("Invalid arguments")
    
    root.add(network);
  }

//  root.printTo(Serial);

  jsonBuffer.clear();

  char jsonChar[1000];
  root.printTo((char*)jsonChar, root.measureLength() + 1);
  
  http_rest_server.send(200, "application/json", jsonChar);
}

void presentWiFiSetupScreen() {
  Serial.println("REST GET /wifi");

  int numberOfNetworks = WiFi.scanNetworks();

  String networks[numberOfNetworks];

  for (int i = 0; i < numberOfNetworks; i++) {

    networks[i] = WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "ddBm)"; // @suppress("Invalid arguments")
//    Serial.println("Network: " + networks[i]);

  }

  restoreBridgeConfig();
  String wifiResponse = "<html><body style='width:600;margin-left:auto;margin-right:auto;font-family:verdana'><br/><br/><h1>Welcome to your Sensate Bridge</h1><br/><br/><form action='/manualsetup' method='post'><table width='100%'><tr><td colspan='2'><h2>Wireless LAN Configuration</h2></td></tr><tr><td align='right'><b>SSID/Name</b></td><td><select name='ssid'>";
  for (int i = 0; i < numberOfNetworks; i++) {
    wifiResponse = wifiResponse + "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "ddBm)</option>"; // @suppress("Invalid arguments")
  }

  wifiResponse = wifiResponse + "</select></td></tr><tr><td align='right'><b>Password</b></td><td><input type='password' name='password' value='' maxlength='63' placeholder='leave empty if unencrypted' size='30'/></td></tr><tr><td colspan='2'>&nbsp;</td></tr><tr><td colspan='2'><h2>Service Configuration</h2></td></tr><tr><td align='right'><b>SensorHub Location</b></td><td><input type='text' name='url' value='https://hub.sensate.cloud' maxlength='100' placeholder='Service Backend URL' size='40'/></td></tr><tr><td align='right'><b>Device Name</b></td><td><input type='text' name='identifier' value='Bridge' maxlength='100' placeholder='Bridge Name' size='40'/></td></tr><tr><td colspan='2'>&nbsp;</td></tr><tr><td colspan='2'>&nbsp;</td></tr><tr><td colspan='2' align='center'><input type='submit' value='Save and restart' style='height: 25px; width: 100px;'/></td></tr></table></form></body></html>";

  http_rest_server.send(200, "text/html", wifiResponse);

}

void tryRestart() {

  String requestUuid = http_rest_server.arg("uuid");

  Serial.println("REST GET /restart for uuid: " + requestUuid);

  if (requestUuid == "")
  {
    Serial.println("restart failed");
    http_rest_server.send(400);
  }
  else
  {
    if (getUUID()==requestUuid)
    {
      http_rest_server.send(200);
      restart();
    }
    else
    {
      Serial.println("restart failed");
      http_rest_server.send(404);
    }
      
  }
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

bool isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}
