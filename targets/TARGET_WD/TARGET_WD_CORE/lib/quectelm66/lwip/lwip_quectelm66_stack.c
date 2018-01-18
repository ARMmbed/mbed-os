/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Additional contributors:
 * - Simon Pfeifhofer (simon.pfeifhofer@gmail.com)
 */

#include "nsapi.h"
#include "mbed_interface.h"
#include "wd_logging.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "eth_arch.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/mld6.h"
#include "lwip/dns.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "netif/ppp/pppapi.h"
#include "netif/ppp/pppos.h"	
#include "emac_api.h"
#include "pppos_utils.h"

#ifndef PPPOS_WRITE_TIMEOUT
#define PPPOS_WRITE_TIMEOUT 1000
#endif

/* Static arena of sockets */
static struct lwip_quectelm66_socket {
	bool in_use;

	struct netconn *conn;
	struct netbuf *buf;
	u16_t offset;

	void(*cb)(void *);
	void *data;
} lwip_quectelm66_arena[MEMP_NUM_NETCONN];

static bool lwip_quectelm66_connected = false;

static void mbed_lwip_quectelm66_arena_init(void)
{
	memset(lwip_quectelm66_arena, 0, sizeof lwip_quectelm66_arena);
}

static struct lwip_quectelm66_socket *mbed_lwip_quectelm66_arena_alloc(void)
{
	sys_prot_t prot = sys_arch_protect();

	for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
		if (!lwip_quectelm66_arena[i].in_use) {
			struct lwip_quectelm66_socket *s = &lwip_quectelm66_arena[i];
			memset(s, 0, sizeof *s);
			s->in_use = true;
			sys_arch_unprotect(prot);
			return s;
		}
	}

	sys_arch_unprotect(prot);
	return 0;
}

static void mbed_lwip_quectelm66_arena_dealloc(struct lwip_quectelm66_socket *s)
{
	s->in_use = false;
}

static void mbed_lwip_quectelm66_socket_callback(struct netconn *nc, enum netconn_evt eh, u16_t len)
{
    // Filter send minus events
	if (eh == NETCONN_EVT_SENDMINUS && nc->state == NETCONN_WRITE) {
		return;
	}

	sys_prot_t prot = sys_arch_protect();

	for (int i = 0; i < MEMP_NUM_NETCONN; i++) {
		if (lwip_quectelm66_arena[i].in_use
		    && lwip_quectelm66_arena[i].conn == nc
		    && lwip_quectelm66_arena[i].cb) {
			    lwip_quectelm66_arena[i].cb(lwip_quectelm66_arena[i].data);
		}
	}

	sys_arch_unprotect(prot);
}


/* TCP/IP and Network Interface Initialisation */
static struct netif lwip_quectelm66_netif;
static ppp_pcb *pcb;
static bool lwip_quectelm66_dhcp = false;
static char lwip_quectelm66_mac_address[NSAPI_MAC_SIZE];

#if !LWIP_IPV4 || !LWIP_IPV6
static bool lwip_quectelm66_all_zeros(const uint8_t *p, int len)
{
	for (int i = 0; i < len; i++) {
		if (p[i]) {
			return false;
		}
	}

	return true;
}
#endif

static bool convert_mbed_addr_to_lwip_quectelm66(ip_addr_t *out, const nsapi_addr_t *in)
{
#if LWIP_IPV6
	if (in->version == NSAPI_IPv6) {
		IP_SET_TYPE(out, IPADDR_TYPE_V6);
		MEMCPY(ip_2_ip6(out), in->bytes, sizeof(ip6_addr_t));
		return true;
	}
#if !LWIP_IPV4
	    /* For bind() and other purposes, need to accept "null" of other type */
	    /* (People use IPv4 0.0.0.0 as a general null) */
	if (in->version == NSAPI_UNSPEC ||
	    (in->version == NSAPI_IPv4 && all_zeros(in->bytes, 4))) {
		ip_addr_set_zero_ip6(out);
		return true;
	}
#endif
#endif

#if LWIP_IPV4
	if (in->version == NSAPI_IPv4) {
		IP_SET_TYPE(out, IPADDR_TYPE_V4);
		MEMCPY(ip_2_ip4(out), in->bytes, sizeof(ip4_addr_t));
		return true;
	}
#if !LWIP_IPV6
	    /* For symmetry with above, accept IPv6 :: as a general null */
	if (in->version == NSAPI_UNSPEC ||
	    (in->version == NSAPI_IPv6 && lwip_quectelm66_all_zeros(in->bytes, 16))) {
		ip_addr_set_zero_ip4(out);
		return true;
	}
#endif
#endif

#if LWIP_IPV4 && LWIP_IPV6
	if (in->version == NSAPI_UNSPEC) {
#if IP_VERSION_PREF == PREF_IPV4
		ip_addr_set_zero_ip4(out);
#else
		ip_addr_set_zero_ip6(out);
#endif
		return true;
	}
#endif

	return false;
}

