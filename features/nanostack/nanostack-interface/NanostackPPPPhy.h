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

#ifndef NANOSTACK_PPP_PHY_H_
#define NANOSTACK_PPP_PHY_H_

#include "NanostackPhy.h"
/** PPP PHY driver class for Nanostack */
class NanostackPPPPhy : public NanostackPhy {
public:
    /** Link state callback function prototype
     *
     *  @param up       link up
     */
    typedef mbed::Callback<void (bool up)> link_state_change_cb_t;

    /** Set link state callback of this physical interface
     *
     *  @param cb       callback
     */
    virtual void set_link_state_change_cb(link_state_change_cb_t cb) = 0;

    /** Read the iid64 address of this physical interface
     *
     *  @param iid64    iid64 address
     */
    virtual void get_iid64(uint8_t *iid64) = 0;

    /** Read the mtu of this physical interface
     *
     * @return          mtu
     */
    virtual uint16_t get_mtu() = 0;

    /** Provide access to the NanostackMACPhy
     *
     * @return          NanostackPPPPhy
     */
    virtual NanostackPPPPhy *nanostack_ppp_phy()
    {
        return this;
    }
};

#endif /* NANOSTACK_PPP_PHY_H_ */
