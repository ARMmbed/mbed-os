/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/* Copyright (c) 2019-2020 Renesas Electronics Corporation.
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
 */
/*******************************************************************************
* File Name   : octaram_init.c
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "mbed_drv_cfg.h"


#if(1) /******* Provisional (Remove this process when the bootloader is complete) ****** */
#define OCTAINF_33MHZ   1
#define OCTAINF_66MHZ   2
#define OCTAINF_132MHZ  3

#define OCTAINF_XXMHZ   OCTAINF_132MHZ

#if defined(__ICCARM__)
#define RAM_CODE_SEC    __ramfunc
#else
#define RAM_CODE_SEC    __attribute__((section("RAM_CODE")))
#endif

static RAM_CODE_SEC void octa_spi_wren(void);
static RAM_CODE_SEC void octa_spi_wrcr2(uint32_t addr, uint32_t data);

RAM_CODE_SEC void OctaFlash_Init(void);

void OctaFlash_Init(void)
{
    volatile uint8_t dummy8;

    /* ---- STBCR9 Setting ---- */
    CPG.STBCR9.BIT.MSTP92 = 0;           // Octa Memory Controller runs
    dummy8 = CPG.STBCR9.BYTE;
    (void)dummy8;

    /* ---- SCLKSEL Setting ---- */
    /*  OCTCR: 11b -> G/2phy    -> 132MHz   */
    /*  OCTCR: 10b -> B/2phy    -> 66MHz    */
    /*  OCTCR: 01b -> P1/2phy   -> 33MHz    */
    /*  OCTCR: 00b -> P0/2phy   -> 16MHz    */
#if (OCTAINF_XXMHZ == OCTAINF_33MHZ)
    CPG.SCLKSEL.BIT.OCTCR = 1;           // Octa clock P1
#elif (OCTAINF_XXMHZ == OCTAINF_66MHZ)
    CPG.SCLKSEL.BIT.OCTCR = 2;           // Octa clock B
#else // (OCTAINF_XXMHZ == OCTAINF_132MHZ)
    CPG.SCLKSEL.BIT.OCTCR = 3;           // Octa clock G
#endif

    /* ---- PHMOM0 Setting ---- */
    GPIO.PHMOM0.BIT.HOSEL = 1;      /* Select Octa Memory Controller */


    // ---------------------------------------
    // OctaFlash
    // ---------------------------------------
    /* ---- Device size register 0 ---- */
    OCTA.DSR0.BIT.DV0TYP = 0;
    OCTA.DSR0.BIT.DV0SZ  = OCTAFLASH_SIZE;

    /* ---- Set Dummy Cycle as 6 cycles ---- */
    octa_spi_wren();
#if (OCTAINF_XXMHZ == OCTAINF_33MHZ)
    octa_spi_wrcr2(0x00000300, 0x7); // 6 dummy cycles @33MHz
#elif (OCTAINF_XXMHZ == OCTAINF_66MHZ)
    octa_spi_wrcr2(0x00000300, 0x6); // 8 dummy cycles @66MHz
#else // (OCTAINF_XXMHZ == OCTAINF_132MHZ)
    octa_spi_wrcr2(0x00000300, 0x3); // 14 dummy cycles @132MHz
#endif

    /* ---- Set OctaFlash to DOPI mode ---- */
    octa_spi_wren();
    octa_spi_wrcr2(0x00000000, 0x2); // DTR OPI mode

    /* ---- Controller and device setting register ---- */
    OCTA.CDSR.BIT.DLFT    = 1;
    OCTA.CDSR.BIT.DV0TTYP = 2;      // Device0 =DOPI mode

    /* ---- Memory Map read/write command register 0 ---- */
    OCTA.MRWCR0.BIT.D0MWCMD1 = 0x12;
    OCTA.MRWCR0.BIT.D0MWCMD0 = 0xED;
    OCTA.MRWCR0.BIT.D0MRCMD1 = 0xEE;
    OCTA.MRWCR0.BIT.D0MRCMD0 = 0x11;

    /* ---- Memory Map read/write setting register ---- */
    OCTA.MRWCSR.BIT.MWO0  = 1;      // write data order: byte1, byte0, byte3, byte2
    OCTA.MRWCSR.BIT.MWCL0 = 2;      // 2 bytes command @Write
    OCTA.MRWCSR.BIT.MWAL0 = 4;      // 4 bytes address @Write
    OCTA.MRWCSR.BIT.MRO0  = 1;      // read data order: byte1, byte0, byte3, byte2
    OCTA.MRWCSR.BIT.MRCL0 = 2;      // 2 bytes command @Read
    OCTA.MRWCSR.BIT.MRAL0 = 4;      // 4 bytes address @Read

    /* ---- Memory delay trim register ---- */
    OCTA.MDTR.BIT.DQSEDOPI = 0x6;
    OCTA.MDTR.BIT.DV0DEL   = 0x48;

    /* ---- Memory Map dummy length register ---- */
#if (OCTAINF_XXMHZ == OCTAINF_33MHZ)
    OCTA.MDLR.BIT.DV0RDL = 6;       // 6 dummy cycles @33MHz
#elif (OCTAINF_XXMHZ == OCTAINF_66MHZ)
    OCTA.MDLR.BIT.DV0RDL = 8;       // 8 dummy cycles @66MHz
#else // (OCTAINF_XXMHZ == OCTAINF_132MHZ)
    OCTA.MDLR.BIT.DV0RDL = 14;      // 14 dummy cycles @132MHz
#endif

    /* ---- Device chip select timing setting register ---- */
    OCTA.DCSTR.BIT.DVSELLO  = 0;
    OCTA.DCSTR.BIT.DVSELHI  = 5;
    OCTA.DCSTR.BIT.DVSELCMD = 0;

    /* ---- Device Memory Map Write chip select timing setting register ---- */
    OCTA.DWCSTR.BIT.DVWLO0  = 0;
    OCTA.DWCSTR.BIT.DVWHI0  = 2;
    OCTA.DWCSTR.BIT.DVWCMD0 = 1;

    /* ---- Device Memory Map Read chip select timing setting register ---- */
    OCTA.DRCSTR.BIT.DVRDLO0  = 0;
    OCTA.DRCSTR.BIT.DVRDHI0  = 5;
    OCTA.DRCSTR.BIT.DVRDCMD0 = 1;
    OCTA.DRCSTR.BIT.CTR0     = 0;
    OCTA.DRCSTR.BIT.CTRW0    = 0x20;
}

