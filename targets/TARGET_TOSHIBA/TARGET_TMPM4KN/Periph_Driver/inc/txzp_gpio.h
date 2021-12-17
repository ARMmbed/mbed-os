/**
 *******************************************************************************
 * @file    txzp_gpio.h
 * @brief   This file provides all the functions prototypes for GPIO driver.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*selects perticularly TMPM4KNA out of TMPM4KyA family */
#ifndef TMPM4KNA
#define TMPM4KNA
#endif
/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txzp_driver_def.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @defgroup GPIO GPIO
 *  @brief    GPIO Driver.
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup GPIO_Exported_define GPIO Exported Define
 *  @{
 */


/**
 *  @defgroup GPIO_Result  Result
 *  @brief    GPIO Result Macro Definition.
 *  @{
 */
#define GPIO_RESULT_SUCCESS     (0)          /*!< Success       */
#define GPIO_RESULT_FAILURE     (-1)         /*!< Failure       */
#define GPIO_READ_FAILURE       (0xFFFFFFFF) /*!< Failure       */
/**
 *  @}
 */ /* End of group GPIO_Result */

/**
 *  @}
 */ /* End of group GPIO_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Typedef GPIO Exported Typedef
  * @{
  */

/**
 *  @enum gpio_pinstate_t
 *  @brief    Pin State Reset/Set Enumerated Type Definition.
 */
typedef enum {
    GPIO_PIN_RESET = 0,  /*!< 0: Clear       */
    GPIO_PIN_SET,        /*!< 1: Set         */
} gpio_pinstate_t;

/**
 *  @enum gpio_pininout_t
 *  @brief    Pin Input/Output Enumerated Type Definition.
 */
typedef enum {
    GPIO_PIN_INPUT = 0,  /*!< 0: Input       */
    GPIO_PIN_OUTPUT,     /*!< 1: Output       */
    GPIO_PIN_INOUT,      /*!< 2: Input/Output */
} gpio_pininout_t;

/**
 *  @enum gpio_gr_t
 *  @brief    Port Group Enumerated Type Definition.
 */
typedef enum {
    GPIO_PORT_A = 0x0, /*!< 0: PA       */
    GPIO_PORT_B,       /*!< 1: PB       */
    GPIO_PORT_C,       /*!< 2: PC       */
    GPIO_PORT_D,       /*!< 3: PD       */
    GPIO_PORT_E,       /*!< 4: PE       */
    GPIO_PORT_F,       /*!< 5: PF       */
    GPIO_PORT_G,       /*!< 6: PG       */
    GPIO_PORT_H,       /*!< 7: PH       */
    GPIO_PORT_J,       /*!< 8: PJ       */
    GPIO_PORT_K,       /*!< 9: PK       */
    GPIO_PORT_L,       /*!< 10:PL       */
    GPIO_PORT_M,       /*!< 11:PM       */
    GPIO_PORT_N,       /*!< 12:PN       */
    GPIO_PORT_U,       /*!< 13:PU       */
    GPIO_PORT_V,       /*!< 14:PV       */
    GPIO_GROUP_Max     /*!< Max Number   */
} gpio_gr_t;

/**
 *  @enum gpio_num_t
 *  @brief    Port Number Enumerated Type Definition.
 */
typedef enum {
    GPIO_PORT_0 = 0x0,  /*!< 0: Port0       */
    GPIO_PORT_1 = 0x1,  /*!< 1: Port1       */
    GPIO_PORT_2 = 0x2,  /*!< 2: Port2       */
    GPIO_PORT_3 = 0x3,  /*!< 3: Port3       */
    GPIO_PORT_4 = 0x4,  /*!< 4: Port4       */
    GPIO_PORT_5 = 0x5,  /*!< 5: Port5       */
    GPIO_PORT_6 = 0x6,  /*!< 6: Port6       */
    GPIO_PORT_7 = 0x7,  /*!< 7: Port7       */
    GPIO_PORT_Max = 0x08  /*!< Max Number     */
} gpio_num_t;

/**
 *  @enum gpio_mode_t
 *  @brief    Port Mode Enumerated Type Definition.
 */
typedef enum {
    GPIO_Mode_DATA = 0x0,  /*!< 0x0: PxDATA       */
    GPIO_Mode_CR  = 0x4,   /*!< 0x4: PxCR         */
    GPIO_Mode_FR1 = 0x8,   /*!< 0x8: PxFR1        */
    GPIO_Mode_FR2 = 0xC,   /*!< 0xC: PxFR2        */
    GPIO_Mode_FR3 = 0x10,  /*!< 0x10: PxFR3        */
    GPIO_Mode_FR4 = 0x14,  /*!< 0x14: PxFR4        */
    GPIO_Mode_FR5 = 0x18,  /*!< 0x18: PxFR5        */
    GPIO_Mode_FR6 = 0x1C,  /*!< 0x1C: PxFR6        */
    GPIO_Mode_FR7 = 0x20,  /*!< 0x1C: PxFR7        */
    GPIO_Mode_OD  = 0x28,  /*!< 0x28: PxOD         */
    GPIO_Mode_PUP = 0x2C,  /*!< 0x2C: PxPUP        */
    GPIO_Mode_PDN = 0x30,  /*!< 0x30: PxPDN        */
    GPIO_Mode_IE  = 0x38   /*!< 0x38: PxIE         */
} gpio_mode_t;

/**
 *  @enum gpio_fr_t
 *  @brief    Port Function Number Enumerated Type Definition.
 */
typedef enum {
    GPIO_FR_1 = 1,   /*!< 1: PxFR1       */
    GPIO_FR_2,       /*!< 2: PxFR2       */
    GPIO_FR_3,       /*!< 3: PxFR3       */
    GPIO_FR_4,       /*!< 4: PxFR4       */
    GPIO_FR_5,       /*!< 5: PxFR5       */
    GPIO_FR_6,       /*!< 6: PxFR6       */
    GPIO_FR_7,       /*!< 7: PxFR7       */
    GPIO_FR_NA,      /*!< 8: N/A         */
    GPIO_FR_Max,      /*!< Max Number     */
} gpio_fr_t;
/**
 *  @enum gpio_pa0_func_t
 *  @brief    PortA0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PA0_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PA0_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PA0_TSPI0CSIN      = 0x1, /*!< 1: TSPI0CSIN        */
    GPIO_PA0_T32A00INB0    = 0x4, /*!< 4: T32A00INB0        */
} gpio_pa0_func_t;