static bool convert_lwip_quectelm66_addr_to_mbed(nsapi_addr_t *out, const ip_addr_t *in)
{
#if LWIP_IPV6
	if (IP_IS_V6(in)) {
		out->version = NSAPI_IPv6;
		MEMCPY(out->bytes, ip_2_ip6(in), sizeof(ip6_addr_t));
		return true;
	}
#endif
#if LWIP_IPV4
	if (IP_IS_V4(in)) {
		out->version = NSAPI_IPv4;
		MEMCPY(out->bytes, ip_2_ip4(in), sizeof(ip4_addr_t));
		return true;
	}
#endif
	return false;
}

static const ip_addr_t *mbed_lwip_quectelm66_get_ipv4_addr(const struct netif *netif)
{
#if LWIP_IPV4
	if (!netif_is_up(netif)) {
		return NULL;
	}

	if (!ip4_addr_isany(netif_ip4_addr(netif))) {
		return netif_ip_addr4(netif);
	}
#endif

	return NULL;
}

static const ip_addr_t *mbed_lwip_quectelm66_get_ipv6_addr(const struct netif *netif)
{
#if LWIP_IPV6
	if (!netif_is_up(netif)) {
		return NULL;
	}

	for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
		if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)) &&
		        !ip6_addr_islinklocal(netif_ip6_addr(netif, i))) {
			return netif_ip_addr6(netif, i);
		}
	}
#endif

	return NULL;

}

const ip_addr_t *mbed_lwip_quectelm66_get_ip_addr(bool any_addr, const struct netif *netif)
{
	const ip_addr_t *pref_ip_addr = 0;
	const ip_addr_t *npref_ip_addr = 0;

#if IP_VERSION_PREF == PREF_IPV4
	pref_ip_addr = mbed_lwip_quectelm66_get_ipv4_addr(netif);
	npref_ip_addr = mbed_lwip_quectelm66_get_ipv6_addr(netif);
#else
	pref_ip_addr = mbed_lwip_get_ipv6_addr(netif);
	npref_ip_addr = mbed_lwip_get_ipv4_addr(netif);
#endif

	if (pref_ip_addr) {
		return pref_ip_addr;
	}
	else if (npref_ip_addr && any_addr) {
		return npref_ip_addr;
	}

	return NULL;
}

void lwip_quectelm66_add_dns_addr(struct netif *lwip_netif)
{
    // Do nothing if not brought up
	const ip_addr_t *ip_addr = mbed_lwip_quectelm66_get_ip_addr(true, lwip_netif);
	if (!ip_addr) {
		return;
	}

	    // Check for existing dns server
	for (char numdns = 0; numdns < DNS_MAX_SERVERS; numdns++) {
		const ip_addr_t *dns_ip_addr = lwip_quectelm66_dns_getserver(numdns);
		if (!ip_addr_isany(dns_ip_addr)) {
			return;
		}
	}

#if LWIP_IPV6
	if (IP_IS_V6(ip_addr)) {
	    /* 2001:4860:4860::8888 google */
		ip_addr_t ipv6_dns_addr = IPADDR6_INIT(
		        PP_HTONL(0x20014860UL),
		        PP_HTONL(0x48600000UL),
		        PP_HTONL(0x00000000UL),
		        PP_HTONL(0x00008888UL));
		dns_setserver(0, &ipv6_dns_addr);
	}
#endif

#if LWIP_IPV4
	if (IP_IS_V4(ip_addr)) {
	    /* 8.8.8.8 google */
		ip_addr_t ipv4_dns_addr = IPADDR4_INIT(0x08080808);
		dns_setserver(0, &ipv4_dns_addr);
	}
#endif
}

static sys_sem_t lwip_quectelm66_tcpip_inited;
static void mbed_lwip_quectelm66_tcpip_init_irq(void *eh)
{
	sys_sem_signal(&lwip_quectelm66_tcpip_inited);
}

static sys_sem_t lwip_quectelm66_netif_linked;
static void mbed_lwip_quectelm66_netif_link_irq(struct netif *lwip_netif)
{
	if (netif_is_link_up(lwip_netif)) {
		sys_sem_signal(&lwip_quectelm66_netif_linked);
	}
}

