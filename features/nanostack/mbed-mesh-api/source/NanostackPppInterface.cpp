/*
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
 */

#include "NanostackPppInterface.h"
#include "ppp_lwip.h"
#include "mesh_system.h"
#include "nsapi_ppp.h"

using namespace mbed;
static FileHandle *my_stream;
rtos::Semaphore ppp_close_sem;
static OnboardNetworkStack::Interface *my_interface;
static ppp_pcb *my_ppp_pcb;
static bool ppp_active = false;
static bool blocking_connect = true;
static const char *login;
static const char *pwd;
static Callback<void(nsapi_event_t, intptr_t)> connection_status_cb;
static nsapi_error_t connect_error_code;


/* Interface implementation */
NanostackPppInterface::NanostackPppInterface(NanostackMemoryManager &mem, void *_hw, bool default_if, nsapi_ip_stack_t stack) :
	hw(_hw), has_addr_state(0), connected(NSAPI_STATUS_DISCONNECTED), dhcp_started(false),
    dhcp_has_to_be_set(false), blocking(true), ppp(true), memory_manager(mem)
{
	memset(&netif, 0, sizeof netif);

	osSemaphoreAttr_t attr;
    attr.name = NULL;
    attr.attr_bits = 0;

    attr.cb_mem = &linked_sem;
    attr.cb_size = sizeof linked_sem;
    linked = osSemaphoreNew(UINT16_MAX, 0, &attr);

    attr.cb_mem = &unlinked_sem;
    attr.cb_size = sizeof unlinked_sem;
    unlinked = osSemaphoreNew(UINT16_MAX, 0, &attr);

    attr.cb_mem = &has_any_addr_sem;
    attr.cb_size = sizeof has_any_addr_sem;
    has_any_addr = osSemaphoreNew(UINT16_MAX, 0, &attr);
}

NanostackPppInterface::~ NanostackPppInterface()
{
}

nsapi_error_t NanostackPppInterface::connect()
{
    return NSAPI_ERROR_NO_CONNECTION;
}
nsapi_error_t NanostackPppInterface::disconnect()
{
    return NSAPI_ERROR_NO_CONNECTION;
}

NetworkStack *NanostackPppInterface::get_stack()
{
    return &OnboardNetworkStack::get_default_instance();
}

nsapi_error_t NanostackPppInterface::bringup(bool dhcp, const char *ip, const char *netmask, const char *gw, const nsapi_ip_stack_t stack, bool block)
{
    nanostack_lock();
    blocking = blocking;

/*    mesh_error_t status = init();
    if (status != MESH_ERROR_NONE) {
        nanostack_unlock();
        return map_mesh_error(status);
    }

    if (client_callback) {
        client_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_CONNECTING);
    }
*/
    err_t err = ppp_if_connect(hw);
    /*    if (err) {
        connected = NSAPI_STATUS_DISCONNECTED;
        if (client_callback) {
            client_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
        }
        return err_remap(err);
    }*/
/*
    status = mesh_connect();
    if (status != MESH_ERROR_NONE) {
        nanostack_unlock();
        return map_mesh_error(status);
    }
*/
    // Release mutex before blocking
    nanostack_unlock();
/*
    if (blocking) {
        // wait connection for ever
        int32_t count = connect_semaphore.wait(osWaitForever);

        if (count <= 0) {
            return NSAPI_ERROR_DHCP_FAILURE; // sort of...
        }
    }*/
    return NSAPI_ERROR_OK;
}

nsapi_error_t NanostackPppInterface::bringdown()
{
    return NSAPI_ERROR_NO_CONNECTION;
}
char *NanostackPppInterface::get_gateway(char *buf, nsapi_size_t buflen)
{
	return NULL;
}
char *NanostackPppInterface::get_mac_address(char *buf, nsapi_size_t buflen)
{
    (void) snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x",
                    netif.hwaddr[0], netif.hwaddr[1], netif.hwaddr[2],
                    netif.hwaddr[3], netif.hwaddr[4], netif.hwaddr[5]);
    return buf;
}

char *NanostackPppInterface::get_interface_name(char *buf)
{
    sprintf(buf, "%c%c%d", netif.name[0], netif.name[1], netif.num);
    return buf;
}

