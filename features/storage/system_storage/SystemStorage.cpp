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
#include "SystemStorage.h"
#include "features/storage/blockdevice/BlockDevice.h"
#include "features/storage/filesystem/FileSystem.h"
#include "features/storage/filesystem/fat/FATFileSystem.h"
#include "features/storage/filesystem/littlefs/LittleFileSystem.h"
#include "mbed_error.h"


#if COMPONENT_SPIF
#include "components/storage/blockdevice/COMPONENT_SPIF/SPIFBlockDevice.h"
#endif

#if COMPONENT_RSPIF
#include "components/storage/blockdevice/COMPONENT_RSPIF/SPIFReducedBlockDevice.h"
#endif

#if COMPONENT_QSPIF
#include "components/storage/blockdevice/COMPONENT_QSPIF/QSPIFBlockDevice.h"
#endif

#if COMPONENT_DATAFLASH
#include "components/storage/blockdevice/COMPONENT_DATAFLASH/DataFlashBlockDevice.h"
#endif

#if COMPONENT_SD
#include "components/storage/blockdevice/COMPONENT_SD/SDBlockDevice.h"
#endif

#if COMPONENT_FLASHIAP
#include "components/storage/blockdevice/COMPONENT_FLASHIAP/FlashIAPBlockDevice.h"
#endif

using namespace mbed;



MBED_WEAK int avoid_conflict_nvstore_tdbstore(owner_type_e in_mem_owner)
{
    int status = MBED_SUCCESS;
    static PlatformMutex _mutex;
    static owner_type_e internal_memory_owner = NONE;

    _mutex.lock();

    if (internal_memory_owner != NONE &&
            internal_memory_owner != in_mem_owner) {

        status = MBED_ERROR_ALREADY_INITIALIZED;

    } else {

        internal_memory_owner = in_mem_owner;
    }

    _mutex.unlock();

    return status;
}

// Align a value to a specified size.
// Parameters :
// val           - [IN]   Value.
// size          - [IN]   Size.
// Return        : Aligned value.
static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

MBED_WEAK BlockDevice *BlockDevice::get_default_instance()
{
#if COMPONENT_SPIF

    static SPIFBlockDevice default_bd(
        MBED_CONF_SPIF_DRIVER_SPI_MOSI,
        MBED_CONF_SPIF_DRIVER_SPI_MISO,
        MBED_CONF_SPIF_DRIVER_SPI_CLK,
        MBED_CONF_SPIF_DRIVER_SPI_CS,
        MBED_CONF_SPIF_DRIVER_SPI_FREQ
    );

    return &default_bd;

#elif COMPONENT_RSPIF

    static SPIFReducedBlockDevice default_bd(
        MBED_CONF_RSPIF_DRIVER_SPI_MOSI,
        MBED_CONF_RSPIF_DRIVER_SPI_MISO,
        MBED_CONF_RSPIF_DRIVER_SPI_CLK,
        MBED_CONF_RSPIF_DRIVER_SPI_CS,
        MBED_CONF_RSPIF_DRIVER_SPI_FREQ
    );

    return &default_bd;

#elif COMPONENT_QSPIF

    static QSPIFBlockDevice default_bd(
        MBED_CONF_QSPIF_QSPI_IO0,
        MBED_CONF_QSPIF_QSPI_IO1,
        MBED_CONF_QSPIF_QSPI_IO2,
        MBED_CONF_QSPIF_QSPI_IO3,
        MBED_CONF_QSPIF_QSPI_SCK,
        MBED_CONF_QSPIF_QSPI_CSN,
        MBED_CONF_QSPIF_QSPI_POLARITY_MODE,
        MBED_CONF_QSPIF_QSPI_FREQ
    );

    return &default_bd;

#elif COMPONENT_DATAFLASH

    static DataFlashBlockDevice default_bd(
        MBED_CONF_DATAFLASH_SPI_MOSI,
        MBED_CONF_DATAFLASH_SPI_MISO,
        MBED_CONF_DATAFLASH_SPI_CLK,
        MBED_CONF_DATAFLASH_SPI_CS
    );

    return &default_bd;

#elif COMPONENT_SD

    static SDBlockDevice default_bd(
        MBED_CONF_SD_SPI_MOSI,
        MBED_CONF_SD_SPI_MISO,
        MBED_CONF_SD_SPI_CLK,
        MBED_CONF_SD_SPI_CS
    );

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
    bottom_address = align_up(FLASHIAP_APP_ROM_END_ADDR, flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR));
    start_address = flash.get_flash_start();
    flash_size = flash.get_flash_size();

    ret = flash.deinit();

    static FlashIAPBlockDevice default_bd(bottom_address, start_address + flash_size - bottom_address);

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
