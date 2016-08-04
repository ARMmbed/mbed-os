/* nsapi.h - The network socket API
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

#ifndef NSAPI_H
#define NSAPI_H


// entry point for nsapi types
#include "nsapi_types.h"

#ifdef __cplusplus

// entry point for C++ api
#include "network-socket/SocketAddress.h"
#include "network-socket/NetworkStack.h"

#include "network-socket/NetworkInterface.h"
#include "network-socket/EthInterface.h"
#include "network-socket/WiFiInterface.h"
#include "network-socket/CellularInterface.h"
#include "network-socket/MeshInterface.h"

#include "network-socket/Socket.h"
#include "network-socket/UDPSocket.h"
#include "network-socket/TCPSocket.h"
#include "network-socket/TCPServer.h"

#endif


#endif
