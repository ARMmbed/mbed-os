# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    cmsis_compiler.h
    cmsis_version.h
    core_armv81mml.h
    core_armv8mbl.h
    core_armv8mml.h
    core_cm0plus.h
    core_cm1.h
    core_cm23.h
    core_cm3.h
    core_cm33.h
    core_cm35p.h
    core_cm4.h
    core_cm7.h
    core_sc000.h
    core_sc300.h
    mbed_tz_context.c
    mpu_armv7.h
    mpu_armv8.h
    tz_context.h
)

target_include_directories(mbed-os PUBLIC .)
