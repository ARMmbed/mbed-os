/**
  ******************************************************************************
  * @file    stm32f0xx_hal_dma.c
  * @author  MCD Application Team
  * @brief   DMA HAL module driver.
  *    
  *         This file provides firmware functions to manage the following 
  *         functionalities of the Direct Memory Access (DMA) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and errors functions
  @verbatim     
  ==============================================================================      
                        ##### How to use this driver #####
  ============================================================================== 
  [..]
   (#) Enable and configure the peripheral to be connected to the DMA Channel
       (except for internal SRAM / FLASH memories: no initialization is 
       necessary). Please refer to Reference manual for connection between peripherals
       and DMA requests .

   (#) For a given Channel, program the required configuration through the following parameters:   
       Transfer Direction, Source and Destination data formats, 
       Circular or Normal mode, Channel Priority level, Source and Destination Increment mode, 
       using HAL_DMA_Init() function.

   (#) Use HAL_DMA_GetState() function to return the DMA state and HAL_DMA_GetError() in case of error 
       detection.
                    
   (#) Use HAL_DMA_Abort() function to abort the current transfer
                   
     -@-   In Memory-to-Memory transfer mode, Circular mode is not allowed.
     *** Polling mode IO operation ***
     =================================   
    [..] 
      (+) Use HAL_DMA_Start() to start DMA transfer after the configuration of Source 
          address and destination address and the Length of data to be transferred
      (+) Use HAL_DMA_PollForTransfer() to poll for the end of current transfer, in this  
          case a fixed Timeout can be configured by User depending from his application.

     *** Interrupt mode IO operation ***    
     =================================== 
    [..]
      (+) Configure the DMA interrupt priority using HAL_NVIC_SetPriority()
      (+) Enable the DMA IRQ handler using HAL_NVIC_EnableIRQ() 
      (+) Use HAL_DMA_Start_IT() to start DMA transfer after the configuration of  
          Source address and destination address and the Length of data to be transferred. 
          In this case the DMA interrupt is configured 
      (+) Use HAL_DMA_Channel_IRQHandler() called under DMA_IRQHandler() Interrupt subroutine
      (+) At the end of data transfer HAL_DMA_IRQHandler() function is executed and user can 
          add his own function by customization of function pointer XferCpltCallback and 
          XferErrorCallback (i.e a member of DMA handle structure). 

     *** DMA HAL driver macros list ***
     ============================================= 
     [..]
       Below the list of most used macros in DMA HAL driver.

     [..] 
      (@) You can refer to the DMA HAL driver header file for more useful macros  

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */


/** @defgroup DMA DMA
  * @brief DMA HAL module driver
  * @{
  */

#ifdef HAL_DMA_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup DMA_Private_Functions DMA Private Functions
  * @{
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
static void DMA_CalcBaseAndBitshift(DMA_HandleTypeDef *hdma);
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup DMA_Exported_Functions DMA Exported Functions
  * @{
  */

/** @defgroup DMA_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief   Initialization and de-initialization functions 
 *
@verbatim   
 ===============================================================================
             ##### Initialization and de-initialization functions  #####
 ===============================================================================  
    [..]
    This section provides functions allowing to initialize the DMA Channel source
    and destination addresses, incrementation and data sizes, transfer direction, 
    circular/normal mode selection, memory-to-memory mode selection and Channel priority value.
    [..]
    The HAL_DMA_Init() function follows the DMA configuration procedures as described in
    reference manual.  

@endverbatim
  * @{
  */
  
/**
  * @brief  Initialize the DMA according to the specified
  *         parameters in the DMA_InitTypeDef and initialize the associated handle.
  * @param  hdma Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{ 
  uint32_t tmp = 0U;
  
  /* Check the DMA handle allocation */
  if(NULL == hdma)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
  assert_param(IS_DMA_DIRECTION(hdma->Init.Direction));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(hdma->Init.PeriphInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(hdma->Init.MemInc));
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(hdma->Init.PeriphDataAlignment));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(hdma->Init.MemDataAlignment));
  assert_param(IS_DMA_MODE(hdma->Init.Mode));
  assert_param(IS_DMA_PRIORITY(hdma->Init.Priority));
  
  /* Change DMA peripheral state */
  hdma->State = HAL_DMA_STATE_BUSY;

  /* Get the CR register value */
  tmp = hdma->Instance->CCR;
  
  /* Clear PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR bits */
  tmp &= ((uint32_t)~(DMA_CCR_PL    | DMA_CCR_MSIZE  | DMA_CCR_PSIZE  | \
                      DMA_CCR_MINC  | DMA_CCR_PINC   | DMA_CCR_CIRC   | \
                      DMA_CCR_DIR));
  
  /* Prepare the DMA Channel configuration */
  tmp |=  hdma->Init.Direction        |
          hdma->Init.PeriphInc           | hdma->Init.MemInc           |
          hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
          hdma->Init.Mode                | hdma->Init.Priority;

  /* Write to DMA Channel CR register */
  hdma->Instance->CCR = tmp;  
  
  /* Initialize DmaBaseAddress and ChannelIndex parameters used 
     by HAL_DMA_IRQHandler() and HAL_DMA_PollForTransfer() */
  DMA_CalcBaseAndBitshift(hdma);
  
  /* Initialise the error code */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;

  /* Initialize the DMA state*/
  hdma->State = HAL_DMA_STATE_READY;
  
  /* Allocate lock resource and initialize it */
  hdma->Lock = HAL_UNLOCKED;
  
  return HAL_OK;
}  
  
