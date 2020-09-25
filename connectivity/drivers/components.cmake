# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

get_property(_internal_components GLOBAL PROPERTY mbed-os-internal-components)

list(APPEND components-drivers
    mbed-os-nfc-pn512
)

# Only add once
foreach(component IN LISTS components-drivers)
    if(NOT component IN_LIST _internal_components)
        list(APPEND _internal_components ${component})
    endif()
endforeach()

set_property(GLOBAL PROPERTY mbed-os-internal-components ${_internal_components})

# Components paths to the main CMake file
set(mbed-os-nfc-pn512_PATH connectivity/drivers/nfc/PN512)