char *NanostackPppInterface::get_ip_address(char *buf, nsapi_size_t buflen)
{
	return (char *)get_stack()->get_ip_address();
}

char *NanostackPppInterface::get_ip_address_if(char *buf, nsapi_size_t buflen, const char *interface_name)
{
    return NULL;
}

char *NanostackPppInterface::get_netmask(char *buf, nsapi_size_t buflen)
{
    return NULL;
}

void NanostackPppInterface::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    client_callback = status_cb;
}

nsapi_connection_status_t NanostackPppInterface::get_connection_status() const
{
    return connected;
}

static u32_t ppp_output(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx)
{
#if 0
    FileHandle *stream = my_stream;
    if (!stream) {
        return 0;
    }

    pollfh fhs;
    fhs.fh = stream;
    fhs.events = POLLOUT;

    // LWIP expects us to block on write
    // File handle will be in non-blocking mode, because of read events.
    // Therefore must use poll to achieve the necessary block for writing.

    uint32_t written = 0;
    while (len != 0) {
        // Block forever until we're selected - don't care about reason we wake;
        // return from write should tell us what's up.
        poll(&fhs, 1, -1);
        // This write will be non-blocking, but blocking would be fine.
        ssize_t ret = stream->write(data, len);
        if (ret == -EAGAIN) {
            continue;
        } else if (ret < 0) {
            break;
        }
        written += ret;
        data += ret;
        len -= ret;
    }

//    /tr_debug("> %ld bytes of data written\n", (long) written);
#endif
    uint32_t written = 0;
    return written;
}

static void ppp_link_status(ppp_pcb *pcb, int err_code, void *ctx)
{
    nsapi_error_t mapped_err_code = NSAPI_ERROR_NO_CONNECTION;
#if 0
    switch (err_code) {
        case PPPERR_NONE:
            mapped_err_code = NSAPI_ERROR_OK;
            tr_info("status_cb: Connected");
#if PPP_IPV4_SUPPORT
            tr_debug("   our_ipaddr  = %s", ipaddr_ntoa(&ppp_netif(pcb)->ip_addr));
            tr_debug("   his_ipaddr  = %s", ipaddr_ntoa(&ppp_netif(pcb)->gw));
            tr_debug("   netmask     = %s", ipaddr_ntoa(&ppp_netif(pcb)->netmask));
#if LWIP_DNS
            const ip_addr_t *ns;
            ns = dns_getserver(0, NULL);
            if (ns) {
                tr_debug("   dns1        = %s", ipaddr_ntoa(ns));
            }
            ns = dns_getserver(1, NULL);
            if (ns) {
                tr_debug("   dns2        = %s", ipaddr_ntoa(ns));
            }
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
            tr_debug("   our6_ipaddr = %s", ip6addr_ntoa(netif_ip6_addr(ppp_netif(pcb), 0)));
#endif /* PPP_IPV6_SUPPORT */
            break;

        case PPPERR_PARAM:
            tr_info("status_cb: Invalid parameter");
            break;

        case PPPERR_OPEN:
            tr_info("status_cb: Unable to open PPP session");
            break;

        case PPPERR_DEVICE:
            tr_info("status_cb: Invalid I/O device for PPP");
            break;

        case PPPERR_ALLOC:
            tr_info("status_cb: Unable to allocate resources");
            break;

        case PPPERR_USER:
            tr_info("status_cb: User interrupt");
            break;

        case PPPERR_CONNECT:
            tr_info("status_cb: Connection lost");
            mapped_err_code = NSAPI_ERROR_CONNECTION_LOST;
            break;

        case PPPERR_AUTHFAIL:
            tr_info("status_cb: Failed authentication challenge");
            mapped_err_code = NSAPI_ERROR_AUTH_FAILURE;
            break;

        case PPPERR_PROTOCOL:
            tr_info("status_cb: Failed to meet protocol");
            break;

        case PPPERR_PEERDEAD:
            tr_info("status_cb: Connection timeout");
            mapped_err_code = NSAPI_ERROR_CONNECTION_TIMEOUT;
            break;

        case PPPERR_IDLETIMEOUT:
            tr_info("status_cb: Idle Timeout");
            break;

        case PPPERR_CONNECTTIME:
            tr_info("status_cb: Max connect time reached");
            break;

        case PPPERR_LOOPBACK:
            tr_info("status_cb: Loopback detected");
            break;

        default:
            tr_info("status_cb: Unknown error code %d", err_code);
            break;

    }

    if (err_code == PPPERR_NONE) {
        /* status changes have to be reported */
        if (connection_status_cb) {
            connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_GLOBAL_UP);
        }
        return;
    }

    /* If some error happened, we need to properly shutdown the PPP interface  */
    if (ppp_active) {
        ppp_active = false;
        connect_error_code = mapped_err_code;
        ppp_close_sem.release();
    }

    /* Alright, PPP interface is down, we need to notify upper layer */
    if (connection_status_cb) {
        connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
    }
