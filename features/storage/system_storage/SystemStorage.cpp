/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "features/storage/blockdevice/BlockDevice.h"
#include "features/storage/filesystem/FileSystem.h"
#include "features/storage/filesystem/fat/FATFileSystem.h"
#include "features/storage/filesystem/littlefs/LittleFileSystem.h"
#include "mbed_error.h"


#if COMPONENT_SPIF
#include "components/storage/blockdevice/COMPONENT_SPIF/SPIFBlockDevice.h"
#endif

#if COMPONENT_QSPIF
#include "components/storage/blockdevice/COMPONENT_QSPIF/QSPIFBlockDevice.h"
#endif

#if COMPONENT_DATAFLASH
#include "components/storage/blockdevice/COMPONENT_DATAFLASH/DataFlashBlockDevice.h"
#endif

#if COMPONENT_SD
#include "components/storage/blockdevice/COMPONENT_SD/SDBlockDevice.h"

#if (STATIC_PINMAP_READY)
const spi_pinmap_t static_spi_pinmap = get_spi_pinmap(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, NC);
#endif
#endif

#if COMPONENT_FLASHIAP
#include "components/storage/blockdevice/COMPONENT_FLASHIAP/FlashIAPBlockDevice.h"
#endif

using namespace mbed;

MBED_WEAK BlockDevice *BlockDevice::get_default_instance()
{
#if COMPONENT_SPIF

    return SPIFBlockDevice::get_target_default_instance();

#elif COMPONENT_QSPIF

    return QSPIFBlockDevice::get_target_default_instance();

#elif COMPONENT_DATAFLASH

    return DataFlashBlockDevice::get_target_default_instance();

#elif COMPONENT_SD

    return SDBlockDevice::get_target_default_instance();

#elif COMPONENT_FLASHIAP

    return FlashIAPBlockDevice::get_target_default_instance();

#else

    return nullptr;

#endif

}

MBED_WEAK FileSystem *FileSystem::get_default_instance()
{
#if COMPONENT_SPIF || COMPONENT_QSPIF || COMPONENT_DATAFLASH

    static LittleFileSystem flash("flash", BlockDevice::get_default_instance());
    flash.set_as_default();

    return &flash;

#elif COMPONENT_SD

    static FATFileSystem sdcard("sd", BlockDevice::get_default_instance());
    sdcard.set_as_default();

    return &sdcard;

#elif COMPONENT_FLASHIAP

    static LittleFileSystem flash("flash", BlockDevice::get_default_instance());
    flash.set_as_default();

    return &flash;

#else

    return NULL;

#endif

}
