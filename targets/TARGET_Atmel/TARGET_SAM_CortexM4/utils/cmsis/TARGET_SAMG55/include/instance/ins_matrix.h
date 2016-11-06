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

#ifndef _SAMG55_MATRIX_INSTANCE_
#define _SAMG55_MATRIX_INSTANCE_

/* ========== Register definition for MATRIX peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_MATRIX_MCFG                     (0x400E0200U) /**< \brief (MATRIX) Master Configuration Register */
#define REG_MATRIX_SCFG                     (0x400E0240U) /**< \brief (MATRIX) Slave Configuration Register */
#define REG_MATRIX_PRAS0                    (0x400E0280U) /**< \brief (MATRIX) Priority Register A for Slave 0 */
#define REG_MATRIX_PRAS1                    (0x400E0288U) /**< \brief (MATRIX) Priority Register A for Slave 1 */
#define REG_MATRIX_PRAS2                    (0x400E0290U) /**< \brief (MATRIX) Priority Register A for Slave 2 */
#define REG_MATRIX_PRAS3                    (0x400E0298U) /**< \brief (MATRIX) Priority Register A for Slave 3 */
#define REG_CCFG_RDMMEM                     (0x400E0310U) /**< \brief (MATRIX) Embedded Memories Read Margin Values Register */
#define REG_CCFG_SYSIO                      (0x400E0314U) /**< \brief (MATRIX) System I/O Configuration Register */
#define REG_CCFG_DYNCKG                     (0x400E0318U) /**< \brief (MATRIX) Dynamic Clock Gating Register */
#define REG_CCFG_I2SCLKSEL                  (0x400E031CU) /**< \brief (MATRIX) I2S Clock Source Selection Register */
#define REG_CCFG_USBMR                      (0x400E0320U) /**< \brief (MATRIX) USB Management Register */
#define REG_MATRIX_WPMR                     (0x400E03E4U) /**< \brief (MATRIX) Write Protection Mode Register */
#define REG_MATRIX_WPSR                     (0x400E03E8U) /**< \brief (MATRIX) Write Protection Status Register */
#else
#define REG_MATRIX_MCFG    (*(__IO uint32_t*)0x400E0200U) /**< \brief (MATRIX) Master Configuration Register */
#define REG_MATRIX_SCFG    (*(__IO uint32_t*)0x400E0240U) /**< \brief (MATRIX) Slave Configuration Register */
#define REG_MATRIX_PRAS0   (*(__IO uint32_t*)0x400E0280U) /**< \brief (MATRIX) Priority Register A for Slave 0 */
#define REG_MATRIX_PRAS1   (*(__IO uint32_t*)0x400E0288U) /**< \brief (MATRIX) Priority Register A for Slave 1 */
#define REG_MATRIX_PRAS2   (*(__IO uint32_t*)0x400E0290U) /**< \brief (MATRIX) Priority Register A for Slave 2 */
#define REG_MATRIX_PRAS3   (*(__IO uint32_t*)0x400E0298U) /**< \brief (MATRIX) Priority Register A for Slave 3 */
#define REG_CCFG_RDMMEM    (*(__IO uint32_t*)0x400E0310U) /**< \brief (MATRIX) Embedded Memories Read Margin Values Register */
#define REG_CCFG_SYSIO     (*(__IO uint32_t*)0x400E0314U) /**< \brief (MATRIX) System I/O Configuration Register */
#define REG_CCFG_DYNCKG    (*(__IO uint32_t*)0x400E0318U) /**< \brief (MATRIX) Dynamic Clock Gating Register */
#define REG_CCFG_I2SCLKSEL (*(__IO uint32_t*)0x400E031CU) /**< \brief (MATRIX) I2S Clock Source Selection Register */
#define REG_CCFG_USBMR     (*(__IO uint32_t*)0x400E0320U) /**< \brief (MATRIX) USB Management Register */
#define REG_MATRIX_WPMR    (*(__IO uint32_t*)0x400E03E4U) /**< \brief (MATRIX) Write Protection Mode Register */
#define REG_MATRIX_WPSR    (*(__I  uint32_t*)0x400E03E8U) /**< \brief (MATRIX) Write Protection Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAMG55_MATRIX_INSTANCE_ */
