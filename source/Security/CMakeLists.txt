# Copyright (c) 2020-2021, Pelion and affiliates.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack
    INTERFACE
        .
        ./Common
        ./PANA
        ./TLS
        ./eapol
        ./kmp
        ./protocols
        ./protocols/eap_tls_sec_prot
        ./protocols/fwh_sec_prot
        ./protocols/gkh_sec_prot
        ./protocols/key_sec_prot
        ./protocols/radius_sec_prot
        ./protocols/tls_sec_prot
)

target_sources(mbed-nanostack-sal_stack
    INTERFACE
        Common/security_lib.c

        PANA/eap_protocol.c
        PANA/pana.c
        PANA/pana_avp.c
        PANA/pana_client.c
        PANA/pana_eap_header.c
        PANA/pana_header.c
        PANA/pana_relay_table.c
        PANA/pana_server.c

        TLS/tls_ccm_crypt.c
        TLS/tls_lib.c

        eapol/eapol_helper.c
        eapol/kde_helper.c

        kmp/kmp_addr.c
        kmp/kmp_api.c
        kmp/kmp_eapol_pdu_if.c
        kmp/kmp_socket_if.c

        protocols/sec_prot_certs.c
        protocols/sec_prot_keys.c
        protocols/sec_prot_lib.c

        protocols/eap_tls_sec_prot/auth_eap_tls_sec_prot.c
        protocols/eap_tls_sec_prot/eap_tls_sec_prot_lib.c
        protocols/eap_tls_sec_prot/radius_eap_tls_sec_prot.c
        protocols/eap_tls_sec_prot/supp_eap_tls_sec_prot.c

        protocols/fwh_sec_prot/auth_fwh_sec_prot.c
        protocols/fwh_sec_prot/supp_fwh_sec_prot.c

        protocols/gkh_sec_prot/auth_gkh_sec_prot.c
        protocols/gkh_sec_prot/supp_gkh_sec_prot.c

        protocols/key_sec_prot/key_sec_prot.c
        
        protocols/msg_sec_prot/msg_sec_prot.c

        protocols/radius_sec_prot/avp_helper.c
        protocols/radius_sec_prot/radius_client_sec_prot.c

        protocols/tls_sec_prot/tls_sec_prot.c
        protocols/tls_sec_prot/tls_sec_prot_lib.c
)
