/*
 * Copyright (c) 2014-2015, 2017, Pelion and affiliates.
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

#ifndef PANA_NVM_H_
#define PANA_NVM_H_

#include "net_nvm_api.h"
#ifdef PANA_SERVER_API
extern void pana_session_nvm_udate(sec_suite_t *suite, pana_nvm_update_process_t update_event);
#else
#define pana_session_nvm_udate(suite, update_event) ((void)0)
#endif
#endif /* PANA_NVM_H_ */
