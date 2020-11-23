/***************************************************************************//**
 * @file PortNames.h
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
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
 *
 ******************************************************************************/
#ifndef MBED_PORTNAMES_H
#define MBED_PORTNAMES_H

#include "pic32cx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PortA = 0, /**< Port A */
    PortB = 1, /**< Port B */
    PortC = 2, /**< Port C */
    PortD = 3, /**< Port D */
} PortName;

#ifdef __cplusplus
}
#endif
#endif
