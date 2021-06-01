# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TARGET_STM32F437xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F437xG)
elseif("STM32F439xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F439xI)
elseif("STM32F756xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32F756xG)
elseif("STM32L443xC" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L443xC)
elseif("STM32L486xG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L486xG)
elseif("STM32L4S5xI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L4S5xI)
elseif("STM32L562xx" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32L562xx)
elseif("STM32WB" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_STM32WB)
endif()

target_sources(mbed-mbedtls
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

target_include_directories(mbed-mbedtls
    INTERFACE
        .
)
