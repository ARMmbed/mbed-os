/**************************************************************************//**
 * @file     KM_app.h
 * @brief    CMSIS Cortex-M7 Core Peripheral Access Layer Header File for
 *           Device KM
 * @version  V5.00
 * @date     02. March 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef KM_APP_H
#define KM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup u-blox
  * @{
  */


/** @addtogroup KM_app
  * @{
  */


/** @addtogroup Configuration_of_CMSIS
  * @{
  */



/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
/* =======================================  ARM Cortex-M7 Specific Interrupt Numbers  ======================================== */

  Reset_IRQn                = -15,              /*!< -15  Reset Vector, invoked on Power up and warm reset                     */
  NonMaskableInt_IRQn       = -14,              /*!< -14  Non maskable Interrupt, cannot be stopped or preempted               */
  HardFault_IRQn            = -13,              /*!< -13  Hard Fault, all classes of Fault                                     */
  MemoryManagement_IRQn     = -12,              /*!< -12  Memory Management, MPU mismatch, including Access Violation
                                                          and No Match                                                         */
  BusFault_IRQn             = -11,              /*!< -11  Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory
                                                          related Fault                                                        */
  UsageFault_IRQn           = -10,              /*!< -10  Usage Fault, i.e. Undef Instruction, Illegal State Transition        */
  SVCall_IRQn               =  -5,              /*!< -5 System Service Call via SVC instruction                                */
  DebugMonitor_IRQn         =  -4,              /*!< -4 Debug Monitor                                                          */
  PendSV_IRQn               =  -2,              /*!< -2 Pendable request for system service                                    */
  SysTick_IRQn              =  -1,              /*!< -1 System Tick Timer                                                      */

/* ===========================================  KM Specific Interrupt Numbers  ========================================= */
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
APP_CPU_APP_IRQ_AC_PWR_ALERT_IRQn = 45,
APP_CPU_NUM_IRQS = 46
} IRQn_Type;



/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ===========================  Configuration of the ARM Cortex-M4 Processor and Core Peripherals  =========================== */
#define __CM7_REV                 0x0101    /*!< Core Revision r2p1 */
#define __MPU_PRESENT             1         /*!< Set to 1 if MPU is present */
#define __VTOR_PRESENT            1         /*!< Set to 1 if VTOR is present */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    1         /*!< Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             0         /*!< Set to 1 if FPU is present */
#define __FPU_DP                  0         /*!< Set to 1 if FPU is double precision FPU (default is single precision FPU) */
#define __ICACHE_PRESENT          1         /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          1         /*!< Set to 1 if D-Cache is present */
#define __DTCM_PRESENT            1         /*!< Set to 1 if DTCM is present */

/** @} */ /* End of group Configuration_of_CMSIS */


#if defined(_lint) || (!defined(WIN32) && !defined(LINUX) && !defined(__linux__))
#include <core_cm7.h>                           /*!< ARM Cortex-M7 processor and core peripherals */
#endif
#include "system_KM_app.h"                    /*!< KM_app System */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#elif defined (_MSC_VER)
  /* anonymous unions are enabled by default */
