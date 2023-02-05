/**
 * @file def.h
 * @author SenMorgan https://github.com/SenMorgan
 * @date 2023-02-05
 *
 * @copyright Copyright (c) 2023 Sen Morgan
 *
 */

#ifndef _DEF_H_
#define _DEF_H_

// MQTT definitions
#define DEFAULT_TOPIC             "/" HOSTNAME "/"
#define MQTT_WILL_TOPIC           DEFAULT_TOPIC "availability"
#define MQTT_QOS                  1
#define MQTT_RETAIN               1
#define MQTT_WILL_MESSAGE         "offline"
#define MQTT_SUBSCRIBE_TOPIC      DEFAULT_TOPIC "set/#"
#define MQTT_PUBLISH_TOPIC        DEFAULT_TOPIC "state"
#define MQTT_AVAILABILITY_TOPIC   DEFAULT_TOPIC "availability"
#define MQTT_AVAILABILITY_MESSAGE "online"
// Topics for publishing
#define MQTT_STATE_UPTIME         DEFAULT_TOPIC "uptime"
#define MQTT_STATE_PWM_1          DEFAULT_TOPIC "state/pwm/1"
#define MQTT_STATE_PWM_2          DEFAULT_TOPIC "state/pwm/2"
#define MQTT_STATE_PWM_3          DEFAULT_TOPIC "state/pwm/3"
#define MQTT_STATE_POWER_1        DEFAULT_TOPIC "state/power/1"
#define MQTT_STATE_POWER_2        DEFAULT_TOPIC "state/power/2"
#define MQTT_STATE_POWER_3        DEFAULT_TOPIC "state/power/3"
// Topics for subscribing
#define MQTT_CMD_TOPIC_RESET      DEFAULT_TOPIC "set/reset"
#define MQTT_CMD_TOPIC_PWM_1      DEFAULT_TOPIC "set/pwm/1"
#define MQTT_CMD_TOPIC_PWM_2      DEFAULT_TOPIC "set/pwm/2"
#define MQTT_CMD_TOPIC_PWM_3      DEFAULT_TOPIC "set/pwm/3"
#define MQTT_CMD_TOPIC_POWER_1    DEFAULT_TOPIC "set/power/1"
#define MQTT_CMD_TOPIC_POWER_2    DEFAULT_TOPIC "set/power/2"
#define MQTT_CMD_TOPIC_POWER_3    DEFAULT_TOPIC "set/power/3"
// Commands
#define MQTT_CMD_ON               "1"
#define MQTT_CMD_OFF              "0"

// Publish interval
#define MQTT_PUBLISH_INTERVAL 1000

// Reconnecting to MQTT server delay
#define RECONNECT_DELAY 5000

// IO pins
// #define LED_BUILTIN 2    // D4 - already defined in pins_arduino.h
#define HEATER_ELEMENT_1 14 // D5
#define HEATER_ELEMENT_2 12 // D6
#define HEATER_ELEMENT_3 13 // D7

#endif // _DEF_H_