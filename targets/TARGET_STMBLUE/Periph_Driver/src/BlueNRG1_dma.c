/**
  ******************************************************************************
  * @file    BlueNRG1_dma.c
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file provides all the DMA firmware functions.            
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_dma.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @defgroup DMA_Peripheral  DMA Peripheral
  * @{
  */ 


/** @defgroup DMA_Private_TypesDefinitions Private Type Definitions
* @{
*/

/**
* @}
*/

/** @defgroup DMA_Private_Defines Private Defines
* @{
*/
/* DMA registers Masks */
#define CCR_CLEAR_MASK           ((uint32_t)0xFFFF800F)

/**
* @}
*/

/** @defgroup DMA_Private_Macros Private Macros
* @{
*/

/* DMA Channelx interrupt pending bit masks */
#define DMA_CHANNEL0_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL0 | DMA_FLAG_TC0 | DMA_FLAG_HT0 | DMA_FLAG_TE0))
#define DMA_CHANNEL1_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL1 | DMA_FLAG_TC1 | DMA_FLAG_HT1 | DMA_FLAG_TE1))
#define DMA_CHANNEL2_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL2 | DMA_FLAG_TC2 | DMA_FLAG_HT2 | DMA_FLAG_TE2))
#define DMA_CHANNEL3_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL3 | DMA_FLAG_TC3 | DMA_FLAG_HT3 | DMA_FLAG_TE3))
#define DMA_CHANNEL4_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL4 | DMA_FLAG_TC4 | DMA_FLAG_HT4 | DMA_FLAG_TE4))
#define DMA_CHANNEL5_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL5 | DMA_FLAG_TC5 | DMA_FLAG_HT5 | DMA_FLAG_TE5))
#define DMA_CHANNEL6_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL6 | DMA_FLAG_TC6 | DMA_FLAG_HT6 | DMA_FLAG_TE6))
#define DMA_CHANNEL7_FLAG_MASK    ((uint32_t)(DMA_FLAG_GL7 | DMA_FLAG_TC7 | DMA_FLAG_HT7 | DMA_FLAG_TE7))


#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA_CH0) || \
                                   ((PERIPH) == DMA_CH1) || \
                                   ((PERIPH) == DMA_CH2) || \
                                   ((PERIPH) == DMA_CH3) || \
                                   ((PERIPH) == DMA_CH4) || \
                                   ((PERIPH) == DMA_CH5) || \
                                   ((PERIPH) == DMA_CH6) || \
                                   ((PERIPH) == DMA_CH7))
/**
* @}
*/

/** @defgroup DMA_Private_Variables Private Variables
* @{
*/

/**
* @}
*/

/** @defgroup DMA_Private_FunctionPrototypes Private Function Prototypes
* @{
*/

/**
* @}
*/

/** @defgroup DMA_Private_Functions Private Functions
* @{
*/



/**
* @}
*/

/** @defgroup DMA_Public_Functions Public Functions
* @{
*/
  
/**
  * @brief  Deinitializes the DMAy Channelx registers to their default reset
  *         values.
  * @param  DMAy_Channelx: where y can be 1 to select the DMA and
  *         x can be 1 to 7 for DMA to select the DMA Channel.
  * @retval None
  */
void DMA_DeInit(DMA_CH_Type* DMAy_Channelx)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));

  /* Disable the selected DMAy Channelx */
  DMAy_Channelx->CCR_b.EN = RESET;

  /* Reset DMAy Channelx control register */
  DMAy_Channelx->CCR  = 0;
  
  /* Reset DMAy Channelx remaining bytes register */
  DMAy_Channelx->CNDTR = 0;
  
  /* Reset DMAy Channelx peripheral address register */
  DMAy_Channelx->CPAR  = 0;
  
  /* Reset DMAy Channelx memory address register */
  DMAy_Channelx->CMAR = 0;
  
  if (DMAy_Channelx == DMA_CH1)
  {
    /* Reset interrupt pending bits for DMA Channel1 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL1_FLAG_MASK);
  }
  else if (DMAy_Channelx == DMA_CH2)
  {
    /* Reset interrupt pending bits for DMA Channel2 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL2_FLAG_MASK);
  }
  else if (DMAy_Channelx == DMA_CH3)
  {
    /* Reset interrupt pending bits for DMA Channel3 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL3_FLAG_MASK);
  }
  else if (DMAy_Channelx == DMA_CH4)
  {
    /* Reset interrupt pending bits for DMA Channel4 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL4_FLAG_MASK);
  }
  else if (DMAy_Channelx == DMA_CH5)
  {
    /* Reset interrupt pending bits for DMA Channel5 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL5_FLAG_MASK);
  }
  else if (DMAy_Channelx == DMA_CH6)
  {
    /* Reset interrupt pending bits for DMA Channel6 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL6_FLAG_MASK);
  }
  else if (DMAy_Channelx == DMA_CH7)
  {
    /* Reset interrupt pending bits for DMA Channel7 */
    SET_BIT(DMA->IFCR, DMA_CHANNEL7_FLAG_MASK);    
  }
}

