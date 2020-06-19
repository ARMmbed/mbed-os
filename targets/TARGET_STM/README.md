# README for Mbed OS STM32 targets

## ST TOOLS

### USB drivers

Mandatory: get the latest USB driver in order to make available all the USB interfaces provided by the ST-LINK:
- ST Debug
- Virtual COM port
- ST Bridge interfaces

Default Windows USB drivers will not setup full capabilities.

https://www.st.com/en/development-tools/stsw-link009.html


### ST-Link FW

Mandatory: get the latest ST-Link Firmware:

https://www.st.com/en/development-tools/stsw-link007.html

You could have some issue to connect your board if you didn't install full USB drivers.


Note that with the latest FW version, you are able to check the version number easily with a simple "mbedls":

```
$ mbedls
| platform_name       | platform_name_unique   | mount_point | serial_port | target_id                | interface_version |
|---------------------|------------------------|-------------|-------------|--------------------------|-------------------|
| DISCO_H747I         | DISCO_H747I[0]         | D:          | COM13       | 081402210D03E72132477E08 | V3J7M2            |
| DISCO_L475VG_IOT01A | DISCO_L475VG_IOT01A[0] | E:          | COM9        | 07640221683B630A577FF553 | V2J37M26          |
```

### STM32 Cube

https://www.st.com/en/embedded-software/stm32cube-mcu-packages.html

There is one STM32Cube package for each individual STM32 MCU family.

It includes:
- The hardware abstraction layer (HAL) enabling portability between different STM32 devices via standardized API calls
- Low-layer (LL) APIs, a light-weight, optimized, expert oriented set of APIs designed for both performance and runtime efficiency
- A collection of middleware components including RTOS, USB library, file system, TCP/IP stack, touch-sensing library or graphics library (depending on the STM32 series)
- BSP drivers, based on HAL drivers.

Part of STM32Cube files are copied in each targets/TARGET_STM/TARGET_STM32\<xx\>/STM32Cube_FW directory:
- CMSIS header files in CMSIS sub-directory
- HAL and LL files in STM32\<XX\>xx_HAL_Driver sub-directory

Mbed OS HAL calls ST porting layer, which calls ST HAL and LL API.

Note that all ST HAL and LL files are available:
- you can then develop some applications with direct ST HAL and LL call, even if feature is not supported in Mbed OS
- BSP for LCD, AUDIO, SENSORS, etc... are not available in Mbed OS, but you should be able to use it in your local application.


Each STM32Cube package is also available in Github.
This table summarizes the STM32Cube versions currently used in Mbed OS master branch :

| STM32 Serie | Cube version | Github source                                     |
|-------------|--------------|---------------------------------------------------|
| F0          |    1.9.0     | https://github.com/STMicroelectronics/STM32CubeF0 |
| F1          |    1.8.0     | https://github.com/STMicroelectronics/STM32CubeF1 |
| F2          |    1.6.0     | https://github.com/STMicroelectronics/STM32CubeF2 |
| F3          |    1.9.0     | https://github.com/STMicroelectronics/STM32CubeF3 |
| F4          |    1.19.0    | https://github.com/STMicroelectronics/STM32CubeF4 |
| F7          |    1.16.0    | https://github.com/STMicroelectronics/STM32CubeF7 |
| G0          |    1.3.0     | https://github.com/STMicroelectronics/STM32CubeG0 |
| H7          |    1.7.0     | https://github.com/STMicroelectronics/STM32CubeH7 |
| L0          |    1.10.0    | https://github.com/STMicroelectronics/STM32CubeL0 |
| L1          |    1.8.1     | https://github.com/STMicroelectronics/STM32CubeL1 |
| L4          |    1.11.0    | https://github.com/STMicroelectronics/STM32CubeL4 |
| L5          |    1.1.0     | https://github.com/STMicroelectronics/STM32CubeL5 |
| WB          |    1.4.0     | https://github.com/STMicroelectronics/STM32CubeWB |

In Mbed OS repository, we try to minimize the difference between "official" and copied files.


### STM32CubeMX

STM32CubeMX is a graphical tool that allows a very easy configuration of all STM32

https://www.st.com/en/development-tools/stm32cubemx.html

Tool is not used in Mbed OS, but it can be useful for you.


### STM32CubeProgrammer

It provides an easy-to-use and efficient environment for reading, writing and verifying device memory.

https://www.st.com/en/development-tools/stm32cubeprog.html

Tool is not used in Mbed OS, but it can be useful for you.


## STM32 families

### STM32WB

[STM32WB README](TARGET_STM32WB/README.md)

### STM32H7

[STM32H7 README](TARGET_STM32H7/README.md)



## Custom boards

It should be "easy" to add your custom board with a STM32 MCU in Mbed OS

### STM32 organisation

STM32 root directory is https://github.com/ARMmbed/mbed-os/tree/master/targets/TARGET_STM

This contains:
- all STM32 families directories: F0, F1, F2, F3, F4, F7, G0, H7, L0, L1, L4, WB
- Mbed OS porting layer common for all

Each STM32 family contains several "sub-families".

Each STM32 Part Number defines a sub-family: STM32F401 / STM32F407 / STM32F429 / ...

But also each STM32 Part Number with different FLASH size : STM32F401xC / STM32F401xE

Mbed OS porting layer specific for this family are placed here.

Example in TARGET_STM32G0:
- TARGET_STM32G031xx
- TARGET_STM32G071xx
- ...

Each STM32 sub-family contains:
- toolchains files
- board specific files


### Add a custom board

ST provides the complete support for few NUCLEO and DISCO boards.

