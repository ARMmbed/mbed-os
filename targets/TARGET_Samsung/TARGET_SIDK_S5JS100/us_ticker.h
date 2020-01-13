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
#define S5JS100_TIMER_LOAD_VALUE            0x00
#define S5JS100_TIMER_CONTROL               0x04

#define S5JS100_TIMER_CNT_CON_MASK          (1 << 1)
#define S5JS100_TIMER_CNT_CON_SINGLESHOT    (0 << 1)
#define S5JS100_TIMER_CNT_CON_RELOAD        (1 << 1)

#define S5JS100_TIMER_CNT_EN_MASK           (1 << 0)
#define S5JS100_TIMER_CNT_EN                (1 << 0)
#define S5JS100_TIMER_CNT_DIS               (0 << 0)

#define S5JS100_TIMER_LOAD_CON_VALUE        0x08
#define S5JS100_TIMER_INT_STATUS            0x0C
#define S5JS100_TIMER_INT_CLEAR         0x10
#define S5JS100_TIMER_INT_ENABLE            0x14
#define S5JS100_TIMER_UP_DOWN_SEL           0x18
#define S5JS100_TIMER_INT_SEL               0x1C
#define S5JS100_TIMER_FAKE_READ_DISABLE 0x20
#define S5JS100_TIMER_DUMP_ENABLE           0x24
#define S5JS100_TIMER_DUMP_READ         0x28
#define S5JS100_TIMER_ALL_DUMP_READ     0x2C
#define S5JS100_TIMER_CDC_ENABLE            0x30
#define S5JS100_TIMER_COUNT_VALUE           0x34
#define S5JS100_TIMER_DUMP_COUNT_VALUE      0x38
#define S5JS100_TIMER_ALL_DUMP_COUNT_VALUE  0x3C
#define S5JS100_TIMER_CDC_COUNT_VALUE       0x40
#define S5JS100_TIMER_PCLK_CTRL         0x44
#define S5JS100_TIMER_ALL_DUMP_ENABLE       0x800