/**
 *  @enum gpio_pa1_func_t
 *  @brief    PortA1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PA1_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PA1_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PA1_INT15      = 0x0, /*!< 0: INT15          */
    GPIO_PA1_TSPI0CS1     = 0x1, /*!< 1: TSPI0CS1         */
    GPIO_PA1_T32A00INB1    = 0x4, /*!< 4: T32A00INB1        */
} gpio_pa1_func_t;

/**
 *  @enum gpio_pa2_func_t
 *  @brief    PortA2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PA2_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PA2_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PA2_INT00   = 0x0, /*!< 0: INT00       */
    GPIO_PA2_TSPI0RXD = 0x1, /*!< 1: TSPI0RXD     */
    GPIO_PA2_T32A00INA0 = 0x4, /*!< 4: T32A00INA0     */
    GPIO_PA2_T32A00INC0 = 0x5, /*!< 5: T32A00INC0     */
    GPIO_PA2_PMD2DBG = 0x6, /*!< 6: PMD2DBG     */
    GPIO_PA2_TRGIN0 = 0x7, /*!< 7: TRGIN0     */
} gpio_pa2_func_t;

/**
 *  @enum gpio_pa3_func_t
 *  @brief    PortA3 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PA3_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PA3_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PA3_INT01b   = 0x0, /*!< 0: INT00       */
    GPIO_PA3_TSPI0TXD = 0x1, /*!< 1: TSPI0TXD     */
    GPIO_PA3_T32A00OUTA = 0x4, /*!< 4: T32A00OUTA     */
    GPIO_PA3_T32A00OUTC = 0x5, /*!< 5: T32A00OUTC     */
    GPIO_PA3_TRGIN1 = 0x7, /*!< 7: TRGIN1     */
} gpio_pa3_func_t;

/**
 *  @enum gpio_pa4_func_t
 *  @brief    PortA4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PA4_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PA4_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PA4_INT01a   = 0x0, /*!< 0: INT01a       */
    GPIO_PA4_TSPI0SCK = 0x1, /*!< 1: TSPI0SCK     */
    GPIO_PA4_T32A00OUTB = 0x4, /*!< 4: T32A00OUTB     */
    GPIO_PA4_TRGIN2 = 0x7, /*!< 7: TRGIN2     */
} gpio_pa4_func_t;

/**
 *  @enum gpio_pb0_func_t
 *  @brief    PortB0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB0_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB0_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB0_UO0    = 0x4, /*!< 4: UO0        */
} gpio_pb0_func_t;

/**
 *  @enum gpio_pb1_func_t
 *  @brief    PortB1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB1_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB1_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB1_XO0     = 0x4, /*!< 4: XO0         */
} gpio_pb1_func_t;

/**
 *  @enum gpio_pb2_func_t
 *  @brief    PortB2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB2_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB2_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB2_VO0     = 0x4, /*!< 4: VO0         */
} gpio_pb2_func_t;

/**
 *  @enum gpio_pb3_func_t
 *  @brief    PortB3 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB3_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB3_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB3_YO0     = 0x4, /*!< 4: YO0         */
} gpio_pb3_func_t;

/**
 *  @enum gpio_pb4_func_t
 *  @brief    PortB4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB4_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB4_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB4_WO0     = 0x4, /*!< 4: WO0         */
} gpio_pb4_func_t;

/**
 *  @enum gpio_pb5_func_t
 *  @brief    PortB5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB5_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB5_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB5_ZO0     = 0x4, /*!< 4: ZO0         */
} gpio_pb5_func_t;

/**
 *  @enum gpio_pb6_func_t
 *  @brief    PortB6 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB6_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB6_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB6_EMG0     = 0x4, /*!< 4: EMG0         */
} gpio_pb6_func_t;

/**
 *  @enum gpio_pb7_func_t
 *  @brief    PortB7 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PB7_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PB7_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PB7_OVV0     = 0x4, /*!< 4: OVV0         */
    GPIO_PB7_PMD0DBG     = 0x5, /*!< 5: PMD0DBG         */
} gpio_pb7_func_t;

/**
 *  @enum gpio_pc0_func_t
 *  @brief    PortC0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC0_INPUT      = 0x0, /*!< 0: Input Port   */
    GPIO_PC0_OUTPUT     = 0x0, /*!< 0: Output Port  */
    GPIO_PC0_UT0TXDA      = 0x1, /*!< 1: UT0TXDA        */
    GPIO_PC0_UT0RXD    = 0x2, /*!< 2: UT0RXD      */
    GPIO_PC0_EI2C0SDA     = 0x3, /*!< 3: EI2C0SDA       */
    GPIO_PC0_I2C0SDA   = 0x4, /*!< 4: I2C0SDA     */
    GPIO_PC0_T32A02INA0 = 0x5, /*!< 5: T32A02INA0   */
    GPIO_PC0_T32A02INC0 = 0x6, /*!< 6: T32A02INC0    */
} gpio_pc0_func_t;

