/*******************************************************************************************************************************************************
 * Copyright ¨Ï 2016 <WIZnet Co.,Ltd.> 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the ¡°Software¡±), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED ¡°AS IS¡±, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************************************************************************************/
/**
  ******************************************************************************
  * @file    W7500x_stdPeriph_Driver/src/W7500x_uart.c    
  * @author  IOP Team
  * @version v1.0.0
  * @date    01-May-2015
  * @brief   This file contains all the functions prototypes for the uart 
  *          		firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "W7500x_uart.h"

/** @addtogroup W7500x_Periph_Driver
  * @{
  */

/** @defgroup UART
  * @brief UART driver modules
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup UART_Private_Functions
  * @{
  */ 

/**
	*	@brief		Fills each UART_InitStruct member with its default value.
	*	@param	UART_StructInit:	pointer to a UART_InitTypeDef	structure	which will
	*								be initialized.
	*	@retval		None
	*/
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
  /* UART_InitStruct members default value */
  UART_InitStruct->UART_BaudRate = 115200;
  UART_InitStruct->UART_WordLength = UART_WordLength_8b ;
  UART_InitStruct->UART_StopBits = UART_StopBits_1;
  UART_InitStruct->UART_Parity = UART_Parity_No ;
  UART_InitStruct->UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
  UART_InitStruct->UART_HardwareFlowControl = UART_HardwareFlowControl_None ;
}

/*	Function check	*/
/* This function is checking now. Update or remove maybe.	*/
//Why be????
void UART_DeInit(UART_TypeDef *UARTx)
{
    
}

/**
	*	@brief		Initializes the UART peripheral according to the specified
	*								parameters in the UART_InitStruct .
	*	@param	UARTx: where x can be 1, 2 or 3
	*	@param	UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
	*								the configuration information for the specified UART peripheral.
	*	@retval		None
	*/
	
uint32_t UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef* UART_InitStruct)
{

	float baud_divisor;
	uint32_t tmpreg=0x00, uartclock=0x00;
	uint32_t integer_baud = 0x00, fractional_baud = 0x00;

	/*	Check the parameters	*/
	assert_param(IS_UART_01_PERIPH(UARTx));
	assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLength));
	assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
	assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
	assert_param(IS_UART_HARDWARE_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
	assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));

	/*	Write to UART CR */
  UARTx->CR &= ~(UART_CR_UARTEN); 

	/*	Set baudrate	*/
	/* CRG_UARTCLK_SSR_RCLK: Set UART Clock using internal Oscilator ( 8MHz ) */
	/* CRG_UARTCLK_SSR_OCLK: Set UART Clock using external Oscilator	*/
	CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_RCLK;  	// Set UART Clock using internal Oscilator ( 8MHz )
  //CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_OCLK;  // Set UART Clock using external Oscilator

	/*	Set uartclock	*/
	uartclock =  (8000000UL) /  (1 << CRG->UARTCLK_PVSR);
	
	/*-----------------------------	UARTx IBRD and FBRD	Configuration ------------------------------*/
  baud_divisor = ((float)uartclock / (16 * UART_InitStruct->UART_BaudRate));
	integer_baud = (uint32_t)baud_divisor;
  fractional_baud = (uint32_t)((baud_divisor - integer_baud) * 64 + 0.5);

  UARTx->IBRD = integer_baud;
  UARTx->FBRD = fractional_baud;


    tmpreg = UARTx->LCR_H;
    tmpreg &= ~(0x00EE);
    tmpreg |= (UART_InitStruct->UART_WordLength | UART_InitStruct->UART_StopBits | UART_InitStruct->UART_Parity);
    UARTx->LCR_H = tmpreg;

    tmpreg = UARTx->CR;
    tmpreg &= ~(UART_CR_CTSEn | UART_CR_RTSEn | UART_CR_RXE | UART_CR_TXE | UART_CR_UARTEN);
    tmpreg |= (UART_InitStruct->UART_Mode | UART_InitStruct->UART_HardwareFlowControl);
    UARTx->CR = tmpreg;

	/*	Debug	*/
	/*	UARTx->LCR_H |=0x10;	*/
	/*	UARTx->IFLS | = 0x00;		*/

    UARTx->CR |= UART_CR_UARTEN;

    return 0;
}

