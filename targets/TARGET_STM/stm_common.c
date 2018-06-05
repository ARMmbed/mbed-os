#include "hal/us_ticker_api.h"

// Overwrite default HAL functions defined as "weak"

uint32_t HAL_GetTick()
{
    return ticker_read_us(get_us_ticker_data()) / 1000; // 1 ms tick is required for ST HAL
}

void HAL_SuspendTick(void)
{
    // Do nothing
}

void HAL_ResumeTick(void)
{
    // Do nothing
}
