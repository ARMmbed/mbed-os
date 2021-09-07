/***************************************************************************//**
* \file cyhal_hwmgr.c
*
* \brief
* Provides a high level interface for managing hardware resources. This is
* used to track what hardware blocks are already being used so they are not over
* allocated.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cyhal_hwmgr.h"
#include "cyhal_hwmgr_impl.h"
#include "cyhal_interconnect.h"
#include "cyhal_system.h"
#include "cmsis_compiler.h"
#include "cyhal_scb_common.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/*******************************************************************************
*       Defines
*******************************************************************************/

#if defined(CY_IP_MXS40PASS_SAR_INSTANCES)
    #define CY_BLOCK_COUNT_ADC  (CY_IP_MXS40PASS_SAR_INSTANCES)
#elif defined (CY_IP_M0S8PASS4A_INSTANCES)
    #define CY_BLOCK_COUNT_ADC  (CY_IP_M0S8PASS4A_INSTANCES)
#else
    #define CY_BLOCK_COUNT_ADC      (0)
#endif

#if defined(CY_IP_MXBLESS_INSTANCES)
    #define CY_BLOCK_COUNT_BLE      CY_IP_MXBLESS_INSTANCES
#else
    #define CY_BLOCK_COUNT_BLE      (0)
#endif

#if defined(CY_IP_MXTTCANFD_INSTANCES)
    #define CY_BLOCK_COUNT_CAN      (CY_IP_MXTTCANFD_INSTANCES)
    #if (CY_IP_MXTTCANFD_INSTANCES == 0)
        #define CY_CHANNEL_COUNT_CAN (0u)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 1)
        #define CY_CHANNEL_COUNT_CAN (CANFD_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 2)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 3)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 4)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 5)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 6)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 7)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 8)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 9)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR + CANFD8_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES == 10)
        #define CY_CHANNEL_COUNT_CAN (CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR + CANFD8_CAN_NR + CANFD9_CAN_NR)
    #elif (CY_IP_MXTTCANFD_INSTANCES > 10)
        #warning Unhandled CAN instance count
    #endif
#elif defined(CY_IP_M0S8CAN_INSTANCES)
    #define CY_BLOCK_COUNT_CAN      (CY_IP_M0S8CAN_INSTANCES)
    #define CY_CHANNEL_COUNT_CAN    (1)
#else
    #define CY_BLOCK_COUNT_CAN      (0)
    #define CY_CHANNEL_COUNT_CAN    (0)
#endif

#if defined(COMPONENT_CAT1A)
// 12 dedicated = IMO, EXT, ILO, FLL, LF, Pump, BAK, Timer, AltSysTick, Slow, Fast, Peri
//  7 optional =  ECO, ALTHF, ALTLF, PILO, WCO, MFO, MF
#define CY_CHANNEL_COUNT_CLOCK      (12 + 7 + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + \
                                    PERI_DIV_8_NR + PERI_DIV_16_NR + PERI_DIV_16_5_NR + PERI_DIV_24_5_NR)
#elif defined(COMPONENT_CAT2)
// 7 dedicated = IMO, EXT, ILO, HF, LF, PUMP, SYSCLK
// 3 optional  = ECO, WCO, PLL, PLLSEL, WDCSEL
#define CY_CHANNEL_COUNT_CLOCK      (7 + 5 + PERI_PCLK_CLOCK_NR)
#endif

#if defined(CY_IP_MXCRYPTO_INSTANCES)
    #define CY_BLOCK_COUNT_CRYPTO   (CY_IP_MXCRYPTO_INSTANCES)
#elif defined(CPUSS_CRYPTO_PRESENT)
    #define CY_BLOCK_COUNT_CRYPTO   (CPUSS_CRYPTO_PRESENT)
#else
    #define CY_BLOCK_COUNT_CRYPTO   (0)
#endif

#if defined(CY_IP_MXS40PASS_CTDAC_INSTANCES)
    #define CY_BLOCK_COUNT_DAC      (CY_IP_MXS40PASS_CTDAC_INSTANCES)
#else
    #define CY_BLOCK_COUNT_DAC      (0)
#endif

#if defined(CY_IP_M4CPUSS_DMAC_INSTANCES)
    #define CY_BLOCK_COUNT_DMA      (CY_IP_M4CPUSS_DMAC_INSTANCES)
    #define CY_CHANNEL_COUNT_DMA    (CPUSS_DMAC_CH_NR)
#elif defined(CPUSS_CPUMEMSS_DMAC_PRESENT)
    #define CY_BLOCK_COUNT_DMA      (CPUSS_CPUMEMSS_DMAC_PRESENT)
    #define CY_CHANNEL_COUNT_DMA    (CPUSS_DMAC_CH_NR)
