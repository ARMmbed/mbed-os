/* mbed Microcontroller Library
 * Copyright (c) 2018, ARM Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_MBED_QSPI_DEVICE_H_
#define _FSL_MBED_QSPI_DEVICE_H_

#include "fsl_qspi.h"

#define FLASH_SIZE            0x01000000U
#define FLASH_PAGE_SIZE       256U

qspi_flash_config_t single_config = {
    .flashA1Size = FLASH_SIZE, /* 16MB */
    .flashA2Size = 0,
#if defined(FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE) && (FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE)
    .flashB1Size = FLASH_SIZE,
    .flashB2Size = 0,
#endif
#if !defined(FSL_FEATURE_QSPI_HAS_NO_TDH) || (!FSL_FEATURE_QSPI_HAS_NO_TDH)
    .dataHoldTime = 0,
#endif
    .CSHoldTime = 0,
    .CSSetupTime = 0,
    .cloumnspace = 0,
    .dataLearnValue = 0,
    .endian = kQSPI_64LittleEndian,
    .enableWordAddress = false
};

/* Pre-defined LUT definitions */
uint32_t lut[FSL_FEATURE_QSPI_LUT_DEPTH] =
    {
        /* Seq0 : Read */
        /* CMD:   0x03 - Read, Single pad */
        /* ADDR:  0x18 - 24bit address, Single pad */
        /* READ:  0x80 - Read 128 bytes, Single pad */
        /* JUMP_ON_CS: 0 */
        [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x03, QSPI_ADDR, QSPI_PAD_1, 0x18),
        [1] = QSPI_LUT_SEQ(QSPI_READ, QSPI_PAD_1, 0x80, QSPI_JMP_ON_CS, QSPI_PAD_1, 0x0),

        /* Seq1: Page Program */
        /* CMD:    0x02 - Page Program, Single pad */
        /* ADDR:   0x18 - 24bit address, Single pad */
        /* WRITE:  0x80 - Write 128 bytes at one pass, Single pad */
        [4] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x02, QSPI_ADDR, QSPI_PAD_1, 0x18),
        [5] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_1, 0x80, 0, 0, 0),

        /* Match MISRA rule */
        [63] = 0
    };

#endif /* _FSL_MBED_QSPI_DEVICE_H_*/