/**
 *  @enum gpio_pc1_func_t
 *  @brief    PortC1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC1_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PC1_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PC1_INT02a      = 0x0, /*!< 0: INT10          */
    GPIO_PC1_UT0RXD     = 0x1, /*!< 1: UT3RXD         */
    GPIO_PC1_UT0TXDA    = 0x2, /*!< 2: UT3TXDA        */
    GPIO_PC1_EI2C0SCL   = 0x3, /*!< 3: TSPI3RXD       */
    GPIO_PC1_I2C0SCL  = 0x4, /*!< 4: T32A3INA0      */
    GPIO_PC1_T32A02OUTA  = 0x5, /*!< 5: T32A2INC0      */
    GPIO_PC1_T32A02OUTC  = 0x6, /*!< 5: T32A2INC0      */
} gpio_pc1_func_t;

/**
 *  @enum gpio_pc2_func_t
 *  @brief    PortC2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC2_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PC2_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PC2_INT10  = 0x0, /*!< 0: INT10       */
    GPIO_PC2_TSPI0CS0 = 0x3, /*!< 3: TSPI0CS0      */
    GPIO_PC2_T32A03OUTA = 0x5, /*!< 5: T32A03OUTA      */
    GPIO_PC2_T32A03OUTC    = 0x6, /*!< 6: T32A03OUTC         */
    GPIO_PC2_PMD0DBG    = 0x7, /*!< 7: PMD0DBG         */
} gpio_pc2_func_t;

/**
 *  @enum gpio_pc3_func_t
 *  @brief    PortC3 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC3_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PC3_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PC3_INT03a  = 0x0, /*!< 0: INT03a       */
    GPIO_PC3_TSPI0RXD = 0x3, /*!< 3: TSPI0CS0      */
    GPIO_PC3_T32A03OUTB = 0x5, /*!< 5: T32A03OUTB      */
    GPIO_PC3_PMD1DBG    = 0x7, /*!< 7: PMD1DBG         */
} gpio_pc3_func_t;

/**
 *  @enum gpio_pc4_func_t
 *  @brief    PortC4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC4_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PC4_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PC4_UT1TXDA  = 0x1, /*!< 1: UT1TXDA       */
    GPIO_PC4_UT1RXD = 0x2, /*!< 2: UT1RXD      */
    GPIO_PC4_TSPI0TXD = 0x3, /*!< 3: TSPI0TXD      */
} gpio_pc4_func_t;

/**
 *  @enum gpio_pc5_func_t
 *  @brief    PortC5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC5_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PC5_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PC5_UT1RXD  = 0x1, /*!< 1: UT1RXD       */
    GPIO_PC5_UT1TXDA = 0x2, /*!< 2: UT1TXDA      */
    GPIO_PC5_TSPI0SCK = 0x3, /*!< 3: TSPI0SCK      */
} gpio_pc5_func_t;

/**
 *  @enum gpio_pc6_func_t
 *  @brief    PortC6 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC6_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PC6_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PC6_INT02b  = 0x0, /*!< 0: INT02b       */
    GPIO_PC6_TSPI0CS1 = 0x3, /*!< 3: TSPI0CS1      */
    GPIO_PC6_T32A02INA1 = 0x5, /*!< 5: T32A02INA1      */
    GPIO_PC6_T32A02INC1    = 0x6, /*!< 6: T32A02INC1         */
} gpio_pc6_func_t;
/**
 *  @enum gpio_pc7_func_t
 *  @brief    PortC7 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PC7_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PC7_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PC7_TSPI0CSIN  = 0x3, /*!< 3: TSPI0CSIN       */
    GPIO_PC7_T32A02INB0 = 0x5, /*!< 5: T32A02INB0      */
} gpio_pc7_func_t;

/**
 *  @enum gpio_pd0_func_t
 *  @brief    PortD0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PD0_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PD0_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PD0_INT17b = 0x0, /*!< 0: INT17b         */
    GPIO_PD0_T32A02INB1 = 0x4, /*!< 4: T32A02INB1         */
} gpio_pd0_func_t;

/**
 *  @enum gpio_pd1_func_t
 *  @brief    PortD1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PD1_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PD1_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PD1_INT17a = 0x0, /*!< 0: INT17a         */
    GPIO_PD1_T32A02OUTB = 0x4, /*!< 4: T32A02OUTB         */
} gpio_pd1_func_t;

/**
 *  @enum gpio_pd2_func_t
 *  @brief    PortD2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PD2_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PD2_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PD2_INT03b = 0x0, /*!< 0: INT03b         */
    GPIO_PD2_UT0CTS_N = 0x1, /*!< 1: UT0CTS_N         */
    GPIO_PD2_T32A03INA0 = 0x4, /*!< 4: T32A03INA0         */
    GPIO_PD2_T32A03INC0 = 0x5, /*!< 5: T32A03INC0         */
} gpio_pd2_func_t;

/**
 *  @enum gpio_pd3_func_t
 *  @brief    PortD3 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PD3_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PD3_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PD3_UT0RTS_N = 0x1, /*!< 1: UT0RTS_N         */
    GPIO_PD3_I2C1SDA = 0x2, /*!< 2: I2C1SDA         */
    GPIO_PD3_EI2C1SDA = 0x3, /*!< 3: EI2C1SDA         */
    GPIO_PD3_T32A03INA1 = 0x4, /*!< 4: T32A03INA1         */
    GPIO_PD3_T32A03INC1 = 0x5, /*!< 5: T32A03INC1         */
    GPIO_PD3_ENC2A = 0x6, /*!< 6: ENC2A         */
} gpio_pd3_func_t;

/**
 *  @enum gpio_pd4_func_t
 *  @brief    PortD4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PD4_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PD4_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PD4_INT18b = 0x0, /*!< 0: INT18b         */
    GPIO_PD4_I2C1SCL = 0x2, /*!< 2: I2C1SCL         */
    GPIO_PD4_EI2C1SCL = 0x3, /*!< 3: EI2C1SCL         */
    GPIO_PD4_T32A03INB0 = 0x4, /*!< 4: T32A03INB0         */
    GPIO_PD4_ENC2B = 0x6, /*!< 6: ENC2B         */
} gpio_pd4_func_t;

