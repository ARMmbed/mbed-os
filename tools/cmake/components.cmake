# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(_MBED_COMPONENTS_ENABLED GLOBAL PROPERTY)

# These 2 link functions are provided due to CMake behavior with object libraries.
# See https://gitlab.kitware.com/cmake/cmake/-/issues/18090 for details

function(mbed_link_component component_target)
    get_property(enabled_components GLOBAL PROPERTY _MBED_COMPONENTS_ENABLED)
    # No magic here, we just store what is being enabled. We later use this list
    # to link with mbed-os and an application
    foreach(component IN LISTS ARGN)
        # we dont need to track mbed-os as it will be always linked to with the app
        if(component STREQUAL "mbed-os")
            continue()
        endif()
        if(NOT component IN_LIST enabled_components)
            list(APPEND enabled_components ${component})
        endif()
    endforeach()
    set_property(GLOBAL PROPERTY _MBED_COMPONENTS_ENABLED ${enabled_components})
    # link with all arguments provided
    target_link_libraries(${component_target} ${ARGN})
endfunction()

function(mbed_link_application application_target)
    get_property(enabled_components GLOBAL PROPERTY _MBED_COMPONENTS_ENABLED)
    message("linking: " ${enabled_components})
    target_link_libraries(${application_target} ${enabled_components} ${ARGN} mbed-os)
endfunction()
