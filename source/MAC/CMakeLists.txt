# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./IEEE802_15_4
        ./virtual_rf
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        rf_driver_storage.c

        IEEE802_15_4/mac_cca_threshold.c
        IEEE802_15_4/mac_fhss_callbacks.c
        IEEE802_15_4/mac_filter.c
        IEEE802_15_4/mac_header_helper_functions.c
        IEEE802_15_4/mac_indirect_data.c
        IEEE802_15_4/mac_mcps_sap.c
        IEEE802_15_4/mac_mlme.c
        IEEE802_15_4/mac_mode_switch.c
        IEEE802_15_4/mac_pd_sap.c
        IEEE802_15_4/mac_security_mib.c
        IEEE802_15_4/mac_timer.c
        IEEE802_15_4/sw_mac.c

        ethernet/ethernet_mac_api.c

        serial/serial_mac_api.c

        virtual_rf/virtual_rf_client.c
        virtual_rf/virtual_rf_driver.c
)
