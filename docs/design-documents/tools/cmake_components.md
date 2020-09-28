# Creating components with CMake

One of the main goals to achieve with CMake is to have components in Mbed OS. A modular built allowing users to select what they require, not building everything as we used to with old tools.

## Components in Mbed OS (bare-metal)

Mbed OS consist of:
- cmsis
- rtos-api
- drivers
- platform
- hal
- targets

Their dependencies:

cmsis: 
None

drivers:
- platform
- rtos-api
- hal
- targets
- events

rtos-api:
- targets
- platform

platform:
- rtos-api
- hal
- targets

hal:
- targets

targets:
- hal
- drivers
- platform
- rtos-api
- cmsis

Breaking the dependencies would be a huge effort and it possibly would result in rewriting these components as they were for years considered as monolitic - having everything available.

## Components as object libraries

CMake provides OBJECT libraries but it does not support circular dependencies that we have in our tree. Therefore we build Mbed OS as whole (all object files combined).

## One object library "mbed-os"

Our current approach on feature-cmake branch is to use OBJECT libraries. We built almost entire tree of Mbed OS, the number of object files is big. As result building take longer and we have again windows path limitation (one file compilation command is more than 43k characters long).

To address the problem, CMake provides response files. They do not work out of the box as we experienced. Generators support them but they contain bugs. We found at least two bugs in CMake itself (Ninja, Make tested with Gcc Arm and ARMClang). We are still possibly having issues with other generators (more testing is required).

Note, we shall use response files with static libraries or any other solution we choose to avoid path limitation in OS.

These bugs will be fixed eventually. However, we still haven not address the main problem - there are too many objects files to compile. We need a solution to split the tree into components and built only what is required. Therefore response files could be considered as a workaround for now until we get components in CMake.

## Only core libraries built as objects

These components would form mbed-os library built as OBJECT library in CMake:
- cmsis
- events
- rtos-api
- drivers
- hal
- targets
- platform

The rest of components and features could be static or object libraries. They could be selected by a user and be added on request.

## Components as static libraries

It is known problem with static libraries - weakly linked symbols are not resolved with strong symbols as someone would have expected. We won't disallow linking with static libraries, just internal Mbed OS components should be object libraries to avoid the problem with weakly symbols as many of the components rely on them.

### Using whole-archive to workaround weak symbols limitation

Toolchains provide a flag to enforce keeping symbols (GCC --whole-archive, clang -force_load). See the issue with forcing linker flag to the components https://github.com/zephyrproject-rtos/zephyr/issues/8441 and https://github.com/zephyrproject-rtos/zephyr/issues/6961 for details. This has drawbacks that will need more research. See also CMake issue https://gitlab.kitware.com/cmake/cmake/-/issues/20078 - not simple to fix in CMake.

`whole-archive` is only available to Gcc Arm, ARMClang does not provide it.

### Removing weak symbols

We could find alternative to weak symbols and fix them in our tree one by one. It was already achieved in Mbed OS 3 where we did not support weak symbols. 

### Object files for files with weak symbols

Mbed 2 was released as a library, we provided object files for files that were providing weak symbols (retarget, file handling and others). They were linked together with the mbed library. This however might not be achievable (its implementation in CMake might contain hacks to get other components paths and flags as retarget file depends on multiple other components).

## External components

They can be either object or static library. Only one limitation due to selecting object library is that any component linked by an application shall not have circular dependencies between the components (CMake will issue an error that we are linking with an object library (mbed-s) and it does not support it - this will be fixed but no ETA given).

## Solution: Mbed-os components

mbed-os component (object library) consists of:
- cmsis
- events
- rtos-api
- drivers
- hal
- targets
- platform

The rest of the tree is formed by components. There are components.cmake files in the tree.

Each component.cmake must add components to `mbed-os-internal-components` and add component_PATH variable. Cmake uses these path variables to find the component.

An example how connectivity components.cmake can look like:

```
get_property(_internal_components GLOBAL PROPERTY mbed-os-internal-components)

list(APPEND connectivity_components
    mbed-os-cellular
    mbed-os-ble
    mbed-os-lorawan
    mbed-os-mbedtls
    mbed-os-nanostack
    mbed-os-netsocket
    mbed-os-nfc
    mbed-os-coap
    mbed-os-nanostack-libservice
)

# Only add once
foreach(component IN LISTS connectivity_components)
    if(NOT component IN_LIST _internal_components)
        list(APPEND _internal_components ${component})
    endif()
endforeach()

set_property(GLOBAL PROPERTY mbed-os-internal-components ${_internal_components})

set(mbed-os-ble_PATH connectivity/FEATURE_BLE)
set(mbed-os-nfc_PATH connectivity/nfc)
set(mbed-os-cellular_PATH connectivity/cellular)
set(mbed-os-lorawan_PATH connectivity/lorawan)
set(mbed-os-mbedtls_PATH connectivity/mbedtls)
set(mbed-os-nanostack_PATH connectivity/nanostack)
set(mbed-os-netsocket_PATH connectivity/netsocket)
set(mbed-os-coap_PATH connectivity/libraries/mbed-coap)
set(mbed-os-nanostack-libservice_PATH connectivity/libraries/nanostack-libservice)
```

Component CMakelists must configure mbed-os component:

```
include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-nanostack OBJECT)

mbed_configure_component(mbed-os-nanostack)
```

If there are dependencies for a component, use `mbed_enable_component`. For instance, nanostack depends on 3 components:

```
set(mbed-os-nanostack_DEPENDS mbed-os-coap mbed-os-nanostack-libservice mbed-os-netsocket)
mbed_enable_components(${mbed-os-nanostack_DEPENDS})

target_link_libraries(mbed-os-nanostack mbed-os-nanostack-libservice mbed-os-netsocket mbed-os-coap mbed-os)
```

The application can enable/disable a components. It must do it prior linking an application with MBed OS, use `mbed_target_link_libraries` to link all together. 

The function enables requested components and links them together with an application and mbed-os.

```
mbed_enable_components(mbed-os-nfc)
mbed_target_link_libraries(mbed-os-example-nfc)
```
