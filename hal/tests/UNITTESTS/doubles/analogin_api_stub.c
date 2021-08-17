/*
 * Copyright (c) 2021 Arm Limited and affiliates.
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
#include "hal/analogin_api.h"
#include <stddef.h>

#if DEVICE_ANALOGIN

void analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
{
}

void analogin_init(analogin_t *obj, PinName pin)
{
}

void analogin_free(analogin_t *obj)
{
}

float analogin_read(analogin_t *obj)
{
    return 0;
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    return 0;
}

const PinMap *analogin_pinmap(void)
{
    return NULL;
}

#endif // DEVICE_ANALOGIN