#else
    #define CY_BLOCK_COUNT_DMA      (0)
    #define CY_CHANNEL_COUNT_DMA    (0)
#endif

#if defined(CY_IP_M4CPUSS_DMA_INSTANCES)
    #define CY_BLOCK_COUNT_DW       (CY_IP_M4CPUSS_DMA_INSTANCES)
    #define CY_CHANNEL_COUNT_DW     (CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR)
#else
    #define CY_BLOCK_COUNT_DW       (0)
    #define CY_CHANNEL_COUNT_DW     (0)
#endif

#if defined(IOSS_GPIO_GPIO_PORT_NR)
    #define CY_BLOCK_COUNT_GPIO     (IOSS_GPIO_GPIO_PORT_NR)
    #define CY_CHANNEL_COUNT_GPIO   (8 * IOSS_GPIO_GPIO_PORT_NR)
#else
    #define CY_BLOCK_COUNT_GPIO     (0)
    #define CY_CHANNEL_COUNT_GPIO   (0)
#endif

#if defined(CY_IP_MXAUDIOSS_INSTANCES)
    #define CY_BLOCK_COUNT_I2S      (CY_IP_MXAUDIOSS_INSTANCES)
#else
    #define CY_BLOCK_COUNT_I2S      (0)
#endif

#if defined(CY_IP_MXLCD_INSTANCES)
    #define CY_BLOCK_COUNT_LCD      (CY_IP_MXLCD_INSTANCES)
#elif defined(CY_IP_M0S8LCD_INSTANCES)
    #define CY_BLOCK_COUNT_LCD      (CY_IP_M0S8LCD_INSTANCES)
#else
    #define CY_BLOCK_COUNT_LCD      (0)
#endif

#if defined(CY_IP_MXLPCOMP_INSTANCES)
    #define CY_BLOCK_COUNT_LPCOMP   (2 * CY_IP_MXLPCOMP_INSTANCES)
#elif defined(CY_IP_M0S8LPCOMP_INSTANCES)
    #define CY_BLOCK_COUNT_LPCOMP   (2 * CY_IP_M0S8LPCOMP_INSTANCES)
#else
    #define CY_BLOCK_COUNT_LPCOMP   (0)
#endif

#if defined(PASS_NR_CTBS)
    #define CY_BLOCK_COUNT_OPAMP    (2 * PASS_NR_CTBS)
#else
    #define CY_BLOCK_COUNT_OPAMP    (0)
#endif

#if defined(CY_IP_MXAUDIOSS_INSTANCES)
    #define CY_BLOCK_COUNT_PDMPCM   (CY_IP_MXAUDIOSS_INSTANCES)
#else
    #define CY_BLOCK_COUNT_PDMPCM   (0)
#endif

#if defined(CY_IP_MXSMIF_INSTANCES)
    #define CY_BLOCK_COUNT_QSPI     (CY_IP_MXSMIF_INSTANCES)
#else
    #define CY_BLOCK_COUNT_QSPI     (0)
#endif

#if defined(CY_IP_MXS40SRSS_RTC_INSTANCES)
    #define CY_BLOCK_COUNT_RTC      (CY_IP_MXS40SRSS_RTC_INSTANCES)
#else
    #define CY_BLOCK_COUNT_RTC      (0)
#endif

#if defined(CY_IP_MXSCB_INSTANCES) || defined(CY_IP_M0S8SCB_INSTANCES)
    #define CY_BLOCK_COUNT_SCB      (_SCB_ARRAY_SIZE)
#else
    #define CY_BLOCK_COUNT_SCB      (0)
#endif

#if defined(CY_IP_MXSDHC_INSTANCES)
    #define CY_BLOCK_COUNT_SDHC     (CY_IP_MXSDHC_INSTANCES)
#else
    #define CY_BLOCK_COUNT_SDHC     (0)
#endif