/**
 *  @enum gpio_pd5_func_t
 *  @brief    PortD5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PD5_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PD5_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PD5_INT18a = 0x0, /*!< 0: INT18a         */
    GPIO_PD5_T32A03INB1 = 0x4, /*!< 4: T32A03INB1         */
    GPIO_PD5_ENC2Z = 0x6, /*!< 6: ENC2Z         */
} gpio_pd5_func_t;

/**
 *  @enum gpio_pe0_func_t
 *  @brief    PortE0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PE0_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE0_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE0_UO1 = 0x6, /*!< 6: UO1         */
} gpio_pe0_func_t;

/**
 *  @enum gpio_pe1_func_t
 *  @brief    PortE1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PE1_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE1_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE1_INT04b = 0x0, /*!< 0: INT04b         */
    GPIO_PE1_T32A03INA0 = 0x4, /*!< 4: T32A03INA0         */
    GPIO_PE1_T32A03INC0 = 0x5, /*!< 5: T32A03INC0         */
    GPIO_PE1_XO1 = 0x6, /*!< 6: XO1         */
} gpio_pe1_func_t;

/**
 *  @enum gpio_pe2_func_t
 *  @brief    PortE2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PE2_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE2_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE2_T32A03OUTA = 0x4, /*!< 4: T32A03OUTA         */
    GPIO_PE2_T32A03OUTC = 0x5, /*!< 5: T32A03OUTC         */
    GPIO_PE2_VO1 = 0x6, /*!< 6: VO1         */
} gpio_pe2_func_t;

/**
*  @enum gpio_pe3_func_t
*  @brief    PortE3 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PE3_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE3_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE3_INT04a = 0x0, /*!< 0: INT04a         */
    GPIO_PE3_T32A03INA1 = 0x4, /*!< 4: T32A03INA1         */
    GPIO_PE3_T32A03INC1 = 0x5, /*!< 5: T32A03INC1         */
    GPIO_PE3_YO1 = 0x6, /*!< 6: YO1         */
} gpio_pe3_func_t;

/**
*  @enum gpio_pe4_func_t
*  @brief    PortE4 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PE4_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE4_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE4_INT11a = 0x0, /*!< 0: INT11a         */
    GPIO_PE4_T32A03INB0 = 0x4, /*!< 4: T32A03INB0         */
    GPIO_PE4_WO1 = 0x6, /*!< 6: WO1         */
} gpio_pe4_func_t;

/**
 *  @enum gpio_pe5_func_t
 *  @brief    PortE5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PE5_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE5_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE5_INT05a = 0x0, /*!< 0: INT05a         */
    GPIO_PE5_INT11b  = 0x0, /*!< 0: INT11b          */
    GPIO_PE5_T32A03INB1  = 0x4, /*!< 4: T32A03INB1          */
    GPIO_PE5_ZO1  = 0x6, /*!< 6: ZO1          */
} gpio_pe5_func_t;

/**
 *  @enum gpio_pe6_func_t
 *  @brief    PortE6 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PE6_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE6_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE6_INT05b = 0x0, /*!< 0: INT05a         */
    GPIO_PE6_T32A03OUTB  = 0x4, /*!< 4: T32A03OUTB          */
    GPIO_PE6_EMG1  = 0x6, /*!< 6: EMG1          */
} gpio_pe6_func_t;

/**
 *  @enum gpio_pe7_func_t
 *  @brief    PortE7 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PE7_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PE7_OUTPUT = 0x0, /*!< 0: Output Port    */
    GPIO_PE7_OVV1 = 0x6, /*!< 6: OVV1         */
    GPIO_PE7_PMD1DBG  = 0x7, /*!< 7: PMD1DBG          */
} gpio_pe7_func_t;

/**
 *  @enum gpio_pf0_func_t
 *  @brief    PortF0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF0_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF0_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF0_UT2TXDA = 0x1, /*!< 1: UT2TXDA     */
    GPIO_PF0_UT2RXD = 0x2, /*!< 2: UT2RXD     */
    GPIO_PF0_T32A05INA0     = 0x4, /*!< 4: T32A05INA0         */
    GPIO_PF0_T32A05INC0     = 0x5, /*!< 5: T32A05INC0         */
    GPIO_PF0_TMS_SWDIO     = 0x7, /*!< 7: TMS_SWDIO         */
} gpio_pf0_func_t;

/**
 *  @enum gpio_pf1_func_t
 *  @brief    PortF1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF1_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF1_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF1_INT06a = 0x0, /*!< 0: INT06a     */
    GPIO_PF1_UT2RXD = 0x1, /*!< 1: UT2RXD     */
    GPIO_PF1_UT2TXDA = 0x2, /*!< 2: UT2TXDA     */
    GPIO_PF1_T32A05OUTA = 0x4, /*!< 4: T32A05OUTA     */
    GPIO_PF1_T32A05OUTC = 0x5, /*!< 5: T32A05OUTC     */
    GPIO_PF1_TCK_SWCLK = 0x7, /*!< 7: TCK_SWCLK     */
} gpio_pf1_func_t;

/**
 *  @enum gpio_pf2_func_t
 *  @brief    PortF2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF2_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF2_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF2_INT06b = 0x0, /*!< 0: INT06b     */
    GPIO_PF2_T32A05INA1 = 0x4, /*!< 4: T32A05INA1     */
    GPIO_PF2_T32A05INC1     = 0x5, /*!< 5: TRGIN2         */
    GPIO_PF2_TDO_SWV     = 0x7, /*!< 7: TDO_SWV         */
} gpio_pf2_func_t;

