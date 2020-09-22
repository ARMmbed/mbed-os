# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Set default core modules
# We use internal as they have to be prefixed with the MBED_ROOT
if(NOT DEFINED MBED_INTERNAL_COMPONENTS)
    set(MBED_INTERNAL_COMPONENTS cmsis drivers hal platform targets)
endif()

if(NOT DEFINED MBED_BAREMETAL)
    option(MBED_BAREMETAL "Build baremetal profile" OFF)
endif()

if(${MBED_BAREMETAL} STREQUAL OFF)
    list(APPEND MBED_INTERNAL_COMPONENTS rtos)
    # TODO: this needs some work, this interdependencies
    # mbedtls is needed by device key driver
    # events are needed by usb
    # storage is needed  by device key driver
    # mbed-trace needs libservice
    list(APPEND MBED_INTERNAL_COMPONENTS connectivity/mbedtls events storage connectivity/libraries/nanostack-libservice)
endif()

# Prefix all internal components with MBED_ROOT
list(TRANSFORM MBED_INTERNAL_COMPONENTS PREPEND "${MBED_ROOT}/")

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
endfunction()

# Enable Mbed OS component
function(mbed_enable_components components)
    foreach(component IN LISTS components)
        if(IS_DIRECTORY "${component}" AND EXISTS "${component}/CMakeLists.txt")
            add_subdirectory("${component}")
        endif()
    endforeach()
endfunction()
