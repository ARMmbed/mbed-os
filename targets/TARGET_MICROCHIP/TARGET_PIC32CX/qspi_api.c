/***************************************************************************//**
 * @file qspi_api.c
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_QSPI

#include "stddef.h"
#include "qspi_api.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "pic32cx.h"
#include "qspia.h"
#include "matrix.h"

#define SUPPORTED_ALT_SIZE 8u

struct qspi_inst_frame_t *qspi_instance;

static qspi_status_t qspi_set_frame_width(struct qspi_inst_frame_t *inst, const qspi_command_t *cmd)
{
    /* Check Address */
    if (cmd->address.disabled) {
        /* No address */
        inst->inst_frame.bm.b_addr_en = false;
        inst->inst_frame.bm.b_addr_len = 0;
        inst->inst_frame.bm.b_opt_en = false;
        inst->inst_frame.bm.b_opt_len = 0;
        if (cmd->instruction.bus_width == QSPI_CFG_BUS_SINGLE) {
            /* I: QSPI_CFG_BUS_SINGLE */
            inst->inst_frame.bm.b_width = QSPI_IFR_WIDTH_SINGLE_BIT_SPI;
        } else if (cmd->instruction.bus_width == QSPI_CFG_BUS_DUAL) {
            /* I: QSPI_CFG_BUS_DUAL */
            inst->inst_frame.bm.b_width = QSPI_IFR_WIDTH_DUAL_CMD;
        } else {
            /* I: QSPI_CFG_BUS_QUAD */
            inst->inst_frame.bm.b_width = QSPI_IFR_WIDTH_QUAD_CMD;
        }
    } else {
        inst->inst_frame.bm.b_addr_en = true;
        inst->inst_frame.bm.b_addr_len = cmd->address.size;
        inst->addr = cmd->address.value;
        if (cmd->instruction.bus_width == QSPI_CFG_BUS_SINGLE) {
            if (cmd->address.bus_width == QSPI_CFG_BUS_SINGLE) {
                if (cmd->data.bus_width == QSPI_CFG_BUS_SINGLE) {
                    /* I: QSPI_CFG_BUS_SINGLE - A: QSPI_CFG_BUS_SINGLE  - D: QSPI_CFG_BUS_SINGLE */
                    inst->inst_frame.bm.b_width = QSPI_IFR_SINGLE_BIT_SPI;
                } else if (cmd->data.bus_width == QSPI_CFG_BUS_DUAL) {
                    /* I: QSPI_CFG_BUS_SINGLE - A: QSPI_CFG_BUS_SINGLE  - D: QSPI_CFG_BUS_DUAL */
                    inst->inst_frame.bm.b_width = QSPI_IFR_DUAL_OUTPUT;
                } else {
                    /* I: QSPI_CFG_BUS_SINGLE - A: QSPI_CFG_BUS_SINGLE  - D: QSPI_CFG_BUS_QUAD */
                    inst->inst_frame.bm.b_width = QSPI_IFR_QUAD_OUTPUT;
                }
            } else if (cmd->address.bus_width == QSPI_CFG_BUS_DUAL) {
                /* I: QSPI_CFG_BUS_SINGLE - A: QSPI_CFG_BUS_DUAL */
                inst->inst_frame.bm.b_width = QSPI_IFR_DUAL_IO;
            } else {
                /* I: QSPI_CFG_BUS_SINGLE - A: QSPI_CFG_BUS_QUAD */
                inst->inst_frame.bm.b_width = QSPI_IFR_QUAD_IO;
            }

        } else if (cmd->instruction.bus_width == QSPI_CFG_BUS_DUAL) {
            if (cmd->address.bus_width != QSPI_CFG_BUS_DUAL) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            /* I: QSPI_CFG_BUS_DUAL - A: QSPI_CFG_BUS_DUAL */
            inst->inst_frame.bm.b_width = QSPI_IFR_DUAL_CMD;
        } else {
            if (cmd->address.bus_width != QSPI_CFG_BUS_QUAD) {
                return QSPI_STATUS_INVALID_PARAMETER;
            }
            /* I: QSPI_CFG_BUS_QUAD - A: QSPI_CFG_BUS_QUAD */
            inst->inst_frame.bm.b_width = QSPI_IFR_QUAD_CMD;
        }

        if (cmd->alt.disabled) {
            inst->inst_frame.bm.b_opt_en = false;
            inst->inst_frame.bm.b_opt_len = 0;
        } else {
            inst->inst_frame.bm.b_opt_en = true;
            /* It's not able to set more than 8 bits as option value */
            inst->inst_frame.bm.b_opt_len = QSPI_IFR_OPTION_8_BIT;
        }
    }

    return QSPI_STATUS_OK;
}

