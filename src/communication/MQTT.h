/**************************************************************************/
/*!
    @file     MQTT.h
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v32 - Added MQTT Support!
*/
/**************************************************************************/

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#ifndef _MQTT_h_
#define _MQTT_h_

#include "../controller/UUID.h"
#include "Data.h"

class MQTT {
  private:
    unsigned long lastMillis;
    String _brokerUrl;
    long _brokerPort;
    String _username;
    String _password;
    WiFiClient espClient;
    PubSubClient* pubSubClient;
    String clientId;
    
  public:
    MQTT (char*, long);
    MQTT (char*, long, String, String);
    bool connect(void);
    void loop(void);
    void publishSensorData(Data* data[], int);
    void publishForAutoDiscovery(Sensor*);
};

#endif