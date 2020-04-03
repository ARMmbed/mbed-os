/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "flash_api.h"
#include "mbed_toolchain.h"
#include "mbed_critical.h"

#if DEVICE_FLASH

#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "flash_defines.h"

AT_QUICKACCESS_SECTION_CODE(void flexspi_update_lut_ram(void));
AT_QUICKACCESS_SECTION_CODE(status_t flexspi_nor_write_enable_ram(uint32_t baseAddr));
AT_QUICKACCESS_SECTION_CODE(status_t flexspi_nor_wait_bus_busy_ram(void));
AT_QUICKACCESS_SECTION_CODE(status_t flexspi_nor_flash_erase_sector_ram(uint32_t address));
AT_QUICKACCESS_SECTION_CODE(status_t flexspi_nor_flash_page_program_ram(uint32_t address,
                                                                        const uint32_t *src,
                                                                        uint32_t size));
AT_QUICKACCESS_SECTION_CODE(void flexspi_nor_flash_read_data_ram(uint32_t addr,
                                                                 uint32_t *buffer,
                                                                 uint32_t size));
AT_QUICKACCESS_SECTION_CODE(void *flexspi_memset(void *buf, int c, size_t n));
/**
 * @brief Set bytes in memory. If put this code in SRAM, Make sure this code
 * does not call functions in Flash.
 *
 * @return pointer to start of buffer
 */
void *flexspi_memset(void *buf, int c, size_t n)
{
    /* do byte-sized initialization until word-aligned or finished */
    unsigned char *d_byte = (unsigned char *)buf;
    unsigned char c_byte = (unsigned char)c;

    while (((unsigned int)d_byte) & 0x3) {
        if (n == 0) {
            return buf;
        }
        *(d_byte++) = c_byte;
        n--;
    };

    /* do word-sized initialization as long as possible */

    unsigned int *d_word = (unsigned int *)d_byte;
    unsigned int c_word = (unsigned int)(unsigned char)c;

    c_word |= c_word << 8;
    c_word |= c_word << 16;

    while (n >= sizeof(unsigned int)) {
        *(d_word++) = c_word;
        n -= sizeof(unsigned int);
    }

    /* do byte-sized initialization until finished */

    d_byte = (unsigned char *)d_word;

    while (n > 0) {
        *(d_byte++) = c_byte;
        n--;
    }

    return buf;
}

