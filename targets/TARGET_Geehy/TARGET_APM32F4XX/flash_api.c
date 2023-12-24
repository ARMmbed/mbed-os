/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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
#include "mbed_critical.h"
#include "mbed_error.h"

#if DEVICE_FLASH
#include "cmsis.h"

#define FLASH_SIZE                (0x00100000U)
#define FLASH_START_ADDR          (0x08000000U)
#define FLASH_END_ADDR            (0x080FFFFFU)
#define SIZE_16KB                 (0x00004000U)
#define SIZE_64KB                 (0x00010000U)
#define SIZE_128KB                (0x00020000U)
#define WRONG_SECTOR_NUM          (0xFFFFFFFFU)
#define WORD_SIZE                 (4U)

typedef struct sector_information
{
    uint32_t sector_num;
    uint32_t sector_size;
} sector_t;

 /*!
 * @brief     get the sector number and size of the given address
 *
 * @param     address : The sector starting address
 *
 * @retval    The struct of a sector
 */
static sector_t flash_sector_infor_get(uint32_t address)
{
    sector_t sector_infor;
    uint32_t temp = 0x00000000;

    if ((address >= FLASH_START_ADDR) && (address <= FLASH_END_ADDR))
    {
        temp = (address - FLASH_START_ADDR) / SIZE_16KB;
        if (temp < 4U)
        {
            sector_infor.sector_num = temp;
            sector_infor.sector_size = SIZE_16KB;
        }
        else if (temp < 8U)
        {
            sector_infor.sector_num = 4U;
            sector_infor.sector_size = SIZE_64KB;
        }
        else 
        {
            temp = (address -FLASH_START_ADDR ) / SIZE_128KB;
            sector_infor.sector_num = temp + 4;
            sector_infor.sector_size = SIZE_128KB;
        }
    }
    else
    {
        sector_infor.sector_num = WRONG_SECTOR_NUM;
        sector_infor.sector_size = MBED_FLASH_INVALID_SIZE;
    }

    return sector_infor;
}

 /*!
 * @brief     get the sector num
 *
 * @param     num
 *
 * @retval    None
 */
FMC_SECTOR_T get_sector_num(int num)
{
    FMC_SECTOR_T sector;
    switch (num)
    {
        case 0:
            sector = FMC_SECTOR_0;
            break;
        case 1:
            sector = FMC_SECTOR_1;
            break;
        case 2:
            sector = FMC_SECTOR_2;
            break;
        case 3:
            sector = FMC_SECTOR_3;
            break;
        case 4:
            sector = FMC_SECTOR_4;
            break;
        case 5:
            sector = FMC_SECTOR_5;
            break;
        case 6:
            sector = FMC_SECTOR_6;
            break;
        case 7:
            sector = FMC_SECTOR_7;
            break;
        case 8:
            sector = FMC_SECTOR_8;
            break;
        case 9:
            sector = FMC_SECTOR_9;
            break;
        case 10:
            sector = FMC_SECTOR_10;
            break;
        case 11:
            sector = FMC_SECTOR_11;
            break;
        default:
            error("the value of num is illegal and can only be set to 0~11");
            break;
    }
    return sector;
}

 /*!
 * @brief     unlock the main FLASH operation
 *
 * @param     None
 *
 * @retval    0 for success, -1 for error
 */
static int32_t flash_unlock(void)
{
    FMC_Unlock();
    if (FMC->CTRL_B.LOCK == SET)
    {
        return -1;
    }
    return 0;
}

 /*!
 * @brief     lock the main FLASH operation
 *
 * @param     None
 *
 * @retval    0 for success, -1 for error
 */
static int32_t flash_lock(void)
{
    FMC_Lock();
    if (FMC->CTRL_B.LOCK != SET)
    {
        return -1;
    }
    return 0;
}

 /*!
 * @brief     Initialize the flash peripheral and the flash_t object
 *
 * @param     obj : The flash object
 *
 * @retval    0 for success, -1 for error
 */
