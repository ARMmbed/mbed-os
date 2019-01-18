/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#if DEVICE_FLASH
#include "flash_api.h"
#include "mbed_critical.h"
#include "cmsis.h"
#include "flc.h"


/**
 *  * \defgroup flash_hal Flash HAL API
 *   * @{
 *    */

/** Initialize the flash peripheral and the flash_t object
 *  *
 *   * @param obj The flash object
 *    * @return 0 for success, -1 for error
 *     */
int32_t flash_init(flash_t *obj)
{
	return FLC_Init();
}

/** Uninitialize the flash peripheral and the flash_t object
 *  *
 *   * @param obj The flash object
 *    * @return 0 for success, -1 for error
 *     */
int32_t flash_free(flash_t *obj)
{
	return 0;
}

/** Erase one sector starting at defined address
 *  *
 *   * The address should be at sector boundary. This function does not do any check for address alignments
 *    * @param obj The flash object
 *     * @param address The sector starting address
 *      * @return 0 for success, -1 for error
 *       */
int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    if(FLC_PageErase(address, MXC_V_FLC_ERASE_CODE_PAGE_ERASE, MXC_V_FLC_FLSH_UNLOCK_KEY) != 0)
    {
        return -1;
    } else {
        return 0;
    }

}
      
/** Program pages starting at defined address
 *  *
 *   * The pages should not cross multiple sectors.
 *    * This function does not do any check for address alignments or if size is aligned to a page size.
 *     * @param obj The flash object
 *      * @param address The sector starting address
 *       * @param data The data buffer to be programmed
 *        * @param size The number of bytes to program
 *         * @return 0 for success, -1 for error
 *          */
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int32_t status = E_BUSY;

    while ( status == E_BUSY ) 
    {
        status = FLC_Write(address, data, size, MXC_V_FLC_FLSH_UNLOCK_KEY);
    }

    if (status != 0)
    {
        return -1;
    } else {
        return 0;
    }
        
}

/** Get sector size
 *  *
 *   * @param obj The flash object
 *    * @param address The sector starting address
 *     * @return The size of a sector
 *      */
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    /*  1 sector = 1 page */
    if (address >= (MXC_FLASH_MEM_BASE + MXC_FLASH_FULL_MEM_SIZE)) {
        return MBED_FLASH_INVALID_SIZE;
    } else {
        return MXC_FLASH_PAGE_SIZE;
    }
}

/** Get page size
 *  *
 *   * The page size defines the writable page size
 *    * @param obj The flash object
 *     * @return The size of a page
 *      */
uint32_t flash_get_page_size(const flash_t *obj)
{
    return MXC_FLASH_PAGE_SIZE;
}

/** Get start address for the flash region
 *  *
 *   * @param obj The flash object
 *    * @return The start address for the flash region
 *     */
uint32_t flash_get_start_address(const flash_t *obj)
{
    return MXC_FLASH_MEM_BASE;
}

/** Get the flash region size
 *  *
 *   * @param obj The flash object
 *    * @return The flash region size
 *     */
uint32_t flash_get_size(const flash_t *obj)
{
    return MXC_FLASH_FULL_MEM_SIZE;
}

/** Get the flash erase value
 *  *
 *   * @param obj The flash object
 *    * @return The flash erase value
 *     */
uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif

