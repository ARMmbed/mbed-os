# nrf51-sdk
Module to contain files provided by the nordic nRF51 SDK. The latest version of this module uses files from Nordic SDK 10.0.0. The files are extracted from [here](https://developer.nordicsemi.com/nRF5_SDK/nRF51_SDK_v10.x.x/nRF51_SDK_10.0.0_dc26b5e.zip).

## Changes made to Nordic files
The files are kept the same as much as possible to the Nordic SDK. Modifications are made in order to integrate with mbed.
    - ble/common/ble_conn_state.c: Preprocessor tests regarding S110, S120 or S130 macro should be replace by TARGET_MCU_NRF51_XXK_SXXX tests
    - `ble.h` in `source/nordic_sdk/components/softdevice/s130/headers/ble.h` has to be renamed `nrf_ble.h`. All files which include this file should be updated accordingly.


## Porting new versions of Nordic SDK
A list of files currently required by mbed is maintained in [script/required_files.txt](https://github.com/ARMmbed/nrf51-sdk/blob/master/script/required_files.txt). [A python script](https://github.com/ARMmbed/nrf51-sdk/blob/master/script/pick_nrf51_files.py) is written to help porting from nordic sdk releases. **required_files.txt** is parsed to find a list of filenames. The script searches for these filenames in the sdk folder, and copy then into the yotta module mirroring the folder structure in the sdk. **extraIncludes** is automatically added to module.json to allow direct inclusion of noridc headers with just the filename.

### Script usage
```
python pick_nrf51_files.py [options] <full-noridc-sdk-path> <nrf51-sdk-yotta-module-path>
options: --purge   : to delete all existing files and start again
         --dry-run : to list the files to be copied but not actually copy them
```

There are files in the sdk with the same filename but in different folder. This is dealt with by excluding certain directories. The excluded directories are listed in [pick_nrf51_files.py](https://github.com/ARMmbed/nrf51-sdk/blob/master/script/pick_nrf51_files.py).

After running the script, the changes in [the previous section](#changes-made-to-nordic-files) will have to be applied manually again.

Folder structure or even file name can change between releases of the nordic sdk, hence a degree of manual adjustment is needed when porting.

## Using Noridc Headers
The nordic sdk is written in C and yotta modules support C++. If you are trying to include Nordic files in a cpp program, you need to use the `extern "C"` keyword around the includes.
```c
extern "C" {
#include "softdevice_handler.h"
}
```