#if defined(CY_IP_MXTCPWM_INSTANCES)
    #define CY_BLOCK_COUNT_TCPWM    CY_IP_MXTCPWM_INSTANCES
    #if (CY_IP_MXTCPWM_VERSION == 1)
        #if (CY_IP_MXTCPWM_INSTANCES == 0)
            #define CY_CHANNEL_COUNT_TCPWM (0u)
        #elif (CY_IP_MXTCPWM_INSTANCES == 1)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 2)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 3)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 4)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 5)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 6)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 7)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 8)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 9)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR + TCPWM8_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES == 10)
            #define CY_CHANNEL_COUNT_TCPWM (TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR + TCPWM8_CNT_NR + TCPWM9_CNT_NR)
        #elif (CY_IP_MXTCPWM_INSTANCES > 10)
            #warning Unhandled TCPWM instance count
        #endif
    #elif (CY_IP_MXTCPWM_VERSION == 2)
        #if (CY_IP_MXTCPWM_INSTANCES == 1)
            #if (TCPWM_GRP_NR == 0)
                #define CY_CHANNEL_COUNT_TCPWM (0u)
            #elif (TCPWM_GRP_NR == 1)
                #define CY_CHANNEL_COUNT_TCPWM (TCPWM_GRP_NR0_GRP_GRP_CNT_NR)
            #elif (TCPWM_GRP_NR == 2)
                #define CY_CHANNEL_COUNT_TCPWM (TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR)
            #elif (TCPWM_GRP_NR == 3)
                #define CY_CHANNEL_COUNT_TCPWM (TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR + TCPWM_GRP_NR2_GRP_GRP_CNT_NR)
            #elif (TCPWM_GRP_NR == 4)
                #define CY_CHANNEL_COUNT_TCPWM (TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR + TCPWM_GRP_NR2_GRP_GRP_CNT_NR + TCPWM_GRP_NR3_GRP_GRP_CNT_NR)
            #elif (TCPWM_GRP_NR > 4)
                #warning Unhandled TCPWM instance count
            #endif
        #elif (CY_IP_MXTCPWM_INSTANCES > 1)
            #warning Unhandled TCPWM instance count
        #endif
    #else
        #warning Unrecognized TCPWM IP version
    #endif
#elif defined(CY_IP_M0S8TCPWM_INSTANCES)
    #define CY_BLOCK_COUNT_TCPWM        (CY_IP_M0S8TCPWM_INSTANCES)
    #if (CY_IP_M0S8TCPWM_INSTANCES == 0)
        #define CY_CHANNEL_COUNT_TCPWM  (0u)
    #elif (CY_IP_M0S8TCPWM_INSTANCES == 1)
        #define CY_CHANNEL_COUNT_TCPWM  (TCPWM_CNT_NR)
    #else
        #warning Unhandled TCPWM instance count
    #endif
#else
    #define CY_BLOCK_COUNT_TCPWM    (0)
    #define CY_CHANNEL_COUNT_TCPWM  (0)
#endif

#if defined(CY_IP_MXUDB_INSTANCES)
    #define CY_BLOCK_COUNT_UDB      CY_IP_MXUDB_INSTANCES
#else
    #define CY_BLOCK_COUNT_UDB      (0)
#endif

#if defined(CY_IP_MXUSBFS_INSTANCES)
    #define CY_BLOCK_COUNT_USB      CY_IP_MXUSBFS_INSTANCES
#else
    #define CY_BLOCK_COUNT_USB      (0)
#endif

#if defined(CY_IP_MXUSBPD_INSTANCES)
    #define CY_BLOCK_COUNT_USBPD    CY_IP_MXUSBPD_INSTANCES
#else
    #define CY_BLOCK_COUNT_USBPD    (0)
#endif

#if defined(CY_IP_MXS40SRSS_MCWDT_INSTANCES)
    #define CY_BLOCK_COUNT_MCWDT    CY_IP_MXS40SRSS_MCWDT_INSTANCES
#elif (defined(CY_IP_S8SRSSLT_INSTANCES) && defined(CY_IP_M0S8WCO))
    #define CY_BLOCK_COUNT_MCWDT    CY_IP_M0S8WCO_INSTANCES
#else
    #define CY_BLOCK_COUNT_MCWDT    (0)
#endif

/*
    All resources have an offset and a size, offsets are stored in an array
    Subsequent resource offset equals the preceding offset + size
    Offsets are bit indexes in the arrays that track used, configured etc.

    Channel based resources have an extra array for block offsets

    Note these are NOT offsets into the device's MMIO address space;
    they are bit offsets into arrays that are internal to the HW mgr.
*/


