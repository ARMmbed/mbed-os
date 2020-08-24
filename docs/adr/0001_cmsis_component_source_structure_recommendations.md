# CMSIS component source structure recommendations
Date: 2020-08-19

## Status
Proposed

## Context

[`ARM-software/CMSIS_5`](https://github.com/ARM-software/CMSIS_5) has been ported to Mbed OS. However, it is split across multiple directories and it is not immediately obvious that it comes from that directory. It also makes it difficult to create an independent CMake target can be built as a library that can be optionally be added by end users.

The current proposal is to create a new directory `CMSIS_5/` at the root of the repository that will mirror closely the structure used by `ARM-software/CMSIS_5/` keeping only the directories and files needed by Mbed OS. It will also include Mbed OS specific files and rename some directories in order to work with Mbed OS build tools (see [Decision](#decision) section below). This will result in the removal of the `rtos/source/TARGET_CORTEX/` and `cmsis/` directories.

Additionally it will provide the following advantages:
* Better maintenance of component
* Easy creation of an independent CMake target
* Easy source navigation that mirrors closely the upstream repository


## <a name="decision"></a>Decision
The following directory restructure is recommended:

```
+--cmsis
|   +-- README.md
|   +-- TARGET_CORTEX_A/
|       +-- TOOLCHAIN_IAR/
|           +-- cmain.S                     # Currently cmsis/TARGET_CORTEX_A/TOOLCHAIN_IAR
|   +-- device/
|       +-- rtos2/
|           +-- mbed_lib.json               # New Mbed library named `cmsis-device-rtos2`, so it can be excluded from bare metal profile
|           +-- include/
|               +-- A                       # Header files currently in rtos/source/TARGET_CORTEX/
|               +-- mbed_cmsis_conf.h       # Currently in cmsis/
|           +-- source/
|               +-- B                       # Source files currently in rtos/source/TARGET_CORTEX/
|           +-- RTE/
|               +-- include/
|                   +-- RTE_Components.h    # Currently in cmsis/
|           +-- TOOLCHAIN_ARM_MICRO/        # Currently in rtos/source/TARGET_CORTEX/
|           +-- TOOLCHAIN_ARM_STD/          # Currently in rtos/source/TARGET_CORTEX/
|           +-- TOOLCHAIN_GCC_ARM/          # Currently in rtos/source/TARGET_CORTEX/
|           +-- TOOLCHAIN_IAR/              # Currently in rtos/source/TARGET_CORTEX/
|   +-- CMSIS_5/
|       +-- CMSIS/
|           +-- RTOS2/
|               +-- mbed_lib.json           # Currently in rtos/source/TARGET_CORTEX/, creates `cmsis-cmsis5-rtos2` Mbed library
|               +-- Include/
|                   +-- C                   # Content of rtos/source/TARGET_CORTEX/rtx5/Include/
|               +-- RTX/
|                   +-- Config/
|                       +-- D               # Content of rtos/source/TARGET_CORTEX/rtx5/RTX/Config/
|                   +-- Include/
|                       +-- E               # Content of rtos/source/TARGET_CORTEX/rtx5/RTX/Include/
|                   +-- Include1/
|                       +-- cmsis_os.h      # Currently in rtos/source/TARGET_CORTEX/rtx4/
|                   +-- Library/
|                       +-- cmsis_os1.c     # Currently in rtos/source/TARGET_CORTEX/rtx4/
|                   +-- Source/
|                       +-- F               # Content of rtos/source/TARGET_CORTEX/rtx5/RTX/Source/
|               +-- Source/
|                   +-- G                   # Content of rtos/source/TARGET_CORTEX/rtx5/Source
|           +-- Core_A/
|               +-- Include/
|                   +-- H                   # Header files currently in cmsis/TARGET_CORTEX_A/
|               +-- Source/
|                   +-- I                   # Source files currently in cmsis/TARGET_CORTEX_A/
|           +-- Core/
|               +-- Include/
|                   +-- J                   # Header files currently in cmsis/TARGET_CORTEX_M/
|               +-- Source/
|                   +-- K                   # Source files currently in cmsis/TARGET_CORTEX_M/
```

However, to cope with Mbed-CLI requirement to find directories to process with the target labels mechanism, some directories will be prefixed with `TARGET_` and capitalized. It will therefore look as follows:

```
+--cmsis
|   +-- README.md
|   +-- TARGET_CORTEX_A/
|       +-- TOOLCHAIN_IAR/
|           +-- cmain.S                     # Currently cmsis/TARGET_CORTEX_A/TOOLCHAIN_IAR
|   +-- device/
|       +-- rtos2/
|           +-- mbed_lib.json               # New Mbed library named `cmsis-rtos2`, so it can be excluded from bare metal profile
|           +-- include/
|               +-- A                       # Header files currently in rtos/source/TARGET_CORTEX/
|               +-- mbed_cmsis_conf.h       # Currently in cmsis/
|           +-- source/
|               +-- B                       # Source files currently in rtos/source/TARGET_CORTEX/
|           +-- RTE/
|               +-- include/
|                   +-- RTE_Components.h    # Currently in cmsis/
|           +-- TOOLCHAIN_ARM_MICRO/        # Currently in rtos/source/TARGET_CORTEX/
|           +-- TOOLCHAIN_ARM_STD/          # Currently in rtos/source/TARGET_CORTEX/
|           +-- TOOLCHAIN_GCC_ARM/          # Currently in rtos/source/TARGET_CORTEX/
|           +-- TOOLCHAIN_IAR/              # Currently in rtos/source/TARGET_CORTEX/
|   +-- CMSIS_5/
|       +-- CMSIS/
|           +-- RTOS2/
|               +-- mbed_lib.json           # Currently in rtos/source/TARGET_CORTEX/, creates `rtos` Mbed library
|               +-- Include/
|                   +-- C                   # Content of rtos/source/TARGET_CORTEX/rtx5/Include/
|               +-- RTX/
|                   +-- Config/
|                       +-- D               # Content of rtos/source/TARGET_CORTEX/rtx5/RTX/Config/
|                   +-- Include/
|                       +-- E               # Content of rtos/source/TARGET_CORTEX/rtx5/RTX/Include/
|                   +-- Include1/
|                       +-- cmsis_os.h      # Currently in rtos/source/TARGET_CORTEX/rtx4/
|                   +-- Library/
|                       +-- cmsis_os1.c     # Currently in rtos/source/TARGET_CORTEX/rtx4/
|                   +-- Source/
|                       +-- F               # Content of rtos/source/TARGET_CORTEX/rtx5/RTX/Source/
|               +-- Source/
|                   +-- G                   # Content of rtos/source/TARGET_CORTEX/rtx5/Source
|           +-- TARGET_CORTEX_A/
|               +-- Include/
|                   +-- H                   # Header files currently in cmsis/TARGET_CORTEX_A/
|               +-- Source/
|                   +-- I                   # Source files currently in cmsis/TARGET_CORTEX_A/
|           +-- TARGET_CORTEX_M/
|               +-- Include/
|                   +-- J                   # Header files currently in cmsis/TARGET_CORTEX_M/
|               +-- Source/
|                   +-- K                   # Source files currently in cmsis/TARGET_CORTEX_M/
```

## Documentation
The README.md may contain a brief introduction to the component. Detailed documentation may be added under `docs` directory.
