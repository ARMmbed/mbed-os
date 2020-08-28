# CMSIS component source structure recommendations
Date: 2020-08-19

## Status
Proposed

## Context
[`ARM-software/CMSIS_5`](https://github.com/ARM-software/CMSIS_5) has been ported to Mbed OS. However, it is split across multiple directories and it is not immediately obvious that it comes from that repository when imported with the [importer script](../../tools/importer/importer.py ).

## Decision
The current proposal is to create new directories within the existing `cmsis/` directory:
* `CMSIS_5/`: mirrors closely the structure used by `ARM-software/CMSIS_5/` keeping only the directories and files needed by Mbed OS and renames some directories in order to work with Mbed OS build tools. See [importer configuration file](./../tools/importer/cmsis_importer.json).
* `device`: includes Mbed OS specific files to configure RTX.

This will result in the removal of the `rtos/source/TARGET_CORTEX/` directory.
Additionally, `cmsis/TARGET_CORTEX_A/TOOLCHAIN_IAR/cmain.S` to `platform/source/TARGET_CORTEX_A/TOOLCHAIN_IAR/cmain.S` so it can be accessible when building with the bare metal profile. Note that we already have the equivalent file for TARGET_CORTEX_M at `platform/source/TARGET_CORTEX_M/TOOLCHAIN_IAR/cmain.S`.

It will provide the following advantages:
* Better maintenance of the CMSIS component
* Easy creation of an independent CMake target that can be built as a library that can be optionally be linked against by end users
* Easy source navigation that mirrors closely the upstream CMSIS_5 repository

## Documentation
A `README.md` within the `cmsis` will contain a brief introduction of the component and will detail the Mbed specific files added.