/**
	*	@brief		UART_FIFO_Enable
	*	@param	case rx_fifo_level=0: 1/8 full, case rx_fifo_level=1: 1/4 full, case rx_fifo_level=2: 1/2 full,
	*								case rx_fifo_level=3: 3/4 full, case rx_fifo_level=4: 7/8 full
	*	@param	case tx_fifo_level=0: 1/8 full, case tx_fifo_level=1: 1/4 full, case tx_fifo_level=2: 1/2 full,
	*								case tx_fifo_level=3: 3/4 full, case tx_fifo_level=4: 7/8 full
	*	@retval		None
	*/

void UART_FIFO_Enable(UART_TypeDef *UARTx, uint16_t rx_fifo_level, uint16_t tx_fifo_level)
{
    UARTx->LCR_H |= UART_LCR_H_FEN;
    UARTx->IFLS = (UART_IFLS_RXIFLSEL(rx_fifo_level) | UART_IFLS_TXIFLSEL(tx_fifo_level));
}

/**
	*	@brief		UART_FIFO_Disable
	*	@param	UARTx: Define structure at "W7500x.h"
	*	@retval		None
	*/
void UART_FIFO_Disable(UART_TypeDef *UARTx)
{
    UARTx->LCR_H &= ~(UART_LCR_H_FEN);
}

/**
	*	@brief		UART_SendData
	*	@param	UARTx: Define structure at "W7500x.h"
	*	@param	Data:	Unsigned short value
	*	@retval		None
	*/
void UART_SendData(UART_TypeDef* UARTx, uint16_t Data)
{
	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    UARTx->DR = Data;
}

/**
	*	@brief		UART_ReceiveData
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@retval		None
	*/
uint16_t UART_ReceiveData(UART_TypeDef* UARTx)
{
	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    return (uint16_t)(UARTx->DR);
}

/**
	*	@brief		UART_SendBreak
	*	@param	UARTx: Define structure at "W7500x.h"
	*	@retval		None
	*/
void UART_SendBreak(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_01_PERIPH(UARTx));

    UARTx->LCR_H |= UART_LCR_H_BRK; 
}

/**
	*	@brief		UART_GetRecvStatus
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_RECV_STATUS
	*	@retval		bitstatus
	*/
FlagStatus UART_GetRecvStatus(UART_TypeDef* UARTx, uint16_t UART_RECV_STATUS)
{
    FlagStatus bitstatus = RESET;
	
	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_RECV_STATUS(UART_RECV_STATUS));

    if( (UARTx->STATUS.RSR & UART_RECV_STATUS) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
	*	@brief		UART_ClearRecvStatus
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_RECV_STATUS	
	*	@retval		None
	*/
void UART_ClearRecvStatus(UART_TypeDef* UARTx, uint16_t UART_RECV_STATUS)
{
	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_RECV_STATUS(UART_RECV_STATUS));

	/* Set STATUS.ECR*/
    UARTx->STATUS.ECR = (uint16_t)UART_RECV_STATUS;
}

/**
	*	@brief		UART_GetFlagStatus
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_FLAG	
	*	@retval		bitstatus
	*/
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{
    FlagStatus bitstatus = RESET;

	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_FLAG(UART_FLAG));

    if ((UARTx->FR & UART_FLAG) != (uint16_t)RESET)
    {
			/* bitstatus setting to SET*/
        bitstatus = SET;    
    }
    else
    {
			/*bitstatus setting to RESET*/
        bitstatus = RESET;
    }
 
    return bitstatus;
    
}

/*	Function check	*/
/* This function is checking now. Update or remove maybe.	*/
//Why be?

