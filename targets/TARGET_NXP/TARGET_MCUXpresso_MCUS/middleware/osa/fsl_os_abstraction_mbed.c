/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * This is the source file for the OS Abstraction layer for freertos.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "fsl_common.h"
#include "fsl_os_abstraction_mbed.h"

void OSA_EnterCritical(uint32_t *sr)
{
    core_util_critical_section_enter();
}

void OSA_ExitCritical(uint32_t sr)
{
    core_util_critical_section_exit();
}

