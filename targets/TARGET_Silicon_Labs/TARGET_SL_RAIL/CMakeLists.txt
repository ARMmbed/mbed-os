# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-sl-rail-efr32-12 INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LIB_RAIL efr32-rf-driver/rail/TARGET_EFR32_12/librail_efr32xg12_release.ar)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LIB_RAIL efr32-rf-driver/rail/TARGET_EFR32_12/librail_efr32xg12_release.a)
endif()

target_link_libraries(mbed-sl-rail-efr32-12 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_RAIL})


target_include_directories(mbed-sl-rail-efr32-12
    INTERFACE
        efr32-rf-driver/rail
        efr32-rf-driver/rail/ble
        efr32-rf-driver/rail/ieee802154
)
