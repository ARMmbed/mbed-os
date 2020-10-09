/*
 * Copyright (c) 2017-2020 Arm Limited. All rights reserved.
 *
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
 */

/**
 * \file device_definition.h
 * \brief The structure definitions in this file are exported based
 * on the peripheral definitions from device_cfg.h.
 * This file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#ifndef __DEVICE_DEFINITION_H__
#define __DEVICE_DEFINITION_H__

#include "device_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ======= Peripheral configuration structure declarations ======= */

/* ARM SCC driver structures */
#ifdef MUSCA_S1_SCC_S
#include "musca_s1_scc_drv.h"
extern struct musca_s1_scc_dev_t MUSCA_S1_SCC_DEV_S;
#endif
#ifdef MUSCA_S1_SCC_NS
#include "musca_s1_scc_drv.h"
extern struct musca_s1_scc_dev_t MUSCA_S1_SCC_DEV_NS;
#endif

/* ARM GPIO driver structures */
#ifdef GPIO0_CMSDK_S
#include "gpio_cmsdk_drv.h"
extern struct gpio_cmsdk_dev_t GPIO0_CMSDK_DEV_S;
#endif
#ifdef GPIO0_CMSDK_NS
#include "gpio_cmsdk_drv.h"
extern struct gpio_cmsdk_dev_t GPIO0_CMSDK_DEV_NS;
#endif

/* ARM MPC SIE 200 driver structures */
#ifdef MPC_ISRAM0_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_ISRAM0_DEV_S;
#endif
#ifdef MPC_ISRAM1_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_ISRAM1_DEV_S;
#endif
#ifdef MPC_ISRAM2_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_ISRAM2_DEV_S;
#endif
#ifdef MPC_ISRAM3_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_ISRAM3_DEV_S;
#endif
#ifdef MPC_CODE_SRAM_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_CODE_SRAM_DEV_S;
#endif
#ifdef MPC_CODE_SRAM_NS
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_CODE_SRAM_DEV_NS;
#endif
#ifdef MPC_QSPI_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_QSPI_DEV_S;
#endif
#ifdef MPC_QSPI_NS
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_QSPI_DEV_NS;
#endif
#ifdef MPC_MRAM_S
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_MRAM_DEV_S;
#endif
#ifdef MPC_MRAM_NS
#include "mpc_sie_drv.h"
extern struct mpc_sie_dev_t MPC_MRAM_DEV_NS;
#endif

/* ARM MHU driver structures */
#ifdef ARM_MHU0_S
#include "mhu_sse_200_drv.h"
extern struct arm_mhu_sse_200_dev_t ARM_MHU0_DEV_S;
#endif
#ifdef ARM_MHU0_NS
#include "mhu_sse_200_drv.h"
extern struct arm_mhu_sse_200_dev_t ARM_MHU0_DEV_NS;
#endif
#ifdef ARM_MHU1_S
#include "mhu_sse_200_drv.h"
extern struct arm_mhu_sse_200_dev_t ARM_MHU1_DEV_S;
#endif
#ifdef ARM_MHU1_NS
#include "mhu_sse_200_drv.h"
extern struct arm_mhu_sse_200_dev_t ARM_MHU1_DEV_NS;
#endif

/* ARM UART PL011 driver structures */
#ifdef UART0_PL011_S
#include "uart_pl011_drv.h"
extern struct uart_pl011_dev_t UART0_PL011_DEV_S;
#endif
#ifdef UART0_PL011_NS
#include "uart_pl011_drv.h"
extern struct uart_pl011_dev_t UART0_PL011_DEV_NS;
#endif
#ifdef UART1_PL011_S
#include "uart_pl011_drv.h"
extern struct uart_pl011_dev_t UART1_PL011_DEV_S;
#endif
#ifdef UART1_PL011_NS
#include "uart_pl011_drv.h"
extern struct uart_pl011_dev_t UART1_PL011_DEV_NS;
#endif

/* SSE-200 Cache driver structure */
#ifdef SSE_200_CACHE_S
#include "cache_drv.h"
extern struct arm_cache_dev_t SSE_200_CACHE_DEV_S;
#endif
#ifdef SSE_200_CACHE_NS
#include "cache_drv.h"
extern struct arm_cache_dev_t SSE_200_CACHE_DEV_NS;
#endif

