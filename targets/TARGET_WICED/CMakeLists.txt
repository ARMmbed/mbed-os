# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("WIO_EMW3166" IN_LIST MBED_TARGET_LABELS)
    if(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIB_WICED_DRIVERS TOOLCHAIN_ARMC6/TARGET_WIO_EMW3166/libwiced_drivers.ar)
    elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIB_WICED_DRIVERS TOOLCHAIN_GCC_ARM/TARGET_WIO_EMW3166/libwiced_drivers.a)
    endif()
endif()

target_link_libraries(mbed-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_WICED_DRIVERS})

target_include_directories(mbed-core
    INTERFACE
        wiced_interface
)

target_sources(mbed-core
    INTERFACE
        wiced_interface/default_wifi_interface.cpp
)
