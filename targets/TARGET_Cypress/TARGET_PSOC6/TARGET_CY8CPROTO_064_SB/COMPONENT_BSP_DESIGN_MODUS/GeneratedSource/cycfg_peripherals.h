/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_scb_ezi2c.h"
#include "cy_sysclk.h"
#if defined (CY_USING_HAL)
	#include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)
#include "cy_smif.h"
#include "cycfg_qspi_memslot.h"
#include "cy_usbfs_dev_drv.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_I2C_ENABLED 1U
#define CYBSP_I2C_HW SCB3
#define CYBSP_I2C_IRQ scb_3_interrupt_IRQn
#define CYBSP_QSPI_ENABLED 1U
#define CYBSP_QSPI_HW SMIF0
#define CYBSP_QSPI_IRQ smif_interrupt_IRQn
#define CYBSP_QSPI_MEMORY_MODE_ALIGMENT_ERROR (0UL)
#define CYBSP_QSPI_RX_DATA_FIFO_UNDERFLOW (0UL)
#define CYBSP_QSPI_TX_COMMAND_FIFO_OVERFLOW (0UL)
#define CYBSP_QSPI_TX_DATA_FIFO_OVERFLOW (0UL)
#define CYBSP_QSPI_RX_FIFO_TRIGEER_LEVEL (0UL)
#define CYBSP_QSPI_TX_FIFO_TRIGEER_LEVEL (0UL)
#define CYBSP_QSPI_DATALINES0_1 (1UL)
#define CYBSP_QSPI_DATALINES2_3 (1UL)
#define CYBSP_QSPI_DATALINES4_5 (0UL)
#define CYBSP_QSPI_DATALINES6_7 (0UL)
#define CYBSP_QSPI_SS0 (1UL)
#define CYBSP_QSPI_SS1 (0UL)
#define CYBSP_QSPI_SS2 (0UL)
#define CYBSP_QSPI_SS3 (0UL)
#define CYBSP_QSPI_DESELECT_DELAY 7
#define CYBSP_USBUART_ENABLED 1U
#define CYBSP_USBUART_ACTIVE_ENDPOINTS_MASK 0U
#define CYBSP_USBUART_ENDPOINTS_BUFFER_SIZE 512U
#define CYBSP_USBUART_ENDPOINTS_ACCESS_TYPE 0U
#define CYBSP_USBUART_HW USBFS0
#define CYBSP_USBUART_HI_IRQ usb_interrupt_hi_IRQn
#define CYBSP_USBUART_MED_IRQ usb_interrupt_med_IRQn
#define CYBSP_USBUART_LO_IRQ usb_interrupt_lo_IRQn

extern const cy_stc_scb_ezi2c_config_t CYBSP_I2C_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_I2C_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_smif_config_t CYBSP_QSPI_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_usbfs_dev_drv_config_t CYBSP_USBUART_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USBUART_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