#define CY_OFFSET_ADC      0
#define CY_SIZE_ADC        CY_BLOCK_COUNT_ADC
#define CY_OFFSET_BLE      (CY_OFFSET_ADC + CY_SIZE_ADC)
#define CY_SIZE_BLE        CY_BLOCK_COUNT_BLE
#define CY_OFFSET_CAN      (CY_OFFSET_BLE + CY_SIZE_BLE)
#define CY_SIZE_CAN        CY_CHANNEL_COUNT_CAN
#define CY_OFFSET_CLOCK    (CY_OFFSET_CAN + CY_SIZE_CAN)
#define CY_SIZE_CLOCK      CY_CHANNEL_COUNT_CLOCK
#define CY_OFFSET_CRYPTO   (CY_OFFSET_CLOCK + CY_SIZE_CLOCK)
#define CY_SIZE_CRYPTO     CY_BLOCK_COUNT_CRYPTO
#define CY_OFFSET_DAC      (CY_OFFSET_CRYPTO + CY_SIZE_CRYPTO)
#define CY_SIZE_DAC        CY_BLOCK_COUNT_DAC
#define CY_OFFSET_DMA      (CY_OFFSET_DAC + CY_SIZE_DAC)
#define CY_SIZE_DMA        CY_CHANNEL_COUNT_DMA
#define CY_OFFSET_DW       (CY_OFFSET_DMA + CY_SIZE_DMA)
#define CY_SIZE_DW         CY_CHANNEL_COUNT_DW
#define CY_OFFSET_GPIO     (CY_OFFSET_DW + CY_SIZE_DW)
#define CY_SIZE_GPIO       CY_CHANNEL_COUNT_GPIO
#define CY_OFFSET_I2S      (CY_OFFSET_GPIO + CY_SIZE_GPIO)
#define CY_SIZE_I2S        CY_BLOCK_COUNT_I2S
#define CY_OFFSET_LCD      (CY_OFFSET_I2S + CY_SIZE_I2S)
#define CY_SIZE_LCD        CY_BLOCK_COUNT_LCD
#define CY_OFFSET_LPCOMP   (CY_OFFSET_LCD + CY_SIZE_LCD)
#define CY_SIZE_LPCOMP     CY_BLOCK_COUNT_LPCOMP
#define CY_OFFSET_LPTIMER  (CY_OFFSET_LPCOMP + CY_SIZE_LPCOMP)
#define CY_SIZE_LPTIMER    CY_BLOCK_COUNT_MCWDT
#define CY_OFFSET_OPAMP    (CY_OFFSET_LPTIMER + CY_SIZE_LPTIMER)
#define CY_SIZE_OPAMP      CY_BLOCK_COUNT_OPAMP
#define CY_OFFSET_PDMPCM   (CY_OFFSET_OPAMP + CY_SIZE_OPAMP)
#define CY_SIZE_PDMPCM     CY_BLOCK_COUNT_PDMPCM
#define CY_OFFSET_QSPI     (CY_OFFSET_PDMPCM + CY_SIZE_PDMPCM)
#define CY_SIZE_QSPI       CY_BLOCK_COUNT_QSPI
#define CY_OFFSET_RTC      (CY_OFFSET_QSPI + CY_SIZE_QSPI)
#define CY_SIZE_RTC        CY_BLOCK_COUNT_RTC
#define CY_OFFSET_SCB      (CY_OFFSET_RTC + CY_SIZE_RTC)
#define CY_SIZE_SCB        CY_BLOCK_COUNT_SCB
#define CY_OFFSET_SDHC     (CY_OFFSET_SCB + CY_SIZE_SCB)
#define CY_SIZE_SDHC       CY_BLOCK_COUNT_SDHC
#define CY_OFFSET_TCPWM    (CY_OFFSET_SDHC + CY_SIZE_SDHC)
#define CY_SIZE_TCPWM      CY_CHANNEL_COUNT_TCPWM
#define CY_OFFSET_UDB      (CY_OFFSET_TCPWM + CY_SIZE_TCPWM)
#define CY_SIZE_UDB        CY_BLOCK_COUNT_UDB
#define CY_OFFSET_USB      (CY_OFFSET_UDB + CY_SIZE_UDB)
#define CY_SIZE_USB        CY_BLOCK_COUNT_USB
#define CY_OFFSET_USBPD    (CY_OFFSET_USB + CY_SIZE_USB)
#define CY_SIZE_USBPD      CY_BLOCK_COUNT_USBPD

#define CY_TOTAL_ALLOCATABLE_ITEMS     (CY_OFFSET_USBPD + CY_SIZE_USBPD)

#define CY_BYTE_NUM_SHIFT (3)
#define CY_BIT_NUM_MASK (0x07)

/*******************************************************************************
*       Variables
*******************************************************************************/

#if defined(COMPONENT_CAT1A)

#define PERI_DIV_NR (PERI_DIV_8_NR + PERI_DIV_16_NR + PERI_DIV_16_5_NR + PERI_DIV_24_5_NR)
#if ((PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 18) >= 256)
#error "Too many clocks to use uint8_t as offset type"
#endif
/* The order of items here must match the order in cyhal_clock_impl.h
 *
 * Each entry in the array below is the prior entry plus the number of clocks that exist
 * of the prior type. When there is only 1 clock (e.g: IMO/ECO) the next number is simply
 * one higher than the previous value. When there are multiple clocks (e.g.: PathMux/PLL)
 * the subsequent value is increased by the define that specifies how many clocks are
 * actually present. */
