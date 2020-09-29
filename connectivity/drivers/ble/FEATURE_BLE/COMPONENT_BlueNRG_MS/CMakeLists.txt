# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-ble-bluenrg OBJECT)

target_sources(mbed-os-ble-bluenrg
    PRIVATE
        BlueNrgMsHCIDriver.cpp
)

mbed_link_component(mbed-os-ble-bluenrg mbed-os-ble mbed-os)
