/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 * This file contains miscellaneous functionality used by the FAT filesystem interface code.
 */

#ifndef FILESYSTEM_FAT_MISC_H
#define FILESYSTEM_FAT_MISC_H

#include "ff.h"

void fat_filesystem_set_errno(FRESULT res);

#endif /* FILESYSTEM_FAT_MISC_H */