static const uint8_t cyhal_block_offsets_clock[26] =
{
    0,                                                                    // 8-bit dividers
    PERI_DIV_8_NR,                                                        // 16-bit dividers
    PERI_DIV_8_NR + PERI_DIV_16_NR,                                       // 16.5 bit dividers
    PERI_DIV_8_NR + PERI_DIV_16_NR + PERI_DIV_16_5_NR,                    // 24.5 bit dividers

    PERI_DIV_NR,                                                          // IMO
    PERI_DIV_NR + 1,                                                      // ECO
    PERI_DIV_NR + 2,                                                      // EXT
    PERI_DIV_NR + 3,                                                      // ALTHF
    PERI_DIV_NR + 4,                                                      // ALTLF
    PERI_DIV_NR + 5,                                                      // ILO
    PERI_DIV_NR + 6,                                                      // PILO
    PERI_DIV_NR + 7,                                                      // WCO
    PERI_DIV_NR + 8,                                                      // MFO

    PERI_DIV_NR + 9,                                                      // PathMux

    PERI_DIV_NR + SRSS_NUM_CLKPATH + 9,                                   // FLL
    PERI_DIV_NR + SRSS_NUM_CLKPATH + 10,                                  // PLL

    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + 10,                   // LF
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + 11,                   // MF
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + 12,                   // HF

    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 12, // PUMP
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 13, // BAK
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 14, // TIMER
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 15, // AltSysTick

    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 16, // Fast
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 17, // Peri
    PERI_DIV_NR + SRSS_NUM_CLKPATH + SRSS_NUM_PLL + SRSS_NUM_HFROOT + 18, // Slow
};

#elif defined(COMPONENT_CAT2)
/* The order of items here must match the order in cyhal_clock_impl.h
 *
 * Each entry in the array below is the prior entry plus the number of clocks that exist
 * of the prior type. When there is only 1 clock (e.g: IMO/ECO) the next number is simply
 * one higher than the previous value. When there are multiple clocks (e.g.: PCLK)
 * the subsequent value is increased by the define that specifies how many clocks are
 * actually present. */
static const uint8_t cyhal_block_offsets_clock[16] =
{
    0,                                                                  // 8-bit dividers
    PERI_PCLK_DIV_8_NR,                                                 // 16-bit dividers
    PERI_PCLK_DIV_8_NR + PERI_PCLK_DIV_16_NR,                           // 16.5 bit dividers
    PERI_PCLK_DIV_8_NR + PERI_PCLK_DIV_16_NR + PERI_PCLK_DIV_16_5_NR,   // 24.5 bit dividers

    PERI_PCLK_CLOCK_NR,                                                 // IMO
    PERI_PCLK_CLOCK_NR + 1,                                             // ECO
    PERI_PCLK_CLOCK_NR + 2,                                             // EXT
    PERI_PCLK_CLOCK_NR + 3,                                             // ILO
    PERI_PCLK_CLOCK_NR + 4,                                             // WCO

    PERI_PCLK_CLOCK_NR + 5,                                             // WDCSEL
    PERI_PCLK_CLOCK_NR + 6,                                             // PLLSEL
    PERI_PCLK_CLOCK_NR + 7,                                             // PLL
    PERI_PCLK_CLOCK_NR + 8,                                             // LF
    PERI_PCLK_CLOCK_NR + 9,                                             // HF
    PERI_PCLK_CLOCK_NR + 10,                                            // PUMP
    PERI_PCLK_CLOCK_NR + 11,                                            // SYS
};
#endif

static const uint8_t cyhal_block_offsets_dma[] =
{
    0,
};

#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
static const uint8_t cyhal_block_offsets_dw[] =
{
    0,
    CPUSS_DW0_CH_NR,
};
#endif

static const uint8_t cyhal_block_offsets_gpio[] =
{
    0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120,
};

static const uint8_t cyhal_block_offsets_can[] =
{
#ifdef CY_IP_MXTTCANFD_INSTANCES
    #if (CY_IP_MXTTCANFD_INSTANCES > 0)
        0,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 1)
        CANFD0_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 2)
        CANFD0_CAN_NR + CANFD1_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 3)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 4)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 5)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 6)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 7)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 8)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 9)
        CANFD0_CAN_NR + CANFD1_CAN_NR + CANFD2_CAN_NR + CANFD3_CAN_NR + CANFD4_CAN_NR + CANFD5_CAN_NR + CANFD6_CAN_NR + CANFD7_CAN_NR + CANFD8_CAN_NR,
    #endif
    #if (CY_IP_MXTTCANFD_INSTANCES > 10)
        #warning Unhandled CAN instance count
    #endif
#else
    0
#endif
};

