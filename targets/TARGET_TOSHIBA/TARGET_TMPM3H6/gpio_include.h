/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
 */
#ifndef __GPIO_INCLUDE_H
#define __GPIO_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>
#include "TMPM3H6.h"
#include "objects.h"
#include "serial_api.h"
#include "txz_driver_def.h"

enum BitMode {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET,
};

enum PortFunction {
    GPIO_Mode_DATA =  0x0, // 0x0: PxDATA
    GPIO_Mode_CR   = 0x04, // 0x4: PxCR
    GPIO_Mode_FR1  = 0x08, // 0x8: PxFR1
    GPIO_Mode_FR2  = 0x0C, // 0xC: PxFR2
    GPIO_Mode_FR3  = 0x10, // 0x10: PxFR3
    GPIO_Mode_FR4  = 0x14, // 0x14: PxFR4
    GPIO_Mode_FR5  = 0x18, // 0x18: PxFR5
    GPIO_Mode_FR6  = 0x1C, // 0x1C: PxFR6
    GPIO_Mode_OD   = 0x28, // 0x28: PxOD
    GPIO_Mode_PUP  = 0x2C, // 0x2C: PxPUP
    GPIO_Mode_PDN  = 0x30, // 0x30: PxPDN
    GPIO_Mode_IE   = 0x38  // 0x38: PxIE
};