Locate one of these boards with the minimum difference with your chosen MCU.

Copy paste, and update!


### Board specific files (pinmap)

2 files in Mbed OS:
- PeripheralPins.c
- PinNames.h

It is recommended to use a python script to generate those files

https://github.com/ARMmbed/mbed-os/blob/master/tools/targets/STM32_gen_PeripheralPins.py

STM32CubeMX has to be installed first. Path has to be specified in the `cube_path.json` file.

```
$ python tools/targets/STM32_gen_PeripheralPins.py -h

Script version 1.9
usage: STM32_gen_PeripheralPins.py [-h] [-l | -b | -m xml | -t HW | -c CUSTOM]

Script will generate PeripheralPins.c thanks to the xml files description available in
STM32CubeMX directory defined in 'cube_path.json':
        C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX

optional arguments:
  -h, --help            show this help message and exit
  -l, --list            list available mcu xml files description in STM32CubeMX
  -b, --boards          list available boards description in STM32CubeMX
  -m xml, --mcu xml     specify the mcu xml file description in STM32CubeMX to use (use double quotes).
                           Parameter can be a filter like L496 if you want to parse all L496 chips (-m STM32 to parse all).
  -t HW, --target HW    specify the board file description in STM32CubeMX to use (use double quotes).
                           Parameter can be a filter like L496 (only the first file found will be parsed).
  -c CUSTOM, --custom CUSTOM
                        specify a custom board .ioc file description to use (use double quotes).

Once generated, you have to check and comment pins that can not be used (specific HW, internal ADC channels, remove PWM using us ticker timer, ...)
```

How to generate files for a custom boards based on a STM32F427 MCU:
```
$ python tools/targets/STM32_gen_PeripheralPins.py -l | grep F427
STM32F427A(G-I)Hx.xml
STM32F427I(G-I)Hx.xml
STM32F427I(G-I)Tx.xml
STM32F427V(G-I)Tx.xml
STM32F427Z(G-I)Tx.xml

$ python tools/targets/STM32_gen_PeripheralPins.py -m "STM32F427V(G-I)Tx.xml"

Script version 1.9
CubeMX DB version DB.5.0.60

 * Output directory: C:\github\mbed\STM32F427V(G-I)Tx
 * Generating PeripheralPins.c and PinNames.h with 'STM32F427V(G-I)Tx.xml'
 * GPIO file: C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeMX\db\mcu\IP\GPIO-STM32F427_gpio_v1_0_Modes.xml
 * I/O pins found: 82 connected: 0
```

### Use of custom_targets.json

https://os.mbed.com/docs/mbed-os/v6.0/porting/porting-a-custom-board.html

Example with a board based on STM32F103C8 (like BluePill):
- MCU_STM32F103x8 generic configuration is already available in targets.json file

```
$ python tools/targets/STM32_gen_PeripheralPins.py -m "STM32F103C(8-B)Tx.xml"
// PeripheralPins.c and PinNames.h template files are created in STM32H745ZITx directory

$ mv STM32F103C\(8-B\)Tx/ TARGET_BLUEPILL_F103C8
// Edit PeripheralPins.c and PinNames.h to match your board configuration

// Create a custom_targets.json with:
{
    "BLUEPILL_F103C8": {
        "inherits": [
            "MCU_STM32F103x8"
        ],
        "overrides": {
            "clock_source": "USE_PLL_HSE_XTAL"
        },
        "device_has_remove": [
            "STDIO_MESSAGES"
        ],
        "device_name": "STM32F103C8"
    }
}
```

Example with a board based on STM32H745ZI
- this is dual core MCU with CM4 and CM7
- MCU_STM32H745I_CM4 and MCU_STM32H745I_CM7 generic configuration is already available in targets.json file

```
$ python tools/targets/STM32_gen_PeripheralPins.py -m "STM32H745ZITx.xml"
// PeripheralPins.c and PinNames.h template files are created in STM32H745ZITx directory

$ mv STM32H745ZITx TARGET_H745ZI_BOARD
// Edit PeripheralPins.c and PinNames.h to match your board configuration

// Create a custom_targets.json with:
{
    "H745ZI_BOARD_CM4": {
        "inherits": [
            "MCU_STM32H745I_CM4"
        ],
        "extra_labels_add": [
            "H745ZI_BOARD"
        ]
    },
    "H745ZI_BOARD_CM7": {
        "inherits": [
            "MCU_STM32H745I_CM7"
        ],
        "extra_labels_add": [
            "H745ZI_BOARD"
        ]
    }
}
```


## ST specific implementation

### WiFi configuration

https://github.com/ARMmbed/wifi-ism43362

https://os.mbed.com/teams/ST/wiki/How-to-make-wifi-tests


### Ethernet configuration

Depending on your PHY, you will have to customize several configuration values:
- the number of RX buffers
- the number of TX buffers
- thread size
- PHY address
- media interface
- AutoNegotiation mode
- DuplexMode mode
- Speed mode
- PHY register Offset
- Speed mask information in the PHY status register
- Duplex mask information in the PHY status register

Check the default values in:
https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/emac-drivers/TARGET_STM/mbed_lib.json

Option is also to define your own `HAL_ETH_MspInit` function,
you then have to add **USE_USER_DEFINED_HAL_ETH_MSPINIT** macro.



## Mbed OS Wiki pages

https://os.mbed.com/teams/ST/wiki/

https://os.mbed.com/teams/ST/wiki/How-to-enable-flash-dual-bank

https://os.mbed.com/teams/ST/wiki/Nucleo-144pins-ethernet-spi-conflict