static const uint8_t cyhal_block_offsets_tcpwm[] =
{
    0,
#ifdef CY_IP_MXTCPWM_INSTANCES
    #if CY_IP_MXTCPWM_VERSION == 1
        #if (CY_IP_MXTCPWM_INSTANCES > 1)
            TCPWM0_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 2)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 3)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 4)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 5)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 6)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 7)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 8)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 9)
            TCPWM0_CNT_NR + TCPWM1_CNT_NR + TCPWM2_CNT_NR + TCPWM3_CNT_NR + TCPWM4_CNT_NR + TCPWM5_CNT_NR + TCPWM6_CNT_NR + TCPWM7_CNT_NR + TCPWM8_CNT_NR,
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 10)
            #warning Unhandled TCPWM instance count
        #endif
    #else // CY_IP_MXTCPWM_VERSION >= 2
        #if (CY_IP_MXTCPWM_INSTANCES == 1)
            #if (TCPWM_GRP_NR > 1)
                TCPWM_GRP_NR0_GRP_GRP_CNT_NR,
            #endif
            #if (TCPWM_GRP_NR > 2)
                TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR,
            #endif
            #if (TCPWM_GRP_NR > 3)
                TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR + TCPWM_GRP_NR2_GRP_GRP_CNT_NR,
            #endif
        #else
            #warning Unhandled TCPWM instance count
        #endif
    #endif
#endif
};

static uint8_t cyhal_used[(CY_TOTAL_ALLOCATABLE_ITEMS + 7) / 8] = {0};

// Note: the ordering here needs to be parallel to that of cyhal_resource_t
static const uint16_t cyhal_resource_offsets[] =
{
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
    CY_OFFSET_ADC,
    CY_OFFSET_BLE,
    CY_OFFSET_CAN,
    CY_OFFSET_CLOCK,    /* Placeholder for ClockPath which is deprecated */
    CY_OFFSET_CLOCK,
    CY_OFFSET_CRYPTO,
    CY_OFFSET_DAC,
    CY_OFFSET_DMA,
    CY_OFFSET_DW,
    CY_OFFSET_GPIO,
    CY_OFFSET_I2S,
    CY_OFFSET_LCD,
    CY_OFFSET_LPCOMP,
    CY_OFFSET_LPTIMER,
    CY_OFFSET_OPAMP,
    CY_OFFSET_PDMPCM,
    CY_OFFSET_QSPI,
    CY_OFFSET_RTC,
    CY_OFFSET_SCB,
    CY_OFFSET_SDHC,
    CY_OFFSET_TCPWM,
    CY_OFFSET_UDB,
    CY_OFFSET_USB,
#elif defined(COMPONENT_CAT2)
    CY_OFFSET_ADC,
    CY_OFFSET_CAN,
    CY_OFFSET_CLOCK,
    CY_OFFSET_CRYPTO,
    CY_OFFSET_DMA,
    CY_OFFSET_GPIO,
    CY_OFFSET_I2S,
    CY_OFFSET_LCD,
    CY_OFFSET_LPCOMP,
    CY_OFFSET_LPTIMER,
    CY_OFFSET_OPAMP,
    CY_OFFSET_SCB,
    CY_OFFSET_TCPWM,
    CY_OFFSET_USB,
    CY_OFFSET_USBPD,
#endif
};

static const uint32_t cyhal_has_channels =
    (1 << CYHAL_RSC_CAN)   |
    (1 << CYHAL_RSC_CLOCK) |
    (1 << CYHAL_RSC_DMA)   |
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
    (1 << CYHAL_RSC_DW)    |
#endif
    (1 << CYHAL_RSC_GPIO)  |
    (1 << CYHAL_RSC_TCPWM) ;

/*
 * This function is designed to verify that the number of valid resources in the cyhal_resource_t
 * enum and the number entries in the cyhal_resource_offsets array are identical. Any mismatch
 * between the two will lead to runtime failures. This will produce a divide by 0 error if they
 * get of of sync.
 * NOTE: This function should never be called, it is only for a compile time error check
 * NOTE: The Supress is to temporaraly disable the IAR warning about an uncalled function
 */
static inline void check_array_size(void) __attribute__ ((deprecated));
#if __ICCARM__
#pragma diag_suppress=Pe177
#elif __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif
static inline void check_array_size(void)
{
    uint32_t dummy = 1 / (sizeof(cyhal_resource_offsets) == (sizeof(uint16_t) * CYHAL_RSC_INVALID));
    (void)dummy;
}
#if __ICCARM__
#pragma diag_default=Pe177
#elif __clang__
#pragma clang diagnostic pop
#endif

/*******************************************************************************
*       Utility helper functions
*******************************************************************************/

static inline uint16_t cyhal_uses_channels(cyhal_resource_t type)
{
    return (cyhal_has_channels & (1 << type)) > 0;
}

static inline uint16_t cyhal_get_resource_offset(cyhal_resource_t type)
{
    return cyhal_resource_offsets[type];
}

