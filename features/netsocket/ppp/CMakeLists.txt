# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        ppp_nsapi.cpp
        ppp_service.cpp
        ppp_service_if.cpp
        polarssl/ppp_arc4.c
        polarssl/ppp_des.c
        polarssl/ppp_md4.c
        polarssl/ppp_md5.c
        polarssl/ppp_sha1.c
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
        source/pppapi.c
        source/pppcrypt.c
        source/pppoe.c
        source/pppol2tp.c
        source/pppos.cpp
        source/upap.c
        source/utils.c
        source/vj.c
)


target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        include
        include/polarssl
)
