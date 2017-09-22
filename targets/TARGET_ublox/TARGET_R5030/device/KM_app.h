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

typedef struct {
    uint32_t UARTDR;
    uint32_t UARTRSR;
    uint32_t res0;
    uint32_t res1;
    uint32_t res2;
    uint32_t res3;
    uint32_t UARTFR;
    uint32_t res4;
    uint32_t UARTILPR;
    uint32_t UARTIBRD; // Integer baud divider
    uint32_t UARTFBRD; // Fractional Baud divider
    uint32_t UARTLCR_H;
    uint32_t UARTCR;
    uint32_t UARTIFLS;
    uint32_t UARTIMSC;
    uint32_t UARTRIS;
    uint32_t UARTMIS;
    uint32_t UARTICR;
    uint32_t UARTDMACR;
} uart_ctrl_t;


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#ifndef EXCLUDE_KM_PERIPHERALS
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/akira_cm3_ss_device.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/akira_cm3_ss_ppb.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ar1de_cr.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ar1de_dbg.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ar1de_mwu.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/arm_crypto310.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/aux_digif_apb_mmap.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/aux_dig_top.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m3_dwt.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m3_etm.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m3_itm.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m3_scs.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m7_cti.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m7_dwt.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m7_etm.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m7_itm.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cortex_m7_scs.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cti.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cxatbfunnel.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cxatbreplicator.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/cxtsgen.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/display_top.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/dl_bpe.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/dmac_pl081.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/etr_limiter.h"
//#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/hal_debug.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/hal_device.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/hal_memory.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/hal_memory_defs.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/hwsequencer.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/hwsequencer_dbg.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/i2c.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/i2s.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ic_rx_scratch_ram.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ic_tx_scratch_ram.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/inari_cm7_ss_device.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/inari_cm7_ss_ppb.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ipc_mbx.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/kmgo_rf_dig_ccu.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/kmgo_sysctrl.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_fbuf.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_gnss_apps_tim.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_go_config_regs.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_leti_cvp.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_ofdm_demux.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_pss.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_pwrctrl.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_sf.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_sss.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/km_ul_bpe.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/nic400_app_ic.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ospi.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/otp_ctrl.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/pio.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/pio_onfi_cal.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/pll_dig_if.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/pmu.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/psram.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/pwm_top.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/rfdma.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/rffe.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/rx_apb_mmap_rxadc.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/rx_apb_mmap_rxbb.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/rx_apb_mmap_rxrf.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/rx_dig_top.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/sch.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/sdio_host.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/security.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/smurf_if_1ch.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/smurf_if_hwacc_config.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/spi.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/stm_cr.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/stm_filter.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/stm_stim.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tcu_seqfifo.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tdr_access_unit_apb.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/timer.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tmc.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tpiu.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tscu.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/ts_mgr.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tx_apb_mmap_txbb.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tx_apb_mmap_txrf.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tx_dig_top_txdfe1.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/tx_dig_top_txdfe2.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/uart.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/usbhs.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/usbhs_cfgs.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/usim.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/vcu_cr.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/vdma_cr.h"
#include "targets/target_ublox/target_r5030/target_r5xxx/sdk/wdg.h"
/*@}*/ /* end of group KM_Peripherals */
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/** @addtogroup KM_MemoryMap KM Memory Mapping
  @{
*/