/**
  * @brief  DeInitialize the DMA peripheral 
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
  /* Check the DMA handle allocation */
  if(NULL == hdma)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));

  /* Disable the selected DMA Channelx */
  hdma->Instance->CCR &= ~DMA_CCR_EN;

  /* Reset DMA Channel control register */
  hdma->Instance->CCR  = 0U;

  /* Reset DMA Channel Number of Data to Transfer register */
  hdma->Instance->CNDTR = 0U;

  /* Reset DMA Channel peripheral address register */
  hdma->Instance->CPAR  = 0U;
  
  /* Reset DMA Channel memory address register */
  hdma->Instance->CMAR = 0U;

/* Get DMA Base Address */  
  DMA_CalcBaseAndBitshift(hdma);

  /* Clear all flags */
  hdma->DmaBaseAddress->IFCR = DMA_FLAG_GL1 << hdma->ChannelIndex;

  /* Clean callbacks */
  hdma->XferCpltCallback = NULL;
  hdma->XferHalfCpltCallback = NULL;
  hdma->XferErrorCallback = NULL;
  hdma->XferAbortCallback = NULL;

  /* Reset the error code */
  hdma->ErrorCode = HAL_DMA_ERROR_NONE;

  /* Reset the DMA state */
  hdma->State = HAL_DMA_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hdma);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group2 Input and Output operation functions 
 *  @brief   I/O operation functions  
 *
@verbatim   
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Configure the source, destination address and data length and Start DMA transfer
      (+) Configure the source, destination address and data length and 
          Start DMA transfer with interrupt
      (+) Abort DMA transfer
      (+) Poll for transfer complete
      (+) Handle DMA interrupt request  

@endverbatim
  * @{
  */

/**
  * @brief  Start the DMA Transfer.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Channel.  
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));
  
  /* Process locked */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
  	/* Change DMA peripheral state */  
  	hdma->State = HAL_DMA_STATE_BUSY;
  	
  	hdma->ErrorCode = HAL_DMA_ERROR_NONE;
  	
  	/* Disable the peripheral */
  	hdma->Instance->CCR &= ~DMA_CCR_EN;  
  	
  	/* Configure the source, destination address and the data length */
  	DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);
  	
  	/* Enable the Peripheral */
  	hdma->Instance->CCR |= DMA_CCR_EN;  
  }
  else
  {
  	/* Process Unlocked */
  	__HAL_UNLOCK(hdma);
  	
  	/* Remain BUSY */
  	status = HAL_BUSY;
  }  

  return status; 
} 

/**
  * @brief  Start the DMA Transfer with interrupt enabled.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Channel.  
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));
  
  /* Process locked */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
  	/* Change DMA peripheral state */  
  	hdma->State = HAL_DMA_STATE_BUSY;
  	
  	hdma->ErrorCode = HAL_DMA_ERROR_NONE;
  	
  	/* Disable the peripheral */
  	hdma->Instance->CCR &= ~DMA_CCR_EN;
  	
  	/* Configure the source, destination address and the data length */  
  	DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);
  	
  	/* Enable the transfer complete, & transfer error interrupts */
  	/* Half transfer interrupt is optional: enable it only if associated callback is available */
    if(NULL != hdma->XferHalfCpltCallback )
    {
      hdma->Instance->CCR |= (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);
    }
  	else
  	{
  		hdma->Instance->CCR |= (DMA_IT_TC | DMA_IT_TE);
  		hdma->Instance->CCR &= ~DMA_IT_HT;
  	}
  	
  	/* Enable the Peripheral */
  	hdma->Instance->CCR |= DMA_CCR_EN;
  }
  else
  {
  	/* Process Unlocked */
    __HAL_UNLOCK(hdma); 
  
    /* Remain BUSY */
    status = HAL_BUSY;
  }     
  
  return status;    
} 

