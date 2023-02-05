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
#define MQTT_SUBSCRIBE_TOPIC      DEFAULT_TOPIC "#"
#define MQTT_PUBLISH_TOPIC        DEFAULT_TOPIC "state"
#define MQTT_AVAILABILITY_TOPIC   DEFAULT_TOPIC "availability"
#define MQTT_AVAILABILITY_MESSAGE "online"
#define MQTT_UPTIME_TOPIC         DEFAULT_TOPIC "uptime"

// Reconnecting to MQTT server delay
#define RECONNECT_DELAY 5000

// IO pins
// #define LED_BUILTIN 2    // D4 - already defined in pins_arduino.h
#define HEATER_ELEMENT_1 14 // D5
#define HEATER_ELEMENT_2 12 // D6
#define HEATER_ELEMENT_3 13 // D7

#endif // _DEF_H_