static inline const uint8_t* cyhal_get_block_offsets(cyhal_resource_t type)
{
    switch (type)
    {
        case CYHAL_RSC_CAN:
            return cyhal_block_offsets_can;
        case CYHAL_RSC_CLOCK:
            return cyhal_block_offsets_clock;
        case CYHAL_RSC_DMA:
            return cyhal_block_offsets_dma;
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
        case CYHAL_RSC_DW:
            return cyhal_block_offsets_dw;
#endif
        case CYHAL_RSC_GPIO:
            return cyhal_block_offsets_gpio;
        case CYHAL_RSC_TCPWM:
            return cyhal_block_offsets_tcpwm;
        default:
            CY_ASSERT(false);
            return NULL;
    }
}

// Gets the number of block offset entries, only valid for blocks which have channels.
static inline uint8_t cyhal_get_block_offset_length(cyhal_resource_t type)
{
    switch (type)
    {
        case CYHAL_RSC_CAN:
            return sizeof(cyhal_block_offsets_can)/sizeof(cyhal_block_offsets_can[0]);
        case CYHAL_RSC_CLOCK:
            return sizeof(cyhal_block_offsets_clock)/sizeof(cyhal_block_offsets_clock[0]);
        case CYHAL_RSC_DMA:
            return sizeof(cyhal_block_offsets_dma)/sizeof(cyhal_block_offsets_dma[0]);
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
        case CYHAL_RSC_DW:
            return sizeof(cyhal_block_offsets_dw)/sizeof(cyhal_block_offsets_dw[0]);
#endif
        case CYHAL_RSC_GPIO:
            return sizeof(cyhal_block_offsets_gpio)/sizeof(cyhal_block_offsets_gpio[0]);
        case CYHAL_RSC_TCPWM:
            return sizeof(cyhal_block_offsets_tcpwm)/sizeof(cyhal_block_offsets_tcpwm[0]);
        default:
            CY_ASSERT(false);
            return 0;
    }
}

static cy_rslt_t cyhal_get_bit_position(cyhal_resource_t type, uint8_t block, uint8_t channel, uint16_t* bitPosition)
{
#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
    /* For backwards compatability. */
    if (type == CYHAL_RSC_CLKPATH)
    {
        channel = block;
        block = CYHAL_CLOCK_BLOCK_PATHMUX;
        type = CYHAL_RSC_CLOCK;
    }
#endif

    uint16_t offsetRsc = cyhal_get_resource_offset(type);
    // Offset that is one past the beginning of the next resource (or one past the end of the array).
    // Our offset must be strictly less than that
    uint16_t offsetEndOfRsc = ((1u + type) < sizeof(cyhal_resource_offsets)/sizeof(cyhal_resource_offsets[0]))
        ? cyhal_get_resource_offset((cyhal_resource_t)(type + 1))
        : CY_TOTAL_ALLOCATABLE_ITEMS;

    if (cyhal_uses_channels(type))
    {
        const uint8_t* blockOffsets = cyhal_get_block_offsets(type);
        *bitPosition = offsetEndOfRsc;
        if (blockOffsets != NULL)
        {
            // Offset (from the beginning of the section for this block type) that is one past the end of
            // the requested block index. The channel number must be strictly less than that.
            uint16_t blocks = cyhal_get_block_offset_length(type);
            if (block < blocks)
            {
                *bitPosition = offsetRsc + blockOffsets[block] + channel;
                if ((block + 1) < blocks)
                {
                    offsetEndOfRsc = offsetRsc + blockOffsets[block + 1];
                }
            }
        }
    }
    else
    {
        *bitPosition = offsetRsc + block;
    }

    return (*bitPosition < offsetEndOfRsc)
        ? CY_RSLT_SUCCESS
        : CYHAL_HWMGR_RSLT_ERR_INVALID;
}

static inline cy_rslt_t cyhal_is_set(const uint8_t* used, cyhal_resource_t type, uint8_t block, uint8_t channel, bool* isSet)
{
    uint16_t bitPosition;
    cy_rslt_t status = cyhal_get_bit_position(type, block, channel, &bitPosition);
    if (status == CY_RSLT_SUCCESS)
    {
        uint8_t byte = (uint8_t)(bitPosition >> CY_BYTE_NUM_SHIFT);
        uint8_t bit = bitPosition & CY_BIT_NUM_MASK;
        *isSet = (used[byte] & (1 << bit));
    }
    return status;
}

static inline cy_rslt_t cyhal_set_bit(uint8_t* used, cyhal_resource_t type, uint8_t block, uint8_t channel)
{
    uint16_t bitPosition;
    cy_rslt_t status = cyhal_get_bit_position(type, block, channel, &bitPosition);
    if (status == CY_RSLT_SUCCESS)
    {
        uint8_t byte = (uint8_t)(bitPosition >> CY_BYTE_NUM_SHIFT);
        uint8_t bit = bitPosition & CY_BIT_NUM_MASK;
        used[byte] |= (1 << bit);
    }
    return status;
}

