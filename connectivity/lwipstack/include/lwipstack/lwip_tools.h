/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef LWIPTOOLS_H_
#define LWIPTOOLS_H_

bool convert_lwip_addr_to_mbed(nsapi_addr_t *out, const ip_addr_t *in);
bool convert_mbed_addr_to_lwip(ip_addr_t *out, const nsapi_addr_t *in);

#endif /* LWIPTOOLS_H_ */
