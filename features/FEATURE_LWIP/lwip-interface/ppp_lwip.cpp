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
static nsapi_error_t connect_error_code;

// Just one interface for now
static FileHandle *my_stream;
static ppp_pcb *my_ppp_pcb;
static bool ppp_active = false;
static const char *login;
static const char *pwd;
static sys_sem_t ppp_close_sem;
static Callback<void(nsapi_error_t)> connection_status_cb;

static EventQueue *prepare_event_queue()
{
    if (event_queue) {
        return event_queue;
    }

    // Should be trying to get a global shared event queue here!
    // Shouldn't have to be making a private thread!

    // Only need to queue 2 events. new blows on failure.
    event_queue = new EventQueue(2 * EVENTS_EVENT_SIZE, NULL);
    event_thread = new Thread(osPriorityNormal, PPP_THREAD_STACK_SIZE);

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

//    /tr_debug("> %ld bytes of data written\n", (long) written);

    return written;
}

static void ppp_link_status(ppp_pcb *pcb, int err_code, void *ctx)
{
    nsapi_error_t mapped_err_code = NSAPI_ERROR_NO_CONNECTION;

    switch(err_code) {
        case PPPERR_NONE:
            mapped_err_code = NSAPI_ERROR_OK;
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
        /* suppress generating a callback event for connection up
         * Because connect() call is blocking, why wait for a callback */
        return;
    }

    /* If some error happened, we need to properly shutdown the PPP interface  */
    if (ppp_active) {
        ppp_active = false;
        connect_error_code = mapped_err_code;
        sys_sem_signal(&ppp_close_sem);
    }

    /* Alright, PPP interface is down, we need to notify upper layer */
    if (connection_status_cb) {
        connection_status_cb(mapped_err_code);
    }
}

static void handle_modem_hangup()
{
    if (my_ppp_pcb->phase != PPP_PHASE_DEAD) {
        ppp_close(my_ppp_pcb, 1);
    }
}

#if !PPP_INPROC_IRQ_SAFE
#error "PPP_INPROC_IRQ_SAFE must be enabled"
#endif
static void ppp_input()
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
        handle_modem_hangup();
        return;
    }

    // Infinite loop, but we assume that we can read faster than the
    // serial, so we will fairly rapidly hit -EAGAIN.
    for (;;) {
        u8_t buffer[16];
        ssize_t len = my_stream->read(buffer, sizeof buffer);
        if (len == -EAGAIN) {
            break;
        } else if (len <= 0) {
            handle_modem_hangup();
            return;
        }
        pppos_input(my_ppp_pcb, buffer, len);
    }
    return;
}

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
#if PPP_AUTH_SUPPORT
   ppp_set_auth(my_ppp_pcb, PPPAUTHTYPE_ANY, login, pwd);
#endif //PPP_AUTH_SUPPORT
   ppp_active = true;
   err_t ret = ppp_connect(my_ppp_pcb, 0);
   // lwIP's ppp.txt says input must not be called until after connect
   if (ret == ERR_OK) {
       my_stream->sigio(callback(stream_cb));
   } else {
       ppp_active = false;
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

nsapi_error_t nsapi_ppp_error_code()
{
    return connect_error_code;
}

nsapi_error_t nsapi_ppp_connect(FileHandle *stream, Callback<void(nsapi_error_t)> cb, const char *uname, const char *password)
{
    if (my_stream) {
        return NSAPI_ERROR_PARAMETER;
    }
    my_stream = stream;
    stream->set_blocking(false);
    connection_status_cb = cb;
    login = uname;
    pwd = password;

    // mustn't start calling input until after connect -
    // attach deferred until ppp_lwip_connect, called from mbed_lwip_bringup
    nsapi_error_t retcode = mbed_lwip_bringup_2(false, true, NULL, NULL, NULL);

    if (retcode != NSAPI_ERROR_OK && connect_error_code != NSAPI_ERROR_OK) {
        return connect_error_code;
    } else {
        return retcode;
    }
}

nsapi_error_t nsapi_ppp_disconnect(FileHandle *stream)
{
    return mbed_lwip_bringdown_2(true);
}

NetworkStack *nsapi_ppp_get_stack()
{
    return nsapi_create_stack(&lwip_stack);
}

const char *nsapi_ppp_get_ip_addr(FileHandle *stream)
{
    static char ip_addr[IPADDR_STRLEN_MAX];

    if (stream == my_stream) {

        if (mbed_lwip_get_ip_address(ip_addr, IPADDR_STRLEN_MAX)) {
            return ip_addr;
        }
    }

    return NULL;
}
const char *nsapi_ppp_get_netmask(FileHandle *stream)
{
#if LWIP_IPV6
    return NULL;
#endif

    static char netmask[IPADDR_STRLEN_MAX];
    if (stream == my_stream) {
        if (mbed_lwip_get_netmask(netmask, IPADDR_STRLEN_MAX)) {
            return netmask;
        }
    }

    return NULL;
}
const char *nsapi_ppp_get_gw_addr(FileHandle *stream)
{
#if LWIP_IPV6
    return NULL;
#endif

    static char gwaddr[IPADDR_STRLEN_MAX];
    if (stream == my_stream) {
        if (mbed_lwip_get_gateway(gwaddr, IPADDR_STRLEN_MAX)) {
            return gwaddr;
        }
    }

    return NULL;
}

#endif /* LWIP_PPP_API */

} // namespace mbed
