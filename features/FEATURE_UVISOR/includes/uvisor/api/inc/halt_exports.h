/*
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_HALT_EXPORTS_H__
#define __UVISOR_API_HALT_EXPORTS_H__

#define UVISOR_ERROR_INVALID_BOX_ID             (-2)
#define UVISOR_ERROR_BUFFER_TOO_SMALL           (-3)
#define UVISOR_ERROR_BOX_NAMESPACE_ANONYMOUS    (-4)
#define UVISOR_ERROR_BAD_MAGIC                  (-5)
#define UVISOR_ERROR_BAD_VERSION                (-6)
#define UVISOR_ERROR_OUT_OF_STRUCTURES          (-7)
#define UVISOR_ERROR_INVALID_PARAMETERS         (-8)
#define UVISOR_ERROR_NOT_IMPLEMENTED            (-9)
#define UVISOR_ERROR_TIMEOUT                    (-10)


#define UVISOR_ERROR_CLASS_MASK     (0xFFFF0000UL)
#define UVISOR_ERROR_MASK           (0x0000FFFFUL)

#define UVISOR_ERROR_CLASS_PAGE     (1UL << 16)

typedef enum {
    USER_NOT_ALLOWED = 1,
    DEBUG_BOX_HALT,
} THaltUserError;

typedef enum {
    HALT_NO_ERROR = 0,
    PERMISSION_DENIED = 1,
    SANITY_CHECK_FAILED,
    NOT_IMPLEMENTED,
    NOT_ALLOWED,
    FAULT_MEMMANAGE,
    FAULT_BUS,
    FAULT_USAGE,
    FAULT_HARD,
    FAULT_DEBUG,
    __THALTERROR_MAX /* always keep as the last element of the enum */
} THaltError;

#endif /* __UVISOR_API_HALT_EXPORTS_H__ */
