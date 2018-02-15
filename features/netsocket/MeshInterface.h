
/** \addtogroup netsocket */
/** @{*/
/* MeshInterface
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

#ifndef MESH_INTERFACE_H
#define MESH_INTERFACE_H

#include "netsocket/NetworkInterface.h"


/** MeshInterface class
 *
 *  Common interface that is shared between mesh hardware
 */
class MeshInterface : public virtual NetworkInterface
{
public:

    virtual MeshInterface *meshInterface() {
        return this;
    }

    /** Get the default Mesh interface.
     *
     * This is provided as a weak method so applications can override.
     * Default behaviour is to get the target's default interface, if
     * any.
     *
     * @return pointer to interface, if any
     */
    static MeshInterface *get_default_instance();

protected:

    /** Get the target's default Mesh interface.
     *
     * This is provided as a weak method so targets can override. The
     * default implementation will invoke LoWPANNDInterface or ThreadInterface
     * with the default NanostackRfPhy.
     *
     * @return pointer to interface, if any
     */
    static MeshInterface *get_target_default_instance();
};


#endif

/** @}*/
