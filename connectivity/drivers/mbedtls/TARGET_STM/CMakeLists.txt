# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-tls-stm INTERFACE)

add_subdirectory(TARGET_STM32F437xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F439xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32F756xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L443xC EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L486xG EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32L4S5xI EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_STM32WB55xx EXCLUDE_FROM_ALL)

target_sources(mbed-tls-stm
    INTERFACE
        aes_alt.cpp
        aes_alt_stm32l4.c
        ccm_alt.cpp
        cryp_stm32.c
        gcm_alt.cpp
        hash_stm32.c
        md5_alt.cpp
        sha1_alt.cpp
        sha256_alt.cpp
)

target_include_directories(mbed-tls-stm
    INTERFACE
        .
)
