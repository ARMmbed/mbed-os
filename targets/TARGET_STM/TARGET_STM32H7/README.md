# STM32H7 family

[st.com STM32H7 family page](https://www.st.com/en/microcontrollers-microprocessors/stm32h7-series.html)

[GitHub STM32CubeH7 FW](https://github.com/STMicroelectronics/STM32CubeH7)


# NUCLEO_H743ZI / NUCLEO_H743ZI2

Note that NUCLEO_H743ZI is deprecated. Please update your board to NUCLEO_H743ZI2:
- new MCU chip revision
- new ST Link version

STM32H743ZI devices are based on the high-performance Arm Cortex-M7 32-bit RISC core operating at up to 480 MHz.

[st.com STM32H743ZI MCU page](https://www.st.com/en/microcontrollers-microprocessors/stm32h743zi.html)

[st.com NUCLEO page](https://www.st.com/en/evaluation-tools/nucleo-h743zi.html)

[mbed.com Target page](https://os.mbed.com/platforms/ST-Nucleo-H743ZI2/)


- Total FLASH is 2 MB (0x200000)
    - 2x8 sectors of 128 KB
    - Flash memory bank 1 @ 0x0800 0000
    - Flash memory bank 2 @ 0x0810 0000

- RAM: 1 MB
    - DTCM :      128 KB (0x20000) @0x2000 0000
    - SRAM :      512 KB (0x80000) @0x2400 0000
    - SRAM1 :     128 KB (0x20000) @0x3000 0000
    - SRAM2 :     128 KB (0x20000) @0x3002 0000
    - SRAM3 :      32 KB  (0x8000) @0x3004 0000
    - SRAM4 :      64 KB (0x10000) @0x3800 0000


# DISCO_H747I

## Overview

The STM32H7x7 lines combine the performance of the Cortex-M7 (with double-precision floating point unit) running up to 480 MHz and the Cortex-M4 core (with single-precision floating point unit).

[st.com STM32H747ZI MCU page](https://www.st.com/en/microcontrollers-microprocessors/stm32h747zi.html)

[st.com board page](https://www.st.com/en/evaluation-tools/stm32h747i-disco.html)

[mbed.com Target page](https://os.mbed.com/platforms/ST-Discovery-H747I/)

- Total FLASH is 2 MB (0x200000)
    - Flash memory bank 1 @ 0x0800 0000: 8 sectors of 128 KB => 1MB
    - Flash memory bank 2 @ 0x0810 0000: 8 sectors of 128 KB => 1MB

- RAM: 1 MB
    - DTCM :  128 KB (0x20000) @0x2000 0000 - only CM7 side
    - SRAM :  512 KB (0x80000) @0x2400 0000 - shared
    - SRAM1 : 128 KB (0x20000) @0x3000 0000 - shared
    - SRAM2 : 128 KB (0x20000) @0x3002 0000 - shared
    - SRAM3 :  32 KB  (0x8000) @0x3004 0000 - shared
    - SRAM4 :  64 KB (0x10000) @0x3800 0000 - shared

## Dual mode configuration

Configuration can be checked/changed with STM32CubeProgrammer software in the Option bytes (OB) panel.

https://www.st.com/en/development-tools/stm32cubeprog.html

Default configuration is booting both CM7 and CM4:
- CM7 at 0x0800 0000
- CM4 at 0x0810 0000

Note that `Drag and Drop` feature for binary application is flashing at 0x0800 0000

**IP HW usage warning**

All resources (GPIO, UART, SPI, ...) can be used by both cores.

It is up to application to choose which instance is used by which core.

For GPIO, HW semaphore have been implemented which allows applications to use same GPIO port in both sides
(CM4 can use PA_1, and CM7 PA_2).

NB : Virtual Com Port is using the same UART pins in CM4 and CM7.

It is not recommended to enable printf from both cores without any protection mechanism.

**System clock configuration**

Clock configuration is shared between both cores.

During boot procedure:
- M7 core is managing clock tree settings, and then wake up M4
- M4 is waiting for wake up from M7

When exiting Deep Sleep mode, first core (either M4 or M7) that woke up, is setting back this clock configuration.


## Make application on M7 side

- Flash memory bank 1 is used
- DTCM and SRAM are used
- SRAM3 is used for Ethernet support

As explained in the `Dual mode configuration` above part, you can drag and drop applications.

For application compilation, you can use DISCO_H747I or DISCO_H747I_CM7 target name.

For MBED tests execution, you have to use DISCO_H747I target name (mbed detect name).


## Make application on M4 side

- Flash memory bank 2 is used
- SRAM1, SRAM1 and SRAM3 is used (with @0x1000 0000 alias)

For application compilation, you have to use DISCO_H747I_CM4 target name.

For flashing, as explained in the `Dual mode configuration` above part, you can not drag and drop applications.
You have to use STM32CubeProgrammer software

https://www.st.com/en/development-tools/stm32cubeprog.html

Here is command lines for flashing (but you can use graphical tool):
```
FLASHPATH="C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
export PATH=$FLASHPATH:$PATH
STM32_Programmer_CLI -c port=SWD mode=UR -w BUILD/DISCO_H747I_CM4/ARM/mbed-os.bin 0x8100000
```

## Ethernet limitation

**WARNING**: Ethernet connector is not enabled by default

More information in the wiki page : [Ethernet HW Patch](https://os.mbed.com/teams/ST/wiki/DISCO_H747I-modifications-for-Ethernet)


