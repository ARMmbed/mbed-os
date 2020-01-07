/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 ARM Limited
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

#if defined(TARGET_M451)
#define NUMBER_OF_LOGICAL_ENDPOINTS (8)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS)
#elif defined (TARGET_M480)
#if (MBED_CONF_TARGET_USB_DEVICE_HSUSBD == 0)
#define NUMBER_OF_LOGICAL_ENDPOINTS (12)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS)
#else
#define NUMBER_OF_LOGICAL_ENDPOINTS (12)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS)
#endif
#elif defined (TARGET_M2351) || defined(TARGET_M261)
#define NUMBER_OF_LOGICAL_ENDPOINTS (12)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS)
#elif defined (TARGET_NANO100)
#define NUMBER_OF_LOGICAL_ENDPOINTS (8)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS)
#elif defined (TARGET_NUC472)
#define NUMBER_OF_LOGICAL_ENDPOINTS (12)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS)
#endif

#define ALLOW_ALL_EXCEPT_CRTL        (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_ISO)
#define ALLOW_NO_ENDPOINTS 0