#ifdef HYPERFLASH_BOOT
AT_QUICKACCESS_SECTION_CODE(void flexspi_lower_clock_ram(void));
AT_QUICKACCESS_SECTION_CODE(void flexspi_clock_update_ram(void));
void flexspi_update_lut_ram(void)
{
    flexspi_config_t config;

    flexspi_memset(&config, 0, sizeof(config));

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch = true;
    /*Allow AHB read start address do not follow the alignment requirement. */
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableAHBCachable    = true;
    /* enable diff clock and DQS */
    config.enableSckBDiffOpt = true;
    config.rxSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
    config.enableCombination = true;
    FLEXSPI_Init(FLEXSPI, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    FLEXSPI_SoftwareReset(FLEXSPI);

    /* Wait for bus idle. */
    while (!FLEXSPI_GetBusIdleStatus(FLEXSPI)) {
    }
}

status_t flexspi_nor_write_enable_ram(uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status = kStatus_Success;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 2;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

    return status;
}

status_t flexspi_nor_wait_bus_busy_ram(void)
{
    /* Wait status ready. */
    bool isBusy = false;
    uint32_t readValue = 0;
    status_t status = kStatus_Success;
    flexspi_transfer_t flashXfer;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 2;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 2;

    do {
        status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

        if (status != kStatus_Success) {
            return status;
        }

        if (readValue & 0x8000) {
            isBusy = false;
        } else {
            isBusy = true;
        }

        if (readValue & 0x3200) {
            status = kStatus_Fail;
            break;
        }

    } while (isBusy);

    return status;

}

status_t flexspi_nor_flash_erase_sector_ram(uint32_t address)
{
    status_t status = kStatus_Success;
    flexspi_transfer_t flashXfer;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    /* Write enable */
    status = flexspi_nor_write_enable_ram(address);
    if (status != kStatus_Success) {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 4;
    flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR;

    status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);
    if (status != kStatus_Success) {
        return status;
    }

    status = flexspi_nor_wait_bus_busy_ram();

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    return status;
}

void flexspi_lower_clock_ram(void)
{
    unsigned int reg = 0;

    /* Wait for bus idle. */
    while (!FLEXSPI_GetBusIdleStatus(FLEXSPI)) {
    }

    FLEXSPI_Enable(FLEXSPI, false);

    /* Disable FlexSPI clock */
    CCM->CCGR6 &= ~CCM_CCGR6_CG5_MASK;

    /* flexspi clock 66M, DDR mode, internal clock 33M. */
    reg = CCM->CSCMR1;
    reg &= ~CCM_CSCMR1_FLEXSPI_PODF_MASK;
    reg |= CCM_CSCMR1_FLEXSPI_PODF(3);
    CCM->CSCMR1 = reg;

    /* Enable FlexSPI clock */
    CCM->CCGR6 |= CCM_CCGR6_CG5_MASK;

    FLEXSPI_Enable(FLEXSPI, true);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    /* Wait for bus idle. */
    while (!FLEXSPI_GetBusIdleStatus(FLEXSPI)) {
    }
}

void flexspi_clock_update_ram(void)
{
    /* Program finished, speed the clock to 133M. */
    /* Wait for bus idle before change flash configuration. */
    while (!FLEXSPI_GetBusIdleStatus(FLEXSPI)) {
    }
    FLEXSPI_Enable(FLEXSPI, false);
    /* Disable FlexSPI clock */
    CCM->CCGR6 &= ~CCM_CCGR6_CG5_MASK;

    /* flexspi clock 260M, DDR mode, internal clock 130M. */
    CCM->CSCMR1 &= ~CCM_CSCMR1_FLEXSPI_PODF_MASK;

    /* Enable FlexSPI clock */
    CCM->CCGR6 |= CCM_CCGR6_CG5_MASK;

    FLEXSPI_Enable(FLEXSPI, true);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    /* Wait for bus idle. */
    while (!FLEXSPI_GetBusIdleStatus(FLEXSPI)) {
    }
}

status_t flexspi_nor_flash_page_program_ram(uint32_t address, const uint32_t *src, uint32_t size)
{
    status_t status = kStatus_Success;
    flexspi_transfer_t flashXfer;
    uint32_t offset = 0;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    flexspi_lower_clock_ram();

    while (size > 0) {
        /* Write enable */
        status = flexspi_nor_write_enable_ram(address + offset);

        if (status != kStatus_Success) {
            return status;
        }

        /* Prepare page program command */
        flashXfer.deviceAddress = address + offset;
        flashXfer.port          = kFLEXSPI_PortA1;
        flashXfer.cmdType       = kFLEXSPI_Write;
        flashXfer.SeqNumber     = 2;
        flashXfer.seqIndex      = HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
        flashXfer.data          = (uint32_t *)(src + offset);
        flashXfer.dataSize      = BOARD_FLASH_PAGE_SIZE;

        status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

        if (status != kStatus_Success) {
            return status;
        }

        status = flexspi_nor_wait_bus_busy_ram();

        if (status != kStatus_Success) {
            return status;
        }

        size -= BOARD_FLASH_PAGE_SIZE;
        offset += BOARD_FLASH_PAGE_SIZE;
    }

    flexspi_clock_update_ram();

    return status;
}

#else
AT_QUICKACCESS_SECTION_CODE(status_t flexspi_nor_enable_quad_mode_ram(void));
status_t flexspi_nor_enable_quad_mode_ram(void)
{
    flexspi_transfer_t flashXfer;
    uint32_t writeValue = FLASH_QUAD_ENABLE;
    status_t status = kStatus_Success;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));
    /* Write enable */
    status = flexspi_nor_write_enable_ram(0);

    if (status != kStatus_Success) {
        return status;
    }

    /* Enable quad mode. */
    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG;
    flashXfer.data          = &writeValue;
    flashXfer.dataSize      = 1;

    status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);
    if (status != kStatus_Success) {
        return status;
    }

    status = flexspi_nor_wait_bus_busy_ram();

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    return status;
}

