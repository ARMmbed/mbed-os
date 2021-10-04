# EMW3080B WiFi driver for STM32

## License

This software is licensed under terms that can be found in the [license](mx_wifi/LICENSE.md) file


## Currently supported platforms

EMW3080B module is available

- with an [ARDUINO extension board](https://www.seeedstudio.com/IOT-AT3080-IoT-development-board-p-4021.html)

- with a ST Discovery kit for IoT node : [B-U585I-IOT02A](https://os.mbed.com/platforms/ST-Discovery-B-U585I-IOT02A/)


## Configuration

B_U585I_IOT02A default configuration is enabled in [mbed_lib.json](mbed_lib.json)

For Arduino shield,  the UART interface is used.
See in mbed_lib.json, B_U585I_IOT02A__ARDUINO_SHIELD configuration example you need to add in your local mbed_app.json.

```
 "YOUR_TARGET": {
            "emw3080b.wifi-spi-interface": false,
            "emw3080b.wifi-tx": "ARDUINO_UNO_D1",
            "emw3080b.wifi-rx": "ARDUINO_UNO_D0",
            "emw3080b.wifi-reset": "ARDUINO_UNO_D13",
            "emw3080b.provide-default": true
        },
```

## Code Structure

The Mbed EMW3080B driver relies on the [STM32 EMW3080B BSP driver](https://github.com/STMicroelectronics/STM32CubeU5/tree/main/Drivers/BSP/Components/mx_wifi)

- COMPONENT_emw3080b/EMW3080BInterface.cpp  :   WiFiInterface class implementation (derived from EMACInterface )
- COMPONENT_emw3080b/EMW3080B_EMAC.cpp      :   EMAC interface
- COMPONENT_emw3080b/EMW3080B_SPI.cpp       : 	SPI communication protocol implementation
- COMPONENT_emw3080b/EMW3080B_UART.cpp      :	UART communication protocol implementation
- COMPONENT_emw3080b/mx_wifi/*              :	[STM32 EMW3080B BSP driver](https://github.com/STMicroelectronics/STM32CubeU5/tree/main/Drivers/BSP/Components/mx_wifi)


## Debug

Some debug print on console can help to debug if necessary.

Either update your mbed_app.json:
```
 "YOUR_TARGET": {
            "emw3080b.wifi-debug": true
        },
```

Either update manually the different .cpp source files to choose the appropriate level of VERBOSITY.

`#define DEFAULT_DEBUG DEBUG_SILENT` ==> choose DEBUG_WARNING/DEBUG_INFO/DEBUG_LOG

NB: in file "EMW3080B_SPI.cpp", DEBUG_LOG may have side effect due to real time issue.


## Validation

- netsocket tests: https://github.com/ARMmbed/mbed-os/tree/master/connectivity/netsocket/tests/TESTS/netsocket

- network-wifi tests: https://github.com/ARMmbed/mbed-os/tree/master/connectivity/netsocket/tests/TESTS/network/wifi

			
## Open point


TODO: netbuffer allocator feedback to choose appropriate memory mngt (copy or not) and fix concern regarding potential race when starting EMACinterface.
		
## Limitations

Performance of SPI limitted to 5Mhz.
			

## Firmware version & firmware update

The recommended firmware version is V2.1.11

NB: with older FW version, you could get an assert:
```
++ MbedOS Error Info ++
Error Status: 0x80FF0100 Code: 256 Module: 255
Error Message: Fatal Run-time error
Location: 0x801CB73
Error Value: 0xFFFFFFFE
Current Thread: lwip_tcpip Id: 0x20006058 Entry: 0x802064D StackSize: 0x4B0 StackMem: 0x20006698 SP: 0x20006ACC
For more info, visit: https://mbed.com/s/error?error=0x8003010D&tgt=B_U585I_IOT02A
-- MbedOS Error Info --
EMW3080BInterface : MX_WIFI_Init failed, you may have to update MXCHIP fimrware module to version 2.1.11
```

### How to get FW version ?

Enable DEBUG_INFO in EMW3080BInterface.cpp

```
EMW3080BInterface : Product name: MXCHIP-WIFI
EMW3080BInterface : Product ID: EMW3080B
EMW3080BInterface : FW revision: V2.1.11
```


### How to upgrade FW for B_U585I_IOT02A ?

Latest FW binary is provided in:

https://www.st.com/en/development-tools/x-wifi-emw3080b.html#get-software

Start TeraTerm (115200)

Drag and drop `EMW3080updateV2.1.11RevC.bin` into `NOD_U585AI`drive

You should get:
```
################### Welcome to EMW3080 firmware update utility 1.0.0 for board RevB or RevC ###################

STM32>Configured for MXchip embedded FW ST-EMW3080B_V2.1.11_SPI.OTA.BIN 0x8000234  / 546080 bytes
STM32>Don't forget to connect EMW3080 Boot to GND thanks to switch SW2 , move it to position "0" (instead of NC) to have boot loader running !
STM32>Push user button (Blue) to flash EMW3080 device or enter command
STM32>MCU system clock 160 MHz , Uart Baud rate 921600  ,  Software emulated UART bit duration 173 cycles


"      help"            print this message
"      boot"            Boot EMW3080 (needed before using mx command)
"        mx"            mx [cmd] : send command to mxchip device (can not use cmd which require ymodem protocol)
"     flash"            flash mxchip firmware (same as pushing blue button)
"     check"            check mxchip firmware
STM32>
```

- on switch SW2 (with 2 white switches), move BOOT one (Id 1) from NC to O position
- press blue button (User B3)

Wait till:
```
STM32>Flash successfull, move back switch to original position, reset the device to get back to prompt
```

So:
- move switch back to postion NC
- Reset
