/***************************************************************************//**
* \file cy_ble_hal_int.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Interrupt Service Routine for the
*  HAL section of the BLE component
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cy_ble_event_handler.h"
#include "cy_ble_hal_pvt.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (CY_BLE_HOST_CONTR_CORE)

#if (CY_BLE_CONFIG_STACK_CONTR_CORE)

/*******************************************************************************
* Function Name: Cy_BLE_BlessInterrupt
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the BLE sub-system.
*
* \return
*  None
*
*******************************************************************************/
void Cy_BLE_BlessInterrupt(void)
{
    /* Call stack manager bless function handler */
    Cy_BLE_HAL_BlessInterruptHandler();
    /* Host stack takes care of clearing interrupts */
}

#endif  /* (CY_BLE_CONFIG_STACK_CONTR_CORE) */

#if (CY_BLE_STACK_MODE_HOST_UART)


/*******************************************************************************
* Function Name: Cy_BLE_HAL_HOST_UART_Interrupt
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the UART.
*
* \return
*  None
*
*******************************************************************************/
void Cy_BLE_HAL_HOST_UART_Interrupt(void)
{
    uint8_t uartTxStatus = CY_BLE_INTR_TX_SUCCESS;
    uint32_t srcInterrupt = 0u;

    uint8_t lenRxFifo;
    uint8_t srcCount;
    uint8_t uartRxStatus = CY_BLE_INTR_RX_SUCCESS;
    uint8_t receivedData;

    /* Evaluate RX interrupt event */
    srcInterrupt = Cy_SCB_GetRxInterruptStatusMasked(cy_ble_configPtr->uartHostHw);

    if(0u != (srcInterrupt & CY_SCB_RX_INTR_NOT_EMPTY))
    {
        if(0u != (srcInterrupt & CY_SCB_RX_INTR_UART_PARITY_ERROR))
        {
            uartRxStatus |= CY_BLE_INTR_RX_PARITY_ERROR;
        }
        if(0u != (srcInterrupt & CY_SCB_RX_INTR_UART_FRAME_ERROR))
        {
            uartRxStatus |= CY_BLE_INTR_RX_FRAME_ERROR;
        }
        if(0u != (srcInterrupt & CY_SCB_RX_INTR_OVERFLOW))
        {
            uartRxStatus |= CY_BLE_INTR_RX_OVERFLOW;
        }
        if(uartRxStatus == CY_BLE_INTR_RX_SUCCESS)
        {
            lenRxFifo = (uint8_t)Cy_SCB_GetNumInRxFifo(cy_ble_configPtr->uartHostHw);
            for(srcCount = 0u; srcCount < lenRxFifo; srcCount++)
            {
                receivedData = (uint8_t)Cy_SCB_ReadRxFifo(cy_ble_configPtr->uartHostHw);
                Cy_BLE_HAL_HciTransportHandler((char8)receivedData, 1u);
            }
        }
        else
        {
            Cy_SCB_ClearRxFifo(cy_ble_configPtr->uartHostHw);
        }
        Cy_SCB_ClearRxInterrupt(cy_ble_configPtr->uartHostHw, srcInterrupt);
    }
    else
    {
        /* No RX interrupt. Do nothing. */
    }

    /* Evaluate TX interrupt event in sequence */
    srcInterrupt = Cy_SCB_GetTxInterruptStatusMasked(cy_ble_configPtr->uartHostHw);

    /* Stack manager TX UART complete */
    if(0u != (srcInterrupt & CY_SCB_TX_INTR_UART_DONE))
    {
        if(0u != (srcInterrupt & CY_SCB_TX_INTR_OVERFLOW))
        {
            /*Stack manager TX UART error */
            uartTxStatus |= CY_BLE_INTR_TX_OVERFLOW;
        }
        Cy_SCB_ClearTxInterrupt(cy_ble_configPtr->uartHostHw, srcInterrupt);
    }
    else
    {
        /* No TX interrupt. Do nothing. */
    }
}
#endif /* (CY_BLE_STACK_MODE_HOST_UART) */

#endif /* (CY_BLE_HOST_CONTR_CORE) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
