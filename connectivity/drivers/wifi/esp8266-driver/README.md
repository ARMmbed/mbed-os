# ESP8266 WiFi driver for Mbed OS

The Mbed OS driver for the ESP8266 WiFi module.

## Firmware version

ESP8266 modules come in different shapes and formats, but the firmware version is the most important factor. To
make sure that the firmware in your module is compatible with Mbed OS, follow the
[Update guide](https://developer.mbed.org/teams/ESP8266/wiki/Firmware-Update).

This driver supports AT firmware versions 1.3.0 to 1.7.0. We advise updating the
[AT firmware](https://www.espressif.com/en/support/download/at?keys=) to at least version 1.7.0.

## Restrictions

* The ESP8266 Wi-Fi module does not allow the TCP client to bind to a specific port.
* Setting up a UDP server is not possible.
* The serial port does not have hardware flow control enabled by default. Additionally, the AT command set does not have a method for limiting the download rate. Therefore, downloading anything larger than the serial port input buffer is unreliable 
unless you use [AT firmware](https://www.espressif.com/en/support/download/at?keys=) version 1.7.0 or later. With older
firmware, an application should be able to read fast enough to stay ahead of the network. This applies mostly to TCP
protocol, where data would be lost without notification. On UDP using all firmware versions, the higher-layer protocol should recover from packet loss.

## Mandatory configuration

[mbed_lib.json](mbed_lib.json) configuration assumes Arduino form factor. Please adjust according to your board. You can override parameters from your app config file.

At minimum, check the following configuration parameters:

```javascript
{
    "name": "esp8266",
    "config": {
        "tx": {
            "help": "TX pin for serial connection",
            "value": null <- 'D1' by default if Arduino, adjust based on your board
        },
        "rx": {
            "help": "RX pin for serial connection",
            "value": null <- 'D0' by default if Arduino, adjust based on your board
        },
        "provide-default": {
            "help": "Provide default WifiInterface. [true/false]",
            "value": false <- Set to 'true' if this is the interface you are using
        },
        "socket-bufsize": {
            "help": "Max socket data heap usage",
            "value": 8192 <- Without HW flow control more is better. Once the limit is reached packets are
                             dropped - does not matter is it TCP or UDP.
        }
    }
}
```

## UART HW flow control

UART HW flow control requires you to additionally wire the CTS and RTS flow control pins between your board and your
ESP8266 module. After this, remember to add the configuration option for flow control to your app configuration file. This example uses the [ST NUCLEO-F429ZI](https://os.mbed.com/platforms/ST-Nucleo-F429ZI/) board and
[ESPBee XBee Module](https://www.cascologix.com/product/espbee/).

**Note:** Not all modules expose ESP8266's RTS and CTS pins, so choose modules carefully.

Once you have your hardware set up, add a configuration like the following to your app configuration file. Arduino pins D1 and D0 are used as TX and RX:

``` javascript
"target_overrides": {
        "NUCLEO_F429ZI": {
            "esp8266.rts": "PG_12",
            "esp8266.cts": "PG_15"
         }
```

### Example board pins

* TX: D1 (Arduino Uno Revision 3 connectivity  headers)
* RX: D0 (Arduino Uno Revision 3 connectivity headers)
* RTS: PG_12 (STMicroelectronics Morpho extension pin headers)
* CTS: PG_15 (STMicroelectronics Morpho extension pin headers)

### Example ESP8266 pins

* TX: D1 (Arduino Wireless Protoshield headers)/ TX (ESPBee XBee headers)
* RX: D0 (Arduino Wireless Protoshield headers)/ RX (ESPBee XBee headers)
* RTS: RTS (ESPBee XBee headers)
* CTS: CTS (ESPBee XBee headers)

### Connections

With these pictures only consider the green and yellow wires which are connected to ESP8266. The pink wire is for reset and
the rest are for firmware update. TX and RX go through Arduino pins D1 and D0.

**Note:** Pull down GPIO15(ESPBee RTS) during startup to **boot from flash**, instead of **firmware update** or
**boot from SD card**. Once the software is running, the same pin is used as the RTS pin:

* Board TX: ESP8266 RX
* Board RX: ESP8266 TX
* Board RTS (grey): ESP8266 CTS(yellow)
* Board CTS (white): ESP8266 RTS(green)

![RTS,CTS](nucleo_esp8266_hw_fc1.jpg)
![RTS,CTS](nucleo_esp8266_hw_fc2.jpg)
