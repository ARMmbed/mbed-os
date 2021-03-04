# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(COMPONENT_STMOD_CELLULAR EXCLUDE_FROM_ALL)

add_subdirectory(Altair)
add_subdirectory(GEMALTO)
add_subdirectory(GENERIC)
add_subdirectory(MultiTech)
add_subdirectory(QUECTEL)
add_subdirectory(RiotMicro)
add_subdirectory(TELIT)
add_subdirectory(UBLOX)

# All drivers here are part of mbed-cellular library by default
# Backward compatiblity with mbed-cli 1
target_link_libraries(mbed-cellular 
    mbed-cellular-alt1250-ppp
    mbed-cellular-gemalt-cinterion
    mbed-cellular-generic-at3gpp
    mbed-cellular-multitech-dragonfly_nano
    mbed-cellular-quectel-bc95
    mbed-cellular-quectel-bg96
    mbed-cellular-quectel-ec2x
    mbed-cellular-quectel-m26
    mbed-cellular-quectel-ug96
    mbed-cellular-riotmicro-at
    mbed-cellular-telit-he910
    mbed-cellular-telit-me310
    mbed-cellular-telit-me910
    mbed-cellular-ublox-ppp
    mbed-cellular-ublox-n2xx
    mbed-cellular-ublox-at
)
