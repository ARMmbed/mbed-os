## CMake targets for Mbed targets

### Target configuration file naming scheme

Default attributes of Mbed targets are defined in `targets/targets.json`, these include the name of the target.

Mbed targets usually inherit their properties from a "base" target which represents a family of targets that share similar properties. For example the `DISCO_L4R9I` Mbed board has the `MCU_STM32L4` as its base target.

### CMake target naming scheme

All Mbed OS CMake targets are prefixed with `mbed-` to avoid clashes with the custom targets (basically targets outside of Mbed OS repository).

Mbed targets should follow the same scheme we establish in CMake, using `mbed-` prefix for all components. To consider how this looks with the package manager - the namespacing packages should be considered.

The proposed scheme: `mbed-target-<VENDOR>`, where <VENDOR> is a partner (stm, cypress, etc). What comes after VENDOR is vendor specific.

`DISCO_L4R9I` Mbed OS board should have a CMake target named `mbed-target-stm-disco-l4r9i` with a dependency on a `mbed-target-stm-mcu-stm32l4` base CMake target.

- `mbed-target-stm-mcu-stm32l4` - base target
- `mbed-target-stm-disco-l4r9i` - final target - a user board or could be custom target, inherits from MCU_  target

We are planning to refactor target directories once the old tools are deprecated and removed. It will drastically reduce the path lengths in Mbed OS. It will also provide an opportunity to rename directories without following the old naming scheme. 
For example, `targets/TARGET_STM32/TARGET_STM32L4/TARGET_MCU_STM32L4` could be become `targets/stm/stm32l4/mcu-stm32l4`. 

#### Targets translation

targets.json stay without a change for now (would we consider to refactor it later? It could be broken into smaller parts - per vendor for instance).

CMakelists for each vendor would have the logic to select the proper target. As a user creates CMake targets when porting, it could do the same as we do:

    ```
        # STM CMakelists.txt would contain this
        CMAKE_TARGET = mbed-target-$(mbed_target_get_vendor())-$(mbed_target_get_cmake_name())
    ```

`mbed_target_get_vendor()` contains our vendors supported to return valid vendor name

`mbed_target_get_cmake_name()` function would just do string replace, to keep things simple.