static sys_sem_t lwip_quectelm66_netif_has_addr;
static void mbed_lwip_quectelm66_netif_status_irq(struct netif *lwip_quectelm66_netif)
{
	static bool any_addr = true;

	if (netif_is_up(lwip_quectelm66_netif)) {
	    // Indicates that has address
		if (any_addr == true && mbed_lwip_quectelm66_get_ip_addr(true, lwip_quectelm66_netif)) {
			sys_sem_signal(&lwip_quectelm66_netif_has_addr);
			any_addr = false;
			return;
		}

		        // Indicates that has preferred address
		if (mbed_lwip_quectelm66_get_ip_addr(false, lwip_quectelm66_netif)) {
			sys_sem_signal(&lwip_quectelm66_netif_has_addr);
		}
	}
	else {
		any_addr = true;
	}
}

/* LWIP interface implementation */

char *mbed_lwip_quectelm66_get_ip_address(char *buf, nsapi_size_t buflen)
{
	const ip_addr_t *addr = mbed_lwip_quectelm66_get_ip_addr(true, &lwip_quectelm66_netif);
	if (!addr) {
		return NULL;
	}
#if LWIP_IPV6
	if (IP_IS_V6(addr)) {
		return ip6addr_ntoa_r(ip_2_ip6(addr), buf, buflen);
	}
#endif
#if LWIP_IPV4
	if (IP_IS_V4(addr)) {
		return ip4addr_ntoa_r(ip_2_ip4(addr), buf, buflen);
	}
#endif
	return NULL;
}

const char *mbed_lwip_quectelm66_get_netmask(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
	const ip4_addr_t *addr = netif_ip4_netmask(&lwip_quectelm66_netif);
	if (!ip4_addr_isany(addr)) {
		return ip4addr_ntoa_r(addr, buf, buflen);
	}
	else {
		return NULL;
	}
#else
	return NULL;
#endif
}

char *mbed_lwip_quectelm66_get_gateway(char *buf, nsapi_size_t buflen)
{
#if LWIP_IPV4
	const ip4_addr_t *addr = netif_ip4_gw(&lwip_quectelm66_netif);
	if (!ip4_addr_isany(addr)) {
		return ip4addr_ntoa_r(addr, buf, buflen);
	}
	else {
		return NULL;
	}
#else
	return NULL;
#endif
}

ppp_pcb *mbed_get_ppp_pcb() {
	return pcb;
}

static u32_t output_cb(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx) {
	wd_log_debug("lwip_quectelm66_stack --> output_cb");
	
	pppos_context_t* ctx_cb = (pppos_context_t*)ctx;
	if (ctx_cb->pppos_write_callback(ctx_cb, data, len, PPPOS_WRITE_TIMEOUT) == 0) {
		return len;
	}
	return -1;
}

static void status_cb(ppp_pcb *pcb, int err_code, void *ctx) {
	
	struct netif *pppif = ppp_netif(pcb);
	LWIP_UNUSED_ARG(ctx);

	switch (err_code) {
	case PPPERR_NONE: {
#if LWIP_DNS
			const ip_addr_t *ns;
#endif /* LWIP_DNS */
		wd_log_debug("lwip_quectelm66_stack --> status_cb: Connected\n");
#if PPP_IPV4_SUPPORT
		wd_log_debug("lwip_quectelm66_stack -->    our_ipaddr  = %s\n", ipaddr_ntoa(&pppif->ip_addr));
		wd_log_debug("lwip_quectelm66_stack -->    his_ipaddr  = %s\n", ipaddr_ntoa(&pppif->gw));
		wd_log_debug("lwip_quectelm66_stack -->    netmask     = %s\n", ipaddr_ntoa(&pppif->netmask));
#if LWIP_DNS
			ns = dns_getserver(0);
		wd_log_debug("lwip_quectelm66_stack -->    dns1        = %s\n", ipaddr_ntoa(ns));
			ns = dns_getserver(1);
		wd_log_debug("lwip_quectelm66_stack -->    dns2        = %s\n", ipaddr_ntoa(ns));
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
		wd_log_debug("lwip_quectelm66_stack -->    our6_ipaddr = %s\n", ip6addr_ntoa(netif_ip6_addr(pppif, 0)));
#endif /* PPP_IPV6_SUPPORT */
			sys_sem_signal(&lwip_quectelm66_netif_linked);
			sys_sem_signal(&lwip_quectelm66_netif_has_addr);
			break;
		}
	case PPPERR_PARAM: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Invalid parameter\n");
			break;
		}
	case PPPERR_OPEN: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Unable to open PPP session\n");
			break;
		}
	case PPPERR_DEVICE: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Invalid I/O device for PPP\n");
			break;
		}
	case PPPERR_ALLOC: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Unable to allocate resources\n");
			break;
		}
	case PPPERR_USER: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: User interrupt\n");
			break;
		}
	case PPPERR_CONNECT: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Connection lost\n");
			break;
		}
	case PPPERR_AUTHFAIL: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Failed authentication challenge\n");
			break;
		}
	case PPPERR_PROTOCOL: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Failed to meet protocol\n");
			break;
		}
	case PPPERR_PEERDEAD: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Connection timeout\n");
			break;
		}
	case PPPERR_IDLETIMEOUT: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Idle Timeout\n");
			break;
		}
	case PPPERR_CONNECTTIME: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Max connect time reached\n");
			break;
		}
	case PPPERR_LOOPBACK: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Loopback detected\n");
			break;
		}
	default: {
		wd_log_error("lwip_quectelm66_stack --> status_cb: Unknown error code %d\n", err_code);
			break;
		}
	}

	/*
	 * This should be in the switch case, this is put outside of the switch
	 * case for example readability.
	 */

	if (err_code == PPPERR_NONE) {
		return;
	}

	  /* ppp_close() was previously called, don't reconnect */
	if (err_code == PPPERR_USER) {
		
		/*
		* Free the PPP control block, can only be called if PPP session is in thereturn;
		* dead state (i.e. disconnected). You need to call ppp_close() before.
		*/
		wd_log_info("lwip_quectelm66_stack --> Free the PPP control block");
		pppapi_free(pcb);
		wd_log_error("lwip_quectelm66_stack --> Free the PPP control block done");
		return;
		
	}
	
	wd_log_warn("lwip_quectelm66_stack --> Reconnect");
	u16_t holdoff = 0;
	if (0 != pppapi_connect(pcb, holdoff)) {
		wd_log_error("lwip_quectelm66_stack --> Reconnect failed");
		return;
	}
	wd_log_debug("lwip_quectelm66_stack --> Reconnect success");
	
}

