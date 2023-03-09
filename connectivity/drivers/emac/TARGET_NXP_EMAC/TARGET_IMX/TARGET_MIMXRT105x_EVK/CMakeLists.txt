# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-emac
    PRIVATE
        hardware_init.c
		fsl_phy_ksz8081rnb.c
)
