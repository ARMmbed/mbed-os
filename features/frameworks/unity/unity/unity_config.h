
/** \addtogroup frameworks */
/** @{*/
/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

/* When using unity with the mbed RTOS printing to the serial port using the stdlib is not 
   allowed as it causes a hardfault. Unity has the following define to control how failure
   messages are written:
   
   #ifndef UNITY_OUTPUT_CHAR
       #include <stdio.h>
       #define UNITY_OUTPUT_CHAR(a) (void)putchar(a)
   #endif
   
   To make this safe we can define our own version of UNITY_OUTPUT_CHAR and make sure it is 
   thread safe.
*/
#ifndef UNITY_OUTPUT_CHAR
#define UNITY_OUTPUT_CHAR(a) utest_safe_putc(a)
#endif //UNITY_OUTPUT_CHAR
    
#endif // UNITY_CONFIG_H

/** @}*/
