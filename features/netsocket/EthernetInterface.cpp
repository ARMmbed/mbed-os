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
 */

#include "EthernetInterface.h"

/* No actual interface implementation here, as EthernetInterface is
 * just an EMACInterface. But we can be the default NetworkInterface - step up
 * if the target has a default EMAC.
 */

#define ETHERNET 0x2345
#if DEVICE_EMAC && MBED_CONF_NSAPI_DEFAULT_INTERFACE_TYPE == ETHERNET
MBED_WEAK NetworkInterface &NetworkInterface::get_default_instance()
{
    static EthernetInterface ethernet;
    return ethernet;
}
#endif