static void ppp_notify_phase_cb(ppp_pcb *pcb, u8_t phase, void *ctx) {
	switch (phase) {

	  /* Session is down (either permanently or briefly) */
	case PPP_PHASE_DEAD:
		wd_log_info("lwip_quectelm66_stack --> PPP_PHASE_DEAD");
		break;

		  /* We are between two sessions */
	case PPP_PHASE_HOLDOFF:
		wd_log_info("lwip_quectelm66_stack --> PPP_PHASE_HOLDOFF");
		break;

		  /* Session just started */
	case PPP_PHASE_INITIALIZE:
		wd_log_info("lwip_quectelm66_stack --> PPP_PHASE_INITIALIZE");
		break;

		  /* Session is running */
	case PPP_PHASE_RUNNING:
		wd_log_info("lwip_quectelm66_stack --> PPP_PHASE_RUNNING");
		break;

	default:
		break;
	}
}

nsapi_error_t mbed_lwip_quectelm66_init(emac_interface_t *emac, const char* username, const char* password, pppos_context_t* ctx_cb)
{

	sys_sem_new(&lwip_quectelm66_netif_linked, 0);
	sys_sem_new(&lwip_quectelm66_netif_has_addr, 0);

	memset(&lwip_quectelm66_netif, 0, sizeof lwip_quectelm66_netif);
	pcb = 
		pppapi_pppos_create(
			&lwip_quectelm66_netif,
			output_cb, 
			status_cb, 
			ctx_cb
		);
		
	wd_log_debug("lwip_quectelm66_stack --> Register notify phase callback");
	int pppapi_set_notify_phase_callback_ret = pppapi_set_notify_phase_callback(pcb, ppp_notify_phase_cb);
	if (pppapi_set_notify_phase_callback_ret != 0) {
		wd_log_error("PPPIPInterface --> Register notify phase callback failed");
		return pppapi_set_notify_phase_callback_ret;
	}
	wd_log_debug("lwip_quectelm66_stack --> Register notify phase callback success");

	/* Set this interface as default route */
	wd_log_debug("lwip_quectelm66_stack --> Set this interface as default route");
	pppapi_set_default(pcb);
	wd_log_debug("lwip_quectelm66_stack --> Set this interface as default route success");
	
	/* Ask the peer for up to 2 DNS server addresses. */
	//wd_log_debug("lwip_quectelm66_stack --> Ask the peer for up to 2 DNS server addresses.");
	ppp_set_usepeerdns(pcb, 1);
	wd_log_debug("lwip_quectelm66_stack --> Done");
		
	/* Auth configuration, this is pretty self-explanatory */
	wd_log_debug("lwip_quectelm66_stack --> Configuring PPP authentication method");
	//ppp_set_auth(pcb, PPPAUTHTYPE_ANY, "M2M4WD", "WORLD6073Direct");
	ppp_set_auth(pcb, PPPAUTHTYPE_ANY, username, password);
	wd_log_debug("lwip_quectelm66_stack --> Configuring PPP authentication method success");	
		
	wd_log_debug("lwip_quectelm66_stack --> Initiate PPP negotiation");
	u16_t holdoff = 0;
	int pppapi_connect_ret = pppapi_connect(pcb, holdoff);
	if (pppapi_connect_ret != 0) {
		wd_log_error("lwip_quectelm66_stack --> Initiate PPP negotiation error");
		return pppapi_connect_ret;
	}
	wd_log_debug("lwip_quectelm66_stack --> Initiate PPP negotiation success");

#if !DEVICE_EMAC
		eth_arch_enable_interrupts();
#endif

	return NSAPI_ERROR_OK;
}