/**
  * @brief  Abort the DMA Transfer.
  * @param  hdma  pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma)
{
  if(hdma->State != HAL_DMA_STATE_BUSY)
  {
    /* no transfer ongoing */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
    
    /* Process Unlocked */
    __HAL_UNLOCK(hdma);
    
    return HAL_ERROR;
  }
  else
  {
    /* Disable DMA IT */
    hdma->Instance->CCR &= ~(DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);
    
    /* Disable the channel */
    hdma->Instance->CCR &= ~DMA_CCR_EN;
    
    /* Clear all flags */
    hdma->DmaBaseAddress->IFCR = (DMA_FLAG_GL1 << hdma->ChannelIndex);
  }
  /* Change the DMA state*/
  hdma->State = HAL_DMA_STATE_READY; 
  
  /* Process Unlocked */
  __HAL_UNLOCK(hdma);
  
  return HAL_OK;
}

/**
  * @brief  Abort the DMA Transfer in Interrupt mode.
  * @param  hdma  pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{  
  HAL_StatusTypeDef status = HAL_OK;
  
  if(HAL_DMA_STATE_BUSY != hdma->State)
  {
    /* no transfer ongoing */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
        
    status = HAL_ERROR;
  }
  else
  { 
  
    /* Disable DMA IT */
    hdma->Instance->CCR &= ~(DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);
    
    /* Disable the channel */
    hdma->Instance->CCR &= ~DMA_CCR_EN;
    
    /* Clear all flags */
    hdma->DmaBaseAddress->IFCR = DMA_FLAG_GL1 << hdma->ChannelIndex;
    
    /* Change the DMA state */
    hdma->State = HAL_DMA_STATE_READY;
    
    /* Process Unlocked */
    __HAL_UNLOCK(hdma);
    
    /* Call User Abort callback */ 
    if(hdma->XferAbortCallback != NULL)
    {
      hdma->XferAbortCallback(hdma);
    } 
  }
  return status;
}

/**
  * @brief  Polling for transfer complete.
  * @param  hdma    pointer to a DMA_HandleTypeDef structure that contains
  *                  the configuration information for the specified DMA Channel.
  * @param  CompleteLevel Specifies the DMA level complete.  
  * @param  Timeout       Timeout duration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t CompleteLevel, uint32_t Timeout)
{
  uint32_t temp;
  uint32_t tickstart = 0U;
  
  if(HAL_DMA_STATE_BUSY != hdma->State)
  {
    /* no transfer ongoing */
    hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
    __HAL_UNLOCK(hdma);
    return HAL_ERROR;
  }
  
  /* Polling mode not supported in circular mode */
  if (RESET != (hdma->Instance->CCR & DMA_CCR_CIRC))
  {
    hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
    return HAL_ERROR;
  }
  
  /* Get the level transfer complete flag */
  if(HAL_DMA_FULL_TRANSFER == CompleteLevel)
  {
    /* Transfer Complete flag */
    temp = DMA_FLAG_TC1 << hdma->ChannelIndex;
  }
  else
  {
    /* Half Transfer Complete flag */
    temp = DMA_FLAG_HT1 << hdma->ChannelIndex;
  }

  /* Get tick */
  tickstart = HAL_GetTick();

  while(RESET == (hdma->DmaBaseAddress->ISR & temp))
  {
    if(RESET != (hdma->DmaBaseAddress->ISR & (DMA_FLAG_TE1 << hdma->ChannelIndex)))
    {      
      /* When a DMA transfer error occurs */
      /* A hardware clear of its EN bits is performed */
      /* Clear all flags */
      hdma->DmaBaseAddress->IFCR = DMA_FLAG_GL1 << hdma->ChannelIndex;
      
      /* Update error code */
      hdma->ErrorCode = HAL_DMA_ERROR_TE;

      /* Change the DMA state */
      hdma->State= HAL_DMA_STATE_READY;       
      
      /* Process Unlocked */
      __HAL_UNLOCK(hdma);
      
      return HAL_ERROR;      
    }      
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        /* Update error code */
        hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;
        
        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hdma);

        return HAL_ERROR;
      }
    }
  }

  if(HAL_DMA_FULL_TRANSFER == CompleteLevel)
  {
    /* Clear the transfer complete flag */
    hdma->DmaBaseAddress->IFCR = DMA_FLAG_TC1 << hdma->ChannelIndex;

    /* The selected Channelx EN bit is cleared (DMA is disabled and 
    all transfers are complete) */
    hdma->State = HAL_DMA_STATE_READY;
  }
  else
  { 
    /* Clear the half transfer complete flag */
    hdma->DmaBaseAddress->IFCR = DMA_FLAG_HT1 << hdma->ChannelIndex;
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hdma);  

  return HAL_OK;
}

