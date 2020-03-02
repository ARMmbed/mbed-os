/*
 * Copyright (c) 2018-2020 Arm Limited
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
/* MUSCA B1 SCC driver structures */
#ifdef MUSCA_B1_SCC_S
static const struct musca_b1_scc_dev_cfg_t MUSCA_B1_SCC_DEV_CFG_S = {
    .base = MUSCA_B1_SCC_S_BASE};
struct musca_b1_scc_dev_t MUSCA_B1_SCC_DEV_S = {&(MUSCA_B1_SCC_DEV_CFG_S)};
#endif

/* ARM PPC SSE 200 driver structures */
#ifdef AHB_PPC0_S
static struct ppc_sse200_dev_cfg_t AHB_PPC0_DEV_CFG_S = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
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
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t AHB_PPCEXP0_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t AHB_PPCEXP0_DEV_S = {
    &AHB_PPCEXP0_DEV_CFG_S, &AHB_PPCEXP0_DEV_DATA_S };
#endif

#ifdef AHB_PPCEXP1_S
static struct ppc_sse200_dev_cfg_t AHB_PPCEXP1_DEV_CFG_S = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t AHB_PPCEXP1_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t AHB_PPCEXP1_DEV_S = {
    &AHB_PPCEXP1_DEV_CFG_S, &AHB_PPCEXP1_DEV_DATA_S };
#endif

#ifdef AHB_PPCEXP2_S
static struct ppc_sse200_dev_cfg_t AHB_PPCEXP2_DEV_CFG_S = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t AHB_PPCEXP2_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t AHB_PPCEXP2_DEV_S = {
    &AHB_PPCEXP2_DEV_CFG_S, &AHB_PPCEXP2_DEV_DATA_S };
#endif

#ifdef AHB_PPCEXP3_S
static struct ppc_sse200_dev_cfg_t AHB_PPCEXP3_DEV_CFG_S = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t AHB_PPCEXP3_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t AHB_PPCEXP3_DEV_S = {
    &AHB_PPCEXP3_DEV_CFG_S, &AHB_PPCEXP3_DEV_DATA_S };
#endif

#ifdef APB_PPC0_S
static struct ppc_sse200_dev_cfg_t APB_PPC0_DEV_CFG_S = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
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
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
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
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
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
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPCEXP1_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPCEXP1_DEV_S = {
    &APB_PPCEXP1_DEV_CFG, &APB_PPCEXP1_DEV_DATA_S };
#endif

#ifdef APB_PPCEXP2_S
static struct ppc_sse200_dev_cfg_t APB_PPCEXP2_DEV_CFG = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPCEXP2_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPCEXP2_DEV_S = {
    &APB_PPCEXP2_DEV_CFG, &APB_PPCEXP2_DEV_DATA_S };
#endif

#ifdef APB_PPCEXP3_S
static struct ppc_sse200_dev_cfg_t APB_PPCEXP3_DEV_CFG = {
    .spctrl_base  = MUSCA_B1_SPCTRL_S_BASE,
    .nspctrl_base = MUSCA_B1_NSPCTRL_NS_BASE };
static struct ppc_sse200_dev_data_t APB_PPCEXP3_DEV_DATA_S = {
    .p_ns_ppc  = 0,
    .p_sp_ppc  = 0,
    .p_nsp_ppc = 0,
    .int_bit_mask = 0,
    .state = 0 };
struct ppc_sse200_dev_t APB_PPCEXP3_DEV_S = {
    &APB_PPCEXP3_DEV_CFG, &APB_PPCEXP3_DEV_DATA_S };
#endif

/* ARM MPC SIE 200 driver structures */
#ifdef MPC_ISRAM0_S
static const struct mpc_sie_dev_cfg_t MPC_ISRAM0_DEV_CFG_S = {
    .base = MUSCA_B1_MPC_SRAM0_S_BASE};
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
    .base = MUSCA_B1_MPC_SRAM1_S_BASE};
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
    .base = MUSCA_B1_MPC_SRAM2_S_BASE};
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
    .base = MUSCA_B1_MPC_SRAM3_S_BASE};