static inline cy_rslt_t cyhal_clear_bit(uint8_t* used, cyhal_resource_t type, uint8_t block, uint8_t channel)
{
    uint16_t bitPosition;
    cy_rslt_t status = cyhal_get_bit_position(type, block, channel, &bitPosition);
    if (status == CY_RSLT_SUCCESS)
    {
        uint8_t byte = (uint8_t)(bitPosition >> CY_BYTE_NUM_SHIFT);
        uint8_t bit = bitPosition & CY_BIT_NUM_MASK;
        used[byte] &= ~(1 << bit);
    }
    return status;
}

/*******************************************************************************
*       Hardware Manager API
*******************************************************************************/

cy_rslt_t cyhal_hwmgr_init(void)
{
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_hwmgr_reserve(const cyhal_resource_inst_t* obj)
{
    bool isSet;
    uint32_t state = cyhal_system_critical_section_enter();
    cy_rslt_t rslt = cyhal_is_set(cyhal_used, obj->type, obj->block_num, obj->channel_num, &isSet);
    if (rslt == CY_RSLT_SUCCESS && isSet)
    {
        rslt = CYHAL_HWMGR_RSLT_ERR_INUSE;
    }

    if (rslt == CY_RSLT_SUCCESS)
    {
        rslt = cyhal_set_bit(cyhal_used, obj->type, obj->block_num, obj->channel_num);
    }
    cyhal_system_critical_section_exit(state);

    return rslt;
}

void cyhal_hwmgr_free(const cyhal_resource_inst_t* obj)
{
    uint32_t state = cyhal_system_critical_section_enter();
    cy_rslt_t rslt = cyhal_clear_bit(cyhal_used, obj->type, obj->block_num, obj->channel_num);
    CY_UNUSED_PARAMETER(rslt); /* CY_ASSERT only processes in DEBUG, ignores for others */
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    cyhal_system_critical_section_exit(state);
}

cy_rslt_t _cyhal_hwmgr_allocate_with_connection(cyhal_resource_t type, const cyhal_source_t *src, const cyhal_dest_t *dest,
    _cyhal_hwmgr_get_output_source_t get_src, _cyhal_hwmgr_get_input_dest_t get_dest, cyhal_resource_inst_t *obj)
{
    uint16_t offsetStartOfRsc = cyhal_get_resource_offset(type);
    uint16_t offsetEndOfRsc = ((1u + type) < sizeof(cyhal_resource_offsets)/sizeof(cyhal_resource_offsets[0]))
        ? cyhal_get_resource_offset((cyhal_resource_t)(type + 1))
        : CY_TOTAL_ALLOCATABLE_ITEMS;
    bool usesChannels = cyhal_uses_channels(type);

    uint16_t count = offsetEndOfRsc - offsetStartOfRsc;
    uint8_t block = 0;
    uint8_t channel = 0;
    for (uint16_t i = 0; i < count; i++)
    {
        bool valid = true;
        if (NULL != src)
        {
            cyhal_dest_t destination = get_dest(block, channel);
            valid = _cyhal_can_connect_signal(*src, destination);
        }
        if (valid && NULL != dest)
        {
            cyhal_source_t source = get_src(block, channel);
            valid = _cyhal_can_connect_signal(source, *dest);
        }

        if (valid)
        {
            cyhal_resource_inst_t rsc = { type, block, channel };
            if (CY_RSLT_SUCCESS == cyhal_hwmgr_reserve(&rsc))
            {
                obj->type = type;
                obj->block_num = block;
                obj->channel_num = channel;
                return CY_RSLT_SUCCESS;
            }
        }

        if (usesChannels)
        {
            const uint8_t* blockOffsets = cyhal_get_block_offsets(type);
            uint16_t blocks = cyhal_get_block_offset_length(type);
            if ((block + 1) < blocks && blockOffsets[block + 1] <= (i + 1))
            {
                channel = 0;
                do
                {
                    block++;
                }
                while (block < blocks && blockOffsets[block] < (i + 1));
            }
            else
            {
                channel++;
            }
        }
        else
        {
            block++;
        }
    }

    return CYHAL_HWMGR_RSLT_ERR_NONE_FREE;
}

cy_rslt_t cyhal_hwmgr_allocate(cyhal_resource_t type, cyhal_resource_inst_t* obj)
{
    return _cyhal_hwmgr_allocate_with_connection(type, NULL, NULL, NULL, NULL, obj);
}

#if defined(__cplusplus)
}
#endif
