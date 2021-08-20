# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32625-boot INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE_BOOT TOOLCHAIN_GCC_ARM/TARGET_MAX32625_BOOT/max32625.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE_BOOT TOOLCHAIN_ARM_STD/TARGET_MAX32625_BOOT/MAX32625.sct)
endif()

mbed_set_linker_script(mbed-max32625-boot ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_BOOT})

target_link_libraries(mbed-max32625-boot INTERFACE mbed-max32625)

add_library(mbed-max32625-no-boot INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE_NO_BOOT TOOLCHAIN_GCC_ARM/TARGET_MAX32625_NO_BOOT/max32625.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE_NO_BOOT TOOLCHAIN_ARM_STD/TARGET_MAX32625_NO_BOOT/MAX32625.sct)
endif()

mbed_set_linker_script(mbed-max32625-no-boot ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE_NO_BOOT})

target_link_libraries(mbed-max32625-no-boot INTERFACE mbed-max32625)
