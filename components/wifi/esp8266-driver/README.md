# ESP8266 WiFi driver for Mbed OS

The Mbed OS driver for the ESP8266 WiFi module.

## Firmware version

ESP8266 modules come in different shapes and formats, but the most important factor is the firmware version in it. To make sure that the firmware in your module is compatible with Mbed OS, follow the [Update guide](https://developer.mbed.org/teams/ESP8266/wiki/Firmware-Update).

## Restrictions

- The ESP8266 WiFi module does not allow the TCP client to bind on a specific port.
- Setting up a UDP server is not possible.
- The serial port does not have hardware flow control enabled. The AT command set does not either have a way to limit the download rate. Therefore, downloading anything larger than the serial port input buffer is unreliable. An application should be able to read fast enough to stay ahead of the network. This affects mostly the TCP protocol where data would be lost with no notification. On UDP, this would lead to only packet losses which the higher layer protocol should recover from.
