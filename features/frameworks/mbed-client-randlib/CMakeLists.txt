# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    source/randLIB.c
    mbed-client-randlib/randLIB.h
    mbed-client-randlib/platform/arm_hal_random.h
)
target_include_directories(mbed-os PUBLIC mbed-client-randlib)
target_include_directories(mbed-os PUBLIC mbed-client-randlib/platform)
