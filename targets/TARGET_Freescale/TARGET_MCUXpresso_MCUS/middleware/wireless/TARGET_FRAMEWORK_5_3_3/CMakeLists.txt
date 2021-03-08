# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-framework-5-3-3 INTERFACE)

target_include_directories(mbed-framework-5-3-3
    INTERFACE
        Common
        OSAbstraction/Interface
        XCVR/MKW41Z4
)

target_sources(mbed-framework-5-3-3
    INTERFACE
        OSAbstraction/Source/fsl_os_abstraction_mbed.c

        XCVR/MKW41Z4/dbg_ram_capture.c
        XCVR/MKW41Z4/fsl_xcvr.c
        XCVR/MKW41Z4/fsl_xcvr_trim.c
        XCVR/MKW41Z4/ifr_radio.c

        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_ant_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_ble_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_common_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p3_h_0p5_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_0p32_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_0p5_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_0p7_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p5_h_1p0_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_gfsk_bt_0p7_h_0p5_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_mode_datarate_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_msk_config.c
        XCVR/MKW41Z4/cfgs_kw4x_3x_2x/fsl_xcvr_zgbe_config.c
)
