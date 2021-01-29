## CMake targets for Mbed targets

### Target configuration file naming scheme

Default attributes of Mbed targets are defined in `targets/targets.json`, these include the name of the target.

Mbed targets usually inherit their properties from a "base" target which represents a family of targets that share similar properties. For example the `DISCO_L4R9I` Mbed board has the `MCU_STM32L4` as its base target.

The targets.json properties use capital letters with underscore but not the our config. Probably to distinguish these from other regular directories (the initial Mbed SDK had it this way).

### CMake target naming scheme

All Mbed OS CMake targets are prefixed with `mbed-` to avoid clashes with user's targets (out of the tree targets): `mbed-ble`, `mbed-nanostack`.

Mbed targets should follow the same scheme in CMake. For example the `DISCO_L4R9I` Mbed board should have a CMake target named `mbed-target-stm32-disco-l4r9i` with a dependency on a `mbed-target-stm32-mcu-stm32l4` base CMake target.

- `mbed-target-stm32-mcu-stm32l4` - base target
- `mbed-target-stm32-disco-l4r9i` - final target - a user board or could be custom target, inherits from MCU_  target

Consistency in naming is a must to avoid having CMake targets with names different than the rest of the Mbed OS repository tree and in the user space.

To consider how this looks with the package manager - the namespacing packages is a good thing (we had it in yotta). We should add `mbed`, `target` and `vendor` at least to the CMake target.

We will refactor targets directories once the old tools are gone. It will drastically reduce the path lengths in Mbed OS. It will also provide an opportunity to rename directories without following the old naming scheme. For example, `targets/TARGET_STM32/TARGET_STM32L4/TARGET_MCU_STM32L4` could be become `targets/stm32/stm32l4/mcu-stm32l4`. 


This allows us to get rid of the name equals directory structure (our tools are not enforcing it anymore). We do not need stm32l4 in the name just because there is a directory in the tree. We can create more logical target structure.

#### Targets translation

targets.json keeps its naming scheme. I do not think we are at the stage to refactor it.
 What we can do however is to keep our CMake naming in `targets` folder.

 The CMake input files map the Mbed target names used in `targets.json` to the directories in the Mbed OS repository.
