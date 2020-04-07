/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

#include "SclSTAInterface.h"

/** @file
*  Provides function definition to override get_target_default_intance of WiFiInterface and NetworkInterface classes
*/

/**
 * Returns the WiFiInterface Object
 * This function can be called using WiFiInterface or NetworkInterface objects
 *
 * @return         pointer to WiFiInterface object.
 */

WiFiInterface *WiFiInterface::get_target_default_instance()
{
    static SclSTAInterface wifi;
    return &wifi;
}