nsapi_error_t mbed_lwip_quectelm66_bringup(const char *ip, const char *netmask, const char *gw, const char* username, const char* password, pppos_context_t* ctx_cb)
{
    // Check if we've already connected
	if (lwip_quectelm66_connected) {
		return NSAPI_ERROR_PARAMETER;
	}

	if (mbed_lwip_quectelm66_init(NULL, username, password, ctx_cb) != NSAPI_ERROR_OK) {
		return NSAPI_ERROR_DEVICE_ERROR;
	}

	    // Zero out socket set
	mbed_lwip_quectelm66_arena_init();

#if LWIP_IPV6
	netif_create_ip6_linklocal_address(&lwip_quectelm66_netif, 1/*from MAC*/);
#if LWIP_IPV6_MLD
	  /*
	   * For hardware/netifs that implement MAC filtering.
	   * All-nodes link-local is handled by default, so we must let the hardware know
	   * to allow multicast packets in.
	   * Should set mld_mac_filter previously. */
	if (lwip_quectelm66_netif.mld_mac_filter != NULL) {
		ip6_addr_t ip6_allnodes_ll;
		ip6_addr_set_allnodes_linklocal(&ip6_allnodes_ll);
		lwip_quectelm66_netif.mld_mac_filter(&lwip_quectelm66_netif, &ip6_allnodes_ll, MLD6_ADD_MAC_FILTER);
	}
#endif /* LWIP_IPV6_MLD */

#if LWIP_IPV6_AUTOCONFIG
	    /* IPv6 address autoconfiguration not enabled by default */
	lwip_quectelm66_netif.ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */

#endif

	u32_t ret;

	if (!netif_is_link_up(&lwip_quectelm66_netif)) {
		ret = sys_arch_sem_wait(&lwip_quectelm66_netif_linked, 15000);

		if (ret == SYS_ARCH_TIMEOUT) {
			return NSAPI_ERROR_NO_CONNECTION;
		}
	}

	netif_set_up(&lwip_quectelm66_netif);
	
	// If doesn't have address
	if (!mbed_lwip_quectelm66_get_ip_addr(true, &lwip_quectelm66_netif)) {
		ret = sys_arch_sem_wait(&lwip_quectelm66_netif_has_addr, 15000);
		if (ret == SYS_ARCH_TIMEOUT) {
			return NSAPI_ERROR_DHCP_FAILURE;
		}
	}

#if ADDR_TIMEOUT
	    // If address is not for preferred stack waits a while to see
	    // if preferred stack address is acquired
	if (!mbed_lwip_get_ip_addr(false, &lwip_quectelm66_netif)) {
		ret = sys_arch_sem_wait(&lwip_quectelm66_netif_has_addr, ADDR_TIMEOUT * 1000);
	}
#endif

	lwip_quectelm66_connected = true;
	return 0;
}

#if LWIP_IPV6
void mbed_lwip_quectelm66_clear_ipv6_addresses(struct netif *lwip_quectelm66_netif)
{
	for (u8_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++) {
		netif_ip6_addr_set_state(lwip_quectelm66_netif, i, IP6_ADDR_INVALID);
	}
}
#endif

nsapi_error_t mbed_lwip_quectelm66_bringdown(void)
{
    // Check if we've connected
	if (!lwip_quectelm66_connected) {
		return NSAPI_ERROR_PARAMETER;
	}
	
	wd_log_debug("lwip_quectelm66_stack --> Initiate the end of the PPP session");
	int ret;
	u8_t nocarrier = 0;
	ret = pppapi_close(pcb, nocarrier);
	if (ret != 0) {
		wd_log_debug("lwip_quectelm66_stack --> Failed");
		return ret;
	}
	wd_log_debug("lwip_quectelm66_stack --> Done");

	netif_set_down(&lwip_quectelm66_netif);

#if LWIP_IPV6
	mbed_lwip_quectelm66_clear_ipv6_addresses(&lwip_quectelm66_netif);
#endif

	sys_sem_free(&lwip_quectelm66_tcpip_inited);
	sys_sem_free(&lwip_quectelm66_netif_linked);
	sys_sem_free(&lwip_quectelm66_netif_has_addr);
	lwip_quectelm66_connected = false;
	return 0;
}

