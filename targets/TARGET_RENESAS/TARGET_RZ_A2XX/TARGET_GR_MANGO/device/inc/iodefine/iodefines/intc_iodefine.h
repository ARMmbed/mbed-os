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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
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
* Rev: 3.01
* Description : IO define header
*******************************************************************************/

#ifndef INTC_IODEFINE_H
#define INTC_IODEFINE_H

struct st_intc {
    union {
        unsigned long LONG;
        struct {
            unsigned long EnableGrp0: 1;
            unsigned long EnableGrp1: 1;
            unsigned long : 30;
        } BIT;
    } GICD_CTLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITLinesNumber: 5;
            unsigned long CPUNumber: 3;
            unsigned long : 2;
            unsigned long SecurityExtn: 1;
            unsigned long LSPI: 5;
            unsigned long : 16;
        } BIT;
    } GICD_TYPER;
    union {
        unsigned long LONG;
        struct {
            unsigned long Implementer: 12;
            unsigned long Revision: 4;
            unsigned long Variant: 4;
            unsigned long : 4;
            unsigned long ProductID: 8;
        } BIT;
    } GICD_IIDR;
    char           wk0[116];
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Securitystatusbits: 32;
        } BIT;
    } GICD_IGROUPR15;
    char           wk1[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_enablebits: 32;
        } BIT;
    } GICD_ISENABLER15;
    char           wk2[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_enablebits: 32;
        } BIT;
    } GICD_ICENABLER15;
    char           wk3[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Set_pendingbits: 32;
        } BIT;
    } GICD_ISPENDR15;
    char           wk4[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Clear_pendingbits: 32;
        } BIT;
    } GICD_ICPENDR15;
    char           wk5[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ISACTIVER15;
    char           wk6[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Activebits: 32;
        } BIT;
    } GICD_ICACTIVER15;
    char           wk7[64];
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR15;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR16;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR17;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR18;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR19;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR20;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR21;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR22;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR23;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR24;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR25;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR26;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR27;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR28;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR29;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR30;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR31;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR32;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR33;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR34;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR35;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR36;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR37;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR38;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR39;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR40;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR41;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR42;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR43;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR44;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR45;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR46;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR47;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR48;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR49;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR50;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR51;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR52;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR53;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR54;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR55;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR56;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR57;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR58;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR59;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR60;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR61;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR62;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR63;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR64;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR65;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR66;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR67;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR68;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR69;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR70;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR71;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR72;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR73;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR74;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR75;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR76;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR77;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR78;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR79;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR80;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR81;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR82;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR83;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR84;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR85;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR86;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR87;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR88;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR89;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR90;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR91;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR92;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR93;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR94;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR95;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR96;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR97;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR98;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR99;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR100;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR101;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR102;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR103;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR104;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR105;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR106;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR107;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR108;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR109;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR110;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR111;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR112;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR113;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR114;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR115;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR116;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR117;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR118;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR119;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR120;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR121;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR122;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR123;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR124;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR125;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR126;
    union {
        unsigned long LONG;
        struct {
            unsigned long Prioritybyteoffset0: 8;
            unsigned long Prioritybyteoffset1: 8;
            unsigned long Prioritybyteoffset2: 8;
            unsigned long Prioritybyteoffset3: 8;
        } BIT;
    } GICD_IPRIORITYR127;
    char           wk8[512];
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR14;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR15;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR16;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR17;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR18;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR19;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR20;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR21;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR22;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR23;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR24;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR25;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR26;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR27;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR28;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR29;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR30;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR31;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR32;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR33;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR34;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR35;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR36;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR37;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR38;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR39;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR40;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR41;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR42;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR43;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR44;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR45;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR46;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR47;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR48;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR49;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR50;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR51;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR52;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR53;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR54;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR55;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR56;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR57;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR58;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR59;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR60;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR61;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR62;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR63;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR64;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR65;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR66;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR67;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR68;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR69;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR70;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR71;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR72;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR73;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR74;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR75;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR76;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR77;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR78;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR79;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR80;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR81;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR82;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR83;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR84;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR85;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR86;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR87;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR88;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR89;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR90;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR91;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR92;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR93;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR94;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR95;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR96;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR97;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR98;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR99;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR100;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR101;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR102;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR103;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR104;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR105;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR106;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR107;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR108;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR109;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR110;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR111;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR112;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR113;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR114;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR115;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR116;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR117;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR118;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR119;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR120;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR121;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR122;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR123;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR124;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR125;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR126;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPUtargetsbyteoffset0: 8;
            unsigned long CPUtargetsbyteoffset1: 8;
            unsigned long CPUtargetsbyteoffset2: 8;
            unsigned long CPUtargetsbyteoffset3: 8;
        } BIT;
    } GICD_ITARGETR127;
    char           wk9[512];
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR14;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR15;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR16;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR17;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR18;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR19;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR20;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR21;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR22;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR23;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR24;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR25;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR26;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR27;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR28;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR29;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR30;
    union {
        unsigned long LONG;
        struct {
            unsigned long Int_config: 32;
        } BIT;
    } GICD_ICFGR31;
    char           wk10[128];
    union {
        unsigned long LONG;
        struct {
            unsigned long ppi_status: 16;
            unsigned long : 16;
        } BIT;
    } GICD_PPISR;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR12;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR13;
    union {
        unsigned long LONG;
        struct {
            unsigned long spi_status: 32;
        } BIT;
    } GICD_SPISR14;
    char           wk11[448];
    union {
        unsigned long LONG;
        struct {
            unsigned long SGIINTID: 4;
            unsigned long : 11;
            unsigned long NSATT: 1;
            unsigned long CPUTargetList: 8;
            unsigned long TargetListFilter: 2;
            unsigned long : 6;
        } BIT;
    } GICD_SGIR;
    char           wk12[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI0_Clers_pending: 8;
            unsigned long SGI1_Clers_pending: 8;
            unsigned long SGI2_Clers_pending: 8;
            unsigned long SGI3_Clers_pending: 8;
        } BIT;
    } GICD_CPENDSGIR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI4_Clers_pending: 8;
            unsigned long SGI5_Clers_pending: 8;
            unsigned long SGI6_Clers_pending: 8;
            unsigned long SGI7_Clers_pending: 8;
        } BIT;
    } GICD_CPENDSGIR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI8_Clers_pending: 8;
            unsigned long SGI9_Clers_pending: 8;
            unsigned long SGI10_Clers_pending: 8;
            unsigned long SGI11_Clers_pending: 8;
        } BIT;
    } GICD_CPENDSGIR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI12_Clers_pending: 8;
            unsigned long SGI13_Clers_pending: 8;
            unsigned long SGI14_Clers_pending: 8;
            unsigned long SGI15_Clers_pending: 8;
        } BIT;
    } GICD_CPENDSGIR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI0_Set_pending: 8;
            unsigned long SGI1_Set_pending: 8;
            unsigned long SGI2_Set_pending: 8;
            unsigned long SGI3_Set_pending: 8;
        } BIT;
    } GICD_SPENDSGIR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI4_Set_pending: 8;
            unsigned long SGI5_Set_pending: 8;
            unsigned long SGI6_Set_pending: 8;
            unsigned long SGI7_Set_pending: 8;
        } BIT;
    } GICD_SPENDSGIR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI8_Set_pending: 8;
            unsigned long SGI9_Set_pending: 8;
            unsigned long SGI10_Set_pending: 8;
            unsigned long SGI11_Set_pending: 8;
        } BIT;
    } GICD_SPENDSGIR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SGI12_Set_pending: 8;
            unsigned long SGI13_Set_pending: 8;
            unsigned long SGI14_Set_pending: 8;
            unsigned long SGI15_Set_pending: 8;
        } BIT;
    } GICD_SPENDSGIR3;
    char           wk13[160];
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_CCfield: 4;
            unsigned long ARM_Reserved: 4;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_Reserved: 8;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_Reserved: 8;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_Reserved: 8;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_DIDfield: 8;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_DIDfield: 4;
            unsigned long ARM_AIDfield: 4;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_AIDfield: 3;
            unsigned long ARM_UJEPfield: 1;
            unsigned long Arfield: 4;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_Reserved: 4;
            unsigned long ARM_Rfield: 4;
            unsigned long : 24;
        } BIT;
    } GICD_PIDR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_FVPCD: 8;
            unsigned long : 24;
        } BIT;
    } GICD_CIDR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_FVPCD: 8;
            unsigned long : 24;
        } BIT;
    } GICD_CIDR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_FVPCD: 8;
            unsigned long : 24;
        } BIT;
    } GICD_CIDR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long ARM_FVPCD: 8;
            unsigned long : 24;
        } BIT;
    } GICD_CIDR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long EnableGrp0: 1;
            unsigned long EnableGrp1: 1;
            unsigned long AckCtl: 1;
            unsigned long FIQEn: 1;
            unsigned long CBPR: 1;
            unsigned long FIQBypDisGrp0: 1;
            unsigned long IRQBypDisGrp0: 1;
            unsigned long FIQBypDisGrp1: 1;
            unsigned long IRQBypDisGrp1: 1;
            unsigned long EOImodeS: 1;
            unsigned long EOImodeNS: 1;
            unsigned long : 21;
        } BIT;
    } GICC_CTLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long Priority: 8;
            unsigned long : 24;
        } BIT;
    } GICC_PMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long Binarypoint: 3;
            unsigned long : 29;
        } BIT;
    } GICC_BPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long InterruptID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_IAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long EOIINTID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_EOIR;
    union {
        unsigned long LONG;
        struct {
            unsigned long Priority: 8;
            unsigned long : 24;
        } BIT;
    } GICC_RPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long PENDINTID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_HPPIR;
    union {
        unsigned long LONG;
        struct {
            unsigned long Binarypoint: 3;
            unsigned long : 29;
        } BIT;
    } GICC_ABPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long InterruptID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_AIAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long InterruptID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_AEOIR;
    union {
        unsigned long LONG;
        struct {
            unsigned long PENDINTID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_AHPPIR;
    char           wk14[164];
    union {
        unsigned long LONG;
        struct {
            unsigned long AP_Group0: 32;
        } BIT;
    } GICC_APR0;
    char           wk15[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long AP_Group1: 16;
            unsigned long AP_Group0: 16;
        } BIT;
    } GICC_NSAPR0;
    char           wk16[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long Implementer: 12;
            unsigned long Revision: 4;
            unsigned long Architectureversion: 4;
            unsigned long ProductID: 12;
        } BIT;
    } GICC_IIDR;
    char           wk17[3840];
    union {
        unsigned long LONG;
        struct {
            unsigned long InterruptID: 10;
            unsigned long CPUID: 3;
            unsigned long : 19;
        } BIT;
    } GICC_DIR;
    char           wk18[350013436];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 1;
            unsigned short NMIF: 1;
            unsigned short : 6;
            unsigned short NMIE: 1;
            unsigned short : 6;
            unsigned short NMIL: 1;
        } BIT;
    } ICR0;
    union {
        unsigned short WORD;
        struct {
            unsigned short IRQ00S: 1;
            unsigned short IRQ01S: 1;
            unsigned short IRQ10S: 1;
            unsigned short IRQ11S: 1;
            unsigned short IRQ20S: 1;
            unsigned short IRQ21S: 1;
            unsigned short IRQ30S: 1;
            unsigned short IRQ31S: 1;
            unsigned short IRQ40S: 1;
            unsigned short IRQ41S: 1;
            unsigned short IRQ50S: 1;
            unsigned short IRQ51S: 1;
            unsigned short IRQ60S: 1;
            unsigned short IRQ61S: 1;
            unsigned short IRQ70S: 1;
            unsigned short IRQ71S: 1;
        } BIT;
    } ICR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short IRQ0F: 1;
            unsigned short IRQ1F: 1;
            unsigned short IRQ2F: 1;
            unsigned short IRQ3F: 1;
            unsigned short IRQ4F: 1;
            unsigned short IRQ5F: 1;
            unsigned short IRQ6F: 1;
            unsigned short IRQ7F: 1;
            unsigned short : 7;
            unsigned short IRQMSK: 1;
        } BIT;
    } IRQRR;
    char           wk19[2];
    char           wk20[2];
    char           wk21[6];
    union {
        unsigned short WORD;
        struct {
            unsigned short DP: 1;
            unsigned short DM: 1;
            unsigned short : 6;
            unsigned short VBUSIN: 1;
            unsigned short OVRCLR: 1;
            unsigned short CC1_RD: 1;
            unsigned short CC2_RD: 1;
            unsigned short : 4;
        } BIT;
    } SSTBCCR0;
    union {
        unsigned short WORD;
        struct {
            unsigned short DP: 1;
            unsigned short DM: 1;
            unsigned short : 6;
            unsigned short VBUSIN: 1;
            unsigned short OVRCLR: 1;
            unsigned short CC1_RD: 1;
            unsigned short CC2_RD: 1;
            unsigned short : 4;
        } BIT;
    } SSTBCCR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short DP: 1;
            unsigned short DM: 1;
            unsigned short : 6;
            unsigned short VBUSIN: 1;
            unsigned short OVRCLR: 1;
            unsigned short CC1_RD: 1;
            unsigned short CC2_RD: 1;
            unsigned short : 4;
        } BIT;
    } SSTBCRR0;
    union {
        unsigned short WORD;
        struct {
            unsigned short DP: 1;
            unsigned short DM: 1;
            unsigned short : 6;
            unsigned short VBUSIN: 1;
            unsigned short OVRCLR: 1;
            unsigned short CC1_RD: 1;
            unsigned short CC2_RD: 1;
            unsigned short : 4;
        } BIT;
    } SSTBCRR1;
};

#define INTC      (*(volatile struct st_intc      *)0xE8221000)

#endif