static struct mpc_sie_dev_data_t MPC_ISRAM3_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_ISRAM3_DEV_S = {
    &(MPC_ISRAM3_DEV_CFG_S),
    &(MPC_ISRAM3_DEV_DATA_S)};
#endif

#ifdef MPC_CODE_SRAM_S
static const struct mpc_sie_dev_cfg_t MPC_CODE_SRAM_DEV_CFG_S = {
    .base = MUSCA_B1_CODE_SRAM_MPC_S_BASE};
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
    .base = MUSCA_B1_QSPI_MPC_S_BASE};
static struct mpc_sie_dev_data_t MPC_QSPI_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_QSPI_DEV_S = {
    &(MPC_QSPI_DEV_CFG_S),
    &(MPC_QSPI_DEV_DATA_S)};
#endif

#ifdef MPC_EFLASH0_S
static const struct mpc_sie_dev_cfg_t MPC_EFLASH0_DEV_CFG_S = {
    .base = MUSCA_B1_EFLASH0_MPC_S_BASE};
static struct mpc_sie_dev_data_t MPC_EFLASH0_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_EFLASH0_DEV_S = {
    &(MPC_EFLASH0_DEV_CFG_S),
    &(MPC_EFLASH0_DEV_DATA_S)};
#endif
#ifdef MPC_EFLASH1_S
static const struct mpc_sie_dev_cfg_t MPC_EFLASH1_DEV_CFG_S = {
    .base = MUSCA_B1_EFLASH1_MPC_S_BASE};
static struct mpc_sie_dev_data_t MPC_EFLASH1_DEV_DATA_S = {
    .range_list = 0,
    .nbr_of_ranges = 0,
    .is_initialized = false};
struct mpc_sie_dev_t MPC_EFLASH1_DEV_S = {
    &(MPC_EFLASH1_DEV_CFG_S),
    &(MPC_EFLASH1_DEV_DATA_S)};
#endif


/** CMSDK GPIO driver structures */
#ifdef GPIO0_CMSDK_S
static const struct gpio_cmsdk_dev_cfg_t GPIO0_CMSDK_DEV_CFG_S = {
    .base = MUSCA_B1_GPIO_S_BASE};
struct gpio_cmsdk_dev_t GPIO0_CMSDK_DEV_S = {&(GPIO0_CMSDK_DEV_CFG_S)};
#endif