/* I2C IP6510 driver structures */
#ifdef I2C0_IP6510_S
#include "i2c_ip6510_drv.h"
extern struct i2c_ip6510_dev_t I2C0_IP6510_DEV_S;
#endif
#ifdef I2C0_IP6510_NS
#include "i2c_ip6510_drv.h"
extern struct i2c_ip6510_dev_t I2C0_IP6510_DEV_NS;
#endif
#ifdef I2C1_IP6510_S
#include "i2c_ip6510_drv.h"
extern struct i2c_ip6510_dev_t I2C1_IP6510_DEV_S;
#endif
#ifdef I2C1_IP6510_NS
#include "i2c_ip6510_drv.h"
extern struct i2c_ip6510_dev_t I2C1_IP6510_DEV_NS;
#endif

/* CMSDK Dualtimer driver structures */
#ifdef CMSDK_DUALTIMER_S
#include "dualtimer_cmsdk_drv.h"
extern struct dualtimer_cmsdk_dev_t CMSDK_DUALTIMER_DEV_S;
#endif
#ifdef CMSDK_DUALTIMER_NS
#include "dualtimer_cmsdk_drv.h"
extern struct dualtimer_cmsdk_dev_t CMSDK_DUALTIMER_DEV_NS;
#endif

/* CMSDK Timer driver structures */
#ifdef CMSDK_TIMER0_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_S;
#endif
#ifdef CMSDK_TIMER0_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_NS;
#endif

#ifdef CMSDK_TIMER1_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_S;
#endif
#ifdef CMSDK_TIMER1_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_NS;
#endif

/* General-Purpose Timer driver structures */
#ifdef GP_TIMER_S
#include "timer_gp_drv.h"
extern struct timer_gp_dev_t GP_TIMER_DEV_S;
#endif
#ifdef GP_TIMER_NS
#include "timer_gp_drv.h"
extern struct timer_gp_dev_t GP_TIMER_DEV_NS;
#endif

/* RTC PL031 */
#ifdef RTC_PL031_S
#include "rtc_pl031_drv.h"
extern struct rtc_pl031_dev_t RTC_PL031_DEV_S;
#endif

#ifdef RTC_PL031_NS
#include "rtc_pl031_drv.h"
extern struct rtc_pl031_dev_t RTC_PL031_DEV_NS;
#endif

/* Cadence SPI IP6524 driver structures */
#ifdef SPI0_IP6524_S
#include "spi_ip6524_drv.h"
extern struct spi_ip6524_dev_t SPI0_DEV_S;
#endif
#ifdef SPI0_IP6524_NS
#include "spi_ip6524_drv.h"
extern struct spi_ip6524_dev_t SPI0_DEV_NS;
#endif

/* QSPI Flash Controller driver structures  */
#ifdef QSPI_IP6514E_S
#include "qspi_ip6514e_drv.h"
extern struct qspi_ip6514e_dev_t QSPI_DEV_S;
#endif

#ifdef QSPI_IP6514E_NS
#include "qspi_ip6514e_drv.h"
extern struct qspi_ip6514e_dev_t QSPI_DEV_NS;
#endif

/* ARM PPC driver structures */
#ifdef AHB_PPC0_S
#include "ppc_sse200_drv.h"
extern struct ppc_sse200_dev_t AHB_PPC0_DEV_S;
#endif

#ifdef AHB_PPCEXP0_S
#include "ppc_sse200_drv.h"
extern struct ppc_sse200_dev_t AHB_PPCEXP0_DEV_S;
#endif

#ifdef APB_PPC0_S
#include "ppc_sse200_drv.h"
extern struct ppc_sse200_dev_t APB_PPC0_DEV_S;
#endif

#ifdef APB_PPC1_S
#include "ppc_sse200_drv.h"
extern struct ppc_sse200_dev_t APB_PPC1_DEV_S;
#endif

#ifdef APB_PPCEXP0_S
#include "ppc_sse200_drv.h"
extern struct ppc_sse200_dev_t APB_PPCEXP0_DEV_S;
#endif

#ifdef APB_PPCEXP1_S
#include "ppc_sse200_drv.h"
extern struct ppc_sse200_dev_t APB_PPCEXP1_DEV_S;
#endif

/* ======= External peripheral configuration structure declarations ======= */

/* MT25QL Flash memory library structures */
#if (defined(MT25QL_S) && defined(QSPI_IP6514E_S))
#include "mt25ql_flash_lib.h"
extern struct mt25ql_dev_t MT25QL_DEV_S;
#endif
#if (defined(MT25QL_NS) && defined(QSPI_IP6514E_NS))
#include "mt25ql_flash_lib.h"
extern struct mt25ql_dev_t MT25QL_DEV_NS;
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __DEVICE_DEFINITION_H__ */
