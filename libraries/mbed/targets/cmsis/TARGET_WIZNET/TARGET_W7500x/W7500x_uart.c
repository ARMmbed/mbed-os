/**
  ******************************************************************************
  * @file    W7500x_uart.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
 *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "W7500x.h"

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

void UART_DeInit(UART_TypeDef *UARTx)
{
    
}

uint32_t UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef* UART_InitStruct)
{
	float baud_divisor;
    uint32_t tmpreg=0x00, uartclock=0x00;
    uint32_t integer_baud = 0x00, fractional_baud = 0x00;

	assert_param(IS_UART_01_PERIPH(UARTx));
	assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLength));
	assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
	assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
	assert_param(IS_UART_HARDWARE_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
	assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));


    UARTx->CR &= ~(UART_CR_UARTEN); 

    // Set baudrate
    CRG->UARTCLK_SSR = CRG_UARTCLK_SSR_RCLK;  // Set UART Clock using internal Oscilator ( 8MHz )
    uartclock =  (8000000UL) /  (1 << CRG->UARTCLK_PVSR);

    baud_divisor = ((float)uartclock / (16 * UART_InitStruct->UART_BaudRate));
    integer_baud = (uint32_t)baud_divisor;
    fractional_baud = (uint32_t)((baud_divisor - integer_baud) * 64 + 0.5);

    UARTx->IBRD = integer_baud;
    UARTx->FBRD = fractional_baud;


    tmpreg = UARTx->LCR_H;
    tmpreg &= ~(0x00EE);
    tmpreg |= (UART_InitStruct->UART_WordLength | UART_InitStruct->UART_StopBits | UART_InitStruct->UART_Parity);
    UARTx->LCR_H |= tmpreg;

    tmpreg = UARTx->CR;
    tmpreg &= ~(UART_CR_CTSEn | UART_CR_RTSEn | UART_CR_RXE | UART_CR_TXE | UART_CR_UARTEN);
    tmpreg |= (UART_InitStruct->UART_Mode | UART_InitStruct->UART_HardwareFlowControl);
    UARTx->CR |= tmpreg;

    UARTx->CR |= UART_CR_UARTEN;

    return 0;
}


void UART_SendData(UART_TypeDef* UARTx, uint16_t Data)
{
    assert_param(IS_UART_01_PERIPH(UARTx));

    UARTx->DR = Data;
}


uint16_t UART_ReceiveData(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_01_PERIPH(UARTx));

    return (uint16_t)(UARTx->DR);
}


void UART_SendBreak(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_01_PERIPH(UARTx));

    UARTx->LCR_H |= UART_LCR_H_BRK; 
}


FlagStatus UART_GetRecvStatus(UART_TypeDef* UARTx, uint16_t UART_RECV_STATUS)
{
    FlagStatus bitstatus = RESET;

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


void UART_ClearRecvStatus(UART_TypeDef* UARTx, uint16_t UART_RECV_STATUS)
{
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_RECV_STATUS(UART_RECV_STATUS));

    UARTx->STATUS.ECR = (uint16_t)UART_RECV_STATUS;
}


FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{
    FlagStatus bitstatus = RESET;

    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_FLAG(UART_FLAG));

    if ((UARTx->FR & UART_FLAG) != (uint16_t)RESET)
    {
        bitstatus = SET;    
    }
    else
    {
        bitstatus = RESET;
    }
 
    return bitstatus;
    
}

/*
void UART_ClearFlag(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{

}
*/

void UART_ITConfig(UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_IT_FLAG(UART_IT));

    if ( NewState != DISABLE )
    {
        UARTx->IMSC |= UART_IT;
    }
    else
    {
        UARTx->ICR |= UART_IT;
    }
}


ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    ITStatus bitstatus = RESET;

    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_IT_FLAG(UART_IT));

    if ((UARTx->MIS & UART_IT) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    assert_param(IS_UART_01_PERIPH(UARTx));
    assert_param(IS_UART_IT_FLAG(UART_IT));

    UARTx->ICR |= UART_IT;
}


void S_UART_DeInit()
{

}

uint32_t S_UART_Init(uint32_t baud)
{
    uint32_t tmpreg=0x00;
    uint32_t uartclock = 0x00, integer_baud = 0x00;

    assert_param(IS_UART_MODE(S_UART_InitStruct->UART_Mode));

    if(CRG->FCLK_SSR == CRG_FCLK_SSR_RCLK)
    {
        uartclock = INTERN_XTAL;  
    }
    else if(CRG->FCLK_SSR == CRG_FCLK_SSR_OCLK)
    {
        uartclock = EXTERN_XTAL;
    }
    else
    {
        uartclock = GetSystemClock();
    }

    integer_baud = (uint32_t)(uartclock / baud);
    UART2->BAUDDIV = integer_baud;

    tmpreg = UART2->CTRL;
    tmpreg &= ~(S_UART_CTRL_RX_EN | S_UART_CTRL_TX_EN);
    tmpreg |= (S_UART_CTRL_RX_EN | S_UART_CTRL_TX_EN);
    UART2->CTRL = tmpreg;

    return 0;
}

void S_UART_SendData(uint16_t Data)
{
    while(UART2->STATE & S_UART_STATE_TX_BUF_FULL); 
    UART2->DATA = Data;
}

uint16_t S_UART_ReceiveData()
{
    return (uint16_t)(UART2->DATA);
}


FlagStatus S_UART_GetFlagStatus(uint16_t S_UART_FLAG)
{
    FlagStatus bitstatus = RESET;

    assert_param(IS_S_UART_FLAG(S_UART_FLAG));

    if ((UART2->STATE & S_UART_FLAG) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}


void S_UART_ITConfig(uint16_t S_UART_IT, FunctionalState NewState)
{
    assert_param(IS_S_UART_IT_FLAG(S_UART_IT));

    if ( NewState != DISABLE )
    {
        UART2->CTRL |= S_UART_IT;
    }
    else
    {
        UART2->CTRL &= ~(S_UART_IT);
    }
}

ITStatus S_UART_GetITStatus(uint16_t S_UART_IT)
{
    ITStatus bitstatus = RESET;

    assert_param(IS_S_UART_IT_FLAG(S_UART_IT));

    if ((UART2->INT.STATUS & (S_UART_IT >> 2)) != (uint16_t) RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

void S_UART_ClearITPendingBit(uint16_t S_UART_IT)
{
    assert_param(IS_S_UART_IT_FLAG(S_UART_IT));

    UART2->INT.CLEAR |= (S_UART_IT >> 2);
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


