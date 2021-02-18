#include "stm32f4xx.h"

/**
  * @brief  Sets F469 "Awake" signal (PK3 pin) to turn on daughterboard power supplies
  * @param  None
  * @retval None
  *
  */
void TargetBSP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable peripheral clock */
    __HAL_RCC_GPIOK_CLK_ENABLE();

    /* GPIO Configuration */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    /* Enable AWAKE pin */
    HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);
}
