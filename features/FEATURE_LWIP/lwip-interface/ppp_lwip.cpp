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
#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LPPP"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)
#include "rtos/Thread.h"
#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/pbuf.h"
extern "C" { // "pppos.h" is missing extern C
#include "netif/ppp/pppapi.h"
}

#include "nsapi_ppp.h"
#include "ppp_lwip.h"
#include "lwip_stack.h"

namespace mbed {

using rtos::Thread;
using events::EventQueue;

#if LWIP_PPP_API

static EventQueue *event_queue;
static Thread *event_thread;
static volatile bool event_queued;

// Just one interface for now
static FileHandle *my_stream;
static ppp_pcb *my_ppp_pcb;
static bool ppp_link_up;
static sys_sem_t ppp_close_sem;
static void (*notify_ppp_link_status)(int) = 0;

static EventQueue *prepare_event_queue()
{
    if (event_queue) {
        return event_queue;
    }

    // Should be trying to get a global shared event queue here!
    // Shouldn't have to be making a private thread!

    // Only need to queue 1 event. new blows on failure.
    event_queue = new EventQueue(2 * EVENTS_EVENT_SIZE, NULL);
    event_thread = new Thread(osPriorityNormal, 900);

    if (event_thread->start(callback(event_queue, &EventQueue::dispatch_forever)) != osOK) {
        delete event_thread;
        delete event_queue;
        return NULL;
    }

    return event_queue;
}

static u32_t ppp_output(ppp_pcb *pcb, u8_t *data, u32_t len, void *ctx)
{
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

    tr_debug("> %ld\n", (long) written);

    return written;
}

static void ppp_link_status(ppp_pcb *pcb, int err_code, void *ctx)
{
    switch(err_code) {
        case PPPERR_NONE: {
            tr_info("status_cb: Connected");
#if PPP_IPV4_SUPPORT
            tr_debug("   our_ipaddr  = %s", ipaddr_ntoa(&ppp_netif(pcb)->ip_addr));
            tr_debug("   his_ipaddr  = %s", ipaddr_ntoa(&ppp_netif(pcb)->gw));
            tr_debug("   netmask     = %s", ipaddr_ntoa(&ppp_netif(pcb)->netmask));
#if LWIP_DNS
            const ip_addr_t *ns;
            ns = dns_getserver(0);
            if (ns) {
                tr_debug("   dns1        = %s", ipaddr_ntoa(ns));
            }
            ns = dns_getserver(1);
            if (ns) {
                tr_debug("   dns2        = %s", ipaddr_ntoa(ns));
            }
#endif /* LWIP_DNS */
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
            tr_debug("   our6_ipaddr = %s", ip6addr_ntoa(netif_ip6_addr(ppp_netif(pcb), 0)));
#endif /* PPP_IPV6_SUPPORT */
            break;
        }
        case PPPERR_PARAM: {
            tr_info("status_cb: Invalid parameter");
            break;
        }
        case PPPERR_OPEN: {
            tr_info("status_cb: Unable to open PPP session");
            break;
        }
        case PPPERR_DEVICE: {
            tr_info("status_cb: Invalid I/O device for PPP");
            break;
        }
        case PPPERR_ALLOC: {
            tr_info("status_cb: Unable to allocate resources");
            break;
        }
        case PPPERR_USER: {
            tr_info("status_cb: User interrupt");
            break;
        }
        case PPPERR_CONNECT: {
            tr_info("status_cb: Connection lost");
            break;
        }
        case PPPERR_AUTHFAIL: {
            tr_info("status_cb: Failed authentication challenge");
            break;
        }
        case PPPERR_PROTOCOL: {
            tr_info("status_cb: Failed to meet protocol");
            break;
        }
        case PPPERR_PEERDEAD: {
            tr_info("status_cb: Connection timeout");
            break;
        }
        case PPPERR_IDLETIMEOUT: {
            tr_info("status_cb: Idle Timeout");
            break;
        }
        case PPPERR_CONNECTTIME: {
            tr_info("status_cb: Max connect time reached");
            break;
        }
        case PPPERR_LOOPBACK: {
            tr_info("status_cb: Loopback detected");
            break;
        }
        default: {
            tr_info("status_cb: Unknown error code %d", err_code);
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

/*static void ppp_carrier_lost()
{
    if (my_stream) {
        ppp_close(my_ppp_pcb, 1);
    }
}*/

static void stream_cb() {
    if (my_stream && !event_queued) {
        event_queued = true;
        if (event_queue->call(callback(ppp_input)) == 0) {
            event_queued = false;
        }
    }
}

extern "C" err_t ppp_lwip_connect()
{
   err_t ret = ppp_connect(my_ppp_pcb, 0);
   // lwIP's ppp.txt says input must not be called until after connect
   if (ret == ERR_OK) {
       my_stream->sigio(callback(stream_cb));
   }
   return ret;
}

extern "C" err_t ppp_lwip_disconnect()
{
    err_t ret = ppp_close(my_ppp_pcb, 0);
    if (ret != ERR_OK) {
        return ret;
    }

    /* close call made, now let's catch the response in the status callback */
    sys_arch_sem_wait(&ppp_close_sem, 0);

    /* Detach callbacks, and put handle back to default blocking mode */
    my_stream->sigio(Callback<void()>());
    my_stream->set_blocking(true);
    my_stream = NULL;

    return ret;
}

extern "C" nsapi_error_t ppp_lwip_if_init(struct netif *netif)
{
    if (!prepare_event_queue()) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    if (!my_ppp_pcb) {
        my_ppp_pcb = pppos_create(netif,
                               ppp_output, ppp_link_status, NULL);
        if (!my_ppp_pcb) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        sys_sem_new(&ppp_close_sem, 0);
    }

#if LWIP_IPV4
    ppp_set_usepeerdns(my_ppp_pcb, true);
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t nsapi_ppp_connect(FileHandle *stream, void (*ppp_link_status_cb)(int))
{
    if (my_stream) {
        return NSAPI_ERROR_PARAMETER;
    }
    my_stream = stream;
    stream->set_blocking(false);
    notify_ppp_link_status = ppp_link_status_cb;
    // mustn't start calling input until after connect -
    // attach deferred until ppp_lwip_connect, called from mbed_lwip_bringup
    return mbed_lwip_bringup(false, true, NULL, NULL, NULL);
}


nsapi_error_t nsapi_ppp_disconnect(FileHandle *stream)
{
    return mbed_lwip_bringdown(true);
}

/*void nsapi_ppp_carrier_lost(FileHandle *stream)
{
    if (stream == my_stream) {
        event_queue->call(callback(ppp_carrier_lost));
    }
}*/

NetworkStack *nsapi_ppp_get_stack()
{
    return nsapi_create_stack(&lwip_stack);
}

#endif /* LWIP_PPP_API */

} // namespace mbed
