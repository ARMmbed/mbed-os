/* Copyright (c) 2016 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef TARGET_STM32F303ZE
#include "USBHAL_STM32F303ZE.h"
#endif

#if defined(TARGET_STM32F207ZG) || \
    defined(TARGET_STM32F401RE) || \
    defined(TARGET_STM32F407VG) || \
    defined(TARGET_STM32F411RE) || \
    defined(TARGET_STM32F412ZG) || \
    defined(TARGET_STM32F429ZI) || \
    defined(TARGET_STM32F446ZE) || \
    defined(TARGET_STM32F746ZG) || \
    defined(TARGET_STM32F767ZI)
#include "USBHAL_STM_144_64pins.h"
#endif

#ifdef TARGET_STM32L476VG
#include "USBHAL_STM32L476VG.h"
#endif

#ifdef TARGET_STM32F769NI
#include "USBHAL_STM32F769NI.h"
#endif
