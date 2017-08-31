/**************************************************************************//**
 * @file     KM_app.h
 * @brief    CMSIS Cortex-M7 Core Peripheral Access Layer Header File for
 *           Device KM
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef KM_APP_H
#define KM_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup KM_Definitions KM Definitions
  This file defines all structures and symbols for KM:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup KM_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M# Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M# Processor Exceptions Numbers ***************************************************/

  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Device Specific Interrupt Numbers ********************************************************/
/* Use enum APP_CPU_IRQ_CFG from hal_device.h instead of this enum??? */
APP_CPU_APP_IRQ_CTIIRQ0_INT_IRQn = 0,
APP_CPU_APP_IRQ_CTIIRQ1_INT_IRQn = 1,
APP_CPU_APP_IRQ_I2C1_INT_IRQn = 2,
APP_CPU_APP_IRQ_I2C2_INT_IRQn = 3,
APP_CPU_APP_IRQ_I2C3_INT_IRQn = 4,
APP_CPU_APP_IRQ_I2S_INT_IRQn = 5,
APP_CPU_APP_IRQ_UART1_INT_IRQn = 6,
APP_CPU_APP_IRQ_UART2_INT_IRQn = 7,
APP_CPU_APP_IRQ_SPI1_INT_IRQn = 8,
APP_CPU_APP_IRQ_SPI2_INT_IRQn = 9,
APP_CPU_APP_IRQ_TIMER1_INT_IRQn = 10,
APP_CPU_APP_IRQ_TIMER2_INT_IRQn = 11,
APP_CPU_APP_IRQ_MDM_WDG_INT_IRQn = 12,
APP_CPU_APP_IRQ_PHY_WDG_INT_IRQn = 13,
APP_CPU_APP_IRQ_RF_WDG_INT_IRQn = 14,
APP_CPU_APP_IRQ_SYSCTRL_ERR_INT_IRQn = 15,
APP_CPU_APP_IRQ_SYSCTRL_INT_IRQn = 16,
APP_CPU_APP_IRQ_DMAC_ERR_INT_IRQn = 17,
APP_CPU_APP_IRQ_DMAC_TC_INT_IRQn = 18,
APP_CPU_APP_IRQ_DMAC_COMB_INT_IRQn = 19,
APP_CPU_APP_IRQ_SDIO_INT_IRQn = 20,
APP_CPU_APP_IRQ_OSPI_INT_IRQn = 21,
APP_CPU_APP_IRQ_PSRAM_INT_IRQn = 22,
APP_CPU_APP_IRQ_CRYPTO_INT_IRQn = 23,
APP_CPU_APP_IRQ_DVS_GLB_INT_IRQn = 24,
APP_CPU_APP_IRQ_DVS_CORE_INT_IRQn = 25,
APP_CPU_APP_IRQ_USB_INT_IRQn = 26,
APP_CPU_APP_IRQ_USB_DMA_INT_IRQn = 27,
APP_CPU_APP_IRQ_PIO_1_INT_IRQn = 28,
APP_CPU_APP_IRQ_PIO_2_INT_IRQn = 29,
APP_CPU_APP_IRQ_PIO_WAKEUP_INT_IRQn = 30,
APP_CPU_APP_IRQ_PWRCTRL_ERR_INT_IRQn = 31,
APP_CPU_APP_IRQ_PWRCTRL_INT_IRQn = 32,
APP_CPU_APP_IRQ_BB_IPC_MBX_TX0_IRQn = 33,
APP_CPU_APP_IRQ_BB_IPC_MBX_TX1_IRQn = 34,
APP_CPU_APP_IRQ_BB_IPC_MBX_TX2_IRQn = 35,
APP_CPU_APP_IRQ_BB_IPC_MBX_RX0_IRQn = 36,
APP_CPU_APP_IRQ_BB_IPC_MBX_RX1_IRQn = 37,
APP_CPU_APP_IRQ_BB_IPC_MBX_RX2_IRQn = 38,
APP_CPU_APP_IRQ_GNSS_TIM_IRQn = 39,
APP_CPU_APP_IRQ_OTP_FAULT_IRQn = 40,
APP_CPU_APP_IRQ_ONFI_CAL_IRQn = 41,
APP_CPU_APP_IRQ_PWRCTRL_WAKEUP_IRQn = 42,
APP_CPU_APP_IRQ_ETR_LIMITER_THRESHOLD_IRQn = 43,
APP_CPU_APP_IRQ_USB_WAKEUP_INT_IRQn = 44,
APP_CPU_NUM_IRQS = 45
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M# Processor and Core Peripherals */
/* ToDo: set the defines according your Device                                                    */
#define __CM7_REV                 0x0101    /*!< Core Revision r?p?                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __FPU_PRESENT             0        /*!< FPU present or not                                */

