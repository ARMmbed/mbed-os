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

#ifndef NU_MISC_UTIL_H
#define NU_MISC_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define NU_MAX(a,b) ((a)>(b)?(a):(b))
#define NU_MIN(a,b) ((a)<(b)?(a):(b))
#define NU_CLAMP(x, min, max)   NU_MIN(NU_MAX((x), (min)), (max))
#define NU_ALIGN_DOWN(X, ALIGN)     ((X) & ~((ALIGN) - 1))
#define NU_ALIGN_UP(X, ALIGN)       (((X) + (ALIGN) - 1) & ~((ALIGN) - 1))

void nu_nop(uint32_t n);
    
    
#ifdef __cplusplus
}
#endif

#endif
