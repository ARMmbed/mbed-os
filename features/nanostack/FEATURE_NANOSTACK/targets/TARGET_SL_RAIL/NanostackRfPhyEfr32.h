/*
 * Copyright (c) 2016 Silicon Laboratories, Inc. http://www.silabs.com
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NANOSTACK_PHY_EFR32_H_
#define NANOSTACK_PHY_EFR32_H_

#include "mbed.h"
#include "NanostackRfPhy.h"

class NanostackRfPhyEfr32 : public NanostackRfPhy {
public:
    NanostackRfPhyEfr32();
    ~NanostackRfPhyEfr32();
    int8_t rf_register();
    void rf_unregister();
    void get_mac_address(uint8_t *mac);
    void set_mac_address(uint8_t *mac);
    uint32_t get_driver_version();
};

#endif /* NANOSTACK_PHY_EFR32_H_ */
