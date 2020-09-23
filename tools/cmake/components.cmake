# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Set default core components. They depend on each other.
# We use internal as they have to be prefixed with the MBED_ROOT
if(NOT DEFINED MBED_CORE_COMPONENTS)
    set(MBED_CORE_COMPONENTS drivers hal platform targets cmsis events)
endif()

if(NOT DEFINED MBED_BAREMETAL)
    option(MBED_BAREMETAL "Build baremetal profile" OFF)
endif()

if(${MBED_BAREMETAL} STREQUAL OFF)
    list(APPEND MBED_CORE_COMPONENTS rtos)
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

# Enable Mbed OS component
function(mbed_enable_components components)
    foreach(component IN LISTS components)
        if(IS_DIRECTORY "${component}" AND EXISTS "${component}/CMakeLists.txt")
            add_subdirectory("${component}")
        endif()
    endforeach()
endfunction()
