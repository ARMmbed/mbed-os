/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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
#include "mbed_trace.h"
#define TRACE_GROUP "PPPNS"
#include "nsapi_ppp.h"
#include "OnboardNetworkStack.h"
#include "netsocket/PPPInterface.h"

#define IPADDR_STRLEN_MAX   46

#if NSAPI_PPP_AVAILABLE

namespace mbed {

void nsapi_ppp_close();

// Just one interface for now
static FileHandle *my_stream;
static OnboardNetworkStack::Interface *my_interface;
static bool blocking_connect = true;
static const char *login;
static const char *pwd;
static PPP *ppp_service = NULL;
static OnboardNetworkStack *stack;

nsapi_error_t nsapi_ppp_set_blocking(bool blocking)
{
    blocking_connect = blocking;
    return NSAPI_ERROR_OK;
}

nsapi_error_t nsapi_ppp_connect(FileHandle *stream, Callback<void(nsapi_event_t, intptr_t)> cb, const char *uname, const char *password, const nsapi_ip_stack_t ip_stack)
{
    if (my_stream) {
        return NSAPI_ERROR_PARAMETER;
    }

    my_stream = stream;
    stream->set_blocking(false);
    login = uname;
    pwd = password;

    nsapi_error_t retcode;

    if (!my_interface) {
        ppp_service = &PPP::get_default_instance();
        stack = &OnboardNetworkStack::get_default_instance();

        ppp_service->set_stream(stream);
        ppp_service->set_ip_stack(ip_stack);
        ppp_service->set_credentials(uname, password);

        retcode = stack->add_ppp_interface(*ppp_service, true, &my_interface);

        if (retcode != NSAPI_ERROR_OK) {
            my_interface = NULL;
            nsapi_ppp_close();
            return retcode;
        }

        if (cb) {
            my_interface->attach(cb);
        }
    }

    retcode = my_interface->bringup(false, NULL, NULL, NULL, ip_stack, blocking_connect);

    if (retcode != NSAPI_ERROR_OK) {
        nsapi_ppp_close();
    }

    return retcode;
}

nsapi_error_t nsapi_ppp_disconnect(FileHandle *stream)
{
    if (my_stream != stream) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    nsapi_error_t retcode = my_interface->bringdown();

    nsapi_ppp_close();

    return retcode;
}

void nsapi_ppp_close()
{
    if (my_interface) {
        stack->remove_ppp_interface(&my_interface);
        my_interface = NULL;
    }

    stack = NULL;

    if (ppp_service) {
        ppp_service->set_stream(NULL);
        ppp_service->set_ip_stack(DEFAULT_STACK);
        ppp_service->set_credentials(NULL, NULL);
        ppp_service = NULL;
    }

    /* Detach callbacks, and put handle back to default blocking mode */
    my_stream->sigio(NULL);
    my_stream->set_blocking(true);
    my_stream = NULL;
}

NetworkStack *nsapi_ppp_get_stack()
{
    return &OnboardNetworkStack::get_default_instance();
}

const char *nsapi_ppp_get_ip_addr(FileHandle *stream)
{
    static char ip_addr[IPADDR_STRLEN_MAX];

    if (stream == my_stream) {
        if (my_interface->get_ip_address(ip_addr, IPADDR_STRLEN_MAX)) {
            return ip_addr;
        }
    }

    return NULL;
}
const char *nsapi_ppp_get_netmask(FileHandle *stream)
{
#if !PPP_IPV4_SUPPORT
    return NULL;
#endif

    static char netmask[IPADDR_STRLEN_MAX];
    if (stream == my_stream) {
        if (my_interface->get_netmask(netmask, IPADDR_STRLEN_MAX)) {
            return netmask;
        }
    }

    return NULL;
}
const char *nsapi_ppp_get_gw_addr(FileHandle *stream)
{
#if !PPP_IPV4_SUPPORT
    return NULL;
#endif

    static char gwaddr[IPADDR_STRLEN_MAX];
    if (stream == my_stream) {
        if (my_interface->get_gateway(gwaddr, IPADDR_STRLEN_MAX)) {
            return gwaddr;
        }
    }

    return NULL;
}

} // namespace mbed

#endif