/* LWIP error remapping */
static nsapi_error_t mbed_lwip_quectelm66_err_remap(err_t err) {
	switch (err) {
	case ERR_OK:
	case ERR_CLSD:
		return 0;
	case ERR_MEM:
		return NSAPI_ERROR_NO_MEMORY;
	case ERR_CONN:
	case ERR_RST:
	case ERR_ABRT:
		return NSAPI_ERROR_NO_CONNECTION;
	case ERR_TIMEOUT:
	case ERR_RTE:
	case ERR_INPROGRESS:
	case ERR_WOULDBLOCK:
		return NSAPI_ERROR_WOULD_BLOCK;
	case ERR_VAL:
	case ERR_USE:
	case ERR_ISCONN:
	case ERR_ARG:
		return NSAPI_ERROR_PARAMETER;
	default:
		return NSAPI_ERROR_DEVICE_ERROR;
	}
}

/* LWIP network stack implementation */
static nsapi_error_t mbed_lwip_quectelm66_gethostbyname(nsapi_stack_t *stack, const char *host, nsapi_addr_t *addr, nsapi_version_t version)
{
	
ip_addr_t lwip_addr;

#if LWIP_IPV4 && LWIP_IPV6
    u8_t addr_type;
    if (version == NSAPI_UNSPEC) {
        const ip_addr_t *ip_addr;
        ip_addr = mbed_lwip_get_ip_addr(true, &lwip_netif);
        // Prefer IPv6
        if (IP_IS_V6(ip_addr)) {
            // If IPv4 is available use it as backup
            if (mbed_lwip_get_ipv4_addr(&lwip_netif)) {
                addr_type = NETCONN_DNS_IPV6_IPV4;
            } else {
                addr_type = NETCONN_DNS_IPV6;
            }
        // Prefer IPv4
        } else {
            // If IPv6 is available use it as backup
            if (mbed_lwip_get_ipv6_addr(&lwip_netif)) {
                addr_type = NETCONN_DNS_IPV4_IPV6;
            } else {
                addr_type = NETCONN_DNS_IPV4;
            }
        }
    } else if (version == NSAPI_IPv4) {
        addr_type = NETCONN_DNS_IPV4;
    } else if (version == NSAPI_IPv6) {
        addr_type = NETCONN_DNS_IPV6;
    } else {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    err_t err = netconn_gethostbyname_addrtype(host, &lwip_addr, addr_type);
#elif LWIP_IPV4
    if (version != NSAPI_IPv4 && version != NSAPI_UNSPEC) {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    err_t err = netconn_gethostbyname(host, &lwip_addr);
#elif LWIP_IPV6
    if (version != NSAPI_IPv6 && version != NSAPI_UNSPEC) {
        return NSAPI_ERROR_DNS_FAILURE;
    }
    err_t err = netconn_gethostbyname(host, &lwip_addr);
#endif

    if (err != ERR_OK) {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    convert_lwip_quectelm66_addr_to_mbed(addr, &lwip_addr);

    return 0;

}

static nsapi_error_t mbed_lwip_quectelm66_add_dns_server(nsapi_stack_t *stack, nsapi_addr_t addr)
{
    // Shift all dns servers down to give precedence to new server
	for (int i = DNS_MAX_SERVERS - 1; i > 0; i--) {
		dns_setserver(i, dns_getserver(i - 1));
	}

	ip_addr_t ip_addr;
	if (!convert_mbed_addr_to_lwip_quectelm66(&ip_addr, &addr)) {
		return NSAPI_ERROR_PARAMETER;
	}

	dns_setserver(0, &ip_addr);
	return 0;
}

static nsapi_error_t mbed_lwip_quectelm66_socket_open(nsapi_stack_t *stack, nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    // check if network is connected
	if (!lwip_quectelm66_connected) {
		return NSAPI_ERROR_NO_CONNECTION;
	}

	    // allocate a socket
	struct lwip_quectelm66_socket *s = mbed_lwip_quectelm66_arena_alloc();
	if (!s) {
		return NSAPI_ERROR_NO_SOCKET;
	}

	u8_t lwip_proto = proto == NSAPI_TCP ? NETCONN_TCP : NETCONN_UDP;

#if LWIP_IPV6 && LWIP_IPV4
	const ip_addr_t *ip_addr;
	ip_addr = mbed_lwip_get_ip_addr(true, &lwip_netif);

	if (IP_IS_V6(ip_addr)) {
	    // Enable IPv6 (or dual-stack). LWIP dual-stack support is
	    // currently incomplete as of 2.0.0rc2 - eg we will only be able
	    // to do a UDP sendto to an address matching the type selected
	    // here. Matching "get_ip_addr" and DNS logic, use v4 if
	    // available.
		lwip_proto |= NETCONN_TYPE_IPV6;
	}
#elif LWIP_IPV6
	lwip_proto |= NETCONN_TYPE_IPV6;
#endif

	s->conn = netconn_new_with_callback(lwip_proto, mbed_lwip_quectelm66_socket_callback);

	if (!s->conn) {
		mbed_lwip_quectelm66_arena_dealloc(s);
		return NSAPI_ERROR_NO_SOCKET;
	}

	netconn_set_recvtimeout(s->conn, 1);
	*(struct lwip_quectelm66_socket **)handle = s;
	return 0;
}

static nsapi_error_t mbed_lwip_quectelm66_socket_close(nsapi_stack_t *stack, nsapi_socket_t handle)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;

	netbuf_delete(s->buf);
	err_t err = netconn_delete(s->conn);
	mbed_lwip_quectelm66_arena_dealloc(s);
	return mbed_lwip_quectelm66_err_remap(err);
}

static nsapi_error_t mbed_lwip_quectelm66_socket_bind(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;
	ip_addr_t ip_addr;

	if ((s->conn->type == NETCONN_TCP && s->conn->pcb.tcp->local_port != 0) ||
	    (s->conn->type == NETCONN_UDP && s->conn->pcb.udp->local_port != 0)) {
		return NSAPI_ERROR_PARAMETER;
	}

	if (!convert_mbed_addr_to_lwip_quectelm66(&ip_addr, &addr)) {
		return NSAPI_ERROR_PARAMETER;
	}

	err_t err = netconn_bind(s->conn, &ip_addr, port);
	return mbed_lwip_quectelm66_err_remap(err);
}

static nsapi_error_t mbed_lwip_quectelm66_socket_listen(nsapi_stack_t *stack, nsapi_socket_t handle, int backlog)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;

	err_t err = netconn_listen_with_backlog(s->conn, backlog);
	return mbed_lwip_quectelm66_err_remap(err);
}

static nsapi_error_t mbed_lwip_quectelm66_socket_connect(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;
	ip_addr_t ip_addr;

	if (!convert_mbed_addr_to_lwip_quectelm66(&ip_addr, &addr)) {
		return NSAPI_ERROR_PARAMETER;
	}

	netconn_set_nonblocking(s->conn, false);
	err_t err = netconn_connect(s->conn, &ip_addr, port);
	netconn_set_nonblocking(s->conn, true);

	return mbed_lwip_quectelm66_err_remap(err);
}

static nsapi_error_t mbed_lwip_quectelm66_socket_accept(nsapi_stack_t *stack, nsapi_socket_t server, nsapi_socket_t *handle, nsapi_addr_t *addr, uint16_t *port)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)server;
	struct lwip_quectelm66_socket *ns = mbed_lwip_quectelm66_arena_alloc();
	if (!ns) {
		return NSAPI_ERROR_NO_SOCKET;
	}

	err_t err = netconn_accept(s->conn, &ns->conn);
	if (err != ERR_OK) {
		mbed_lwip_quectelm66_arena_dealloc(ns);
		return mbed_lwip_quectelm66_err_remap(err);
	}

	netconn_set_recvtimeout(ns->conn, 1);
	*(struct lwip_socket **)handle = ns;

	ip_addr_t peer_addr;
	(void) netconn_peer(ns->conn, &peer_addr, port);
	convert_lwip_quectelm66_addr_to_mbed(addr, &peer_addr);

	return 0;
}