void UART_ClearFlag(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{

}


/**
	*	@brief		UART_ITConfig
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_IT
	*	@param	NewState
	*	@retval		None
	*/
void UART_ITConfig(UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
		/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_IT_FLAG(UART_IT));

    if ( NewState != DISABLE )
    {
			/*Set the IMSC*/
        UARTx->IMSC |= UART_IT;
    }
    else
    {
			/*Set the ICR*/
        UARTx->ICR |= UART_IT;
    }
}

/**
	*	@brief		UART_GetITStatus
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_IT
	*	@param	bitstatus
	*	@retval		None
	*/
ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    ITStatus bitstatus = RESET;
	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_IT_FLAG(UART_IT));

    if ((UARTx->MIS & UART_IT) != (uint16_t)RESET)
    {
			/*Set bitstatus = SET */
        bitstatus = SET;
    }
    else
    {
			/*Set bitstatus = RESET */
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
	*	@brief		UART_ITPendingBit
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_IT
	*	@retval		None
	*/
void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint16_t UART_IT)
{
	/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_IT_FLAG(UART_IT));

	/* Set ICR */
    UARTx->ICR |= UART_IT;
}

/**
	*	@brief		UART_DMA_Config
	*	@param	UARTx:	Define structure at "W7500x.h"
	*	@param	UART_DMA_CONTROL
	*	@retval		None
	*/
void UART_DMA_Config(UART_TypeDef* UARTx, uint16_t UART_DMA_CONTROL)
{
		/*	Check the parameters	*/
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_DMA_CONTROL(UART_DMA_CONTROL));
    
	/*Set DMACR	*/
     UARTx->DMACR |= UART_DMA_CONTROL;
}

/*	Function check	*/
/* This function is checking now. Update or remove maybe.	*/
//Why be?
void S_UART_DeInit()
{

}

/**
	*	@brief		S_UART_Init
	*	@param	baud 
	*	@retval		None
	*/
uint32_t S_UART_Init(uint32_t baud)
{
    uint32_t tmpreg=0x00;

    S_UART_SetBaud(baud);

	/*	Set temp register for UART2 CTRL */
    tmpreg = UART2->CTRL;
    tmpreg &= ~(S_UART_CTRL_RX_EN | S_UART_CTRL_TX_EN);
    tmpreg |= (S_UART_CTRL_RX_EN | S_UART_CTRL_TX_EN);
	
	/*	Write to UART2 CTRL	*/
    UART2->CTRL = tmpreg;

    return 0;
}

/**
	*	@brief		S_UART_SetBaud
	*	@param	baud
	*	@retval		None
	*/
void S_UART_SetBaud(uint32_t baud)
{
    uint32_t uartclock = 0x00, integer_baud = 0x00;
	
		/*	Check the parameters	*/
    assert_param(IS_UART_MODE(S_UART_InitStruct->UART_Mode));

    if(CRG->FCLK_SSR == CRG_FCLK_SSR_RCLK)
    {
			/* Set uartclock: INTERN_XTAL */
        uartclock = INTERN_XTAL;  
    }
    else if(CRG->FCLK_SSR == CRG_FCLK_SSR_OCLK)
    {
			/* Set uartclock: EXTERN_XTAL */
        uartclock = EXTERN_XTAL;
    }
    else
    {
			/* Set uartclock: GetSystemClock */
        uartclock = GetSystemClock();
    }

		/* Set (Calculate) integer_baud */
    integer_baud = (uint32_t)(uartclock / baud);
		/* Write UART2 BAUDDIV */
    UART2->BAUDDIV = integer_baud;
}


/**
	*	@brief		S_UART_SendData
	*	@param	Data
	*	@retval		None
	*/
void S_UART_SendData(uint16_t Data)
{
    while(UART2->STATE & S_UART_STATE_TX_BUF_FULL);
		/* Write UART2 DATA */
    UART2->DATA = Data;
}


/**
	*	@brief		S_UART_ReceiveData
	*	@retval		UART2 DATA
	*/
uint16_t S_UART_ReceiveData()
{
    return (uint16_t)(UART2->DATA);
}


/**
	*	@brief		S_UART_GetFlagStatus
	*	@param	UART_STATE
	*	@retval		bitstatus
	*/
