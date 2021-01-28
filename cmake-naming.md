## CMake targets for Mbed "targets"

### targets.json naming scheme

- `MCU_STM32L4` - base target
- `DISCO_L4R9I` - final target - a user board or could be custom target, inherits from MCU_  target

The targets.json properties use capital letters with underscore but not the our config. Probably to distinguish these from other regular directories (the initial Mbed SDK had it this way).

### CMake naming scheme

All Mbed OS CMake targets are prefixed with `mbed-` to avoid clashes with user's targets (out of the tree targets): `mbed-ble`, `mbed-nanostack`.

Mbed OS targets should follow the same scheme in CMake.

- `mbed-target-stm32-mcu-stm32l4` - base target
- `mbed-target-stm32-disco-l4r9i` - final target - a user board or could be custom target, inherits from MCU_  target

If we are not consistent with naming, we would have targets different named than the rest of the tree in Mbed OS and potentionally also in the user space.

To consider how this looks with the package manager - the namespacing packages is a good thing (we had it in yotta). We should add `mbed`, `target` and `vendor` at least to the CMake target.

We will refactor targets directories once the old tools are gone. It will help us to reduce the paths in Mbed OS drastically. This will be our chance to also rename directories, not following the old scheme (if we decide it's a good change to apply the new scheme to targets).

An example what we can actually do with renaming:

`targets/TARGET_STM32/TARGET_STM32L4/TARGET_MCU_STM32L4` could be changed to `targets/stm32/stm32l4/mcu-stm32l4`. 

This allows us to get rid of the name equals directory structure (our tools are not enforcing it anymore). We do not need stm32l4 in the name just because there is a directory in the tree. We can create more logical target structure.

#### Targets translation

targets.json keeps its naming scheme. I do not think we are at the stage to refactor it.
 What we can do however is to keep our CMake naming in `targets` folder.

 We provide in CMake the translation from targets.json to cmake targets.
