# STMOD CELLULAR library

This library allows enabling the cellular module that is connected to the STMOD+ connector of the board.

## Supported modems.

Currently supported cellular modules are Quectel UG96 and BG96 expansion boards that are included in [p-l496g-cell01](https://www.st.com/en/evaluation-tools/p-l496g-cell01.html)
and [p-l496g-cell02](https://www.st.com/en/evaluation-tools/p-l496g-cell02.html) packs.

## Specifications

The STMOD+ Connector specification can be found [here](https://www.st.com/content/ccc/resource/technical/document/technical_note/group0/04/7f/90/c1/ad/54/46/1f/DM00323609/files/DM00323609.pdf/jcr:content/translations/en.DM00323609.pdf).

## Debug print

mbed_trace feature is used: https://github.com/ARMmbed/mbed-os/blob/master/features/frameworks/mbed-trace/README.md

Enable it in your mbed_app.json file:

````
{
    "target_overrides": {
        "*": {
            "mbed-trace.enable": 1
        }
    }
}
````

Look for "STMOD" group name:
````
[DBG ][STMOD]: STMOD_CELLULAR default instance
[INFO][STMOD]: STModCellular creation
[DBG ][STMOD]: STMOD cellular modem power ON
[INFO][STMOD]: Booting BG96
[INFO][STMOD]: Modem ready to receive AT commands
[INFO][STMOD]: Enable flow control
[DBG ][STMOD]: Flow control turned ON
````


## Cellular tests in mbed-os

Since mbed-os-5.14.1, cellular tests have been replaced with generic mbed-os netsocket and network interface tests.

https://github.com/ARMmbed/mbed-os/blob/master/TESTS/netsocket/README.md

Here is an example of needed mbed_app.json:

````
{
    "config": {
        "echo-server-discard-port": {
            "help": "Discard port of echo server",
            "value": "9
        },
        "echo-server-discard-port-tls": {
            "help": "Discard port of echo server",
            "value": "2009"
        },
        "echo-server-port": {
            "help": "Port of echo server",
            "value": "7"
        },
        "echo-server-port-tls": {
            "help": "Echo port of echo server",
            "value": "2007"
        },
    },
    "target_overrides": {
        "DISCO_L496AG": {
            "target.components_add": [
                "STMOD_CELLULAR"
            ],
            "stmod_cellular.provide-default": "true",
            "nsapi.default-cellular-apn": "\"APN\"",
            "target.network-default-interface-type": "CELLULAR"
        }
    }
}
````


````
$ mbed test -t ARM -m DISCO_L496AG -v -n tests-net*
````

## Cellular mbed-os example

https://github.com/ARMmbed/mbed-os-example-cellular

````
mbed import mbed-os-example-cellular
cd mbed-os-example-cellular
 
<edit mbed_app.json file as explained in the example: APN, PIN,... >
 
mbed compile -t GCC_ARM -m DISCO_L496AG --flash
````

Then check the serial console (serial baudrate set to 115200 bit/s)

## Board with STMOD+ Connector

### DISCO_L496AG

https://os.mbed.com/platforms/ST-Discovery-L496AG/

Default HW configuration:
- STMOD pins 1, 2, 3 and 4 are mapped to UART CTS/TX/RX/RTS

BG96 expansion board is fully compatible.

### DISCO_L4R9I

https://os.mbed.com/platforms/DISCO-L4R9I/

Default HW configuration:
- STMOD pins 1 and 2 are mapped to SPI
- STMOD pins 2 and 3 are mapped to UART TX/RX

BG96 expansion board can be used but without flow control.

### DISCO_L562_QE

https://os.mbed.com/platforms/ST-Discovery-L562QE/

HW configuration:

Depending on PF_11 and PF_12 GPIO configuration, STMOD pins 1, 2, 3 and 4 can be mapped to SPI3 or UART3.

STModCellular default init procedure is setting the correct values in order to get full UART connectivity.

See in mbed-trace:
```
[INFO][STMOD]: STModCellular creation
[INFO][STMOD]: STMOD+ UART pins re-configuration
```

BG96 expansion board is then fully compatible.

### DISCO_H747I

https://os.mbed.com/platforms/ST-Discovery-H747I/

Default HW configuration:
- STMOD pins 1, 2, 3 and 4 are mapped to SPI

BG96 expansion board can not be used without solder bridges update
