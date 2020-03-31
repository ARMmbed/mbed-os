# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    cmsis_armcc.h
    cmsis_armclang.h
    cmsis_compiler.h
    cmsis_cp15.h
    cmsis_gcc.h
    cmsis_iccarm.h
    core_ca.h
    irq_ctrl.h
    irq_ctrl_gic.c
)

if(${MBED_OS_TOOLCHAIN} STREQUAL "IAR")
target_sources(mbed-os PRIVATE TOOLCHAIN_IAR/cmain.S)
endif()

target_include_directories(mbed-os PUBLIC .)
