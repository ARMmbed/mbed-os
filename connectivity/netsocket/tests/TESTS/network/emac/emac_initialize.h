/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EMAC_INITIALIZE_H
#define EMAC_INITIALIZE_H

unsigned char *emac_if_get_hw_addr(void);
bool emac_if_init(EMAC *emac);
EMAC *emac_if_get(void);
EmacTestMemoryManager *emac_m_mngr_get(void);

#endif /* EMAC_INITIALIZE_H */
