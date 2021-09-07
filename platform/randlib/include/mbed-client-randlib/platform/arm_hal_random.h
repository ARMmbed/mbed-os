/*
 * Copyright (c) 2014-2016, Pelion and affiliates.
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

#ifndef ARM_HAL_RANDOM_H_
#define ARM_HAL_RANDOM_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \brief This function performs Random number driver init.
 */
extern void arm_random_module_init(void);
/**
 * \brief Get random library seed value.
 *
 * This function should return as random a value as possible, using
 * hardware sources. Repeated calls should return different values if
 * at all possible.
 */
extern uint32_t arm_random_seed_get(void);
#ifdef __cplusplus
}
#endif
#endif /* ARM_HAL_RANDOM_H_ */
