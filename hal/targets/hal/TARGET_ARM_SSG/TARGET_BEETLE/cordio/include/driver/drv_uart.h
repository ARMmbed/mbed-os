/*************************************************************************************************/
/*!
 *  \file   drv_uart.h
 *
 *  \brief  UART driver.
 *
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*************************************************************************************************/

#ifndef __DRV_UART_H
#define __DRV_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
** INCLUDES
***************************************************************************************************/

#include "chip.h"
#include "board.h"

/***************************************************************************************************
** DEFINES
***************************************************************************************************/

/*--------------------------------------------------------------------------------------------------
** ENABLE/DISABLE, ON/OFF DEFINES
**
** DESCRIPTION: For clarity in enable parameters.
--------------------------------------------------------------------------------------------------*/

#ifndef DRV_ENABLE
#define DRV_ENABLE  1
#endif

#ifndef DRV_DISABLE
#define DRV_DISABLE  0
#endif

#ifndef DRV_ON
#define DRV_ON  1
#endif

#ifndef DRV_OFF
#define DRV_OFF  0
#endif

/*--------------------------------------------------------------------------------------------------
** TX/RX DEFINES
**
** DESCRIPTION: For tx/rx argument of driver functions.
--------------------------------------------------------------------------------------------------*/

#define DRV_UART_TX  (1u << 0)
#define DRV_UART_RX  (1u << 1)

/***************************************************************************************************
** TYPES
***************************************************************************************************/

/*--------------------------------------------------------------------------------------------------
** DRV_UART_RX_CALLBACK_t()
**
** DESCRIPTION: Callback for received byte.
**
** PARAMETERS:  c  Received byte
**------------------------------------------------------------------------------------------------*/

typedef void (*DRV_UART_RX_CALLBACK_t)(uint8_t c);

/*--------------------------------------------------------------------------------------------------
** DRV_UART_TX_COMPLETE_t()
**
** DESCRIPTION: Callback for transmit interrupt.
**------------------------------------------------------------------------------------------------*/

typedef void (*DRV_UART_TX_CALLBACK_t)(void);


/*--------------------------------------------------------------------------------------------------
** DRV_UART_BUF_LEN
**
** DESCRIPTION: The maximum length of printed strings.
**------------------------------------------------------------------------------------------------*/
#define DRV_UART_BUF_LEN 128u

/*--------------------------------------------------------------------------------------------------
** struct drv_console
**
** DESCRIPTION: Access structure of driver.
--------------------------------------------------------------------------------------------------*/
struct drv_uart {
    /*----------------------------------------------------------------------------------------------
    ** Initialize()
    **
    ** DESCRIPTION: Initialize UART.
    **
    ** PARAMETERS:  baud   Baud rate
    **              rx_tx  Indicates allowed modes of driver (DRV_UART_RX &/| DRV_UART_TX).
    **--------------------------------------------------------------------------------------------*/
    void (*Initialize)(uint32_t baud, uint32_t rx_tx);

    /*----------------------------------------------------------------------------------------------
    ** Sleep()
    **
    ** DESCRIPTION: Allow UART settings to be saved before processor enters deep sleep.
    **--------------------------------------------------------------------------------------------*/
    void (*Sleep)(void);

    /*----------------------------------------------------------------------------------------------
    ** Wake()
    **
    ** DESCRIPTION: Allow UART settings to be restored after processor exits deep sleep.
    **--------------------------------------------------------------------------------------------*/
    void (*Wake)(void);

    /*----------------------------------------------------------------------------------------------
    ** Receive()
    **
    ** DESCRIPTION: Receive data.
    **
    ** PARAMETERS:  buf  Pointer to buffer that will receive data
    **              len  Number of data bytes to data
    **
    ** RETURNS:     Number of bytes receuved
    **--------------------------------------------------------------------------------------------*/
    int32_t (*Receive)(uint8_t *buf, uint32_t len);

   /*----------------------------------------------------------------------------------------------
    ** Transmit()
    **
    ** DESCRIPTION: Transmit data.
    **
    ** PARAMETERS:  buf  Pointer to buffer of data to transmit
    **              len  Number of data bytes to transmit
    **
    ** RETURNS:     Number of bytes transmitted
    **--------------------------------------------------------------------------------------------*/
    int32_t (*Transmit)(const uint8_t *buf, uint32_t len);

    /*----------------------------------------------------------------------------------------------
    ** SetReceiveHandler()
    **
    ** DESCRIPTION: Set receive handler.
    **
    ** PARAMETERS:  cb  Pointer to callbak
    **--------------------------------------------------------------------------------------------*/
    void (*SetReceiveHandler)(DRV_UART_RX_CALLBACK_t cb);

    /*----------------------------------------------------------------------------------------------
    ** SetTransmitHandler()
    **
    ** DESCRIPTION: Set transmit handler.
    **
    ** PARAMETERS:  cb  Pointer to callbak
    **--------------------------------------------------------------------------------------------*/
    void (*SetTransmitHandler)(DRV_UART_TX_CALLBACK_t cb);

    /*----------------------------------------------------------------------------------------------
    ** EnableInterrupt()
    **
    ** DESCRIPTION: Enable or disable an interrupt.
    **
    ** PARAMETERS:  rx_tx   Select between receive (DRV_UART_RX) or transmit (DRV_UART_TX) interrupt.
    **              enable  Indicates whether interrupt should be enable (not 0) or disabled (0).
    **--------------------------------------------------------------------------------------------*/
    void (*EnableInterrupt)(uint32_t rx_tx, int32_t enable);
};

#ifdef __cplusplus
}
#endif

#endif /* __DRV_UART_H */
