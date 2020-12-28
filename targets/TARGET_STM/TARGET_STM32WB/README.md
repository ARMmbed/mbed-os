# STM32WB family

[st.com STM32WB family page](https://www.st.com/en/microcontrollers-microprocessors/stm32wb-series.html)

This ST MCU family is dual-core : based on an Arm Cortex-M4 core and an Arm Cortex-M0+ core.

- M4 core is dedicated to application (mbed-os)
- M0 core is dedicated to BLE FW (only binary, source files not provided)

# Supported boards

## NUCLEO_WB55RG

[st.com STM32WB55RG MCU page](https://www.st.com/en/microcontrollers-microprocessors/stm32wb55rg.html)

[st.com NUCLEO page](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html)

[mbed.com NUCLEO page](https://os.mbed.com/platforms/ST-Nucleo-WB55RG/)

- Total FLASH is 1MB, but note that it is shared by M4 and M0 cores.
    - mbed-os application size is then limited to 768 KB

- RAM: 256 KB
    - SRAM1: 192 KB
    - SRAM2a: 32 KB
    - SRAM2b: 32 KB

SRAM1 is dedicated for M4 core, and then for mbed-os applications.

SRAM2 is dedicated for M0 core and inter CPU communication, and then can not be addressed.


# BLE

## MBED-OS support

BLE feature is based on Cordio stack and API (above HCI) implementation.

Note that the BLE controller firmware running on the cortex-M0 is the same as in STcubeFW solution

- within mbed, the controller will run only the below-HCI part
- within STcubeFW (and STM32WPAN middleware), it can run also up to GATT / GAP layers ...

Official ST Application Note AN5289: Building wireless applications with STM32WB Series microcontrollers :
https://www.st.com/resource/en/application_note/dm00598033-building-wireless-applications-with-stm32wb-series-microcontrollers-stmicroelectronics.pdf



## mbed-trace support

trace group: BLE_WB

example:
````
[INFO][BLE_WB]: WIRELESS COPROCESSOR FW VERSION ID = 1.8.0
[INFO][BLE_WB]: WIRELESS COPROCESSOR FW STACK TYPE = 1
[DBG ][BLE_WB]: mbox_write type:1, len:3
[INFO][BLE_WB]: TX>> BLE CMD
[DBG ][BLE_WB]:   Type 0x1
[DBG ][BLE_WB]:    Cmd 0xc03
[DBG ][BLE_WB]:    Len 0D]
````

## BLE FW update

Official ST Application Note AN5185: ST firmware upgrade services for STM32WB Series :
http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00513965.pdf

Latest BLE FW :
https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/stm32wb5x_BLE_Stack_full_fw.bin

## BLE FW flashing procedure

Release Note and complete flashing procedure:
https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/Release_Notes.html


- STEP 1: Use STM32CubeProgrammer

https://www.st.com/en/development-tools/stm32cubeprog.html

````
FLASHPATH="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
export PATH=$FLASHPATH:$PATH
````

- STEP 2: Access to Bootloader USB Interface (system flash)

    * Boot0 pin set to VDD : Jumper between CN7.5(VDD) and CN7.7(Boot0)
    * Jumper JP1 on USB_MCU
    * Power ON via USB_USER

![Image description](stm32wb_ble_update.jpg)

- STEP 3 : Delete current wireless stack :

```
$ STM32_Programmer_CLI.exe -c port=usb1 -fwdelete
...
FUS state is FUS_IDLE

FUS status is FUS_NO_ERROR
Deleting firmware ...
Firmware delete finished
fwdelete command execution finished
```

- STEP 4 : Read and upgrade FUS Version

```
$ STM32_Programmer_CLI.exe -c port=usb1 -r32 0x20030030 1
...

Reading 32-bit memory content
  Size          : 4 Bytes
  Address:      : 0x20030030

0x20030030 : 00050300
```

- STEP 5A if last result is 00050300 : Download new FUS :

```
$ ./STM32_Programmer_CLI.exe -c port=usb1 -fwupgrade stm32wb5x_FUS_fw_1_0_2.bin 0x080EC000 firstinstall=0
```

- STEP 5B if last result is 01000100 or 01000200 : Download new FUS :

```
$ STM32_Programmer_CLI.exe -c port=usb1 -fwupgrade stm32wb5x_FUS_fw.bin 0x080EC000 firstinstall=0
```


- STEP 4 (to check) : Read and upgrade FUS Version

```
$ STM32_Programmer_CLI.exe -c port=usb1 -r32 0x20030030 1

Reading 32-bit memory content
  Size          : 4 Bytes
  Address:      : 0x20030030

0x20030030 : 01010000
```

- STEP 6 : Download new wireless stack :


```
$ STM32_Programmer_CLI.exe -c port=usb1 -fwupgrade stm32wb5x_BLE_Stack_full_fw.bin 0x080CB000 firstinstall=1

...
Download firmware image at address 0x80cb000 ...
...
File download complete
...
Firmware Upgrade Success
```

- STEP 7 : Revert STEP 2 procedure to put back device in normal mode.