/** ARM UART PL011 driver structures */
#ifdef UART0_PL011_S
static const struct uart_pl011_dev_cfg_t UART0_PL011_DEV_CFG_S = {
    .base = MUSCA_B1_UART0_S_BASE,
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
    .base = MUSCA_B1_UART0_NS_BASE,
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
    .base = MUSCA_B1_UART1_S_BASE,
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
    .base = MUSCA_B1_UART1_NS_BASE,
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

/** CMSDK Timers driver structures */
#ifdef CMSDK_TIMER0_S
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER0_DEV_CFG_S = {
    .base = MUSCA_B1_CMSDK_TIMER0_S_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER0_DEV_DATA_S = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_S = {&(CMSDK_TIMER0_DEV_CFG_S),
                                               &(CMSDK_TIMER0_DEV_DATA_S)};
#endif
#ifdef CMSDK_TIMER0_NS
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER0_DEV_CFG_NS = {
    .base = MUSCA_B1_CMSDK_TIMER0_NS_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER0_DEV_DATA_NS = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_NS = {&(CMSDK_TIMER0_DEV_CFG_NS),
                                                &(CMSDK_TIMER0_DEV_DATA_NS)};
#endif

#ifdef CMSDK_TIMER1_S
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER1_DEV_CFG_S = {
    .base = MUSCA_B1_CMSDK_TIMER1_S_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER1_DEV_DATA_S = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_S = {&(CMSDK_TIMER1_DEV_CFG_S),
                                               &(CMSDK_TIMER1_DEV_DATA_S)};
#endif
#ifdef CMSDK_TIMER1_NS
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER1_DEV_CFG_NS = {
    .base = MUSCA_B1_CMSDK_TIMER1_NS_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER1_DEV_DATA_NS = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_NS = {&(CMSDK_TIMER1_DEV_CFG_NS),
                                                &(CMSDK_TIMER1_DEV_DATA_NS)};
#endif

/** GFC-100 eflash driver structures */
#ifdef GFC100_EFLASH0_S
static const struct gfc100_eflash_dev_cfg_t GFC100_EFLASH0_CFG_S = {
    .base = MUSCA_B1_EFLASH0_REG_MAP_S_BASE};
static struct gfc100_eflash_dev_data_t GFC100_EFLASH0_DATA_S = {
    .is_initialized = false,
    .flash_size = 0};
struct gfc100_eflash_dev_t GFC100_EFLASH0_DEV_S = {&(GFC100_EFLASH0_CFG_S),
                                                   &(GFC100_EFLASH0_DATA_S)};
#endif

#ifdef GFC100_EFLASH1_S
static const struct gfc100_eflash_dev_cfg_t GFC100_EFLASH1_CFG_S = {
    .base = MUSCA_B1_EFLASH1_REG_MAP_S_BASE};
static struct gfc100_eflash_dev_data_t GFC100_EFLASH1_DATA_S = {
    .is_initialized = false,
    .flash_size = 0};
struct gfc100_eflash_dev_t GFC100_EFLASH1_DEV_S = {&(GFC100_EFLASH1_CFG_S),
                                                   &(GFC100_EFLASH1_DATA_S)};
#endif

/* General-Purpose Timer driver structures */
#ifdef GP_TIMER_S
static const struct timer_gp_dev_cfg_t GP_TIMER_DEV_CFG_S = {
    .base = MUSCA_B1_TIMER_S_BASE};
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
    .base = MUSCA_B1_TIMER_NS_BASE};
static struct timer_gp_dev_data_t GP_TIMER_DEV_DATA_NS = {
    .is_initialized = false,
    .alarm0_init = 0,
    .alarm1_init = 0};
struct timer_gp_dev_t GP_TIMER_DEV_NS = {
    &(GP_TIMER_DEV_CFG_NS),
    &(GP_TIMER_DEV_DATA_NS)};
#endif

/* QSPI IP6514E driver structures */
#ifdef QSPI_IP6514E_S
static const struct qspi_ip6514e_dev_cfg_t QSPI_DEV_CFG_S = {
    .base = MUSCA_B1_QSPI_REG_S_BASE,
    .addr_mask = (1U << 23) - 1, /* 8MiB minus 1 byte */
};
struct qspi_ip6514e_dev_t QSPI_DEV_S = {
    &QSPI_DEV_CFG_S
};
#endif

#ifdef QSPI_IP6514E_NS
static const struct qspi_ip6514e_dev_cfg_t QSPI_DEV_CFG_NS = {
    .base = MUSCA_B1_QSPI_REG_NS_BASE,
    .addr_mask = (1U << 23) - 1, /* 8MiB minus 1 byte */
};
struct qspi_ip6514e_dev_t QSPI_DEV_NS = {
    &QSPI_DEV_CFG_NS
};
#endif

/* PL031 Real-Time Clock structure */
#ifdef RTC_PL031_S
static const struct rtc_pl031_dev_cfg_t RTC_PL031_DEV_CFG_S = {
    .base = MUSCA_B1_RTC_S_BASE};
struct rtc_pl031_dev_t RTC_PL031_DEV_S = {&(RTC_PL031_DEV_CFG_S)};
#endif

#ifdef RTC_PL031_NS
static const struct rtc_pl031_dev_cfg_t RTC_PL031_DEV_CFG_NS = {
    .base = MUSCA_B1_RTC_NS_BASE};
struct rtc_pl031_dev_t RTC_PL031_DEV_NS = {&(RTC_PL031_DEV_CFG_NS)};
#endif

/* I2S IP6718E driver structures */
#ifdef I2S_IP6718E_S
static const struct i2s_ip6718e_dev_cfg_t I2S_IP6718E_DEV_CFG_S = {
    .base = MUSCA_B1_I2S_S_BASE,
    .audio_intf_mode = I2S_IP6718E_PHILIPS_MODE};
static struct i2s_ip6718e_dev_data_t I2S_IP6718E_DEV_DATA_S = {
    .controller_state = 0,
    .audio_interface_mode = 0,
    .transmitter_audio_frequency = 0,
    .receiver_audio_frequency = 0,
    .muted = 0};
struct i2s_ip6718e_dev_t I2S_IP6718E_DEV_S = {&(I2S_IP6718E_DEV_CFG_S),
                                              &(I2S_IP6718E_DEV_DATA_S)};
#endif

#ifdef I2S_IP6718E_NS
static const struct i2s_ip6718e_dev_cfg_t I2S_IP6718E_DEV_CFG_NS = {
    .base = MUSCA_B1_I2S_NS_BASE,
    .audio_intf_mode = I2S_IP6718E_PHILIPS_MODE};
static struct i2s_ip6718e_dev_data_t I2S_IP6718E_DEV_DATA_NS = {
    .controller_state = 0,
    .audio_interface_mode = 0,
    .transmitter_audio_frequency = 0,
    .receiver_audio_frequency = 0,
    .muted = 0};
struct i2s_ip6718e_dev_t I2S_IP6718E_DEV_NS = {&(I2S_IP6718E_DEV_CFG_NS),
                                               &(I2S_IP6718E_DEV_DATA_NS)};
#endif

/* SSE-200 Cache driver structure */
#ifdef SSE_200_CACHE_S
static const struct arm_cache_dev_cfg_t SSE_200_CACHE_CFG_S = {
    .base = MUSCA_B1_CPU_ELEMENT_S_BASE};
struct arm_cache_dev_t SSE_200_CACHE_DEV_S = {&(SSE_200_CACHE_CFG_S)};
#endif

#ifdef SSE_200_CACHE_NS
static const struct arm_cache_dev_cfg_t SSE_200_CACHE_CFG_NS = {
    .base = MUSCA_B1_CPU_ELEMENT_NS_BASE};
struct arm_cache_dev_t SSE_200_CACHE_DEV_NS = {&(SSE_200_CACHE_CFG_NS)};
#endif

/* I2C IP6510 driver structures */
#ifdef I2C0_IP6510_S
static const struct i2c_ip6510_dev_cfg_t I2C0_IP6510_DEV_CFG_S = {
        .base = MUSCA_B1_I2C0_S_BASE,
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
        .base = MUSCA_B1_I2C0_NS_BASE,
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
        .base = MUSCA_B1_I2C1_S_BASE,
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
        .base = MUSCA_B1_I2C1_NS_BASE,
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

/* ======= External peripheral configuration structure definitions ======= */

/* MT25QL Flash memory library structures */
#if (defined(MT25QL_S) && defined(QSPI_IP6514E_S))
struct mt25ql_dev_t MT25QL_DEV_S = {
    .controller = &QSPI_DEV_S,
    .direct_access_start_addr = MUSCA_B1_QSPI_FLASH_S_BASE,
    .baud_rate_div = 4U,
    /*
     * 8 MiB flash memory are advertised in the Arm Musca-B1 Test Chip and Board
     * Technical Reference Manual. The MT25QL Flash device may however contain
     * more.
     */
    .size = 0x00800000U, /* 8 MiB */
    .config_state = { 0 },
};
#endif

#if (defined(MT25QL_NS) && defined(QSPI_IP6514E_NS))
struct mt25ql_dev_t MT25QL_DEV_NS = {
    .controller = &QSPI_DEV_NS,
    .direct_access_start_addr = MUSCA_B1_QSPI_FLASH_NS_BASE,
    .baud_rate_div = 4U,
    /*
     * 8 MiB flash memory are advertised in the Arm Musca-B1 Test Chip and Board
     * Technical Reference Manual. The MT25QL Flash device may however contain
     * more.
     */
    .size = 0x00800000U, /* 8 MiB */
    .config_state = { 0 },
};
#endif
