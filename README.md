# Electric Heater Controller

DIY electric heater controller made to communicate with Home Assistant via MQTT protocol.
The project was created in PlatformIO 05.02.2023

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=AA101F)](https://www.espressif.com/en/products/socs/esp8266)<br>
[![Build with PlatformIO](https://img.shields.io/badge/Build%20with-PlatformIO-orange)](https://platformio.org/)<br>
[![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)

<br>


## Hardware
Electric cabin heater radiator (part number 9153884) from BMW E90 driven by Wemos D1 Mini board. Wemos GPIOs are connected directly to the GPIOs of the original radiator MCU to control 3 onboard power switches that drives heater matrix elements.

## High side switches
PROGRAMMABLE CURRENT SENSING HIGH SIDE SWITCH
Datasheet: https://www.irf.com/product-info/datasheets/data/ir3310.pdf

## Settings
WiFi, OTA and MQTT settings must be set by renaming `platformio_override.ini.example` to `platformio_override.ini` and setting your own values.
Hardware and other settings are in `lib/defs/def.h` file.

## Home Assistant YAML configuration
```yaml
mqtt:
    button:
      - name: "Electric Heater Controller Reset"
        command_topic: "/electric-heater-controller/set/reset"
        payload_press: "1"
        retain: false
        entity_category: "config"
        device_class: "restart"
    light:
      - name: "Electric Heater Controller PWM 1"
        unique_id: "electric_heater_controller_pwm_1"
        state_topic: "/electric-heater-controller/state/power/1"
        command_topic: "/electric-heater-controller/set/power/1"
        payload_on: "1"
        payload_off: "0"
        availability_topic: "/electric-heater-controller/availability"
        payload_available: "online"
        payload_not_available: "offline"
        brightness_state_topic: "/electric-heater-controller/state/pwm/1"
        brightness_command_topic: "/electric-heater-controller/set/pwm/1"
        brightness_scale: 255
        retain: false
        optimistic: false
        qos: 0
        icon: "mdi:radiator"
      - name: "Electric Heater Controller PWM 2"
        unique_id: "electric_heater_controller_pwm_2"
        state_topic: "/electric-heater-controller/state/power/2"
        command_topic: "/electric-heater-controller/set/power/2"
        payload_on: "1"
        payload_off: "0"
        availability_topic: "/electric-heater-controller/availability"
        payload_available: "online"
        payload_not_available: "offline"
        brightness_state_topic: "/electric-heater-controller/state/pwm/2"
        brightness_command_topic: "/electric-heater-controller/set/pwm/2"
        brightness_scale: 255
        retain: false
        optimistic: false
        qos: 0
        icon: "mdi:radiator"
      - name: "Electric Heater Controller PWM 3"
        unique_id: "electric_heater_controller_pwm_3"
        state_topic: "/electric-heater-controller/state/power/3"
        command_topic: "/electric-heater-controller/set/power/3"
        payload_on: "1"
        payload_off: "0"
        availability_topic: "/electric-heater-controller/availability"
        payload_available: "online"
        payload_not_available: "offline"
        brightness_state_topic: "/electric-heater-controller/state/pwm/3"
        brightness_command_topic: "/electric-heater-controller/set/pwm/3"
        brightness_scale: 255
        retain: false
        optimistic: false
        qos: 0
        icon: "mdi:radiator"
```

## Dependencies
All dependencies will be automatically installed by PlatformIO:
- PubSubClient@2.8.0

## Copyright
Copyright (c) 2023 Sen Morgan. Licensed under the MIT license, see LICENSE.md
