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
#include "BlockDevice.h"
#include "FileSystem.h"
#include "FATFileSystem.h"
#include "LittleFileSystem.h"

#if COMPONENT_SPIF
#include "SPIFBlockDevice.h"
#endif

#if COMPONENT_DATAFLASH
#include "DataFlashBlockDevice.h"
#endif

#if COMPONENT_SD
#include "SDBlockDevice.h"
#endif

using namespace mbed;

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

#else

    return NULL;

#endif

}

MBED_WEAK FileSystem *FileSystem::get_default_instance()
{
#if COMPONENT_SPIF || COMPONENT_DATAFLASH

    static LittleFileSystem flash("flash", BlockDevice::get_default_instance());
    flash.set_as_default();

    return &flash;

#elif COMPONENT_SD

    static FATFileSystem sdcard("sd", BlockDevice::get_default_instance());
    sdcard.set_as_default();

    return &sdcard;

#else

    return NULL;

#endif

}
