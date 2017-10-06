/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "mbed_assert.h"
#include "PinNames.h"
#include "nu_modutil.h"

const struct nu_modinit_s *get_modinit(uint32_t modname, const struct nu_modinit_s *modprop_tab)
{
    MBED_ASSERT(modprop_tab != NULL);
    const struct nu_modinit_s *modprop_ind = modprop_tab;
    while (modprop_ind->modname != NC) {
        if ((int) modname == modprop_ind->modname) {
            return modprop_ind;
        }
        else {
            modprop_ind ++;
        }
    }
    
    return NULL;
}
