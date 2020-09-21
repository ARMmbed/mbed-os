# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Enable Mbed OS component
function(mbed_enable_components components)
    foreach(component IN LISTS components)
        if(IS_DIRECTORY "${component}" AND EXISTS "${component}/CMakeLists.txt")
            add_subdirectory("${component}")
        endif()
    endforeach()
endfunction()
