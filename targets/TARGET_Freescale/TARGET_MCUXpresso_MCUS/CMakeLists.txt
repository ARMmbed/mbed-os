# SPDX-License-Identifier: Apache-2.0

include(${MBED_OS_BASE}/util.cmake)

target_sources(mbed_os PRIVATE fsl_common.c)

target_sources(mbed_os PRIVATE api/analogin_api.c)
target_sources(mbed_os PRIVATE api/analogout_api.c)
target_sources(mbed_os PRIVATE api/dma_api.c)
target_sources(mbed_os PRIVATE api/dma_api_hal.h)
target_sources(mbed_os PRIVATE api/flash_api.c)
target_sources(mbed_os PRIVATE api/gpio_api.c)
target_sources(mbed_os PRIVATE api/gpio_irq_api.c)
target_sources(mbed_os PRIVATE api/gpio_object.h)
target_sources(mbed_os PRIVATE api/i2c_api.c)
target_sources(mbed_os PRIVATE api/lp_ticker.c)
target_sources(mbed_os PRIVATE api/objects.h)
target_sources(mbed_os PRIVATE api/PeripheralPins.h)
target_sources(mbed_os PRIVATE api/pinmap.c)
target_sources(mbed_os PRIVATE api/PortNames.h)
target_sources(mbed_os PRIVATE api/port_api.c)
target_sources(mbed_os PRIVATE api/qspi_api.c)
target_sources(mbed_os PRIVATE api/rtc_api.c)
target_sources(mbed_os PRIVATE api/sleep.c)

mbed_os_add_cmake_directory("TARGET"
"${MBED_OS_TARGET_KEYS}")

target_include_directories(mbed_os PUBLIC api)