/**
 *  @enum gpio_pf3_func_t
 *  @brief    PortF3 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF3_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF3_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF3_UT3TXDA = 0x1, /*!< 1: UT3TXDA     */
    GPIO_PF3_UT3RXD = 0x2, /*!< 2: UT3RXD     */
    GPIO_PF3_T32A01INA0     = 0x4, /*!< 4: T32A01INA0         */
    GPIO_PF3_T32A01INC0     = 0x5, /*!< 5: T32A01INC0         */
    GPIO_PF3_ENC1A     = 0x6, /*!< 6: ENC1A         */
    GPIO_PF3_TDI     = 0x7, /*!< 7: TDI         */
} gpio_pf3_func_t;

/**
 *  @enum gpio_pf4_func_t
 *  @brief    PortF4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF4_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF4_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF4_INT14a = 0x0, /*!< 0: INT14a     */
    GPIO_PF4_UT3RXD = 0x1, /*!< 1: UT3RXD     */
    GPIO_PF4_UT3TXDA     = 0x2, /*!< 2: UT3TXDA         */
    GPIO_PF4_NBDSYNC     = 0x3, /*!< 3: NBDSYNC         */
    GPIO_PF4_T32A01OUTA     = 0x4, /*!< 4: NBDSYNC         */
    GPIO_PF4_T32A01OUTC     = 0x5, /*!< 5: T32A01OUTC         */
    GPIO_PF4_ENC1B     = 0x6, /*!< 6: ENC1B         */
    GPIO_PF4_TRST_N     = 0x7, /*!< 7: TRST_N         */
} gpio_pf4_func_t;

/**
 *  @enum gpio_pf5_func_t
 *  @brief    PortF5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF5_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF5_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF5_INT14b = 0x0, /*!< 0: INT14b     */
    GPIO_PF5_NBDCLK = 0x3, /*!< 3: NBDCLK     */
    GPIO_PF5_T32A01INA1     = 0x4, /*!< 4: T32A01INA1         */
    GPIO_PF5_T32A01INC1     = 0x5, /*!< 5: T32A01INC1         */
    GPIO_PF5_ENC1Z     = 0x6, /*!< 6: ENC1Z         */
    GPIO_PF5_TRACECLK     = 0x7, /*!< 7: TRACECLK         */
} gpio_pf5_func_t;

/**
 *  @enum gpio_pf6_func_t
 *  @brief    PortF6 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF6_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF6_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF6_UT3TXDA = 0x1, /*!< 1: UT3TXDA     */
    GPIO_PF6_UT3RXD = 0x2, /*!< 2: UT3RXD     */
    GPIO_PF6_NBDDATA0 = 0x3, /*!< 3: NBDDATA0     */
    GPIO_PF6_T32A01INB0     = 0x4, /*!< 4: T32A01INB0         */
    GPIO_PF6_TRACEDATA0     = 0x7, /*!< 7: TRACEDATA0         */
} gpio_pf6_func_t;

/**
 *  @enum gpio_pf7_func_t
 *  @brief    PortF7 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PF7_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PF7_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PF7_UT3RXD = 0x1, /*!< 1: UT3RXD     */
    GPIO_PF7_UT3TXDA = 0x2, /*!< 2: UT3TXDA     */
    GPIO_PF7_NBDDATA1     = 0x3, /*!< 3: NBDDATA1         */
    GPIO_PF7_T32A01INB1     = 0x4, /*!< 4: T32A01INB1         */
    GPIO_PF7_TRACEDATA1     = 0x7, /*!< 7: TRACEDATA1         */
} gpio_pf7_func_t;

/**
 *  @enum gpio_pg0_func_t
 *  @brief    PortG0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG0_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PG0_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PG0_T32A04INA0     = 0x4, /*!< 4: T32A04INA0          */
    GPIO_PG0_T32A04INC0   = 0x5, /*!< 5: T32A04INC0        */
} gpio_pg0_func_t;

/**
 *  @enum gpio_pg1_func_t
 *  @brief    PortG1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG1_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PG1_OUTUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PG1_TSPI1CS1     = 0x1, /*!< 1: TSPI1CS1          */
    GPIO_PG1_T32A04INA1    = 0x4, /*!< 4: T32A04INA1         */
    GPIO_PG1_T32A04INC1  = 0x5, /*!< 5: T32A04INC1       */
} gpio_pg1_func_t;

/**
 *  @enum gpio_pg2_func_t
 *  @brief    PortG2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG2_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PG2_OUTUT      = 0x0, /*!< 0: Output Port    */
    GPIO_PG2_TSPI1CS0   = 0x1, /*!< 1: TSPI1CS0       */
    GPIO_PG2_T32A04OUTA = 0x4, /*!< 4: T32A04OUTA     */
    GPIO_PG2_T32A04OUTC = 0x5, /*!< 5: T32A04OUTC     */
} gpio_pg2_func_t;

/**
 *  @enum gpio_pg3_func_t
 *  @brief    PortG3 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG3_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PG3_OUTUT      = 0x0, /*!< 0: Output Port    */
    GPIO_PG3_INT21        = 0x0, /*!< 0: XO1            */
    GPIO_PG3_TSPI1CSIN    = 0x1, /*!< 1: TSPI1CSIN        */
    GPIO_PG3_T32A04OUTB    = 0x4, /*!< 4: T32A04OUTB        */
} gpio_pg3_func_t;

/**
 *  @enum gpio_pg4_func_t
 *  @brief    PortG4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG4_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PG4_OUTUT      = 0x0, /*!< 0: Output Port    */
    GPIO_PG4_TSPI1RXD        = 0x1, /*!< 1: TSPI1RXD            */
    GPIO_PG4_T32A04INB0    = 0x4, /*!< 4: T32A04INB0        */
} gpio_pg4_func_t;

