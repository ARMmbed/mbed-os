# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND MBED_COMPONENTS_LIST mbed-ble mbed-cellular mbed-nfc mbed-nanostack mbed-netsockets mbed-wifi)

# Add libraries to specified components
# This can be used to link additional libraries that are required
# ARGN is a list of libraries to add to mbed-<component>
function(mbed_add_component_libraries component)
    # we use global symbols, using upper case
    string(TOUPPER ${component} component_upper)
    if(component IN_LIST MBED_COMPONENTS_LIST)
        set(${component_upper}_EXTRA_LIBRARIES "${${component}_EXTRA_LIBRARIES};{ARGN}" CACHE INTERNAL "")
    else()
        message("Mbed OS components are: ${MBED_COMPONENTS_LIST}")
        message(FATAL_ERROR "Component: ${component} not found")
    endif()
endfunction()
