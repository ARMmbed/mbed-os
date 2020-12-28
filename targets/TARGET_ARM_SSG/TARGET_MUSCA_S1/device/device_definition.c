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
 * \file device_definition.c
 * \brief This file defines exports the structures based on the peripheral
 * definitions from device_cfg.h.
 * This file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#include "device_cfg.h"
#include "device_definition.h"
#include "platform_base_address.h"

/* ======= Peripheral configuration structure definitions ======= */
/* MUSCA S1 SCC driver structures */
#ifdef MUSCA_S1_SCC_S
static const struct musca_s1_scc_dev_cfg_t MUSCA_S1_SCC_DEV_CFG_S = {
    .base = MUSCA_S1_SCC_S_BASE};
struct musca_s1_scc_dev_t MUSCA_S1_SCC_DEV_S = {&(MUSCA_S1_SCC_DEV_CFG_S)};
#endif
#ifdef MUSCA_S1_SCC_NS
static const struct musca_s1_scc_dev_cfg_t MUSCA_S1_SCC_DEV_CFG_NS = {
    .base = MUSCA_S1_SCC_NS_BASE};
struct musca_s1_scc_dev_t MUSCA_S1_SCC_DEV_NS = {&(MUSCA_S1_SCC_DEV_CFG_NS)};
#endif

/* CMSDK GPIO driver structures */
#ifdef GPIO0_CMSDK_S
static const struct gpio_cmsdk_dev_cfg_t GPIO0_CMSDK_DEV_CFG_S = {
    .base = MUSCA_S1_GPIO_S_BASE};
struct gpio_cmsdk_dev_t GPIO0_CMSDK_DEV_S = {&(GPIO0_CMSDK_DEV_CFG_S)};
#endif
#ifdef GPIO0_CMSDK_NS
static const struct gpio_cmsdk_dev_cfg_t GPIO0_CMSDK_DEV_CFG_NS = {
    .base = MUSCA_S1_GPIO_NS_BASE};
struct gpio_cmsdk_dev_t GPIO0_CMSDK_DEV_NS = {&(GPIO0_CMSDK_DEV_CFG_NS)};
#endif

