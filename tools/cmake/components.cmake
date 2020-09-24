# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

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

# Prefix all internal components with MBED_ROOT
list(TRANSFORM MBED_CORE_COMPONENTS PREPEND "${MBED_ROOT}/")

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
        # if we find ${component}_PATH, we use it for specified component name
        # otherwise name = path
        set(component_path ${component})
        if (${component}_PATH)
            set(component_path ${component}_PATH)
        endif()
        if(IS_DIRECTORY "${component_path}" AND EXISTS "${component_path}/CMakeLists.txt")
            add_subdirectory("${component_path}")
        endif()
    endforeach()
endfunction()

# Enable Mbed OS component, used by an application to enable Mbed OS component.
# This is not for an external component oustide of Mbed OS, it is application
# responsibility to include and link to it
function(mbed_enable_components components)
    # Create a value in cache, components below extend the list
    list(APPEND mbed-os-internal-components CACHE INTERNAL)

    # Gather all non-core components from the Mbed OS tree
    include(${MBED_ROOT}/connectivity/components.cmake)

    # Find all components and add them to the application
    foreach(component IN LISTS components)
        if(NOT component IN_LIST mbed-os-internal-components)
            message(ERROR
                " Component ${component} is not available. Available components are: ${mbed-os-internal-components}"
            )
        endif()

        # if we find ${component}_PATH, we use it for the specified component name
        # otherwise name is equaled to a path
        set(component_path ${component})
        if (${component}_PATH)
            set(component_path ${${component}_PATH})
        endif()
        set(component_path ${MBED_ROOT}/${component_path})
        if(IS_DIRECTORY "${component_path}" AND EXISTS "${component_path}/CMakeLists.txt")
            # only adding a directory. We can't link here otherwise there will be circular depedency
            # with Mbed OS - not really needed. An application links components together as it needs
            # to request them (enable + link)
            add_subdirectory("${component_path}")
        endif()
    endforeach()
endfunction()
