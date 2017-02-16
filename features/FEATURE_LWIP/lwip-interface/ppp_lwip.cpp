/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include <errno.h>
#include "platform/FileHandle.h"
#include "platform/mbed_poll.h"
#include "events/EventQueue.h"
#include "rtos/Thread.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/pbuf.h"
extern "C" { // "pppos.h" is missing extern C
#include "netif/ppp/pppapi.h"
}

#include "lwip_stack.h"

namespace mbed {

using rtos::Thread;
using events::EventQueue;

#if LWIP_PPP_API

static EventQueue *event_queue;
static Thread *event_thread;
static volatile bool event_queued;

// Just one interface for now
static ppp_pcb *my_ppp_pcb;

static EventQueue *prepare_event_queue()
{
    if (event_queue) {
        return event_queue;
    }

    // Should be trying to get a global shared event queue here!
    // Shouldn't have to be making a private thread!

    // Only need to queue 1 event. new blows on failure.
    event_queue = new EventQueue(2 * EVENTS_EVENT_SIZE, NULL);
    event_thread = new Thread(osPriorityNormal, 700);

    if (event_thread->start(callback(event_queue, &EventQueue::dispatch_forever)) != osOK) {
        delete event_thread;
        delete event_queue;
        return NULL;
    }

    return event_queue;
}

static u32_t ppp_output(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx)
{
    FileHandle *stream = static_cast<FileHandle *>(ctx);
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

    printf("> %ld\n", (long) written);

    return written;
}

static void ppp_link_status(ppp_pcb *pcb, int err_code, void *ctx)
{
    switch(err_code) {
        case PPPERR_NONE: {
            printf("status_cb: Connected\n");
#if PPP_IPV4_SUPPORT
            printf("   our_ipaddr  = %s\n", ipaddr_ntoa(&ppp_netif(pcb)->ip_addr));
            printf("   his_ipaddr  = %s\n", ipaddr_ntoa(&ppp_netif(pcb)->gw));
            printf("   netmask     = %s\n", ipaddr_ntoa(&ppp_netif(pcb)->netmask));
#if LWIP_DNS
            const ip_addr_t *ns;
            ns = dns_getserver(0);
            if (ns) {
                printf("   dns1        = %s\n", ipaddr_ntoa(ns));
            }
            ns = dns_getserver(1);
            if (ns) {
                printf("   dns2        = %s\n", ipaddr_ntoa(ns));
            }
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
            printf("   our6_ipaddr = %s\n", ip6addr_ntoa(netif_ip6_addr(ppp_netif(pcb), 0)));
#endif /* PPP_IPV6_SUPPORT */
            break;
        }
        case PPPERR_PARAM: {
            printf("status_cb: Invalid parameter\n");
            break;
        }
        case PPPERR_OPEN: {
            printf("status_cb: Unable to open PPP session\n");
            break;
        }
        case PPPERR_DEVICE: {
            printf("status_cb: Invalid I/O device for PPP\n");
            break;
        }
        case PPPERR_ALLOC: {
            printf("status_cb: Unable to allocate resources\n");
            break;
        }
        case PPPERR_USER: {
            printf("status_cb: User interrupt\n");
            break;
        }
        case PPPERR_CONNECT: {
            printf("status_cb: Connection lost\n");
            break;
        }
        case PPPERR_AUTHFAIL: {
            printf("status_cb: Failed authentication challenge\n");
            break;
        }
        case PPPERR_PROTOCOL: {
            printf("status_cb: Failed to meet protocol\n");
            break;
        }
        case PPPERR_PEERDEAD: {
            printf("status_cb: Connection timeout\n");
            break;
        }
        case PPPERR_IDLETIMEOUT: {
            printf("status_cb: Idle Timeout\n");
            break;
        }
        case PPPERR_CONNECTTIME: {
            printf("status_cb: Max connect time reached\n");
            break;
        }
        case PPPERR_LOOPBACK: {
            printf("status_cb: Loopback detected\n");
            break;
        }
        default: {
            printf("status_cb: Unknown error code %d\n", err_code);
            break;
        }
    }

#if 0
    /*
     * This should be in the switch case, this is put outside of the switch
     * case for example readability.
     */

    if (err_code == PPPERR_NONE) {
        return;
    }

    /* ppp_close() was previously called, don't reconnect */
    if (err_code == PPPERR_USER) {
        /* ppp_free(); -- can be called here */
        return;
    }

    /*
     * Try to reconnect in 30 seconds, if you need a modem chatscript you have
     * to do a much better signaling here ;-)
     */
    ppp_connect(pcb, 30);
    /* OR ppp_listen(pcb); */

#endif
}

static void flush(FileHandle *stream)
{
    char buffer[8];
    for (;;) {
        ssize_t ret = stream->read(buffer, sizeof buffer);
        if (ret <= 0) {
            break;
        }
    }
}

#if !PPP_INPROC_IRQ_SAFE
#error "PPP_INPROC_IRQ_SAFE must be enabled"
#endif
static void ppp_input(FileHandle *stream)
{
    // Allow new events from now, avoiding potential races around the read
    event_queued = false;

    if (!my_stream) {
        return;
    }

    // Non-blocking error check handler
    pollfh fhs;
    fhs.fh = my_stream;
    fhs.events = POLLIN;
    poll(&fhs, 1, 0);
    if (fhs.revents & (POLLHUP|POLLERR|POLLNVAL)) {
        goto hup;
    }

    // Infinite loop, but we assume that we can read faster than the
    // serial, so we will fairly rapidly hit -EAGAIN.
    for (;;) {
        u8_t buffer[16];
        ssize_t len = stream->read(buffer, sizeof buffer);
        if (len == -EAGAIN) {
            break;
        } else if (len <= 0) {
            goto hup;
        }
        pppos_input(my_ppp_pcb, buffer, len);
    }
    return;

hup:
    if (my_ppp_pcb->phase != PPP_PHASE_DEAD) {
        ppp_close(my_ppp_pcb, 1);
    }
    return;
}

static void stream_cb(FileHandle *stream) {
    if (!event_queued) {
        event_queued = true;
        if (event_queue->call(callback(ppp_input, stream)) == 0) {
            event_queued = false;
        }
    }
}

err_t ppp_lwip_if_init(struct netif *netif, FileHandle *stream)
{
    if (!prepare_event_queue()) {
        return ERR_MEM;
    }

    if (!my_ppp_pcb) {
        my_ppp_pcb = pppos_create(netif,
                               ppp_output, ppp_link_status, stream);
    }

    if (!my_ppp_pcb) {
        return ERR_IF;
    }

#if LWIP_IPV6_AUTOCONFIG
    /* IPv6 address autoconfiguration not enabled by default */
    netif->ip6_autoconfig_enabled = 1;
#endif /* LWIP_IPV6_AUTOCONFIG */

#if LWIP_IPV4
    ppp_set_usepeerdns(my_ppp_pcb, true);
#endif

    ppp_set_default(my_ppp_pcb);

    stream->sigio(callback(stream_cb, stream));
    stream->set_blocking(false);

    return ppp_connect(my_ppp_pcb, 0);
}

#include "drivers/BufferedSerial.h"
using mbed::BufferedSerial;

static struct netif my_ppp_netif;

nsapi_error_t mbed_ppp_init(FileHandle *stream)
{
    mbed_lwip_init();
    ppp_lwip_if_init(&my_ppp_netif, stream);

    return NSAPI_ERROR_OK;
}

NetworkStack *mbed_ppp_get_stack()
{
    return nsapi_create_stack(&lwip_stack);
}

#endif /* LWIP_PPP_API */

} // namespace mbed
