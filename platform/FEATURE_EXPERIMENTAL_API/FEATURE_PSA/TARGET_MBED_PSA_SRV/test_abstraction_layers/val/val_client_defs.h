/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _VAL_CLIENT_H_
#define _VAL_CLIENT_H_

#include "val.h"
#include "psa/client.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"

#define INVALID_SID                     0x0000FA20

#ifndef CLIENT_TEST_DISPATCHER_SID
#define CLIENT_TEST_DISPATCHER_SID      0x0
#endif

#ifndef SERVER_TEST_DISPATCHER_SID
#define SERVER_TEST_DISPATCHER_SID      0x0
#endif
#endif /* _VAL_CLIENT_H_ */
