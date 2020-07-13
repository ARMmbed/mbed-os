# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Neighbor_cache
        ${CMAKE_CURRENT_SOURCE_DIR}/Trickle
        ${CMAKE_CURRENT_SOURCE_DIR}/blacklist
        ${CMAKE_CURRENT_SOURCE_DIR}/etx
        ${CMAKE_CURRENT_SOURCE_DIR}/fhss
        ${CMAKE_CURRENT_SOURCE_DIR}/fnv_hash
        ${CMAKE_CURRENT_SOURCE_DIR}/hmac
        ${CMAKE_CURRENT_SOURCE_DIR}/ieee_802_11
        ${CMAKE_CURRENT_SOURCE_DIR}/load_balance
        ${CMAKE_CURRENT_SOURCE_DIR}/mac_neighbor_table
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/port
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/port/compiler
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/port/cpu
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/services
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/services/dns
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/services/mdns
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/services/poll
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/services/serial
        ${CMAKE_CURRENT_SOURCE_DIR}/mdns/fnet/fnet_stack/stack
        ${CMAKE_CURRENT_SOURCE_DIR}/mle_service
        ${CMAKE_CURRENT_SOURCE_DIR}/nd_proxy
        ${CMAKE_CURRENT_SOURCE_DIR}/nist_aes_kw
        ${CMAKE_CURRENT_SOURCE_DIR}/pan_blacklist
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/whiteboard
)

target_sources(mbed-os
    PRIVATE
        CCM_lib/ccm_security.c

        CCM_lib/mbedOS/aes_mbedtls_adapter.c

        Neighbor_cache/neighbor_cache.c

        SHA256_Lib/ns_sha256.c
        SHA256_Lib/shalib.c

        Trickle/trickle.c

        blacklist/blacklist.c

        etx/etx.c

        fhss/channel_functions.c
        fhss/channel_list.c
        fhss/fhss.c
        fhss/fhss_channel.c
        fhss/fhss_common.c
        fhss/fhss_configuration_interface.c
        fhss/fhss_statistics.c
        fhss/fhss_test_api.c
        fhss/fhss_ws.c
        fhss/fhss_ws_empty_functions.c

        fnv_hash/fnv_hash.c

        hmac/hmac_sha1.c

        ieee_802_11/ieee_802_11.c

        load_balance/load_balance.c

        mac_neighbor_table/mac_neighbor_table.c

        mdns/ns_fnet_events.c
        mdns/ns_fnet_port.c
        mdns/ns_mdns_api.c

        mdns/fnet/fnet_stack/services/mdns/fnet_mdns.c

        mdns/fnet/fnet_stack/services/poll/fnet_poll.c

        mdns/fnet/fnet_stack/stack/fnet_stdlib.c

        mle_service/mle_service.c
        mle_service/mle_service_buffer.c
        mle_service/mle_service_frame_counter_table.c
        mle_service/mle_service_interface.c
        mle_service/mle_service_security.c

        nd_proxy/nd_proxy.c

        nist_aes_kw/nist_aes_kw.c

        pan_blacklist/pan_blacklist.c

        utils/isqrt.c
        utils/ns_conf.c
        utils/ns_crc.c
        utils/ns_file_system.c

        whiteboard/whiteboard.c
)
