# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("DISCO_F746NG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_F746NG)
elseif("DISCO_F769NI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_DISCO_F769NI)
elseif("NUCLEO_F746ZG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F746ZG)
elseif("NUCLEO_F756ZG" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F756ZG)
elseif("NUCLEO_F767ZI" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_NUCLEO_F767ZI)
endif()

target_include_directories(mbed-emac
    INTERFACE
        .
)

target_sources(mbed-emac
    INTERFACE
        stm32f7_eth_conf.c
)
