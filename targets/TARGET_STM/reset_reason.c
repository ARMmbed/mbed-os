#include "reset_reason_api.h"

#ifdef DEVICE_RESET_REASON

#include "device.h"

reset_reason_t hal_reset_reason_get(void)
{
#ifdef RCC_FLAG_SFTRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        return RESET_REASON_SOFTWARE;
    }
#endif

#ifdef RCC_FLAG_BORRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)) {
        return RESET_REASON_BROWN_OUT;
    }
#endif

#ifdef RCC_FLAG_PORRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
        return RESET_REASON_POWER_ON;
    }
#endif

#ifdef RCC_FLAG_PINRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
        return RESET_REASON_PIN_RESET;
    }
#endif

#ifdef RCC_FLAG_IWDGRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        return RESET_REASON_WATCHDOG;
    }
#endif

#ifdef RCC_FLAG_WWDGRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) {
        return RESET_REASON_WATCHDOG;
    }
#endif

#ifdef RCC_FLAG_LPWRRST
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) {
        return RESET_REASON_WAKE_LOW_POWER;
    }
#endif

    return RESET_REASON_UNKNOWN;
}


uint32_t hal_reset_reason_get_raw(void)
{
    return RCC->CSR;
}


void hal_reset_reason_clear(void)
{
    __HAL_RCC_CLEAR_RESET_FLAGS();
}

#endif // DEVICE_RESET_REASON
