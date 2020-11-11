# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./Bootstraps
        ./Fragmentation
        ./IPHC_Decode
        ./MAC
        ./Mesh
        ./ND
        ./NVM
        ./Thread
        ./ws
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        adaptation_interface.c

        Bootstraps/Generic/network_lib.c
        Bootstraps/Generic/protocol_6lowpan.c
        Bootstraps/Generic/protocol_6lowpan_bootstrap.c
        Bootstraps/Generic/protocol_6lowpan_interface.c

        Fragmentation/cipv6_fragmenter.c

        IPHC_Decode/6lowpan_iphc.c
        IPHC_Decode/iphc_compress.c
        IPHC_Decode/iphc_decompress.c
        IPHC_Decode/lowpan_context.c

        MAC/beacon_handler.c
        MAC/mac_data_poll.c
        MAC/mac_helper.c
        MAC/mac_ie_lib.c
        MAC/mac_pairwise_key.c
        MAC/mac_response_handler.c

        Mesh/mesh.c

        ND/nd_router_object.c

        NVM/nwk_nvm.c

        Thread/thread_bbr_api.c
        Thread/thread_bbr_commercial.c
        Thread/thread_beacon.c
        Thread/thread_bootstrap.c
        Thread/thread_border_router_api.c
        Thread/thread_ccm.c
        Thread/thread_commissioning_api.c
        Thread/thread_commissioning_if.c
        Thread/thread_common.c
        Thread/thread_dhcpv6_server.c
        Thread/thread_diagnostic.c
        Thread/thread_discovery.c
        Thread/thread_host_bootstrap.c
        Thread/thread_joiner_application.c
        Thread/thread_leader_service.c
        Thread/thread_lowpower_api.c
        Thread/thread_lowpower_private_api.c
        Thread/thread_management_api.c
        Thread/thread_management_client.c
        Thread/thread_management_if.c
        Thread/thread_management_server.c
        Thread/thread_mdns.c
        Thread/thread_meshcop_lib.c
        Thread/thread_mle_message_handler.c
        Thread/thread_nd.c
        Thread/thread_neighbor_class.c
        Thread/thread_net_config_api.c
        Thread/thread_network_data_lib.c
        Thread/thread_network_data_storage.c
        Thread/thread_network_synch.c
        Thread/thread_nvm_store.c
        Thread/thread_resolution_client.c
        Thread/thread_resolution_server.c
        Thread/thread_router_bootstrap.c
        Thread/thread_routing.c
        Thread/thread_test_api.c

        ws/ws_bbr_api.c
        ws/ws_bootstrap.c
        ws/ws_cfg_settings.c
        ws/ws_common.c
        ws/ws_eapol_auth_relay.c
        ws/ws_eapol_pdu.c
        ws/ws_eapol_relay.c
        ws/ws_eapol_relay_lib.c
        ws/ws_empty_functions.c
        ws/ws_ie_lib.c
        ws/ws_llc_data_service.c
        ws/ws_management_api.c
        ws/ws_mpx_header.c
        ws/ws_neighbor_class.c
        ws/ws_pae_auth.c
        ws/ws_pae_controller.c
        ws/ws_pae_key_storage.c
        ws/ws_pae_lib.c
        ws/ws_pae_nvm_data.c
        ws/ws_pae_nvm_store.c
        ws/ws_pae_supp.c
        ws/ws_pae_time.c
        ws/ws_pae_timers.c
        ws/ws_stats.c
        ws/ws_test_api.c
)
