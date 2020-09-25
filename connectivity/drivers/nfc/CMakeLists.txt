# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/tools/cmake/components.cmake)

add_library(mbed-os-nfc-pn512 OBJECT)

mbed_configure_component(mbed-os-nfc-pn512c)

add_subdirectory(PN512)

target_link_libraries(mbed-os-nfc-pn512 mbed-os mbed-os-nfc)
