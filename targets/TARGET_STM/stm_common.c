#include "hal/us_ticker_api.h"

// Overwrite default HAL function
uint32_t HAL_GetTick()
{
    return ticker_read_us(get_us_ticker_data()) / 1000;
}
