/**
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef LORAWANBASE_H_
#define LORAWANBASE_H_

/**
 * This class is deprecated and will be removed altogether after expiration of
 * deprecation notice.
 */
#include "LoRaWANInterface.h"

MBED_DEPRECATED_SINCE("mbed-os-5.12", "Migrated to LoRaWANInterface")
typedef LoRaWANInterface LoRaWANBase;

#endif /* LORAWANBASE_H_ */
