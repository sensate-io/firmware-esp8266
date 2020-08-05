/**************************************************************************/
/*!
    @file     MQTT.cpp
    @author   M. Fegerl (Sensate Digital Solutions GmbH)
    @license  GPL (see LICENSE file)
    The Sensate ESP8266 firmware is used to connect ESP8266 based hardware 
    with the Sensate Cloud and the Sensate apps.

    ----> https://www.sensate.io

    SOURCE: https://github.com/sensate-io/firmware-esp8266.git

    @section  HISTORY
    v34 - Added Generic Analog Sensor Support
    v33 - Added Digital Sensor Switch Support
    v32 - Added MQTT Support!
*/
/**************************************************************************/

#include "MQTT.h"

extern unsigned long currentMillis;
boolean enableMQTT;
MQTT *mqtt;

MQTT::MQTT (char* brokerUrl, long brokerPort, String username, String password)
{
  _username = username;
  _password = password;
  _brokerPort = brokerPort;
  _brokerUrl = String(brokerUrl);

  espClient = WiFiClient();
  pubSubClient = new PubSubClient(espClient);

  clientId = "Sensate-"+getUUID().substring(0,7);

  pubSubClient->setServer(_brokerUrl.c_str(), _brokerPort);

  lastMillis = 0;
}

MQTT::MQTT (char* brokerUrl, long brokerPort)
{
  _username = "NULL";
  _password = "NULL";

  clientId = "Sensate-"+getUUID().substring(0,7);

  pubSubClient = new PubSubClient(espClient);
  pubSubClient->setServer(brokerUrl, brokerPort);

}

bool MQTT::connect()
{
    Serial.println("Connecting to broker with clientId: "+clientId);

    if(_username=="NULL" && _password=="NULL")
    {
        Serial.println("Connecting to MQTT broker...");
        
        if (pubSubClient->connect(clientId.c_str())) 
        {
            Serial.println("Connected to MQTT broker");  
            return true;
        } 
        else 
        {
            Serial.print("Connection to MQTT Broker failed with state ");
            Serial.print(pubSubClient->state());
            return false;
        }
    }  
    else
    {
        Serial.println("Connecting to MQTT broker with credentials...");

        if (pubSubClient->connect(clientId.c_str(), _username.c_str(), _password.c_str())) 
        {
            Serial.println("Connected to MQTT broker");  
            return true;
        } 
        else 
        {
            Serial.print("Connection to MQTT Broker failed with state ");
            Serial.print(pubSubClient->state());
            return false;
        }
    } 
}

void MQTT::publishForAutoDiscovery(Sensor* sensor)
{
    String pTopic;
    
    if(sensor->isBinary())
    {
        pTopic = "homeassistant/binary_sensor/"+clientId+"/"+String(sensor->getId())+"/config";
    }
    else
     pTopic = "homeassistant/sensor/"+clientId+"/"+String(sensor->getId())+"/config";

    String category = sensor->getCategory();
    String pPayload;

    if(category==NULL)
        category = "Unnamed";

    if(sensor->getMqttClass()=="plevel" || sensor->getMqttClass()=="resistance" || sensor->getMqttClass()=="altitude" || sensor->getMqttClass()=="flux" || sensor->getMqttClass()=="" || sensor->getMqttClass()=="raw" || sensor->getMqttClass()=="voltage")
    {
             pPayload = "{\"name\": \""+sensor->getName()+"\", \"state_topic\": \"Sensate/"+category+"/"+sensor->getName()+"/value\"";
             if(sensor->isBinary())
                pPayload = pPayload + ", \"payload_on\": \"1\", \"payload_off\": \"0\"}";
             else
             {
                 pPayload = pPayload + ", \"unit_of_measurement\": \""+sensor->getMqttUnit()+"\"}";
             }
    }   
    else
        pPayload = "{\"name\": \""+sensor->getName()+"\", \"device_class\": \""+sensor->getMqttClass()+"\", \"state_topic\": \"Sensate/"+category+"/"+sensor->getName()+"/value\", \"unit_of_measurement\": \""+sensor->getMqttUnit()+"\"}";
        
    pubSubClient->publish(pTopic.c_str(), pPayload.c_str());
}

void MQTT::loop()
{
    pubSubClient->loop();
}

void MQTT::publishSensorData(Data* data[], int dataCount)
{
    boolean republish = false;

    if (!pubSubClient->connected()) {
        Serial.println("Trying reconnecting to MQTT broker...");
        republish=connect();
    }

    if((lastMillis == 0) || (currentMillis>lastMillis+300000))
    {
        Serial.println("Republish MQTT Sensors (once every 5min)");
        republish=true;
        lastMillis = currentMillis;
    }

    for (int i = 0; i < dataCount; i++)
    {
        if(republish)
        {
            publishForAutoDiscovery(data[i]->getSensor());
            delay(250);
        }

        String cat = data[i]->getSensor()->getCategory();
        if(cat.equals("")) 
            cat="Unnamed";
        String name = data[i]->getSensor()->getName();
        String vTopic = "Sensate/"+cat+"/"+name+"/value";
        pubSubClient->publish(vTopic.c_str(), data[i]->getValueString().c_str());
    }
}
