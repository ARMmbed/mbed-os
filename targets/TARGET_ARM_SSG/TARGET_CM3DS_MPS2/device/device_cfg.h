/*
 * Copyright (c) 2018 Arm Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ARM_LTD_DEVICE_CFG_H__
#define __ARM_LTD_DEVICE_CFG_H__

/**
 * \file device_cfg.h
 * \brief
 * This is the default device configuration file with all peripherals
 * defined and configured to be used via the non-secure base address.
 * This file is an example of how to define your own configuration file
 * with the peripherals required for your application.
 */

/* CMSDK Timers */
#define ARM_CMSDK_TIMER1
#define ARM_CMSDK_DUALTIMER

/* Timer Peripherals are driven by APB System Core Clocks,
 * defined in system_CMSDK_CM3DS.c
 */
#define TIMERS_INPUT_CLOCK_FREQ_HZ       25000000U

/* mbed usec high-resolution ticker configuration */
#define USEC_TIMER_DEV                   CMSDK_TIMER1_DEV

#define usec_interval_irq_handler        TIMER1_IRQHandler
#define USEC_INTERVAL_IRQ                TIMER1_IRQn

/* The us ticker uses CMSDK Timer1, that does not have HW prescaler.
 * The reported shift define is necessary for the software emulated
 * prescaler behavior, so the ticker works as if it was ticking on a
 * virtually slower frequency. The value 5 sets up the ticker to work
 * properly in the specified frequency interval.
 */
#define USEC_TIMER_BIT_WIDTH    32U
#define USEC_REPORTED_SHIFT     5U
#define USEC_REPORTED_FREQ_HZ   (TIMERS_INPUT_CLOCK_FREQ_HZ >> \
                                 USEC_REPORTED_SHIFT)
#define USEC_REPORTED_BITS      (USEC_TIMER_BIT_WIDTH - USEC_REPORTED_SHIFT)

/* mbed low power ticker configuration */
#define LP_TIMER_DEV                     CMSDK_DUALTIMER_DEV

#define lp_interval_irq_handler          DUALTIMER_IRQHandler
#define LP_INTERVAL_IRQ                  DUALTIMER_IRQn

/* The lp ticker a CMSDK Dual Timer that is capable of prescaling
 * its input clock frequency by 256 at most. Having 25MHz as input
 * frequency requires an additional slowing factor in order for the ticker
 * to operate in the specified frequency interval, thus the effective
 * prescaler value is going to be the sum of the HW and the virtual
 * prescaler values.
 */
#define LP_TIMER_BIT_WIDTH            32U
#define LP_TIMER_HW_PRESCALER         8U
#define LP_REPORTED_SHIFT             1U
#define LP_REPORTED_FREQ_HZ           (TIMERS_INPUT_CLOCK_FREQ_HZ >> \
                                      (LP_TIMER_HW_PRESCALER+LP_REPORTED_SHIFT))
#define LP_REPORTED_BITS              (LP_TIMER_BIT_WIDTH - LP_REPORTED_SHIFT)

/* RTC PL031 */
#define RTC_PL031

/* ARM GPIO */
#define ARM_GPIO0
#define ARM_GPIO1
#define ARM_GPIO2
#define ARM_GPIO3

/* ARM MPS2 IO FPGAIO */
#define ARM_MPS2_IO_FPGAIO

/* ARM MPS2 IO SCC */
#define ARM_MPS2_IO_SCC

/* ARM SPI PL022 */
#define DEFAULT_SPI_SPEED_HZ  4000000U /* 4MHz */
#define ARM_SPI0
#define ARM_SPI1
#define ARM_SPI2
#define ARM_SPI3
#define ARM_SPI4

/* ARM UART */
#define DEFAULT_UART_BAUDRATE  9600U
#define ARM_UART0
#define ARM_UART1
#define ARM_UART2
#define ARM_UART3
#define ARM_UART4

/* SMSC9220 Ethernet */
#ifdef COMPONENT_SMSC9220
#define SMSC9220_ETH
#define SMSC9220_Ethernet_Interrupt_Handler     ETHERNET_IRQHandler
#endif

#endif  /* __ARM_LTD_DEVICE_CFG_H__ */
