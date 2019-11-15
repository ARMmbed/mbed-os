/*
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
#include "netsocket/NetworkInterface.h"
#include "WiFiInterface.h"
#include "CellularInterface.h"
#include "MeshInterface.h"

MBED_WEAK WiFiInterface *WiFiInterface::get_default_instance()
{
    return NULL;
}

MBED_WEAK MeshInterface *MeshInterface::get_default_instance()
{
    return NULL;
}

MBED_WEAK CellularInterface *CellularInterface::get_default_instance()
{
    return NULL;
}

MBED_WEAK WiFiInterface *WiFiInterface::get_target_default_instance()
{
    return NULL;
}

MBED_WEAK NetworkInterface *NetworkInterface::get_default_instance()
{
    return NULL;
}

void NetworkInterface::set_default_parameters()
{
}

void WiFiInterface::set_default_parameters()
{
}

void CellularInterface::set_default_parameters()
{
}

MBED_WEAK NetworkInterface *NetworkInterface::get_target_default_instance()
{
    return NULL;
}
