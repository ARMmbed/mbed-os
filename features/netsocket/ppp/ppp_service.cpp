/*
 * Copyright (c) 2019 ARM Limited
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

#include <stdlib.h>
#include <stdio.h>
#include "mbed_interface.h"
#include "mbed_trace.h"
#include "netsocket/nsapi_types.h"

#if PPP_MBED_EVENT_QUEUE
#include "mbed_shared_queues.h"
#else
#include "Thread.h"
#include "EventQueue.h"
#endif

#include "mbed_poll.h"
#include "ip4string.h"
#include "ip6string.h"
#include "ppp_service.h"

#include "ppp_opts.h"
#include "ppp_service_if.h"

extern "C" { // "pppos.h" is missing extern C
#include "pppos.h"
#include "ppp_impl.h"
}

#define TRACE_GROUP "ppp_serv"

#define PPP_SERVICE_IF_NAME "pp"

#if PPP_SUPPORT

#if PPP_DEBUG || PPP_TRACE_ENABLE
// PPP internal traces enabled
#define PPP_THREAD_STACKSIZE   MBED_CONF_PPP_THREAD_STACKSIZE * 3
#elif MBED_DEBUG
// Compiled with debug enabled
#define PPP_THREAD_STACKSIZE   MBED_CONF_PPP_THREAD_STACKSIZE * 2
#else
#define PPP_THREAD_STACKSIZE   MBED_CONF_PPP_THREAD_STACKSIZE
#endif

/* Timeout to wait for PPP connection to be terminated
 * (LCP Terminate-Request is answered with Terminate-Ack)
 */
#define PPP_TERMINATION_TIMEOUT           30000

// If both IPCP and IPCP6 are made, how long to wait for both to complete
#define PPP_IPCP_IPCP6_DELAY              5000

bool ppp_service::prepare_event_queue()
{
#if PPP_MBED_EVENT_QUEUE
    ppp_service_event_queue = mbed::mbed_event_queue();
    if (!ppp_service_event_queue) {
        return false;
    }

    return true;
#else
    static events::EventQueue *event_queue = nullptr;
    static rtos::Thread *event_thread = nullptr;
    // Already prepared
    if (event_queue && event_thread) {
        return true;
    }

    // Used for incoming data, timers, link status callback, power up callback
    event_queue = new events::EventQueue(10 * EVENTS_EVENT_SIZE, nullptr);
    event_thread = new rtos::Thread(osPriorityNormal, PPP_THREAD_STACKSIZE, nullptr, "ppp");

    if (event_thread->start(callback(event_queue, &events::EventQueue::dispatch_forever)) != osOK) {
        delete event_thread;
        delete event_queue;
        return false;
    }

    ppp_service_event_queue = event_queue;

    return true;
#endif
}

uint32_t ppp_output(struct ppp_pcb_s *pcb, uint8_t *data, uint32_t len, void *ctx)
{
#if PPP_TRACE_ENABLE
    ppp_trace_to_ascii_hex_dump(OUTPUT_BUFFER, len, reinterpret_cast<char *>(data));
    ppp_trace_to_ascii_hex_dump_print(OUTPUT_BUFFER);
#endif

    mbed::FileHandle *stream = (mbed::FileHandle *) pcb->netif->stream;
    if (!stream) {
        return 0;
    }

    mbed::pollfh fhs;
    fhs.fh = stream;
    fhs.events = POLLOUT;

    // stack expects us to block on write
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

    return written;
}

#if !PPP_INPROC_IRQ_SAFE
#error "PPP_INPROC_IRQ_SAFE must be enabled"
#endif
void ppp_service::ppp_input()
{
    // Allow new events from now, avoiding potential races around the read
    ppp_service_event_queued = false;

    if (!ppp_service_stream) {
        return;
    }

    // Non-blocking error check handler
    mbed::pollfh fhs;
    fhs.fh = ppp_service_stream;
    fhs.events = POLLIN;
    poll(&fhs, 1, 0);
    if (fhs.revents & (POLLHUP | POLLERR | POLLNVAL)) {
        ppp_handle_modem_hangup();
        return;
    }

    // Infinite loop, but we assume that we can read faster than the
    // serial, so we will fairly rapidly hit -EAGAIN.
    for (;;) {
        u8_t buffer[16];

        ssize_t len = ppp_service_stream->read(buffer, sizeof buffer);

        if (len == -EAGAIN) {
            break;
        } else if (len <= 0) {
            ppp_handle_modem_hangup();
            return;
        }

#if PPP_TRACE_ENABLE
        ppp_trace_to_ascii_hex_dump(INPUT_BUFFER, len, reinterpret_cast<char *>(buffer));
#endif

        pppos_input(ppp_service_pcb, buffer, len);
    }
}

