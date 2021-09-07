# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./Neighbor_cache
        ./Trickle
        ./blacklist
        ./etx
        ./fhss
        ./fnv_hash
        ./hmac
        ./ieee_802_11
        ./load_balance
        ./mac_neighbor_table
        ./mdns
        ./mdns/fnet
        ./mdns/fnet/fnet_stack
        ./mdns/fnet/fnet_stack/port
        ./mdns/fnet/fnet_stack/port/compiler
        ./mdns/fnet/fnet_stack/port/cpu
        ./mdns/fnet/fnet_stack/services
        ./mdns/fnet/fnet_stack/services/dns
        ./mdns/fnet/fnet_stack/services/mdns
        ./mdns/fnet/fnet_stack/services/poll
        ./mdns/fnet/fnet_stack/services/serial
        ./mdns/fnet/fnet_stack/stack
        ./mle_service
        ./nd_proxy
        ./nist_aes_kw
        ./pan_blacklist
        ./random_early_detection
        ./utils
        ./whiteboard
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
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

        hmac/hmac_md.c

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
        
        random_early_detection/random_early_detection.c

        utils/isqrt.c
        utils/ns_conf.c
        utils/ns_crc.c
        utils/ns_file_system.c
        utils/ns_time.c

        whiteboard/whiteboard.c
)
