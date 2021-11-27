# README for Mbed OS STM32 targets

Table of Contents
=================

   * [ST TOOLS](#st-tools)
      * [USB drivers](#usb-drivers)
      * [ST-Link FW](#st-link-fw)
      * [STM32 Cube](#stm32-cube)
      * [STM32CubeMX](#stm32cubemx)
      * [STM32CubeProgrammer](#stm32cubeprogrammer)
   * [STM32 families](#stm32-families)
      * [STM32WB](#stm32wb)
      * [STM32WL](#stm32wl)
      * [STM32H7](#stm32h7)
   * [Custom boards](#custom-boards)
      * [STM32 organisation](#stm32-organisation)
      * [Add a custom board](#add-a-custom-board)
      * [Board specific files (pinmap)](#board-specific-files-pinmap)
      * [Use of custom_targets.json](#use-of-custom_targetsjson)
      * [Make your custom board public](#make-you-custom-board-public)
   * [ST specific implementation](#st-specific-implementation)
      * [Pin configuration](#pin-configuration)
         * [Alternate feature](#alternate-feature)
         * [Conflict pins](#conflict-pins)
      * [Clock selection](#clock-selection)
         * [System clock](#system-clock)
         * [Low power clock](#low-power-clock)
         * [I2C Timing calculation algorithm](#i2c-timing-calculation-algorithm)
      * [Sleep feature](#sleep-feature)
      * [WiFi configuration](#wifi-configuration)
      * [Ethernet configuration](#ethernet-configuration)
      * [Asynchronous SPI limitation](#asynchronous-spi-limitation)
   * [Mbed OS Wiki pages](#mbed-os-wiki-pages)


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

```
$ mbedtools detect
Board name       Serial number             Serial port    Mount point(s)    Build target(s)    Interface Version
---------------  ------------------------  -------------  ----------------  -----------------  -------------------
NUCLEO-U575ZI-Q  0022003c5553500d20393256  COM25          D:                NUCLEO_U575ZI_Q    V3J7M3
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
| F0          |    1.11.2    | https://github.com/STMicroelectronics/STM32CubeF0 |
| F1          |    1.8.3     | https://github.com/STMicroelectronics/STM32CubeF1 |
| F2          |    1.6.0     | https://github.com/STMicroelectronics/STM32CubeF2 |
| F3          |    1.11.2    | https://github.com/STMicroelectronics/STM32CubeF3 |
| F4          |    1.26.1    | https://github.com/STMicroelectronics/STM32CubeF4 |
| F7          |    1.16.1    | https://github.com/STMicroelectronics/STM32CubeF7 |
| G0          |    1.5.0     | https://github.com/STMicroelectronics/STM32CubeG0 |
| G4          |    1.4.0     | https://github.com/STMicroelectronics/STM32CubeG4 |
| H7          |    1.9.0     | https://github.com/STMicroelectronics/STM32CubeH7 |
| L0          |    1.12.0    | https://github.com/STMicroelectronics/STM32CubeL0 |
| L1          |    1.10.2    | https://github.com/STMicroelectronics/STM32CubeL1 |
| L4          |    1.17.0    | https://github.com/STMicroelectronics/STM32CubeL4 |
| L5          |    1.4.0     | https://github.com/STMicroelectronics/STM32CubeL5 |
| U5          |    1.0.0     | https://github.com/STMicroelectronics/STM32CubeU5 |
| WB          |    1.11.1    | https://github.com/STMicroelectronics/STM32CubeWB |
| WL          |    1.1.0     | https://github.com/STMicroelectronics/STM32CubeWL |

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

### STM32WL

[STM32WL README](TARGET_STM32WL/README.md)

### STM32H7

[STM32H7 README](TARGET_STM32H7/README.md)



## Custom boards

It should be "easy" to add your custom board with a STM32 MCU in Mbed OS

You can also check in https://github.com/ARMmbed/stm32customtargets


### STM32 organisation

STM32 root directory is https://github.com/ARMmbed/mbed-os/tree/master/targets/TARGET_STM

This contains:
- all STM32 families directories: F0, F1, F2, F3, F4, F7, G0, G4, H7, L0, L1, L4, L5, U5, WB, WL
- Mbed OS porting layer common for all

Each STM32 family contains several "sub-families".

Each STM32 Part Number defines a sub-family: STM32F401 / STM32F407 / STM32F429 / ...

But also each STM32 Part Number with different FLASH size : STM32F401xC / STM32F401xE

Mbed OS porting layer specific for this family are placed here.

Example in TARGET_STM32G0:
- TARGET_STM32G031x8
- TARGET_STM32G071xB
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

https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py

This script is using MCU database from https://github.com/STMicroelectronics/STM32_open_pin_data.git repo

```
$ python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py -h

SScript version 1.19

Checking STM32_open_pin_data repo...
*** git clone done

usage: STM32_gen_PeripheralPins.py [-h] (-l | -b | -m xml | -t HW | -c CUSTOM)
                                   [-g]

Script will generate PeripheralPins.c thanks to the xml files description available in STM32_open_pin_data GitHub repo

More information in targets/TARGET_STM/README.md

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
  -g, --gpio            Add GPIO PinMap table

Once generated, you have to check and comment pins that can not be used (specific HW, internal ADC channels, remove PWM using us ticker timer, ...)

```

How to generate files for a custom boards based on a STM32F427 MCU:
```
$ python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py -l | grep F427
STM32F427A(G-I)Hx.xml
STM32F427I(G-I)Hx.xml
STM32F427I(G-I)Tx.xml
STM32F427V(G-I)Tx.xml
STM32F427Z(G-I)Tx.xml

$ python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py -m "STM32F427V(G-I)Tx.xml"

Script version 1.19

Checking STM32_open_pin_data repo...
        Already up to date.

STM32_open_pin_data DB version STM32CubeMX-DB.6.0.10

 * Output directory: targets_custom\TARGET_STM\TARGET_STM32F4\TARGET_STM32F427xG\TARGET_STM32F427VGT
 * Generating PeripheralPins.c and PinNames.h with 'STM32_open_pin_data\mcu\STM32F427V(G-I)Tx.xml'
 * GPIO file: STM32_open_pin_data\mcu\IP\GPIO-STM32F427_gpio_v1_0_Modes.xml
 * I/O pins found: 135 connected: 0

 * Output directory: targets_custom\TARGET_STM\TARGET_STM32F4\TARGET_STM32F427xI\TARGET_STM32F427VIT
 * Generating PeripheralPins.c and PinNames.h with 'STM32_open_pin_data\mcu\STM32F427V(G-I)Tx.xml'
 * GPIO file: STM32_open_pin_data\mcu\IP\GPIO-STM32F427_gpio_v1_0_Modes.xml
 * I/O pins found: 135 connected: 0
```

### Use of custom_targets.json

https://os.mbed.com/docs/mbed-os/latest/porting/porting-a-custom-board.html

Example with a board based on STM32F103C8 (like BluePill):
- MCU_STM32F103x8 generic configuration is already available in targets.json file

```
$ python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py -m "STM32F103C(8-B)Tx.xml"
// PeripheralPins.c and PinNames.h template files are created in targets_custom/TARGET_STM/TARGET_STM32F1/TARGET_STM32F103x8/TARGET_STM32F103C8T directory

$ mv TARGET_STM32F103C8T TARGET_BLUEPILL_F103C8
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
$ python targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py -m "STM32H745ZITx.xml"
// PeripheralPins.c and PinNames.h template files are created in targets_custom/TARGET_STM/TARGET_STM32H7/TARGET_STM32H745xI/TARGET_STM32H745ZIT directory

$ mv TARGET_STM32H745ZIT TARGET_H745ZI_BOARD
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


### Make your custom board public

We will be happy to add every public board in https://github.com/ARMmbed/stm32customtargets

Make a Pull request, we will check consistency and build.


## ST specific implementation

### Pin configuration

It can be useful to have a look on files that describes pin configuration for your board:
- targets/TARGET_STM/TARGET_STM32**XX**/TARGET_STM32**XXXXX**/TARGET_**XXXXX**/PeripheralPins.c
- targets/TARGET_STM/TARGET_STM32**XX**/TARGET_STM32**XXXXX**/TARGET_**XXXXX**/PinNames.h

#### Alternate feature

You can easily see the alternate functions for each pin.

Ex:
```
    {PC_10,      UART_3,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {PC_10_ALT0, UART_4,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_UART4)},
```
- If your application is using PC_10 pin for UART, drivers will configure UART3 instance.
- If your application is using PC_10_ALT0 pin for UART, drivers will configure UART4 instance.

The same alternate choice feature is also used for PWM, ADC, SPI, etc...

#### Conflict pins

Sometimes there are some conflicts in pin use.

Ex:
```
    {PA_5,       SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI1)}, // Connected to LD2 [green led]
```
==> You can use PA_5 pin as SPI, **only** if your application is not using LED...

Sometimes, pin is explicitly removed by default to avoid issues (but you can uncomment the line for your custom board)
```
//  {PB_4,       UART_2,  STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // Connected to same instance as STDIO 
```

### Clock selection

#### System clock

System Core Clock is based on an high-speed clock.

- the HSI is the high-speed internal (MCU) clock with low accuracy
- the HSE is the high-speed external clock with higher accuray

For each target, a default choice has been made in the "clock_source" config settings in the targets.json file.

For main targets, it is something like:

```
    "clock_source": {
        "value": "USE_PLL_HSE_EXTC|USE_PLL_HSI",
```

Meaning that:
- PLL with the external HSE clock is first configured
- if it fails, PLL with HSI is then configured


#### Low power clock

Low power ticker and RTC are based on an low-speed clock.

- the LSI is the low-speed internal clock with low accuracy
- the LSE is the low-speed external clock connected to 32.768 kHz quartz crystal

In targets.json file, it is supposed that a LSE is provided in the board

```
"config": {
    "lse_available": {
        "value": "1"
```

You can change this in you local mbed_app.json:
```
{
    "target_overrides":
    {
        "XXXX": {
            "target.lse_available": "0"
        }
    }
}
```

#### I2C Timing calculation algorithm

I2C drivers version 2 use I2C timing register.

Enable I2C timing algorithm by setting the value of `i2c_timing_value_algo`
target config to `true`

```
"i2c_timing_value_algo": {
                "help": "If value was set to true I2C timing algorithm is 
                enabled. Enabling may leads to performance issue. Keeping this
                false and changing system clock will trigger assert.",
                "value": false
            }
```
Default configuration disables I2C timing algorithm. If user need to use
different system clock speed other than default system clock configuration.
Then I2C timing calculation algorithm need to enable. To enable

```
"i2c_timing_value_algo": {
                "value": true
            }
```


### Sleep feature

ST MCUs feature several low-power modes, please check Reference Manual of each one for more details.

- MBED sleep mode is usually mapped to ST SLEEP mode:
  - CPU clock is off
  - all peripherals can run and wake up the CPU when an interrupt or an event
occurs

- MBED deepsleep mode is mapped to ST STOP2 mode:
  - all clocks in the VCORE domain are stopped
  - the PLL, the MSI, the HSI and the HSE are disabled
  - the LSI and the LSE can be kept running
  - RTC can remain active

Detailed sleep Mbed OS description : https://os.mbed.com/docs/mbed-os/latest/apis/power-management-sleep.html
- debug profile is disabling deepsleep
- deepsleep can also be disabled by application or drivers using sleep_manager_lock_deep_sleep()
- deep-sleep-latency value is configured to 4 by default for STM32
- trace with MBED_SLEEP_TRACING_ENABLED macro is set by default with low verbosity
```
    "target_overrides": {
        "*": {
            "platform.deepsleep-stats-enabled": true,
            "platform.deepsleep-stats-verbose": false
        },
```


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
https://github.com/ARMmbed/mbed-os/blob/master/connectivity/drivers/emac/TARGET_STM/mbed_lib.json

Option is also to define your own `HAL_ETH_MspInit` function,
you then have to add **USE_USER_DEFINED_HAL_ETH_MSPINIT** macro.

#### Custom IRQ Handler and Callback from user application
To use the custom IRQ Handler and the callbacks, you need to add
**USE_USER_DEFINED_HAL_ETH_IRQ_CALLBACK** macro
inside any of the JASON file in either targets.json or in mbed_app.json file.

For example in the targets.json,
you need to add this line in your target:
```"macros_add": ["USE_USER_DEFINED_HAL_ETH_IRQ_CALLBACK"],```
or for example in the mbed_app.json, you need to add:
``` "macros": ["USE_USER_DEFINED_HAL_ETH_IRQ_CALLBACK"]```

By doing the any of the above json files, the corresponding user defined custom apis like
HAL_ETH_RxCpltCallback() and STM_HAL_ETH_Handler() can be called from
the user application.

#### Changing default MAC address in STM32
To change the default MAC address in STM32,
If we have the function mbed_otp_mac_address() in the user application,the default ethernet address
can be changed.
Because as this is defined as weak in mbed-os/connectivity/drivers/emac/TARGET_STM/stm32xx_emac.cpp
```
#include "platform/mbed_toolchain.h"
MBED_WEAK uint8_t mbed_otp_mac_address(char *mac).
```

Please find the code snippet here for reference:

```
..
uint8_t mbed_otp_mac_address(char *mac);
uint8_t mbed_otp_mac_address(char *mac)
{
	unsigned char ST_mac_addr[6] = {0x00, 0x88, 0xe0,0x90,0x80,0x70}; // New User mac address
	// printf("%s:%s\n",__FILE__,__func__);
	memcpy(mac,ST_mac_addr,sizeof(ST_mac_addr));
	return 1;
}

int main()
{
	// Bring up the ethernet interface
	printf("Ethernet socket example\n");
	uint8_t MyMAC[6];
	printf("return of set_mac_address:%d\n",net.set_mac_address(MyMAC,sizeof(MyMAC)));

	net.connect();
	printf("MAC address %s\n",net.get_mac_address());
...

```
### Asynchronous SPI limitation

The current Asynchronous SPI implementation will not be able to support high speeds (MHz Range).
The maximum speed supported depends on
- core operating frequency
- depth of SPI FIFOs (if available).

For application that require optimized maximum performance, the recommendation is to implement the DMA-based SPI transfer.
The SPI DMA transfer support shall be implemented on a case-by-case based on below example
https://github.com/ABOSTM/mbed-os/tree/I2C_SPI_DMA_IMPLEMENTATION_FOR_STM32L4

### CAN receive interrupt problem due to mutex and resolution

In bxCAN and earlier versions the receive interrupt flags can be cleared only on performing a read operation in ST MCUs
But can_read() cannot be used in interrupt context as it is gaurded by lock operation and mbed does not allow locks in 
interrupt context. Hence the Rx interrupt is disabled for a while and read is deferred to thread context, the interrupt is
enabled on a successful read.

As an other option RawCAN (with unlocked CAN apis) is also available and can be used directly, if only one thread is accessing
the CAN interface.

While using RxInterrupt with the CAN object the receive ISR callback registered should defer read to thread context.
A simple example is as shown below:

```
#include "mbed.h"

Ticker ticker;
Thread canReadThread;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

CAN can1(PD_0 ,PD_1);

EventQueue queue(32 * EVENTS_EVENT_SIZE);

int counter = 0xABCD;
CANMessage msg;

void canRead(){
        if(can1.read(msg)) {
            if(msg.id==1100)
                led2 = !led2;
            if(msg.id==1102){
                led3 = !led3;
            }
        }
}

void canISR(){
    queue.call(canRead);
    led3 = !led3;
}

int main() {

    can1.frequency(100000);
    can1.mode(CAN::Normal);

    can1.attach(canISR, CAN::RxIrq);

    canReadThread.start(callback(&queue, &EventQueue::dispatch_forever));

    while(1) {
    }
}
```


## Mbed OS Wiki pages

https://os.mbed.com/teams/ST/wiki/

https://os.mbed.com/teams/ST/wiki/STDIO

https://os.mbed.com/teams/ST/wiki/How-to-enable-flash-dual-bank

https://os.mbed.com/teams/ST/wiki/Nucleo-144pins-ethernet-spi-conflict
