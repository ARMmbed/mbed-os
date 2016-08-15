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

#ifndef NU_MODULE_UTIL_H
#define NU_MODULE_UTIL_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nu_modinit_s {
    int modname;
    uint32_t clkidx;
    uint32_t clksrc;
    uint32_t clkdiv;
    uint32_t rsetidx;
    
    IRQn_Type irq_n;
    //int irq_n;
    
    void *var;
};

const struct nu_modinit_s *get_modinit(uint32_t modname, const struct nu_modinit_s *modprop_tab);

#ifdef __cplusplus
}
#endif

#endif
