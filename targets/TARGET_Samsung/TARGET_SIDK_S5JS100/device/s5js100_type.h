/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef S5JS100_TYPE_H
#define S5JS100_TYPE_H

#define getreg8(a)           (*(volatile unsigned char *)(a))
#define putreg8(v, a)        (*(volatile unsigned char *)(a) = (v))
#define getreg32(a)          (*(volatile unsigned int *)(a))
#define putreg32(v, a)       (*(volatile unsigned int *)(a) = (v))
#define modifyreg32(a, m, v)    putreg32((((getreg32(a)) & ~(m)) | ((v) & (m))), (a))

#define is_evt0()       (((getreg32(0x82010000)) & 0xF0) == 0)

#endif  /* S5JS100_TYPE_H */
