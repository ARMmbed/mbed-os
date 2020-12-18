# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("EFR32_12" IN_LIST MBED_TARGET_LABELS)
    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIB_RAIL efr32-rf-driver/rail/TARGET_EFR32_12/librail_efr32xg12_release.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIB_RAIL efr32-rf-driver/rail/TARGET_EFR32_12/librail_efr32xg12_release.a)
    endif()
endif()

target_link_libraries(mbed-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_RAIL})


target_include_directories(mbed-core
    INTERFACE
        efr32-rf-driver/rail
        efr32-rf-driver/rail/ble
        efr32-rf-driver/rail/ieee802154
)