static nsapi_size_or_error_t mbed_lwip_quectelm66_socket_send(nsapi_stack_t *stack, nsapi_socket_t handle, const void *data, nsapi_size_t size)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;
	size_t bytes_written = 0;

	err_t err = netconn_write_partly(s->conn, data, size, NETCONN_COPY, &bytes_written);
	if (err != ERR_OK) {
		return mbed_lwip_quectelm66_err_remap(err);
	}

	return (nsapi_size_or_error_t)bytes_written;
}

static nsapi_size_or_error_t mbed_lwip_quectelm66_socket_recv(nsapi_stack_t *stack, nsapi_socket_t handle, void *data, nsapi_size_t size)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;

	if (!s->buf) {
		err_t err = netconn_recv(s->conn, &s->buf);
		s->offset = 0;

		if (err != ERR_OK) {
			return mbed_lwip_quectelm66_err_remap(err);
		}
	}

	u16_t recv = netbuf_copy_partial(s->buf, data, (u16_t)size, s->offset);
	s->offset += recv;

	if (s->offset >= netbuf_len(s->buf)) {
		netbuf_delete(s->buf);
		s->buf = 0;
	}

	return recv;
}

static nsapi_size_or_error_t mbed_lwip_quectelm66_socket_sendto(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t addr, uint16_t port, const void *data, nsapi_size_t size)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;
	ip_addr_t ip_addr;

	if (!convert_mbed_addr_to_lwip_quectelm66(&ip_addr, &addr)) {
		return NSAPI_ERROR_PARAMETER;
	}

	struct netbuf *buf = netbuf_new();
	err_t err = netbuf_ref(buf, data, (u16_t)size);
	if (err != ERR_OK) {
		netbuf_free(buf);
		return mbed_lwip_quectelm66_err_remap(err);
	}

	err = netconn_sendto(s->conn, buf, &ip_addr, port);
	netbuf_delete(buf);
	if (err != ERR_OK) {
		return mbed_lwip_quectelm66_err_remap(err);
	}

	return size;
}

