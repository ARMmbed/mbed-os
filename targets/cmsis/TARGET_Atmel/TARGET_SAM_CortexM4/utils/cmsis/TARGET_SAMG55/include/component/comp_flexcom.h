/**
 * \file
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAMG55_FLEXCOM_COMPONENT_
#define _SAMG55_FLEXCOM_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Flexible Serial Communication */
/* ============================================================================= */
/** \addtogroup SAMG55_FLEXCOM Flexible Serial Communication */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Flexcom hardware registers */
typedef struct {
    __IO uint32_t FLEXCOM_MR;   /**< \brief (Flexcom Offset: 0x0000) FLEXCOM Mode register */
    __I  uint32_t Reserved1[3];
    __I  uint32_t FLEXCOM_RHR;  /**< \brief (Flexcom Offset: 0x0010) FLEXCOM Receive Holding Register */
    __I  uint32_t Reserved2[3];
    __IO uint32_t FLEXCOM_THR;  /**< \brief (Flexcom Offset: 0x0020) FLEXCOM Transmit Holding Register */
} Flexcom;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- FLEXCOM_MR : (FLEXCOM Offset: 0x0000) FLEXCOM Mode register -------- */
#define FLEXCOM_MR_OPMODE_Pos 0
#define FLEXCOM_MR_OPMODE_Msk (0x3u << FLEXCOM_MR_OPMODE_Pos) /**< \brief (FLEXCOM_MR) FLEXCOM Operating Mode */
#define   FLEXCOM_MR_OPMODE_NO_COM (0x0u << 0) /**< \brief (FLEXCOM_MR) No communication */
#define   FLEXCOM_MR_OPMODE_USART (0x1u << 0) /**< \brief (FLEXCOM_MR) All related USART related protocols are selected (RS232, RS485, ISO7816, LIN,)All SPI/TWI related registers are not accessible and have no impact on IOs. */
#define   FLEXCOM_MR_OPMODE_SPI (0x2u << 0) /**< \brief (FLEXCOM_MR) SPI operating mode is selected.All USART/TWI related registers are not accessible and have no impact on IOs. */
#define   FLEXCOM_MR_OPMODE_TWI (0x3u << 0) /**< \brief (FLEXCOM_MR) All related TWI protocols are selected (TWI, SMBUS). All USART/SPI related registers are not accessible and have no impact on IOs. */
/* -------- FLEXCOM_RHR : (FLEXCOM Offset: 0x0010) FLEXCOM Receive Holding Register -------- */
#define FLEXCOM_RHR_RXDATA_Pos 0
#define FLEXCOM_RHR_RXDATA_Msk (0xffffu << FLEXCOM_RHR_RXDATA_Pos) /**< \brief (FLEXCOM_RHR) Receive Data */
/* -------- FLEXCOM_THR : (FLEXCOM Offset: 0x0020) FLEXCOM Transmit Holding Register -------- */
#define FLEXCOM_THR_TXDATA_Pos 0
#define FLEXCOM_THR_TXDATA_Msk (0xffffu << FLEXCOM_THR_TXDATA_Pos) /**< \brief (FLEXCOM_THR) Transmit Data */
#define FLEXCOM_THR_TXDATA(value) ((FLEXCOM_THR_TXDATA_Msk & ((value) << FLEXCOM_THR_TXDATA_Pos)))

/*@}*/


#endif /* _SAMG55_FLEXCOM_COMPONENT_ */
