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
 * just an EMACInterface. But we can be the default EthInterface - step up
 * if the target has a default EMAC.
 */
#if DEVICE_EMAC
MBED_WEAK EthInterface *EthInterface::get_target_default_instance()
{
    static EthernetInterface ethernet;
    return &ethernet;
}
#else
MBED_WEAK EthInterface *EthInterface::get_target_default_instance()
{
    return NULL;
}
#endif