static qspi_status_t qspi_smm_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    struct qspid_t qspid;

    qspid.qspi_hw = obj->qspi;
    qspid.qspi_frame = qspi_instance;

    qspid.qspi_command.instruction = command->instruction.value;
    qspid.qspi_command.option = command->alt.value;

    qspid.qspi_buffer.data_tx = (uint8_t *)data;
    qspid.qspi_buffer.tx_data_size = *(uint32_t *)length;
    qspid.qspi_buffer.data_rx = 0;
    qspid.qspi_buffer.rx_data_size = 0;

    qspi_instance->inst_frame.bm.b_inst_en = 1;
    /* Configure memory Array Access */
	qspi_instance->inst_frame.bm.b_tfr_type = 1;
	qspi_instance->inst_frame.bm.b_smrm = 0;
    /* Access Mode */
    qspi_instance->inst_frame.bm.b_data_en = 1;
    qspi_instance->inst_frame.bm.b_apb_tfr_type = 0;

    /* Set Frame Witdth */
    if (qspi_set_frame_width(qspi_instance, command) != QSPI_STATUS_OK) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    qspi_instance->inst_frame.bm.b_continues_read = false;
    qspi_instance->inst_frame.bm.b_ddr_en = false;
    qspi_instance->inst_frame.bm.b_dummy_cycles = command->dummy_count;
    qspi_instance->inst_frame.bm.b_dqs_en = false;
    qspi_instance->inst_frame.bm.b_ddr_cmd_en = false;
    qspi_instance->inst_frame.bm.b_prot_type = 0;

    qspi_flash_access_memory(&qspid, QSPI_WRITE_ACCESS, 0, 0);

    return QSPI_STATUS_OK;
}

