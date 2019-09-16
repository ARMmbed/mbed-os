/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#ifndef NANOSTACK_RF_PHY_H_
#define NANOSTACK_RF_PHY_H_

#include "NanostackMACPhy.h"

/** Radio PHY driver class for Nanostack */
class NanostackRfPhy : public NanostackMACPhy {
public:

    /** Return the default on-board NanostackRfPhy
     *
     * Returns the default on-board NanostackRfPhy - this will be target-specific, and
     * may not be available on all targets.
     */
    static NanostackRfPhy &get_default_instance();

    /** Register this physical interface with Nanostack
     *
     *  @return         Device driver ID or a negative error
     *                  code on failure
     */
    virtual int8_t rf_register() = 0;

    /** Unregister this physical interface
     *
     */
    virtual void rf_unregister() = 0;

    /** Register this physical interface with Nanostack
     *
     *  @return         Device driver ID or a negative error
     *                  code on failure
     */
    virtual int8_t phy_register()
    {
        return rf_register();
    }

    /** Unregister this physical interface
     *
     */
    virtual void unregister()
    {
        rf_unregister();
    }
};

#endif /* NANOSTACK_RF_PHY_H_ */
