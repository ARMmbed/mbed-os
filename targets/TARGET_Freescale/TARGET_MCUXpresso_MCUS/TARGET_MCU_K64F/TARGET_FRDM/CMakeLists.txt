# SPDX-License-Identifier: Apache-2.0

target_sources(mbed_os PRIVATE crc.c)
target_sources(mbed_os PRIVATE crc.h)
target_sources(mbed_os PRIVATE device.h)
target_sources(mbed_os PRIVATE fsl_clock_config.c)
target_sources(mbed_os PRIVATE fsl_clock_config.h)
target_sources(mbed_os PRIVATE fsl_phy.c)
target_sources(mbed_os PRIVATE fsl_phy.h)
target_sources(mbed_os PRIVATE mbed_overrides.c)
target_sources(mbed_os PRIVATE PeripheralNames.h)
target_sources(mbed_os PRIVATE PeripheralPinMaps.h)
target_sources(mbed_os PRIVATE PeripheralPins.c)
target_sources(mbed_os PRIVATE PinNames.h)

target_include_directories(mbed_os PUBLIC .)
