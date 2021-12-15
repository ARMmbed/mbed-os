# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_WICED EXCLUDE_FROM_ALL)

add_subdirectory(TARGET_STM EXCLUDE_FROM_ALL)

if("WHD" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(COMPONENT_WHD EXCLUDE_FROM_ALL)
endif()

add_subdirectory(esp8266-driver)

target_link_libraries(mbed-wifi
    INTERFACE
        mbed-netsocket
)
