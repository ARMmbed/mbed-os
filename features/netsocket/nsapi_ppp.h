/** \addtogroup netsocket */
/** @{*/
/* nsapi_ppp.h
 * Modified work Copyright (c) 2017 ARM Limited
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
#ifndef NSAPI_PPP_H_
#define NSAPI_PPP_H_

#include "FileHandle.h"
#include "NetworkStack.h"

namespace mbed {

NetworkStack *nsapi_ppp_get_stack();
nsapi_error_t nsapi_ppp_connect(FileHandle *stream, void(*link_down_cb)(int)=0, const char *uname=0, const char *pwd=0);
nsapi_error_t nsapi_ppp_disconnect(FileHandle *stream);
char *nsapi_ppp_get_ip_addr(char *ip_addr, nsapi_size_t buflen);
char *nsapi_ppp_get_netmask(char *netmask, nsapi_size_t buflen);
char *nsapi_ppp_get_gw_addr(char *gateway_addr, nsapi_size_t buflen);
} //namespace mbed

#endif /* NSAPI_PPP_H_ */
