
/** \addtogroup netsocket */
/** @{*/
/* UDPSocket
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
 */

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "netsocket/InternetSocket.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/NetworkInterface.h"
#include "netsocket/InternetDatagramSocket.h"
#include "rtos/EventFlags.h"
#include "ICMPSocket.h"


/** UDP socket implementation.
 */
class UDPSocket : public InternetDatagramSocket {
public:
    /** Create an uninitialized socket.
     *
     *  @note Must call open to initialize the socket on a network stack.
     */
    UDPSocket();

    /** Create and open a socket on the network stack of the given
     *  network interface.
     *
     *  @tparam S    Type of the Network stack.
     *  @param stack Network stack as target for socket.
     *  @deprecated since mbed-os-5.11
     */
    template <typename S>
    MBED_DEPRECATED_SINCE("mbed-os-5.11",
                          "The UDPSocket(S *stack) constructor is deprecated"
                          "It discards the open() call return value."
                          "Use another constructor and call open() explicitly, instead.")
    UDPSocket(S *stack)
    {
        open(stack);
    }

#if !defined(DOXYGEN_ONLY)

protected:
    virtual nsapi_protocol_t get_proto();

#endif //!defined(DOXYGEN_ONLY)

};


#endif

/** @}*/
