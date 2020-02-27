/*
 * Copyright (c) 2019 Arm Limited
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

#ifndef __PLATFORM_IRQ_H__
#define __PLATFORM_IRQ_H__

/* ========================================================================== */
/* =============         Interrupt Number Definition         ================ */
/* ========================================================================== */

typedef enum IRQn
{
/* ====================  Core Specific Interrupt Numbers  =================== */
  NonMaskableInt_IRQn         = -14,  /*  -14 Non Maskable Interrupt          */
  HardFault_IRQn              = -13,  /*  -13 HardFault Interrupt             */
  MemoryManagement_IRQn       = -12,  /*  -12 Memory Management Interrupt     */
  BusFault_IRQn               = -11,  /*  -11 Bus Fault Interrupt             */
  UsageFault_IRQn             = -10,  /*  -10 Usage Fault Interrupt           */
  SecureFault_IRQn            =  -9,  /*  -9 Secure Fault Interrupt           */
  SVCall_IRQn                 =  -5,  /*  -5 SV Call Interrupt                */
  DebugMonitor_IRQn           =  -4,  /*  -4 Debug Monitor Interrupt          */
  PendSV_IRQn                 =  -2,  /*  -2 Pend SV Interrupt                */
  SysTick_IRQn                =  -1,  /*  -1 System Tick Interrupt            */
/* ====================  SSE-200 Interrupt Numbers  ========================= */
  NS_WATCHDOG_RESET_IRQn      = 0,   /* NonSecure Wdog Rst Request Interrupt  */
  NS_WATCHDOG_IRQn            = 1,   /* Non-Secure Watchdog Interrupt         */
  S32K_TIMER_IRQn             = 2,   /* S32K Timer Interrupt                  */
  TIMER0_IRQn                 = 3,   /* CMSDK Timer 0 Interrupt               */
  TIMER1_IRQn                 = 4,   /* CMSDK Timer 1 Interrupt               */
  DUALTIMER_IRQn              = 5,   /* CMSDK Dual Timer Interrupt            */
  MHU0_IRQn                   = 6,   /* Message Handling Unit 0 Interrupt     */
  MHU1_IRQn                   = 7,   /* Message Handling Unit 1 Interrupt     */
  /* Reserved                 = 8,      Reserved                              */
  S_MPC_COMBINED_IRQn         = 9,   /* Secure Combined MPC Interrupt         */
  S_PPC_COMBINED_IRQn         = 10,  /* Secure Combined PPC Interrupt         */
  S_MSC_COMBINED_IRQn         = 11,  /* Secure Combined MSC Interrupt         */
  S_BRIDGE_ERR_IRQn           = 12,  /* Secure Bridge Error Combined Interrupt*/
  I_CACHE_INV_ERR_IRQn        = 13,  /* ICache Invalidation Interrupt         */
  /* Reserved                 = 14,     Reserved                              */
  SYS_PPU_IRQn                = 15,  /* System PPU Interrupt                  */
  CPU0_PPU_IRQn               = 16,  /* CPU0 PPU Interrupt                    */
  CPU1_PPU_IRQn               = 17,  /* CPU1 PPU Interrupt                    */
  CPU0_DGB_PPU_IRQn           = 18,  /* CPU0 Debug PPU Interrupt              */
  CPU1_DGB_PPU_IRQn           = 19,  /* CPU1 Debug PPU Interrupt              */
  /* Reserved                 = 20,     Reserved                              */
  /* Reserved                 = 21,     Reserved                              */
  RAM0_PPU_IRQn               = 22,  /* RAM 0 PPU Interrupt                   */
  RAM1_PPU_IRQn               = 23,  /* RAM 1 PPU Interrupt                   */
  RAM2_PPU_IRQn               = 24,  /* RAM 2 PPU Interrupt                   */
  RAM3_PPU_IRQn               = 25,  /* RAM 3 PPU Interrupt                   */
  DEBUG_PPU_IRQn              = 26,  /* Debug PPU Interrupt                   */
  /* Reserved                 = 27,     Reserved                              */
  CPUn_CTI_IRQ0               = 28,  /* CPUn CTI Interrupt 0                  */
  CPUn_CTI_IRQ1               = 29,  /* CPUn CTI Interrupt 1                  */
  /* Reserved                 = 30,     Reserved                              */
  /* Reserved                 = 31,     Reserved                              */
/* =============  Musca B1 Specific Expansion Interrupt Numbers ============= */
  /* Reserved                 = 32,     Reserved                              */
  GpTimer_IRQn                = 33,  /* General Purpose Timer Interrupt       */
  I2C0_IRQn                   = 34,  /* I2C0 Interrupt                        */
  I2C1_IRQn                   = 35,  /* I2C1 Interrupt                        */
  I2S_IRQn                    = 36,  /* I2S Interrupt                         */
  SPI_IRQn                    = 37,  /* SPI Interrupt                         */
  QSPI_IRQn                   = 38,  /* QSPI Interrupt                        */
  UART0_Rx_IRQn               = 39,  /* UART0 receive FIFO interrupt          */
  UART0_Tx_IRQn               = 40,  /* UART0 transmit FIFO interrupt         */
  UART0_RxTimeout_IRQn        = 41,  /* UART0 receive timeout interrupt       */
  UART0_ModemStatus_IRQn      = 42,  /* UART0 modem status interrupt          */
  UART0_Error_IRQn            = 43,  /* UART0 error interrupt                 */
  UART0_IRQn                  = 44,  /* UART0 interrupt                       */
  UART1_Rx_IRQn               = 45,  /* UART1 receive FIFO interrupt          */
  UART1_Tx_IRQn               = 46,  /* UART1 transmit FIFO interrupt         */
  UART1_RxTimeout_IRQn        = 47,  /* UART1 receive timeout interrupt       */
  UART1_ModemStatus_IRQn      = 48,  /* UART1 modem status interrupt          */
  UART1_Error_IRQn            = 49,  /* UART1 error interrupt                 */
  UART1_IRQn                  = 50,  /* UART1 interrupt                       */
  GPIO_0_IRQn                 = 51,  /* GPIO 0 interrupt                      */
  GPIO_1_IRQn                 = 52,  /* GPIO 1 interrupt                      */
  GPIO_2_IRQn                 = 53,  /* GPIO 2 interrupt                      */
  GPIO_3_IRQn                 = 54,  /* GPIO 3 interrupt                      */
  GPIO_4_IRQn                 = 55,  /* GPIO 4 interrupt                      */
  GPIO_5_IRQn                 = 56,  /* GPIO 5 interrupt                      */
  GPIO_6_IRQn                 = 57,  /* GPIO 6 interrupt                      */
  GPIO_7_IRQn                 = 58,  /* GPIO 7 interrupt                      */
  GPIO_8_IRQn                 = 59,  /* GPIO 8 interrupt                      */
  GPIO_9_IRQn                 = 60,  /* GPIO 9 interrupt                      */
  GPIO_10_IRQn                = 61,  /* GPIO 10 interrupt                     */
  GPIO_11_IRQn                = 62,  /* GPIO 11 interrupt                     */
  GPIO_12_IRQn                = 63,  /* GPIO 12 interrupt                     */
  GPIO_13_IRQn                = 64,  /* GPIO 13 interrupt                     */
  GPIO_14_IRQn                = 65,  /* GPIO 14 interrupt                     */
  GPIO_15_IRQn                = 66,  /* GPIO 15 interrupt                     */
  GPIO_Combined_IRQn          = 67,  /* GPIO Combined interrupt               */
  PVT_IRQn                    = 68,  /* PVT sensor interrupt                  */
  /* Reserved                 = 69,     Reserved                              */
  PWM_0_IRQn                  = 70,  /* PWM0 interrupt                        */
  RTC_IRQn                    = 71,  /* RTC interrupt                         */
  GpTimer1_IRQn               = 72,  /* General Purpose Timer1 Interrupt      */
  GpTimer0_IRQn               = 73,  /* General Purpose Timer0 Interrupt      */
  PWM_1_IRQn                  = 74,  /* PWM1 interrupt                        */
  PWM_2_IRQn                  = 75,  /* PWM2 interrupt                        */
  GPIO_Combined_NS_IRQn       = 76,  /* GPIO Combined Non-Secure interrupt    */
  SDIO_IRQn                   = 77,  /* SDIO interrupt                        */
  /* Reserved                 = 83:78   Reserved                              */
  CryptoSS_Reset_Status_IRQn  = 84,  /* Crypto SS reset status                */
  HostMHUS0_Int_Acc_NR2R_IRQn = 85,  /* MHU0 Sender IRQ not-ready to ready    */
  HostMHUS0_Int_Acc_R2NR_IRQn = 86,  /* MHU0 Sender IRQ ready to not ready    */
  HostMHUR0_IRQ_Reg0_IRQn     = 87,  /* MHU0 Receiver IRQ Register 0          */
  HostMHUR0_IRQ_Reg1_IRQn     = 88,  /* MHU0 Receiver IRQ Register 1          */
  HostMHUR0_IRQComb_IRQn      = 89,  /* MHU0 Receiver IRQ combined            */
  HostMHUS1_Int_Acc_NR2R_IRQn = 90,  /* MHU1 Sender IRQ not-ready to ready    */
  HostMHUS1_Int_Acc_R2NR_IRQn = 91,  /* MHU1 Sender IRQ ready to not ready    */
  HostMHUR1_IRQ_Reg0_IRQn     = 92,  /* MHU1 Receiver IRQ Register 0          */
  HostMHUR1_IRQ_Reg1_IRQn     = 93,  /* MHU1 Receiver IRQ Register 1          */
  HostMHUR1_IRQComb_IRQn      = 94,  /* MHU1 Receiver IRQ combined            */
  EFlash0_Controller_IRQn     = 95,  /* GFC-100 EFlash 0 controller interrupt */
  EFlash1_Controller_IRQn     = 96,  /* GFC-100 EFlash 1 controller interrupt */
  /* Reserved                 = 127:97  Reserved                              */
} IRQn_Type;

#endif  /* __PLATFORM_IRQ_H__ */
