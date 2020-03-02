# CMake Mbed OS

Two steps approach:

- Mbed OS core CMake
- Application CMake

All what can be defined should be in CMake files and configuration (app/mbed .json files). Our build system would parse the configuration, create rules and generate top level CMake file that would include others (application CMake files plus also Mbed OS core CMake).

To stay backward compatible, we create common rules as follows.

We replace current `TOOLCHAIN_`, `FEATURE_`, `TARGET_` by providing `add_subdirectory` macros. There would be `if_target()`, `if_feature()`, `_if_toolchain()` macros for better readability.

Example, to add stm_directory only for STM targets (current rule TARGET_STM):

```
add_subdirectory_if_target(STM stm_directory)

```

Application example:

```
./TARGET_STM/qspi/driver_stm_qspi.cpp
./TARGET_STM/peripheral_specific_to_stm/stm_specific_driver.cpp
```

As result, the top level application CMake:

```
add_subdirectory_if_target(STM TARGET_STM/qspi)
add_subdirectory_if_target(STM TARGET_STM/peripheral_specific_to_stm)

```

There would be static CMakes in the `./TARGET_STM/qspi/` and `./TARGET_STM/peripheral_specific_to_stm` that would define what files should be included or specific settins for these modules.

To migrate to the new build system, we can provide auto scanning of the module and generate CMake based on what we find or use the way as described above. In both cases, we could stay backward compatible.

## Mbed OS Core (Mbed OS repository)

There are couple of CMakes in the tree.

1. Mbed OS boiler plate defined in Mbed OS root (provides way for build system to overwrite/add configuration)
2. Mbed OS Toolchain settings (toolchain.cmake) that would get generated based on the toolchain selected
3. Each module has own CMake

The next sections will describe static CMake files within Mbed OS Core repository.

### 1. Boilerplate CMake

The main CMake file in Mbes OS repository provides just boilerplate for Mbed OS to be built.

### 2. Toolchain CMake

There are 3 toolchains, we provide the template that tools could fill in. The information is already in the build tools, we just need to extract that info and make it CMake compatible.

This toolchain CMake is included by the Mbed OS CMake.

### 3. Module CMake

This file statically defines the structure of the module within Mbed OS. It's conditionally config based. We use regular CMake expressions plus extend it with own macros to conditionally include/exclude directories.

`add_subdirectory` always adds the directory to the main CMake.

```
add_subdirectory(drivers)
```

Conditionally directories addition based on the config:

```
add_subdirectory_if_config(CONFIG_REQUIRES_RTOS rtos)
```

Conditionally directories addition based on the target/toolchain/feature.


```
add_subdirectory_if_target(STM targets\TARGET_STM)
add_subdirectory_if_toolchain(GCC_ARM cmsis\TARGET_GCC_ARM)
add_subdirectory_if_feature(BLE ble\FEATURE_BLE)
```


## Application CMake

We should provide application CMake functionality with  our own configuration. There are couple of approaches we could take. Statically defined CMake but then this disconnectes config and CMake - as CMake contains configuration for a project (like includes, sources, etc). Our build tool would need to parse CMake to get all paths used in the project or Mbed OS to find out where to look for configuration file.

A user create own CMake file to configure an application, also with `mbed_app.json` configuration file. The building of an app would look like:

1. Parse the arguments provided to build command
2. Parse the application configuration
3. Get the target configuration
4. Get the Mbed OS configuration
5. Create the toolchain.cmake file
6. Inject all previous steps to the Mbed OS Core CMake
7. Build an application
