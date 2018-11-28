/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef BLE_PAL_DEPRECATED_H
#define BLE_PAL_DEPRECATED_H

/**
 * Declare the beginning of a code block that uses a deprecated API
 */
#if defined(__GNUC__) && !defined(__CC_ARM)
#define BLE_DEPRECATED_API_USE_BEGIN() \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif defined(__CC_ARM)
#define BLE_DEPRECATED_API_USE_BEGIN() \
    _Pragma("push") \
    _Pragma("diag_suppress 1361")
#else
#define BLE_DEPRECATED_API_USE_BEGIN()
#endif

/**
 * Declare the end of a code block that uses a deprecated API
 */
#if defined(__GNUC__) && !defined(__CC_ARM)
#define BLE_DEPRECATED_API_USE_END() \
    _Pragma("GCC diagnostic pop")
#elif defined(__CC_ARM)
#define BLE_DEPRECATED_API_USE_END() \
        _Pragma("pop")
#else
#define BLE_DEPRECATED_API_USE_END()
#endif


#endif //BLE_PAL_DEPRECATED_H
