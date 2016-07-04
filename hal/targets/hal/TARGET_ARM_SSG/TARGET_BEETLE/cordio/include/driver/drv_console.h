/*************************************************************************************************/
/*!
 *  \file   drv_console.h
 *
 *  \brief  Upper-layer UART driver.
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

#ifndef __DRV_CONSOLE_H
#define __DRV_CONSOLE_H

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

#define DRV_CONSOLE_TX  (1u << 0)
#define DRV_CONSOLE_RX  (1u << 1)

/*--------------------------------------------------------------------------------------------------
** DRV_CONSOLE_BUF_LEN
**
** DESCRIPTION: The maximum length of printed strings.
**------------------------------------------------------------------------------------------------*/
#define DRV_CONSOLE_BUF_LEN 128u

/***************************************************************************************************
** TYPES
***************************************************************************************************/

/*----------------------------------------------------------------------------------------------
** DRV_CONSOLE_RX_CALLBACK_t()
**
** DESCRIPTION: Callback for received byte.
**
** PARAMETERS:  c  Received byte
**--------------------------------------------------------------------------------------------*/

typedef void (*DRV_CONSOLE_RX_CALLBACK_t)(uint8_t c);

/*----------------------------------------------------------------------------------------------
** DRV_CONSOLE_TX_COMPLETE_CALLBACK_t()
**
** DESCRIPTION: Callback for transmission completion.
**
** PARAMETERS:  buf  Pointer to buffer that was transmitted
**--------------------------------------------------------------------------------------------*/

typedef void (*DRV_CONSOLE_TX_COMPLETE_CALLBACK_t)(const uint8_t *buf);

/*--------------------------------------------------------------------------------------------------
** struct drv_console
**
** DESCRIPTION: Access structure of driver.
--------------------------------------------------------------------------------------------------*/

struct drv_console {
    /*----------------------------------------------------------------------------------------------
    ** Initialize()
    **
    ** DESCRIPTION: Initialize console.
    **
    ** PARAMETERS:  baud   Baud rate
    **              rx_tx  Indicates allowed modes of driver (DRV_CONSOLE_RX &/| DRV_CONSOLE_TX).
    **--------------------------------------------------------------------------------------------*/
    void (*Initialize)(uint32_t baud, uint32_t rx_tx);

    /*----------------------------------------------------------------------------------------------
    ** Receive()
    **
    ** DESCRIPTION: Receive data.
    **
    ** PARAMETERS:  buf  Pointer to buffer that will receive data
    **              len  Number of data bytes to data
    **
    ** RETURNS:     Number of bytes received
    **--------------------------------------------------------------------------------------------*/
    int32_t (*Receive)(uint8_t *buf, uint32_t len);

    /*----------------------------------------------------------------------------------------------
    ** ReceiveAsyncStart()
    **
    ** DESCRIPTION: Start receiving data asynchronously.
    **
    ** PARAMETERS:  cb  Callback for receive events
    **
    ** RETURNS:     0 or error (if not zero)
    **--------------------------------------------------------------------------------------------*/
    int32_t (*ReceiveAsyncStart)(DRV_UART_RX_CALLBACK_t cb);

    /*----------------------------------------------------------------------------------------------
    ** ReceiveAsyncStart()
    **
    ** DESCRIPTION: Stop receiving data asynchronously.
    **--------------------------------------------------------------------------------------------*/
    void (*ReceiveAsyncStop)(void);

    /*----------------------------------------------------------------------------------------------
    ** Transmit()
    **
    ** DESCRIPTION: Transmit buffer synchronously, waiting for any active transmission to finish.
    **
    ** PARAMETERS:  buf    Pointer to buffer of data to transmit
    **              len    Number of data bytes to transmit
    **
    ** RETURNS:     Number of bytes transmitted
    **--------------------------------------------------------------------------------------------*/
    int32_t (*Transmit)(const uint8_t *buf, uint32_t len);

    /*----------------------------------------------------------------------------------------------
    ** TransmitAsync()
    **
    ** DESCRIPTION: Transmit buffer asynchronously, waiting for any active transmission to finish.
    **
    ** PARAMETERS:  buf  Pointer to buffer of data to transmit
    **              len  Number of data bytes to transmit
    **              cb   Pointer to completion callback
    **
    ** RETURNS:     0 or error (if not zero)
    **--------------------------------------------------------------------------------------------*/
    int32_t (*TransmitAsync)(const uint8_t *buf, uint32_t len, DRV_CONSOLE_TX_COMPLETE_CALLBACK_t cb);

    /*----------------------------------------------------------------------------------------------
    ** GetUART()
    **
    ** DESCRIPTION: Get UART associated with this console.
    **
    ** RETURNS:     Pointer to UART
    **--------------------------------------------------------------------------------------------*/
    const struct drv_uart *(*GetUART)(void);
};

#ifdef __cplusplus
}
#endif

#endif /* __DRV_CONSOLE_H */
