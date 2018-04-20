/*
 * Copyright (c) 2017-2018 ARM Limited
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

#ifndef __ARM_LTD_PLATFORM_DEVICES_H__
#define __ARM_LTD_PLATFORM_DEVICES_H__

/* ======= Configures the peripheral set ======= */
#include "device_cfg.h"

/* ======= Includes generic driver headers ======= */
#include "timer_cmsdk_drv.h"
#include "arm_gpio_drv.h"
#include "arm_mps2_io_drv.h"
#include "spi_pl022_drv.h"
#include "arm_uart_drv.h"

/* ======= Defines peripheral configuration structures ======= */

/* ARM CMSDK Timer driver structures */
#ifdef ARM_CMSDK_TIMER0
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV;
#endif

#ifdef ARM_CMSDK_TIMER1
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV;
#endif

/* ARM GPIO driver structures */
#ifdef ARM_GPIO0
extern struct arm_gpio_dev_t ARM_GPIO0_DEV;
#endif
#ifdef ARM_GPIO1
extern struct arm_gpio_dev_t ARM_GPIO1_DEV;
#endif
#ifdef ARM_GPIO2
extern struct arm_gpio_dev_t ARM_GPIO2_DEV;
#endif
#ifdef ARM_GPIO3
extern struct arm_gpio_dev_t ARM_GPIO3_DEV;
#endif

/* ARM MPS2 IO FPGAIO driver structures */
#ifdef ARM_MPS2_IO_FPGAIO
extern struct arm_mps2_io_dev_t ARM_MPS2_IO_FPGAIO_DEV;
#endif

/* ARM MPS2 IO SCC driver structures */
#ifdef ARM_MPS2_IO_SCC
extern struct arm_mps2_io_dev_t ARM_MPS2_IO_SCC_DEV;
#endif

/* ARM SPI driver structures */
#ifdef ARM_SPI0
extern struct spi_pl022_dev_t SPI0_PL022_DEV;
#endif
#ifdef ARM_SPI1
extern struct spi_pl022_dev_t SPI1_PL022_DEV;
#endif
#ifdef ARM_SPI2
extern struct spi_pl022_dev_t SPI2_PL022_DEV;
#endif
#ifdef ARM_SPI3
extern struct spi_pl022_dev_t SPI3_PL022_DEV;
#endif
#ifdef ARM_SPI4
extern struct spi_pl022_dev_t SPI4_PL022_DEV;
#endif

/* ARM UART driver structures */
#ifdef ARM_UART0
extern struct arm_uart_dev_t ARM_UART0_DEV;
#endif
#ifdef ARM_UART1
extern struct arm_uart_dev_t ARM_UART1_DEV;
#endif
#ifdef ARM_UART2
extern struct arm_uart_dev_t ARM_UART2_DEV;
#endif
#ifdef ARM_UART3
extern struct arm_uart_dev_t ARM_UART3_DEV;
#endif
#ifdef ARM_UART4
extern struct arm_uart_dev_t ARM_UART4_DEV;
#endif

#endif  /* __ARM_LTD_PLATFORM_DEVICES_H__ */
