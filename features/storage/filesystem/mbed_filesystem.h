/* filesystem
 * Copyright (c) 2016 ARM Limited
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
#ifndef MBED_FILESYSTEM_API_H
#define MBED_FILESYSTEM_API_H
/** \addtogroup filesystem */
/** @{*/


// Standard types
#include "platform/platform.h"

// FileSystem classes
#include "features/storage/filesystem/FileSystem.h"
#include "features/storage/filesystem/File.h"
#include "features/storage/filesystem/Dir.h"

// BlockDevice classes
#include "features/storage/blockdevice/BlockDevice.h"
#include "features/storage/blockdevice/ChainingBlockDevice.h"
#include "features/storage/blockdevice/SlicingBlockDevice.h"
#include "features/storage/blockdevice/HeapBlockDevice.h"

/** @}*/
#endif