#endif
}

extern "C" nsapi_error_t ppp_if_init(void *pcb, struct netif *netif, const nsapi_ip_stack_t stack)
{
/*    if (!prepare_event_queue()) {
        return NSAPI_ERROR_NO_MEMORY;
    }*/

    if (!my_ppp_pcb) {
        my_ppp_pcb = pppos_create(netif, ppp_output, ppp_link_status, NULL);
        if (!my_ppp_pcb) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        //sys_sem_new(&ppp_close_sem, 0);
    }

#if LWIP_IPV4
    if (stack != IPV6_STACK) {
        ppp_set_usepeerdns(my_ppp_pcb, true);
    }
#endif

#if LWIP_IPV4 && LWIP_IPV6
    if (stack == IPV4_STACK) {
        my_ppp_pcb->ipv6cp_disabled = true;
    } else if (stack == IPV6_STACK) {
        my_ppp_pcb->ipcp_disabled = true;
    }
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::add_ppp_interface(void *hw, bool default_if, nsapi_ip_stack_t stack, OnboardNetworkStack::Interface **interface_out)
{
	printf("Nanostack::add_ppp_interface \n\r");

	NanostackPppInterface *interface = new (std::nothrow) NanostackPppInterface(this->memory_manager, hw, default_if, stack);

    if (!interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    nsapi_error_t ret = ppp_stack_if_init(hw, &interface->netif, stack);
    if (ret != NSAPI_ERROR_OK) {
        free(interface);
        return ret;
    }

    /*
    if (default_if) {
        netif_set_default(&interface->netif);
        default_interface = interface;
    }*/


    //netif_set_link_callback(&interface->netif, &LWIP::Interface::netif_link_irq);
    //netif_set_status_callback(&interface->netif, &LWIP::Interface::netif_status_irq);
    *interface_out = interface;


    return NSAPI_ERROR_OK;

}
#if 0
nsapi_error_t nsapi_ppp_connect(FileHandle *stream, Callback<void(nsapi_event_t, intptr_t)> cb, const char *uname, const char *password, const nsapi_ip_stack_t stack)
{
    if (my_stream) {
        return NSAPI_ERROR_PARAMETER;
    }
    my_stream = stream;
    stream->set_blocking(false);
    connection_status_cb = cb;
    login = uname;
    pwd = password;

    nsapi_error_t retcode;
    if (!my_interface) {
    	OnboardNetworkStack &_stack = OnboardNetworkStack::get_default_instance();
        retcode = _stack.add_ppp_interface(stream, true, stack, &my_interface);
        if (retcode != NSAPI_ERROR_OK) {
            my_interface = NULL;
            my_stream->set_blocking(true);
            my_stream = NULL;
            connection_status_cb = NULL;
            return retcode;
        }
        if (my_interface) {
            _stack.set_default_interface(my_interface);
        }
    }

    // mustn't start calling input until after connect -
    // attach deferred until ppp_connect, called from mbed_lwip_bringup
    retcode = my_interface->bringup(false, NULL, NULL, NULL, stack, blocking_connect);

    if (retcode != NSAPI_ERROR_OK) {
        connection_status_cb = NULL;
        my_stream->sigio(NULL);
        my_stream->set_blocking(true);
        my_stream = NULL;
    }

    if (retcode != NSAPI_ERROR_OK && connect_error_code != NSAPI_ERROR_OK) {
        return connect_error_code;
    } else {
        return retcode;
    }
}
#endif
