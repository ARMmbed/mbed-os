/*******************************************************************************
* File Name: cycfg_qspi_memslot.h
*
* Description:
* Provides declarations of the SMIF-driver memory configuration.
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

#ifndef CYCFG_QSPI_MEMSLOT_H
#define CYCFG_QSPI_MEMSLOT_H
#include "cy_smif_memslot.h"

#define CY_SMIF_DEVICE_NUM 1

extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_readCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_writeEnCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_writeDisCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_eraseCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_chipEraseCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_programCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_readStsRegQeCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_readStsRegWipCmd;
extern cy_stc_smif_mem_cmd_t S25HL512T_SlaveSlot_0_writeStsRegQeCmd;

extern cy_stc_smif_mem_device_cfg_t deviceCfg_S25HL512T_SlaveSlot_0;

extern const cy_stc_smif_mem_config_t S25HL512T_SlaveSlot_0;
extern const cy_stc_smif_mem_config_t* smifMemConfigs[CY_SMIF_DEVICE_NUM];

extern const cy_stc_smif_block_config_t smifBlockConfig;


#endif /*CY_SMIF_MEMCONFIG_H*/