#elif defined (__clang__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripherals
  * @{
  */


/*@}*/ /* end of group Device_Peripheral_peripherals */


/* =========================================  End of section using anonymous unions  ========================================= */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#elif defined (_MSC_VER)
  /* anonymous unions are enabled by default */
#elif defined (__clang__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/* ToDo: add here your device peripherals base addresses
         following is an example for timer */
/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */

/* Peripheral and SRAM base address */

/* Peripheral memory map */

/** @} */ /* End of group Device_Peripheral_peripheralAddr */


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_declaration
  * @{
  */

#ifndef EXCLUDE_KM_PERIPHERALS
#include "akira_cm3_ss_device.h"
#include "akira_cm3_ss_ppb.h"
#include "ar1de_cr.h"
#include "ar1de_dbg.h"
#include "ar1de_mwu.h"
#include "arm_crypto310.h"
#include "aux_dig_top.h"
#include "aux_digif_apb_mmap.h"
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
//#include "hal_debug.h"
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
#include "kmgo_rf_dig_ccu.h"
#include "kmgo_sysctrl.h"
#include "nic400_app_ic.h"
#include "nic400_modem_ic.h"
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
#include "tdr_access_unit.h"
#include "timer.h"
#include "tmc.h"
#include "tpiu.h"
#include "ts_mgr.h"
#include "tscu.h"
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


/******************************************************************************/
/*                        Peripheral Base Addresses                           */
/******************************************************************************/

#define PERIPH_BASE            						(0x40000000U) 						/*!< Peripherals Base Address  */ 


/*!< Peripherals Base Addresses*/

#define SYS_CONTROL_BASE    (PERIPH_BASE + 0x09000000U)			/* sys control interface base address */ 
#define POWER_CONTROL_BASE    (PERIPH_BASE + 0x09001000U)         /* power controller interface base address */ 
#define SECURITY_BASE    (PERIPH_BASE + 0x09002000U)         /* security base address */ 
#define DVS_CONTROL_BASE    (PERIPH_BASE + 0x09003000U)         /* dvs control interface base address */ 
#define USB_CONTROL_AND_SETUP_BASE    (PERIPH_BASE + 0x09004000U)         /* usb control and setup base address */ 
#define SF_AO_CONTROL_BASE    (PERIPH_BASE + 0x09005000U)         /* sf ao control registers base address */ 
#define OSPI_CONTROL_BASE    (PERIPH_BASE + 0x09006000U)         /* ospi control interface base address              	*/ 
#define WATCHDOG1_BASE    (PERIPH_BASE + 0x09007000U)         /* watchdog1 base address                           	*/ 
#define TIMER_BASE    (PERIPH_BASE + 0x09008000U)         /* timer base address                               	*/ 
#define I2C3_CONTROL_BASE    (PERIPH_BASE + 0x09009000U)			/* i2c3 control interface base address         			*/ 

#define PIO_CONTROL_BASE    (PERIPH_BASE + 0x0900A000U)         /* pio control interface base address               	*/ 
#define OTP_CONTROL_BASE    (PERIPH_BASE + 0x0900B000U)         /* otp control interface base address               	*/ 
#define GNSS_APPS_TIMING_BASE    (PERIPH_BASE + 0x0900C000U)         /* gnss apps timing base address                    	*/ 
#define TDR_ACCESS_GATEWAY_BASE    (PERIPH_BASE + 0x0900D000U)         /* tdr access gateway base address                  	*/ 
		
#define UART1_CONTROL_BASE    (PERIPH_BASE + 0x09100000U)         /* uart1 control interface base address					*/ 
#define UART2_CONTROL_BASE    (PERIPH_BASE + 0x09101000U)         /* uart2 control interface base address					*/ 
#define SPI1_CONTROL_BASE    (PERIPH_BASE + 0x09102000U)         /* spi1 control interface base address              	*/ 
#define SPI2_CONTROL_BASE    (PERIPH_BASE + 0x09103000U)         /* spi2 control interface base address              	*/ 
//#define APB_SPARE_BASE    (PERIPH_BASE + 0x09104000U)         /* apb spare base address                           	*/ 
#define I2C1_CONTROL_BASE    (PERIPH_BASE + 0x09105000U)         /* i2c1 control interface base address              	*/ 
#define I2C2_CONTROL_BASE    (PERIPH_BASE + 0x09106000U)         /* i2c2 control interface base address              	*/ 
#define I2S_BASE    (PERIPH_BASE + 0x09107000U)         /* i2s interface base address                       	*/ 
#define DISPLAY_BASE    (PERIPH_BASE + 0x09108000U)         /* display interface base address                   	*/ 
#define PWM_BASE    (PERIPH_BASE + 0x09109000U)         /* pwm interface base address                       	*/ 
		
#define SCU_CONTROL_BASE    (PERIPH_BASE + 0x09200000U)         /* scu control registers base address					*/ 
#define WATCHDOG2_BASE    (PERIPH_BASE + 0x09201000U)         /* watchdog2 base address								*/ 
#define WATCHDOG3_BASE    (PERIPH_BASE + 0x09202000U)         /* watchdog3 base address                           	*/ 
		
#define SDMA_PL081_CONTROL_BASE    (PERIPH_BASE + 0x09400000U)         /* sdma pl081 control base address						*/ 
#define USB_CONTROL_BASE    (PERIPH_BASE + 0x09401000U)         /* usb control interface base address					*/ 
		
#define PSRAM_CONTROL_BASE    (PERIPH_BASE + 0x09600000U)         /* psram control interface base address					*/ 
#define ARM_CRYPTOCELL_310_CONTROL_BASE    (PERIPH_BASE + 0x09601000U)         /* arm cryptocell 310 control interface base address	*/ 
#define SDIO_CONTROL_BASE    (PERIPH_BASE + 0x09601000U)         /* sdio control interface base address					*/ 
		
//#define CSYS_DEBUG_BASE    (PERIPH_BASE + 0x09800000U)         /* csys debug base address								*/ 
#define MAIN_INTERCONNECT_GPV_BASE    (PERIPH_BASE + 0x09900000U)         /* main interconnect gpv base address					*/ 
//#define MODEM_INTERCONNECT_GPV_BASE    (PERIPH_BASE + 0x09A00000U)         /* modem interconnect gpv base address					*/ 
		
#define STM_EXT_STIMULUS_PORT_BASE    (0x50000000U)                       /* stm extended stimulus ports base address             */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/** @addtogroup KM_PeripheralDecl KM Peripheral Declaration
  @{
*/
  
#define SYS    ((kmgo_sysctrl_s *) SYS_CONTROL_BASE)		  		
#define POWER    ((km_pwrctrl_s *) POWER_CONTROL_BASE)      	   		
#define SECURITY    ((security_s *) SECURITY_BASE)  	      		 	
#define DVS    ((km_leti_cvp_s *) DVS_CONTROL_BASE)   		          		
#define USB_SETUP    ((usbhs_cfgs_s *) USB_CONTROL_AND_SETUP_BASE)         		
#define SF_AO    ((km_sf_s *) SF_AO_CONTROL_BASE)           	 			
#define OSPI    ((ospi_s *) OSPI_CONTROL_BASE)          	 				
#define WATCHDOG1    ((wdg_s *) WATCHDOG1_BASE)        	    				
#define TIMER    ((timer_s *) TIMER_BASE)          	 			
#define I2C3    ((i2c_s *) I2C3_CONTROL_BASE)			  	 			
#define PIO    ((pio_s *) PIO_CONTROL_BASE)           		 				
#define OTP    ((otp_ctrl_s *) OTP_CONTROL_BASE)         	  			
#define GNSS_APPS_TIMING    ((km_gnss_apps_tim_s *) GNSS_APPS_TIMING_BASE)         	   
#define TDR_ACCESS_GATEWAY    ((tdr_access_unit_s *) TDR_ACCESS_GATEWAY_BASE)   	 
  
#define UART1    ((uart_s *) UART1_CONTROL_BASE)           
#define UART2    ((uart_s *) UART2_CONTROL_BASE)         
#define SPI1    ((spi_s *) SPI1_CONTROL_BASE)          		
#define SPI2    ((spi_s *) SPI2_CONTROL_BASE)          		
//#define APB_SPARE    (( *) APB_SPARE_BASE)               
#define I2C1    ((i2c_s *) I2C1_CONTROL_BASE)          		
#define I2C2    ((i2c_s *) I2C2_CONTROL_BASE)          		
#define I2S    ((i2s_s *) I2S_BASE)        
#define DISPLAY    ((display_top_s *) DISPLAY_BASE)    
#define PWM    ((pwm_top_s *) PWM_BASE)          	

#define SCU    ((tscu_s *) SCU_CONTROL_BASE)         		
#define WATCHDOG2    ((wdg_s *) WATCHDOG2_BASE)          
#define WATCHDOG3    ((wdg_s *) WATCHDOG3_BASE)          

#define SDMA    ((dmac_pl081_s *) SDMA_PL081_CONTROL_BASE)        
#define USB    ((usbhs_s *) USB_CONTROL_BASE)         		
	
#define PSRAM    ((psram_s *) PSRAM_CONTROL_BASE)           	
#define ARM_CRYPTOCELL_310_CONTROL    ((arm_crypto310_s *) ARM_CRYPTOCELL_310_CONTROL_BASE) 	 
#define SDIO    ((sdio_host_s *) SDIO_CONTROL_BASE)       	

//#define CSYS_DEBUG    (( *) CSYS_DEBUG_BASE)          	
#define MAIN_GPV    ((nic400_app_ic_s *) MAIN_INTERCONNECT_GPV_BASE) 
//#define MODEM_GPV    (( *) MODEM_GPV_BASE)               

#define STM    ((stm_stim_s *) STM_EXT_STIMULUS_PORT_BASE)           

/*@}*/ /* end of group KM_PeripheralDecl */



/******************************************************************************/
/*                        System Control Intreface                            */
/******************************************************************************/

#define SYS_CPU_BOOT_VINIT_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x000U))
#define SYS_CPU_MDM_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x004U))
#define SYS_CPU_RF_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x008U))
#define SYS_CPU_APP_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x00CU))
#define SYS_CPU_PHY_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x010U))
#define SYS_RST_REASON_CPU_MDM_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x014U))
#define SYS_RST_REASON_CPU_RF_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x018U))
#define SYS_RST_REASON_CPU_APP_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x01CU))
#define SYS_RST_REASON_CPU_PHY_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x020U))
#define SYS_RST_SW_SET_1S_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x024U))
#define SYS_RST_SW_SET_1C_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x028U))
#define SYS_RST_SW_SET_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x02CU))
#define SYS_RST_SW_SET_2S_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x030U))
#define SYS_RST_SW_SET_2C_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x034U))
#define SYS_RST_SW_SET_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x038U))
#define SYS_RST_SW_STROBE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x03CU))
#define SYS_RST_SW_STROBE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x040U))
#define SYS_RST_SW_STROBE_COUNT_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x044U))
#define SYS_RST_WDG_MDM_DISABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x048U))
#define SYS_RST_WDG_MDM_DISABLE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x04CU))
#define SYS_RST_WDG_RF_DISABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x050U))
#define SYS_RST_WDG_RF_DISABLE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x054U))
#define SYS_RST_WDG_APP_DISABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x058U))
#define SYS_RST_WDG_APP_DISABLE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x05CU))
#define SYS_RST_WDG_PHY_DISABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x060U))
#define SYS_RST_WDG_PHY_DISABLE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x064U))
#define SYS_RST_CTRL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x068U))
#define SYS_CTRL_GENERAL_PURPOSE_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x06CU))
#define SYS_RF_CCU_CTRL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x070U))	// CLOCK_CONTROL_UNIT (CCU)
#define SYS_PERIPHERAL_ID4_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x074U))
#define SYS_PERIPHERAL_ID5_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x078U))
#define SYS_PERIPHERAL_ID6_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x07CU))
#define SYS_PERIPHERAL_ID7_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x080U))
#define SYS_PERIPHERAL_ID0_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x084U))
#define SYS_PERIPHERAL_ID1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x088U))
#define SYS_PERIPHERAL_ID2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x08CU))
#define SYS_PERIPHERAL_ID3_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x090U))
#define SYS_COMPONENT_ID0_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x094U))
#define SYS_COMPONENT_ID1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x098U))
#define SYS_COMPONENT_ID2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x09CU))
#define SYS_COMPONENT_ID3_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0A0U))
#define SYS_FUNC_MBIST_CTRL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0A4U))
#define SYS_FUNC_MBIST_STAT_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0A8U))
#define SYS_FUNC_MBIST_ENABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0ACU))
#define SYS_FUNC_MBIST_ENABLE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0B0U))
#define SYS_FUNC_MBIST_FAIL_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0B4U))
#define SYS_FUNC_MBIST_FAIL_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0B8U))
#define SYS_FUNC_MBIST_DONE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0BCU))
#define SYS_FUNC_MBIST_DONE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0C0U))
#define SYS_RST_FUNC_MBIST_DISABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0C4U))
#define SYS_RST_FUNC_MBIST_DISABLE_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0C8U))
#define SYS_RST_CTRL_STATUS_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x0CCU))
#define SYS_PLL1_FREQ_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x400U))
#define SYS_PLL1_PDIV1_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x404U))
#define SYS_PLL1_PDIV2_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x408U))
#define SYS_PLL1_CTRL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x40CU))
#define SYS_PLL1_STATUS_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x410U))
#define SYS_PLL2_FREQ_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x414U))
#define SYS_PLL2_PDIV1_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x418U))
#define SYS_PLL2_PDIV2_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x41CU))
#define SYS_PLL2_CTRL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x420U))
#define SYS_PLL2_STATUS_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x424U))
#define SYS_PLL_LOCKCNT_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x450U))
#define SYS_ROOT_CLK_VSP_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x460U))
#define SYS_ROOT_CLK_CPU_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x464U))
#define SYS_ROOT_CLK_EXTIF_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x468U))
#define SYS_ROOT_CLK_EXTIF_FAST_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x46CU))
#define SYS_ROOT_CLK_ETH_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x470U))
#define SYS_ROOT_CLK_USB_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x474U))
#define SYS_CLK_MAIN_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x480U))
#define SYS_CLK_PERIPH_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x484U))
#define SYS_CLK_CPU_MDM_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x488U))
#define SYS_CLK_CPU_RF_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x48CU))
#define SYS_CLK_CPU_APP_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x490U))
#define SYS_CLK_CPU_PHY_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x494U))
#define SYS_CLK_TPIU_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x498U))
#define SYS_CLK_DBG_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x49CU))
#define SYS_CLK_VSP_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4A0U))
#define SYS_CLK_UART_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4A4U))
#define SYS_CLK_AUDIO_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4A8U))
#define SYS_CLK_OSPI_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4ACU))
#define SYS_CLK_PSRAM_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4B0U))
#define SYS_CLK_SPI_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4B4U))
#define SYS_CLK_I2C_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4B8U))
#define SYS_CLK_SDIO_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4BCU))
#define SYS_CLK_RFTSF_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4C0U))
#define SYS_CLK_CPU_TSCU_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4C4U))
#define SYS_CLK_TMR_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4D0U))
#define SYS_CLK_TMR_CPU_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4D4U))
#define SYS_CLK_PLL1_TMFLTRNG_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4E0U))
#define SYS_CLK_PLL2_TMFLTRNG_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4E4U))
#define SYS_CLK_AUDIO_NCO_CONFIG_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x4FCU))
#define SYS_CLK_ENABLE_1_SET_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x500U))
#define SYS_CLK_ENABLE_1_CLR_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x504U))
#define SYS_CLK_ENABLE_1_VAL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x508U))
#define SYS_CLK_ENABLE_1_READ_BACK_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x50CU))
#define SYS_SLEEP_CLK_ENABLE_1_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x510U))
#define SYS_CLK_ENABLE_2_SET_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x520U))
#define SYS_CLK_ENABLE_2_CLR_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x524U))
#define SYS_CLK_ENABLE_2_VAL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x528U))
#define SYS_CLK_ENABLE_2_RDBK_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x52CU))
#define SYS_SLEEP_CLKEN_2_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x530U))
#define SYS_CLK_CTRL_MODE_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x540U))
#define SYS_CLK_CTRL_STATUS_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x544U))
#define SYS_CLK_ENABLE_LOCAL_CTRL_REGISTER    (*(volatile uint32_t *)(SYS_CONTROL_BASE + 0x548U))


/******************************************************************************/
/*                        Power Controller Interface                          */
/******************************************************************************/

#define PWR_TICKS_PD_START_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x000U))
#define PWR_TICKS_PD_SMALL_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x004U))
#define PWR_TICKS_PD_BIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x008U))
#define PWR_TICKS_PD_ISOLATE_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x00CU))
#define PWR_TICKS_PD_RESET_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x010U))
#define PWR_HANDSHAKE_STATUS_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x024U))
#define PWR_HANDSHAKE_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x028U))
#define PWR_CURRENT_MODE_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x030U))
#define PWR_NEXT_MODE_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x034U))
#define PWR_UPDATE_MODE_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x038U))
#define PWR_GPC_0_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x040U))
#define PWR_GPC_1_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x044U))
#define PWR_GPC_2_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x048U))
#define PWR_GPC_3_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x04CU))
#define PWR_GPC_4_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x050U))
#define PWR_GPC_5_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x054U))
#define PWR_GPC_6_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x058U))
#define PWR_GPC_7_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x05CU))
#define PWR_WAKEUP_SOURCE_0_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x080U))
#define PWR_WAKEUP_SOURCE_1_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x084U))
#define PWR_WAKEUP_SOURCE_2_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x088U))
#define PWR_WAKEUP_SOURCE_3_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x08CU))
#define PWR_WAKEUP_SOURCE_4_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x090U))
#define PWR_WAKEUP_SOURCE_5_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x094U))
#define PWR_WAKEUP_SOURCE_6_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x098U))
#define PWR_WAKEUP_SOURCE_7_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x09CU))
#define PWR_WAKEUP_SOURCE_8_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0A0U))
#define PWR_WAKEUP_SOURCE_9_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0A4U))
#define PWR_WAKEUP_SOURCE_10_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0A8U))
#define PWR_WAKEUP_SOURCE_11_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0ACU))
#define PWR_WAKEUP_SOURCE_12_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0B0U))
#define PWR_WAKEUP_SOURCE_13_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0B4U))
#define PWR_WAKEUP_SOURCE_14_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0B8U))
#define PWR_WAKEUP_SOURCE_15_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0BCU))
#define PWR_WAKEUP_SOURCE_16_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0C0U))
#define PWR_WAKEUP_SOURCE_17_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0C4U))
#define PWR_WAKEUP_SOURCE_18_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0C8U))
#define PWR_WAKEUP_SOURCE_19_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0CCU))
#define PWR_WAKEUP_SOURCE_20_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0D0U))
#define PWR_WAKEUP_SOURCE_21_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0D4U))
#define PWR_WAKEUP_SOURCE_22_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0D8U))
#define PWR_WAKEUP_SOURCE_23_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0DCU))
#define PWR_WAKEUP_SOURCE_24_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0E0U))
#define PWR_WAKEUP_SOURCE_25_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0E4U))
#define PWR_WAKEUP_SOURCE_26_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0E8U))
#define PWR_WAKEUP_SOURCE_27_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0ECU))
#define PWR_WAKEUP_SOURCE_28_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0F0U))
#define PWR_WAKEUP_SOURCE_29_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0F4U))
#define PWR_WAKEUP_SOURCE_30_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0F8U))
#define PWR_WAKEUP_SOURCE_31_CFG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x0FCU))
#define PWR_MODE_P0_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x100U))
#define PWR_MODE_P1_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x104U))
#define PWR_MODE_P2_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x108U))
#define PWR_MODE_P3_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x10CU))
#define PWR_MODE_P4_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x110U))
#define PWR_MODE_P5_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x114U))
#define PWR_MODE_P6_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x118U))
#define PWR_MODE_P7_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x11CU))
#define PWR_MEM_00_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x200U))
#define PWR_MEM_01_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x204U))
#define PWR_MEM_02_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x208U))
#define PWR_MEM_03_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x20CU))
#define PWR_MEM_04_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x210U))
#define PWR_MEM_05_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x214U))
#define PWR_MEM_06_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x218U))
#define PWR_MEM_07_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x21CU))
#define PWR_MEM_08_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x220U))
#define PWR_MEM_09_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x224U))
#define PWR_MEM_10_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x228U))
#define PWR_MEM_11_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x22CU))
#define PWR_MEM_12_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x230U))
#define PWR_MEM_13_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x234U))
#define PWR_MEM_14_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x238U))
#define PWR_MEM_15_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x23CU))
#define PWR_MEM_16_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x240U))
#define PWR_MEM_17_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x244U))
#define PWR_MEM_18_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x248U))
#define PWR_MEM_19_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x24CU))
#define PWR_MEM_20_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x250U))
#define PWR_MEM_21_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x254U))
#define PWR_MEM_22_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x258U))
#define PWR_MEM_23_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x25CU))
#define PWR_MEM_24_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x260U))
#define PWR_MEM_25_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x264U))
#define PWR_MEM_26_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x268U))
#define PWR_MEM_27_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x26CU))
#define PWR_MEM_28_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x270U))
#define PWR_MEM_29_CONFIG_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x274U))
#define PWR_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x300U))
#define PWR_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x304U))
#define PWR_INTERRUPT_CLEAR_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x308U))
#define PWR_FAULT_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x310U))
#define PWR_FAULT_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x314U))
#define PWR_FAULT_INTERRUPT_CLEAR_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x318U))
#define PWR_MISC_CTRL_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0x320U))
#define PWR_CPU_BOOT_VINIT_REGISTER    (*(volatile uint32_t *)(POWER_CONTROL_BASE + 0xD00U))


/******************************************************************************/
/*                                  Security                                  */
/******************************************************************************/

#define SEC_IDENTIFIER_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0x000U))
#define SEC_STATUS_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0x004U))
#define SEC_CONFIGURATION_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0x008U))
#define SEC_CHALLENGE_BUFFER_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0x200U))
#define SEC_RESPONSE_BUFFER_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0x400U))
#define SEC_PRIMECELL_PERIPHERAL_ID_4_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFD0U))
#define SEC_PRIMECELL_PERIPHERAL_ID_5_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFD4U))
#define SEC_PRIMECELL_PERIPHERAL_ID_6_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFD8U))
#define SEC_PRIMECELL_PERIPHERAL_ID_7_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFDCU))
#define SEC_PRIMECELL_PERIPHERAL_ID_0_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFE0U))
#define SEC_PRIMECELL_PERIPHERAL_ID_1_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFE4U))
#define SEC_PRIMECELL_PERIPHERAL_ID_2_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFE8U))
#define SEC_PRIMECELL_PERIPHERAL_ID_3_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFECU))
#define SEC_PRIMECELL_COMPONENT_ID_0_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFF0U))
#define SEC_PRIMECELL_COMPONENT_ID_1_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFF4U))
#define SEC_PRIMECELL_COMPONENT_ID_2_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFF8U))
#define SEC_PRIMECELL_COMPONENT_ID_3_REGISTER    (*volatile uint32_t *)(SECURITY_BASE + 0xFFCU))


/******************************************************************************/
/*                           DVS ControlInterface                             */
/******************************************************************************/

#define DVS_ITC_IT_FLAGS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0x000U))
#define DVS_ITC_IT_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X004U))     
#define DVS_ITC_IT0_MASK_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X010U))
#define DVS_ITC_IT0_PENDING_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X014U))    
#define DVS_ITC_IT0_CLEAR_PENDING_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X01CU))
#define DVS_ITC_IT1_MASK_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X020U))
#define DVS_ITC_IT1_PENDING_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X024U))
#define DVS_ITC_IT1_CLEAR_PENDING_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X02CU)) 
#define DVS_ITC_IT_CLK_SAFE_MASK_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X030U))
#define DVS_ITC_IT_CLK_SAFE_PENDING_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X034U)) 
#define DVS_PD_APP0_TMFLT_STATUS0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X080U))
#define DVS_PD_APP0_TMFLT_STATUS1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X084U))
#define DVS_PD_APP0_TMFLT_STATUS2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X088U))
#define DVS_PD_APP0_TMFLT_STATUS3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X08CU))  
#define DVS_PD_APP0_TMFLT_WARNING_MASK0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X090U))
#define DVS_PD_APP0_TMFLT_WARNING_MASK1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X094U))
#define DVS_PD_APP0_TMFLT_WARNING_MASK2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X098U))
#define DVS_PD_APP0_TMFLT_WARNING_MASK3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X09CU))
#define DVS_PD_APP0_TMFLT_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X0A0U))
#define DVS_PD_APP1_TMFLT_STATUS0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X100U))
#define DVS_PD_APP1_TMFLT_STATUS1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X104U))
#define DVS_PD_APP1_TMFLT_STATUS2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X108U))
#define DVS_PD_APP1_TMFLT_STATUS3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X10CU)) 
#define DVS_PD_APP1_TMFLT_WARNING_MASK0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X110U))
#define DVS_PD_APP1_TMFLT_WARNING_MASK1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X114U))
#define DVS_PD_APP1_TMFLT_WARNING_MASK2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X118U))
#define DVS_PD_APP1_TMFLT_WARNING_MASK3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X11CU))
#define DVS_PD_APP1_TMFLT_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X120U))
#define DVS_PD_MODEM0_TMFLT_STATUS0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X180U))
#define DVS_PD_MODEM0_TMFLT_STATUS1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X184U))
#define DVS_PD_MODEM0_TMFLT_STATUS2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X188U))
#define DVS_PD_MODEM0_TMFLT_STATUS3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X18CU)) 
#define DVS_PD_MODEM0_TMFLT_WARNING_MASK0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X190U))
#define DVS_PD_MODEM0_TMFLT_WARNING_MASK1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X194U))
#define DVS_PD_MODEM0_TMFLT_WARNING_MASK2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X198U))
#define DVS_PD_MODEM0_TMFLT_WARNING_MASK3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X19CU))
#define DVS_PD_MODEM0_TMFLT_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X1A0U))
#define DVS_PD_MODEM1_TMFLT_STATUS0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X200U))
#define DVS_PD_MODEM1_TMFLT_STATUS1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X204U))
#define DVS_PD_MODEM1_TMFLT_STATUS2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X208U))
#define DVS_PD_MODEM1_TMFLT_STATUS3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X20CU))
#define DVS_PD_MODEM1_TMFLT_WARNING_MASK0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X210U))
#define DVS_PD_MODEM1_TMFLT_WARNING_MASK1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X214U))
#define DVS_PD_MODEM1_TMFLT_WARNING_MASK2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X218U))
#define DVS_PD_MODEM1_TMFLT_WARNING_MASK3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X21CU))     
#define DVS_PD_MODEM1_TMFLT_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X220U))     
#define DVS_PD_APP0_CTRL_POWER_MODE0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X280U))
#define DVS_PD_APP0_CTRL_POWER_MODE1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X284U))
#define DVS_PD_APP0_CTRL_POWER_MODE2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X288U))
#define DVS_PD_APP0_CTRL_POWER_MODE3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X28CU))    
#define DVS_PD_APP0_CTRL_POWER_MODE4_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X290U))
#define DVS_PD_APP0_CTRL_POWER_MODE5_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X294U))
#define DVS_PD_APP0_CTRL_POWER_MODE6_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X298U))
#define DVS_PD_APP0_CTRL_POWER_MODE7_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X29CU))    
#define DVS_PD_APP0_RING_SIGNATURE_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X2A0U))
#define DVS_PD_APP0_RING_STATUS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X2A4U))
#define DVS_PD_APP0_RING_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X2A8U))
#define DVS_PD_APP0_RING_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X2ACU))  
#define DVS_PD_APP1_CTRL_POWER_MODE0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X300U))
#define DVS_PD_APP1_CTRL_POWER_MODE1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X304U))
#define DVS_PD_APP1_CTRL_POWER_MODE2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X308U))
#define DVS_PD_APP1_CTRL_POWER_MODE3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X30CU))  
#define DVS_PD_APP1_CTRL_POWER_MODE4_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X310U))
#define DVS_PD_APP1_CTRL_POWER_MODE5_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X314U))
#define DVS_PD_APP1_CTRL_POWER_MODE6_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X318U))
#define DVS_PD_APP1_CTRL_POWER_MODE7_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X31CU))  
#define DVS_PD_APP1_RING_SIGNATURE_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X320U))
#define DVS_PD_APP1_RING_STATUS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X324U))
#define DVS_PD_APP1_RING_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X328U))
#define DVS_PD_APP1_RING_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X32CU)) 
#define DVS_PD_MODEM0_CTRL_POWER_MODE0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X380U))
#define DVS_PD_MODEM0_CTRL_POWER_MODE1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X384U))
#define DVS_PD_MODEM0_CTRL_POWER_MODE2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X388U))
#define DVS_PD_MODEM0_CTRL_POWER_MODE3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X38CU))    
#define DVS_PD_MODEM0_CTRL_POWER_MODE4_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X390U))
#define DVS_PD_MODEM0_CTRL_POWER_MODE5_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X394U))
#define DVS_PD_MODEM0_CTRL_POWER_MODE6_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X398U))
#define DVS_PD_MODEM0_CTRL_POWER_MODE7_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X39CU))    
#define DVS_PD_MODEM0_RING_SIGNATURE_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X3A0U))
#define DVS_PD_MODEM0_RING_STATUS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X3A4U))
#define DVS_PD_MODEM0_RING_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X3A8U))
#define DVS_PD_MODEM0_RING_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X3ACU))    
#define DVS_PD_MODEM1_CTRL_POWER_MODE0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X400U))
#define DVS_PD_MODEM1_CTRL_POWER_MODE1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X404U))
#define DVS_PD_MODEM1_CTRL_POWER_MODE2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X408U))
#define DVS_PD_MODEM1_CTRL_POWER_MODE3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X40CU))     
#define DVS_PD_MODEM1_CTRL_POWER_MODE4_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X410U))
#define DVS_PD_MODEM1_CTRL_POWER_MODE5_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X414U))
#define DVS_PD_MODEM1_CTRL_POWER_MODE6_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X418U))
#define DVS_PD_MODEM1_CTRL_POWER_MODE7_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X41CU))     
#define DVS_PD_MODEM1_RING_SIGNATURE_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X420U))
#define DVS_PD_MODEM1_RING_STATUS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X424U))
#define DVS_PD_MODEM1_RING_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X428U))
#define DVS_PD_MODEM1_RING_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X42CU))   
#define DVS_PD_MODEM2_CTRL_POWER_MODE0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X480U))
#define DVS_PD_MODEM2_CTRL_POWER_MODE1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X484U))
#define DVS_PD_MODEM2_CTRL_POWER_MODE2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X488U))
#define DVS_PD_MODEM2_CTRL_POWER_MODE3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X48CU))   
#define DVS_PD_MODEM2_CTRL_POWER_MODE4_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X490U))
#define DVS_PD_MODEM2_CTRL_POWER_MODE5_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X494U))
#define DVS_PD_MODEM2_CTRL_POWER_MODE6_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X498U))
#define DVS_PD_MODEM2_CTRL_POWER_MODE7_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X49CU))   
#define DVS_PD_MODEM2_RING_SIGNATURE_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X4A0U))
#define DVS_PD_MODEM2_RING_STATUS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X4A4U))
#define DVS_PD_MODEM2_RING_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X4A8U))
#define DVS_PD_MODEM2_RING_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X4ACU))   
#define DVS_PD_MODEM3_CTRL_POWER_MODE0_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X500U))
#define DVS_PD_MODEM3_CTRL_POWER_MODE1_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X504U))
#define DVS_PD_MODEM3_CTRL_POWER_MODE2_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X508U))
#define DVS_PD_MODEM3_CTRL_POWER_MODE3_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X50CU))   
#define DVS_PD_MODEM3_CTRL_POWER_MODE4_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X510U))
#define DVS_PD_MODEM3_CTRL_POWER_MODE5_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X514U))
#define DVS_PD_MODEM3_CTRL_POWER_MODE6_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X518U))
#define DVS_PD_MODEM3_CTRL_POWER_MODE7_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X51CU))    
#define DVS_PD_MODEM3_RING_SIGNATURE_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X520U))
#define DVS_PD_MODEM3_RING_STATUS_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X524U))
#define DVS_PD_MODEM3_RING_CONTROL_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X528U))
#define DVS_PD_MODEM3_RING_DEBUG_REGISTER    (*(volatile uint32_t *)(DVS_CONTROL_BASE + 0X52CU))


/******************************************************************************/
/*                           USB Control and Setup                            */
/******************************************************************************/

#define USB_SETUP_CONFIG_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0x000U))
#define USB_SETUP_DP_DM_PHY_CTRL_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X010U))
#define USB_SETUP_DP_DM_PHY_OVR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X014U))
#define USB_SETUP_DP_DM_PHY_RDBK_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X018U))
#define USB_SETUP_CORE_RDBK_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X01CU))    
#define USB_SETUP_DP_DM_PHY_EXTRA_CTRL1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X030U))
#define USB_SETUP_DP_DM_PHY_EXTRA_CTRL2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X034U))
#define USB_SETUP_DP_DM_PHY_EXTRA_CTRL3_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X038U))
#define USB_SETUP_DP_DM_PHY_EXTRA_RDBK1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X03CU))
#define USB_SETUP_DP_DM_PHY_EXTRA_RDBK2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_AND_SETUP_BASE + 0X040U))


/******************************************************************************/
/*                           SF AO Control Interface                          */
/******************************************************************************/

#define SF_MAGIC_NUMBER_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x000U))
#define SF_MEMORY_MARGIN_EN0_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x004U))
#define SF_MEMORY_MARGIN_EN1_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x008U))
#define SF_MEMORY_MARGIN_LSB0_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x00CU))
#define SF_MEMORY_MARGIN_LSB1_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x010U))
#define SF_MEMORY_MARGIN_MSB0_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x014U))
#define SF_MEMORY_MARGIN_MSB1_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x018U))
#define SF_REFCLK_CONTROL_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x01CU))
#define SF_DYNAMIC_VOLTAGE_SCALING_CONTROL_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x020U))
#define SF_PERIPHERAL_DMA_CONTROL_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x024U))
#define SF_SDIO_CONTROL_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x028U))
#define SF_RFFE_RESET_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x02CU))
#define SF_OSPI_CTRL_S_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x030U))
#define SF_OSPI_CTRL_C_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x034U))
#define SF_OSPI_CTRL_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x038U))
#define SF_RF_ANA_RST_ORIDE_S_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x03CU))
#define SF_RF_ANA_RST_ORIDE_C_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x040U))
#define SF_RF_ANA_RST_ORIDE_REGISTER    (*(volatile uint32_t *)(SF_AO_CONTROL_BASE + 0x044U))


/******************************************************************************/
/*                            OSPI Control Interface                          */
/******************************************************************************/

#define OSPI_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x000U))
#define OSPI_DEVICE_READ_INSTRUCTION_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x004U))
#define OSPI_DEVICE_WRITE_INSTRUCTION_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x008U))        
#define OSPI_DEVICE_DELAY_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x00CU))
#define OSPI_READ_DATA_CAPTURE_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x010U))
#define OSPI_DEVICE_SIZE_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x014U))
#define OSPI_SRAM_PARTITION_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x018U))  
#define OSPI_INDIRECT_AHB_ADDRESS_TRIGGER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x01CU))    
#define OSPI_DMA_PERIPHERAL_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x020U))   
#define OSPI_REMAP_ADDRESS_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x024U))
#define OSPI_MODE_BIT_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x028U))
#define OSPI_SRAM_FILL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x02CU))
#define OSPI_TX_THRESHOLD_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x030U))   
#define OSPI_RX_THRESHOLD_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x034U))
#define OSPI_WRITE_COMPLETION_CONTROL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x038U))
#define OSPI_POLLING_EXPIRATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x03CU))
#define OSPI_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x040U))   
#define OSPI_IRQ_MASK_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x044U))
#define OSPI_LOWER_WRITE_PROTECTION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x050U))   
#define OSPI_UPPER_WRITE_PROTECTION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x054U))
#define OSPI_WRITE_PROTECTION_CONTROL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x058U))
#define OSPI_INDIRECT_READ_TRANSFER_CONTROL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x060U))  
#define OSPI_INDIRECT_READ_TRANSFER_WATERMARK_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x064U))
#define OSPI_INDIRECT_READ_TRANSFER_START_ADDRESS_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x068U))   
#define OSPI_INDIRECT_READ_TRANSFER_NUMBER_BYTES_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x06CU))
#define OSPI_INDIRECT_WRITE_TRANSFER_CONTROL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x070U))   
#define OSPI_INDIRECT_WRITE_TRANSFER_WATERMARK_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x074U))
#define OSPI_INDIRECT_WRITE_TRANSFER_START_ADDRESS_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x078U))   
#define OSPI_INDIRECT_WRITE_TRANSFER_NUMBER_BYTES_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x07CU))
#define OSPI_INDIRECT_TRIGGER_ADDRESS_RANGE_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x080U))    
#define OSPI_FLASH_COMMAND_CONTROL_MEMORY_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x08CU))
#define OSPI_FLASH_COMMAND_CONTROL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x090U))   
#define OSPI_FLASH_COMMAND_ADDRESS_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x094U))
#define OSPI_FLASH_COMMAND_READ_DATA_LOWER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0A0U))   
#define OSPI_FLASH_COMMAND_READ_DATA_UPPER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0A4U))
#define OSPI_FLASH_COMMAND_WRITE_DATA_LOWER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0A8U))   
#define OSPI_FLASH_COMMAND_WRITE_DATA_UPPER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0ACU))
#define OSPI_POLLING_FLASH_STATUS_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0B0U))   
#define OSPI_PHY_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0B4U))
#define OSPI_PHY_DLL_MASTER_CONTROL_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0B8U))   
#define OSPI_DLL_OBSERVABLE_LOWER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0BCU))
#define OSPI_DLL_OBSERVABLE_UPPER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0C0U))
#define OSPI_OPCODE_EXTENSION_LOWER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0E0U))   
#define OSPI_OPCODE_EXTENSION_UPPER_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0E4U))
#define OSPI_MODULE_ID_REGISTER    (*(volatile uint32_t *)(OSPI_CONTROL_BASE + 0x0FCU))


/******************************************************************************/
/*                                     Watchdog                               */
/******************************************************************************/

//----------------------------------- WATCHDOG 1 ------------------------------------------//
#define WATCHDOG1_OVERFLOW_MODE_REGISTER    (*(volatile uint32_t *)(WATCHDOG1_BASE + 0x000U))
#define WATCHDOG1_CONTROL_MODE_REGISTER    (*(volatile uint32_t *)(WATCHDOG1_BASE + 0x004U))
#define WATCHDOG1_CONTROL_REGISTER    (*(volatile uint32_t *)(WATCHDOG1_BASE + 0x008U))
#define WATCHDOG1_STATUS_REGISTER    (*(volatile uint32_t *)(WATCHDOG1_BASE + 0x00CU))

//----------------------------------- WATCHDOG 2 ------------------------------------------//
#define WATCHDOG2_OVERFLOW_MODE_REGISTER    (*(volatile uint32_t *)(WATCHDOG2_BASE + 0x000U))
#define WATCHDOG2_CONTROL_MODE_REGISTER    (*(volatile uint32_t *)(WATCHDOG2_BASE + 0x004U))
#define WATCHDOG2_CONTROL_REGISTER    (*(volatile uint32_t *)(WATCHDOG2_BASE + 0x008U))
#define WATCHDOG2_STATUS_REGISTER    (*(volatile uint32_t *)(WATCHDOG2_BASE + 0x00CU))

//----------------------------------- WATCHDOG 3 ------------------------------------------//
#define WATCHDOG3_OVERFLOW_MODE_REGISTER    (*(volatile uint32_t *)(WATCHDOG3_BASE + 0x000U))
#define WATCHDOG3_CONTROL_MODE_REGISTER    (*(volatile uint32_t *)(WATCHDOG3_BASE + 0x004U))
#define WATCHDOG3_CONTROL_REGISTER    (*(volatile uint32_t *)(WATCHDOG3_BASE + 0x008U))
#define WATCHDOG3_STATUS_REGISTER    (*(volatile uint32_t *)(WATCHDOG3_BASE + 0x00CU))


/******************************************************************************/
/*                                     Timer                                  */
/******************************************************************************/

#define TIMER1_CTRL_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x000U))
#define TIMER1_LOAD_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x004U))
#define TIMER1_TIME_VALUE_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x008U))
#define TIMER1_STATUS_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x00CU))

#define TIMER2_CTRL_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x020U))
#define TIMER2_LOAD_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x024U))
#define TIMER2_TIME_VALUE_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x028U))
#define TIMER2_STATUS_REGISTER    (*(volatile uint32_t *)(TIMER_BASE + 0x02CU))


/******************************************************************************/
/*                                     I2C Control Interface                  */
/******************************************************************************/

//--------------------------------------- I2C 1 --------------------------------------//
#define I2C1_CONTROL_SET_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x000U))
#define I2C1_CONTROL_CLEAR_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x004U))
#define I2C1_CONTROL_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x008U))
#define I2C1_INTERRUPT_ENABLE_SET_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x010U))
#define I2C1_INTERRUPT_ENABLE_CLEAR_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x014U))
#define I2C1_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x018U))
#define I2C1_STATUS_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x020U))
#define I2C1_OWN_ADDRESS_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x024U))
#define I2C1_CLOCK_CONTROL_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x028U))
#define I2C1_TRANSMIT_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x02CU))
#define I2C1_RECEIVE_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x030U))
#define I2C1_TRANSMIT_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x034U))
#define I2C1_RECEIVE_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x038U))
#define I2C1_NUMBER_RECEIVED_BYTES_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x03CU))
#define I2C1_TRANSMIT_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x080U))
#define I2C1_RECEIVE_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(I2C1_CONTROL_BASE + 0x100U))

//--------------------------------------- I2C 2 --------------------------------------//
#define I2C2_CONTROL_SET_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x000U))
#define I2C2_CONTROL_CLEAR_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x004U))
#define I2C2_CONTROL_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x008U))
#define I2C2_INTERRUPT_ENABLE_SET_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x010U))
#define I2C2_INTERRUPT_ENABLE_CLEAR_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x014U))
#define I2C2_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x018U))
#define I2C2_STATUS_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x020U))
#define I2C2_OWN_ADDRESS_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x024U))
#define I2C2_CLOCK_CONTROL_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x028U))
#define I2C2_TRANSMIT_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x02CU))
#define I2C2_RECEIVE_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x030U))
#define I2C2_TRANSMIT_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x034U))
#define I2C2_RECEIVE_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x038U))
#define I2C2_NUMBER_RECEIVED_BYTES_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x03CU))
#define I2C2_TRANSMIT_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x080U))
#define I2C2_RECEIVE_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(I2C2_CONTROL_BASE + 0x100U))

//--------------------------------------- I2C 3 --------------------------------------//
#define I2C3_CONTROL_SET_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x000U))
#define I2C3_CONTROL_CLEAR_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x004U))
#define I2C3_CONTROL_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x008U))
#define I2C3_INTERRUPT_ENABLE_SET_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x010U))
#define I2C3_INTERRUPT_ENABLE_CLEAR_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x014U))
#define I2C3_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x018U))
#define I2C3_STATUS_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x020U))
#define I2C3_OWN_ADDRESS_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x024U))
#define I2C3_CLOCK_CONTROL_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x028U))
#define I2C3_TRANSMIT_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x02CU))
#define I2C3_RECEIVE_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x030U))
#define I2C3_TRANSMIT_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x034U))
#define I2C3_RECEIVE_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x038U))
#define I2C3_NUMBER_RECEIVED_BYTES_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x03CU))
#define I2C3_TRANSMIT_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x080U))
#define I2C3_RECEIVE_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(I2C3_CONTROL_BASE + 0x100U))


/******************************************************************************/
/*                                     PIO Control Interface                  */
/******************************************************************************/

//------------------------------------- PIO 0 ----------------------------------------//
#define PIO0_PIO_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x000U))
#define PIO0_PIO_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x004U))
#define PIO0_PIO_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x008U))
#define PIO0_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x010U))
#define PIO0_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x014U))
#define PIO0_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x018U))
#define PIO0_OUTPUT_DIRECTION_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x01CU))
#define PIO0_INPUT_FILTER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x020U))
#define PIO0_INPUT_FILTER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x024U))
#define PIO0_INPUT_FILTER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x028U))
#define PIO0_SET_OUTPUT_DATA_SODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x030U))
#define PIO0_CLEAR_OUTPUT_DATA_CODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x034U))
#define PIO0_OUTPUT_DATA_STATUS_ODSR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x038U))
#define PIO0_PIN_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x03CU))
#define PIO0_IST_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x040U))
#define PIO0_IST_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x044U))
#define PIO0_IST_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x048U))
#define PIO0_2ND_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x050U))
#define PIO0_2ND_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x054U))
#define PIO0_2ND_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x058U))
#define PIO0_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x05CU))
#define PIO0_MULTI_DRIVER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x060U))
#define PIO0_MULTI_DRIVER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x064U))
#define PIO0_MULTI_DRIVER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x068U))
#define PIO0_PIO_CHECK_IN_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x070U))
#define PIO0_PIO_GENERAL_PURPOSE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x074U))
#define PIO0_SELECT_A_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x078U))
#define PIO0_SELECT_B_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x07CU))
#define PIO0_SELECT_C_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x080U))
#define PIO0_SELECT_D_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x084U))
#define PIO0_MSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x088U))
#define PIO0_LSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x07CU))
#define PIO0_MSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x090U))
#define PIO0_MSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x094U))
#define PIO0_MSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x098U))
#define PIO0_LSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0A0U))
#define PIO0_LSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0A4U))
#define PIO0_LSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0A8U)) 
#define PIO0_SLEW_RATE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0B0U))
#define PIO0_SLEW_RATE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0B4U))
#define PIO0_SLEW_RATE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0B8U)) 
#define PIO0_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0C0U))
#define PIO0_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0C4U))
#define PIO0_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0C8U)) 
#define PIO0_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0D0U))
#define PIO0_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0D4U))
#define PIO0_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0D8U))
#define PIO0_SCHMITT_TRIGGER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0E0U))
#define PIO0_SCHMITT_TRIGGER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0E4U))
#define PIO0_SCHMITT_TRIGGER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0E8U)) 
#define PIO0_INPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0F0U))
#define PIO0_INPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0F4U))
#define PIO0_INPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x0F8U)) 
#define PIO0_PERIPH_CTRL_PU_PD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x100U))
#define PIO0_PERIPH_CTRL_PU_PD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x104U))
#define PIO0_PERIPH_CTRL_PU_PD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x108U)) 
#define PIO0_HW_OBSERVATION_SELECTOR_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x110U))
#define PIO0_INPUT_FILTER_STATUS_SELECTOR_2_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x114U))
#define PIO0_INPUT_FILTER_STATUS_SELECTOR_3_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x118U)) 
#define PIO0_INTERRUPT_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x120U))
#define PIO0_INTERRUPT_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x124U))
#define PIO0_INTERRUPT_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x128U))
#define PIO0_INTERRUPT_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x130U))
#define PIO0_INTERRUPT_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x134U))
#define PIO0_INTERRUPT_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x138U)) 
#define PIO0_INTERRUPT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x13CU)) 
#define PIO0_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x140U))
#define PIO0_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x144U))
#define PIO0_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x148U)) 
#define PIO0_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x14CU)) 
#define PIO0_PIO_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x150U))
#define PIO0_PIO_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x154U))
#define PIO0_PIO_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x158U)) 
#define PIO0_PIO_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x15CU)) 
#define PIO0_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x160U))
#define PIO0_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x164U))
#define PIO0_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x168U)) 
#define PIO0_WAKEUP_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x16CU)) 
#define PIO0_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x170U))
#define PIO0_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x174U))
#define PIO0_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x178U)) 
#define PIO0_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x17CU)) 
#define PIO0_PIO_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x180U))
#define PIO0_PIO_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x184U))
#define PIO0_PIO_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x188U))
#define PIO0_PIO_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x190U))
#define PIO0_PIO_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x194U))
#define PIO0_PIO_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x198U))
#define PIO0_PIO_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x19CU))
#define PIO0_WAKEUP_REDUX_0_3_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1A0U))
#define PIO0_WAKEUP_REDUX_4_7_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1A4U))
#define PIO0_WAKEUP_REDUX_8_11_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1A8U))
#define PIO0_WAKEUP_REDUX_12_15_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1ACU)) 
#define PIO0_WAKEUP_REDUX_16_19_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1B0U))
#define PIO0_WAKEUP_REDUX_20_23_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1B4U))
#define PIO0_WAKEUP_REDUX_24_27_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1B8U))
#define PIO0_WAKEUP_REDUX_28_31_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1BCU)) 
#define PIO0_ALTERNATIVE_PAD_CFG_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1C0U))
#define PIO0_ALTERNATIVE_PAD_CFG_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1C4U))
#define PIO0_ALTERNATIVE_PAD_CFG_ENABLE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1C8U))
#define PIO0_ALTERNATIVE_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1D0U))
#define PIO0_ALTERNATIVE_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1D4U))
#define PIO0_ALTERNATIVE_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1D8U))
#define PIO0_SET_OUTPUT_DATA_ASODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1E0U))
#define PIO0_CLEAR_OUTPUT_DATA_ACODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1E4U))
#define PIO0_OUTPUT_DATA_STATUS_AODSR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1E8U))
#define PIO0_ALT_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1F0U))
#define PIO0_ALT_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1F4U))
#define PIO0_ALT_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x1F8U))
#define PIO0_ALT_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x200U))
#define PIO0_ALT_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x204U))
#define PIO0_ALT_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x208U))
#define PIO0_ALTERNATIVE_PAD_CONFIGURATION_HOLD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x210U))
#define PIO0_ALTERNATIVE_PAD_CONFIGURATION_HOLD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x214U))
#define PIO0_ALTERNATIVE_PAD_CONFIGURATION_HOLD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x218U))
 
//------------------------------------- PIO 1 ----------------------------------------//
#define PIO1_PIO_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x220U))
#define PIO1_PIO_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x224U))
#define PIO1_PIO_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x228U))
#define PIO1_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x230U))
#define PIO1_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x234U))
#define PIO1_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x238U))
#define PIO1_OUTPUT_DIRECTION_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x238U))
#define PIO1_INPUT_FILTER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x240U))
#define PIO1_INPUT_FILTER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x244U))
#define PIO1_INPUT_FILTER_STATUSSELECTOR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x248U))
#define PIO1_SET_OUTPUT_DATA_SODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x250U))
#define PIO1_CLEAR_OUTPUT_DATA_CODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x254U))
#define PIO1_OUTPUT_OUTPUT_DATA_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x258U))
#define PIO1_PIN DATA STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x25CU))
#define PIO1_IST_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x260U))
#define PIO1_IST_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x264U))
#define PIO1_IST_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x268U))
#define PIO1_2ND_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x270U))
#define PIO1_2ND_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x274U))
#define PIO1_2ND_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x278U))
#define PIO1_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x27CU))
#define PIO1_MULTI_DRIVER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x280U))
#define PIO1_MULTI_DRIVER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x284U))
#define PIO1_MULTI_DRIVER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x288U))
#define PIO1_PIO_CHECK_IN_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x290U))
#define PIO1_PIO_GENERAL_PURPOSE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x294U))
#define PIO1_SELECT_A_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x298U))
#define PIO1_SELECT_B_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x29CU))
#define PIO1_SELECT_C_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2A0U))
#define PIO1_SELECT_D_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2A4U))
#define PIO1_MSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2A8U))
#define PIO1_LSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2ACU))
#define PIO1_MSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2B0U))
#define PIO1_MSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2B4U))
#define PIO1_MSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2B8U))
#define PIO1_LSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2C0U))
#define PIO1_LSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2C4U))
#define PIO1_LSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2C8U)) 
#define PIO1_SLEW_RATE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2D0U))
#define PIO1_SLEW_RATE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2D4U))
#define PIO1_SLEW_RATE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2D8U)) 
#define PIO1_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2E0U))
#define PIO1_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2E4U))
#define PIO1_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2E8U)) 
#define PIO1_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2F0U))
#define PIO1_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2F4U))
#define PIO1_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x2F8U)) 
#define PIO1_SCHMITT_TRIGGER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x300U))
#define PIO1_SCHMITT_TRIGGER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x304U))
#define PIO1_SCHMITT_TRIGGER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x308U)) 
#define PIO1_INPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x310U))
#define PIO1_INPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x314U))
#define PIO1_INPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x318U)) 
#define PIO1_PERIPH_CTRL_PU_PD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x320U))
#define PIO1_PERIPH_CTRL_PU_PD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x324U))
#define PIO1_PERIPH_CTRL_PU_PD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x328U)) 
#define PIO1_HW_OBSERVATION_SELECTOR_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x330U))
#define PIO1_INPUT_FILTER_STATUS_SELECTOR_2_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x334U))
#define PIO1_INPUT_FILTER_STATUS_SELECTOR_3_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x338U)) 
#define PIO1_INTERRUPT_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x340U))
#define PIO1_INTERRUPT_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x344U))
#define PIO1_INTERRUPT_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x348U)) 
#define PIO1_INTERRUPT_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x350U))
#define PIO1_INTERRUPT_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x354U))
#define PIO1_INTERRUPT_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x358U)) 
#define PIO1_INTERRUPT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x35CU)) 
#define PIO1_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x360U))
#define PIO1_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x364U))
#define PIO1_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x368U)) 
#define PIO1_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x36CU)) 
#define PIO1_PIO_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x370U))
#define PIO1_PIO_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x374U))
#define PIO1_PIO_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x378U)) 
#define PIO1_PIO_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x37CU)) 
#define PIO1_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x380U))
#define PIO1_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x384U))
#define PIO1_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x388U)) 
#define PIO1_WAKEUP_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x38CU)) 
#define PIO1_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x390U))
#define PIO1_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x394U))
#define PIO1_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x398U)) 
#define PIO1_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x39CU)) 
#define PIO1_PIO_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3A0U))
#define PIO1_PIO_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3A4U))
#define PIO1_PIO_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3A8U)) 
#define PIO1_PIO_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3B0U))
#define PIO1_PIO_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3B4U))
#define PIO1_PIO_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3B8U))
#define PIO1_PIO_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3BCU)) 
#define PIO1_WAKEUP_REDUX_0_3_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3C0U))
#define PIO1_WAKEUP_REDUX_4_7_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3C4U))
#define PIO1_WAKEUP_REDUX_8_11_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3C8U))
#define PIO1_WAKEUP_REDUX_12_15_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3CCU)) 
#define PIO1_WAKEUP_REDUX_16_19_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3D0U))
#define PIO1_WAKEUP_REDUX_20_23_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3D4U))
#define PIO1_WAKEUP_REDUX_24_27_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3D8U))
#define PIO1_WAKEUP_REDUX_28_31_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3DCU)) 
#define PIO1_ALTERNATIVE_PAD_CFG_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3E0U))
#define PIO1_ALTERNATIVE_PAD_CFG_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3E4U))
#define PIO1_ALTERNATIVE_PAD_CFG_ENABLE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3E8U))
#define PIO1_ALTERNATIVE_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3F0U))
#define PIO1_ALTERNATIVE_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3F4U))
#define PIO1_ALTERNATIVE_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x3F8U))
#define PIO1_SET_OUTPUT_DATA_ASODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x400U))
#define PIO1_CLEAR_OUTPUT_DATA_ACODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x404U))
#define PIO1_OUTPUT_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x408U))
#define PIO1_ALT_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x410U))
#define PIO1_ALT_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x414U))
#define PIO1_ALT_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x418U))
#define PIO1_ALT_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x420U))
#define PIO1_ALT_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x424U))
#define PIO1_ALT_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x428U))
#define PIO1_ALTERNATIVE_PAD_CONFIGURATION_HOLD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x430U))
#define PIO1_ALTERNATIVE_PAD_CONFIGURATION_HOLD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x434U))
#define PIO1_ALTERNATIVE_PAD_CONFIGURATION_HOLD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x438U))
   
//------------------------------------- PIO 2 ----------------------------------------//
#define PIO2_PIO_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x440U))
#define PIO2_PIO_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x444U))
#define PIO2_PIO_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x448U))
#define PIO2_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x450U))
#define PIO2_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x454U))
#define PIO2_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x458U))
#define PIO2_OUTPUT_DIRECTION_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x45CU))
#define PIO2_INPUT_FILTER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x460U))
#define PIO2_INPUT_FILTER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x464U))
#define PIO2_INPUT_FILTER_STATUSSELECTOR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x468U))
#define PIO2_SET_OUTPUT_DATA_SODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x470U))
#define PIO2_CLEAR_OUTPUT_DATA_CODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x474U))
#define PIO2_OUTPUT_OUTPUT_DATA_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x478U))
#define PIO2_PIN DATA STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x47CU))
#define PIO2_IST_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x480U))
#define PIO2_IST_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x484U))
#define PIO2_IST_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x488U))
#define PIO2_2ND_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x490U))
#define PIO2_2ND_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x494U))
#define PIO2_2ND_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x498U))
#define PIO2_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x49CU))
#define PIO2_MULTI_DRIVER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4A0U))
#define PIO2_MULTI_DRIVER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4A4U))
#define PIO2_MULTI_DRIVER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4A8U))
#define PIO2_PIO_CHECK_IN_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4B0U))
#define PIO2_PIO_GENERAL_PURPOSE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4B4U))
#define PIO2_SELECT_A_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4B8U))
#define PIO2_SELECT_B_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4BCU))
#define PIO2_SELECT_C_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4C0U))
#define PIO2_SELECT_D_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4C4U))
#define PIO2_MSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4C8U))
#define PIO2_LSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4CCU))
#define PIO2_MSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4D0U))
#define PIO2_MSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4D4U))
#define PIO2_MSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4D8U))
#define PIO2_LSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4E0U))
#define PIO2_LSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4E4U))
#define PIO2_LSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4E8U)) 
#define PIO2_SLEW_RATE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4F0U))
#define PIO2_SLEW_RATE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4F4U))
#define PIO2_SLEW_RATE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x4F8U)) 
#define PIO2_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x500U))
#define PIO2_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x504U))
#define PIO2_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x508U)) 
#define PIO2_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x510U))
#define PIO2_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x514U))
#define PIO2_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x518U)) 
#define PIO2_SCHMITT_TRIGGER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x520U))
#define PIO2_SCHMITT_TRIGGER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x524U))
#define PIO2_SCHMITT_TRIGGER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x528U)) 
#define PIO2_INPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x530U))
#define PIO2_INPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x534U))
#define PIO2_INPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x538U)) 
#define PIO2_PERIPH_CTRL_PU_PD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x540U))
#define PIO2_PERIPH_CTRL_PU_PD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x544U))
#define PIO2_PERIPH_CTRL_PU_PD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x548U)) 
#define PIO2_HW_OBSERVATION_SELECTOR_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x550U))
#define PIO2_INPUT_FILTER_STATUS_SELECTOR_2_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x554U))
#define PIO2_INPUT_FILTER_STATUS_SELECTOR_3_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x558U)) 
#define PIO2_INTERRUPT_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x560U))
#define PIO2_INTERRUPT_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x564U))
#define PIO2_INTERRUPT_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x568U)) 
#define PIO2_INTERRUPT_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x570U))
#define PIO2_INTERRUPT_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x574U))
#define PIO2_INTERRUPT_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x578U)) 
#define PIO2_INTERRUPT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x57CU)) 
#define PIO2_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x580U))
#define PIO2_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x584U))
#define PIO2_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x588U)) 
#define PIO2_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x58CU)) 
#define PIO2_PIO_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x590U))
#define PIO2_PIO_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x594U))
#define PIO2_PIO_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x598U)) 
#define PIO2_PIO_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x59CU)) 
#define PIO2_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5A0U))
#define PIO2_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5A4U))
#define PIO2_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5A8U)) 
#define PIO2_WAKEUP_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5ACU)) 
#define PIO2_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5B0U))
#define PIO2_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5B4U))
#define PIO2_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5B8U)) 
#define PIO2_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5BCU)) 
#define PIO2_PIO_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5C0U))
#define PIO2_PIO_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5C4U))
#define PIO2_PIO_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5C8U)) 
#define PIO2_PIO_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5D0U))
#define PIO2_PIO_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5D4U))
#define PIO2_PIO_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5D8U))
#define PIO2_PIO_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5DCU))  
#define PIO2_WAKEUP_REDUX_0_3_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5E0U))
#define PIO2_WAKEUP_REDUX_4_7_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5E4U))
#define PIO2_WAKEUP_REDUX_8_11_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5E8U))
#define PIO2_WAKEUP_REDUX_12_15_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5ECU)) 
#define PIO2_WAKEUP_REDUX_16_19_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5F0U))
#define PIO2_WAKEUP_REDUX_20_23_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5F4U))
#define PIO2_WAKEUP_REDUX_24_27_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5F8U))
#define PIO2_WAKEUP_REDUX_28_31_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x5FCU)) 
#define PIO2_ALTERNATIVE_PAD_CFG_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x600U))
#define PIO2_ALTERNATIVE_PAD_CFG_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x604U))
#define PIO2_ALTERNATIVE_PAD_CFG_ENABLE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x608U))
#define PIO2_ALTERNATIVE_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x610U))
#define PIO2_ALTERNATIVE_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x614U))
#define PIO2_ALTERNATIVE_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x618U))
#define PIO2_SET_OUTPUT_DATA_ASODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x620U))
#define PIO2_CLEAR_OUTPUT_DATA_ACODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x624U))
#define PIO2_OUTPUT_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x628U))
#define PIO2_ALT_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x630U))
#define PIO2_ALT_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x634U))
#define PIO2_ALT_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x638U))
#define PIO2_ALT_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x640U))
#define PIO2_ALT_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x644U))
#define PIO2_ALT_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x648U))
#define PIO2_ALTERNATIVE_PAD_CONFIGURATION_HOLD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x650U))
#define PIO2_ALTERNATIVE_PAD_CONFIGURATION_HOLD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x654U))
#define PIO2_ALTERNATIVE_PAD_CONFIGURATION_HOLD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x658U))
   
//------------------------------------- PIO 3 ----------------------------------------//
#define PIO3_PIO_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x660U))
#define PIO3_PIO_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x664U))
#define PIO3_PIO_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x668U))
#define PIO3_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x670U))
#define PIO3_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x674U))
#define PIO3_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x678U))
#define PIO3_OUTPUT_DIRECTION_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x67CU))
#define PIO3_INPUT_FILTER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x680U))
#define PIO3_INPUT_FILTER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x684U))
#define PIO3_INPUT_FILTER_STATUSSELECTOR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x688U))
#define PIO3_SET_OUTPUT_DATA_SODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x690U))
#define PIO3_CLEAR_OUTPUT_DATA_CODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x694U))
#define PIO3_OUTPUT_OUTPUT_DATA_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x698U))
#define PIO3_PIN DATA STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x69CU))
#define PIO3_IST_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6A0U))
#define PIO3_IST_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6A4U))
#define PIO3_IST_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6A8U))
#define PIO3_2ND_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6B0U))
#define PIO3_2ND_INTERRUPT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6B4U))
#define PIO3_2ND_INTERRUPT_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6B8U))
#define PIO3_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6BCU))
#define PIO3_MULTI_DRIVER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6C0U))
#define PIO3_MULTI_DRIVER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6C4U))
#define PIO3_MULTI_DRIVER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6C8U))
#define PIO3_PIO_CHECK_IN_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6D0U))
#define PIO3_PIO_GENERAL_PURPOSE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6D4U))
#define PIO3_SELECT_A_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6D8U))
#define PIO3_SELECT_B_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6DCU))
#define PIO3_SELECT_C_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6E0U))
#define PIO3_SELECT_D_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6E4U))
#define PIO3_MSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6E8U))
#define PIO3_LSB_SELECT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6ECU))
#define PIO3_MSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6F0U))
#define PIO3_MSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6F4U))
#define PIO3_MSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x6F8U))
#define PIO3_LSB_DRIVE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x700U))
#define PIO3_LSB_DRIVE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x704U))
#define PIO3_LSB_DRIVE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x708U)) 
#define PIO3_SLEW_RATE_STRENGTH_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x710U))
#define PIO3_SLEW_RATE_STRENGTH_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x714U))
#define PIO3_SLEW_RATE_STRENGTH_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x718U)) 
#define PIO3_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x720U))
#define PIO3_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x724U))
#define PIO3_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x728U)) 
#define PIO3_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x730U))
#define PIO3_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x734U))
#define PIO3_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x738U)) 
#define PIO3_SCHMITT_TRIGGER_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x740U))
#define PIO3_SCHMITT_TRIGGER_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x744U))
#define PIO3_SCHMITT_TRIGGER_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x748U)) 
#define PIO3_INPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x750U))
#define PIO3_INPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x754U))
#define PIO3_INPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x758U)) 
#define PIO3_PERIPH_CTRL_PU_PD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x760U))
#define PIO3_PERIPH_CTRL_PU_PD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x764U))
#define PIO3_PERIPH_CTRL_PU_PD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x768U)) 
#define PIO3_HW_OBSERVATION_SELECTOR_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x770U))
#define PIO3_INPUT_FILTER_STATUS_SELECTOR_2_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x774U))
#define PIO3_INPUT_FILTER_STATUS_SELECTOR_3_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x778U)) 
#define PIO3_INTERRUPT_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x780U))
#define PIO3_INTERRUPT_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x784U))
#define PIO3_INTERRUPT_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x788U)) 
#define PIO3_INTERRUPT_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x790U))
#define PIO3_INTERRUPT_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x794U))
#define PIO3_INTERRUPT_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x798U)) 
#define PIO3_INTERRUPT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x79CU)) 
#define PIO3_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7A0U))
#define PIO3_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7A4U))
#define PIO3_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7A8U)) 
#define PIO3_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7ACU)) 
#define PIO3_PIO_WAKEUP_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7B0U))
#define PIO3_PIO_WAKEUP_SOURCE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7B4U))
#define PIO3_PIO_WAKEUP_SOURCE_MASK_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7B8U)) 
#define PIO3_PIO_WAKEUP_SOURCE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7BCU)) 
#define PIO3_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7C0U))
#define PIO3_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7C4U))
#define PIO3_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7C8U)) 
#define PIO3_WAKEUP_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7CCU)) 
#define PIO3_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7D0U))
#define PIO3_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7D4U))
#define PIO3_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7D8U)) 
#define PIO3_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7DCU)) 
#define PIO3_PIO_WAKEUP_EVENT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7E0U))
#define PIO3_PIO_WAKEUP_EVENT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7E4U))
#define PIO3_PIO_WAKEUP_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7E8U)) 
#define PIO3_PIO_WAKEUP_EVENT_TYPE_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7F0U))
#define PIO3_PIO_WAKEUP_EVENT_TYPE_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7F4U))
#define PIO3_PIO_WAKEUP_EVENT_TYPE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7F8U))
#define PIO3_PIO_WAKEUP_EVENT_DOUBLE_EDGE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x7FCU))  
#define PIO3_WAKEUP_REDUX_0_3_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x800U))
#define PIO3_WAKEUP_REDUX_4_7_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x804U))
#define PIO3_WAKEUP_REDUX_8_11_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x808U))
#define PIO3_WAKEUP_REDUX_12_15_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x80CU))  
#define PIO3_WAKEUP_REDUX_16_19_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x810U))
#define PIO3_WAKEUP_REDUX_20_23_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x814U))
#define PIO3_WAKEUP_REDUX_24_27_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x818U))
#define PIO3_WAKEUP_REDUX_28_31_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x81CU)) 
#define PIO3_ALTERNATIVE_PAD_CFG_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x820U))
#define PIO3_ALTERNATIVE_PAD_CFG_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x824U))
#define PIO3_ALTERNATIVE_PAD_CFG_ENABLE_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x828U))
#define PIO3_ALTERNATIVE_OUTPUT_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x830U))
#define PIO3_ALTERNATIVE_OUTPUT_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x834U))
#define PIO3_ALTERNATIVE_OUTPUT_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x838U))
#define PIO3_SET_OUTPUT_DATA_ASODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x840U))
#define PIO3_CLEAR_OUTPUT_DATA_ACODR_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x844U))
#define PIO3_OUTPUT_DATA_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x848U))
#define PIO3_ALT_PULL_TO_HIGH_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x850U))
#define PIO3_ALT_PULL_TO_HIGH_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x854U))
#define PIO3_ALT_PULL_TO_HIGH_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x858U))
#define PIO3_ALT_PULL_TO_LOW_LEVEL_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x860U))
#define PIO3_ALT_PULL_TO_LOW_LEVEL_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x864U))
#define PIO3_ALT_PULL_TO_LOW_LEVEL_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x868U))
#define PIO3_ALTERNATIVE_PAD_CONFIGURATION_HOLD_ENABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x870U))
#define PIO3_ALTERNATIVE_PAD_CONFIGURATION_HOLD_DISABLE_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x874U))
#define PIO3_ALTERNATIVE_PAD_CONFIGURATION_HOLD_STATUS_REGISTER    (*(volatile uint32_t *)(PIO_CONTROL_BASE + 0x878U))
   

/******************************************************************************/
/*                                     OTP Control Interface                  */
/******************************************************************************/

#define OTP_CURRENT_STATUS_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x000U))
#define OTP_EXECUTE_COMMAND_SET_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x004U))
#define OTP_ADDRESS_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x008U))        
#define OTP_WRITE_DATA_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x00CU))
#define OTP_COUNT_STEP_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x010U))
#define OTP_BURN_COUNT_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x014U))
#define OTP_ERROR_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x018U))  
#define OTP_PAGE_READ_LOCK1_CONTROL_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x01CU))     
#define OTP_PAGE_READ_LOCK2_CONTROL_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x020U))   
#define OTP_IRQ_MASK_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x024U))
#define OTP_SHADOW_REGION_1_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x100U))
#define OTP_SHADOW_REGION_2_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0x104U))
#define OTP_PRIMECELL_PERIPHERAL_ID_4_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFD0U))
#define OTP_PRIMECELL_PERIPHERAL_ID_5_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFD4U))
#define OTP_PRIMECELL_PERIPHERAL_ID_6_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFD8U))
#define OTP_PRIMECELL_PERIPHERAL_ID_7_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFDCU))
#define OTP_PRIMECELL_PERIPHERAL_ID_0_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFE0U))
#define OTP_PRIMECELL_PERIPHERAL_ID_1_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFE4U))
#define OTP_PRIMECELL_PERIPHERAL_ID_2_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFE8U))
#define OTP_PRIMECELL_PERIPHERAL_ID_3_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFECU))
#define OTP_PRIMECELL_COMPONENT_ID_0_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFF0U))
#define OTP_PRIMECELL_COMPONENT_ID_1_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFF4U))
#define OTP_PRIMECELL_COMPONENT_ID_2_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFF8U))
#define OTP_PRIMECELL_COMPONENT_ID_3_REGISTER    (*(volatile uint32_t *)(OTP_CONTROL_BASE + 0xFFCU))


/******************************************************************************/
/*                             GNSS Application Timing                        */
/******************************************************************************/

#define GNSS_APP_CONTROL_ENABLE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x000U))
#define GNSS_APP_CONTROL_DISABLE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x004U))
#define GNSS_APP_CONTROL_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x008U))
#define GNSS_APP_ENABLE_RTC_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x00CU))
#define GNSS_APP_RTC_COUNTER_PRELOAD_MSB_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x010U))
#define GNSS_APP_RTC_COUNTER_PRELOAD_LSB_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x014U))
#define GNSS_APP_TRIGGER_FUNCITONALITY_ENABLE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x018U))
#define GNSS_APP_TIMESTAMP_TRIGGER_CONTROL_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x01CU))
#define GNSS_APP_GPO_CONTROL_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x020U))
#define GNSS_APP_INTERRUPT_STATUS_MASK_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x024U))
#define GNSS_APP_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x028U))
#define GNSS_APP_APPS_CPU_TIMESTAMP_MANUAL_TRIGGER_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x02CU))
#define GNSS_APP_RTC_VALUE_READ_REQUEST_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x030U))
#define GNSS_APP_RTC_MSB_VALUE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x034U))
#define GNSS_APP_RTC_LSB_VALUE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x038U))
#define GNSS_APP_TIMER_COMPARISON_ENABLE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x03CU))
#define GNSS_APP_TIMER_COMPARISON_MSB_VALUE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x040U))
#define GNSS_APP_TIMER_COMPARISON_LSB_VALUE_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x044U))
#define GNSS_APP_TIMER_COMPARISON_MASK_MSB_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x048U))
#define GNSS_APP_TIMER_COMPARISON_MASK_LSB_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x04CU))
#define GNSS_APP_EDGE_COUNTER_REGISTER    (*(volatile uint32_t *)(GNSS_APPS_TIMING_BASE + 0x050U))


/******************************************************************************/
/*                               TDR Access Gateway                           */
/******************************************************************************/

#define TDR_CONTROL_REGISTER    (*(volatile uint32_t *)(TDR_ACCESS_GATEWAY_BASE + 0x000U))
#define TDR_DATA_REGISTER    (*(volatile uint32_t *)(TDR_ACCESS_GATEWAY_BASE + 0x004U))
#define TDR_WRITEKEY_REGISTER    (*(volatile uint32_t *)(TDR_ACCESS_GATEWAY_BASE + 0x008U))


/******************************************************************************/
/*                               USART Control Interface                      */
/******************************************************************************/

//---------------------------------------- UART 1 -----------------------------------------//
#define UART1_CONTROL_CRS_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x000U))
#define UART1_CONTROL_CRC_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x004U))
#define UART1_CONTROL_CR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x008U))
#define UART1_INTERRUPT_ENABLE_IMRS_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x010U))
#define UART1_INTERRUPT_ENABLE_IMRC_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x014U))
#define UART1_INTERRUPT_ENABLE_IMR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x018U))
#define UART1_STATUS_SR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x020U))
#define UART1_BAUDRATE_BR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x024U))
#define UART1_TOR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x028U))
#define UART1_TX_FIFO_FLUSH_TFFR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x02CU))
#define UART1_RX_FIFO_FLUSH_RFFR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x030U))
#define UART1_TX_FIFO_DATA_WORDS_CURRENT_TWCR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x034U))
#define UART1_RX_FIFO_DATA_WORDS_CURRENT_RWCR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x038U))
#define UART1_DST_EXT_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x03CU))
#define UART1_AUTOBAUD_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x040U))
#define UART1_NCO_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x044U))
#define UART1_TX_DATA_BUFFER_THR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x080U))
#define UART1_RX_DATA_BUFFER_RHR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x100U))
#define UART1_TX_DMA_CURRENT_POINTER_CTXPTR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x200U))
#define UART1_TX_DMA_NEXT_POINTER_CTXCNT_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x204U))
#define UART1_TX_DMA_NEXT_BYTE_COUNT_NTXPTR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x208U))
#define UART1_TX_DMA_NEXT_BYTE_COUNT_NTXCNT_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x20CU))
#define UART1_RX_DMA_CURRENT_POINTER_CRXPTR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x210U))
#define UART1_RX_DMA_CURRENT_FREE_BYTE_COUNT_CRXCNT_REGISTER  	(*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x214U))
#define UART1_RX_DMA_NEXT_POINTER_NRXPTR_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x218U))
#define UART1_RX_DMA_NEXT_FREE_BYTE_COUNT_NRXCNT_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x21CU))
#define UART1_RX_DMA_RX_DATA_BYTE_COMPARE_RXCMP_REGISTER    (*(volatile uint32_t *)(UART1_CONTROL_BASE + 0x220U))    

//---------------------------------------- UART 2 -----------------------------------------//
#define UART2_CONTROL_CRS_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x000U))
#define UART2_CONTROL_CRC_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x004U))
#define UART2_CONTROL_CR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x008U))
#define UART2_INTERRUPT_ENABLE_IMRS_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x010U))
#define UART2_INTERRUPT_ENABLE_IMRC_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x014U))
#define UART2_INTERRUPT_ENABLE_IMR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x018U))
#define UART2_STATUS_SR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x020U))
#define UART2_BAUDRATE_BR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x024U))
#define UART2_TOR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x028U))
#define UART2_TX_FIFO_FLUSH_TFFR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x02CU))
#define UART2_RX_FIFO_FLUSH_RFFR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x030U))
#define UART2_TX_FIFO_DATA_WORDS_CURRENT_TWCR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x034U))
#define UART2_RX_FIFO_DATA_WORDS_CURRENT_RWCR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x038U))
#define UART2_DST_EXT_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x03CU))
#define UART2_AUTOBAUD_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x040U))
#define UART2_NCO_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x044U))
#define UART2_TX_DATA_BUFFER_THR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x080U))
#define UART2_RX_DATA_BUFFER_RHR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x100U))
#define UART2_TX_DMA_CURRENT_POINTER_CTXPTR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x200U))
#define UART2_TX_DMA_NEXT_POINTER_CTXCNT_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x204U))
#define UART2_TX_DMA_NEXT_BYTE_COUNT_NTXPTR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x208U))
#define UART2_TX_DMA_NEXT_BYTE_COUNT_NTXCNT_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x20CU))
#define UART2_RX_DMA_CURRENT_POINTER_CRXPTR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x210U))
#define UART2_RX_DMA_CURRENT_FREE_BYTE_COUNT_CRXCNT_REGISTER  	(*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x214U))
#define UART2_RX_DMA_NEXT_POINTER_NRXPTR_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x218U))
#define UART2_RX_DMA_NEXT_FREE_BYTE_COUNT_NRXCNT_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x21CU))
#define UART2_RX_DMA_RX_DATA_BYTE_COMPARE_RXCMP_REGISTER    (*(volatile uint32_t *)(UART2_CONTROL_BASE + 0x220U))    


/******************************************************************************/
/*                               SPI Control Interface                        */
/******************************************************************************/

//----------------------------------------- SPI 1 ----------------------------------------------//
#define SPI1_CONTROL_SET_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x000U))
#define SPI1_CONTROL_CLEAR_SET_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x004U))
#define SPI1_CONTROL_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x008U))        
#define SPI1_INTERRUPT_ENABLE_SET_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x010U))
#define SPI1_INTERRUPT_ENABLE_CLEAR_SET_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x014U))
#define SPI1_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x018U))       
#define SPI1_STATUS_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x020U))
#define SPI1_CLOCK_DIVIDER_RELOAD_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x024U))
#define SPI1_TIMEOUT_COUNTER_RELOAD_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x028U))
#define SPI1_TX_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x02CU))         
#define SPI1_RX_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x030U))
#define SPI1_TRANSMIT_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x034U))
#define SPI1_RECEIVE_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x038U))
#define SPI1_WAIT_SELECT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x03CU))
#define SPI1_WAIT_GAP_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x040U))
#define SPI1_CHIP_SELECT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x044U))
#define SPI1_COUNTER_BYTES_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0x048U))
#define SPI1_TRANSMIT_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X080U))
#define SPI1_RECEIVE_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X100U))
#define SPI1_TX_DMA_CURRENT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X200U))
#define SPI1_TX_DMA_CURRENT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X204U))
#define SPI1_TX_DMA_NEXT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X208U))
#define SPI1_TX_DMA_NEXT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X20CU))
#define SPI1_RX_DMA_CURRENT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X210U))
#define SPI1_RX_DMA_CURRENT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X214U))
#define SPI1_RX_DMA_NEXT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X218U))
#define SPI1_RX_DMA_NEXT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X21CU))
#define SPI1_RX_DMA_RX_DATA_BYTE_COMPARE_REGISTER    (*(volatile uint32_t *)(SPI1_CONTROL_BASE + 0X220U))

//----------------------------------------- SPI 2 ----------------------------------------------//
#define SPI2_CONTROL_SET_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x000U))
#define SPI2_CONTROL_CLEAR_SET_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x004U))
#define SPI2_CONTROL_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x008U))        
#define SPI2_INTERRUPT_ENABLE_SET_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x010U))
#define SPI2_INTERRUPT_ENABLE_CLEAR_SET_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x014U))
#define SPI2_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x018U))       
#define SPI2_STATUS_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x020U))
#define SPI2_CLOCK_DIVIDER_RELOAD_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x024U))
#define SPI2_TIMEOUT_COUNTER_RELOAD_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x028U))
#define SPI2_TX_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x02CU))         
#define SPI2_RX_FIFO_FLUSH_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x030U))
#define SPI2_TRANSMIT_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x034U))
#define SPI2_RECEIVE_FIFO_WORD_COUNT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x038U))
#define SPI2_WAIT_SELECT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x03CU))
#define SPI2_WAIT_GAP_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x040U))
#define SPI2_CHIP_SELECT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x044U))
#define SPI2_COUNTER_BYTES_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0x048U))
#define SPI2_TRANSMIT_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X080U))
#define SPI2_RECEIVE_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X100U))
#define SPI2_TX_DMA_CURRENT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X200U))
#define SPI2_TX_DMA_CURRENT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X204U))
#define SPI2_TX_DMA_NEXT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X208U))
#define SPI2_TX_DMA_NEXT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X20CU))
#define SPI2_RX_DMA_CURRENT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X210U))
#define SPI2_RX_DMA_CURRENT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X214U))
#define SPI2_RX_DMA_NEXT_POINTER_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X218U))
#define SPI2_RX_DMA_NEXT_BYTE_COUNT_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X21CU))
#define SPI2_RX_DMA_RX_DATA_BYTE_COMPARE_REGISTER    (*(volatile uint32_t *)(SPI2_CONTROL_BASE + 0X220U))


/******************************************************************************/
/*                                 I2S Interface                              */
/******************************************************************************/

#define I2S_CONTROL_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x000U))
#define I2S_FULL_DUPLEX_MODE_CONTROL_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x004U))
#define I2S_TRANSCEIVER_SAMPLE_RESOLUTION_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x008U))
#define I2S_FULL_DUPLEX_MODE_RECEIVE_SAMPLES_RESOLUTION_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x00CU))         
#define I2S_TRANSCEIVER_SAMPLE_RATE_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x010U))
#define I2S_STATUS_FLAGS_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x014U))
#define I2S_FIFO_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x018U))
#define I2S_FIFO_ALMOST_EMPTY_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x01CU))      
#define I2S_FIFO_ALMOST_FULL_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x020U))
#define I2S_FULL_DUPLEX_MODE_RECEIVER_FIFO_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x024U))
#define I2S_FULL_DUPLEX_MODE_RECEIVER_FIFO_ALMOST_EMPTY_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x028U))
#define I2S_FULL_DUPLEX_MODE_RECEIVER_FIFO_ALMOST_FULL_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x02CU))        
#define I2S_TIME_DIVISION_MULTIPLEXING_CONTROL_REGISTER    (*(volatile uint32_t *)(I2S_BASE + 0x030U))
#define I2S_TIME_DIVISION_MULTIPLEXING_FULL_DUPLEX_MODE_CHANNELS_DIRECTION_REGISTER   (*(volatile uint32_t *)(I2S_BASE + 0x034U))


/******************************************************************************/
/*                               DisplaY Interface                            */
/******************************************************************************/

#define DISPLAY_FIFO_WRITE_SETTING_CS0_AND_A0_0_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x000U))
#define DISPLAY_FIFO_WRITE_SETTING_CS0_AND_A0_1_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x004U))
#define DISPLAY_FIFO_WRITE_SETTING_CS1_AND_A0_0_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x008U))
#define DISPLAY_FIFO_WRITE_SETTING_CS1_AND_A0_1_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x00CU))          
#define DISPLAY_DISPLAY_CFG_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x010U))
#define DISPLAY_READ_CFG_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x014U))
#define DISPLAY_READ_DATA_VALID_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x018U))
#define DISPLAY_READ_DATA_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x01CU))          
#define DISPLAY_SERIAL_ENGINE_CFG_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x020U))
#define DISPLAY_START_DMA_WRITE_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x024U))
#define DISPLAY_DMA_CFG_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x028U))
#define DISPLAY_TOTAL_NUMBER_OF_PACKETS_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x02CU))          
#define DISPLAY_TIMING_CONTROL_REG1_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x030U))
#define DISPLAY_TIMING_CONTROL_REG2_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x034U))
#define DISPLAY_POLARITY_REG_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x038U))
#define DISPLAY_DISPLAY_STATUS_REG_REGISTER    (*(volatile uint32_t *)(DISPLAY_BASE + 0x03CU))


/******************************************************************************/
/*                                PWM Interface                               */
/******************************************************************************/

#define PWM_LOAD_SHADOW_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x000U))
#define PWM_COUNTER_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x004U))        
#define PWM_NCO_FREQUENCY_1_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x008U))
#define PWM_NCO_FREQUENCY_1_PHASE_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x00CU))
#define PWM_NCO_FREQUENCY_1_DUTY_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x010U))
#define PWM_NCO_FREQUENCY_1_OVERFLOW_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x014U))              
#define PWM_NCO_FREQUENCY_2_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x018U))
#define PWM_NCO_FREQUENCY_2_PHASE_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x01CU))
#define PWM_NCO_FREQUENCY_2_DUTY_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x020U))
#define PWM_NCO_FREQUENCY_2_OVERFLOW_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x024U))               
#define PWM_NCO_FREQUENCY_3_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x028U))
#define PWM_NCO_FREQUENCY_3_PHASE_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x02CU))
#define PWM_NCO_FREQUENCY_3_DUTY_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x030U))
#define PWM_NCO_FREQUENCY_3_OVERFLOW_REGISTER    (*(volatile uint32_t *)(PWM_BASE + 0x034U))
		

/******************************************************************************/
/*                            SCU Control Registers                           */
/******************************************************************************/

#define TCU0_CTRL_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x000U))
#define TCU0_SLOW_SLEEP_SET_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x008U))
#define TCU0_FAST_SLEEP_SET_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x00CU))
#define TCU0_SLOW_PWR_UP_DELAY_SET_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x010U))
#define TCU0_DEFINSTR_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x014U))
#define TCU0_ASSERT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x018U))
#define TCU0_ABORT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x01CU))
#define TCU0_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x100U))
#define TCU0_SLOW_SLEEP_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x104U))
#define TCU0_FAST_SLEEP_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x108U))
#define TCU0_CALIBRATION_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x10CU))
#define TCU0_SFNEMPTYWM_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x120U)) // 	SEQUENCE_FIFO_NEAR_EMPTY_WATERMARK
#define TCU0_SFNFULLWM_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x124U)) //	SEQUENCE_FIFO_NEAR_FULL_WATERMARK
#define TCU0_FAULT_CLEAR_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x300U))
#define TCU0_FAULT_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x304U))
#define TCU0_FAULT_RAW_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x308U))
#define TCU0_FAULT_MASK_CLR_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x30CU))
#define TCU0_FAULT_MASK_SET_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x310U))
#define TCU0_FAULT_MASK_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x314U))
#define TCU0_SET_EVENT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x400U))
#define TCU0_CLR_EVENT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x404U))
#define TCU0_PULSE_EVENT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x408U))
#define TCU0_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x40CU))
#define TCU0_EVENT_HALT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x410U))
#define TCU0_CONFIG_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x500U))

#define TCU1_CTRL_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x600U))
#define TCU1_ABORT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x61CU))
#define TCU1_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x700U))
#define TCU1_SFNEMPTYWM_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x720U)) // 	SEQUENCE_FIFO_NEAR_EMPTY_WATERMARK
#define TCU1_SFNFULLWM_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x724U)) //	SEQUENCE_FIFO_NEAR_FULL_WATERMARK
#define TCU1_FAULT_CLEAR_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x900U))
#define TCU1_FAULT_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x904U))
#define TCU1_FAULT_RAW_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x908U))
#define TCU1_FAULT_MASK_CLR_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x90CU))
#define TCU1_FAULT_MASK_SET_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x910U))
#define TCU1_FAULT_MASK_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0x914U))
#define TCU1_SET_EVENT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xA00U))
#define TCU1_CLR_EVENT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xA04U))
#define TCU1_PULSE_EVENT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xA08U))
#define TCU1_EVENT_STATUS_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xA0CU))
#define TCU1_EVENT_HALT_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xA10U))
#define TCU1_CONFIG_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xB00U))
#define TCU_DPLL_X_DIV_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xC00U))
#define TCU_DPLL_Y_DIV_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xC04U))
#define TCU_PERIPHERAL_ID4_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFD0U))
#define TCU_PERIPHERAL_ID5_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFD4U))
#define TCU_PERIPHERAL_ID6_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFD8U))
#define TCU_PERIPHERAL_ID7_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFDCU))
#define TCU_PERIPHERAL_ID0_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFE0U))
#define TCU_PERIPHERAL_ID1_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFE4U))
#define TCU_PERIPHERAL_ID2_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFE8U))
#define TCU_PERIPHERAL_ID3_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFECU))
#define TCU_COMPONENT_ID0_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFF0U))
#define TCU_COMPONENT_ID1_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFF4U))
#define TCU_COMPONENT_ID2_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFF8U))
#define TCU_COMPONENT_ID3_REGISTER    (*(volatile uint32_t *)(SCU_CONTROL_BASE + 0xFFCU))


/******************************************************************************/
/*                          SDMA PL081 Control Registers                      */
/******************************************************************************/

#define DMAC_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x000U))
#define DMAC_INTERRUPT_TERMINAL_COUNT_STATUS_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x004U))
#define DMAC_INTERRUPT_TERMINAL_COUNT_CLEAR_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x008U))
#define DMAC_INTERRUPT_ERROR_STATUS_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x00CU))
#define DMAC_INTERRUPT_ERROR_CLEAR_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x010U))
#define DMAC_INTERRUPT_TERMINAL_COUNT_RAW_STATUS_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x014U))
#define DMAC_INTERRUPT_ERROR_RAW_STATUS_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x018U))
#define DMAC_ENABLED_CHANNEL_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x01CU))
#define DMAC_SOFTWARE_BURST_REQUEST_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x020U))
#define DMAC_SOFTWARE_SINGLE_REQUEST_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x024U))
#define DMAC_SOFTWARE_LAST_BURST_REQUEST_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x028U))
#define DMAC_SOFTWARE_LAST_SINGLE_REQUEST_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x02CU))
#define DMAC_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x030U))
#define DMAC_SYNCHRONIZATION_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x034U))
#define DMAC_PL081_DMAC_CHANNEL_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x100U))
#define DMAC_INTEGRATION_TEST_CONTROL_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x500U))
#define DMAC_INTEGRATION_TEST_OUTPUT_1_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x504U))
#define DMAC_INTEGRATION_TEST_OUTPUT_2_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x508U))
#define DMAC_INTEGRATION_TEST_OUTPUT_3_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0x50CU))
#define DMAC_PERIPHERAL_ID0_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFE0U))
#define DMAC_PERIPHERAL_ID1_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFE4U))
#define DMAC_PERIPHERAL_ID2_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFE8U))
#define DMAC_PERIPHERAL_ID3_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFECU))
#define DMAC_COMPONENT_ID0_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFF0U))
#define DMAC_COMPONENT_ID1_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFF4U))
#define DMAC_COMPONENT_ID2_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFF8U))
#define DMAC_COMPONENT_ID3_REGISTER    (*(volatile uint32_t *)(SDMA_PL081_CONTROL_BASE + 0xFFCU))


/******************************************************************************/
/*                             USB Control Interface                          */
/******************************************************************************/

#define USB_EP0_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x000U))					       
#define USB_LPM_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x004U))                         
#define USB_EP1_OUT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x008U))                     
#define USB_EP1_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x00CU))                     
#define USB_EP2_OUT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x010U))                     
#define USB_EP2_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x014U))                     
#define USB_EP3_OUT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x018U))                     
#define USB_EP3_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x01CU))                      
#define USB_EP4_OUT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x020U))                   
#define USB_EP4_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x024U))                     
#define USB_EP5_OUT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x028U))                     
#define USB_EP5_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x02CU))                     
#define USB_EP6_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X034U))   
#define USB_EP7_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X03CU))
#define USB_EP8_IN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X044U))
#define USB_FIFO1_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X084U))	
#define USB_FIFO2_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X088U))
#define USB_FIFO3_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X08CU))
#define USB_FIFO4_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X090U))
#define USB_FIFO5_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X094U))
#define USB_FIFO6_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X098U))
#define USB_FIFO7_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X09CU))   
#define USB_FIFO8_DAT_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X0A0U))
#define USB_EP0_IN_DAT_1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X100U))
#define USB_EP0_IN_DAT_2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X104U))
#define USB_EP0_IN_DAT_3_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X108U))
#define USB_EP0_IN_DAT_4_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X10CU))
#define USB_EP0_IN_DAT_5_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X110U))
#define USB_EP0_IN_DAT_6_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X114U))
#define USB_EP0_IN_DAT_7_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X118U))
#define USB_EP0_IN_DAT_8_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X11CU))
#define USB_EP0_IN_DAT_9_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X120U))
#define USB_EP0_IN_DAT_10_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X124U))
#define USB_EP0_IN_DAT_11_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X128U))
#define USB_EP0_IN_DAT_12_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X12CU))
#define USB_EP0_IN_DAT_13_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X130U))
#define USB_EP0_IN_DAT_14_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X134U))
#define USB_EP0_IN_DAT_15_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X138U))
#define USB_EP0_IN_DAT_16_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X13CU))
#define USB_EP0_OUT_DAT_1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X140U))
#define USB_EP0_OUT_DAT_2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X144U))
#define USB_EP0_OUT_DAT_3_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X148U))
#define USB_EP0_OUT_DAT_4_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X14CU))
#define USB_EP0_OUT_DAT_5_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X150U))
#define USB_EP0_OUT_DAT_6_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X154U))
#define USB_EP0_OUT_DAT_7_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X158U))
#define USB_EP0_OUT_DAT_8_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X15CU))
#define USB_EP0_OUT_DAT_9_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X160U))
#define USB_EP0_OUT_DAT_10_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X164U))
#define USB_EP0_OUT_DAT_11_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X168U))
#define USB_EP0_OUT_DAT_12_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X16CU))
#define USB_EP0_OUT_DAT_13_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X170U))
#define USB_EP0_OUT_DAT_14_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0x004U))
#define USB_EP0_OUT_DAT_15_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X178U))
#define USB_EP0_OUT_DAT_16_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X17CU))
#define USB_SETUP_DAT_0_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X180U))
#define USB_SETUP_DAT_1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X184U))
#define USB_ENDPOINT_INTERRUPT_REQUEST_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X188U))
#define USB_USB_INTERRUPT_REQUEST_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X18CU))
#define USB_FIFO_INTERRUPT_REQUEST_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X190U))
#define USB_ENDPOINT_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X194U))
#define USB_USB_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X198U))
#define USB_FIFO_INTERRUPT_ENABLE_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X19CU))
#define USB_INTERRUPT_VECTOR_AND_RESET_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X1A0U)
#define USB_USB_CSR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X1A4U))
#define USB_FIFOCTRL_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X1A8U))
#define USB_OUTMAXPCK_1_0_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X1E0U))
#define USB_OUTMAXPCK_3_2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X1E4U))
#define USB_OUTMAXPCK_5_4_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X1E8U))
#define USB_OUT1_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X304U))
#define USB_OUT2_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X308U))
#define USB_OUT3_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X30CU))
#define USB_OUT4_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X310U))
#define USB_OUT5_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X314U))
#define USB_IN1_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X344U))
#define USB_IN2_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X348U))
#define USB_IN3_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X34CU))
#define USB_IN4_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X350U))
#define USB_IN5_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X354U))
#define USB_IN6_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X358U))
#define USB_IN7_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X35CU))
#define USB_IN8_STADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X360U))
#define USB_CPUCTRL2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3C4U))
#define USB_ENDIAN_SFR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3DCU))
#define USB_INMAXPCK_1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3E0U))
#define USB_INMAXPCK_3_2_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3E4U))
#define USB_INMAXPCK_5_4_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3E8U))
#define USB_INMAXPCK_7_6_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3ECU))
#define USB_INMAXPCK_9_8_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X3F0U))
#define USB_DMA_CONF_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X400U))
#define USB_DMA_STS_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X404U))
#define USB_DMA_EP_SEL_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X41CU))
#define USB_DMA_EP_TRADDR_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X420U))
#define USB_EP_CFG_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X424U))
#define USB_DMA_EP_CMD_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X428U))
#define USB_DMA_EP_STS_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X42CU))
#define USB_DMA_EP_STS_EN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X434U))
#define USB_DMA_DRBL_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X438U))
#define USB_DMA_EP_IEN_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X43CU))
#define USB_DMA_EP_ISTS_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X440U))
#define USB_DMA_AXIM_CTRL_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X444U))
#define USB_DMA_AXIM_ID_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X448U))
#define USB_DMA_AXIM_CAP_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X450U))
#define USB_DMA_AXIM_CTRL0_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X458U))
#define USB_DMA_AXIM_CTRL1_REGISTER    (*(volatile uint32_t *)(USB_CONTROL_BASE + 0X45CU))


/******************************************************************************/
/*                            PSRAM Control Interface                         */
/******************************************************************************/

#define PSRAM_AXI_ID_PRIORITY_SELECTION_MASK_0_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x000U))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_MASK_1_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x004U))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_MASK_2_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x008U))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_MASK_3_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x00CU))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_LEVEL_0_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x010U))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_LEVEL_1_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x014U))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_LEVEL_2_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x018U))
#define PSRAM_AXI_ID_PRIORITY_SELECTION_LEVEL_3_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x01CU))
#define PSRAM_AXI_CONTROL_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x020U))
#define PSRAM_SYNCHRONOUS_RESET_ABORT_REQUEST_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x024U))
#define PSRAM_AXI_BUSY_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x028U))
#define PSRAM_ADDRESS_WIDTH_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x02CU))
#define PSRAM_ROW_LENGTH_SPLITTING_CONFIGURE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x030U))
#define PSRAM_IRQ_AND_FAULT_STATUS_RAW_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x100U))
#define PSRAM_IRQ_AND_FAULT_STATUS_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x104U))
#define PSRAM_IRQ_AND_FAULT_STATUS_MASK_SET_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x108U))
#define PSRAM_IRQ_AND_FAULT_STATUS_MASK_CLR_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x10CU))
#define PSRAM_IRQ_AND_FAULT_STATUS_MASK_STATUS_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x110U))
#define PSRAM_IRQ_AND_FAULT_STATUS_CLR_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x114U))
#define PSRAM_FE_DEBUG_FSM_STATES_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x200U))
#define PSRAM_FE_DEBUG_COMMAND_ADDR_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x204U))
#define PSRAM_FE_DEBUG_COMMAND_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x208U))
#define PSRAM_FE_DEBUG_FIFO_STATUS_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x20CU))
#define PSRAM_FE_DEBUG_ARBITER_CNT_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x210U))
#define PSRAM_CONTROLLER_PHY_DEBUG_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x214U))
#define PSRAM_CONTROLLER_PHY_DEBUG_CNT_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x218U))
#define PSRAM_HW_OBSERVATION_CONTROL_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x21CU))
#define PSRAM_BUS_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x300U))
#define PSRAM_REFRESH_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x304U))
#define PSRAM_DEVICE_IDENTIFICATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x308U))
#define PSRAM_BCR_WRITE_REQUEST_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x30CU))
#define PSRAM_BCR_READ_REQUEST_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x310U))
#define PSRAM_RCR_WRITE_REQUEST_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x314U))
#define PSRAM_RCR_READ_REQUEST_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x318U))
#define PSRAM_DIDR_READ_REQUEST_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x31CU))
#define PSRAM_PHY_TX_DLL_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x400U))
#define PSRAM_PHY_RX_DLL_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x404U))
#define PSRAM_PHY_TX_DLL_MASTER_CONTROL_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x408U))
#define PSRAM_PHY_RX_DLL_MASTER_CONTROL_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x40CU))
#define PSRAM_PHY_TX_DLL_MASTER_OBSERVABLE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x410U))
#define PSRAM_PHY_TX_DLL_SLAVE_OBSERVABLE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x414U))
#define PSRAM_PHY_RX_DLL_MASTER_OBSERVABLE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x418U))
#define PSRAM_PHY_RX_DLL_SLAVE_OBSERVABLE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x41CU))
#define PSRAM_PHY_RX_READ_DATA_CAPTURE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x420U))
#define PSRAM_ASYNCHRONOUS_MODE_LATENCIES_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x424U))
#define PSRAM_DPD_EXIT_COUNTER_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x428U))
#define PSRAM_DPD_EXIT_CONTROL_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x42CU))
#define PSRAM_CONTROLLER_PHY_STATUS_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x430U))
#define PSRAM_CONTROLLER_PHY_IGNORE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x434U))
#define PSRAM_CONTROLLER_PHY_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x438U))
#define PSRAM_SYNCHRONOUS_MODE_LATENCY_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x43CU))
#define PSRAM_DFT_LOOPBACK_CONTROL_LOWER_BYTE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x500U))
#define PSRAM_DFT_LOOPBACK_CONTROL_UPPER_BYTE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x504U))
#define PSRAM_DFT_LOOPBACK_DATA_OBSERVABLE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x508U))
#define PSRAM_DFT_MEMORY_CONTROL_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x50CU))
#define PSRAM_DFT_MEMORY_COMMAND_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x510U))
#define PSRAM_DFT_MEMORY_WRITE_DATA_POSITIVE_EDGE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x514U))
#define PSRAM_DFT_MEMORY_WRITE_DATA_NEGATIVE_EDGE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x518U))
#define PSRAM_DFT_MEMORY_READ_DATA_POSITIVE_EDGE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x51CU))              
#define PSRAM_DFT_MEMORY_READ_DATA_NEGATIVE_EDGE_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x520U))         
#define PSRAM_DFT_READ_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(PSRAM_CONTROL_BASE + 0x524U))


/******************************************************************************/
/*                    ARM Cryptocell 310 Control Interface                    */
/******************************************************************************/

#define CRYPTO_MEMORY_MAP0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x000U))
#define CRYPTO_MEMORY_MAP1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x004U))
#define CRYPTO_MEMORY_MAP2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x008U))
#define CRYPTO_MEMORY_MAP3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x00CU))
#define CRYPTO_MEMORY_MAP4_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x010U))
#define CRYPTO_MEMORY_MAP5_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x014U))
#define CRYPTO_MEMORY_MAP6_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x018U))
#define CRYPTO_MEMORY_MAP7_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x01CU))
#define CRYPTO_MEMORY_MAP8_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x020U))
#define CRYPTO_MEMORY_MAP9_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x024U))
#define CRYPTO_MEMORY_MAP10_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x028U))
#define CRYPTO_MEMORY_MAP11_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x02CU))
#define CRYPTO_MEMORY_MAP12_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x030U))
#define CRYPTO_MEMORY_MAP13_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x034U))
#define CRYPTO_MEMORY_MAP14_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x038U))
#define CRYPTO_MEMORY_MAP15_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x03CU))
#define CRYPTO_MEMORY_MAP16_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x040U))
#define CRYPTO_MEMORY_MAP17_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x044U))
#define CRYPTO_MEMORY_MAP18_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x048U))
#define CRYPTO_MEMORY_MAP19_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x04CU))
#define CRYPTO_MEMORY_MAP20_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x050U))
#define CRYPTO_MEMORY_MAP21_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x054U))
#define CRYPTO_MEMORY_MAP22_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x058U))
#define CRYPTO_MEMORY_MAP23_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x05CU))
#define CRYPTO_MEMORY_MAP24_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x060U))
#define CRYPTO_MEMORY_MAP25_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x064U))
#define CRYPTO_MEMORY_MAP26_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x068U))
#define CRYPTO_MEMORY_MAP27_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x06CU))
#define CRYPTO_MEMORY_MAP28_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x070U))
#define CRYPTO_MEMORY_MAP29_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x074U))
#define CRYPTO_MEMORY_MAP30_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x078U))
#define CRYPTO_MEMORY_MAP31_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x07CU))
#define CRYPTO_OPCODE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x080U))
#define CRYPTO_N_NP_T0_T1_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x084U))
#define CRYPTO_PKA_STATUS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x088U))
#define CRYPTO_PKA_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x08CU))
#define CRYPTO_PKA_L0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x090U))
#define CRYPTO_PKA_L1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x094U))
#define CRYPTO_PKA_L2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x098U))
#define CRYPTO_PKA_L3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x09CU))
#define CRYPTO_PKA_L4_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0A0U))
#define CRYPTO_PKA_L5_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0A4U))
#define CRYPTO_PKA_L6_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0A8U))
#define CRYPTO_PKA_L7_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0ACU))
#define CRYPTO_PKA_PIPE_RDY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0B0U))
#define CRYPTO_PKA_DONE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0B4U))
#define CRYPTO_PKA_VERSION_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0C4U))
#define CRYPTO_PKA_MSB_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x00CU))
#define CRYPTO_PKA_MON_READ_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0D0U))
#define CRYPTO_PKA_SRAM_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0D4U))
#define CRYPTO_PKA_SRAM_WDATA_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0D8U))
#define CRYPTO_PKA_SRAM_RDATA_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0DCU))
#define CRYPTO_PKA_SRAM_WR_CLR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0E0U))
#define CRYPTO_PKA_SRAM_RADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0E4U))
#define CRYPTO_PKA_WORD_ACCESS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0F0U))
#define CRYPTO_PKA_BUFF_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x0F8U))
#define CRYPTO_RNG_IMR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x100U))
#define CRYPTO_RNG_ISR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x104U))
#define CRYPTO_RNG_ICR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x108U))
#define CRYPTO_RNG_TRNG_CONFIG_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x10CU))
#define CRYPTO_RNG_TRNG_VALID_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x110U))
#define CRYPTO_RNG_EHR_DATA_0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x114U))
#define CRYPTO_RNG_EHR_DATA_1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x118U))
#define CRYPTO_RNG_EHR_DATA_2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x11CU))
#define CRYPTO_RNG_EHR_DATA_3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x120U))
#define CRYPTO_RNG_EHR_DATA_4_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x124U))
#define CRYPTO_RNG_EHR_DATA_5_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x128U))
#define CRYPTO_RNG_RND_SOURCE_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x12CU))
#define CRYPTO_RNG_SAMPLE_CNT1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x130U))
#define CRYPTO_RNG_AUTOCORR_STATISTIC_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x134U))
#define CRYPTO_RNG_TRNG_DEBUG_CONTROL_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x138U))
#define CRYPTO_RNG_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x140U))
#define CRYPTO_RNG_DEBUG_EN_INPUT_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1B4U))
#define CRYPTO_RNG_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1B8U))
#define CRYPTO_RST_BITS_COUNTER_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1BCU))
#define CRYPTO_RNG_VERSION_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1C0U))
#define CRYPTO_RNG_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1C4U))
#define CRYPTO_RNG_DMA_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1C8U))
#define CRYPTO_RNG_DMA_SRC_MASK_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1CCU))
#define CRYPTO_RNG_DMA_SRAM_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1D0U))
#define CRYPTO_RNG_DMA_SAMPLES_NUM_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1D4U))
#define CRYPTO_RNG_WATCHDOG_VAL_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1D8U))
#define CRYPTO_RNG_DMA_STATUS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x1DCU))
#define CRYPTO_CHACHA_CONTROL_REG_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x380U))
#define CRYPTO_CHACHA_VERSION_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x384U))
#define CRYPTO_CHACHA_KEY0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x388U))
#define CRYPTO_CHACHA_KEY1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x38CU))
#define CRYPTO_CHACHA_KEY2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x390U))
#define CRYPTO_CHACHA_KEY3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x394U))
#define CRYPTO_CHACHA_KEY4_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x398U))
#define CRYPTO_CHACHA_KEY5_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x39CU))
#define CRYPTO_CHACHA_KEY6_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3A0U))
#define CRYPTO_CHACHA_KEY7_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3A4U))
#define CRYPTO_CHACHA_IV_0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3A8U))
#define CRYPTO_CHACHA_IV_1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3ACU))
#define CRYPTO_CHACHA_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3B0U))
#define CRYPTO_CHACHA_HW_FLAGS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3B4U))
#define CRYPTO_CHACHA_BLOCK_CNT_LSB_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3B8U))
#define CRYPTO_CHACHA_BLOCK_CNT_MSB_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3BCU))
#define CRYPTO_CHACHA_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3C0U))
#define CRYPTO_CHACHA_FOR_POLY_KEY0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3C4U))
#define CRYPTO_CHACHA_FOR_POLY_KEY1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3C8U))
#define CRYPTO_CHACHA_FOR_POLY_KEY2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3CCU))
#define CRYPTO_CHACHA_FOR_POLY_KEY3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3D0U))
#define CRYPTO_CHACHA_FOR_POLY_KEY4_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3D4U))
#define CRYPTO_CHACHA_FOR_POLY_KEY5_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3D8U))
#define CRYPTO_CHACHA_FOR_POLY_KEY6_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3DCU))
#define CRYPTO_CHACHA_FOR_POLY_KEY7_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3E0U))
#define CRYPTO_CHACHA_BYTE_WORD_ORDER_CNTL_REG_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3E4U))
#define CRYPTO_CHACHA_DEBUG_REG_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x3E8U))
#define CRYPTO_AES_KEY_0_0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x400U))
#define CRYPTO_AES_KEY_0_1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x404U))
#define CRYPTO_AES_KEY_0_2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x408U))
#define CRYPTO_AES_KEY_0_3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x40CU))
#define CRYPTO_AES_IV_0_0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x440U))
#define CRYPTO_AES_IV_0_1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x444U))
#define CRYPTO_AES_IV_0_2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x448U))
#define CRYPTO_AES_IV_0_3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x44CU))
#define CRYPTO_AES_CTR_0_0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x460U))
#define CRYPTO_AES_CTR_0_1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x464U))
#define CRYPTO_AES_CTR_0_2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x468U))
#define CRYPTO_AES_CTR_0_3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x46CU))
#define CRYPTO_AES_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x470U))
#define CRYPTO_AES_SK_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x478U))
#define CRYPTO_AES_CMAC_INIT_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x47CU))
#define CRYPTO_AES_REMAINING_BYTES_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x4BCU))
#define CRYPTO_AES_CONTROL_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x4C0U))
#define CRYPTO_AES_HW_FLAGS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x4C8U))
#define CRYPTO_AES_CTR_NO_INCREMENT_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x4D8U))
#define CRYPTO_AES_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x4F4U))
#define CRYPTO_AES_CMAC_SIZE0_KICK_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x524U))
#define CRYPTO_HASH_H0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x640U))
#define CRYPTO_HASH_H1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x644U))
#define CRYPTO_HASH_H2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x648U))
#define CRYPTO_HASH_H3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x64CU))
#define CRYPTO_HASH_H4_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x650U))
#define CRYPTO_HASH_H5_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x654U))
#define CRYPTO_HASH_H6_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x658U))
#define CRYPTO_HASH_H7_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x65CU))
#define CRYPTO_HASH_H8_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x660U))
#define CRYPTO_AUTO_HW_PADDING_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x684U))
#define CRYPTO_LOAD_INIT_STATE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x694U))
#define CRYPTO_HASH_VERSION_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7B0U))
#define CRYPTO_HASH_CONTROL_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7C0U))
#define CRYPTO_HASH_PAD_EN_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7C4U))
#define CRYPTO_HASH_PAD_CFG_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7C8U))
#define CRYPTO_HASH_CUR_LEN_0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7CCU))
#define CRYPTO_HASH_CUR_LEN_1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7D0U))
#define CRYPTO_HASH_PARAM_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7DCU))
#define CRYPTO_HASH_AES_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7E4U))
#define CRYPTO_HASH_ENDIANESS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x7E8U))
#define CRYPTO_AES_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x800U))
#define CRYPTO_DES_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x804U))
#define CRYPTO_HASH_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x808U))
#define CRYPTO_PKA_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x80CU))
#define CRYPTO_DMA_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x820U))
#define CRYPTO_CLK_STATUS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x824U))
#define CRYPTO_CHACHA_CLK_ENABLE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x858U))
#define CRYPTO_CRYPTO_CTL_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x900U))
#define CRYPTO_CRYPTO_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x910U))
#define CRYPTO_HASH_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x91CU))
#define CRYPTO_VERSION_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0x928U))
#define CRYPTO_HOST_RGF_IRR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA00U))
#define CRYPTO_HOST_RGF_IMR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA04U))
#define CRYPTO_HOST_RGF_ICR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA08U))
#define CRYPTO_HOST_RGF_ENDIAN_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA0CU))
#define CRYPTO_HOST_RGF_CC_SW_RST_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA20U))
#define CRYPTO_HOST_RGF_SIGNATURE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA24U))
#define CRYPTO_HOST_BOOT_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA28U))
#define CRYPTO_HOST_CRYPTOKEY_SEL_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA38U))
#define CRYPTO_HOST_IOT_KPRTL_LOCK_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA4CU))
#define CRYPTO_HOST_IOT_KDR0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA50U))
#define CRYPTO_HOST_IOT_KDR1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA54U))
#define CRYPTO_HOST_IOT_KDR2_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA58U))
#define CRYPTO_HOST_IOT_KDR3_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA5CU))
#define CRYPTO_HOST_IOT_LCS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xA60U))
#define CRYPTO_DIN_BUFFER_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC00U))
#define CRYPTO_DIN_MEM_DMA_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC20U))
#define CRYPTO_SRC_LLI_SRAM_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC24U))
#define CRYPTO_SRC_LLI_WORD0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC28U))
#define CRYPTO_SRC_LLI_WORD1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC2CU))
#define CRYPTO_SRAM_SRC_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC30U))
#define CRYPTO_DIN_SRAM_BYTES_LEN_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC34U))
#define CRYPTO_DIN_SRAM_DMA_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC38U))
#define CRYPTO_DIN_SRAM_ENDIANNESS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC3CU))
#define CRYPTO_DIN_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC44U))
#define CRYPTO_DIN_CPU_DATA_SIZE_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC48U))
#define CRYPTO_WRITE_ALIGN_LAST_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC4CU))
#define CRYPTO_FIFO_IN_EMPTY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC50U))
#define CRYPTO_DISABLE_OUTSTD_REQ_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC54U))
#define CRYPTO_DIN_FIFO_RST_PNTR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xC58U))
#define CRYPTO_DOUT_BUFFER_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD00U))
#define CRYPTO_DOUT_MEM_DMA_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD20U))
#define CRYPTO_DST_LLI_SRAM_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD24U))
#define CRYPTO_DST_LLI_WORD0_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD28U))
#define CRYPTO_DST_LLI_WORD1_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD2CU))
#define CRYPTO_SRAM_DEST_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD30U))
#define CRYPTO_DOUT_SRAM_BYTES_LEN_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD34U))
#define CRYPTO_DOUT_SRAM_DMA_BUSY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD38U))
#define CRYPTO_DOUT_SRAM_ENDIANNESS_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD3CU))
#define CRYPTO_READ_ALIGN_LAST_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD44U))
#define CRYPTO_DOUT_FIFO_EMPTY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD50U))
#define CRYPTO_DOUT_SW_RESET_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xD58U))
#define CRYPTO_SRAM_DATA_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xF00U))
#define CRYPTO_SRAM_ADDR_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xF04U))
#define CRYPTO_SRAM_DATA_READY_REGISTER    (*(volatile uint32_t *)(ARM_CRYPTOCELL_310_CONTROL_BASE + 0xF08U))


/******************************************************************************/
/*                            SDIO Control Interface                          */
/******************************************************************************/

#define SDIO_HOST_HRS00_GENERAL_INFORMATION_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x000U))
#define SDIO_HOST_HRS01_DEBOUNCE_SETTING_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x004U))
#define SDIO_HOST_HRS02_BUS_SETTING_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x008U))
#define SDIO_HOST_HRS03_AXI_ERROR_RESPONSES_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x00CU))
#define SDIO_HOST_HRS04_UHS_I_PHY_SETTINGS_THE_HRS04_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x010U))
#define SDIO_HOST_HRS05_UHS_I_PHY_BIST_CONTROL_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x014U))
#define SDIO_HOST_HRS06_EMMC_CONTROL_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x018U))
#define SDIO_HOST_HRS32_FSM_MONITOR_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x080U))
#define SDIO_HOST_HRS33_TUNE_STATUS_0_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x084U))
#define SDIO_HOST_HRS34_TUNE_STATUS_1_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x088U))
#define SDIO_HOST_HRS35_TUNE_DEBUG_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x08CU))
#define SDIO_HOST_HRS36_BOOT_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x090U))
#define SDIO_HOST_HRS37_READ_BLOCK_GAP_COEFFICIENT_INTERFACE_MODE_SELECT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x094U))
#define SDIO_HOST_HRS38_READ_BLOCK_GAP_COEFFICIENT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x098U))
#define SDIO_HOST_HRS59_GENERIC_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x0ECU))
#define SDIO_HOST_CRS63_HOST_CONTROLLER_VERSION_OR_SLOT_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x0FCU))
#define SDIO_HOST_SRS00_SDMA_SYSTEM_ADDRESS_OR_ARGUMENT_2_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x200U))
#define SDIO_HOST_SRS01_BLOCK_SIZE_OR_BLOCK_COUNT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x204U))
#define SDIO_HOST_SRS02_ARGUMENT_1_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x208U))
#define SDIO_HOST_SRS03_COMMAND_OR_TRANSFER_MODE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x20CU))			    			
#define SDIO_HOST_SRS04_STORE_CARD_RESPONSE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x210U)) 
#define SDIO_HOST_SRS05_STORE_CARD_RESPONSE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x214U)) 
#define SDIO_HOST_SRS06_STORE_CARD_RESPONSE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x218U)) 
#define SDIO_HOST_SRS07_STORE_CARD_RESPONSE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x21CU)) 			
#define SDIO_HOST_SRS08_DATA_BUFFER_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x220U))
#define SDIO_HOST_SRS09_PRESENT_STATE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x224U))
#define SDIO_HOST_SRS10_HOST_CONTROL_1_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x228U))
#define SDIO_HOST_SRS11_HOST_CONTROL_2_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x22CU))	 
#define SDIO_HOST_SRS12_ERROR_OR_NORMAL_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x230U))
#define SDIO_HOST_SRS13_ERROR_OR_NORMAL_STATUS_ENABLE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x234U))
#define SDIO_HOST_SRS14_ERROR_OR_NORMAL_SIGNAL_ENABLE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x238U))
#define SDIO_HOST_SRS15_HOST_CONTROL_2_OR_AUTO_CMD_ERROR_STATU_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x23CU))	
#define SDIO_HOST_SRS16_CAPABILITIES_1_HWINIT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x240U))
#define SDIO_HOST_SRS17_CAPABILITIES_2_HWINIT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x244U))
#define SDIO_HOST_SRS18_CAPABILITIES_3_HWINIT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x248U))
#define SDIO_HOST_SRS12_ERROR_SRS19_CAPABILITIES_4_HWINIT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x24CU))	
#define SDIO_HOST_SRS20_FORCE_EVENT_EACH_FIELD_OF_THIS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x250U))
#define SDIO_HOST_SRS21_ADMA_ERROR_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x254U))
#define SDIO_HOST_SRS22_ADMA_OR_SDMA_SYSTEM_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x258U))
#define SDIO_HOST_SRS23_ADMA_OR_SDMA_SYSTEM_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x25CU))	
#define SDIO_HOST_SRS24_PRESET_VALUE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x260U))
#define SDIO_HOST_SRS25_PRESET_VALUE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x264U))
#define SDIO_HOST_SRS26_PRESET_VALUE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x268U))
#define SDIO_HOST_SRS27_PRESET_VALUE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x26CU))	
#define SDIO_HOST_CQRS00_COMMAND_QUEUING_VERSION_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x400U))
#define SDIO_HOST_CQRS01_COMMAND_QUEUING_CAPABILITIES_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x404U))
#define SDIO_HOST_CQRS02_COMMAND_QUEUING_CONFIGURATION_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x408U))
#define SDIO_HOST_CQRS03_COMMAND_QUEUING_CONTROL_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x40CU))
#define SDIO_HOST_CQRS04_COMMAND_QUEUING_INTERRUPT_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x410U))
#define SDIO_HOST_CQRS05_COMMAND_QUEUING_INTERRUPT_STATUS_ENABLE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x414U))
#define SDIO_HOST_CQRS06_COMMAND_QUEUING_INTERRUPT_SIGNAL_ENABLE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x418U))
#define SDIO_HOST_CQRS07_INTERRUPT_COALESCING_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x41CU))
#define SDIO_HOST_CQRS08_COMMAND_QUEUING_TASK_DESCRIPTOR_LIST_BASE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x420U))
#define SDIO_HOST_CQRS09_COMMAND_QUEUING_TASK_DESCRIPTOR_LIST_BASE_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x424U))
#define SDIO_HOST_CQRS10_COMMAND_QUEUING_TASK_DOORBELL_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x428U))
#define SDIO_HOST_CQRS11_TASK_COMPLETE_NOTIFICATION_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x42CU))	
#define SDIO_HOST_CQRS12_DEVICE_QUEUE_STATUS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x430U))
#define SDIO_HOST_CQRS13_DEVICE_PENDING_TASKS_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x434U))
#define SDIO_HOST_CQRS14_TASK_CLEAR_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x438U))	
#define SDIO_HOST_CQRS16_SEND_STATUS_CONFIGURATION_1_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x440U))
#define SDIO_HOST_CQRS17_SEND_STATUS_CONFIGURATION_2_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x444U))
#define SDIO_HOST_CQRS18_COMMAND_RESPONSE_FOR_DIRECT_COMMAND_TASK_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x448U))
#define SDIO_HOST_CQRS20_RESPONSE_MODE_ERROR_MASK_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x450U))
#define SDIO_HOST_CQRS21_TASK_ERROR_INFORMATION_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x454U))
#define SDIO_HOST_CQRS22_COMMAND_RESPONSE_INDEX_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x458U))
#define SDIO_HOST_CQRS23_COMMAND_RESPONSE_ARGUMENT_REGISTER    (*(volatile uint32_t *)(SDIO_CONTROL_BASE + 0x45CU))


/******************************************************************************/
/*                              MAIN Interface GPV                            */
/******************************************************************************/
	
#define MAIN_GPV_REMAP_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0x000U))
#define MAIN_GPV_PERIPHERAL_ID4_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FD0U))
#define MAIN_GPV_PERIPHERAL_ID5_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FD4U))
#define MAIN_GPV_PERIPHERAL_ID6_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FD8U))
#define MAIN_GPV_PERIPHERAL_ID7_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FDCU))
#define MAIN_GPV_PERIPHERAL_ID0_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FE0U))
#define MAIN_GPV_PERIPHERAL_ID1_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FE4U))
#define MAIN_GPV_PERIPHERAL_ID2_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FE8U))
#define MAIN_GPV_PERIPHERAL_ID3_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FECU))
#define MAIN_GPV_COMPONENT_ID0_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FF0U))
#define MAIN_GPV_COMPONENT_ID1_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FF4U))
#define MAIN_GPV_COMPONENT_ID2_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FF8U))
#define MAIN_GPV_COMPONENT_ID3_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X1FFCU))
#define MAIN_GPV_XHB32_APPCPUS_IB_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X2008U))
#define MAIN_GPV_XHB32_APPCPUS_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X2108U))
#define MAIN_GPV_AHB32_SDMAS_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X4008U))
#define MAIN_GPV_AHB32_SDMAS_AHB_CNTL_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X4044U))
#define MAIN_GPV_AXI32_STMS_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X5008U))
#define MAIN_GPV_AXI64_ROMS_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X6008U))
#define MAIN_GPV_AHB32_USBS_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X7008U))
#define MAIN_GPV_AHB32_USBS_AHB_CNTL_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X7044U))
#define MAIN_GPV_XHB32_OSPIS_IB_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X8008U))
#define MAIN_GPV_XHB32_OSPIS_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X8108U))
#define MAIN_GPV_AXI64_PSRAMS_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X9008U))
#define MAIN_GPV_XHB32_PERIPH_RAMS_IB_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0XB008U))
#define MAIN_GPV_XHB32_PERIPH_RAMS_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0XB108U))
#define MAIN_GPV_AXI32_MODEMS_FN_MOD_ISS_BM_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0XC008U))
#define MAIN_GPV_AXI64_APPCPUM_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X42108U))
#define MAIN_GPV_AXI64_MODEMM_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X43108U))
#define MAIN_GPV_AHB32_USBM_FN_MOD_AHB_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X44028U))
#define MAIN_GPV_AHB32_USBM_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X44108U))
#define MAIN_GPV_AXI64_SDIOM_IB_FN_MOD2_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X45024U))
#define MAIN_GPV_AXI64_SDIOM_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X45108U))
#define MAIN_GPV_AXI32_DBGM_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X46108U))
#define MAIN_GPV_AHB32_APPCPUM_FN_MOD_AHB_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X47028U))
#define MAIN_GPV_AHB32_APPCPUM_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X47108U))
#define MAIN_GPV_AHB32_SDMAM_FN_MOD_AHB_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X49028U))
#define MAIN_GPV_AHB32_SDMAM_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0X49108U))
#define MAIN_GPV_AHB32_CIPHM_FN_MOD_AHB_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0x4A028U))
#define MAIN_GPV_AHB32_CIPHM_IB_FN_MOD_REGISTER    (*(volatile uint32_t *)(MAIN_INTERCONNECT_GPV_BASE + 0x4A108U))

//________________________________________________________________________________________________________________________________//

/*@}*/ /* end of group KM_Definitions */

#ifdef __cplusplus
}
#endif

#endif  /* KM_APP_H */
