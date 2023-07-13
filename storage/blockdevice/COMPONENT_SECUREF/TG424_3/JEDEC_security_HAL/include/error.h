/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

typedef int32_t jedec_error_t;

#define JEDEC_ERROR_NONE                          0
#define JEDEC_ERROR_SESSION                       -0x0001
#define JEDEC_ERROR_INV_ARGS                      -0x0002
#define JEDEC_ERROR_AUTH_FAIL                     -0x0003
#define JEDEC_ERROR_COMM_FAIL                     -0x0004
#define JEDEC_ERROR_INVALID_ADDR                  -0x0005
#define JEDEC_ERROR_MAX_SESSIONS_REACHED          -0x0006
#define JEDEC_ERROR_CMD_NOT_SUPPORTED_ON_DEVICE   -0x000a
#define JEDEC_ERROR_INIT_FAIL                     -0x000b
#define JEDEC_ERROR_NOT_PERMITTED                 -0x000c
#define JEDEC_ERROR_DEVICE_BUSY                   -0x0011
#define JEDEC_ERROR_CMD_PACKET                    -0x0012
#define JEDEC_ERROR_INSUFFICIENT_MEMORY           -0x0013
#define JEDEC_ERROR_GENERIC                       -0x0014

#ifdef __cplusplus
}
#endif

#endif /* _ERROR_H_ */
