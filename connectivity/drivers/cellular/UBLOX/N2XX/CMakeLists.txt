# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-cellular-ublox-n2xx OBJECT)

target_include_directories(mbed-os-cellular-ublox-n2xx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os-cellular-ublox-n2xx
    PRIVATE
        UBLOX_N2XX.cpp
        UBLOX_N2XX_CellularContext.cpp
        UBLOX_N2XX_CellularNetwork.cpp
        UBLOX_N2XX_CellularSMS.cpp
        UBLOX_N2XX_CellularStack.cpp
)

mbed_link_component(mbed-os-cellular-ublox-n2xx mbed-os-cellular mbed-os)