/**
 *  @enum gpio_pg5_func_t
 *  @brief    PortG5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG5_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PG5_OUTUT      = 0x0, /*!< 0: Output Port    */
    GPIO_PG5_TSPI1TXD        = 0x1, /*!< 1: TSPI1TXD            */
    GPIO_PG5_T32A04INB1    = 0x4, /*!< 4: T32A04INB1        */
} gpio_pg5_func_t;

/**
 *  @enum gpio_pg6_func_t
 *  @brief    PortG6 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PG6_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PG6_OUTUT      = 0x0, /*!< 0: Output Port    */
    GPIO_PG6_TSPI1SCK        = 0x1, /*!< 1: TSPI1TXD            */
} gpio_pg6_func_t;

/**
 *  @enum gpio_ph0_func_t
 *  @brief    PortH0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PH0_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PH0_X1        = 0x0, /*!< 0: X1             */
    GPIO_PH0_EHCLKIN        = 0x0, /*!< 0: X1             */
} gpio_ph0_func_t;

/**
 *  @enum gpio_ph1_func_t
 *  @brief    PortH1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PH1_INPUT  = 0x0, /*!< 0: Input Port     */
    GPIO_PH1_X2     = 0x0, /*!< 0: X2             */
} gpio_ph1_func_t;

/**
 *  @enum gpio_pj0_func_t
 *  @brief    PortJ0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PJ0_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PJ0_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PJ0_AINC00      = 0x0, /*!< 0: AINC00            */
} gpio_pj0_func_t;

/**
 *  @enum gpio_pj1_func_t
 *  @brief    PortJ1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PJ1_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PJ1_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PJ1_AINC01       = 0x0, /*!< 0: AINC01            */
} gpio_pj1_func_t;

/**
 *  @enum gpio_pj2_func_t
 *  @brief    PortJ2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PJ2_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PJ2_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PJ2_AINC02       = 0x0, /*!< 0: AINC02         */
} gpio_pj2_func_t;

/**
*  @enum gpio_pj3_func_t
*  @brief    PortJ3 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PJ3_INPUT       = 0x0, /*!< 0: Input Port     */
    GPIO_PJ3_OUTPUT      = 0x0, /*!< 0: Output Port    */
    GPIO_PJ3_AINC03         = 0x0, /*!< 0: AINC03             */
} gpio_pj3_func_t;

/**
*  @enum gpio_pj4_func_t
*  @brief    PortJ4 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PJ4_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PJ4_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PJ4_AINC04        = 0x0, /*!< 0: AINC04             */
} gpio_pj4_func_t;

/**
 *  @enum gpio_pj5_func_t
 *  @brief    PortJ5 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PJ5_INPUT     = 0x0, /*!< 0: Input Port     */
    GPIO_PJ5_OUTPUT    = 0x0, /*!< 0: Output Port    */
    GPIO_PJ5_AINC05       = 0x0, /*!< 0: AINC05            */
} gpio_pj5_func_t;

/**
 *  @enum gpio_pk0_func_t
 *  @brief    PortK0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PK0_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PK0_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PK0_AINB00      = 0x0, /*!< 0: AINB00          */
} gpio_pk0_func_t;

/**
 *  @enum gpio_pk1_func_t
 *  @brief    PortK1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PK1_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PK1_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PK1_AINB01      = 0x0, /*!< 0: AINB01          */
} gpio_pk1_func_t;

/**
 *  @enum gpio_pk2_func_t
 *  @brief    PortK2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PK2_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PK2_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PK2_AINB02      = 0x0, /*!< 0: AINB02          */
} gpio_pk2_func_t;

/**
*  @enum gpio_pk3_func_t
*  @brief    PortK3 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PK3_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PK3_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PK3_AINB03      = 0x0, /*!< 0: AINB03          */
} gpio_pk3_func_t;

/**
*  @enum gpio_pk4_func_t
*  @brief    PortK4 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PK4_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PK4_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PK4_AINB04   = 0x0, /*!< 0: AINB01       */
} gpio_pk4_func_t;

/**
 *  @enum gpio_pl0_func_t
 *  @brief    PortL0 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PL0_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PL0_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PL0_AINA16   = 0x0, /*!< 0: AINA16       */
} gpio_pl0_func_t;

/**
 *  @enum gpio_pl1_func_t
 *  @brief    PortL1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PL1_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PL1_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PL1_AINA15   = 0x0, /*!< 0: AINA15       */
} gpio_pl1_func_t;

/**
 *  @enum gpio_pl2_func_t
 *  @brief    PortL2 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PL2_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PL2_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PL2_AINA17   = 0x0, /*!< 0: NBDDATA2       */
} gpio_pl2_func_t;

/**
*  @enum gpio_pl3_func_t
*  @brief    PortL3 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PL3_INPUT      = 0x0, /*!< 0: Input Port     */
    GPIO_PL3_OUTPUT     = 0x0, /*!< 0: Output Port    */
    GPIO_PL3_AINA14   = 0x0, /*!< 0: AINA14          */
} gpio_pl3_func_t;

/**
*  @enum gpio_pl4_func_t
*  @brief    PortL4 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PL4_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PL4_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PL4_AINA18   = 0x0, /*!< 0: AINA18          */
} gpio_pl4_func_t;

/**
*  @enum gpio_pl5_func_t
*  @brief    PortL5 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PL5_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PL5_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PL5_AINA13   = 0x0, /*!< 0: AINA13          */
} gpio_pl5_func_t;

/**
*  @enum gpio_pl6_func_t
*  @brief    PortL6 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PL6_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PL6_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PL6_AINA09   = 0x0, /*!< 0: AINA09          */
} gpio_pl6_func_t;

/**
*  @enum gpio_pl7_func_t
*  @brief    PortL7 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PL7_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PL7_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PL7_AINA08   = 0x0, /*!< 0: AINA08          */
} gpio_pl7_func_t;

