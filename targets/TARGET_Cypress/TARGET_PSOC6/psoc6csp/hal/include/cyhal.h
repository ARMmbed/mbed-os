/*******************************************************************************
* File Name: cyhal.h
*
* Description:
* Top-level HAL header file that includes all available HAL header files. This
* will pull in all of the specific HAL files needed. Not all of these may be
* supported in the target device. The target device must provide a
* cyhal_hw_types.h file that is in the include path for the hal headers to
* depend on. The cyhal_hw_types.h file must provide the following:
*   1) definitions for each of the resource types consumed by the HAL driver
*      functions.
*   2) A CYHAL_ISR_PRIORITY_DEFAULT define for the default interrupt priority
*   3) A CYHAL_MAIN_FLASH_BASE define for the base address of flash memory
*   4) OPTIONAL: Implementation specific header files can be used by creating
*      a #define in cyhal_hw_types.h with a name of CYHAL_<DRIVER>_IMPL_HEADER
*      and the value being the name of the header file.  eg:
*      #define CYHAL_GPIO_IMPL_HEADER "cyhal_gpio_impl.h"
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_hal HAL Drivers
* This section documents the drivers which form the stable API of the Cypress HAL.
* In order to remain portable across platforms and HAL versions, applications should
* rely only on functionality documented in this section.
* \{
* \defgroup group_result Result Type
* \}
*/

#pragma once

#include "cyhal_modules.h"
#include "cyhal_hw_types.h"

#include "cyhal_adc.h"
#include "cyhal_crc.h"
#include "cyhal_dac.h"
#include "cyhal_dma.h"
#include "cyhal_flash.h"
#include "cyhal_gpio.h"
#include "cyhal_hwmgr.h"
#include "cyhal_i2c.h"
#include "cyhal_ezi2c.h"
#include "cyhal_interconnect.h"
#include "cyhal_lptimer.h"
#include "cyhal_pwm.h"
#include "cyhal_qspi.h"
#include "cyhal_rtc.h"
#include "cyhal_sdhc.h"
#include "cyhal_sdio.h"
#include "cyhal_spi.h"
#include "cyhal_system.h"
#include "cyhal_timer.h"
#include "cyhal_trng.h"
#include "cyhal_uart.h"
#include "cyhal_usb_dev.h"
#include "cyhal_wdt.h"
