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

# Gather all non-core components from the Mbed OS tree
include(connectivity/components.cmake)
include(storage/components.cmake)

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

# Enable Mbed OS component
function(mbed_enable_components components)
    foreach(component IN LISTS components)
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
