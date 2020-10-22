# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-ppp
    INTERFACE
        .
        ./include
        ./include/polarssl
        ./include/ppp
)

target_sources(mbed-ppp
    INTERFACE
        source/auth.c
        source/ccp.c
        source/chap-md5.c
        source/chap-new.c
        source/chap_ms.c
        source/demand.c
        source/eap.c
        source/eui64.c
        source/fsm.c
        source/ipcp.c
        source/ipv6cp.c
        source/lcp.c
        source/magic.c
        source/mppe.c
        source/multilink.c
        source/ppp.c
        source/ppp_ecp.c
        source/ppp_service.cpp
        source/ppp_service_if.cpp
        source/pppapi.c
        source/pppcrypt.c
        source/pppoe.c
        source/pppol2tp.c
        source/pppos.cpp
        source/upap.c
        source/utils.c
        source/vj.c

        source/polarssl/ppp_arc4.c
        source/polarssl/ppp_des.c
        source/polarssl/ppp_md4.c
        source/polarssl/ppp_md5.c
        source/polarssl/ppp_sha1.c
)

target_compile_definitions(mbed-ppp
    INTERFACE
        MBED_CONF_PPP_PRESENT=1
)
