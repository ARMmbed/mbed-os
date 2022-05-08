# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(TARGET mbed-ep-agora)
    set(FLASH_TARGET_CONFIG_HEADER NORDIC/EP_AGORA)
elseif(TARGET mbed-nrf52840-dk)
    set(FLASH_TARGET_CONFIG_HEADER NORDIC/NRF52840_DK)
elseif(TARGET mbed-k82f)
    set(FLASH_TARGET_CONFIG_HEADER NXP/K82F)
elseif(TARGET mbed-lpc546xx)
    set(FLASH_TARGET_CONFIG_HEADER NXP/LPC546XX)
elseif(TARGET mbed-efm32gg11-stk3701)
    set(FLASH_TARGET_CONFIG_HEADER SiliconLabs/EFM32GG11_STK3701)
endif()

if(NOT "DEVICE_QSPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "QSPI is not supported for this target")
endif()


list(
    APPEND
        TEST_HEADER_DIRS
            flash_configs
            flash_configs/${FLASH_TARGET_CONFIG_HEADER}
)

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-qspi
	TEST_SOURCES
		main.cpp
        qspi_test_utils.cpp
    TEST_INCLUDE_DIRS
        ${TEST_HEADER_DIRS}
	HOST_TESTS_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
)