/**
  * @brief  Initializes the DMAy Channelx according to the specified
  *         parameters in the DMA_InitStruct.
  * @param  DMAy_Channelx: where y can be 1 to select the DMA and
  *         x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  DMA_InitStruct: pointer to a DMA_InitType structure that
  *         contains the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_Init(DMA_CH_Type* DMAy_Channelx, DMA_InitType* DMA_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
  assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));   
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
  assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
  assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
  assert_param(IS_DMA_M2M_STATE(DMA_InitStruct->DMA_M2M));

/*--------------------------- DMAy Channelx CCR Configuration -----------------*/
  /* Get the DMAy_Channelx CCR value */
  tmpreg = DMAy_Channelx->CCR;
  /* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
  tmpreg &= CCR_CLEAR_MASK;
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode */
  /* Set DIR bit according to DMA_DIR value */
  /* Set CIRC bit according to DMA_Mode value */
  /* Set PINC bit according to DMA_PeripheralInc value */
  /* Set MINC bit according to DMA_MemoryInc value */
  /* Set PSIZE bits according to DMA_PeripheralDataSize value */
  /* Set MSIZE bits according to DMA_MemoryDataSize value */
  /* Set PL bits according to DMA_Priority value */
  /* Set the MEM2MEM bit according to DMA_M2M value */
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;

  /* Write to DMAy Channelx CCR */
  DMAy_Channelx->CCR = tmpreg;

/*--------------------------- DMAy Channelx CNDTR Configuration ---------------*/
  /* Write to DMAy Channelx CNDTR */
  DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;

/*--------------------------- DMAy Channelx CPAR Configuration ----------------*/
  /* Write to DMAy Channelx CPAR */
  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

/*--------------------------- DMAy Channelx CMAR Configuration ----------------*/
  /* Write to DMAy Channelx CMAR */
  DMAy_Channelx->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct: pointer to a DMA_InitType structure which will
  *         be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitType* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
  /* Initialize the DMA_MemoryBaseAddr member */
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;
  /* Initialize the DMA_DIR member */
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
  /* Initialize the DMA_BufferSize member */
  DMA_InitStruct->DMA_BufferSize = 0;
  /* Initialize the DMA_PeripheralInc member */
  DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* Initialize the DMA_MemoryInc member */
  DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
  /* Initialize the DMA_PeripheralDataSize member */
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* Initialize the DMA_MemoryDataSize member */
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* Initialize the DMA_Mode member */
  DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
  /* Initialize the DMA_Priority member */
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
  /* Initialize the DMA_M2M member */
  DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/**
  * @brief  Enables or disables the specified DMAy Channelx.
  * @param  DMAy_Channelx: where y can be 1 to select the DMA and
  *         x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  NewState: new state of the DMAy Channelx. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(DMA_CH_Type* DMAy_Channelx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE) {
    /* Enable the selected DMAy Channelx */
    DMAy_Channelx->CCR_b.EN = SET;
  }
  else {
    /* Disable the selected DMAy Channelx */
    DMAy_Channelx->CCR_b.EN = RESET;
  }
}


/**
  * @brief  Enables or disables the specified DMA ADC Channel.
  * @param  DMA_AdcChannel: ADC channel from 0 to 7.
  * @param  NewState: new state of the DMA ADC Channel.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_SelectAdcChannel(uint8_t DMA_AdcChannel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DMA_ADC_CH(DMA_AdcChannel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE) {
    /* Enable the selected of DMA ADC Channel */
    SET_BIT(CKGEN_SOC->DMA_CONFIG, DMA_AdcChannel);
  }
  else {
    /* Disable the selected of DMA ADC Channel */
    CLEAR_BIT(CKGEN_SOC->DMA_CONFIG, DMA_AdcChannel);
  }
}


