/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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

#ifndef NANOSTACK_PHY_H_
#define NANOSTACK_PHY_H_

class NanostackMACPhy;
class NanostackPPPPhy;

/** PHY driver class for Nanostack */
class NanostackPhy {
public:

    /** Register this physical interface with Nanostack
     *
     *  @return         Device driver ID or a negative error
     *                  code on failure
     */
    virtual int8_t phy_register() = 0;

    /** Return pointer to a NanostackMACPhy.
     *
     *  @return         Pointer to requested phy type or NULL if this
     *                  class doesn't implement the phy.
     */
    virtual NanostackMACPhy *nanostack_mac_phy()
    {
        return nullptr;
    }

    /** Return pointer to a NanostackPPPPhy.
     *
     *  @return         Pointer to requested phy type or NULL if this
     *                  class doesn't implement the phy.
     */
    virtual NanostackPPPPhy *nanostack_ppp_phy()
    {
        return nullptr;
    }

protected:
    NanostackPhy() = default;
    virtual ~NanostackPhy() = default;
};

#endif /* NANOSTACK_INTERFACE_H_ */
