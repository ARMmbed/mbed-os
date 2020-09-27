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

# Internal functionality
# Enable core Mbed OS componens (from MBED_CORE_COMPONENTS)
function(_mbed_enable_core_components)
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

function(mbed_enable_components_as_dependency)
    mbed_enable_components(${ARGV})
    # check what was actually enabled from the deps and add them
    get_property(_enabled_components GLOBAL PROPERTY mbed-os-internal-components-enabled)
    foreach(component IN LISTS ARGV)
        set(component_path ${MBED_ROOT}/${${component}_PATH})
        add_subdirectory("${component_path}")
    endforeach()
endfunction()

# Enable Mbed OS component, used by an application to enable Mbed OS component or a component
# to enable its dependencies.
# Boards can enable/disable components, a user should be able to overwrite it
# This is not for an external component oustide of Mbed OS.
# Arguments expected:
#   ARGV - list of components to be enabled
function(mbed_enable_components)
    # Gather all non-core components from the Mbed OS tree
    # Only include it in enable components
    include(${MBED_ROOT}/connectivity/components.cmake)
    include(${MBED_ROOT}/connectivity/drivers/components.cmake)

    # Find all components and add them to the applicati on
    get_property(_internal_components GLOBAL PROPERTY mbed-os-internal-components)
    get_property(_enabled_components GLOBAL PROPERTY mbed-os-internal-components-enabled)
    foreach(component IN LISTS ARGV)
        if(NOT component IN_LIST _internal_components)
            message(ERROR
                " Component ${component} is not available. Available components are: ${_internal_components}"
            )
        endif()
        if(NOT component IN_LIST _enabled_components)
            list(APPEND _enabled_components ${component})
        endif()
    endforeach()
    set_property(GLOBAL PROPERTY mbed-os-internal-components-enabled ${_enabled_components})
endfunction()

# Disable Mbed OS component, used by an application to disable Mbed OS component.
# Be careful as there can be a component that depends on component being removed
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
    include(${MBED_ROOT}/connectivity/drivers/components.cmake)

    get_property(_enabled_internal_components GLOBAL PROPERTY mbed-os-internal-components-enabled)

    # Contains up-to-date list of components to be added
    list(APPEND components_to_be_enabled ${_enabled_internal_components})

    # Continue adding until we resolve all dependencies (components_to_be_enabled is empty)
    while(components_to_be_enabled)
        foreach(component IN LISTS _enabled_internal_components)
            # check if we resolved this dependency already
            if(component IN_LIST components_already_enabled)
                continue()
            endif()

            if (DEFINED ${component}_PATH)
                set(component_path ${MBED_ROOT}/${${component}_PATH})
            else()
                message(ERROR 
                    "${${component}_PATH} not found"
                )
            endif()
            if(IS_DIRECTORY "${component_path}" AND EXISTS "${component_path}/CMakeLists.txt")
                add_subdirectory("${component_path}")

                string(REGEX REPLACE "mbed-os-" "" no_prefix_component ${component})
                string(REPLACE "-" "_" no_prefix_component ${no_prefix_component})
                string(TOUPPER ${no_prefix_component} no_prefix_component)
                target_compile_definitions(mbed-os
                    PUBLIC
                        "MBED_COMPONENT_${no_prefix_component}_ENABLED"
                )
            else()
                message(ERROR
                    " Component path ${component_path} not found."
                )
            endif()
            list(APPEND components_already_enabled ${component})
            list(REMOVE_ITEM components_to_be_enabled ${component})
        endforeach()

        # Resolve dependencies added after foreach above. Find a component we haven't
        # yet added and append it to our components_to_be_enabled list for enabling
        get_property(_enabled_internal_components GLOBAL PROPERTY mbed-os-internal-components-enabled)
        foreach(component IN LISTS _enabled_internal_components)
            if(NOT component IN_LIST components_already_enabled)
                list(APPEND components_to_be_enabled ${component})
            endif()
        endforeach()
    endwhile()

    message("Linking these: " "${components_already_enabled}")
    target_link_libraries(${app_target} ${components_already_enabled} ${ARGN} mbed-os)
endfunction()
