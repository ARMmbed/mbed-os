/*
 * Copyright (c) 2018 ARM Limited
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
#define ARM_CMSDK_TIMER0
#define ARM_CMSDK_TIMER1

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
#define DEFAULT_UART_BAUDRATE  9600
#define ARM_UART0
#define ARM_UART1
#define ARM_UART2
#define ARM_UART3
#define ARM_UART4

#endif  /* __ARM_LTD_DEVICE_CFG_H__ */
