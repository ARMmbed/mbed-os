# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-ble-cordio OBJECT)

if("CORDIO" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_CORDIO_LL)
endif()

target_link_libraries(mbed-os-ble mbed-os-ble-cordio)

add_subdirectory(cordio_stack)

target_include_directories(mbed-os-ble-cordio
    PUBLIC
        .
)
