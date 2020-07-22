/*
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

/* @file EthInterface.h Common interface between Ethernet hardware */
/** \addtogroup netinterface */
/** @{*/

#ifndef ETH_INTERFACE_H
#define ETH_INTERFACE_H

#include "netsocket/NetworkInterface.h"


/** Common interface between Ethernet hardware.
 */
class EthInterface : public virtual NetworkInterface {
public:

    /** @copydoc NetworkInterface::ethInterface
     */
    EthInterface *ethInterface() final
    {
        return this;
    }

    /** Get the default Ethernet interface.
     *
     * This is provided as a weak method so applications can override it.
     * Default behavior is to get the target's default interface, if
     * any.
     *
     * @return Pointer to interface, if one exists.
     */
    static EthInterface *get_default_instance();

#if !defined(DOXYGEN_ONLY)
protected:

    /** Get the target's default Ethernet interface.
     *
     * This is provided as a weak method so targets can override it. The
     * default implementation invokes EthernetInterface with the
     * default EMAC and default network stack, if DEVICE_EMAC is set.
     *
     * @return Pointer to interface, if one exists.
     */
    static EthInterface *get_target_default_instance();
#endif //!defined(DOXYGEN_ONLY)
};


#endif

/** @}*/