FlagStatus S_UART_GetFlagStatus(uint16_t S_UART_STATE)
{
    FlagStatus bitstatus = RESET;

    assert_param(IS_S_UART_STATE(S_UART_STATE));

    if ((UART2->STATE & S_UART_STATE) != (FlagStatus)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}


/**
	*	@brief		S_UART_SetCTRL
	*	@param	S_UART_CTRL
	*	@param	NewState
	*	@retval		None
	*/
void S_UART_SetCTRL(uint16_t S_UART_CTRL, FunctionalState NewState)
{
    if ( NewState != DISABLE )
    {
        UART2->CTRL |= S_UART_CTRL;
    }
    else
    {
        UART2->CTRL &= ~(S_UART_CTRL);
    }
}


/**
	*	@brief		S_UART_ITConfig
	*	@param	S_UART_CTRL
	*	@param	NewState
	*	@retval		None
	*/
void S_UART_ITConfig(uint16_t S_UART_CTRL, FunctionalState NewState)
{
		/*	Check the parameters	*/	
    assert_param(IS_S_UART_CTRL_FLAG(S_UART_CTRL));

    if ( NewState != DISABLE )
    {
			/* Set UART2 CTRL: S_UART_CTRL */
        UART2->CTRL |= S_UART_CTRL;
    }
    else
    {
			/*Set UART2 CTRL: ~S_UART_CTRL */
        UART2->CTRL &= ~(S_UART_CTRL);
    }
}


/**
	*	@brief		S_UART_GetTStatus
	*	@param	S_UART_INTSTATUS
	*	@retval		bitstatus
	*/
ITStatus S_UART_GetITStatus(uint16_t S_UART_INTSTATUS)
{
    ITStatus bitstatus = RESET;
	
		/*	Check the parameters	*/
    assert_param(IS_S_UART_INTSATUS(S_UART_INTSTATUS));

    if ((UART2->INT.STATUS & (S_UART_INTSTATUS)) != (uint16_t) RESET)
    {
			/* Set bitstatus: SET */
        bitstatus = SET;
    }
    else
    {
			/* Set bitstatus: RESET */
        bitstatus = RESET;
    }

    return bitstatus;
}


/**
	*	@brief		S_UART_ClearITPendingBit
	*	@param	S_UART_INTSTATUS
	*	@retval		None
	*/
void S_UART_ClearITPendingBit(uint16_t S_UART_INTSTATUS)
{
	/*	Check the parameters	*/
    assert_param(IS_S_UART_INTSATUS(S_UART_INTSTATUS));
	/* Write UART2 INT.CLEAR: S_UART_INSTATUS */
    UART2->INT.CLEAR |= (S_UART_INTSTATUS);
}



/**************************************************/
// It will be moved to application board's driver */
/**************************************************/


uint8_t UartPutc(UART_TypeDef* UARTx, uint8_t ch)
{
    UART_SendData(UARTx,ch);

    while(UARTx->FR & UART_FR_BUSY);

    return (ch);
}



void UartPuts(UART_TypeDef* UARTx, uint8_t *str)
{
    uint8_t ch;

    do{
        ch = *str;
        if(ch != (uint8_t)0x0)
        {
            UartPutc(UARTx, ch);
        }
        *str++;
    }while(ch != 0);
}


uint8_t UartGetc(UART_TypeDef* UARTx)
{
    while(UARTx->FR & UART_FR_RXFE);

    return (UARTx->DR & 0xFF);
}


uint8_t S_UartPutc(uint8_t ch)
{
    S_UART_SendData(ch);

    return (ch);
}



void S_UartPuts(uint8_t *str)
{
    uint8_t ch;

    do{
        ch = *str;
        if(ch != (uint8_t)0x0)
        {
            S_UART_SendData(ch);
        }
        *str++;
    }while(ch != 0);
}


uint8_t S_UartGetc()
{
    while( (UART2->STATE & S_UART_STATE_RX_BUF_FULL) == 0 ); 
    return (uint8_t)S_UART_ReceiveData();
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
