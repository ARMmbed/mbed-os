/*
 * Copyright (c) 2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/mbed_toolchain.h"


MBED_DEPRECATED_SINCE("5.10", "FEATURE_NANOSTACK is deprecated. You do not need to enable it anymore in mbed_app.json")
static void feature_nanostack(void) { }
void dummy_feature_nanostack_is_deprecated(void)
{
    feature_nanostack();
}
