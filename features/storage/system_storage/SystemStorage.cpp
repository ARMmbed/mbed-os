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

// Align a value to a specified size.
// Parameters :
// val           - [IN]   Value.
// size          - [IN]   Size.
// Return        : Aligned value.
static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

static inline uint32_t align_down(uint64_t val, uint64_t size)
{
    return (((val) / size)) * size;
}

MBED_WEAK BlockDevice *BlockDevice::get_default_instance()
{
#if COMPONENT_SPIF

    static SPIFBlockDevice default_bd;

    return &default_bd;

#elif COMPONENT_QSPIF

    static QSPIFBlockDevice default_bd;

    return &default_bd;

#elif COMPONENT_DATAFLASH

    static DataFlashBlockDevice default_bd;

    return &default_bd;

#elif COMPONENT_SD

#if (STATIC_PINMAP_READY)
    static SDBlockDevice default_bd(
        static_spi_pinmap,
        MBED_CONF_SD_SPI_CS
    );
#else
    static SDBlockDevice default_bd;
#endif

    return &default_bd;

#elif COMPONENT_FLASHIAP

#if (MBED_CONF_FLASHIAP_BLOCK_DEVICE_SIZE == 0) && (MBED_CONF_FLASHIAP_BLOCK_DEVICE_BASE_ADDRESS == 0xFFFFFFFF)

    size_t flash_size;
    uint32_t start_address;
    uint32_t bottom_address;
    FlashIAP flash;

    int ret = flash.init();
    if (ret != 0) {
        return 0;
    }

    //Find the start of first sector after text area
    int sector_size = flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR);
    bottom_address = align_up(FLASHIAP_APP_ROM_END_ADDR, sector_size);
    start_address = flash.get_flash_start();
    flash_size = flash.get_flash_size();

    ret = flash.deinit();

    int  total_size = start_address + flash_size - bottom_address;
    if (total_size % (sector_size * 2)) {
        total_size =  align_down(total_size, sector_size * 2);
    }
    static FlashIAPBlockDevice default_bd(bottom_address, total_size);

#else

    static FlashIAPBlockDevice default_bd;

#endif

    return &default_bd;

#else

    return NULL;

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
