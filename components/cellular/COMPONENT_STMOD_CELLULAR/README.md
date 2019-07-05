# STMOD CELLULAR library

This library allows enabling the cellular module that is connected to the STMOD+ connector of the board.

## Supported modems.

Currently supported cellular modules are Quectel UG96 and BG96 expansion boards that are included in [p-l496g-cell01](https://www.st.com/en/evaluation-tools/p-l496g-cell01.html)
and [p-l496g-cell02](https://www.st.com/en/evaluation-tools/p-l496g-cell02.html) packs.

## Specifications

The STMOD+ Connector specification can be found [here](https://www.st.com/content/ccc/resource/technical/document/technical_note/group0/04/7f/90/c1/ad/54/46/1f/DM00323609/files/DM00323609.pdf/jcr:content/translations/en.DM00323609.pdf).

## Cellular tests in mbed-os

- features-cellular-tests-api-cellular_device
- features-cellular-tests-api-cellular_information
- features-cellular-tests-api-cellular_network
- features-cellular-tests-api-cellular_sms
- features-cellular-tests-socket-udp

Here is the used mbed_app.json:

````
{
    "config": {
        "cellular-sim-pin" : {
            "help": "PIN code",
            "value": "\"1234\""
        },
        "apn": {
            "help": "The APN string to use for this SIM/network, set to 0 if none",
            "value": "\"APN\""
        },
        "username": {
            "help": "The user name string to use for this APN, set to zero if none",
            "value": 0
        },
        "password": {
            "help": "The password string to use for this APN, set to 0 if none",
            "value": 0
        }
   },
    "target_overrides": {
        "DISCO_L496AG": {
            "target.macros_add": ["CELLULAR_DEVICE=STModCellular"],
            "target.components_add": ["STMOD_CELLULAR"],
            "stmod_cellular.provide-default": "true"
        }
    }
}
````


````
$ mbed test -t ARM -m DISCO_L496AG -v -n features-cellular-*
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
