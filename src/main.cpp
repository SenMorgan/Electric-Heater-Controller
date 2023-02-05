#include <Arduino.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include "def.h"

const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWD;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Helper functions declarations
uint8_t reconnect(void);
void publish_data(void);

void setup(void)
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(HEATER_ELEMENT_1, OUTPUT);
    pinMode(HEATER_ELEMENT_2, OUTPUT);
    pinMode(HEATER_ELEMENT_3, OUTPUT);

    digitalWrite(HEATER_ELEMENT_1, LOW);
    digitalWrite(HEATER_ELEMENT_2, LOW);
    digitalWrite(HEATER_ELEMENT_3, LOW);

    // Enable status LED to indicate that device is booting
    digitalWrite(LED_BUILTIN, LOW);

    // WiFi initializing
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    WiFi.hostname(HOSTNAME);

    // Arduino OTA initializing
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    ArduinoOTA.begin();
    ArduinoOTA.onProgress([](uint16_t progress, uint16_t total)
                          { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); });
    ArduinoOTA.onEnd([]()
                     { digitalWrite(LED_BUILTIN, HIGH); });

    // MQTT initializing
    mqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT);
    Serial.println("Connecting to MQTT server...");
    reconnect();

    // Disable status LED to indicate that device is ready
    digitalWrite(LED_BUILTIN, HIGH);
}

// Function that is looped forever
void loop(void)
{
    ArduinoOTA.handle();

    uint8_t connectedToServer = mqttClient.loop();

    if (connectedToServer)
    {
        publish_data();
    }
    else
    {
        if (reconnect())
        {
            publish_data();
        }
    }

    yield();
}

/**
 * @brief Connecting to MQTT server
 *
 * @return true if successfully reconnected to MQTT server, otherwise false
 */
uint8_t reconnect(void)
{
    if (mqttClient.connect(HOSTNAME, MQTT_LOGIN, MQTT_PASSWORD,
                           MQTT_WILL_TOPIC, MQTT_QOS, MQTT_RETAIN, MQTT_WILL_MESSAGE))
    {
        Serial.println("Successfully connected to " MQTT_SERVER);
        return 1;
    }
    Serial.println("Can't connect to MQTT server...");
    return 0;
}

/**
 * @brief Publish data to server
 */
void publish_data(void)
{
    static char buff[20];

    mqttClient.publish(MQTT_AVAILABILITY_TOPIC, MQTT_AVAILABILITY_MESSAGE);
    sprintf(buff, "%ld sec", millis() / 1000);
    mqttClient.publish(MQTT_UPTIME_TOPIC, buff);

    Serial.println("Data were sent");
}