/*@}*/ /* end of group KM_CMSIS */


#if !defined(WIN32)
#include <core_cm7.h>                       /* Cortex-M# processor and core peripherals           */
#endif
#include "system_KM_app.h"                /* KM System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup KM_Peripherals KM Peripherals
  KM Device Specific Peripheral registers structures
  @{
*/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#ifndef EXCLUDE_KM_PERIPHERALS
#include "akira_cm3_ss_device.h"
#include "akira_cm3_ss_ppb.h"
#include "ar1de_cr.h"
#include "ar1de_dbg.h"
#include "ar1de_mwu.h"
#include "arm_crypto310.h"
#include "aux_digif_apb_mmap.h"
#include "aux_dig_top.h"
#include "cortex_m3_dwt.h"
#include "cortex_m3_etm.h"
#include "cortex_m3_itm.h"
#include "cortex_m3_scs.h"
#include "cortex_m7_cti.h"
#include "cortex_m7_dwt.h"
#include "cortex_m7_etm.h"
#include "cortex_m7_itm.h"
#include "cortex_m7_scs.h"
#include "cti.h"
#include "cxatbfunnel.h"
#include "cxatbreplicator.h"
#include "cxtsgen.h"
#include "display_top.h"
#include "dl_bpe.h"
#include "dmac_pl081.h"
#include "etr_limiter.h"
#include "hal_debug.h"
#include "hal_device.h"
#include "hal_memory.h"
#include "hal_memory_defs.h"
#include "hwsequencer.h"
#include "hwsequencer_dbg.h"
#include "i2c.h"
#include "i2s.h"
#include "ic_rx_scratch_ram.h"
#include "ic_tx_scratch_ram.h"
#include "inari_cm7_ss_device.h"
#include "inari_cm7_ss_ppb.h"
#include "ipc_mbx.h"
#include "kmgo_rf_dig_ccu.h"
#include "kmgo_sysctrl.h"
#include "km_fbuf.h"
#include "km_gnss_apps_tim.h"
#include "km_go_config_regs.h"
#include "km_leti_cvp.h"
#include "km_ofdm_demux.h"
#include "km_pss.h"
#include "km_pwrctrl.h"
#include "km_sf.h"
#include "km_sss.h"
#include "km_ul_bpe.h"
#include "nic400_app_ic.h"
#include "ospi.h"
#include "otp_ctrl.h"
#include "pio.h"
#include "pio_onfi_cal.h"
#include "pll_dig_if.h"
#include "pmu.h"
#include "psram.h"
#include "pwm_top.h"
#include "rfdma.h"
#include "rffe.h"
#include "rx_apb_mmap_rxadc.h"
#include "rx_apb_mmap_rxbb.h"
#include "rx_apb_mmap_rxrf.h"
#include "rx_dig_top.h"
#include "sch.h"
#include "sdio_host.h"
#include "security.h"
#include "smurf_if_1ch.h"
#include "smurf_if_hwacc_config.h"
#include "spi.h"
#include "stm_cr.h"
#include "stm_filter.h"
#include "stm_stim.h"
#include "tcu_seqfifo.h"
#include "tdr_access_unit_apb.h"
#include "timer.h"
#include "tmc.h"
#include "tpiu.h"
#include "tscu.h"
#include "ts_mgr.h"
#include "tx_apb_mmap_txbb.h"
#include "tx_apb_mmap_txrf.h"
#include "tx_dig_top_txdfe1.h"
#include "tx_dig_top_txdfe2.h"
#include "uart.h"
#include "usbhs.h"
#include "usbhs_cfgs.h"
#include "usim.h"
#include "vcu_cr.h"
#include "vdma_cr.h"
#include "wdg.h"
/*@}*/ /* end of group KM_Peripherals */
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/** @addtogroup KM_MemoryMap KM Memory Mapping
  @{
*/

/*@}*/ /* end of group KM_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/** @addtogroup KM_PeripheralDecl KM Peripheral Declaration
  @{
*/

/*@}*/ /* end of group KM_PeripheralDecl */

/*@}*/ /* end of group KM_Definitions */

#ifdef __cplusplus
}
#endif

#endif  /* KM_APP_H */
