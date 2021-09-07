# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if("TARGET_CORTEX_A" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Cortex-A target not supported for this test")
endif()

macro(GetMemSize resultVar memRegion)
  # ARGN holds all arguments to function after last named one
  foreach(ITR ${ARGN})
    if(ITR MATCHES "${memRegion}*")
        string(FIND ${ITR} "=" index)
        MATH(EXPR index "${index}+1")
        string(SUBSTRING ${ITR} ${index} -1 value)
        set(${resultVar} ${value} PARENT_SCOPE)
        return()
    endif()
  endforeach()
endmacro()

GetMemSize(ram_size "MBED_CONF_TARGET_RAM_SIZE" ${MBED_CONFIG_DEFINITIONS})
GetMemSize(boot_stack_size "MBED_CONF_TARGET_BOOT_STACK_SIZE" ${MBED_CONFIG_DEFINITIONS})
GetMemSize(thread_stack_size "MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE" ${MBED_CONFIG_DEFINITIONS})
MATH(EXPR total_ram_boot_stack_size "${ram_size}-${boot_stack_size}" OUTPUT_FORMAT HEXADECIMAL)
MATH(EXPR total_boot_thread_stack_size "${boot_stack_size}+${thread_stack_size}" OUTPUT_FORMAT HEXADECIMAL)

if(${total_ram_boot_stack_size} LESS_EQUAL ${total_boot_thread_stack_size})
    set(TEST_SKIPPED "Insufficient stack for stack_size_unification tests")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-stack-size-unification
    TEST_INCLUDE_DIRS
        .
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
