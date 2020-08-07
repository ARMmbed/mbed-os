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

#ifndef NANOSTACK_MACPHY_H_
#define NANOSTACK_MACPHY_H_

#include "NanostackPhy.h"

/** MAC PHY driver class for Nanostack */
class NanostackMACPhy : public NanostackPhy {
public:
    /** Read the mac address of this physical interface
     *
     *  Note - some devices do not have a mac address
     *         in hardware.
     *
     *  @param mac      mac address
     */
    virtual void get_mac_address(uint8_t *mac) = 0;

    /** Set the mac address of this physical interface
     *
     *  @param mac      mac address
     */
    virtual void set_mac_address(uint8_t *mac) = 0;

    /** Provide access to the NanostackMACPhy
     *
     *  @return          NanostackMACPhy
     */
    NanostackMACPhy *nanostack_mac_phy() final
    {
        return this;
    }
};

#endif /* NANOSTACK_MACPHY_H_ */