void octa_spi_wren(void)
{
    /* ---- Device command register ---- */
    OCTA.DCR.BIT.DVCMD0 = 0x06;          // Write Enable

    /* ---- Device command setting register ---- */
    OCTA.DCSR.BIT.ACDA   = 0;
    OCTA.DCSR.BIT.DOPI   = 1;
    OCTA.DCSR.BIT.ADLEN  = 0;
    OCTA.DCSR.BIT.DAOR   = 0;
    OCTA.DCSR.BIT.CMDLEN = 1;
    OCTA.DCSR.BIT.ACDV   = 0;
    OCTA.DCSR.BIT.DMLEN  = 0;
    OCTA.DCSR.BIT.DALEN  = 0;

    /* ---- Configure write without data register ---- */
    OCTA.CWNDR = 0x00000000;
}

void octa_spi_wrcr2(uint32_t addr, uint32_t data)
{
    /* ---- Device command register ---- */
    OCTA.DCR.BIT.DVCMD0 = 0x72;          // Read Configuration Register 2

    /* ---- Device address register ---- */
    OCTA.DAR.LONG = addr;

    /* ---- Device command setting register ---- */
    OCTA.DCSR.BIT.ACDA   = 0;
    OCTA.DCSR.BIT.DOPI   = 1;
    OCTA.DCSR.BIT.ADLEN  = 4;
    OCTA.DCSR.BIT.DAOR   = 0;
    OCTA.DCSR.BIT.CMDLEN = 1;
    OCTA.DCSR.BIT.ACDV   = 0;
    OCTA.DCSR.BIT.DMLEN  = 0;
    OCTA.DCSR.BIT.DALEN  = 1;

    /* ---- Configure write data register ---- */
    OCTA.CWDR.LONG = data;
}
#endif

static void octa_ram_mode_register_write(uint16_t config)
{
    /* ---- Device command register ---- */
    OCTA.DCR.BIT.DVCMD1 = 0x40;          // Mode Register Write

    /* ---- Device address register ---- */
    OCTA.DAR.LONG = 0x00040000;

    /* ---- Device command setting register ---- */
    OCTA.DCSR.BIT.ACDA   = 0;
    OCTA.DCSR.BIT.DOPI   = 0;
    OCTA.DCSR.BIT.ADLEN  = 4;
    OCTA.DCSR.BIT.DAOR   = 1;
    OCTA.DCSR.BIT.CMDLEN = 2;
    OCTA.DCSR.BIT.ACDV   = 1;
    OCTA.DCSR.BIT.DMLEN  = 0;
    OCTA.DCSR.BIT.DALEN  = 2;

    /* ---- Configure write data register ---- */
    OCTA.CWDR.LONG = (uint32_t)config;
}

