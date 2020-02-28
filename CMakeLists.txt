# This is not the application cmake, only boilerplate for Mbed OS

# TODO: what version should we require?
cmake_minimum_required(VERSION 3.9)

project("Mbed OS")

# System settings

enable_language(C CXX ASM)

set(CMAKE_SYSTEM_NAME MbedOS)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_CROSSCOMPILING TRUE)

# if the environment does not specify build type, set to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release"
        CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

# TODO: Verbose build, should also be under condition
# set(CMAKE_VERBOSE_MAKEFILE ON)

# force compiler settings
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Compiler setup
set(CMAKE_ASM_COMPILER    "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER      "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER    "arm-none-eabi-g++")
set(ELF2BIN               "arm-none-eabi-objcopy")


# Add mbed.h file
set(MBED_HEADER_FILE mbed.h)
add_library(mbed_os ${MBED_HEADER_FILE})
target_include_directories (mbed_os
    PUBLIC
      .
)

# Baremetal build as default, if we want more, app needs to request it

add_subdirectory(cmsis)
add_subdirectory(drivers)
add_subdirectory(events)
add_subdirectory(hal)
add_subdirectory(platform)
add_subdirectory(targets)

# Selectable components

# TODO: use subdirectory_ifdef or similar own function based on config

# add_subdirectory(rtos)
# add_subdirectory(components)
# add_subdirectory(features/mbedtls)
# add_subdirectory(features/cellular)
# add_subdirectory(features/frameworks)
# add_subdirectory(features/lorawan)
# add_subdirectory(features/lwipstack)
# add_subdirectory(features/nanostack)
# add_subdirectory(features/netsocket)
# add_subdirectory(features/nfc)
# add_subdirectory(features/storage)
# add_subdirectory(features/device_key)