void ppp_service::ppp_stream_sigio_callback()
{
    if (ppp_service_stream && !ppp_service_event_queued) {
        ppp_service_event_queued = true;
        if (ppp_service_event_queue->call(mbed::callback(this, &ppp_service::ppp_input)) == 0) {
            ppp_service_event_queued = false;
        }
    }
}

void ppp_service::ppp_handle_modem_hangup()
{
    if (ppp_service_pcb->phase != PPP_PHASE_DEAD) {
        ppp_close(ppp_service_pcb, 1);
    }
}

void ppp_service::ppp_link_status(struct ppp_pcb_s *pcb, int err_code, void *ctx)
{
    ppp_service *ppp_service_ptr = static_cast<ppp_service *>(pcb->netif->service_ptr);

    switch (err_code) {
        case PPPERR_NONE:
#if PPP_DEBUG
            PPPDEBUG(LOG_DEBUG, ("status_cb: Connected"));
            char address[40];
#if PPP_IPV4_SUPPORT
            ip4tos(pcb->netif->ipv4_addr.bytes, address);
            PPPDEBUG(LOG_DEBUG, ("   our_ipaddr  = %s", address));
            ip4tos(pcb->netif->ipv4_gateway.bytes, address);
            PPPDEBUG(LOG_DEBUG, ("   his_ipaddr  = %s", address));
            ip4tos(pcb->netif->ipv4_netmask.bytes, address);
            PPPDEBUG(LOG_DEBUG, ("   netmask     = %s", address));
            if (pcb->netif->ipv4_dns_server[0].version == NSAPI_IPv4) {
                ip4tos(pcb->netif->ipv4_dns_server[0].bytes, address);
                PPPDEBUG(LOG_DEBUG, ("   dns1        = %s", address));
            }
            if (pcb->netif->ipv4_dns_server[1].version == NSAPI_IPv4) {
                ip4tos(pcb->netif->ipv4_dns_server[1].bytes, address);
                PPPDEBUG(LOG_DEBUG, ("   dns2        = %s", address));
            }
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
            ip6tos(pcb->netif->ipv6_addr.bytes, address);
            PPPDEBUG(LOG_DEBUG, ("   our6_ipaddr = %s", address));
#endif /* PPP_IPV6_SUPPORT */
#endif
            break;

        case PPPERR_PARAM:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Invalid parameter"));
            break;

        case PPPERR_OPEN:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Unable to open PPP session"));
            break;

        case PPPERR_DEVICE:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Invalid I/O device for PPP"));
            break;

        case PPPERR_ALLOC:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Unable to allocate resources"));
            break;

        case PPPERR_USER:
            PPPDEBUG(LOG_DEBUG, ("status_cb: User interrupt"));
            break;

        case PPPERR_CONNECT:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Connection lost"));
            break;

        case PPPERR_AUTHFAIL:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Failed authentication challenge"));
            break;

        case PPPERR_PROTOCOL:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Failed to meet protocol"));
            break;

        case PPPERR_PEERDEAD:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Connection timeout"));
            break;

        case PPPERR_IDLETIMEOUT:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Idle Timeout"));
            break;

        case PPPERR_CONNECTTIME:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Max connect time reached"));
            break;

        case PPPERR_LOOPBACK:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Loopback detected"));
            break;

        default:
            PPPDEBUG(LOG_DEBUG, ("status_cb: Unknown error code %d", err_code));
            break;
    }

    if (err_code == PPPERR_NONE) {
        return;
    }

    /* If some error happened, we need to properly shutdown the PPP interface  */
    if (ppp_service_ptr->ppp_service_active) {
        ppp_service_ptr->ppp_service_active = false;
        ppp_service_ptr->ppp_service_close_sem.release();
    }
}

