# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-lwipstack
    INTERFACE
        .
        ./include
        ./include/lwipstack
        ./lwip/src/include
        ./lwip/src/include/lwip
        ./lwip-sys
        ./lwip-sys/arch
)

target_sources(mbed-lwipstack
    INTERFACE
        lwip/src/api/lwip_api_lib.c
        lwip/src/api/lwip_api_msg.c
        lwip/src/api/lwip_err.c
        lwip/src/api/lwip_if_api.c
        lwip/src/api/lwip_netbuf.c
        lwip/src/api/lwip_netdb.c
        lwip/src/api/lwip_netifapi.c
        lwip/src/api/lwip_sockets.c
        lwip/src/api/lwip_tcpip.c

        lwip/src/core/ipv4/lwip_autoip.c
        lwip/src/core/ipv4/lwip_dhcp.c
        lwip/src/core/ipv4/lwip_etharp.c
        lwip/src/core/ipv4/lwip_icmp.c
        lwip/src/core/ipv4/lwip_igmp.c
        lwip/src/core/ipv4/lwip_ip4.c
        lwip/src/core/ipv4/lwip_ip4_addr.c
        lwip/src/core/ipv4/lwip_ip4_frag.c

        lwip/src/core/ipv6/lwip_dhcp6.c
        lwip/src/core/ipv6/lwip_ethip6.c
        lwip/src/core/ipv6/lwip_icmp6.c
        lwip/src/core/ipv6/lwip_inet6.c
        lwip/src/core/ipv6/lwip_ip6.c
        lwip/src/core/ipv6/lwip_ip6_addr.c
        lwip/src/core/ipv6/lwip_ip6_frag.c
        lwip/src/core/ipv6/lwip_mld6.c
        lwip/src/core/ipv6/lwip_nd6.c

        lwip/src/core/lwip_altcp.c
        lwip/src/core/lwip_altcp_alloc.c
        lwip/src/core/lwip_altcp_tcp.c
        lwip/src/core/lwip_def.c
        lwip/src/core/lwip_dns.c
        lwip/src/core/lwip_inet_chksum.c
        lwip/src/core/lwip_init.c
        lwip/src/core/lwip_ip.c
        lwip/src/core/lwip_mem.c
        lwip/src/core/lwip_memp.c
        lwip/src/core/lwip_netif.c
        lwip/src/core/lwip_pbuf.c
        lwip/src/core/lwip_raw.c
        lwip/src/core/lwip_stats.c
        lwip/src/core/lwip_sys.c
        lwip/src/core/lwip_tcp.c
        lwip/src/core/lwip_tcp_in.c
        lwip/src/core/lwip_tcp_out.c
        lwip/src/core/lwip_timeouts.c
        lwip/src/core/lwip_udp.c

        lwip/src/netif/lwip_bridgeif.c
        lwip/src/netif/lwip_bridgeif_fdb.c
        lwip/src/netif/lwip_ethernet.c
        lwip/src/netif/lwip_lowpan6.c
        lwip/src/netif/lwip_lowpan6_ble.c
        lwip/src/netif/lwip_lowpan6_common.c
        lwip/src/netif/lwip_zepif.c

        lwip-sys/arch/lwip_checksum.c
        lwip-sys/arch/lwip_memcpy.c
        lwip-sys/arch/lwip_sys_arch.c
        lwip-sys/lwip_random.c
        lwip-sys/lwip_tcp_isn.c

        source/LWIPInterface.cpp
        source/LWIPInterfaceEMAC.cpp
        source/LWIPInterfaceL3IP.cpp
        source/LWIPInterfacePPP.cpp
        source/LWIPMemoryManager.cpp
        source/LWIPStack.cpp
        source/lwip_tools.cpp
)

target_compile_definitions(mbed-lwipstack
    INTERFACE
        MBED_CONF_LWIPSTACK_PRESENT=1
)

target_link_libraries(mbed-lwipstack
    INTERFACE
        mbed-netsocket
        mbed-ppp
        mbed-randlib
)