static qspi_status_t qspi_smm_read(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    struct qspid_t qspid;

    qspid.qspi_hw = obj->qspi;
    qspid.qspi_frame = qspi_instance;

    qspid.qspi_command.instruction = command->instruction.value;
    qspid.qspi_command.option = command->alt.value;

    qspid.qspi_buffer.data_tx = 0;
    qspid.qspi_buffer.tx_data_size = 0;
    qspid.qspi_buffer.data_rx = (uint8_t *)data;
    qspid.qspi_buffer.rx_data_size = *(uint32_t *)length;

    qspi_instance->inst_frame.bm.b_inst_en = 1;
    /* Configure memory Array Access */
	qspi_instance->inst_frame.bm.b_tfr_type = 1;
	qspi_instance->inst_frame.bm.b_smrm = 0;
    /* Access Mode */
    qspi_instance->inst_frame.bm.b_data_en = 1;
    qspi_instance->inst_frame.bm.b_apb_tfr_type = 0;

    /* Set Frame Witdth */
    if (qspi_set_frame_width(qspi_instance, command) != QSPI_STATUS_OK) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    qspi_instance->inst_frame.bm.b_continues_read = false;
    qspi_instance->inst_frame.bm.b_ddr_en = false;
    qspi_instance->inst_frame.bm.b_dummy_cycles = command->dummy_count;
    qspi_instance->inst_frame.bm.b_dqs_en = false;
    qspi_instance->inst_frame.bm.b_ddr_cmd_en = false;
    qspi_instance->inst_frame.bm.b_prot_type = 0;

    qspi_flash_access_memory(&qspid, QSPI_READ_ACCESS, 0, 0);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_init(qspi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, uint32_t hz, uint8_t mode)
{
    struct qspi_config_t qspi_config;
    uint32_t sysclk, div;

    if (hz <= 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    if (mode != 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    QSPIName qspi_io0 = (QSPIName) pinmap_peripheral(io0, PinMap_QSPI_IO0);
    QSPIName qspi_io1 = (QSPIName) pinmap_peripheral(io1, PinMap_QSPI_IO1);
    QSPIName qspi_io2 = (QSPIName) pinmap_peripheral(io2, PinMap_QSPI_IO2);
    QSPIName qspi_io3 = (QSPIName) pinmap_peripheral(io3, PinMap_QSPI_IO3);
    QSPIName qspi_sclk = (QSPIName) pinmap_peripheral(sclk, PinMap_QSPI_SCLK);
    QSPIName qspi_ssel = (QSPIName) pinmap_peripheral(ssel, PinMap_QSPI_CS0);
    QSPIName qspi_data = (QSPIName) pinmap_merge(qspi_io0, qspi_io1);

    qspi_data = (QSPIName) pinmap_merge(qspi_data, qspi_io2);
    qspi_data = (QSPIName) pinmap_merge(qspi_data, qspi_io3);

    QSPIName qspi_ctrl = (QSPIName) pinmap_merge(qspi_sclk, qspi_data);

    obj->qspi = (Qspi0 *) pinmap_merge(qspi_data, qspi_ctrl);
    if (obj->qspi != (Qspi0 *)QSPI_0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    obj->io0 = io0;
    obj->io1 = io1;
    obj->io2 = io2;
    obj->io3 = io3;
    obj->ssel = ssel;
    obj->sclk = sclk;

    pinmap_pinout(io0, PinMap_QSPI_IO0);
    pinmap_pinout(io1, PinMap_QSPI_IO1);
    pinmap_pinout(io2, PinMap_QSPI_IO2);
    pinmap_pinout(io3, PinMap_QSPI_IO3);
    pinmap_pinout(sclk, PinMap_QSPI_SCLK);
    pinmap_pinout(ssel, PinMap_QSPI_CS0);

    /* Get QSPI memory mode configure */
    qspi_get_config_default(&qspi_config);

    /* Configure Peripheral CLK and Generic CLK by default */
    sysclk = sysclk_get_main_hz();
    div = sysclk / hz;
    if (sysclk % hz) {
        div++;
    }
	pmc_configure_generic(ID_QSPI, PMC_PCR_GCLKCSS_MCK0, PMC_PCR_GCLKDIV(div-1));
	pmc_configure_peripheral(ID_QSPI, PMC_PCR_DIV_DIV1);

    /* Set Mode */
    qspi_set_clock_polarity(obj->qspi, 0);
    qspi_set_clock_phase(obj->qspi, 0);
    qspi_config.serial_memory_mode = QSPI_MEM_MODE;

	/* Enable Generick Clock */
	pmc_enable_generic_clk(ID_QSPI);
	/* Enable Peripheral clock */
	pmc_enable_periph_clk(ID_QSPI);

    /* Configure MATRIX to provide access to QSPI in mem mode for full range */
    /* Adapt next line depending on memory size to handle */
	matrix_set_slave_protected_region_top(SLAVE_MATRIX1_ID_QSPI, MATRIX_SIZE_8MB);

    /* Init QSPI instance */
    qspi_instance = (struct qspi_inst_frame_t *)malloc(sizeof(struct qspi_inst_frame_t));
	memset(qspi_instance, 0, sizeof(struct qspi_inst_frame_t));

    /* QSPI memory mode configure */
    qspi_initialize(obj->qspi, &qspi_config);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_free(qspi_t *obj)
{
    qspi_disable(obj->qspi);
	qspi_reset(obj->qspi);

    /* Disable Generick Clock */
	pmc_disable_generic_clk(ID_QSPI);
	/* Disable Peripheral clock */
	pmc_disable_periph_clk(ID_QSPI);

    /* Free QSPI instance */
    free(qspi_instance);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_frequency(qspi_t *obj, int hz)
{
    uint32_t sysclk;
    uint32_t div;

    if (hz <= 0) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    qspi_disable(obj->qspi);

    /* Disable Generick Clock */
	pmc_disable_generic_clk(ID_QSPI);
	/* Disable Peripheral clock */
	pmc_disable_periph_clk(ID_QSPI);

    sysclk = sysclk_get_main_hz();
    div = sysclk / hz;
    if (sysclk % hz) {
        div++;
    }

    /* Configure Generic CLK */
	pmc_configure_generic(ID_QSPI, PMC_PCR_GCLKCSS_MCK0, PMC_PCR_GCLKDIV(div-1));

	/* Enable Generick Clock */
	pmc_enable_generic_clk(ID_QSPI);
	/* Enable Peripheral clock */
	pmc_enable_periph_clk(ID_QSPI);

    /* Enable QSPI */
	qspi_enable(obj->qspi);

    return QSPI_STATUS_OK;
}

qspi_status_t qspi_write(qspi_t *obj, const qspi_command_t *command, const void *data, size_t *length)
{
    if ((length == 0) || (data == 0)) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* QSPI Serial Memory mode */
    return qspi_smm_write(obj, command, data, length);
}

qspi_status_t qspi_read(qspi_t *obj, const qspi_command_t *command, void *data, size_t *length)
{
    if ((length == 0) || (data == 0)) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    /* QSPI Serial Memory mode */
    return qspi_smm_read(obj, command, data, length);
}

qspi_status_t qspi_command_transfer(qspi_t *obj, const qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size)
{
    struct qspid_t qspid;

    qspid.qspi_hw = obj->qspi;

    qspid.qspi_command.instruction = command->instruction.value;
    qspid.qspi_command.option = command->alt.value;

    qspid.qspi_frame = qspi_instance;

	qspi_instance->inst_frame.bm.b_inst_en = 1;
    /* Configure Registers/Commands Access */
	qspi_instance->inst_frame.bm.b_tfr_type = 0;
	qspi_instance->inst_frame.bm.b_smrm = 1;

    qspid.qspi_buffer.data_tx = (uint8_t *)tx_data;
    qspid.qspi_buffer.tx_data_size = tx_size;
    qspid.qspi_buffer.data_rx = (uint8_t *)rx_data;
    qspid.qspi_buffer.rx_data_size = rx_size;

    if ((tx_data != NULL) && tx_size) {
        qspi_instance->inst_frame.bm.b_data_en = 1;
        qspi_instance->inst_frame.bm.b_apb_tfr_type = 0;
    } else if ((rx_data != NULL) && rx_size) {
        qspi_instance->inst_frame.bm.b_data_en = 1;
        qspi_instance->inst_frame.bm.b_apb_tfr_type = 1;
    } else {
        qspi_instance->inst_frame.bm.b_data_en = 0;
        qspi_instance->inst_frame.bm.b_apb_tfr_type = 0;
    }

    /* Set Frame Witdth */
    if (qspi_set_frame_width(qspi_instance, command) != QSPI_STATUS_OK) {
        return QSPI_STATUS_INVALID_PARAMETER;
    }

    qspi_instance->inst_frame.bm.b_continues_read = false;
    qspi_instance->inst_frame.bm.b_ddr_en = false;
    qspi_instance->inst_frame.bm.b_dummy_cycles = command->dummy_count;
    qspi_instance->inst_frame.bm.b_dqs_en = false;
    qspi_instance->inst_frame.bm.b_ddr_cmd_en = false;
    qspi_instance->inst_frame.bm.b_prot_type = 0;

    qspi_flash_execute_command(&qspid, QSPI_CMD_ACCESS);

    return QSPI_STATUS_OK;
}

const PinMap *qspi_master_sclk_pinmap()
{
    return PinMap_QSPI_SCLK;
}

const PinMap *qspi_master_ssel_pinmap()
{
    return PinMap_QSPI_CS0;
}

const PinMap *qspi_master_data0_pinmap()
{
    return PinMap_QSPI_IO0;
}

const PinMap *qspi_master_data1_pinmap()
{
    return PinMap_QSPI_IO1;
}

const PinMap *qspi_master_data2_pinmap()
{
    return PinMap_QSPI_IO2;
}

const PinMap *qspi_master_data3_pinmap()
{
    return PinMap_QSPI_IO3;
}

#endif /* DEVICE_QSPI */