nsapi_error_t ppp_service::ppp_stack_if_init()
{
    ppp_init();

    if (!ppp_service_pcb) {
        ppp_service_pcb = pppos_create(ppp_service_netif,
                                       ppp_output, ppp_link_status, nullptr);
        if (!ppp_service_pcb) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

#if PPP_IPV4_SUPPORT
    ppp_pcb *pcb = ppp_service_pcb;

#if PPP_IPV6_SUPPORT
    if (ppp_service_stack != IPV6_STACK) {
        ppp_set_usepeerdns(pcb, true);
    }
#else
    ppp_set_usepeerdns(pcb, true);
#endif
#endif

#if PPP_IPV4_SUPPORT && PPP_IPV6_SUPPORT
    if (ppp_service_stack == IPV4_STACK) {
        pcb->ipv6cp_disabled = true;
    } else if (ppp_service_stack == IPV6_STACK) {
        pcb->ipcp_disabled = true;
    }
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t ppp_service::ppp_if_connect()
{
    ppp_pcb *pcb = ppp_service_pcb;

#if PPP_AUTH_SUPPORT
    ppp_set_auth(pcb, PPPAUTHTYPE_ANY, ppp_service_uname, ppp_service_password);
#endif //PPP_AUTH_SUPPORT
    ppp_service_active = true;
    ppp_service_terminating = false;
    err_t ret = ppp_connect(pcb, 0);
    // input must not be called until after connect
    if (ret == ERR_OK) {
        ppp_service_stream->sigio(mbed::callback(this, &ppp_service::ppp_stream_sigio_callback));
    } else {
        ppp_service_active = false;
    }

    return ret;
}

nsapi_error_t ppp_service::ppp_if_disconnect()
{
    err_t ret = ERR_OK;
    if (ppp_service_active) {
        ppp_service_terminating = true;
        ret = ppp_close(ppp_service_pcb, 0);
        if (ret == ERR_OK) {
            /* close call made, now let's catch the response in the status callback */
            ppp_service_close_sem.try_acquire_for(PPP_TERMINATION_TIMEOUT);
        }
        ppp_service_active = false;
    }
    return ret;
}

ppp_service::ppp_service()
{
    ppp_service_netif = new (std::nothrow) netif{};
    if (ppp_service_netif) {
        ppp_service_netif->service_ptr = this;
    }
    ppp_service_active = false;
    ppp_service_event_queued = false;
    ppp_service_terminating = false;
    ppp_link_state_up = false;
}

bool ppp_service::link_out(net_stack_mem_buf_t *buf, nsapi_ip_stack_t ip_stack)
{
    if (ppp_service_terminating) {
        memory_manager->free(buf);
        return true;
    }

    struct pbuf *p = ppp_memory_buffer_convert_to(memory_manager, buf);
    if (!p) {
        memory_manager->free(buf);
        return true;
    }

#if PPP_IPV4_SUPPORT && PPP_IPV6_SUPPORT
    if (ip_stack == IPV4_STACK) {
        ppp_service_netif->output(ppp_service_netif, p, nullptr);
    } else {
        ppp_service_netif->output_ip6(ppp_service_netif, p, nullptr);
    }
#elif PPP_IPV4_SUPPORT
    ppp_service_netif->output(ppp_service_netif, p, nullptr);
#elif PPP_IPV6_SUPPORT
    ppp_service_netif->output_ip6(ppp_service_netif, p, nullptr);
#endif

    ppp_memory_buffer_free(p); // Not done on PPP lower layers
    return true;
}

bool ppp_service::power_up()
{
    if (!ppp_service_netif || !ppp_service_stream) {
        return false;
    }

    if (!prepare_event_queue()) {
        return false;
    }

    if (ppp_service_event_queue->call(mbed::callback(this, &ppp_service::power_up_call)) == 0) {
        return false;
    }

    return true;
}

void ppp_service::power_up_call()
{
    nsapi_error_t err = ppp_stack_if_init();

    if (err != NSAPI_ERROR_OK) {
        return;
    }

    err = ppp_if_connect();

    if (err != NSAPI_ERROR_OK) {
        return;
    }
}

uint32_t ppp_service::get_mtu_size()
{
    if (!ppp_service_netif) {
        return 0;
    }

    return ppp_service_netif->mtu;
}

uint32_t ppp_service::get_align_preference() const
{
    return 0;
}

void ppp_service::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, PPP_SERVICE_IF_NAME, (size < sizeof(PPP_SERVICE_IF_NAME)) ? size : sizeof(PPP_SERVICE_IF_NAME));
}

void ppp_service::set_link_input_cb(ppp_link_input_cb_t input_cb)
{
    ppp_link_input_cb = input_cb;
}

void ppp_service::set_link_state_cb(ppp_link_state_change_cb_t state_cb)
{
    ppp_link_state_cb = state_cb;
}

void ppp_service::power_down()
{
    ppp_if_disconnect();
}

