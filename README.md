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

## Dependencies
All dependencies will be automatically installed by PlatformIO:
- PubSubClient@2.8.0

## Copyright
Copyright (c) 2023 Sen Morgan. Licensed under the MIT license, see LICENSE.md