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

#ifndef __S1SBP6A_TYPE_H
#define __S1SBP6A_TYPE_H

#define _Wait(tick)     {                                   \
                            volatile uint32_t cnt = tick;   \
                            while(cnt--);                   \
                        }

#define UNUSED(a)            ((void)(a))

#define getreg8(a)           (*(volatile uint8_t *)(a))
#define putreg8(a, v)        (*(volatile uint8_t *)(a) = (v))
#define getreg32(a)          (*(volatile uint32_t *)(a))
#define putreg32(a, v)       (*(volatile uint32_t *)(a) = (v))
#define modifyreg32(a, m, v)    putreg32((a),(((getreg32(a)) & ~(m)) | ((v) & (m))))

#endif  /* __S1SBP6A_TYPE_H */