void ppp_service::set_memory_manager(NetStackMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;

    if (!ppp_service_netif) {
        return;
    }
    ppp_service_netif->memory_manager = &mem_mngr;
}

void ppp_service::set_stream(mbed::FileHandle *stream)
{
    ppp_service_stream = stream;
    if (!ppp_service_netif) {
        return;
    }
    ppp_service_netif->stream = stream;
}

void ppp_service::set_ip_stack(nsapi_ip_stack_t ip_stack)
{
    ppp_service_stack = ip_stack;
}

void ppp_service::set_credentials(const char *uname, const char *password)
{
    if (strlen(uname) > 0) {
        ppp_service_uname = uname;
    } else {
        ppp_service_uname = nullptr;
    }
    if (strlen(password) > 0) {
        ppp_service_password = password;
    } else {
        password = nullptr;
    }
}

const nsapi_addr_t *ppp_service::get_ip_address(nsapi_version_t version)
{
#if PPP_IPV6_SUPPORT
    if (version == NSAPI_IPv6 && ppp_service_netif->ipv6_addr.version == NSAPI_IPv6) {
        return &ppp_service_netif->ipv6_addr;
    }
#endif

#if PPP_IPV4_SUPPORT
    if (version == NSAPI_IPv4 && ppp_service_netif->ipv4_addr.version == NSAPI_IPv4) {
        return &ppp_service_netif->ipv4_addr;
    }
#endif

    return nullptr;
}

const nsapi_addr_t *ppp_service::get_netmask()
{
#if PPP_IPV4_SUPPORT
    if (ppp_service_netif->ipv4_netmask.version == NSAPI_IPv4) {
        return &ppp_service_netif->ipv4_netmask;
    }
#endif

    return nullptr;
}

const nsapi_addr_t *ppp_service::get_gateway()
{
#if PPP_IPV4_SUPPORT
    if (ppp_service_netif->ipv4_gateway.version == NSAPI_IPv4) {
        return &ppp_service_netif->ipv4_gateway;
    }
#endif

    return nullptr;
}

const nsapi_addr_t *ppp_service::get_dns_server(uint8_t index)
{
#if PPP_IPV4_SUPPORT
    if (index > 1) {
        return nullptr;
    }
    if (ppp_service_netif->ipv4_dns_server[index].version == NSAPI_IPv4) {
        return &ppp_service_netif->ipv4_dns_server[index];
    }
#endif

    return nullptr;
}

void ppp_service::link_state(bool up)
{
    if (!ppp_service_active) {
        return;
    }

    bool call_link_state = false;
#if PPP_IPV4_SUPPORT && PPP_IPV6_SUPPORT
    bool callin_link_state = false;
    if (ppp_service_stack == IPV4V6_STACK) {
        if (up) {
            if (ppp_service_netif->ipv4_up && ppp_service_netif->ipv6_up) {
                call_link_state = true;
            } else {
                callin_link_state = true;
            }
        } else {
            call_link_state = true;
        }
    } else {
        call_link_state = true;
    }
#else
    call_link_state = true;
#endif

    if (call_link_state) {
        if (ppp_service_event_queue->call(mbed::callback(this, &ppp_service::link_state_call), up) == 0) {
            return;
        }
    }
#if PPP_IPV4_SUPPORT && PPP_IPV6_SUPPORT
    if (callin_link_state) {
        if (ppp_service_event_queue->call_in(PPP_IPCP_IPCP6_DELAY, mbed::callback(this, &ppp_service::link_state_call), up) == 0) {
            return;
        }
    }
#endif
}

void ppp_service::link_state_call(bool up)
{
    if (ppp_link_state_up != up) {
        ppp_link_state_up = up;
        ppp_link_state_cb(up);
    }
}

void ppp_service::link_input(net_stack_mem_buf_t *buf)
{
    if (ppp_service_terminating) {
        memory_manager->free(buf);
        return;
    }

    ppp_link_input_cb(buf);
}

events::EventQueue *ppp_service::event_queue_get()
{
    return ppp_service_event_queue;
}

void ppp_service::resource_lock()
{
    ppp_service_mutex.lock();
}

void ppp_service::resource_unlock()
{
    ppp_service_mutex.unlock();
}

ppp_service &ppp_service::get_instance()
{
    static ppp_service ppp_service_instance;
    return ppp_service_instance;
}

// Weak so a module can override
MBED_WEAK PPP &PPP::get_default_instance()
{
    return ppp_service::get_instance();
}

#endif

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