/**
*  @enum gpio_pm0_func_t
*  @brief    PortM0 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PM0_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PM0_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PM0_AINA07   = 0x0, /*!< 0: AINA07          */
} gpio_pm0_func_t;

/**
 *  @enum gpio_pm1_func_t
 *  @brief    PortM1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PM1_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PM1_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PM1_AINA06   = 0x0, /*!< 0: AINA06          */
} gpio_pm1_func_t;

/**
*  @enum gpio_pm2_func_t
*  @brief    PortM2 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PM2_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PM2_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PM2_AINA05   = 0x0, /*!< 0: AINA05          */
} gpio_pm2_func_t;

/**
*  @enum gpio_pn0_func_t
*  @brief    PortN0 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PN0_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PN0_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PN0_UT0TXDA   = 0x1, /*!< 0: UT0TXDA          */
    GPIO_PN0_UT0RXD   = 0x2, /*!< 0: UT0RXD          */
    GPIO_PN0_NBDDATA2   = 0x3, /*!< 0: NBDDATA2          */
    GPIO_PN0_T32A05INA0   = 0x4, /*!< 0: T32A05INA0          */
    GPIO_PN0_T32A05INC0   = 0x5, /*!< 0: T32A05INC0          */
    GPIO_PN0_ENC0A   = 0x6, /*!< 0: ENC0A          */
    GPIO_PN0_TRACEDATA2   = 0x7, /*!< 0: TRACEDATA2          */
} gpio_pn0_func_t;

/**
 *  @enum gpio_pn1_func_t
 *  @brief    PortN1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PN1_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PN1_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PN1_INT16a   = 0x0, /*!< 0: INT16a          */
    GPIO_PN1_UT0RXD   = 0x1, /*!< 0: UT0RXD          */
    GPIO_PN1_UT0TXDA   = 0x2, /*!< 0: UT0TXDA          */
    GPIO_PN1_NBDDATA3   = 0x3, /*!< 0: NBDDATA3          */
    GPIO_PN1_T32A05OUTA   = 0x4, /*!< 0: T32A05OUTA          */
    GPIO_PN1_T32A05OUTC   = 0x5, /*!< 0: T32A05OUTC          */
    GPIO_PN1_ENC0B   = 0x6, /*!< 0: ENC0B          */
    GPIO_PN1_TRACEDATA3   = 0x7, /*!< 0: TRACEDATA3          */
} gpio_pn1_func_t;

/**
*  @enum gpio_pn2_func_t
*  @brief    PortN2 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PN2_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PN2_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PN2_INT16b   = 0x0, /*!< 0: INT16b          */
    GPIO_PN2_UT0CTS_N   = 0x1, /*!< 0: UT0CTS          */
    GPIO_PN2_T32A05INA1   = 0x4, /*!< 0: T32A05INA1          */
    GPIO_PN2_T32A05INC1   = 0x5, /*!< 0: T32A05INC1          */
    GPIO_PN2_ENC0Z   = 0x6, /*!< 0: ENC0Z          */
} gpio_pn2_func_t;
/**
*  @enum gpio_pu0_func_t
*  @brief    PortU0 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PU0_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU0_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU0_INT12   = 0x0, /*!< 0: INT12          */
    GPIO_PU0_UT2TXDA   = 0x1, /*!< 0: UT2TXDA          */
    GPIO_PU0_UT2RXD   = 0x2, /*!< 0: UT2RXD          */
    GPIO_PU0_I2C1SDA   = 0x3, /*!< 0: I2C1SDA          */
    GPIO_PU0_T32A02INB1   = 0x4, /*!< 0: T32A02INB1          */
    GPIO_PU0_UO2   = 0x6, /*!< 0: UO2          */
    GPIO_PU0_EI2C1SDA   = 0x7, /*!< 0: EI2C1SDA          */
} gpio_pu0_func_t;

/**
 *  @enum gpio_pu1_func_t
 *  @brief    PortU1 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PU1_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU1_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU1_INT07a   = 0x0, /*!< 0: INT07a          */
    GPIO_PU1_UT2RXD   = 0x1, /*!< 0: UT2RXD          */
    GPIO_PU1_UT2TXDA   = 0x2, /*!< 0: UT2TXDA          */
    GPIO_PU1_I2C1SCL   = 0x3, /*!< 0: I2C1SCL          */
    GPIO_PU1_T32A02INA0   = 0x4, /*!< 0: T32A02INA0          */
    GPIO_PU1_T32A02INC0   = 0x5, /*!< 0: T32A02INC0          */
    GPIO_PU1_XO2   = 0x6, /*!< 0: XO2          */
    GPIO_PU1_EI2C1SCL   = 0x7, /*!< 0: EI2C1SCL          */
} gpio_pu1_func_t;

/**
*  @enum gpio_pu2_func_t
*  @brief    PortU2 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PU2_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU2_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU2_INT07b   = 0x0, /*!< 0: INT07b          */
    GPIO_PU2_T32A02OUTA   = 0x4, /*!< 0: T32A02OUTA          */
    GPIO_PU2_T32A02OUTC   = 0x5, /*!< 0: T32A02OUTC          */
    GPIO_PU2_VO2   = 0x6, /*!< 0: VO2          */
} gpio_pu2_func_t;
/**
*  @enum gpio_pu3_func_t
*  @brief    PortU0 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PU3_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU3_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU3_INT08a   = 0x0, /*!< 0: INT08a          */
    GPIO_PU3_UT1RTS_N   = 0x1, /*!< 0: UT1RTS_N          */
    GPIO_PU3_T32A02INB0   = 0x4, /*!< 0: T32A02INB0          */
    GPIO_PU3_ENC2A   = 0x5, /*!< 0: ENC2A          */
    GPIO_PU3_YO2   = 0x6, /*!< 0: YO2          */
} gpio_pu3_func_t;

