/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       OS Event Observer
 *
 * -----------------------------------------------------------------------------
 */
#include "rt_OsEventObserver.h"

/*
 *  _____ _____  ____  __ _____
 * |  ___|_ _\ \/ /  \/  | ____|
 * | |_   | | \  /| |\/| |  _|
 * |  _|  | | /  \| |  | | |___
 * |_|   |___/_/\_\_|  |_|_____|
 *
 * FIXME:
 * The osEventObs variable must be in protected memory. If not every box
 * and box 0 can modify osEventObs to point to any handler to run code
 * privileged. This issue is tracked at
 * <https://github.com/ARMmbed/uvisor/issues/235>.
 */
const OsEventObserver *osEventObs;

void osRegisterForOsEvents(const OsEventObserver *observer)
{
    static uint8_t has_been_called = 0;
    if (has_been_called) {
        return;
    }
    has_been_called = 1;

    osEventObs = observer;
}
