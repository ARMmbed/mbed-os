/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "mbed_assert.h"
#include "mbed_critical.h"
#include "hal/flash_api.h"
#include "flash.h"
//#include "hal/lp_ticker_api.h"
//#include "PeripheralPins.h"
#include "PeripheralNames.h"

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

    uint32_t address = 0;

    for (int i = 0; i < PICO_FLASH_SIZE_BYTES/FLASH_SECTOR_SIZE; i++ ) {
      flash_range_erase(address, FLASH_SECTOR_SIZE);
      address = address + FLASH_SECTOR_SIZE;
    }

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)(obj);

    flash_range_erase(address, FLASH_SECTOR_SIZE);

    return 0;
}

int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    (void)(obj);

    for (int i = 0; i < size; i++) {
        data = (const uint8_t *) (XIP_BASE + address + i);
        data++;
    }
    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    (void)(obj);

    flash_range_program(address, data, size);

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