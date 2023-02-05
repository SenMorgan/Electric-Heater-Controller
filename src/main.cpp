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
void callback(String topic, byte *payload, unsigned int length);
void send_power_state(const char *topic, uint8_t state);
void send_pwm_state(const char *topic, int pwm);

struct heater_state_t
{
    uint8_t power_state;
    int pwm;
};

heater_state_t heater_1 = {0, 0};
heater_state_t heater_2 = {0, 0};
heater_state_t heater_3 = {0, 0};

static char send_buff[20];

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

    // Set PWM frequency to 200 Hz as per IR3310 datasheet
    analogWriteFreq(200);

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
    mqttClient.setCallback(callback);
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
        mqttClient.subscribe(MQTT_SUBSCRIBE_TOPIC);
        Serial.println("Successfully connected to " MQTT_SERVER);

        return 1;
    }
    Serial.println("Can't connect to MQTT server...");
    delay(1000);

    return 0;
}

/**
 * @brief Publish data to server
 */
void publish_data(void)
{
    static uint32_t last_publish_timestamp;

    if (millis() - last_publish_timestamp > MQTT_PUBLISH_INTERVAL)
    {
        last_publish_timestamp = millis();
        mqttClient.publish(MQTT_AVAILABILITY_TOPIC, MQTT_AVAILABILITY_MESSAGE);
        sprintf(send_buff, "%ld sec", millis() / 1000);
        mqttClient.publish(MQTT_STATE_UPTIME, send_buff);

        // Send PWM states
        send_pwm_state(MQTT_STATE_PWM_1, heater_1.pwm);
        send_pwm_state(MQTT_STATE_PWM_2, heater_2.pwm);
        send_pwm_state(MQTT_STATE_PWM_3, heater_3.pwm);
        // Send power states
        send_power_state(MQTT_STATE_POWER_1, heater_1.power_state);
        send_power_state(MQTT_STATE_POWER_2, heater_2.power_state);
        send_power_state(MQTT_STATE_POWER_3, heater_3.power_state);
    }
}

void set_pwm(uint8_t heater_element, int pwm)
{
    analogWrite(heater_element, pwm);
}

void send_pwm_state(const char *heater_element, int pwm)
{
    sprintf(send_buff, "%d", pwm);
    mqttClient.publish(heater_element, send_buff);
}

void send_power_state(const char *heater_element, uint8_t power)
{
    sprintf(send_buff, "%d", power);
    mqttClient.publish(heater_element, send_buff);
}

/**
 * @brief Callback function for MQTT client
 *
 * @param topic Topic of the message
 * @param payload Payload of the message
 * @param length Length of the payload
 */
void callback(String topic, byte *payload, unsigned int length)
{
    String msgString = "";
    for (uint16_t i = 0; i < length; i++)
        msgString += (char)payload[i];

    Serial.println("Message arrived in topic: " + topic);
    Serial.println("Message: " + msgString);

    // Reset board when received "reset" message
    if (topic == (MQTT_CMD_TOPIC_RESET))
    {
        // We need to add reset_toggled flag, to prevent resetting the board twice
        if (msgString == MQTT_CMD_ON)
        {
            ESP.restart();
        }
    }
    // Set power for heater element 1
    else if (topic == (MQTT_CMD_TOPIC_POWER_1))
    {
        if (msgString == MQTT_CMD_ON)
        {
            set_pwm(HEATER_ELEMENT_1, heater_1.pwm);
            heater_1.power_state = HIGH;
        }
        else if (msgString == MQTT_CMD_OFF)
        {
            set_pwm(HEATER_ELEMENT_1, 0);
            heater_1.power_state = LOW;
        }
        send_power_state(MQTT_STATE_POWER_1, heater_1.power_state);
        Serial.println("Set power for heater element 1 to " + msgString);
    }
    // Set power for heater element 2
    else if (topic == (MQTT_CMD_TOPIC_POWER_2))
    {
        if (msgString == MQTT_CMD_ON)
        {
            set_pwm(HEATER_ELEMENT_2, heater_2.pwm);
            heater_2.power_state = HIGH;
        }
        else if (msgString == MQTT_CMD_OFF)
        {
            set_pwm(HEATER_ELEMENT_2, 0);
            heater_2.power_state = LOW;
        }
        send_power_state(MQTT_STATE_POWER_2, heater_2.power_state);
        Serial.println("Set power for heater element 2 to " + msgString);
    }
    // Set power for heater element 3
    else if (topic == (MQTT_CMD_TOPIC_POWER_3))
    {
        if (msgString == MQTT_CMD_ON)
        {
            set_pwm(HEATER_ELEMENT_3, heater_3.pwm);
            heater_3.power_state = HIGH;
        }
        else if (msgString == MQTT_CMD_OFF)
        {
            set_pwm(HEATER_ELEMENT_3, 0);
            heater_3.power_state = LOW;
        }
        send_power_state(MQTT_STATE_POWER_3, heater_3.power_state);
        Serial.println("Set power for heater element 3 to " + msgString);
    }

    // Set PWM duty cycle for heater element 1
    else if (topic == (MQTT_CMD_TOPIC_PWM_1))
    {
        heater_1.pwm = msgString.toInt();
        set_pwm(HEATER_ELEMENT_1, heater_1.pwm);
        send_pwm_state(MQTT_STATE_PWM_1, heater_1.pwm);
        Serial.println("Set PWM duty cycle for heater element 1 to " + msgString);
    }
    // Set PWM duty cycle for heater element 2
    else if (topic == (MQTT_CMD_TOPIC_PWM_2))
    {
        heater_2.pwm = msgString.toInt();
        set_pwm(HEATER_ELEMENT_2, heater_2.pwm);
        send_pwm_state(MQTT_STATE_PWM_2, heater_2.pwm);
        Serial.println("Set PWM duty cycle for heater element 2 to " + msgString);
    }
    // Set PWM duty cycle for heater element 3
    else if (topic == (MQTT_CMD_TOPIC_PWM_3))
    {
        heater_3.pwm = msgString.toInt();
        set_pwm(HEATER_ELEMENT_3, heater_3.pwm);
        send_pwm_state(MQTT_STATE_PWM_3, heater_3.pwm);
        Serial.println("Set PWM duty cycle for heater element 3 to " + msgString);
    }
}
