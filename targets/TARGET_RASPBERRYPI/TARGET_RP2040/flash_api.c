/******************************************************************************
 * INCLUDE
 ******************************************************************************/
#if DEVICE_FLASH

#include "mbed_assert.h"
#include "mbed_critical.h"
#include "hal/flash_api.h"
#include "hardware/flash.h"

/******************************************************************************
 * CONSTANT
 ******************************************************************************/



/******************************************************************************
 * FUNCTION DEFINITION
 ******************************************************************************/

int32_t flash_init(flash_t *obj)
{
    (void)(obj);

    return 0;
}

int32_t flash_free(flash_t *obj)
{
    (void)(obj);

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)(obj);

    address = address - XIP_BASE;

    if ((address % FLASH_SECTOR_SIZE) != 0) {
        return -1;
    }

    core_util_critical_section_enter();
    flash_range_erase(address, FLASH_SECTOR_SIZE);
    core_util_critical_section_exit();

    return 0;
}

int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    (void)(obj);

    const uint8_t *flash_target_contents = (const uint8_t *) (address);

    for (int i = 0; i < size; i++) {
        data[i] = flash_target_contents[i];
    }
    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    (void)(obj);

    address = address - XIP_BASE;

    if ((address % FLASH_PAGE_SIZE) != 0) {
        return -1;
    }

    uint8_t buf[FLASH_PAGE_SIZE];

    for (int j = 0; j < size/FLASH_PAGE_SIZE; j++) {
        for (int i = 0; i < FLASH_PAGE_SIZE; i++) {
            buf[i] = data[j*FLASH_PAGE_SIZE + i];
        }
        address = address + j*FLASH_PAGE_SIZE;
        core_util_critical_section_enter();
        flash_range_program(address, buf, FLASH_PAGE_SIZE);
        core_util_critical_section_exit();
    }


    return 0;

}

uint32_t flash_get_size(const flash_t *obj)
{
    (void)(obj);

    return PICO_FLASH_SIZE_BYTES;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)(obj);

    return FLASH_SECTOR_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)(obj);

    return FLASH_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return XIP_BASE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;

}

#endif