/**
******************************************************************************
* @file cmsis_nvic.c
* @brief   Contains relocatable exception table.
* @internal
* @author ON Semiconductor.
* $Rev: 0.1 $
* $Date: 2015-11-06 $
******************************************************************************
* @copyright (c) 2012 ON Semiconductor. All rights reserved.
* ON Semiconductor is supplying this software for use with ON Semiconductor
* processor based microcontrollers only.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup app
*
* @details
*/
#include <cmsis_nvic.h>

#define NVIC_RAM_VECTOR_ADDRESS   (0x20000000)  // Vectors positioned at start of RAM
#define NVIC_FLASH_VECTOR_ADDRESS (0x00000000)  // Initial vector position in flash


void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector) 
{
    static volatile uint32_t *vectors = (uint32_t *)NVIC_FLASH_VECTOR_ADDRESS;

    vectors[IRQn + 16] = vector;
}
