# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

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