/* ARM PPC SSE 200 driver structures */
#ifdef AHB_PPC0_S
static struct ppc_sse200_dev_cfg_t AHB_PPC0_DEV_CFG_S = {
    .spctrl_base  = MUSCA_S1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_S1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t AHB_PPC0_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t AHB_PPC0_DEV_S = {
    &AHB_PPC0_DEV_CFG_S, &AHB_PPC0_DEV_DATA_S };
#endif

#ifdef AHB_PPCEXP0_S
static struct ppc_sse200_dev_cfg_t AHB_PPCEXP0_DEV_CFG_S = {
    .spctrl_base  = MUSCA_S1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_S1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t AHB_PPCEXP0_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t AHB_PPCEXP0_DEV_S = {
    &AHB_PPCEXP0_DEV_CFG_S, &AHB_PPCEXP0_DEV_DATA_S };
#endif

#ifdef APB_PPC0_S
static struct ppc_sse200_dev_cfg_t APB_PPC0_DEV_CFG_S = {
    .spctrl_base  = MUSCA_S1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_S1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPC0_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPC0_DEV_S = {
    &APB_PPC0_DEV_CFG_S, &APB_PPC0_DEV_DATA_S };
#endif

#ifdef APB_PPC1_S
static struct ppc_sse200_dev_cfg_t APB_PPC1_DEV_CFG_S = {
    .spctrl_base  = MUSCA_S1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_S1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPC1_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPC1_DEV_S = {
    &APB_PPC1_DEV_CFG_S, &APB_PPC1_DEV_DATA_S};
#endif

#ifdef APB_PPCEXP0_S
static struct ppc_sse200_dev_cfg_t APB_PPCEXP0_DEV_CFG_S = {
    .spctrl_base  = MUSCA_S1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_S1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPCEXP0_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPCEXP0_DEV_S = {
    &APB_PPCEXP0_DEV_CFG_S, &APB_PPCEXP0_DEV_DATA_S };
#endif

#ifdef APB_PPCEXP1_S
static struct ppc_sse200_dev_cfg_t APB_PPCEXP1_DEV_CFG = {
    .spctrl_base  = MUSCA_S1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_S1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPCEXP1_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPCEXP1_DEV_S = {
    &APB_PPCEXP1_DEV_CFG, &APB_PPCEXP1_DEV_DATA_S };
#endif

/* ARM MPC SIE 200 driver structures */
#ifdef MPC_ISRAM0_S
static const struct mpc_sie_dev_cfg_t MPC_ISRAM0_DEV_CFG_S = {
    .base = MUSCA_S1_MPC_SRAM0_S_BASE};
static struct mpc_sie_dev_data_t MPC_ISRAM0_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_ISRAM0_DEV_S = {
    &(MPC_ISRAM0_DEV_CFG_S),
    &(MPC_ISRAM0_DEV_DATA_S)};
#endif

#ifdef MPC_ISRAM1_S
static const struct mpc_sie_dev_cfg_t MPC_ISRAM1_DEV_CFG_S = {
    .base = MUSCA_S1_MPC_SRAM1_S_BASE};
static struct mpc_sie_dev_data_t MPC_ISRAM1_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_ISRAM1_DEV_S = {
    &(MPC_ISRAM1_DEV_CFG_S),
    &(MPC_ISRAM1_DEV_DATA_S)};
#endif

#ifdef MPC_ISRAM2_S
static const struct mpc_sie_dev_cfg_t MPC_ISRAM2_DEV_CFG_S = {
    .base = MUSCA_S1_MPC_SRAM2_S_BASE};
static struct mpc_sie_dev_data_t MPC_ISRAM2_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_ISRAM2_DEV_S = {
    &(MPC_ISRAM2_DEV_CFG_S),
    &(MPC_ISRAM2_DEV_DATA_S)};
#endif

#ifdef MPC_ISRAM3_S
static const struct mpc_sie_dev_cfg_t MPC_ISRAM3_DEV_CFG_S = {
    .base = MUSCA_S1_MPC_SRAM3_S_BASE};
static struct mpc_sie_dev_data_t MPC_ISRAM3_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_ISRAM3_DEV_S = {
    &(MPC_ISRAM3_DEV_CFG_S),
    &(MPC_ISRAM3_DEV_DATA_S)};
#endif

#ifdef MPC_CODE_SRAM_NS
static const struct mpc_sie_dev_cfg_t MPC_CODE_SRAM_DEV_CFG_NS = {
    .base = MUSCA_S1_CODE_SRAM_MPC_NS_BASE};
static struct mpc_sie_dev_data_t MPC_CODE_SRAM_DEV_DATA_NS = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_CODE_SRAM_DEV_NS = {
    &(MPC_CODE_SRAM_DEV_CFG_NS),
    &(MPC_CODE_SRAM_DEV_DATA_NS)};
#endif

#ifdef MPC_CODE_SRAM_S
static const struct mpc_sie_dev_cfg_t MPC_CODE_SRAM_DEV_CFG_S = {
    .base = MUSCA_S1_CODE_SRAM_MPC_S_BASE};
static struct mpc_sie_dev_data_t MPC_CODE_SRAM_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_CODE_SRAM_DEV_S = {
    &(MPC_CODE_SRAM_DEV_CFG_S),
    &(MPC_CODE_SRAM_DEV_DATA_S)};
#endif

#ifdef MPC_QSPI_S
static const struct mpc_sie_dev_cfg_t MPC_QSPI_DEV_CFG_S = {
    .base = MUSCA_S1_QSPI_MPC_S_BASE};
static struct mpc_sie_dev_data_t MPC_QSPI_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_QSPI_DEV_S = {
    &(MPC_QSPI_DEV_CFG_S),
    &(MPC_QSPI_DEV_DATA_S)};
#endif

#ifdef MPC_QSPI_NS
static const struct mpc_sie_dev_cfg_t MPC_QSPI_DEV_CFG_NS = {
    .base = MUSCA_S1_QSPI_MPC_NS_BASE};
static struct mpc_sie_dev_data_t MPC_QSPI_DEV_DATA_NS = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_QSPI_DEV_NS = {
    &(MPC_QSPI_DEV_CFG_NS),
    &(MPC_QSPI_DEV_DATA_NS)};
#endif

#ifdef MPC_MRAM_S
static const struct mpc_sie_dev_cfg_t MPC_MRAM_DEV_CFG_S = {
    .base = MUSCA_S1_MRAM_MPC_S_BASE};
static struct mpc_sie_dev_data_t MPC_MRAM_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_MRAM_DEV_S = {
    &(MPC_MRAM_DEV_CFG_S),
    &(MPC_MRAM_DEV_DATA_S)};
#endif

#ifdef MPC_MRAM_NS
static const struct mpc_sie_dev_cfg_t MPC_MRAM_DEV_CFG_NS = {
    .base = MUSCA_S1_MRAM_MPC_NS_BASE};
static struct mpc_sie_dev_data_t MPC_MRAM_DEV_DATA_NS = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_MRAM_DEV_NS = {
    &(MPC_MRAM_DEV_CFG_NS),
    &(MPC_MRAM_DEV_DATA_NS)};
#endif

/* ARM MHU driver structures */
#ifdef ARM_MHU0_S
static const struct arm_mhu_sse_200_dev_cfg_t ARM_MHU0_DEV_CFG_S = {
    .base = MUSCA_S1_MHU0_S_BASE};
struct arm_mhu_sse_200_dev_t ARM_MHU0_DEV_S = {&(ARM_MHU0_DEV_CFG_S)};
#endif
#ifdef ARM_MHU0_NS
static const struct arm_mhu_sse_200_dev_cfg_t ARM_MHU0_DEV_CFG_NS = {
    .base = MUSCA_S1_MHU0_NS_BASE};
struct arm_mhu_sse_200_dev_t ARM_MHU0_DEV_NS = {&(ARM_MHU0_DEV_CFG_NS)};
#endif

#ifdef ARM_MHU1_S
static const struct arm_mhu_sse_200_dev_cfg_t ARM_MHU1_DEV_CFG_S = {
    .base = MUSCA_S1_MHU1_S_BASE};
struct arm_mhu_sse_200_dev_t ARM_MHU1_DEV_S = {&(ARM_MHU1_DEV_CFG_S)};
#endif
#ifdef ARM_MHU1_NS
static const struct arm_mhu_sse_200_dev_cfg_t ARM_MHU1_DEV_CFG_NS = {
    .base = MUSCA_S1_MHU1_NS_BASE};
struct arm_mhu_sse_200_dev_t ARM_MHU1_DEV_NS = {&(ARM_MHU1_DEV_CFG_NS)};
#endif

/* SSE-200 Cache driver structure */
#ifdef SSE_200_CACHE_S
static const struct arm_cache_dev_cfg_t SSE_200_CACHE_CFG_S = {
    .base = MUSCA_S1_CPU_ELEMENT_S_BASE};
struct arm_cache_dev_t SSE_200_CACHE_DEV_S = {&(SSE_200_CACHE_CFG_S)};
#endif

#ifdef SSE_200_CACHE_NS
static const struct arm_cache_dev_cfg_t SSE_200_CACHE_CFG_NS = {
    .base = MUSCA_S1_CPU_ELEMENT_NS_BASE};
struct arm_cache_dev_t SSE_200_CACHE_DEV_NS = {&(SSE_200_CACHE_CFG_NS)};
#endif

/* I2C IP6510 driver structures */
#ifdef I2C0_IP6510_S
static const struct i2c_ip6510_dev_cfg_t I2C0_IP6510_DEV_CFG_S = {
    .base = MUSCA_S1_I2C0_S_BASE,
    .default_mode = I2C_IP6510_MASTER_MODE,
    .default_bus_speed = I2C_IP6510_SPEED_100KHZ};
static struct i2c_ip6510_dev_data_t I2C0_IP6510_DEV_DATA_S = {
    .state = 0,
    .mode = 0,
    .bus_speed = 0};
struct i2c_ip6510_dev_t I2C0_IP6510_DEV_S = {
    &(I2C0_IP6510_DEV_CFG_S),
    &(I2C0_IP6510_DEV_DATA_S)};
#endif

#ifdef I2C0_IP6510_NS
static const struct i2c_ip6510_dev_cfg_t I2C0_IP6510_DEV_CFG_NS = {
    .base = MUSCA_S1_I2C0_NS_BASE,
    .default_mode = I2C_IP6510_MASTER_MODE,
    .default_bus_speed = I2C_IP6510_SPEED_100KHZ};
static struct i2c_ip6510_dev_data_t I2C0_IP6510_DEV_DATA_NS = {
    .state = 0,
    .mode = 0,
    .bus_speed = 0};
struct i2c_ip6510_dev_t I2C0_IP6510_DEV_NS = {
    &(I2C0_IP6510_DEV_CFG_NS),
    &(I2C0_IP6510_DEV_DATA_NS)};
#endif

#ifdef I2C1_IP6510_S
static const struct i2c_ip6510_dev_cfg_t I2C1_IP6510_DEV_CFG_S = {
    .base = MUSCA_S1_I2C1_S_BASE,
    .default_mode = I2C_IP6510_MASTER_MODE,
    .default_bus_speed = I2C_IP6510_SPEED_100KHZ};
static struct i2c_ip6510_dev_data_t I2C1_IP6510_DEV_DATA_S = {
    .state = 0,
    .mode = 0,
    .bus_speed = 0};
struct i2c_ip6510_dev_t I2C1_IP6510_DEV_S = {
    &(I2C1_IP6510_DEV_CFG_S),
    &(I2C1_IP6510_DEV_DATA_S)};
#endif

#ifdef I2C1_IP6510_NS
static const struct i2c_ip6510_dev_cfg_t I2C1_IP6510_DEV_CFG_NS = {
    .base = MUSCA_S1_I2C1_NS_BASE,
    .default_mode = I2C_IP6510_MASTER_MODE,
    .default_bus_speed = I2C_IP6510_SPEED_100KHZ};
static struct i2c_ip6510_dev_data_t I2C1_IP6510_DEV_DATA_NS = {
    .state = 0,
    .mode = 0,
    .bus_speed = 0};
struct i2c_ip6510_dev_t I2C1_IP6510_DEV_NS = {
    &(I2C1_IP6510_DEV_CFG_NS),
    &(I2C1_IP6510_DEV_DATA_NS)};
#endif

/* CMSDK Timers driver structures */
#ifdef CMSDK_TIMER0_S
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER0_DEV_CFG_S = {
    .base = MUSCA_S1_CMSDK_TIMER0_S_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER0_DEV_DATA_S = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_S = {&(CMSDK_TIMER0_DEV_CFG_S),
                                               &(CMSDK_TIMER0_DEV_DATA_S)};
#endif
#ifdef CMSDK_TIMER0_NS
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER0_DEV_CFG_NS = {
    .base = MUSCA_S1_CMSDK_TIMER0_NS_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER0_DEV_DATA_NS = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_NS = {&(CMSDK_TIMER0_DEV_CFG_NS),
                                                &(CMSDK_TIMER0_DEV_DATA_NS)};
#endif

#ifdef CMSDK_TIMER1_S
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER1_DEV_CFG_S = {
    .base = MUSCA_S1_CMSDK_TIMER1_S_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER1_DEV_DATA_S = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_S = {&(CMSDK_TIMER1_DEV_CFG_S),
                                               &(CMSDK_TIMER1_DEV_DATA_S)};
#endif
#ifdef CMSDK_TIMER1_NS
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER1_DEV_CFG_NS = {
    .base = MUSCA_S1_CMSDK_TIMER1_NS_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER1_DEV_DATA_NS = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_NS = {&(CMSDK_TIMER1_DEV_CFG_NS),
                                                &(CMSDK_TIMER1_DEV_DATA_NS)};
#endif

/* CMSDK Dualtimer driver structures */
#ifdef CMSDK_DUALTIMER_S
static const struct dualtimer_cmsdk_dev_cfg_t CMSDK_DUALTIMER_DEV_CFG_S = {
    .base = MUSCA_S1_CMSDK_DUALTIMER_S_BASE};
static struct dualtimer_cmsdk_dev_data_t CMSDK_DUALTIMER_DEV_DATA_S = {
    .is_initialized = 0};

struct dualtimer_cmsdk_dev_t CMSDK_DUALTIMER_DEV_S = {
    &(CMSDK_DUALTIMER_DEV_CFG_S),
    &(CMSDK_DUALTIMER_DEV_DATA_S)};
#endif

#ifdef CMSDK_DUALTIMER_NS
static const struct dualtimer_cmsdk_dev_cfg_t CMSDK_DUALTIMER_DEV_CFG_NS = {
    .base = MUSCA_S1_CMSDK_DUALTIMER_NS_BASE};
static struct dualtimer_cmsdk_dev_data_t CMSDK_DUALTIMER_DEV_DATA_NS = {
    .is_initialized = 0};

struct dualtimer_cmsdk_dev_t CMSDK_DUALTIMER_DEV_NS = {
    &(CMSDK_DUALTIMER_DEV_CFG_NS),
    &(CMSDK_DUALTIMER_DEV_DATA_NS)};
#endif

/* General-Purpose Timer driver structures */
#ifdef GP_TIMER_S
static const struct timer_gp_dev_cfg_t GP_TIMER_DEV_CFG_S = {
    .base = MUSCA_S1_TIMER_S_BASE};
static struct timer_gp_dev_data_t GP_TIMER_DEV_DATA_S = {
    .is_initialized = false,
    .alarm0_init = 0,
    .alarm1_init = 0};
struct timer_gp_dev_t GP_TIMER_DEV_S = {
    &(GP_TIMER_DEV_CFG_S),
    &(GP_TIMER_DEV_DATA_S)};
#endif

#ifdef GP_TIMER_NS
static const struct timer_gp_dev_cfg_t GP_TIMER_DEV_CFG_NS = {
    .base = MUSCA_S1_TIMER_NS_BASE};
static struct timer_gp_dev_data_t GP_TIMER_DEV_DATA_NS = {
    .is_initialized = false,
    .alarm0_init = 0,
    .alarm1_init = 0};
struct timer_gp_dev_t GP_TIMER_DEV_NS = {
    &(GP_TIMER_DEV_CFG_NS),
    &(GP_TIMER_DEV_DATA_NS)};
#endif

/* PL031 Real-Time Clock structure */
#ifdef RTC_PL031_S
static const struct rtc_pl031_dev_cfg_t RTC_PL031_DEV_CFG_S = {
    .base = MUSCA_S1_RTC_S_BASE};
struct rtc_pl031_dev_t RTC_PL031_DEV_S = {&(RTC_PL031_DEV_CFG_S)};
#endif

#ifdef RTC_PL031_NS
static const struct rtc_pl031_dev_cfg_t RTC_PL031_DEV_CFG_NS = {
    .base = MUSCA_S1_RTC_NS_BASE};
struct rtc_pl031_dev_t RTC_PL031_DEV_NS = {&(RTC_PL031_DEV_CFG_NS)};
#endif

#ifdef UART0_PL011_S
static const struct uart_pl011_dev_cfg_t UART0_PL011_DEV_CFG_S = {
    .base = MUSCA_S1_UART0_S_BASE,
    .def_baudrate = UART_DEFAULT_BAUD_RATE,
    .def_wlen = UART_PL011_WLEN_8,
    .def_parity = UART_PL011_PARITY_DISABLED,
    .def_stopbit = UART_PL011_STOPBIT_1};
static struct uart_pl011_dev_data_t UART0_PL011_DEV_DATA_S = {
    .state = 0,
    .uart_clk = 0,
    .baudrate = 0};
struct uart_pl011_dev_t UART0_PL011_DEV_S = {&(UART0_PL011_DEV_CFG_S),
                                               &(UART0_PL011_DEV_DATA_S)};
#endif

#ifdef UART0_PL011_NS
static const struct uart_pl011_dev_cfg_t UART0_PL011_DEV_CFG_NS = {
    .base = MUSCA_S1_UART0_NS_BASE,
    .def_baudrate = UART_DEFAULT_BAUD_RATE,
    .def_wlen = UART_PL011_WLEN_8,
    .def_parity = UART_PL011_PARITY_DISABLED,
    .def_stopbit = UART_PL011_STOPBIT_1};
static struct uart_pl011_dev_data_t UART0_PL011_DEV_DATA_NS = {
    .state = 0,
    .uart_clk = 0,
    .baudrate = 0};
struct uart_pl011_dev_t UART0_PL011_DEV_NS = {&(UART0_PL011_DEV_CFG_NS),
                                                &(UART0_PL011_DEV_DATA_NS)};
#endif

#ifdef UART1_PL011_S
static const struct uart_pl011_dev_cfg_t UART1_PL011_DEV_CFG_S = {
    .base = MUSCA_S1_UART1_S_BASE,
    .def_baudrate = UART_DEFAULT_BAUD_RATE,
    .def_wlen = UART_PL011_WLEN_8,
    .def_parity = UART_PL011_PARITY_DISABLED,
    .def_stopbit = UART_PL011_STOPBIT_1};
static struct uart_pl011_dev_data_t UART1_PL011_DEV_DATA_S = {
    .state = 0,
    .uart_clk = 0,
    .baudrate = 0};
struct uart_pl011_dev_t UART1_PL011_DEV_S = {&(UART1_PL011_DEV_CFG_S),
                                               &(UART1_PL011_DEV_DATA_S)};
#endif

#ifdef UART1_PL011_NS
static const struct uart_pl011_dev_cfg_t UART1_PL011_DEV_CFG_NS = {
    .base = MUSCA_S1_UART1_NS_BASE,
    .def_baudrate = UART_DEFAULT_BAUD_RATE,
    .def_wlen = UART_PL011_WLEN_8,
    .def_parity = UART_PL011_PARITY_DISABLED,
    .def_stopbit = UART_PL011_STOPBIT_1};
static struct uart_pl011_dev_data_t UART1_PL011_DEV_DATA_NS = {
    .state = 0,
    .uart_clk = 0,
    .baudrate = 0};
struct uart_pl011_dev_t UART1_PL011_DEV_NS = {&(UART1_PL011_DEV_CFG_NS),
                                                &(UART1_PL011_DEV_DATA_NS)};
#endif

/* SPI IP6524 driver structures */
#ifdef SPI0_IP6524_S
static const struct spi_ip6524_dev_cfg_t SPI0_DEV_CFG_S = {
    .base = MUSCA_S1_SPI0_S_BASE};
static struct spi_ip6524_dev_data_t SPI0_DEV_DATA_S = {
    .state = 0};
struct spi_ip6524_dev_t SPI0_DEV_S = {&(SPI0_DEV_CFG_S),
                                      &(SPI0_DEV_DATA_S)};
#endif

#ifdef SPI0_IP6524_NS
static const struct spi_ip6524_dev_cfg_t SPI0_DEV_CFG_NS = {
    .base = MUSCA_S1_SPI0_NS_BASE};
static struct spi_ip6524_dev_data_t SPI0_DEV_DATA_NS = {
    .state = 0};
struct spi_ip6524_dev_t SPI0_DEV_NS = {&(SPI0_DEV_CFG_NS),
                                       &(SPI0_DEV_DATA_NS)};
#endif

/* QSPI IP6514E driver structures */
#ifdef QSPI_IP6514E_S
static const struct qspi_ip6514e_dev_cfg_t QSPI_DEV_CFG_S = {
    .base = MUSCA_S1_QSPI_REG_S_BASE,
    .addr_mask = (1U << 25) - 1, /* 32 MiB minus 1 byte */
};
struct qspi_ip6514e_dev_t QSPI_DEV_S = {
    &QSPI_DEV_CFG_S
};
#endif

#ifdef QSPI_IP6514E_NS
static const struct qspi_ip6514e_dev_cfg_t QSPI_DEV_CFG_NS = {
    .base = MUSCA_S1_QSPI_REG_NS_BASE,
    .addr_mask = (1U << 25) - 1, /* 32 MiB minus 1 byte */
};
struct qspi_ip6514e_dev_t QSPI_DEV_NS = {
    &QSPI_DEV_CFG_NS
};
#endif

/* ======= External peripheral configuration structure definitions ======= */

/* MT25QL Flash memory library structures */
#if (defined(MT25QL_S) && defined(QSPI_IP6514E_S))
struct mt25ql_dev_t MT25QL_DEV_S = {
    .controller = &QSPI_DEV_S,
    .direct_access_start_addr = MUSCA_S1_QSPI_FLASH_S_BASE,
    .baud_rate_div = 4U,
    .size = 0x01000000U, /* 16 MiB */
    .config_state = { 0 },
};
#endif

#if (defined(MT25QL_NS) && defined(QSPI_IP6514E_NS))
struct mt25ql_dev_t MT25QL_DEV_NS = {
    .controller = &QSPI_DEV_NS,
    .direct_access_start_addr = MUSCA_S1_QSPI_FLASH_NS_BASE,
    .baud_rate_div = 4U,
    .size = 0x01000000U, /* 16 MiB */
    .config_state = { 0 },
};
#endif
