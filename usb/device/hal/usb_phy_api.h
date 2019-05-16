
/** \addtogroup hal */
/** @{*/
/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#ifndef MBED_USB_PHY_API_H
#define MBED_USB_PHY_API_H

#include "USBPhy.h"

/** Return a the USBPhy instance for this hardware
 *
 * For details on adding support for a USBPhy see the specification in USBPhy.h.
 *
 * @return A pointer to a USBPhy instance
 * @note Calling this function on platforms without a USBPhy will result in an
 * error
 */
USBPhy *get_usb_phy();

#endif

/** @}*/