void flexspi_update_lut_ram(void)
{
    flexspi_config_t config;

    flexspi_memset(&config, 0, sizeof(config));

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(FLEXSPI, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);
    /* Wait for bus idle. */
    while (!FLEXSPI_GetBusIdleStatus(FLEXSPI)) {
    }
    flexspi_nor_enable_quad_mode_ram();
}

status_t flexspi_nor_write_enable_ram(uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status = kStatus_Success;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));
    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

    return status;
}

status_t flexspi_nor_wait_bus_busy_ram(void)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status = kStatus_Success;
    flexspi_transfer_t flashXfer;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 1;

    do {
        status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

        if (status != kStatus_Success) {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL) {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET)) {
                isBusy = true;
            } else {
                isBusy = false;
            }
        } else {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET)) {
                isBusy = false;
            } else {
                isBusy = true;
            }
        }

    } while (isBusy);

    return status;
}


status_t flexspi_nor_flash_erase_sector_ram(uint32_t address)
{
    flexspi_transfer_t flashXfer;
    status_t status = kStatus_Success;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    /* Write enable */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

    if (status != kStatus_Success) {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status                  = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

    if (status != kStatus_Success) {
        return status;
    }

    status = flexspi_nor_wait_bus_busy_ram();

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    return status;
}

status_t flexspi_nor_flash_page_program_ram(uint32_t address, const uint32_t *src, uint32_t size)
{
    flexspi_transfer_t flashXfer;
    status_t status = kStatus_Success;
    uint32_t offset = 0;

    flexspi_memset(&flashXfer, 0, sizeof(flashXfer));

    while (size > 0) {
        /* Write enable */
        status = flexspi_nor_write_enable_ram(address + offset);

        if (status != kStatus_Success) {
            return status;
        }

        /* Prepare page program command */
        flashXfer.deviceAddress = address + offset;
        flashXfer.port          = kFLEXSPI_PortA1;
        flashXfer.cmdType       = kFLEXSPI_Write;
        flashXfer.SeqNumber     = 1;
        flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
        flashXfer.data          = (uint32_t *)(src + offset);
        flashXfer.dataSize      = BOARD_FLASH_PAGE_SIZE;

        status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);

        if (status != kStatus_Success) {
            return status;
        }

        status = flexspi_nor_wait_bus_busy_ram();

        if (status != kStatus_Success) {
            return status;
        }

        size -= BOARD_FLASH_PAGE_SIZE;
        offset += BOARD_FLASH_PAGE_SIZE;
    }

    /* Do software reset. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    return status;
}

#endif
void flexspi_nor_flash_read_data_ram(uint32_t addr, uint32_t *buffer, uint32_t size)
{
    memcpy(buffer, (void *)addr, size);
}

int32_t flash_init(flash_t *obj)
{
    core_util_critical_section_enter();
    flexspi_update_lut_ram();
    core_util_critical_section_exit();

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    status_t status = kStatus_Success;
    int32_t ret = 0;

    core_util_critical_section_enter();

    status = flexspi_nor_flash_erase_sector_ram(address - FlexSPI_AMBA_BASE);

    if (status != kStatus_Success) {
        ret = -1;
    } else {
        DCACHE_InvalidateByRange(address, BOARD_FLASH_SECTOR_SIZE);
    }

    core_util_critical_section_exit();

    return ret;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    status_t status = kStatus_Success;
    int32_t ret = 0;

    core_util_critical_section_enter();

    status = flexspi_nor_flash_page_program_ram(address - FlexSPI_AMBA_BASE, (uint32_t *)data, size);

    if (status != kStatus_Success) {
        ret = -1;
    } else {
        DCACHE_InvalidateByRange(address, size);
    }

    core_util_critical_section_exit();

    return ret;
}

int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    flexspi_nor_flash_read_data_ram(address, (uint32_t *)data, size);

    return 0;
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    uint32_t sectorsize = MBED_FLASH_INVALID_SIZE;
    uint32_t devicesize = BOARD_FLASH_SIZE;
    uint32_t startaddr = BOARD_FLASH_START_ADDR;

    if ((address >= startaddr) && (address < (startaddr + devicesize))) {
        sectorsize = BOARD_FLASH_SECTOR_SIZE;
    }

    return sectorsize;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return BOARD_FLASH_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return BOARD_FLASHIAP_START_ADDR;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return BOARD_FLASHIAP_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif //DEVICE_FLASH

