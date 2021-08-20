# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_WICED EXCLUDE_FROM_ALL)

add_subdirectory(COMPONENT_WHD EXCLUDE_FROM_ALL)

add_subdirectory(esp8266-driver)

target_link_libraries(mbed-wifi
    INTERFACE
        mbed-netsocket
)
