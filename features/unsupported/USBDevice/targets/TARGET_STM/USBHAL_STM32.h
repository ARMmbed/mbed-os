/* Copyright (c) 2017 mbed.org, MIT License
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
#ifndef USBHAL_STM32_H
#define USBHAL_STM32_H

#if defined(TARGET_NUCLEO_F207ZG) || \
    defined(TARGET_NUCLEO_F401RE) || \
    defined(TARGET_STEVAL_3DP001V1) || \
    defined(TARGET_NUCLEO_F411RE) || \
    defined(TARGET_NUCLEO_F412ZG) || \
    defined(TARGET_NUCLEO_F413ZH) || \
    defined(TARGET_NUCLEO_F429ZI) || \
    defined(TARGET_NUCLEO_F446RE) || \
    defined(TARGET_NUCLEO_F446ZE) || \
    defined(TARGET_NUCLEO_F767ZI) || \
    defined(TARGET_NUCLEO_F746ZG) || \
    defined(TARGET_DISCO_F407VG) || \
    defined(TARGET_DISCO_F413ZH) || \
    defined(TARGET_DISCO_F429ZI) || \
    defined(TARGET_DISCO_F469NI) || \
    defined(TARGET_DISCO_F746NG) || \
    defined(TARGET_DISCO_F769NI) || \
    defined(TARGET_DISCO_L475VG_IOT01A) || \
    defined(TARGET_DISCO_L476VG)
#include "USBHAL_IP_OTGFSHS.h"

#elif defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F303ZE) || \
      defined(TARGET_DISCO_L053C8) || \
      defined(TARGET_DISCO_L072CZ_LRWAN1)
#include "USBHAL_IP_DEVICE.h"

#else
#error "Target not supported !"

#endif

#endif // USBHAL_STM32_H
