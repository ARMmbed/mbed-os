/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef AT_CELLULARDEVICE_STUB_H_
#define AT_CELLULARDEVICE_STUB_H_

#include "AT_CellularDevice.h"
#include "ATHandler.h"

namespace AT_CellularDevice_stub {
extern int failure_count;
extern nsapi_error_t nsapi_error_value;
extern int init_module_failure_count;
extern int set_pin_failure_count;
extern int get_sim_failure_count;
extern bool pin_needed;
extern bool supported_bool;
extern int max_sock_value;
}


#endif /* AT_CELLULARDEVICE_STUB_H_ */
