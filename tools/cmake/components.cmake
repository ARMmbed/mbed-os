# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Create global values, components below extend the list
set_property(GLOBAL PROPERTY mbed-os-internal-components)
set_property(GLOBAL PROPERTY mbed-os-internal-components-enabled)

# Set default core components. They depend on each other.
if(NOT DEFINED MBED_CORE_COMPONENTS)
    set(MBED_CORE_COMPONENTS 
        events
        cmsis
        drivers
        hal
        platform
        targets
    )
endif()

if(NOT DEFINED MBED_BAREMETAL)
    option(MBED_BAREMETAL "Build baremetal profile" OFF)
endif()

if(${MBED_BAREMETAL} STREQUAL OFF)
    list(APPEND MBED_CORE_COMPONENTS 
        rtos
    )
endif()

#
# Configure Mbed OS CMake component (default).
#
function(mbed_configure_component component)
    mbed_set_cpu_core_options(${component} ${MBED_TOOLCHAIN})
    mbed_set_toolchain_options(${component})
    mbed_set_c_lib(${component} ${MBED_C_LIB})
    mbed_set_printf_lib(${component} ${MBED_PRINTF_LIB})
    mbed_set_language_standard(${component})
    mbed_set_profile_options(${component} ${MBED_TOOLCHAIN})
    # Add definitions for each component
    target_compile_definitions(${component}
        PUBLIC
            ${MBED_TARGET_DEFINITIONS}
            ${MBED_CONFIG_DEFINITIONS}
    )
endfunction()

# Enable core Mbed OS componens (from MBED_CORE_COMPONENTS)
function(mbed_enable_core_components)
    foreach(component IN LISTS MBED_CORE_COMPONENTS)
        add_subdirectory(${component})
    endforeach()

    if(rtos IN_LIST MBED_CORE_COMPONENTS)
        target_compile_definitions(mbed-os
            PUBLIC
                MBED_COMPONENT_RTOS_ENABLED
        )
    endif()
endfunction()

# Enable Mbed OS component, used by an application to enable Mbed OS component.
# Boards can enable/disable components, a user should be able to overwrite it
# This is not for an external component oustide of Mbed OS.
# Arguments expected:
#   ARGV - list of components to be enabled
function(mbed_enable_components)
    # Gather all non-core components from the Mbed OS tree
    # Only include it in enable components
    include(${MBED_ROOT}/connectivity/components.cmake)

    # Find all components and add them to the application
    get_property(_internal_components GLOBAL PROPERTY mbed-os-internal-components)
    get_property(_enabled_components GLOBAL PROPERTY mbed-os-internal-components-enabled)
    foreach(component IN LISTS ARGV)
        if(NOT component IN_LIST _internal_components)
            message(ERROR
                " Component ${component} is not available. Available components are: ${_internal_components}"
            )
        endif()
        # Add to the list to enable right before linking
        list(APPEND _enabled_components ${component})
    endforeach()
    set_property(GLOBAL PROPERTY mbed-os-internal-components-enabled ${_enabled_components})
endfunction()

# Disable Mbed OS component, used by an application to disable Mbed OS component.
# Arguments expected:
#   ARGV - list of components to be disabled
function(mbed_disable_components)
    get_property(_enabled_components GLOBAL PROPERTY mbed-os-internal-components-enabled)
    # Find all components and remove them
    foreach(component IN LISTS ARGV)
        if(NOT component IN_LIST _enabled_components)
            message(ERROR
                " Component ${component} has not been enabled. Enabled components are: ${_enabled_components}"
            )
        endif()

        get_property(_enabled_components GLOBAL PROPERTY mbed-os-internal-components-enabled)
        list(REMOVE_ITEM _enabled_components ${component})
        set_property(GLOBAL PROPERTY mbed-os-internal-components-enabled ${_enabled_components})
    endforeach()
endfunction()

# Use this function to link with Mbed OS and its components. This is just a wrapper on
# top of target_link_libraries
# Arguments expected:
# app_target - the application target
# ARGN - list of external components to link with app_target + mbed-os + mbed-os components
function(mbed_target_link_libraries app_target)
    # get paths for components
    include(${MBED_ROOT}/connectivity/components.cmake)

    get_property(_enabled_internal_components GLOBAL PROPERTY mbed-os-internal-components-enabled)

    foreach(component IN LISTS _enabled_internal_components)
        if (${component}_PATH)
            set(component_path ${MBED_ROOT}/${${component}_PATH})
        else()
            message(ERROR 
                "${component}_PATH not found"
            )
        endif()
        if(IS_DIRECTORY "${component_path}" AND EXISTS "${component_path}/CMakeLists.txt")
            add_subdirectory("${component_path}")

            string(REPLACE "-" "_" component ${component})
            string(TOUPPER ${component} component_uppercase)
            target_compile_definitions(mbed-os
                PUBLIC
                    "MBED_COMPONENT_${component_uppercase}_ENABLED"
            )
        else()
            message(ERROR
                " Component path ${component_path} not found."
            )
        endif()
    endforeach()
    target_link_libraries(${app_target} ${_enabled_internal_components} ${ARGN} mbed-os)
endfunction()
