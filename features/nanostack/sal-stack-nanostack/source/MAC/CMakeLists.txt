# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/IEEE802_15_4
        ${CMAKE_CURRENT_SOURCE_DIR}/virtual_rf
)

target_sources(mbed-os
    PRIVATE
        rf_driver_storage.c

        IEEE802_15_4/mac_cca_threshold.c
        IEEE802_15_4/mac_fhss_callbacks.c
        IEEE802_15_4/mac_filter.c
        IEEE802_15_4/mac_header_helper_functions.c
        IEEE802_15_4/mac_indirect_data.c
        IEEE802_15_4/mac_mcps_sap.c
        IEEE802_15_4/mac_mlme.c
        IEEE802_15_4/mac_pd_sap.c
        IEEE802_15_4/mac_security_mib.c
        IEEE802_15_4/mac_timer.c
        IEEE802_15_4/sw_mac.c

        ethernet/ethernet_mac_api.c

        serial/serial_mac_api.c

        virtual_rf/virtual_rf_client.c
        virtual_rf/virtual_rf_driver.c
)