/**
 *  @enum gpio_pu4_func_t
 *  @brief    PortU4 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PU4_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU4_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU4_INT08b   = 0x0, /*!< 0: INT08b          */
    GPIO_PU4_UT1CTS_N  = 0x1, /*!< 0: UT1CTS_N          */
    GPIO_PU4_T32A02OUTB   = 0x4, /*!< 0: T32A02OUTB          */
    GPIO_PU4_T32A02INC1   = 0x5, /*!< 0: T32A02INC1          */
    GPIO_PU4_WO2   = 0x6, /*!< 0: WO2          */
} gpio_pu4_func_t;

/**
*  @enum gpio_pu5_func_t
*  @brief    PortU5 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PU5_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU5_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU5_INT13   = 0x0, /*!< 0: INT13          */
    GPIO_PU5_UT1TXDA   = 0x1, /*!< 0: UT1TXDA          */
    GPIO_PU5_UT1RXD   = 0x2, /*!< 0: UT1RXD          */
    GPIO_PU5_T32A02INA1   = 0x4, /*!< 0: T32A02INA1          */
    GPIO_PU5_ENC2B   = 0x5, /*!< 0: ENC2B          */
    GPIO_PU5_ZO2   = 0x6, /*!< 0: ZO2          */
} gpio_pu5_func_t;

/**
*  @enum gpio_pu6_func_t
*  @brief    PortU6 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PU6_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU6_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU6_INT09   = 0x0, /*!< 0: INT09          */
    GPIO_PU6_UX1RXD   = 0x1, /*!< 0: UX1RXD          */
    GPIO_PU6_UT1TXDA   = 0x2, /*!< 0: UT1TXDA          */
    GPIO_PU6_ENC2Z   = 0x5, /*!< 0: ENC2Z          */
    GPIO_PU6_EMG2   = 0x6, /*!< 0: EMG2          */
} gpio_pu6_func_t;

/**
 *  @enum gpio_pu7_func_t
 *  @brief    PortU7 Function Enumerated Type Definition.
 */
typedef enum {
    GPIO_PU7_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PU7_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PU7_OVV2   = 0x6, /*!< 0: OVV2          */
    GPIO_PU7_PMD2DBG   = 0x7, /*!< 0: PMD2DBG          */
} gpio_pu7_func_t;

/**
*  @enum gpio_pv0_func_t
*  @brief    PortV0 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PV0_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PV0_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PV0_TSPI1CSIN   = 0x2, /*!< 0: TSPI1CSIN          */
    GPIO_PV0_T32A01OUTB   = 0x4, /*!< 0: T32A01OUTB          */
} gpio_pv0_func_t;

/**
*  @enum gpio_pv1_func_t
*  @brief    PortV1 Function Enumerated Type Definition.
*/
typedef enum {
    GPIO_PV1_INPUT    = 0x0, /*!< 0: Input Port     */
    GPIO_PV1_OUTPUT   = 0x0, /*!< 0: Output Port    */
    GPIO_PV1_UT0RTS_N   = 0x1, /*!< 0: UT0RTS_N          */
    GPIO_PV1_TSPI1RXD   = 0x2, /*!< 0: TSPI1RXD          */
} gpio_pv1_func_t;

/**
 *  @}
 */ /* End of group GPIO_Exported_Typedef */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup GPIO_Exported_Typedef GPIO Exported Typedef
 *  @{
 */
/*----------------------------------*/
/**
 * @brief  GPIO handle structure definenition.
*/
/*----------------------------------*/

typedef struct gpio_pa_handle {
    TSB_PA_TypeDef       *p_pa_instance;     /*!< Registers base address.           */
    TSB_PB_TypeDef       *p_pb_instance;     /*!< Registers base address.           */
    TSB_PC_TypeDef       *p_pc_instance;     /*!< Registers base address.           */
    TSB_PD_TypeDef       *p_pd_instance;     /*!< Registers base address.           */
    TSB_PE_TypeDef       *p_pe_instance;     /*!< Registers base address.           */
    TSB_PF_TypeDef       *p_pf_instance;     /*!< Registers base address.           */
    TSB_PG_TypeDef       *p_pg_instance;     /*!< Registers base address.           */
    TSB_PH_TypeDef       *p_ph_instance;     /*!< Registers base address.           */
    TSB_PJ_TypeDef       *p_pj_instance;     /*!< Registers base address.           */
    TSB_PK_TypeDef       *p_pk_instance;     /*!< Registers base address.           */
    TSB_PL_TypeDef       *p_pl_instance;     /*!< Registers base address.           */
    TSB_PM_TypeDef       *p_pm_instance;     /*!< Registers base address.           */
    TSB_PN_TypeDef       *p_pn_instance;     /*!< Registers base address.           */
    TSB_PU_TypeDef       *p_pu_instance;     /*!< Registers base address.           */
    TSB_PV_TypeDef       *p_pv_instance;     /*!< Registers base address.           */
} _gpio_t;

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup GPIO_Exported_functions GPIO Exported Functions
 *  @{
 */
TXZ_Result _gpio_init(_gpio_t *p_obj, uint32_t group);
TXZ_Result gpio_deinit(_gpio_t *p_obj, uint32_t group);
TXZ_Result gpio_func(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t func, uint32_t inout);
TXZ_Result gpio_write_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t val);
TXZ_Result gpio_read_mode(_gpio_t *p_obj, uint32_t group, uint32_t mode, uint32_t *val);
TXZ_Result gpio_write_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, uint32_t val);
TXZ_Result gpio_read_bit(_gpio_t *p_obj, uint32_t group, uint32_t num, uint32_t mode, gpio_pinstate_t *pinstate);

/**
 *  @}
 */ /* End of group GPIO_Exported_functions */

/**
 *  @}
 */ /* End of group GPIO */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __GPIO_H */
