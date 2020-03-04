/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
* SPDX-License-Identifier: BSD-3-Clause
*
* \file XcvrSpi.h
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __XCVR_SPI_H__
#define __XCVR_SPI_H__


/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 * Note that it is not a good practice to include header files into header   *
 * files, so use this section only if there is no other better solution.     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/


/*****************************************************************************
 *                             PUBLIC MACROS                                 *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define gXcvrSpiInstance_c              0

/*****************************************************************************
 *                            PUBLIC FUNCTIONS                               *
 *---------------------------------------------------------------------------*
 * Add to this section all the global functions prototype preceded (as a     *
 * good practice) by the keyword 'extern'                                    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void RF_RST_Set(int state);
void RF_CS_Set(int state);
void RF_IRQ_Init(void);
void RF_IRQ_Disable(void);
void RF_IRQ_Enable(void);
uint8_t RF_isIRQ_Pending(void);

void gXcvrAssertCS_d(void);
void gXcvrDeassertCS_d(void);

void xcvr_spi_init(uint32_t instance);
void xcvr_spi_configure_speed(uint32_t instance, uint32_t freq);
void xcvr_spi_transfer(uint32_t instance,
                       uint8_t *sendBuffer,
                       uint8_t *receiveBuffer,
                       uint32_t transferByteCount);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __XCVR_SPI_H__ */
