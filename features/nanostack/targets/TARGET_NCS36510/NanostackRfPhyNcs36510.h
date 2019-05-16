/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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

#ifndef NANOSTACK_PHY_NCS36510_H_
#define NANOSTACK_PHY_NCS36510_H_

#include "NanostackRfPhy.h"

class NanostackRfPhyNcs36510 : public NanostackRfPhy {
public:
    NanostackRfPhyNcs36510();
    ~NanostackRfPhyNcs36510();
    int8_t rf_register();
    void rf_unregister();
    void get_mac_address(uint8_t *mac);
    void set_mac_address(uint8_t *mac);
};

#endif /* NANOSTACK_PHY_NCS36510_H_ */
