/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#ifndef SEC_PROT_CFG_H_
#define SEC_PROT_CFG_H_

/* Security protocol configuration settings */

typedef struct sec_prot_cfg_s {
    trickle_params_t sec_prot_trickle_params;
    uint16_t sec_prot_retry_timeout;
    uint16_t sec_max_ongoing_authentication;
} sec_prot_cfg_t;

#endif /* SEC_PROT_CONF_H_ */
