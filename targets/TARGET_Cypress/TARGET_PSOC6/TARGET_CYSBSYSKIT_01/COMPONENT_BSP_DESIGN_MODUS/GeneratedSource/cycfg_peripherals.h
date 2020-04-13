/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* Device Configurator: 2.0.0.1483
* Device Support Library (../../../psoc6pdl): 1.3.1.1499
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
#include "cy_smif.h"
#include "cycfg_qspi_memslot.h"
#if defined (CY_USING_HAL)
#include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)

#if defined(__cplusplus)
extern "C" {
#endif

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

extern const cy_stc_smif_config_t CYBSP_QSPI_config;
#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_QSPI_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
