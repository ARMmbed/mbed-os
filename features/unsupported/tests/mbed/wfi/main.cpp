#include "mbed.h"
#include "test_env.h"

int main(void)
{
    MBED_HOSTTEST_TIMEOUT(15);
    MBED_HOSTTEST_SELECT(wfi_auto);
    MBED_HOSTTEST_DESCRIPTION(WFI correct behavior);
    MBED_HOSTTEST_START("MBED_36");
#if defined TARGET_STM32F4    
    TIM_HandleTypeDef TimMasterHandle;
    TimMasterHandle.Instance = TIM5;
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC2);
#endif
    int count = 0;
    while(1) {
      printf("%d\r\n", count++);
      __WFI();
    }
}