/**
  * @brief  Sets the number of data units in the current DMAy Channelx transfer.
  * @param  DMAy_Channelx: where y can be 1 to select the DMA and
  *         x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  DataNumber: The number of data units in the current DMAy Channelx
  *         transfer.   
  * @note   This function can only be used when the DMAy_Channelx is disabled.                 
  * @retval None.
  */
void DMA_SetCurrDataCounter(DMA_CH_Type* DMAy_Channelx, uint16_t DataNumber)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  
/*--------------------------- DMAy Channelx CNDTR Configuration ---------------*/
  /* Write to DMAy Channelx CNDTR */
  DMAy_Channelx->CNDTR = (uint32_t)DataNumber;  
}

/**
  * @brief  Returns the number of remaining data units in the current
  *         DMAy Channelx transfer.
  * @param  DMAy_Channelx: where y can be 1 to select the DMA and
  *         x can be 1 to 7 for DMA to select the DMA Channel.
  * @retval The number of remaining data units in the current DMAy Channelx
  *         transfer.
  */
uint16_t DMA_GetCurrDataCounter(DMA_CH_Type* DMAy_Channelx)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* Return the number of remaining data units for DMAy Channelx */
  return ((uint16_t)(DMAy_Channelx->CNDTR));
}

/**
  * @brief  Enables or disables the specified DMAy Channelx interrupts.
  * @param  DMAy_Channelx: where y can be 1 to select the DMA and
  *         x can be 1 to 7 for DMA to select the DMA Channel.
  * @param  DMA_Flag: specifies the DMA interrupts sources to be enabled
  *         or disabled. 
  *   This parameter can be any combination of the following values:
  *     @arg DMA_Flag_TC: Transfer complete interrupt mask
  *     @arg DMA_Flag_HT: Half transfer interrupt mask
  *     @arg DMA_Flag_TE: Transfer error interrupt mask
  * @param  NewState: new state of the specified DMA interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_FlagConfig(DMA_CH_Type* DMAy_Channelx, uint32_t DMA_Flag, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_FLAG(DMA_Flag));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE) {
    /* Enable the selected DMA interrupts */
    SET_BIT(DMAy_Channelx->CCR, DMA_Flag);
  }
  else {
    /* Disable the selected DMA interrupts */
    CLEAR_BIT(DMAy_Channelx->CCR, DMA_Flag);
  }
}

/**
  * @brief  Checks whether the specified DMAy Channelx flag is set or not.
  * @param  DMA_Flag: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg DMA_FLAG_GL1: DMA Channel1 global flag.
  *     @arg DMA_FLAG_TC1: DMA Channel1 transfer complete flag.
  *     @arg DMA_FLAG_HT1: DMA Channel1 half transfer flag.
  *     @arg DMA_FLAG_TE1: DMA Channel1 transfer error flag.
  *     @arg DMA_FLAG_GL2: DMA Channel2 global flag.
  *     @arg DMA_FLAG_TC2: DMA Channel2 transfer complete flag.
  *     @arg DMA_FLAG_HT2: DMA Channel2 half transfer flag.
  *     @arg DMA_FLAG_TE2: DMA Channel2 transfer error flag.
  *     @arg DMA_FLAG_GL3: DMA Channel3 global flag.
  *     @arg DMA_FLAG_TC3: DMA Channel3 transfer complete flag.
  *     @arg DMA_FLAG_HT3: DMA Channel3 half transfer flag.
  *     @arg DMA_FLAG_TE3: DMA Channel3 transfer error flag.
  *     @arg DMA_FLAG_GL4: DMA Channel4 global flag.
  *     @arg DMA_FLAG_TC4: DMA Channel4 transfer complete flag.
  *     @arg DMA_FLAG_HT4: DMA Channel4 half transfer flag.
  *     @arg DMA_FLAG_TE4: DMA Channel4 transfer error flag.
  *     @arg DMA_FLAG_GL5: DMA Channel5 global flag.
  *     @arg DMA_FLAG_TC5: DMA Channel5 transfer complete flag.
  *     @arg DMA_FLAG_HT5: DMA Channel5 half transfer flag.
  *     @arg DMA_FLAG_TE5: DMA Channel5 transfer error flag.
  *     @arg DMA_FLAG_GL6: DMA Channel6 global flag.
  *     @arg DMA_FLAG_TC6: DMA Channel6 transfer complete flag.
  *     @arg DMA_FLAG_HT6: DMA Channel6 half transfer flag.
  *     @arg DMA_FLAG_TE6: DMA Channel6 transfer error flag.
  *     @arg DMA_FLAG_GL7: DMA Channel7 global flag.
  *     @arg DMA_FLAG_TC7: DMA Channel7 transfer complete flag.
  *     @arg DMA_FLAG_HT7: DMA Channel7 half transfer flag.
  *     @arg DMA_FLAG_TE7: DMA Channel7 transfer error flag.
  *     
  * @note
  *    The Global flag (DMAy_FLAG_GLx) is set whenever any of the other flags 
  *    relative to the same channel is set (Transfer Complete, Half-transfer 
  *    Complete or Transfer Error flags: DMAy_FLAG_TCx, DMAy_FLAG_HTx or 
  *    DMAy_FLAG_TEx). 
  *      
  * @retval The new state of DMA_Flag (SET or RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_Flag)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_DMA_CH_FLAG(DMA_Flag));

  /* Get DMA ISR register value */
  tmpreg = DMA->ISR;

  /* Check the status of the specified DMA flag */
  if ((tmpreg & DMA_Flag) != (uint32_t)RESET)
  {
    /* DMA_Flag is set */
    return SET;
  }
  else
  {
    /* DMA_Flag is reset */
    return RESET;
  }
  
}


