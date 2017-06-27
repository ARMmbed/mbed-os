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

/*  144 pins boards */
#if defined(TARGET_NUCLEO_F429ZI) || defined(TARGET_NUCLEO_F446ZE)  || defined(TARGET_NUCLEO_F207ZG) \
|| defined(TARGET_NUCLEO_F767ZI) || defined(TARGET_NUCLEO_F746ZG) || defined(TARGET_NUCLEO_F412ZG) \
|| defined(TARGET_DISCO_F413ZH)
#include "USBHALHost_STM_144_64pins.h"
#endif

/*  64 pins boards */
#if defined(TARGET_NUCLEO_F401RE) || defined(TARGET_NUCLEO_L476RG) || defined(TARGET_NUCLEO_F411RE)
#define USBHALHOST_64pins
#include "USBHALHost_STM_144_64pins.h"
#endif

/*  DISCO board  */

#ifdef TARGET_DISCO_F429ZI
#include "USBHALHost_DISCOF429ZI.h"
#endif

#ifdef TARGET_DISCO_L476VG
#include "USBHALHost_DISCOL476VG.h"
#endif
