# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# MIMXRT1050_EVK
add_library(mbed-mimxrt1050-evk INTERFACE)

target_include_directories(mbed-mimxrt1050-evk
    INTERFACE
        .
)

target_sources(mbed-mimxrt1050-evk
    INTERFACE
        fsl_phy.c
		fsl_flexspi_nor_boot.c

	    TARGET_1050_EVK/flash_api.c

        TARGET_1050_EVK/xip/evkbimxrt1050_flexspi_nor_config.c
        TARGET_1050_EVK/xip/evkbimxrt1050_sdram_ini_dcd.c
)

target_link_libraries(mbed-mimxrt1050-evk
	INTERFACE
		mbed-mimxrt105x
)

# MIMXRT1060_EVK
add_library(mbed-mimxrt1060-evk INTERFACE)

target_include_directories(mbed-mimxrt1060-evk
    INTERFACE
        .
)

target_sources(mbed-mimxrt1060-evk
    INTERFACE
        fsl_phy.c
		fsl_flexspi_nor_boot.c

        TARGET_1060_EVK/xip/evkbmimxrt1060_flexspi_nor_config.c
        TARGET_1060_EVK/xip/dcd.c
)

target_link_libraries(mbed-mimxrt1060-evk
	INTERFACE
		mbed-mimxrt105x
)