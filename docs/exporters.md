# About the exporters

The mbed exporters are used to export your code to various 3rd party tools and IDEs. Each exporter implements a `generate` function that produces an IDE specific project file. Exporters benefit from [mbed build tools](https://github.com/ARMmbed/mbed-os/blob/master/docs/BUILDING.md#build-mbed-sdk-library-from-sources). However, instead of using your source and [config data](https://github.com/ARMmbed/mbed-os/blob/master/docs/config_system.md) to create an executable, we use that information to populate an IDE project file that will be configured to build, flash, and debug your code. You can find exporter implementations [here](https://github.com/ARMmbed/mbed-os/tree/master/tools/export).

## mbed-cli command

`mbed export -m [target] -i [IDE]`

# Adding export support for a target

If you have added new target to the mbed SDK, exporting will allow users to transition from mbed source code to the offline development environment of their choice. This functionality activates the use of your device for larger number of users.

## Eclipse and Make 

Eclipse project export utilizes a generated Makefile for building. Other than target configuration within the [config system](https://github.com/ARMmbed/mbed-os/blob/master/docs/mbed_targets.md) for mbed build system support, there is no additional work to provide Make export.

### Available mbed-cli commands

`mbed export -m [target] -i [make_gcc_arm, make_iar, make_armc5, eclipse_gcc_arm, eclipse_iar, eclipse_armc5]`

## UVision and IAR

### CMSIS Packs

UVision and IAR both utilize [CMSIS packs](http://www.keil.com/pack/doc/CMSIS/Pack/html/index.html) to find target information necessary to create a valid project file. 

We utilize the tool [ArmPackManager](https://github.com/ARMmbed/mbed-os/tree/master/tools/arm_pack_manager) to scrape [MDK5 Software Packs](https://www.keil.com/dd2/Pack/) for target information. This is achieved by parsing [http://www.keil.com/pack/index.idx](http://sadevicepacksprod.blob.core.windows.net/idxfile/index.idx). The relevant information in the [PDSC (Pack Description)](http://www.keil.com/pack/doc/CMSIS/Pack/html/_pack_format.html) retrieved from each URL in the index is stored in [index.json](https://github.com/ARMmbed/mbed-os/blob/master/tools/arm_pack_manager/index.json). A `.pdsc` file typically describes a family of devices. Each device is uniquely identified by its [device name](https://github.com/ARMmbed/mbed-os/blob/master/docs/mbed_targets.md#device_name). Thus, this name makes a natural key to associate a device with its information in `index.json`. 

#### What's in a device name?
There is no reliable way to map an mbed alias like [NUCLEO_F030R8](https://github.com/ARMmbed/mbed-os/blob/master/targets/targets.json#L603) to its unique identifier, [STM32F030R8](https://github.com/ARMmbed/mbed-os/blob/master/targets/targets.json#L615), as it is listed in a CMSIS pack (and subsequently `index.json`). So, we added a [device name](https://github.com/ARMmbed/mbed-os/blob/master/docs/mbed_targets.md#device_name) field in `targets.json`. **This field is required for IAR or UVision exporter support**.

#### Code Usage
http://www.keil.com/pack/Keil.Kinetis_K20_DFP.pdsc is the PDSC that contains TEENSY_31 device (MK20DX256xxx7). It has been parsed by ArmPackManager and stored in `index.json`. The device information begins on line 156:
```xml
      <device Dname="MK20DX256xxx7">
        <processor Dfpu="0" Dmpu="0" Dendian="Little-endian" Dclock="72000000"/>
        <compile header="Device\Include\MK20D7.h"  define="MK20DX256xxx7"/>
        <debug      svd="SVD\MK20D7.svd"/>
        <memory     id="IROM1"                      start="0x00000000"  size="0x40000"    startup="1"   default="1"/>
        <memory     id="IROM2"                      start="0x10000000"  size="0x8000"     startup="0"   default="0"/>
        <memory     id="IRAM1"                      start="0x20000000"  size="0x8000"     init   ="0"   default="1"/>
        <memory     id="IRAM2"                      start="0x1FFF8000"  size="0x8000"     init   ="0"   default="0"/>
        <algorithm  name="Flash\MK_P256.FLM"        start="0x00000000"  size="0x40000"                  default="1"/>
        <algorithm  name="Flash\MK_D32_72MHZ.FLM"   start="0x10000000"  size="0x8000"                   default="1"/>
        <book name="Documents\K20P100M72SF1RM.pdf"         title="MK20DX256xxx7 Reference Manual"/>
        <book name="Documents\K20P100M72SF1.pdf"           title="MK20DX256xxx7 Data Sheet"/>
      </device>
```

##### Uvision
The dname (device name) field on line 156 directly corresponds to that in the Uvision5 IDE target selection window. [`tools/export/uvision/uvision.tmpl`](https://github.com/ARMmbed/mbed-os/blob/master/tools/export/uvision/uvision.tmpl#L15), uses target information from these packs is used to generate valid Uvision5 projects. If the device name is not found, we use a generic ARM CPU target in Uvision5.

##### IAR
[`tools/export/iar/iar_definitions.json`](https://github.com/ARMmbed/mbed-os/blob/master/tools/export/iar/iar_definitions.json) utilizes this device name to store information necessary to set the target in an IAR project.

##### Updating index.json
You can regenerate `index.json` to contain a newly made CMSIS pack with the following command:

`mbed export -i [IDE] --update-packs`

You should include the changes to `index.json` in your PR that adds support for the new target.




