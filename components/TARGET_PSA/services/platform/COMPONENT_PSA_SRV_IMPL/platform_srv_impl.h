/* Copyright (c) 2019-2020 Arm Limited
 *
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

#ifndef __PLATFROM_SRV_IMPL_H__
#define __PLATFROM_SRV_IMPL_H__

#include "psa/client.h"
#include "psa/lifecycle.h"
#include "mbed_toolchain.h"
#include "tfm_platform_api.h"

psa_status_t psa_platfrom_lifecycle_get_impl(uint32_t *lc_state);
psa_status_t psa_platfrom_lifecycle_change_request_impl(uint32_t lc_state);
MBED_NORETURN void mbed_psa_system_reset_impl(void);

/*!
 * \brief Performs a platform-specific service
 *
 * \param[in]  request      Request identifier (valid values vary
 *                          based on the platform)
 * \param[in]  in_vec       Input buffer to the requested service (or NULL)
 * \param[out] out_vec      Output buffer to the requested service (or NULL)
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec *in_vec,
                                               psa_outvec *out_vec);
#endif // __PLATFROM_SRV_IMPL_H__