// GPIO
#define PORT_BASE                               (0x400C0000UL) // Port Register Base Adress
#define BITBAND_PORT_OFFSET                     (0x0000100UL) // Port Register Offset Value
#define BITBAND_PORT_BASE(gr)                   (PORT_BASE + (uint32_t)((BITBAND_PORT_OFFSET) * (gr)) ) // Operational target Port Adress
#define BITBAND_PORT_MODE_BASE(base, pinmode)   ((uint32_t)(base) + (uint32_t)(pinmode) ) // Operational target Control Register Adress
#define BITBAND_PORT_SET(base, bitnum)          (*((__IO uint32_t *)base) |= (uint32_t)(0x0000001UL<< bitnum)) // Target Pin Bit set
#define BITBAND_PORT_CLR(base, bitnum)          (*((__IO uint32_t *)base) &= ~((uint32_t)(0x0000001UL<< bitnum))) // Target Pin Bit clear
#define BITBAND_PORT_READ(val, base, bitnum)    val = ((*((__IO uint32_t *)base) & (uint32_t)(0x0000001UL<< bitnum)) >> bitnum) // Target Pin Bit read
#define GPIO_DATA                               PIN_DATA(0, 2)
#define CHANNEL_NUM                             16
#define PORT_PIN_NUM                            8
#define PIN_PORT(X)                             (((uint32_t)(X) >> 3) & 0xF)
#define PIN_POS(X)                              ((uint32_t)(X) & 0x7)
// Pin data, bit 31..16: Pin Function, bit 15..0: Pin Direction
#define PIN_DATA(FUNC, DIR)                     (int)(((FUNC) << 16)| ((DIR) << 0))
#define PIN_FUNC(X)                             (((X) & 0xFFFF0000) >> 16)
#define PIN_DIR(X)                              ((X) & 0xFFFF)
// SPI
#define TIMEOUT                                 1000
#define INITIAL_SPI_FREQ                        1000000
// I2C
#define I2CxCR2_I2CM_ENABLE                     ((uint32_t)0x00000080)
#define I2CxCR2_SWRES_10                        ((uint32_t)0x00000002)
#define I2CxCR2_SWRES_01                        ((uint32_t)0x00000001)
#define I2CxCR2_START_CONDITION                 ((uint32_t)0x000000F8)
#define I2CxCR2_STOP_CONDITION                  ((uint32_t)0x000000D8)
#define I2CxCR2_INIT                            ((uint32_t)0x00000008)
#define I2CxCR2_PIN_CLEAR                       ((uint32_t)0x00000010)
#define I2CxCR2_TRX                             ((uint32_t)0x00000040)
#define I2CxST_I2C                              ((uint32_t)0x00000001)
#define I2CxST_CLEAR                            ((uint32_t)0x0000000F)
#define I2CxCR1_ACK                             ((uint32_t)0x00000010)
#define I2CxSR_BB                               ((uint32_t)0x00000020)
#define I2CxSR_LRB                              ((uint32_t)0x00000001)
#define I2CxOP_RSTA                             ((uint32_t)0x00000008)
#define I2CxOP_SREN                             ((uint32_t)0x00000002)
#define I2CxOP_MFACK                            ((uint32_t)0x00000001)
#define I2CxOP_INIT                             ((uint32_t)0x00000084)
#define I2CxIE_CLEAR                            ((uint32_t)0x00000000)
#define I2CxPRS_PRCK                            ((uint32_t)0x0000000F)
#define I2CxDBR_DB_MASK                         ((uint32_t)0x000000FF)
#define I2CxOP_SLAVE_INIT                       ((uint32_t)0x00000084)
#define I2CAR_SA_MASK                           ((uint32_t)0x000000FE)
#define I2CxSR_TRX                              ((uint32_t)0x00000040)
#define I2CxOP_SAST                             ((uint32_t)0x00000020)
#define I2CxIE_INTI2C                           ((uint32_t)0x00000001)
#define I2C_NO_DATA                             (0)
#define I2C_READ_ADDRESSED                      (1)
#define I2C_WRITE_ADDRESSED                     (3)
#define I2C_ACK                                 (1)
#define I2C_TIMEOUT                             (100000)
// DAC
#define DAC_START                               ((uint32_t)0x00000001)
#define DAC_STOP                                ((uint32_t)0x00000000)
// ADC
#define ADC_12BIT_RANGE                         0xFFF
#define ADC_SCLK_1                              ((uint32_t)0x00000000)
#define ADxMOD0_RCUT_NORMAL                     ((uint32_t)0x00000000)
#define ADxMOD0_DACON_ON                        ((uint32_t)0x00000001)
#define ADxTSETn_ENINT_DISABLE                  ((uint32_t)0x00000000)
#define ADxTSETn_TRGS_SGL                       ((uint32_t)0x00000040)
#define ADxCR1_CNTDMEN_DISABLE                  ((uint32_t)0x00000000)
#define ADxCR1_SGLDMEN_DISABLE                  ((uint32_t)0x00000000)
#define ADxCR1_TRGDMEN_DISABLE                  ((uint32_t)0x00000000)
#define ADxCR1_TRGEN_DISABLE                    ((uint32_t)0x00000000)
#define ADxCR0_ADEN_DISABLE                     ((uint32_t)0x00000000)
#define ADxCR0_ADEN_ENABLE                      ((uint32_t)0x00000080)
#define ADxCR0_SGL_ENABLE                       ((uint32_t)0x00000002)
#define ADxCR0_CNT_DISABLE                      ((uint32_t)0x00000000)
#define ADxST_SNGF_IDLE                         ((uint32_t)0x00000000)
#define ADxST_SNGF_RUN                          ((uint32_t)0x00000004)
#define ADxREGn_ADRFn_MASK                      ((uint32_t)0x00000001)
#define ADxREGn_ADRFn_ON                        ((uint32_t)0x00000001)
#define ADxREGn_ADRn_MASK                       ((uint32_t)0x0000FFF0)
// Timer & PWM
#define T32A_INT_MASK                           ((uint32_t)0x0000000F)
#define T32A_MODE_32                            ((uint32_t)0x00000001)
#define T32A_PRSCLx_32                          ((uint32_t)0x30000000)
#define T32A_IMUFx_MASK_REQ                     ((uint32_t)0x00000008)
#define T32A_IMOFx_MASK_REQ                     ((uint32_t)0x00000004)
#define T32A_COUNT_STOP                         ((uint32_t)0x00000004)
#define T32A_COUNT_START                        ((uint32_t)0x00000002)
#define T32A_RUN_ENABLE                         ((uint32_t)0x00000001)
#define T32A_DBG_HALT_STOP                      ((uint32_t)0x00000002)
#define T32A_COUNT_DONT_START                   ((uint32_t)0x00000000)
#define T32A_RUN_DISABLE                        ((uint32_t)0x00000000)
#define T32A_WBF_ENABLE                         ((uint32_t)0x00100000)
#define T32A_RELOAD_TREGx                       ((uint32_t)0x00000700)
#define T32A_OCRCMPx0_SET                       ((uint32_t)0x00000001)
#define T32A_OCRCMPx1_CLR                       ((uint32_t)0x00000008)
#define T32A_OCR_DISABLE                        ((uint32_t)0x00000000)
#define DEFAULT_PERIOD                          0.02f     // 20ms
#define DEFAULT_CLOCK_DIVISION                  32
#define MAX_COUNTER_16B                         0xFFFF
// UART
#define UART_ENABLE_RX                          ((uint32_t)0x00000001)
#define UART_ENABLE_TX                          ((uint32_t)0x00000002)
#define UARTxSWRST_SWRSTF_MASK                  ((uint32_t)0x00000080)
#define UARTxSWRST_SWRSTF_RUN                   ((uint32_t)0x00000080)
#define UARTxSWRST_SWRST_10                     ((uint32_t)0x00000002)
#define UARTxSWRST_SWRST_01                     ((uint32_t)0x00000001)
#define UARTxFIFOCLR_TFCLR_CLEAR                ((uint32_t)0x00000002)
#define UARTxFIFOCLR_RFCLR_CLEAR                ((uint32_t)0x00000001)
#define UART_PLESCALER_1                        ((uint32_t)0x00000000)
#define UART_DIVISION_ENABLE                    ((uint32_t)0x00800000)
#define UART_TX_INT_ENABLE                      ((uint32_t)0x00000040)
#define UART_RX_INT_ENABLE                      ((uint32_t)0x00000010)
#define UART_RX_FIFO_FILL_LEVEL                 ((uint32_t)0x00000100)
#define UART_RANGE_K_MIN                        ((uint32_t)0x00000000)
#define UART_RANGE_K_MAX                        ((uint32_t)0x0000003F)
#define UART_RANGE_N_MIN                        ((uint32_t)0x00000001)
#define UART_RANGE_N_MAX                        ((uint32_t)0x0000FFFF)
#define UART_NUM                                3

typedef struct {
    uint32_t ken;  // Enable/Disable Division Definition
    uint32_t brk;  // Division Value K
    uint32_t brn;  // Division Value N
} uart_boudrate_t;

// Sleep
#define CG_STBY_MODE_IDLE                        0x0
#define CG_STBY_MODE_STOP1                       0x1
#define EXTERNEL_OSC_MASK                        0xFFFFFFF9
#define IHOSC_CFG_WARM_UP_TIME                   ((uint64_t)(5000))
#define IHOSC_CFG_CLOCK                          ((uint64_t)(10000000))
#define CGWUPHCR_WUPT_HIGH_MASK                  ((uint32_t)0xFFF00000)

#endif // __GPIO_INCLUDE_H