/**
  * @brief  Handle DMA interrupt request.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.  
  * @retval None
  */
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
	uint32_t flag_it = hdma->DmaBaseAddress->ISR;
  uint32_t source_it = hdma->Instance->CCR;
          
  /* Half Transfer Complete Interrupt management ******************************/
  if ((RESET != (flag_it & (DMA_FLAG_HT1 << hdma->ChannelIndex))) && (RESET != (source_it & DMA_IT_HT)))
  {
  	/* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
  	if((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
  	{
  		/* Disable the half transfer interrupt */
  		hdma->Instance->CCR &= ~DMA_IT_HT;
  	}
  	
  	/* Clear the half transfer complete flag */
  	hdma->DmaBaseAddress->IFCR = DMA_FLAG_HT1 << hdma->ChannelIndex;
  	
  	/* DMA peripheral state is not updated in Half Transfer */
  	/* State is updated only in Transfer Complete case */
  	
  	if(hdma->XferHalfCpltCallback != NULL)
  	{
  		/* Half transfer callback */
  		hdma->XferHalfCpltCallback(hdma);
  	}
  }
  
  /* Transfer Complete Interrupt management ***********************************/
  else if ((RESET != (flag_it & (DMA_FLAG_TC1 << hdma->ChannelIndex))) && (RESET != (source_it & DMA_IT_TC)))
  {
  	if((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
  	{
  		/* Disable the transfer complete  & transfer error interrupts */
  		/* if the DMA mode is not CIRCULAR */
  		hdma->Instance->CCR &= ~(DMA_IT_TC | DMA_IT_TE);
  		
  		/* Change the DMA state */
  		hdma->State = HAL_DMA_STATE_READY;
  	}
  	
  	/* Clear the transfer complete flag */
  	hdma->DmaBaseAddress->IFCR = DMA_FLAG_TC1 << hdma->ChannelIndex;
  	
  	/* Process Unlocked */
  	__HAL_UNLOCK(hdma);
  	
  	if(hdma->XferCpltCallback != NULL)
  	{
  		/* Transfer complete callback */
  		hdma->XferCpltCallback(hdma);
  	}
  }
  
  /* Transfer Error Interrupt management ***************************************/
  else if (( RESET != (flag_it & (DMA_FLAG_TE1 << hdma->ChannelIndex))) && (RESET != (source_it & DMA_IT_TE)))
  {
  	/* When a DMA transfer error occurs */
    /* A hardware clear of its EN bits is performed */
    /* Then, disable all DMA interrupts */
    hdma->Instance->CCR &= ~(DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);
    
    /* Clear all flags */
    hdma->DmaBaseAddress->IFCR = DMA_FLAG_GL1 << hdma->ChannelIndex;
    
    /* Update error code */
    hdma->ErrorCode = HAL_DMA_ERROR_TE;
    
    /* Change the DMA state */
    hdma->State = HAL_DMA_STATE_READY;    
    
    /* Process Unlocked */
    __HAL_UNLOCK(hdma); 
    
    if(hdma->XferErrorCallback != NULL)
    {
    	/* Transfer error callback */
    	hdma->XferErrorCallback(hdma);
    }
   }
}  

/**
  * @brief  Register callbacks
  * @param  hdma                 pointer to a DMA_HandleTypeDef structure that contains
  *                               the configuration information for the specified DMA Stream.
  * @param  CallbackID           User Callback identifer
  *                               a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @param  pCallback            pointer to private callback function which has pointer to 
  *                               a DMA_HandleTypeDef structure as parameter.
  * @retval HAL status
  */                          
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)( DMA_HandleTypeDef * _hdma))
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Process locked */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
    switch (CallbackID)
    {
     case  HAL_DMA_XFER_CPLT_CB_ID:
           hdma->XferCpltCallback = pCallback;
           break;
       
     case  HAL_DMA_XFER_HALFCPLT_CB_ID:
           hdma->XferHalfCpltCallback = pCallback;
           break;         

     case  HAL_DMA_XFER_ERROR_CB_ID:
           hdma->XferErrorCallback = pCallback;
           break;         
           
     case  HAL_DMA_XFER_ABORT_CB_ID:
           hdma->XferAbortCallback = pCallback;
           break; 
           
     default:
           status = HAL_ERROR;
           break;                                                            
    }
  }
  else
  {
    status = HAL_ERROR;
  } 
  
  /* Release Lock */
  __HAL_UNLOCK(hdma);
  
  return status;
}

