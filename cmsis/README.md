
# CMSIS Version 6

This directory contains
 - submodule [CMSIS 6.2.0](https://arm-software.github.io/CMSIS_6/v6.2.0/General/index.html)
 - submodule [CMSIS RTX 5.9](https://arm-software.github.io/CMSIS-RTX/v5.9.0/index.html)
 - Mbed specific configuration and boot source files for RTOS.

None: All submodules are without any intervention for future update

The directory is organised as follows:

```
+-- cmsis
|   +-- README.md                               # The present mardown file
|   +-- device/
|       +-- mbed_cmsis_conf.h                   # Process stack configuration
|       +-- RTE/
|           +-- include/
|               +-- RTE_Components.h            # Run-Time-Environment Component Configuration File
|       +-- rtos/
|           +-- mbed_lib.json                   # Mbed library configuration file
|           +-- include/
|               +-- mbed_boot.h                 # Declares the functions that run before main()
|               +-- cmsis_os.h                  # Declares old cmsis_os header
|               +-- RTX/
|                   +-- mbed_rtx_conf.h         # Changes to RTX configuration
|                   +-- mbed_rtx_storage.h      # Declares the primitives storage types for RTX
|                   +-- RTX_Config.h            # RTX configuration file for CMSIS-RTX
|           +-- source/
|               +-- cmsis_os1.c                 # Implements old cmsis_os functions
|               +-- mbed_boot.c                 # Implements some of the functions that run before main()
|               +-- mbed_tz_context.c           # Implements of cmsis-core tz context functions
|               +-- RTX/
|                   +-- mbed_rtos_rtx.c         # Implements some of the functions that run before main()
|                   +-- mbed_rtx_handlers.c     # Implements RTX handlers
|                   +-- mbed_rtx_idle.cpp       # Implements RTX idle loop
|           +-- TOOLCHAIN_GCC_ARM/
|               +-- mbed_boot_gcc_arm.c         # Mbed entry point for the GCC_ARM toolchain
|   +-- CMSIS_cmake/
|       +-- CMakeLists.txt
|       +-- cmsis_core.cmake                    # CMake script that includes CMSIS Core from CMSIS_6 submodule
|       +-- cmsis_rtos_api.cmake                # CMake script that includes CMSIS RTOS2 API from CMSIS_6 submodule
|       +-- cmsis_rtos_backend.cmake            # CMake script that select RTOS backend RTX (default), FreeRTOS (future)
|       +-- cmsis_rtos_rtx.cmake                # CMake script that includes CMSIS-RTX submodule
|   +-- CMSIS_6/                                # Imported from the upstream
|   +-- CMSIS-RTX/                              # Imported from the upstream 
```

Three Mbed libraries live within this directory:
* `mbed-core-flags`     that includes RTOS headers what are necessry for full and also bare-metal profiles
* `mbed-rtos-flags`     that includes RTOS header for full profile only
* `mbed-rtos-sources`   that brings all source files for full profile only
