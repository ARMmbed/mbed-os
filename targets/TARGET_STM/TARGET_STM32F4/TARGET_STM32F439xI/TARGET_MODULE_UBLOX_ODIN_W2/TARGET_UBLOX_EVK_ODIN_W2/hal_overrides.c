#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

void HAL_MspInit(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitDef;

    GPIO_InitDef.Pin = GPIO_PIN_6 | GPIO_PIN_8;
    GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitDef.Pull = GPIO_NOPULL;
    GPIO_InitDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
    
    GPIO_InitDef.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
}