/**
  * @brief  UnRegister callbacks
  * @param  hdma                 pointer to a DMA_HandleTypeDef structure that contains
  *                               the configuration information for the specified DMA Stream.
  * @param  CallbackID           User Callback identifer
  *                               a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @retval HAL status
  */              
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

    /* Process locked */
  __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
    switch (CallbackID)
    {
     case  HAL_DMA_XFER_CPLT_CB_ID:
           hdma->XferCpltCallback = NULL;
           break;
       
     case  HAL_DMA_XFER_HALFCPLT_CB_ID:
           hdma->XferHalfCpltCallback = NULL;
           break;         

     case  HAL_DMA_XFER_ERROR_CB_ID:
           hdma->XferErrorCallback = NULL;
           break;         
           
     case  HAL_DMA_XFER_ABORT_CB_ID:
           hdma->XferAbortCallback = NULL;
           break; 
     
    case   HAL_DMA_XFER_ALL_CB_ID:
           hdma->XferCpltCallback = NULL;
           hdma->XferHalfCpltCallback = NULL;
           hdma->XferErrorCallback = NULL;
           hdma->XferAbortCallback = NULL;
           break; 
     
    default:
           status = HAL_ERROR;
           break;                                                            
    }
  }
  else
  {
    status = HAL_ERROR;
  } 
  
  /* Release Lock */
  __HAL_UNLOCK(hdma);
  
  return status;
}

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions_Group3 Peripheral State functions
 *  @brief    Peripheral State functions 
 *
@verbatim   
 ===============================================================================
                    ##### State and Errors functions #####
 ===============================================================================  
    [..]
    This subsection provides functions allowing to
      (+) Check the DMA state
      (+) Get error code

@endverbatim
  * @{
  */  

/**
  * @brief  Returns the DMA state.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Channel.  
  * @retval HAL state
  */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma)
{
  return hdma->State;
}

/**
  * @brief  Return the DMA error code
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Channel.
  * @retval DMA Error Code
  */
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma)
{
  return hdma->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup DMA_Private_Functions
  * @{
  */

/**
  * @brief  Set the DMA Transfer parameters.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Channel.  
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	/* Clear all flags */
  hdma->DmaBaseAddress->IFCR  = (DMA_FLAG_GL1 << hdma->ChannelIndex);
  
  /* Configure DMA Channel data length */
  hdma->Instance->CNDTR = DataLength;
  
  /* Memory to Peripheral */
  if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
  {   
    /* Configure DMA Channel destination address */
    hdma->Instance->CPAR = DstAddress;
    
    /* Configure DMA Channel source address */
    hdma->Instance->CMAR = SrcAddress;
  }
  /* Peripheral to Memory */
  else
  {
    /* Configure DMA Channel source address */
    hdma->Instance->CPAR = SrcAddress;
    
    /* Configure DMA Channel destination address */
    hdma->Instance->CMAR = DstAddress;
  }
}

/**
  * @brief  set the DMA base address and channel index depending on DMA instance
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream. 
  * @retval None
  */
static void DMA_CalcBaseAndBitshift(DMA_HandleTypeDef *hdma)
{
#if defined (DMA2)
  /* calculation of the channel index */
  if ((uint32_t)(hdma->Instance) < (uint32_t)(DMA2_Channel1))
  {
    /* DMA1 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2U;
    hdma->DmaBaseAddress = DMA1;
  }
  else 
  {
    /* DMA2 */
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA2_Channel1) / ((uint32_t)DMA2_Channel2 - (uint32_t)DMA2_Channel1)) << 2U;
    hdma->DmaBaseAddress = DMA2;
  }
#else
  /* calculation of the channel index */
  /* DMA1 */
  hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2U;
  hdma->DmaBaseAddress = DMA1;
#endif
}

/**
  * @}
  */

/**
  * @}
  */
#endif /* HAL_DMA_MODULE_ENABLED */

/**
  * @}
  */
  
  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
