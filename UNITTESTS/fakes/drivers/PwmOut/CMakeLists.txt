# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-fakes-drivers-pwmout)

target_compile_definitions(mbed-fakes-drivers-pwmout 
    PRIVATE
        DEVICE_PWMOUT
)

target_sources(mbed-fakes-drivers-pwmout
    PRIVATE
        ${MBED_PATH}/drivers/source/PwmOut.cpp
)

target_link_libraries(mbed-fakes-drivers-pwmout
    PRIVATE
        mbed-headers        
)