/******************************************************************************
* Function Name: OctaRAM_Init
* Description  :
* Arguments    : none
* Return Value : none
******************************************************************************/
void OctaRAM_Init(void)
{
    volatile uint8_t dummy8;

    CPG.STBCR9.BIT.MSTP92 = 0;
    dummy8 = CPG.STBCR9.BYTE;
    (void)dummy8;

    /* ---- Device size register 1 ---- */
    OCTA.DSR1.BIT.DV1TYP = 1;            // TYPE=RAM
    OCTA.DSR1.BIT.DV1SZ  = OCTARAM_SIZE; // RAM size

    /* ---- Controller and device setting register ---- */
    OCTA.CDSR.BIT.DV1TTYP = 2;           // Device1 =DOPI mode

    /* ---- Memory Map dummy length register ---- */
    OCTA.MDLR.BIT.DV1WDL = 5;            // Device1 Write DUMMY = 5
    OCTA.MDLR.BIT.DV1RDL = 5;            // Device1 Read DUMMY = 5

    /* ---- Memory delay trim register ---- */
    OCTA.MDTR.BIT.DV1DEL  = 40;          // Device1 Delay 40
    OCTA.MDTR.BIT.DQSERAM = 6;           // OM_DQS enable counter

    /* ---- Device Memory Map Read chip select timing setting register ---- */
    OCTA.DRCSTR.BIT.DVRDLO1  = 0;        // Device1 select signal High timing setting = 1.5 clock cycles
    OCTA.DRCSTR.BIT.DVRDHI1  = 5;        // Device1 select signal High timing setting = 6.5 clock cycles
    OCTA.DRCSTR.BIT.DVRDCMD1 = 0;        // Device1 Command execution interval = 2 clock cycles

    /* ---- Device Memory Map Write chip select timing setting register ---- */
    OCTA.DWCSTR.BIT.DVWLO1   = 0;        // Device1 select signal High timing setting = 1.5 clock cycles
    OCTA.DWCSTR.BIT.DVWHI1   = 1;        // Device1 select signal High timing setting = 2.5 clock cycles
    OCTA.DWCSTR.BIT.DVWCMD1  = 0;        // Device1 Command execution interval = 2 clock cycles

    /* ---- Memory Map read/write command register 1 ---- */
    /* Wrap Burst */
    OCTA.MRWCR1.BIT.D1MWCMD1 = 0x00;     // write command
    OCTA.MRWCR1.BIT.D1MRCMD1 = 0x80;     // read command

    /* ---- Memory Map read/write setting register ---- */
    OCTA.MRWCSR.BIT.MWO1  = 1;           // Device1 write order setting = Write order is byte1, byte0, byte3, byte2
    OCTA.MRWCSR.BIT.MWCL1 = 2;           // Device1 write command length setting = 2
    OCTA.MRWCSR.BIT.MWAL1 = 4;           // Device1 write address length setting = 4
    OCTA.MRWCSR.BIT.MRO1  = 1;           // Device1 read order setting = Read order is byte1, byte0, byte3, byte2
    OCTA.MRWCSR.BIT.MRCL1 = 2;           // Device1 read command length setting = 2
    OCTA.MRWCSR.BIT.MRAL1 = 4;           // Device1 read address length setting = 4

    GPIO.PHMOM0.BIT.HOSEL = 1;           // Select Octa Memory Controller

    uint16_t config = 0x8021;            // CR[15]    Deep Power Down        : 1    - Normal
    // CR[14:12] Driver Strength        : 000  - 100ohms
    // CR[11:9]  Reserved               : 000
    // CR[8]     DQSM Read Pre-Cycle    : 0    - 0 clock
    // CR[7:4]   Latency Counter        : 0010 - 5 clocks
    // CR[3]     Initial Access Latency : 0    - Variable Latency
    // CR[2]     CLK2 Input             : 0    - No Support
    // CR[1:0]   Bust Wrap Length       : 01   - 64 bytes
    octa_ram_mode_register_write(config);
}

/* End of File */