int32_t flash_init(flash_t *obj)
{
    return 0;
}

 /*!
 * @brief     Uninitialize the flash peripheral and the flash_t object
 *
 * @param     obj : The flash object
 *
 * @retval    0 for success, -1 for error
 */
int32_t flash_free(flash_t *obj)
{
    return 0;
}

 /*!
 * @brief     Erase one sector starting at defined address.
 *
 * @param     obj : The flash object
 *
 * @param     address : The sector starting address
 *
 * @retval    0 for success, -1 for error
 */
int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    FMC_SECTOR_T sector_num;
    sector_t sector_info;

    if ((address > FLASH_END_ADDR) || (address < FLASH_START_ADDR))
    {
        return -1;
    }

    flash_unlock();

    FMC_ClearStatusFlag(FMC_FLAG_ENDOP | FMC_FLAG_ERRWRP | FMC_FLAG_ERROP |\
                        FMC_FLAG_ERRPGA | FMC_FLAG_ERRPGP | FMC_FLAG_ERRPGS);

    sector_info = flash_sector_infor_get(address);

    sector_num = get_sector_num(sector_info.sector_num);

    if (FMC_EraseSector(sector_num, FMC_VOLTAGE_3) != FMC_COMPLETE)
    {
        return -1;
    }

    return 0;
}

 /*!
 * @brief     Program pages starting at defined address.The pages should not cross multiple sectors.
 *
 * @param     obj : The flash object
 *
 * @param     address : The sector starting address
 *
 * @param     data : The data buffer to be programmed
 *
 * @param     size : The number of bytes to program
 *
 * @retval    0 for success, -1 for error
 */
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t *p_data;
    p_data = (uint32_t *)data;
    uint32_t num = 0;
    uint32_t status_flag = 0;

    if ((address > FLASH_END_ADDR) || (address < FLASH_START_ADDR))
    {
        return -1;
    }

    flash_unlock();
    
    FMC_ClearStatusFlag(FMC_FLAG_ENDOP | FMC_FLAG_ERRWRP | FMC_FLAG_ERROP |\
                        FMC_FLAG_ERRPGA | FMC_FLAG_ERRPGP | FMC_FLAG_ERRPGS);

    if (size % 4)
    {
        num = size / 4 + 1;
    }
    else
    {
        num = size / 4;
    }
    
    for (uint32_t i = 0; i < num; i++)
    {
        if (FMC_ProgramWord(address, *(p_data + i)) != FMC_COMPLETE)
        {
            status_flag = -1;
            break;
        }
        address += 4;
    }
    flash_lock();

    return status_flag;
}

 /*!
 * @brief     Get sector size.
 *
 * @param     obj : The flash object
 *
 * @param     address : The sector starting address
 *
 * @retval    The size of a sector
 */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    sector_t sector_infor;
    sector_infor = flash_sector_infor_get(address);
    return sector_infor.sector_size;
}

 /*!
 * @brief     Get sector size.The page size defines the writable page size
 *
 * @param     obj : The flash object
 *
 * @retval    The size of a sector
 */
uint32_t flash_get_page_size(const flash_t *obj)
{
    return WORD_SIZE;
}

 /*!
 * @brief     Get start address for the flash region
 *
 * @param     obj : The flash object
 *
 * @retval    The start address for the flash region
 */
uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_START_ADDR;
}

 /*!
 * @brief     Get the flash region size
 *
 * @param     obj : The flash object
 *
 * @retval    The flash region size
 */
uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_SIZE;
}

 /*!
 * @brief     Get the flash erase value
 *
 * @param     obj : The flash object
 *
 * @retval    The flash erase value
 */
uint8_t flash_get_erase_value(const flash_t *obj)
{
    return 0xFF;
}

#endif /* DEVICE_FLASH */
