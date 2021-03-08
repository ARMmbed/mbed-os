# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nu-m23-s-tfm INTERFACE)
target_include_directories(mbed-nu-m23-s-tfm
    INTERFACE
        TARGET_TFM
)
target_sources(mbed-nu-m23-s-tfm
    INTERFACE
        TARGET_TFM/spm_hal.c
        TARGET_TFM/target_cfg.cpp
)

add_library(mbed-m23-s INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M2351.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/M2351.sct)
endif()

mbed_set_linker_script(mbed-m23-s ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-m23-s INTERFACE mbed-2351)
