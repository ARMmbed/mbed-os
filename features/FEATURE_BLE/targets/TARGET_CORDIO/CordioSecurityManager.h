/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef CORDIO_SECURITY_MANAGER_H_
#define CORDIO_SECURITY_MANAGER_H_

#include <stddef.h>

#include "ble/generic/GenericSecurityManager.h"
#include "ble/generic/GenericSecurityDb.h"
#include "ble/pal/PalSecurityManager.h"
#include "CordioPalSecurityManager.h"
#include "CordioGap.h"
#include "ble/SecurityManager.h"

namespace ble {
namespace vendor {
namespace cordio {

class SecurityManager : public generic::GenericSecurityManager
{
public:
    static SecurityManager &getInstance()
    {
        static generic::MemoryGenericSecurityDb m_db;
        static pal::vendor::cordio::CordioSecurityManager m_pal;
        static SecurityManager m_instance(m_pal, m_db, cordio::Gap::getInstance());
        return m_instance;
    }

public:
    SecurityManager(
        pal::SecurityManager &palImpl,
        generic::GenericSecurityDb &dbImpl,
        Gap &gapImpl
    ) : generic::GenericSecurityManager(palImpl, dbImpl, gapImpl) {
        /* empty */
    }
};

} // namespace cordio
} // namespace vendor
} // namespace ble

#endif /* CORDIO_SECURITY_MANAGER_H_ */