#define RTC_IRQ_TIME_LSBS (*(volatile uint32_t *)(0x40002000))
#define RTC_IRQ_TIME_LSBS_BITSET (*(volatile uint32_t *)(0x40002400))
#define RTC_IRQ_TIME_LSBS_BITCLR (*(volatile uint32_t *)(0x40002800))
#define RTC_IRQ_TIME_LSBS_BITTOG (*(volatile uint32_t *)(0x40002C00))
#define RTC_IRQ_TIME_MSBS (*(volatile uint32_t *)(0x40002004))
#define RTC_IRQ_TIME_MSBS_BITSET (*(volatile uint32_t *)(0x40002404))
#define RTC_IRQ_TIME_MSBS_BITCLR (*(volatile uint32_t *)(0x40002804))
#define RTC_IRQ_TIME_MSBS_BITTOG (*(volatile uint32_t *)(0x40002C04))
#define RTC_IRQ_CLR (*(volatile uint32_t *)(0x40002008))
#define RTC_IRQ_CLR_BITSET (*(volatile uint32_t *)(0x40002408))
#define RTC_IRQ_CLR_BITCLR (*(volatile uint32_t *)(0x40002808))
#define RTC_IRQ_CLR_BITTOG (*(volatile uint32_t *)(0x40002C08))
#define TIMER0_LOAD (*(volatile uint32_t *)(0x4000200C))
#define TIMER0_LOAD_BITSET (*(volatile uint32_t *)(0x4000240C))
#define TIMER0_LOAD_BITCLR (*(volatile uint32_t *)(0x4000280C))
#define TIMER0_LOAD_BITTOG (*(volatile uint32_t *)(0x40002C0C))
#define TIMER0_CTRL (*(volatile uint32_t *)(0x40002010))
#define TIMER0_CTRL_BITSET (*(volatile uint32_t *)(0x40002410))
#define TIMER0_CTRL_BITCLR (*(volatile uint32_t *)(0x40002810))
#define TIMER0_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C10))
#define TIMER0_TIME (*(volatile uint32_t *)(0x40002014))
#define ARM_IRQ_REG (*(volatile uint32_t *)(0x40002018))
#define ARM_IRQ_REG_BITSET (*(volatile uint32_t *)(0x40002418))
#define ARM_IRQ_REG_BITCLR (*(volatile uint32_t *)(0x40002818))
#define ARM_IRQ_REG_BITTOG (*(volatile uint32_t *)(0x40002C18))
#define PIO_FUNC0 (*(volatile uint32_t *)(0x4000201C))
#define PIO_FUNC0_BITSET (*(volatile uint32_t *)(0x4000241C))
#define PIO_FUNC0_BITCLR (*(volatile uint32_t *)(0x4000281C))
#define PIO_FUNC0_BITTOG (*(volatile uint32_t *)(0x40002C1C))
#define PIO_FUNC1 (*(volatile uint32_t *)(0x40002020))
#define PIO_FUNC1_BITSET (*(volatile uint32_t *)(0x40002420))
#define PIO_FUNC1_BITCLR (*(volatile uint32_t *)(0x40002820))
#define PIO_FUNC1_BITTOG (*(volatile uint32_t *)(0x40002C20))
#define PIO_FUNC2 (*(volatile uint32_t *)(0x40002024))
#define PIO_FUNC2_BITSET (*(volatile uint32_t *)(0x40002424))
#define PIO_FUNC2_BITCLR (*(volatile uint32_t *)(0x40002824))
#define PIO_FUNC2_BITTOG (*(volatile uint32_t *)(0x40002C24))
#define PIO_FUNC3 (*(volatile uint32_t *)(0x40002028))
#define PIO_FUNC3_BITSET (*(volatile uint32_t *)(0x40002428))
#define PIO_FUNC3_BITCLR (*(volatile uint32_t *)(0x40002828))
#define PIO_FUNC3_BITTOG (*(volatile uint32_t *)(0x40002C28))
#define PIO_FUNC4 (*(volatile uint32_t *)(0x4000202C))
#define PIO_FUNC4_BITSET (*(volatile uint32_t *)(0x4000242C))
#define PIO_FUNC4_BITCLR (*(volatile uint32_t *)(0x4000282C))
#define PIO_FUNC4_BITTOG (*(volatile uint32_t *)(0x40002C2C))
#define GPIO_DIR (*(volatile uint32_t *)(0x40002030))
#define GPIO_DIR_BITSET (*(volatile uint32_t *)(0x40002430))
#define GPIO_DIR_BITCLR (*(volatile uint32_t *)(0x40002830))
#define GPIO_DIR_BITTOG (*(volatile uint32_t *)(0x40002C30))
#define GPIO_OUT (*(volatile uint32_t *)(0x40002034))
#define GPIO_OUT_BITSET (*(volatile uint32_t *)(0x40002434))
#define GPIO_OUT_BITCLR (*(volatile uint32_t *)(0x40002834))
#define GPIO_OUT_BITTOG (*(volatile uint32_t *)(0x40002C34))
#define GPIO_DRIVE (*(volatile uint32_t *)(0x40002038))
#define GPIO_DRIVE_BITSET (*(volatile uint32_t *)(0x40002438))
#define GPIO_DRIVE_BITCLR (*(volatile uint32_t *)(0x40002838))
#define GPIO_DRIVE_BITTOG (*(volatile uint32_t *)(0x40002C38))
#define GPIO_PULLEN (*(volatile uint32_t *)(0x4000203C))
#define GPIO_PULLEN_BITSET (*(volatile uint32_t *)(0x4000243C))
#define GPIO_PULLEN_BITCLR (*(volatile uint32_t *)(0x4000283C))
#define GPIO_PULLEN_BITTOG (*(volatile uint32_t *)(0x40002C3C))
#define GPIO_INT_RISE (*(volatile uint32_t *)(0x40002040))
#define GPIO_INT_RISE_BITSET (*(volatile uint32_t *)(0x40002440))
#define GPIO_INT_RISE_BITCLR (*(volatile uint32_t *)(0x40002840))
#define GPIO_INT_RISE_BITTOG (*(volatile uint32_t *)(0x40002C40))
#define GPIO_INT_FALL (*(volatile uint32_t *)(0x40002044))
#define GPIO_INT_FALL_BITSET (*(volatile uint32_t *)(0x40002444))
#define GPIO_INT_FALL_BITCLR (*(volatile uint32_t *)(0x40002844))
#define GPIO_INT_FALL_BITTOG (*(volatile uint32_t *)(0x40002C44))
#define GPIO_INT_HIGH (*(volatile uint32_t *)(0x40002048))
#define GPIO_INT_HIGH_BITSET (*(volatile uint32_t *)(0x40002448))
#define GPIO_INT_HIGH_BITCLR (*(volatile uint32_t *)(0x40002848))
#define GPIO_INT_HIGH_BITTOG (*(volatile uint32_t *)(0x40002C48))
#define GPIO_INT_LOW (*(volatile uint32_t *)(0x4000204C))
#define GPIO_INT_LOW_BITSET (*(volatile uint32_t *)(0x4000244C))
#define GPIO_INT_LOW_BITCLR (*(volatile uint32_t *)(0x4000284C))
#define GPIO_INT_LOW_BITTOG (*(volatile uint32_t *)(0x40002C4C))
#define GPIO_INT_CLR (*(volatile uint32_t *)(0x40002050))
#define GPIO_INT_CLR_BITSET (*(volatile uint32_t *)(0x40002450))
#define GPIO_INT_CLR_BITCLR (*(volatile uint32_t *)(0x40002850))
#define GPIO_INT_CLR_BITTOG (*(volatile uint32_t *)(0x40002C50))
#define GPIO_VALUE (*(volatile uint32_t *)(0x40002054))
#define GPIO_IRQ (*(volatile uint32_t *)(0x40002058))
#define WDT_INTERVAL (*(volatile uint32_t *)(0x4000205C))
#define WDT_INTERVAL_BITSET (*(volatile uint32_t *)(0x4000245C))
#define WDT_INTERVAL_BITCLR (*(volatile uint32_t *)(0x4000285C))
#define WDT_INTERVAL_BITTOG (*(volatile uint32_t *)(0x40002C5C))
#define WDT_CTRL (*(volatile uint32_t *)(0x40002060))
#define WDT_CTRL_BITSET (*(volatile uint32_t *)(0x40002460))
#define WDT_CTRL_BITCLR (*(volatile uint32_t *)(0x40002860))
#define WDT_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C60))
#define WDT_TIME (*(volatile uint32_t *)(0x40002064))
#define RESET_CAUSE (*(volatile uint32_t *)(0x40002134))
#define PWM0_CTRL (*(volatile uint32_t *)(0x40002068))
#define PWM0_CTRL_BITSET (*(volatile uint32_t *)(0x40002468))
#define PWM0_CTRL_BITCLR (*(volatile uint32_t *)(0x40002868))
#define PWM0_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C68))
#define PWM0_COUNT (*(volatile uint32_t *)(0x4000206C))
#define PWM0_COUNT_BITSET (*(volatile uint32_t *)(0x4000246C))
#define PWM0_COUNT_BITCLR (*(volatile uint32_t *)(0x4000286C))
#define PWM0_COUNT_BITTOG (*(volatile uint32_t *)(0x40002C6C))
#define PWM1_CTRL (*(volatile uint32_t *)(0x40002070))
#define PWM1_CTRL_BITSET (*(volatile uint32_t *)(0x40002470))
#define PWM1_CTRL_BITCLR (*(volatile uint32_t *)(0x40002870))
#define PWM1_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C70))
#define PWM1_COUNT (*(volatile uint32_t *)(0x40002074))
#define PWM1_COUNT_BITSET (*(volatile uint32_t *)(0x40002474))
#define PWM1_COUNT_BITCLR (*(volatile uint32_t *)(0x40002874))
#define PWM1_COUNT_BITTOG (*(volatile uint32_t *)(0x40002C74))
#define PWM_STATUS (*(volatile uint32_t *)(0x40002078))
#define CLKEN_REG (*(volatile uint32_t *)(0x4000207C))
#define CLKEN_REG_BITSET (*(volatile uint32_t *)(0x4000247C))
#define CLKEN_REG_BITCLR (*(volatile uint32_t *)(0x4000287C))
#define CLKEN_REG_BITTOG (*(volatile uint32_t *)(0x40002C7C))
#define I2C_INTERRUPT_STATUS (*(volatile uint32_t *)(0x40002080))
#define I2C_INTERRUPT_CLEAR (*(volatile uint32_t *)(0x40002084))
#define I2C_INTERRUPT_CLEAR_BITSET (*(volatile uint32_t *)(0x40002484))
#define I2C_INTERRUPT_CLEAR_BITCLR (*(volatile uint32_t *)(0x40002884))
#define I2C_INTERRUPT_CLEAR_BITTOG (*(volatile uint32_t *)(0x40002C84))
#define I2C_INTERRUPT_ENABLE (*(volatile uint32_t *)(0x40002088))
#define I2C_INTERRUPT_ENABLE_BITSET (*(volatile uint32_t *)(0x40002488))
#define I2C_INTERRUPT_ENABLE_BITCLR (*(volatile uint32_t *)(0x40002888))
#define I2C_INTERRUPT_ENABLE_BITTOG (*(volatile uint32_t *)(0x40002C88))
#define I2C_MODE (*(volatile uint32_t *)(0x4000208C))
#define I2C_MODE_BITSET (*(volatile uint32_t *)(0x4000248C))
#define I2C_MODE_BITCLR (*(volatile uint32_t *)(0x4000288C))
#define I2C_MODE_BITTOG (*(volatile uint32_t *)(0x40002C8C))
#define I2C_TX_DATA (*(volatile uint32_t *)(0x40002090))
#define I2C_TX_DATA_BITSET (*(volatile uint32_t *)(0x40002490))
#define I2C_TX_DATA_BITCLR (*(volatile uint32_t *)(0x40002890))
#define I2C_TX_DATA_BITTOG (*(volatile uint32_t *)(0x40002C90))
#define I2C_RX_DATA (*(volatile uint32_t *)(0x40002144))
#define I2C_TX_RD_WRB (*(volatile uint32_t *)(0x40002094))
#define I2C_TX_RD_WRB_BITSET (*(volatile uint32_t *)(0x40002494))
#define I2C_TX_RD_WRB_BITCLR (*(volatile uint32_t *)(0x40002894))
#define I2C_TX_RD_WRB_BITTOG (*(volatile uint32_t *)(0x40002C94))
#define I2C_TX_NO_BYTES (*(volatile uint32_t *)(0x40002098))
#define I2C_TX_NO_BYTES_BITSET (*(volatile uint32_t *)(0x40002498))
#define I2C_TX_NO_BYTES_BITCLR (*(volatile uint32_t *)(0x40002898))
#define I2C_TX_NO_BYTES_BITTOG (*(volatile uint32_t *)(0x40002C98))
#define I2C_RX_NO_BYTES (*(volatile uint32_t *)(0x4000209C))
#define I2C_RX_NO_BYTES_MASTER (*(volatile uint32_t *)(0x400020A0))
#define I2C_RX_NO_BYTES_MASTER_BITSET (*(volatile uint32_t *)(0x400024A0))
#define I2C_RX_NO_BYTES_MASTER_BITCLR (*(volatile uint32_t *)(0x400028A0))
#define I2C_RX_NO_BYTES_MASTER_BITTOG (*(volatile uint32_t *)(0x40002CA0))
#define I2C_GO (*(volatile uint32_t *)(0x400020A4))
#define I2C_GO_BITSET (*(volatile uint32_t *)(0x400024A4))
#define I2C_GO_BITCLR (*(volatile uint32_t *)(0x400028A4))
#define I2C_GO_BITTOG (*(volatile uint32_t *)(0x40002CA4))
#define I2C_RX_EARLY_THRESHOLD (*(volatile uint32_t *)(0x400020A8))
#define I2C_RX_EARLY_THRESHOLD_BITSET (*(volatile uint32_t *)(0x400024A8))
#define I2C_RX_EARLY_THRESHOLD_BITCLR (*(volatile uint32_t *)(0x400028A8))
#define I2C_RX_EARLY_THRESHOLD_BITTOG (*(volatile uint32_t *)(0x40002CA8))
#define I2C_RX_AUTO_NAG_BYTE_CNT (*(volatile uint32_t *)(0x400020AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITSET (*(volatile uint32_t *)(0x400024AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITCLR (*(volatile uint32_t *)(0x400028AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITTOG (*(volatile uint32_t *)(0x40002CAC))
#define I2C_HALF_TIME (*(volatile uint32_t *)(0x400020B0))
#define I2C_HALF_TIME_BITSET (*(volatile uint32_t *)(0x400024B0))
#define I2C_HALF_TIME_BITCLR (*(volatile uint32_t *)(0x400028B0))
#define I2C_HALF_TIME_BITTOG (*(volatile uint32_t *)(0x40002CB0))
#define I2C_ADDRESS (*(volatile uint32_t *)(0x400020B4))
#define I2C_ADDRESS_BITSET (*(volatile uint32_t *)(0x400024B4))
#define I2C_ADDRESS_BITCLR (*(volatile uint32_t *)(0x400028B4))
#define I2C_ADDRESS_BITTOG (*(volatile uint32_t *)(0x40002CB4))
#define I2C_ADDR_TYPE (*(volatile uint32_t *)(0x400020B8))
#define I2C_ADDR_TYPE_BITSET (*(volatile uint32_t *)(0x400024B8))
#define I2C_ADDR_TYPE_BITCLR (*(volatile uint32_t *)(0x400028B8))
#define I2C_ADDR_TYPE_BITTOG (*(volatile uint32_t *)(0x40002CB8))
#define I2C_SOFT_RESET (*(volatile uint32_t *)(0x400020BC))
#define I2C_SOFT_RESET_BITSET (*(volatile uint32_t *)(0x400024BC))
#define I2C_SOFT_RESET_BITCLR (*(volatile uint32_t *)(0x400028BC))
#define I2C_SOFT_RESET_BITTOG (*(volatile uint32_t *)(0x40002CBC))
#define I2C_SLAVE_RWB (*(volatile uint32_t *)(0x400020C0))
#define I2C_MASTER_SM (*(volatile uint32_t *)(0x400020C4))
#define I2C_SLAVE_SM (*(volatile uint32_t *)(0x400020C8))
#define I2C_SLAVE_ENABLE (*(volatile uint32_t *)(0x400020CC))
#define I2C_SLAVE_ENABLE_BITSET (*(volatile uint32_t *)(0x400024CC))
#define I2C_SLAVE_ENABLE_BITCLR (*(volatile uint32_t *)(0x400028CC))
#define I2C_SLAVE_ENABLE_BITTOG (*(volatile uint32_t *)(0x40002CCC))
#define I2C_MASTER_SEND_RESTART (*(volatile uint32_t *)(0x400020D0))
#define I2C_MASTER_SEND_RESTART_BITSET (*(volatile uint32_t *)(0x400024D0))
#define I2C_MASTER_SEND_RESTART_BITCLR (*(volatile uint32_t *)(0x400028D0))
#define I2C_MASTER_SEND_RESTART_BITTOG (*(volatile uint32_t *)(0x40002CD0))
#define DMA_MUX (*(volatile uint32_t *)(0x400020D4))
#define DMA_MUX_BITSET (*(volatile uint32_t *)(0x400024D4))
#define DMA_MUX_BITCLR (*(volatile uint32_t *)(0x400028D4))
#define DMA_MUX_BITTOG (*(volatile uint32_t *)(0x40002CD4))
#define DMA_CTRL_STAT (*(volatile uint32_t *)(0x400020D8))
#define COMP_CTRL (*(volatile uint32_t *)(0x400020DC))
#define COMP_CTRL_BITSET (*(volatile uint32_t *)(0x400024DC))
#define COMP_CTRL_BITCLR (*(volatile uint32_t *)(0x400028DC))
#define COMP_CTRL_BITTOG (*(volatile uint32_t *)(0x40002CDC))
#define COMP_STAT (*(volatile uint32_t *)(0x400020E0))
#define LP_UART_CTRL (*(volatile uint32_t *)(0x400020E4))
#define LP_UART_CTRL_BITSET (*(volatile uint32_t *)(0x400024E4))
#define LP_UART_CTRL_BITCLR (*(volatile uint32_t *)(0x400028E4))
#define LP_UART_CTRL_BITTOG (*(volatile uint32_t *)(0x40002CE4))
#define LP_UART_STATUS (*(volatile uint32_t *)(0x400020E8))
#define LP_UART_DATA (*(volatile uint32_t *)(0x40002154))
#define CAP_FILT_CONF (*(volatile uint32_t *)(0x400020EC))
#define CAP_FILT_CONF_BITSET (*(volatile uint32_t *)(0x400024EC))
#define CAP_FILT_CONF_BITCLR (*(volatile uint32_t *)(0x400028EC))
#define CAP_FILT_CONF_BITTOG (*(volatile uint32_t *)(0x40002CEC))
#define CAP_IRQ_CONF (*(volatile uint32_t *)(0x400020F0))
#define CAP_IRQ_CONF_BITSET (*(volatile uint32_t *)(0x400024F0))
#define CAP_IRQ_CONF_BITCLR (*(volatile uint32_t *)(0x400028F0))
#define CAP_IRQ_CONF_BITTOG (*(volatile uint32_t *)(0x40002CF0))
#define CAP_STATUS (*(volatile uint32_t *)(0x400020F4))
#define CORE_ENABLE_SWD_ACCESS_APPS (*(volatile uint32_t *)(0x400020F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITSET (*(volatile uint32_t *)(0x400024F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITCLR (*(volatile uint32_t *)(0x400028F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITTOG (*(volatile uint32_t *)(0x40002CF8))
#define APPS_DEBUGGER_TO_CORE_DATA (*(volatile uint32_t *)(0x400020FC))
#define APPS_CORE_TO_DEBUGGER_DATA (*(volatile uint32_t *)(0x40002100))
#define APPS_CORE_TO_DEBUGGER_DATA_BITSET (*(volatile uint32_t *)(0x40002500))
#define APPS_CORE_TO_DEBUGGER_DATA_BITCLR (*(volatile uint32_t *)(0x40002900))
#define APPS_CORE_TO_DEBUGGER_DATA_BITTOG (*(volatile uint32_t *)(0x40002D00))
#define APPS_DEBUG_DATA_TO_CORE_AVAILABLE (*(volatile uint32_t *)(0x40002104))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED (*(volatile uint32_t *)(0x40002108))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITSET (*(volatile uint32_t *)(0x40002508))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITCLR (*(volatile uint32_t *)(0x40002908))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITTOG (*(volatile uint32_t *)(0x40002D08))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE (*(volatile uint32_t *)(0x4000210C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITSET (*(volatile uint32_t *)(0x4000250C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITCLR (*(volatile uint32_t *)(0x4000290C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITTOG (*(volatile uint32_t *)(0x40002D0C))
#define APPS_CORE_DATA_TO_DEBUGGER_ACCEPTED (*(volatile uint32_t *)(0x40002110))
#define SWD_REQUEST (*(volatile uint32_t *)(0x40002114))
#define EDGE_CTRL0 (*(volatile uint32_t *)(0x40002118))
#define EDGE_CTRL0_BITSET (*(volatile uint32_t *)(0x40002518))
#define EDGE_CTRL0_BITCLR (*(volatile uint32_t *)(0x40002918))
#define EDGE_CTRL0_BITTOG (*(volatile uint32_t *)(0x40002D18))
#define EDGE_CTRL1 (*(volatile uint32_t *)(0x4000211C))
#define EDGE_CTRL1_BITSET (*(volatile uint32_t *)(0x4000251C))
#define EDGE_CTRL1_BITCLR (*(volatile uint32_t *)(0x4000291C))
#define EDGE_CTRL1_BITTOG (*(volatile uint32_t *)(0x40002D1C))
#define EDGE_COUNT (*(volatile uint32_t *)(0x40002120))
#define RESET_REG (*(volatile uint32_t *)(0x40002124))
#define RESET_REG_BITSET (*(volatile uint32_t *)(0x40002524))
#define RESET_REG_BITCLR (*(volatile uint32_t *)(0x40002924))
#define RESET_REG_BITTOG (*(volatile uint32_t *)(0x40002D24))
#define DIGITAL_VERSION (*(volatile uint32_t *)(0x40000000))
#define CLK_FREQ_DAC (*(volatile uint32_t *)(0x40000004))
#define CLK_FREQ_SET (*(volatile uint32_t *)(0x40000008))
#define CLK_FREQ_SET_BITSET (*(volatile uint32_t *)(0x40000408))
#define CLK_FREQ_SET_BITCLR (*(volatile uint32_t *)(0x40000808))
#define CLK_FREQ_SET_BITTOG (*(volatile uint32_t *)(0x40000C08))
#define CLK_FREQ_NREFCLKS (*(volatile uint32_t *)(0x4000000C))
#define CLK_FREQ_NREFCLKS_BITSET (*(volatile uint32_t *)(0x4000040C))
#define CLK_FREQ_NREFCLKS_BITCLR (*(volatile uint32_t *)(0x4000080C))
#define CLK_FREQ_NREFCLKS_BITTOG (*(volatile uint32_t *)(0x40000C0C))
#define CLK_FREQ_REF_SEL (*(volatile uint32_t *)(0x40000010))
#define CLK_FREQ_REF_SEL_BITSET (*(volatile uint32_t *)(0x40000410))
#define CLK_FREQ_REF_SEL_BITCLR (*(volatile uint32_t *)(0x40000810))
#define CLK_FREQ_REF_SEL_BITTOG (*(volatile uint32_t *)(0x40000C10))
#define CLK_FREQ_DIG_CLKS (*(volatile uint32_t *)(0x40000014))
#define CLK_FREQ_HIGHTARGET (*(volatile uint32_t *)(0x40000018))
#define CLK_FREQ_HIGHTARGET_BITSET (*(volatile uint32_t *)(0x40000418))
#define CLK_FREQ_HIGHTARGET_BITCLR (*(volatile uint32_t *)(0x40000818))
#define CLK_FREQ_HIGHTARGET_BITTOG (*(volatile uint32_t *)(0x40000C18))
#define CLK_FREQ_LOWTARGET (*(volatile uint32_t *)(0x4000001C))
#define CLK_FREQ_LOWTARGET_BITSET (*(volatile uint32_t *)(0x4000041C))
#define CLK_FREQ_LOWTARGET_BITCLR (*(volatile uint32_t *)(0x4000081C))
#define CLK_FREQ_LOWTARGET_BITTOG (*(volatile uint32_t *)(0x40000C1C))
#define CLK_FREQ_LP_BACKOFF (*(volatile uint32_t *)(0x40000020))
#define CLK_FREQ_LP_BACKOFF_BITSET (*(volatile uint32_t *)(0x40000420))
#define CLK_FREQ_LP_BACKOFF_BITCLR (*(volatile uint32_t *)(0x40000820))
#define CLK_FREQ_LP_BACKOFF_BITTOG (*(volatile uint32_t *)(0x40000C20))
#define CLK_FREQ_ENABLE (*(volatile uint32_t *)(0x40000024))
#define CLK_FREQ_ENABLE_BITSET (*(volatile uint32_t *)(0x40000424))
#define CLK_FREQ_ENABLE_BITCLR (*(volatile uint32_t *)(0x40000824))
#define CLK_FREQ_ENABLE_BITTOG (*(volatile uint32_t *)(0x40000C24))
#define CLK_GATE_SYS (*(volatile uint32_t *)(0x40000028))
#define CLK_GATE_SYS_BITSET (*(volatile uint32_t *)(0x40000428))
#define CLK_GATE_SYS_BITCLR (*(volatile uint32_t *)(0x40000828))
#define CLK_GATE_SYS_BITTOG (*(volatile uint32_t *)(0x40000C28))
#define CLK_GATE_MODEM (*(volatile uint32_t *)(0x4000002C))
#define CLK_GATE_MODEM_BITSET (*(volatile uint32_t *)(0x4000042C))
#define CLK_GATE_MODEM_BITCLR (*(volatile uint32_t *)(0x4000082C))
#define CLK_GATE_MODEM_BITTOG (*(volatile uint32_t *)(0x40000C2C))
#define CLK_GATE_RADIO (*(volatile uint32_t *)(0x40000030))
#define CLK_GATE_RADIO_BITSET (*(volatile uint32_t *)(0x40000430))
#define CLK_GATE_RADIO_BITCLR (*(volatile uint32_t *)(0x40000830))
#define CLK_GATE_RADIO_BITTOG (*(volatile uint32_t *)(0x40000C30))
#define CLK_GATE_DEBUG (*(volatile uint32_t *)(0x40000034))
#define CLK_GATE_DEBUG_BITSET (*(volatile uint32_t *)(0x40000434))
#define CLK_GATE_DEBUG_BITCLR (*(volatile uint32_t *)(0x40000834))
#define CLK_GATE_DEBUG_BITTOG (*(volatile uint32_t *)(0x40000C34))
#define CLK_GATE_RBIST (*(volatile uint32_t *)(0x40000038))
#define CLK_GATE_RBIST_BITSET (*(volatile uint32_t *)(0x40000438))
#define CLK_GATE_RBIST_BITCLR (*(volatile uint32_t *)(0x40000838))
#define CLK_GATE_RBIST_BITTOG (*(volatile uint32_t *)(0x40000C38))
#define LPC_CTRL (*(volatile uint32_t *)(0x4000003C))
#define LPC_CTRL_BITSET (*(volatile uint32_t *)(0x4000043C))
#define LPC_CTRL_BITCLR (*(volatile uint32_t *)(0x4000083C))
#define LPC_CTRL_BITTOG (*(volatile uint32_t *)(0x40000C3C))
#define LPC_TEST (*(volatile uint32_t *)(0x40000040))
#define LPC_TEST_BITSET (*(volatile uint32_t *)(0x40000440))
#define LPC_TEST_BITCLR (*(volatile uint32_t *)(0x40000840))
#define LPC_TEST_BITTOG (*(volatile uint32_t *)(0x40000C40))
#define FPGA_FLASH_WR (*(volatile uint32_t *)(0x40000044))
#define FPGA_FLASH_WR_BITSET (*(volatile uint32_t *)(0x40000444))
#define FPGA_FLASH_WR_BITCLR (*(volatile uint32_t *)(0x40000844))
#define FPGA_FLASH_WR_BITTOG (*(volatile uint32_t *)(0x40000C44))
#define FPGA_FLASH_RD (*(volatile uint32_t *)(0x40000048))
#define PMU_CTRL (*(volatile uint32_t *)(0x4000004C))
#define PMU_CTRL_BITSET (*(volatile uint32_t *)(0x4000044C))
#define PMU_CTRL_BITCLR (*(volatile uint32_t *)(0x4000084C))
#define PMU_CTRL_BITTOG (*(volatile uint32_t *)(0x40000C4C))
#define APP_CTRL0 (*(volatile uint32_t *)(0x40000050))
#define APP_CTRL0_BITSET (*(volatile uint32_t *)(0x40000450))
#define APP_CTRL0_BITCLR (*(volatile uint32_t *)(0x40000850))
#define APP_CTRL0_BITTOG (*(volatile uint32_t *)(0x40000C50))
#define APP_CTRL1 (*(volatile uint32_t *)(0x40000054))
#define APP_CTRL1_BITSET (*(volatile uint32_t *)(0x40000454))
#define APP_CTRL1_BITCLR (*(volatile uint32_t *)(0x40000854))
#define APP_CTRL1_BITTOG (*(volatile uint32_t *)(0x40000C54))
#define APP_CTRL2 (*(volatile uint32_t *)(0x40000058))
#define APP_CTRL2_BITSET (*(volatile uint32_t *)(0x40000458))
#define APP_CTRL2_BITCLR (*(volatile uint32_t *)(0x40000858))
#define APP_CTRL2_BITTOG (*(volatile uint32_t *)(0x40000C58))
#define APP_CTRL3 (*(volatile uint32_t *)(0x4000005C))
#define APP_CTRL3_BITSET (*(volatile uint32_t *)(0x4000045C))
#define APP_CTRL3_BITCLR (*(volatile uint32_t *)(0x4000085C))
#define APP_CTRL3_BITTOG (*(volatile uint32_t *)(0x40000C5C))
#define PMU_STAT (*(volatile uint32_t *)(0x40000060))
#define PMUBIST_ADC_CONF (*(volatile uint32_t *)(0x40000064))
#define PMUBIST_ADC_CONF_BITSET (*(volatile uint32_t *)(0x40000464))
#define PMUBIST_ADC_CONF_BITCLR (*(volatile uint32_t *)(0x40000864))
#define PMUBIST_ADC_CONF_BITTOG (*(volatile uint32_t *)(0x40000C64))
#define PMUBIST_ADC_DATA (*(volatile uint32_t *)(0x40000068))
#define STATUS (*(volatile uint32_t *)(0x4000006C))
#define LPC_STATUS (*(volatile uint32_t *)(0x40000070))
#define LPC_PDTIMER (*(volatile uint32_t *)(0x40000074))
#define LPC_PDTIMER_BITSET (*(volatile uint32_t *)(0x40000474))
#define LPC_PDTIMER_BITCLR (*(volatile uint32_t *)(0x40000874))
#define LPC_PDTIMER_BITTOG (*(volatile uint32_t *)(0x40000C74))
#define PIO_OWNER0 (*(volatile uint32_t *)(0x40000078))
#define PIO_OWNER1 (*(volatile uint32_t *)(0x4000007C))
#define RTC_TIME_LSBS (*(volatile uint32_t *)(0x40000080))
#define RTC_TIME_MSBS (*(volatile uint32_t *)(0x40000084))
#define DEBUG_SEL (*(volatile uint32_t *)(0x40000088))
#define DEBUG_SEL_BITSET (*(volatile uint32_t *)(0x40000488))
#define DEBUG_SEL_BITCLR (*(volatile uint32_t *)(0x40000888))
#define DEBUG_SEL_BITTOG (*(volatile uint32_t *)(0x40000C88))
#define FLASH_STATUS (*(volatile uint32_t *)(0x4000008C))
#define CHIP_WDT_INTERVAL (*(volatile uint32_t *)(0x40000090))
#define CHIP_WDT_INTERVAL_BITSET (*(volatile uint32_t *)(0x40000490))
#define CHIP_WDT_INTERVAL_BITCLR (*(volatile uint32_t *)(0x40000890))
#define CHIP_WDT_INTERVAL_BITTOG (*(volatile uint32_t *)(0x40000C90))
#define CHIP_WDT_CTRL (*(volatile uint32_t *)(0x40000094))
#define CHIP_WDT_CTRL_BITSET (*(volatile uint32_t *)(0x40000494))
#define CHIP_WDT_CTRL_BITCLR (*(volatile uint32_t *)(0x40000894))
#define CHIP_WDT_CTRL_BITTOG (*(volatile uint32_t *)(0x40000C94))
#define CHIP_WDT_TIME (*(volatile uint32_t *)(0x40000098))
#define CHIP_RESET (*(volatile uint32_t *)(0x4000009C))
#define CHIP_RESET_BITSET (*(volatile uint32_t *)(0x4000049C))
#define CHIP_RESET_BITCLR (*(volatile uint32_t *)(0x4000089C))
#define CHIP_RESET_BITTOG (*(volatile uint32_t *)(0x40000C9C))
#define SWD_PIN_CFG (*(volatile uint32_t *)(0x400000A0))
#define SWD_PIN_CFG_BITSET (*(volatile uint32_t *)(0x400004A0))
#define SWD_PIN_CFG_BITCLR (*(volatile uint32_t *)(0x400008A0))
#define SWD_PIN_CFG_BITTOG (*(volatile uint32_t *)(0x40000CA0))


/*@}*/ /* end of group KM_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/** @addtogroup KM_PeripheralDecl KM Peripheral Declaration
  @{
*/
/* UART Defines */
#define UART0_BASE 0x50003000
#define UART1_BASE 0x50004000
/*@}*/ /* end of group KM_PeripheralDecl */

/*@}*/ /* end of group KM_Definitions */

#ifdef __cplusplus
}
#endif

#endif  /* KM_APP_H */
