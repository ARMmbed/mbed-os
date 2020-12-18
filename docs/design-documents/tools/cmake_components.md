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

## One object library "mbed-core"

Our current approach on feature-cmake branch is to use OBJECT libraries. We built almost entire tree of Mbed OS, the number of object files is big. As result building take longer and we have again windows path limitation (one file compilation command is more than 43k characters long).

To address the problem, CMake provides response files. They do not work out of the box as we experienced. Generators support them but they contain bugs. We found at least two bugs in CMake itself (Ninja, Make tested with Gcc Arm and ARMClang). We are still possibly having issues with other generators (more testing is required).

Note, we shall use response files with static libraries or any other solution we choose to avoid path limitation in OS.

These bugs will be fixed eventually. However, we still have not addressed the main problem - there are too many objects files to compile. We need a solution to split the tree into components and built only what is required. Therefore response files could be considered as a workaround for now until we get components in CMake.

## Only core libraries built as objects

These components would form mbed-core library built as OBJECT library in CMake:
- cmsis
- events
- rtos-api
- drivers
- hal
- targets
- platform

The rest of components and features could be static or object libraries. They could be selected by a user and be added on request.

## Components as static libraries

It is a known problem that weakly linked symbols are not resolved with strong symbols with static libraries. We won't disallow linking with static libraries, just internal Mbed OS components should be object libraries to avoid the problem with weakly symbols as many of the components rely on them.

### Using whole-archive to workaround weak symbols limitation

Toolchains provide a flag to enforce keeping symbols (GCC --whole-archive, clang -force_load). See the issue with forcing linker flag to the components https://github.com/zephyrproject-rtos/zephyr/issues/8441 and https://github.com/zephyrproject-rtos/zephyr/issues/6961 for details. This has drawbacks that will need more research. See also CMake issue https://gitlab.kitware.com/cmake/cmake/-/issues/20078 - not simple to fix in CMake.

`whole-archive` is only available to Gcc Arm, ARMClang does not provide it.

### Removing weak symbols

We could find alternative to weak symbols and fix them in our tree one by one. It was already achieved in Mbed OS 3 where we did not support weak symbols. 

### Object files for files with weak symbols

Mbed 2 was released as a library, we provided object files for files that were providing weak symbols (retarget, file handling and others). They were linked together with the mbed library. This however might not be achievable (its implementation in CMake might contain hacks to get other components paths and flags as retarget file depends on multiple other components).

## External components

They can be either object or static library. Only one limitation due to selecting object library is that any component linked by an application shall not have circular dependencies between the components (CMake will issue an error that we are linking with an object library (`mbed-core`) and it does not support it - this will be fixed but no ETA given).

## Solution: Mbed-os components

mbed-core component (object library) consists of:
- cmsis
- events
- rtos-api
- drivers
- hal
- targets
- platform

The rest of the tree is formed by components. 

Each component creates a new CMake library and adds includes/sources:

```
add_library(mbed-nanostack INTERFACE)

target_include_directories(mbed-nanostack 
    INTERFACE
        include
)

target_sources(mbed-nanostack 
    INTERFACE
        file.c
)
```

If there are dependencies for a component, use `target_link_libraries`. For instance, nanostack depends on 4 other components:

```
target_link_libraries(mbed-nanostack INTERFACE mbed-nanostack-libservice mbed-netsocket mbed-coap)
```

An application just links to what is required:

```
target_link_libraries(mbed-os-example-nanostack-example mbed-nanostack mbed-core)
```