/**
  * @brief  Clears the DMAy Channelx's pending flags.
  * @param  DMA_Flag: specifies the flag to clear.
  *   This parameter can be any combination (for the same DMA) of the following values:
  *     @arg DMA_FLAG_GL1: DMA Channel1 global flag.
  *     @arg DMA_FLAG_TC1: DMA Channel1 transfer complete flag.
  *     @arg DMA_FLAG_HT1: DMA Channel1 half transfer flag.
  *     @arg DMA_FLAG_TE1: DMA Channel1 transfer error flag.
  *     @arg DMA_FLAG_GL2: DMA Channel2 global flag.
  *     @arg DMA_FLAG_TC2: DMA Channel2 transfer complete flag.
  *     @arg DMA_FLAG_HT2: DMA Channel2 half transfer flag.
  *     @arg DMA_FLAG_TE2: DMA Channel2 transfer error flag.
  *     @arg DMA_FLAG_GL3: DMA Channel3 global flag.
  *     @arg DMA_FLAG_TC3: DMA Channel3 transfer complete flag.
  *     @arg DMA_FLAG_HT3: DMA Channel3 half transfer flag.
  *     @arg DMA_FLAG_TE3: DMA Channel3 transfer error flag.
  *     @arg DMA_FLAG_GL4: DMA Channel4 global flag.
  *     @arg DMA_FLAG_TC4: DMA Channel4 transfer complete flag.
  *     @arg DMA_FLAG_HT4: DMA Channel4 half transfer flag.
  *     @arg DMA_FLAG_TE4: DMA Channel4 transfer error flag.
  *     @arg DMA_FLAG_GL5: DMA Channel5 global flag.
  *     @arg DMA_FLAG_TC5: DMA Channel5 transfer complete flag.
  *     @arg DMA_FLAG_HT5: DMA Channel5 half transfer flag.
  *     @arg DMA_FLAG_TE5: DMA Channel5 transfer error flag.
  *     @arg DMA_FLAG_GL6: DMA Channel6 global flag.
  *     @arg DMA_FLAG_TC6: DMA Channel6 transfer complete flag.
  *     @arg DMA_FLAG_HT6: DMA Channel6 half transfer flag.
  *     @arg DMA_FLAG_TE6: DMA Channel6 transfer error flag.
  *     @arg DMA_FLAG_GL7: DMA Channel7 global flag.
  *     @arg DMA_FLAG_TC7: DMA Channel7 transfer complete flag.
  *     @arg DMA_FLAG_HT7: DMA Channel7 half transfer flag.
  *     @arg DMA_FLAG_TE7: DMA Channel7 transfer error flag.
  *     
  * @note
  *    Clearing the Global flag (DMAy_FLAG_GLx) results in clearing all other flags
  *    relative to the same channel (Transfer Complete, Half-transfer Complete and 
  *    Transfer Error flags: DMAy_FLAG_TCx, DMAy_FLAG_HTx and DMAy_FLAG_TEx).  
  *      
  * @retval None
  */
void DMA_ClearFlag(uint32_t DMA_Flag)
{
  /* Check the parameters */
  assert_param(IS_DMA_CH_FLAG(DMA_Flag));

  /* Clear the selected DMA flags */
  DMA->IFCR = DMA_Flag;

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
