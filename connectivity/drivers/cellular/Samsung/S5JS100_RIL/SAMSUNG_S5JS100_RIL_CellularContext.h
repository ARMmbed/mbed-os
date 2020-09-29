/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef SAMSUNG_S5JS100_RIL_CELLULARCONTEXT_H_
#define SAMSUNG_S5JS100_RIL_CELLULARCONTEXT_H_

#include "RIL_CellularContext.h"

#if defined(TARGET_S5JS100)
#include "SAMSUNG_S5JS100_RIL_ControlPlane_netif.h"
#else
#include "ControlPlane_netif.h"
#endif

namespace mbed {

class SAMSUNG_S5JS100_RIL_CellularContext: public RIL_CellularContext {
public:
    SAMSUNG_S5JS100_RIL_CellularContext(RIL_CellularDevice &device, const char *apn = 0, bool cp_req = false,
                                        bool nonip_req = false);
    virtual ~SAMSUNG_S5JS100_RIL_CellularContext();

    virtual ControlPlane_netif *get_cp_netif();

protected: // from RIL_CellularContext
    virtual L3IP *get_L3IP_driver();
    virtual void delete_L3IP_driver();

#if defined(TARGET_S5JS100)
    SAMSUNG_S5JS100_RIL_ControlPlane_netif *_cp_netif;
#else
    ControlPlane_netif *_cp_netif;
#endif
};

} /* namespace mbed */

#endif /* SAMSUNG_S5JS100_RIL_CELLULARCONTEXT_H_ */
