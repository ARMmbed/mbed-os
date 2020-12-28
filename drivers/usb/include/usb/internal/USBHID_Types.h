/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#ifndef USBCLASS_HID_TYPES
#define USBCLASS_HID_TYPES

#include <stdint.h>

/* */
#define HID_VERSION_1_11    (0x0111)

/* HID Class */
#define HID_CLASS             (3)
#define HID_SUBCLASS_NONE     (0)
#define HID_SUBCLASS_BOOT     (1)
#define HID_PROTOCOL_NONE     (0)
#define HID_PROTOCOL_KEYBOARD (1)
#define HID_PROTOCOL_MOUSE    (2)

/* Descriptors */
#define HID_DESCRIPTOR          (33)
#define HID_DESCRIPTOR_LENGTH   (0x09)
#define REPORT_DESCRIPTOR       (34)

/* Class requests */
#define GET_REPORT (0x1)
#define GET_IDLE   (0x2)
#define SET_REPORT (0x9)
#define SET_IDLE   (0xa)

/* HID Class Report Descriptor */
/* Short items: size is 0, 1, 2 or 3 specifying 0, 1, 2 or 4 (four) bytes */
/* of data as per HID Class standard */

/* Main items */
#define INPUT(size)             (0x80 | size)
#define OUTPUT(size)            (0x90 | size)
#define FEATURE(size)           (0xb0 | size)
#define COLLECTION(size)        (0xa0 | size)
#define END_COLLECTION(size)    (0xc0 | size)

/* Global items */
#define USAGE_PAGE(size)        (0x04 | size)
#define LOGICAL_MINIMUM(size)   (0x14 | size)
#define LOGICAL_MAXIMUM(size)   (0x24 | size)
#define PHYSICAL_MINIMUM(size)  (0x34 | size)
#define PHYSICAL_MAXIMUM(size)  (0x44 | size)
#define UNIT_EXPONENT(size)     (0x54 | size)
#define UNIT(size)              (0x64 | size)
#define REPORT_SIZE(size)       (0x74 | size)
#define REPORT_ID(size)         (0x84 | size)
#define REPORT_COUNT(size)      (0x94 | size)
#define PUSH(size)              (0xa4 | size)
#define POP(size)               (0xb4 | size)

/* Local items */
#define USAGE(size)                 (0x08 | size)
#define USAGE_MINIMUM(size)         (0x18 | size)
#define USAGE_MAXIMUM(size)         (0x28 | size)
#define DESIGNATOR_INDEX(size)      (0x38 | size)
#define DESIGNATOR_MINIMUM(size)    (0x48 | size)
#define DESIGNATOR_MAXIMUM(size)    (0x58 | size)
#define STRING_INDEX(size)          (0x78 | size)
#define STRING_MINIMUM(size)        (0x88 | size)
#define STRING_MAXIMUM(size)        (0x98 | size)
#define DELIMITER(size)             (0xa8 | size)

/* HID Report */
/* Where report IDs are used the first byte of 'data' will be the */
/* report ID and 'length' will include this report ID byte. */

#define MAX_HID_REPORT_SIZE (64)

typedef struct {
    uint32_t length;
    uint8_t data[MAX_HID_REPORT_SIZE];
} HID_REPORT;

#endif
