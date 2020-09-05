/*************************************************************************************************/
/*!
 *  \file   wsf_bufio.h
 *
 *  \brief  Buffer I/O service.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef WSF_BUF_IO_H
#define WSF_BUF_IO_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_BUF_IO_API
 *  \{ */


/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Callback Function Datatypes
**************************************************************************************************/

/*! \brief    Buffer IO UART Rx callback. */
typedef void (*WsfBufIoUartRxCback_t)(uint8_t rxByte);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the platform UART.
 *
 *  \param  pBuf    Tx Buffer pointer.
 *  \param  size    Length of buffer.
 *
 *  \return memory used.
 */
/*************************************************************************************************/
uint32_t WsfBufIoUartInit(void *pBuf, uint32_t size);

/*************************************************************************************************/
/*!
 *  \brief     Register the platform UART RX callback.
 *
 *  \param[in] Callback function for UART RX.
 */
/*************************************************************************************************/
void WsfBufIoUartRegister(WsfBufIoUartRxCback_t rxCback);

/*************************************************************************************************/
/*!
 *  \brief  Transmit buffer on platform UART.
 *
 *  \param  pBuf    Buffer to transmit.
 *  \param  len     Length of buffer in octets.
 */
/*************************************************************************************************/
bool_t WsfBufIoWrite(const      uint8_t *pBuf, uint32_t len);

/*! \} */    /* WSF_BUF_IO_API */

#ifdef __cplusplus
};
#endif

#endif /* WSF_BUF_IO_H */