static nsapi_size_or_error_t mbed_lwip_quectelm66_socket_recvfrom(nsapi_stack_t *stack, nsapi_socket_t handle, nsapi_addr_t *addr, uint16_t *port, void *data, nsapi_size_t size)
{
	struct lwip_quectelm66_socket *s = (struct lwip_socket *)handle;
	struct netbuf *buf;

	err_t err = netconn_recv(s->conn, &buf);
	if (err != ERR_OK) {
		return mbed_lwip_quectelm66_err_remap(err);
	}

	convert_lwip_quectelm66_addr_to_mbed(addr, netbuf_fromaddr(buf));
	*port = netbuf_fromport(buf);

	u16_t recv = netbuf_copy(buf, data, (u16_t)size);
	netbuf_delete(buf);

	return recv;
}

static nsapi_error_t mbed_lwip_quectelm66_setsockopt(nsapi_stack_t *stack, nsapi_socket_t handle, int level, int optname, const void *optval, unsigned optlen)
{
	struct lwip_quectelm66_socket *s = (struct lwip_quectelm66_socket *)handle;

	switch (optname) {
	case NSAPI_KEEPALIVE:
		if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
			return NSAPI_ERROR_UNSUPPORTED;
		}

		s->conn->pcb.tcp->so_options |= SOF_KEEPALIVE;
		return 0;

	case NSAPI_KEEPIDLE:
		if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
			return NSAPI_ERROR_UNSUPPORTED;
		}

		s->conn->pcb.tcp->keep_idle = *(int*)optval;
		return 0;

	case NSAPI_KEEPINTVL:
		if (optlen != sizeof(int) || s->conn->type != NETCONN_TCP) {
			return NSAPI_ERROR_UNSUPPORTED;
		}

		s->conn->pcb.tcp->keep_intvl = *(int*)optval;
		return 0;

	case NSAPI_REUSEADDR:
		if (optlen != sizeof(int)) {
			return NSAPI_ERROR_UNSUPPORTED;
		}

		if (*(int *)optval) {
			s->conn->pcb.tcp->so_options |= SOF_REUSEADDR;
		}
		else {
			s->conn->pcb.tcp->so_options &= ~SOF_REUSEADDR;
		}
		return 0;

	default:
		return NSAPI_ERROR_UNSUPPORTED;
	}
}

static void mbed_lwip_quectelm66_socket_attach(nsapi_stack_t *stack, nsapi_socket_t handle, void(*callback)(void *), void *data)
{
	struct lwip_quectelm66_socket *s = (struct lwip_quectelm66_socket *)handle;

	s->cb = callback;
	s->data = data;
}

/* LWIP network stack */
const nsapi_stack_api_t lwip_quectelm66_stack_api = {
	.gethostbyname = mbed_lwip_quectelm66_gethostbyname, // ok
	.add_dns_server = mbed_lwip_quectelm66_add_dns_server, // ok
	.socket_open = mbed_lwip_quectelm66_socket_open, // ok
	.socket_close = mbed_lwip_quectelm66_socket_close, // ok
	.socket_bind = mbed_lwip_quectelm66_socket_bind, // ok
	.socket_listen = mbed_lwip_quectelm66_socket_listen, // ok
	.socket_connect = mbed_lwip_quectelm66_socket_connect, // ok
	.socket_accept = mbed_lwip_quectelm66_socket_accept, // ok
	.socket_send = mbed_lwip_quectelm66_socket_send, // ok
	.socket_recv = mbed_lwip_quectelm66_socket_recv, // ok
	.socket_sendto = mbed_lwip_quectelm66_socket_sendto, // ok
	.socket_recvfrom = mbed_lwip_quectelm66_socket_recvfrom, // ok
	.setsockopt = mbed_lwip_quectelm66_setsockopt, // ok
	.socket_attach = mbed_lwip_quectelm66_socket_attach, // ok
};

nsapi_stack_t lwip_quectelm66_stack = {
	.stack_api = &lwip_quectelm66_stack_api,
};
