/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef NWK_NVM_H_
#define NWK_NVM_H_
#include "net_nvm_api.h"


/**
 * \brief Function to get setup from NVM
 *
 * \param parameters Updated wpan parameters
 *
 * \return Pointer for setup allways never NULL!
 */
typedef wpan_nvm_params_t  *wpan_nvm_params_get(struct nwk_wpan_nvm_api *api, uint16_t pan_id);

/**
 * \brief Function to update dynamic values caaled at every second and ifdown process
 *
 * \param if_down_call true for ifdown call and false
 */
typedef void wpan_nvm_params_update(struct nwk_wpan_nvm_api *api, bool if_down_call);

typedef struct nwk_wpan_nvm_api {
    wpan_nvm_params_t params;
    struct protocol_interface_info_entry *interface;
    wpan_nvm_params_get *nvm_params_get_cb;
    wpan_nvm_params_update *nvm_params_update_cb;
    wpan_params_updated *NVM_PUSH;
    wpan_params_get *NVM_GET;
} nwk_wpan_nvm_api_t;

#endif /* NWK_NVM_H_ */
