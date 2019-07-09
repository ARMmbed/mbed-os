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

/*******************************************************************************

QSPI_CONFIG_START

<CySMIFConfiguration>
    <DevicePath>PSoC 6.xml</DevicePath>
    <SlotConfigs>
        <SlotConfig>
            <SlaveSlot>0</SlaveSlot>
            <PartNumber>S25FL512S</PartNumber>
            <MemoryMapped>true</MemoryMapped>
            <DualQuad>None</DualQuad>
            <StartAddress>0x18000000</StartAddress>
            <Size>0x40000</Size>
            <EndAddress>0x1803FFFF</EndAddress>
            <WriteEnable>true</WriteEnable>
            <Encrypt>false</Encrypt>
            <DataSelect>QUAD_SPI_DATA_0_3</DataSelect>
            <MemoryConfigsPath>S25FL512S</MemoryConfigsPath>
            <ConfigDataInFlash>true</ConfigDataInFlash>
        </SlotConfig>
        <SlotConfig>
            <SlaveSlot>1</SlaveSlot>
            <PartNumber>Not used</PartNumber>
            <MemoryMapped>false</MemoryMapped>
            <DualQuad>None</DualQuad>
            <StartAddress>0x18010000</StartAddress>
            <Size>0x10000</Size>
            <EndAddress>0x1801FFFF</EndAddress>
            <WriteEnable>false</WriteEnable>
            <Encrypt>false</Encrypt>
            <DataSelect>SPI_MOSI_MISO_DATA_0_1</DataSelect>
            <MemoryConfigsPath>default_memory.xml</MemoryConfigsPath>
            <ConfigDataInFlash>false</ConfigDataInFlash>
        </SlotConfig>
        <SlotConfig>
            <SlaveSlot>2</SlaveSlot>
            <PartNumber>Not used</PartNumber>
            <MemoryMapped>false</MemoryMapped>
            <DualQuad>None</DualQuad>
            <StartAddress>0x18020000</StartAddress>
            <Size>0x10000</Size>
            <EndAddress>0x1802FFFF</EndAddress>
            <WriteEnable>false</WriteEnable>
            <Encrypt>false</Encrypt>
            <DataSelect>SPI_MOSI_MISO_DATA_0_1</DataSelect>
            <MemoryConfigsPath>default_memory.xml</MemoryConfigsPath>
            <ConfigDataInFlash>false</ConfigDataInFlash>
        </SlotConfig>
        <SlotConfig>
            <SlaveSlot>3</SlaveSlot>
            <PartNumber>Not used</PartNumber>
            <MemoryMapped>false</MemoryMapped>
            <DualQuad>None</DualQuad>
            <StartAddress>0x18030000</StartAddress>
            <Size>0x10000</Size>
            <EndAddress>0x1803FFFF</EndAddress>
            <WriteEnable>false</WriteEnable>
            <Encrypt>false</Encrypt>
            <DataSelect>SPI_MOSI_MISO_DATA_0_1</DataSelect>
            <MemoryConfigsPath>default_memory.xml</MemoryConfigsPath>
            <ConfigDataInFlash>false</ConfigDataInFlash>
        </SlotConfig>
    </SlotConfigs>
</CySMIFConfiguration>

QSPI_CONFIG_END

*******************************************************************************/

#ifndef CYCFG_QSPI_MEMSLOT_H
#define CYCFG_QSPI_MEMSLOT_H
#include "cy_smif_memslot.h"

#define CY_SMIF_DEVICE_NUM 1

extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_readCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_writeEnCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_writeDisCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_eraseCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_chipEraseCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_programCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_readStsRegQeCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_readStsRegWipCmd;
extern const cy_stc_smif_mem_cmd_t S25FL128S_SlaveSlot_0_writeStsRegQeCmd;

extern const cy_stc_smif_mem_device_cfg_t deviceCfg_S25FL128S_SlaveSlot_0;

extern const cy_stc_smif_mem_config_t S25FL128S_SlaveSlot_0;

extern const cy_stc_smif_mem_config_t* const smifMemConfigs[CY_SMIF_DEVICE_NUM];

extern const cy_stc_smif_block_config_t smifBlockConfig;


#endif /*CY_SMIF_MEMCONFIG_H*/

