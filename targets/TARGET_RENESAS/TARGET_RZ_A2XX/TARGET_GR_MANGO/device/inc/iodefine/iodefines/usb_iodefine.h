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

#ifndef USB_IODEFINE_H
#define USB_IODEFINE_H

struct st_usb00 {
    unsigned long  HCREVISION;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBSR: 2;
            unsigned long PLE: 1;
            unsigned long IE: 1;
            unsigned long CLE: 1;
            unsigned long BLE: 1;
            unsigned long HCFS: 2;
            unsigned long : 1;
            unsigned long RWC: 1;
            unsigned long : 1;
            unsigned long : 21;
        } BIT;
    } HCCONTROL;
    union {
        unsigned long LONG;
        struct {
            unsigned long HCR: 1;
            unsigned long CLF: 1;
            unsigned long BLF: 1;
            unsigned long : 1;
            unsigned long : 12;
            unsigned long SOC: 2;
            unsigned long : 14;
        } BIT;
    } HCCOMMANDSTATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SO: 1;
            unsigned long WDH: 1;
            unsigned long SF: 1;
            unsigned long RD: 1;
            unsigned long UE: 1;
            unsigned long FNO: 1;
            unsigned long RHSC: 1;
            unsigned long : 23;
            unsigned long : 1;
            unsigned long : 1;
        } BIT;
    } HCINTERRUPTSTATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SOE: 1;
            unsigned long WDHE: 1;
            unsigned long SFE: 1;
            unsigned long RDE: 1;
            unsigned long UEE: 1;
            unsigned long FNOE: 1;
            unsigned long RHSCE: 1;
            unsigned long : 23;
            unsigned long OCE: 1;
            unsigned long MIE: 1;
        } BIT;
    } HCINTERRUPTENABLE;
    union {
        unsigned long LONG;
        struct {
            unsigned long SOD: 1;
            unsigned long WDHD: 1;
            unsigned long SFD: 1;
            unsigned long RDD: 1;
            unsigned long UED: 1;
            unsigned long FNOD: 1;
            unsigned long RHSCD: 1;
            unsigned long : 23;
            unsigned long OCD: 1;
            unsigned long MID: 1;
        } BIT;
    } HCINTERRUPTDISABLE;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long HcHCCA: 24;
        } BIT;
    } HCHCCA;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long PECD: 28;
        } BIT;
    } HCPERIODCURRENTED;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long CHED: 28;
        } BIT;
    } HCCONTROLHEADED;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long CCED: 28;
        } BIT;
    } HCCONTROLCURRENTED;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long BHED: 28;
        } BIT;
    } HCBULKHEADED;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long BCED: 28;
        } BIT;
    } HCBULKCURRENTED;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long DH: 28;
        } BIT;
    } HCDONEHEAD;
    union {
        unsigned long LONG;
        struct {
            unsigned long FI: 14;
            unsigned long : 2;
            unsigned long FSMPS: 15;
            unsigned long FIT: 1;
        } BIT;
    } HCFMINTERVAL;
    union {
        unsigned long LONG;
        struct {
            unsigned long FR: 14;
            unsigned long : 17;
            unsigned long FRT: 1;
        } BIT;
    } HCFMREMAINING;
    union {
        unsigned long LONG;
        struct {
            unsigned long FN: 16;
            unsigned long : 16;
        } BIT;
    } HCFMNUMBER;
    union {
        unsigned long LONG;
        struct {
            unsigned long PS: 14;
            unsigned long : 18;
        } BIT;
    } HCPERIODICSTART;
    union {
        unsigned long LONG;
        struct {
            unsigned long LST: 12;
            unsigned long : 20;
        } BIT;
    } HCLSTHRESHOLD;
    union {
        unsigned long LONG;
        struct {
            unsigned long NDP: 8;
            unsigned long PSM: 1;
            unsigned long NPS: 1;
            unsigned long DT: 1;
            unsigned long OCPM: 1;
            unsigned long NOCP: 1;
            unsigned long : 11;
            unsigned long POTPGT: 8;
        } BIT;
    } HCRHDESCRIPTORA;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long DR: 1;
            unsigned long : 15;
            unsigned long PPCM: 1;
            unsigned long : 14;
        } BIT;
    } HCRHDESCRIPTORB;
    union {
        unsigned long LONG;
        struct {
            unsigned long LPS: 1;
            unsigned long OCI: 1;
            unsigned long : 13;
            unsigned long DRWE: 1;
            unsigned long LPSC: 1;
            unsigned long OCIC: 1;
            unsigned long : 13;
            unsigned long CRWE: 1;
        } BIT;
    } HCRHSTATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long CCS: 1;
            unsigned long PES: 1;
            unsigned long PSS: 1;
            unsigned long POCI: 1;
            unsigned long PRS: 1;
            unsigned long : 3;
            unsigned long PPS: 1;
            unsigned long LSDA: 1;
            unsigned long : 6;
            unsigned long CSC: 1;
            unsigned long PESC: 1;
            unsigned long PSSC: 1;
            unsigned long OCIC: 1;
            unsigned long PRSC: 1;
            unsigned long : 11;
        } BIT;
    } HCRHPORTSTATUS1;
    char           wk0[4];
    char           wk1[164];
    union {
        unsigned long LONG;
        struct {
            unsigned long CapabilityRegistersLength: 8;
            unsigned long : 8;
            unsigned long InterfaceVersionNumber: 16;
        } BIT;
    } CAPL_VERSION;
    union {
        unsigned long LONG;
        struct {
            unsigned long N_PORTS: 4;
            unsigned long PPC: 1;
            unsigned long : 2;
            unsigned long PortRoutingRules: 1;
            unsigned long N_PCC: 4;
            unsigned long N_CC: 4;
            unsigned long P_INDICATOR: 1;
            unsigned long : 3;
            unsigned long DebugPortNumber: 4;
            unsigned long : 8;
        } BIT;
    } HCSPARAMS;
    union {
        unsigned long LONG;
        struct {
            unsigned long AddressingCapability: 1;
            unsigned long ProgramableFrameListFlag: 1;
            unsigned long AsynchronousScheduleParkCapability: 1;
            unsigned long : 1;
            unsigned long IsochronousSchedulingThreshold: 4;
            unsigned long EECP: 8;
            unsigned long HardwarePrefetch: 1;
            unsigned long LinkPowerManagementCapability: 1;
            unsigned long PerPortChangeEventCapability: 1;
            unsigned long FramePeriodicListCapability: 1;
            unsigned long : 12;
        } BIT;
    } HCCPARAMS;
    union {
        unsigned long LONG;
        struct {
            unsigned long CompanionPortRoute: 32;
        } BIT;
    } HCSP_PORTROUTE;
    char           wk2[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long RS: 1;
            unsigned long HCRESET: 1;
            unsigned long FrameListSize: 2;
            unsigned long PeriodicScheduleEnable: 1;
            unsigned long AsynchronousScheduleEnable: 1;
            unsigned long InterruptonAsyncAdvanceDoorbell: 1;
            unsigned long : 1;
            unsigned long AsynchronousScheduleParkModeCount: 2;
            unsigned long : 1;
            unsigned long AsynchronousScheduleParkModeEnable: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long PerPortChangeEventsEnable: 1;
            unsigned long InterruptThresholdControl: 8;
            unsigned long HostInitiatedResumeDuration: 4;
            unsigned long : 4;
        } BIT;
    } USBCMD;
    union {
        unsigned long LONG;
        struct {
            unsigned long USBINT: 1;
            unsigned long USBERRINT: 1;
            unsigned long PortChangeDetect: 1;
            unsigned long FrameListRollover: 1;
            unsigned long HostSystemError: 1;
            unsigned long InterruptonAsyncAdvance: 1;
            unsigned long : 6;
            unsigned long HCHalted: 1;
            unsigned long Reclamation: 1;
            unsigned long PeriodicScheduleStatus: 1;
            unsigned long AsynchronousScheduleStatus: 1;
            unsigned long Port1ChangeDetect: 1;
            unsigned long : 15;
        } BIT;
    } USBSTS;
    union {
        unsigned long LONG;
        struct {
            unsigned long USBInterruptEnable: 1;
            unsigned long USBErrorInterruptEnable: 1;
            unsigned long PortChangeDetectEnable: 1;
            unsigned long FrameListRolloverEnable: 1;
            unsigned long HostSystemErrorEnable: 1;
            unsigned long InterruptonAsyncAdvanceEnable: 1;
            unsigned long : 10;
            unsigned long Port1ChangeEventEnable: 1;
            unsigned long : 15;
        } BIT;
    } USBINTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long FrameIndex: 14;
            unsigned long : 18;
        } BIT;
    } FRINDEX;
    union {
        unsigned long LONG;
        struct {
            unsigned long CTRLDSSEGMENT: 32;
        } BIT;
    } CTRLDSSEGMENT;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 12;
            unsigned long BaseAddress: 20;
        } BIT;
    } PERIODICLISTBASE;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 5;
            unsigned long LPL: 27;
        } BIT;
    } ASYNCLISTADDR;
    char           wk3[36];
    union {
        unsigned long LONG;
        struct {
            unsigned long CF: 1;
            unsigned long : 31;
        } BIT;
    } CONFIGFLAG;
    union {
        unsigned long LONG;
        struct {
            unsigned long CurrentConnectStatus: 1;
            unsigned long ConnectStatusChange: 1;
            unsigned long PortEnabled_Disabled: 1;
            unsigned long PortEnable_DisableChange: 1;
            unsigned long OvercurrentActive: 1;
            unsigned long OvercurrentChange: 1;
            unsigned long ForcePortResume: 1;
            unsigned long Suspend: 1;
            unsigned long PortReset: 1;
            unsigned long SuspendusingL1: 1;
            unsigned long LineStatus: 2;
            unsigned long PP: 1;
            unsigned long PortOwner: 1;
            unsigned long : 2;
            unsigned long PortTestControl: 4;
            unsigned long WKCNNT_E: 1;
            unsigned long WKDSCNNT_E: 1;
            unsigned long WKOC_E: 1;
            unsigned long SuspendStatus: 2;
            unsigned long DeviceAddress: 7;
        } BIT;
    } PORTSC1;
    char           wk4[4];
    char           wk5[132];
    char           wk6[4];
    char           wk7[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long AHB_INTEN: 1;
            unsigned long USBH_INTAEN: 1;
            unsigned long USBH_INTBEN: 1;
            unsigned long UCOM_INTEN: 1;
            unsigned long WAKEON_INTEN: 1;
            unsigned long : 1;
            unsigned long : 26;
        } BIT;
    } INT_ENABLE;
    union {
        unsigned long LONG;
        struct {
            unsigned long AHB_INT: 1;
            unsigned long USBH_INTA: 1;
            unsigned long USBH_INTB: 1;
            unsigned long UCOM_INT: 1;
            unsigned long WAKEON_INT: 1;
            unsigned long : 1;
            unsigned long : 26;
        } BIT;
    } INT_STATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long MAX_BURST_LEN: 2;
            unsigned long : 2;
            unsigned long ALIGN_ADDRESS: 2;
            unsigned long : 2;
            unsigned long PROT_MODE: 1;
            unsigned long : 3;
            unsigned long PROT_TYPE: 4;
            unsigned long : 16;
        } BIT;
    } AHB_BUS_CTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long USBH_RST: 1;
            unsigned long PLL_RST: 1;
            unsigned long DIRPD: 1;
            unsigned long : 29;
        } BIT;
    } USBCTR;
    char           wk8[240];
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long : 15;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long RPB_WEN: 1;
            unsigned long : 3;
            unsigned long PERI_CLK_MSK: 1;
            unsigned long HOST_CLK_MSK: 1;
            unsigned long : 1;
            unsigned long NONUSE_CLK_MSK: 1;
        } BIT;
    } REGEN_CG_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long GLOBAL_SUSPENDM_P1: 1;
            unsigned long : 1;
            unsigned long : 2;
            unsigned long : 6;
            unsigned long : 12;
            unsigned long : 1;
            unsigned long WKCNNT_ENABLE: 1;
            unsigned long : 6;
            unsigned long SLEEPM_ENABLE: 1;
            unsigned long SUSPENDM_ENABLE: 1;
        } BIT;
    } SPD_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long TIMER_RESUME: 16;
            unsigned long TIMER_CONNECT: 16;
        } BIT;
    } SPD_RSM_TIMSET;
    union {
        unsigned long LONG;
        struct {
            unsigned long TIMER_OC: 20;
            unsigned long TIMER_SLEEP: 9;
            unsigned long : 3;
        } BIT;
    } OC_SLP_TIMSET;
    union {
        unsigned long LONG;
        struct {
            unsigned long SBRN: 8;
            unsigned long FLADJ: 8;
            unsigned long PORTWAKECAP: 16;
        } BIT;
    } SBRN_FLADJ_PW;
    char           wk10[4];
    char           wk11[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long HIRD_SEL_P1: 1;
            unsigned long RETRY_ENABLE_NYET_P1: 1;
            unsigned long SLEEP_INT_EN_P1: 1;
            unsigned long REMOTEWAKE_EN_P1: 1;
            unsigned long NYET_RETRY_CNT_P1: 4;
            unsigned long : 8;
            unsigned long : 11;
            unsigned long : 5;
        } BIT;
    } PORT_LPM_CTRL1;
    char           wk12[4];
    char           wk13[24];
    char           wk14[4];
    char           wk15[4];
    char           wk16[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 5;
            unsigned long DUR_CTRL: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 4;
            unsigned long : 3;
            unsigned long : 5;
            unsigned long : 3;
            unsigned long : 4;
            unsigned long : 1;
        } BIT;
    } U2HC_EXT2;
    char           wk17[4];
    char           wk18[4];
    char           wk19[4];
    char           wk20[144];
    char           wk21[4];
    char           wk22[4];
    char           wk23[4];
    char           wk24[1012];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 30;
            unsigned long : 1;
            unsigned long OTG_PERI: 1;
        } BIT;
    } COMMCTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long IDCHG_STA: 1;
            unsigned long OCINT_STA: 1;
            unsigned long VBSTACHG_STA: 1;
            unsigned long VBSTAINT_STA: 1;
            unsigned long PDDETCHG1_STA: 1;
            unsigned long : 1;
            unsigned long CHGDETCHG1_STA: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long DMMONCHG_STA: 1;
            unsigned long DPMONCHG_STA: 1;
            unsigned long : 1;
            unsigned long : 5;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 4;
        } BIT;
    } OBINTSTA;
    union {
        unsigned long LONG;
        struct {
            unsigned long IDCHG_EN: 1;
            unsigned long OCINT_EN: 1;
            unsigned long VBSTACHG_EN: 1;
            unsigned long VBSTAINT_EN: 1;
            unsigned long PDDETCHG1_EN: 1;
            unsigned long : 1;
            unsigned long CHGDETCHG1_EN: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long DMMONCHG_EN: 1;
            unsigned long DPMONCHG_EN: 1;
            unsigned long : 1;
            unsigned long : 5;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 4;
        } BIT;
    } OBINTEN;
    union {
        unsigned long LONG;
        struct {
            unsigned long VBOUT: 1;
            unsigned long : 3;
            unsigned long VGPUO: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long VBLVL: 1;
            unsigned long : 7;
            unsigned long VBSTA: 1;
            unsigned long : 2;
        } BIT;
    } VBCTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long IDMON: 1;
            unsigned long : 1;
            unsigned long DMMON: 1;
            unsigned long DPMON: 1;
            unsigned long : 2;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 6;
            unsigned long DM_RPD: 1;
            unsigned long DMRPD_EN: 1;
            unsigned long DP_RPD: 1;
            unsigned long DPRPD_EN: 1;
            unsigned long DSDP: 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 6;
        } BIT;
    } LINECTRL1;
    char           wk25[4];
    char           wk26[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long IDPSRCE: 1;
            unsigned long IDMSINKE: 1;
            unsigned long VDPSRCE: 1;
            unsigned long IDPSINKE: 1;
            unsigned long VDMSRCE: 1;
            unsigned long DCPMODE: 1;
            unsigned long : 2;
            unsigned long CHGDETSTS: 1;
            unsigned long PDDETSTS: 1;
            unsigned long : 14;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 6;
        } BIT;
    } BCCTRL1;
    char           wk27[4];
    char           wk28[8];
    char           wk29[4];
    char           wk30[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long CC2_RA: 1;
            unsigned long CC2_RD: 1;
            unsigned long CC1_RA: 1;
            unsigned long CC1_RD: 1;
            unsigned long CC_LVL_STA: 1;
            unsigned long CC_PERI_STA: 1;
            unsigned long : 1;
            unsigned long : 16;
            unsigned long : 1;
            unsigned long CC_LVL_EN: 1;
            unsigned long CC_LVL: 4;
            unsigned long CC_LVL_CLR: 1;
            unsigned long : 1;
            unsigned long CC_INT_SEL: 1;
        } BIT;
    } CC_STATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long UCLKSEL: 1;
            unsigned long : 31;
        } BIT;
    } PHYCLK_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long FIXPHY: 1;
            unsigned long : 31;
        } BIT;
    } PHYIF_CTRL;
};

struct st_usb01 {
    union {
        unsigned short WORD;
        struct {
            unsigned short USBE: 1;
            unsigned short : 3;
            unsigned short DPRPU: 1;
            unsigned short DRPD: 1;
            unsigned short : 1;
            unsigned short HSE: 1;
            unsigned short CNEN: 1;
            unsigned short : 7;
        } BIT;
    } SYSCFG0;
    union {
        unsigned short WORD;
        struct {
            unsigned short BWAIT: 6;
            unsigned short : 2;
            unsigned short : 6;
            unsigned short : 2;
        } BIT;
    } SYSCFG1;
    union {
        unsigned short WORD;
        struct {
            unsigned short LNST: 2;
            unsigned short : 1;
            unsigned short : 11;
            unsigned short : 2;
        } BIT;
    } SYSSTS0;
    char           wk0[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short RHST: 3;
            unsigned short : 5;
            unsigned short WKUP: 1;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short : 4;
            unsigned short BRST1: 1;
        } BIT;
    } DVSTCTR0;
    char           wk1[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short UTST: 4;
            unsigned short : 7;
            unsigned short BRST3: 1;
            unsigned short : 2;
            unsigned short BRST2: 1;
            unsigned short : 1;
        } BIT;
    } TESTMODE;
    char           wk2[6];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long FIFOPORT: 32;
        } BIT;
    } CFIFO;
    char           wk3[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short CURPIPE: 4;
            unsigned short : 1;
            unsigned short ISEL: 1;
            unsigned short : 2;
            unsigned short BIGEND: 1;
            unsigned short : 1;
            unsigned short MBW: 2;
            unsigned short : 2;
            unsigned short REW: 1;
            unsigned short RCNT: 1;
        } BIT;
    } CFIFOSEL;
    union {
        unsigned short WORD;
        struct {
            unsigned short DTLN: 12;
            unsigned short : 1;
            unsigned short FRDY: 1;
            unsigned short BCLR: 1;
            unsigned short BVAL: 1;
        } BIT;
    } CFIFOCTR;
    char           wk4[4];
    union {
        unsigned short WORD;
        struct {
            unsigned short CURPIPE: 4;
            unsigned short : 4;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short MBW: 2;
            unsigned short DREQE: 1;
            unsigned short DCLRM: 1;
            unsigned short REW: 1;
            unsigned short RCNT: 1;
        } BIT;
    } D0FIFOSEL;
    union {
        unsigned short WORD;
        struct {
            unsigned short DTLN: 12;
            unsigned short : 1;
            unsigned short FRDY: 1;
            unsigned short BCLR: 1;
            unsigned short BVAL: 1;
        } BIT;
    } D0FIFOCTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short CURPIPE: 4;
            unsigned short : 4;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short MBW: 2;
            unsigned short DREQE: 1;
            unsigned short DCLRM: 1;
            unsigned short REW: 1;
            unsigned short RCNT: 1;
        } BIT;
    } D1FIFOSEL;
    union {
        unsigned short WORD;
        struct {
            unsigned short DTLN: 12;
            unsigned short : 1;
            unsigned short FRDY: 1;
            unsigned short BCLR: 1;
            unsigned short BVAL: 1;
        } BIT;
    } D1FIFOCTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short BRDYE: 1;
            unsigned short NRDYE: 1;
            unsigned short BEMPE: 1;
            unsigned short CTRE: 1;
            unsigned short DVSE: 1;
            unsigned short SOFE: 1;
            unsigned short RSME: 1;
            unsigned short VBSE: 1;
        } BIT;
    } INTENB0;
    char           wk5[2];
    char           wk6[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPEBRDYE: 16;
        } BIT;
    } BRDYENB;
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPENRDYE: 16;
        } BIT;
    } NRDYENB;
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPEBEMPE: 16;
        } BIT;
    } BEMPENB;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 2;
            unsigned short SOFM: 2;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short BRDYM: 1;
            unsigned short : 9;
        } BIT;
    } SOFCFG;
    char           wk7[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short CTSQ: 3;
            unsigned short VALID: 1;
            unsigned short DVSQ: 3;
            unsigned short VBSTS: 1;
            unsigned short BRDY: 1;
            unsigned short NRDY: 1;
            unsigned short BEMP: 1;
            unsigned short CTRT: 1;
            unsigned short DVST: 1;
            unsigned short SOFR: 1;
            unsigned short RESM: 1;
            unsigned short VBINT: 1;
        } BIT;
    } INTSTS0;
    char           wk8[2];
    char           wk9[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPEBRDY: 16;
        } BIT;
    } BRDYSTS;
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPENRDY: 16;
        } BIT;
    } NRDYSTS;
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPEBEMP: 16;
        } BIT;
    } BEMPSTS;
    union {
        unsigned short WORD;
        struct {
            unsigned short FRNM: 11;
            unsigned short : 3;
            unsigned short CRCE: 1;
            unsigned short OVRN: 1;
        } BIT;
    } FRMNUM;
    union {
        unsigned short WORD;
        struct {
            unsigned short UFRNM: 3;
            unsigned short : 12;
            unsigned short DVCHG: 1;
        } BIT;
    } UFRMNUM;
    union {
        unsigned short WORD;
        struct {
            unsigned short USBADDR: 7;
            unsigned short : 1;
            unsigned short STSRECOV0: 3;
            unsigned short : 5;
        } BIT;
    } USBADDR;
    char           wk10[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short bmRequestType: 8;
            unsigned short bRequest: 8;
        } BIT;
    } USBREQ;
    union {
        unsigned short WORD;
        struct {
            unsigned short wValue: 16;
        } BIT;
    } USBVAL;
    union {
        unsigned short WORD;
        struct {
            unsigned short wIndex: 16;
        } BIT;
    } USBINDX;
    union {
        unsigned short WORD;
        struct {
            unsigned short wLength: 16;
        } BIT;
    } USBLENG;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 7;
            unsigned short SHTNAK: 1;
            unsigned short CNTMD: 1;
            unsigned short : 7;
        } BIT;
    } DCPCFG;
    union {
        unsigned short WORD;
        struct {
            unsigned short MXPS: 7;
            unsigned short : 9;
        } BIT;
    } DCPMAXP;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short CCPL: 1;
            unsigned short : 2;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short : 6;
            unsigned short BSTS: 1;
        } BIT;
    } DCPCTR;
    char           wk11[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short PIPESEL: 4;
            unsigned short : 12;
        } BIT;
    } PIPESEL;
    char           wk12[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EPNUM: 4;
            unsigned short DIR: 1;
            unsigned short : 2;
            unsigned short SHTNAK: 1;
            unsigned short CNTMD: 1;
            unsigned short DBLB: 1;
            unsigned short BFRE: 1;
            unsigned short : 3;
            unsigned short TYPE: 2;
        } BIT;
    } PIPECFG;
    union {
        unsigned short WORD;
        struct {
            unsigned short BUFNMB: 8;
            unsigned short : 2;
            unsigned short BUFSIZE: 5;
            unsigned short : 1;
        } BIT;
    } PIPEBUF;
    union {
        unsigned short WORD;
        struct {
            unsigned short MXPS: 11;
            unsigned short : 5;
        } BIT;
    } PIPEMAXP;
    union {
        unsigned short WORD;
        struct {
            unsigned short IITV: 3;
            unsigned short : 9;
            unsigned short IFIS: 1;
            unsigned short : 3;
        } BIT;
    } PIPEPERI;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE1CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE2CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE3CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE4CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE5CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short : 5;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE6CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short : 5;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE7CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short : 5;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE8CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPE9CTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPEACTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPEBCTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPECCTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPEDCTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPEECTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short PID: 2;
            unsigned short : 3;
            unsigned short PBUSY: 1;
            unsigned short SQMON: 1;
            unsigned short SQSET: 1;
            unsigned short SQCLR: 1;
            unsigned short ACLRM: 1;
            unsigned short ATREPM: 1;
            unsigned short : 3;
            unsigned short INBUFM: 1;
            unsigned short BSTS: 1;
        } BIT;
    } PIPEFCTR;
    char           wk13[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPE1TRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPE1TRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPE2TRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPE2TRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPE3TRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPE3TRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPE4TRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPE4TRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPE5TRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPE5TRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPEBTRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPEBTRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPECTRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPECTRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPEDTRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPEDTRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPEETRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPEETRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPEFTRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPEFTRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPE9TRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPE9TRN;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short TRCLR: 1;
            unsigned short TRENB: 1;
            unsigned short : 6;
        } BIT;
    } PIPEATRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRNCNT: 16;
        } BIT;
    } PIPEATRN;
    char           wk14[16];
    char           wk15[2];
    char           wk16[2];
    char           wk17[2];
    char           wk18[2];
    char           wk19[2];
    char           wk20[2];
    char           wk21[2];
    char           wk22[2];
    char           wk23[2];
    char           wk24[2];
    char           wk25[2];
    char           wk26[26];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 7;
            unsigned short HWUPM: 1;
            unsigned short : 8;
        } BIT;
    } LPCTRL;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 14;
            unsigned short SUSPM: 1;
            unsigned short : 1;
        } BIT;
    } LPSTS;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short : 2;
            unsigned short : 1;
            unsigned short : 2;
            unsigned short : 1;
            unsigned short SusMon: 1;
            unsigned short : 1;
        } BIT;
    } PHYFUNCTR;
    char           wk27[4];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 9;
            unsigned short DpPuDwn: 1;
            unsigned short DmPuDwn: 1;
            unsigned short : 5;
        } BIT;
    } PHYOTGCTR;
    char           wk28[52];
    char           wk29[2];
    char           wk30[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short L1RESPEN: 1;
            unsigned short L1RESPMD: 2;
            unsigned short L1NEGOMD: 1;
            unsigned short DVSQ: 4;
            unsigned short HIRDTHR: 4;
            unsigned short : 2;
            unsigned short L1EXTMD: 1;
            unsigned short : 1;
        } BIT;
    } PL1CTRL1;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short HIRDMON: 4;
            unsigned short RWEMON: 1;
            unsigned short : 3;
        } BIT;
    } PL1CTRL2;
    char           wk31[2];
    char           wk32[2];
    char           wk33[20];
    char           wk34[4];
    char           wk35[4];
    char           wk36[4];
    char           wk37[660];
    union {
        unsigned long LONG;
        struct {
            unsigned long SA_WD: 32;
        } BIT;
    } N0SA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA_WD: 32;
        } BIT;
    } N1SA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTM: 1;
            unsigned long DMARQM: 1;
            unsigned long SWPRQ: 1;
            unsigned long : 5;
            unsigned long DNUM: 8;
        } BIT;
    } CHSTAT_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long CLRDER: 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 2;
            unsigned long SETREN: 1;
            unsigned long : 1;
            unsigned long SETSSWPRQ: 1;
            unsigned long : 1;
            unsigned long SETINTM: 1;
            unsigned long CLRINTM: 1;
            unsigned long SETDMARQM: 1;
            unsigned long CLRDMARQM: 1;
            unsigned long : 12;
        } BIT;
    } CHCTRL_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 1;
            unsigned long : 2;
            unsigned long REQD: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long DRRP: 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long : 1;
            unsigned long WONLY: 1;
            unsigned long DEM: 1;
            unsigned long TCM: 1;
            unsigned long DIM: 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 4;
            unsigned long : 4;
            unsigned long DPR: 4;
            unsigned long : 20;
        } BIT;
    } CHEXT_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA_WD: 32;
        } BIT;
    } N0SA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA_WD: 32;
        } BIT;
    } N1SA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTM: 1;
            unsigned long DMARQM: 1;
            unsigned long SWPRQ: 1;
            unsigned long : 5;
            unsigned long DNUM: 8;
        } BIT;
    } CHSTAT_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long CLRDER: 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 2;
            unsigned long SETREN: 1;
            unsigned long : 1;
            unsigned long SETSSWPRQ: 1;
            unsigned long : 1;
            unsigned long SETINTM: 1;
            unsigned long CLRINTM: 1;
            unsigned long SETDMARQM: 1;
            unsigned long CLRDMARQM: 1;
            unsigned long : 12;
        } BIT;
    } CHCTRL_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 1;
            unsigned long : 2;
            unsigned long REQD: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long DRRP: 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long : 1;
            unsigned long WONLY: 1;
            unsigned long DEM: 1;
            unsigned long TCM: 1;
            unsigned long DIM: 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 4;
            unsigned long : 4;
            unsigned long DPR: 4;
            unsigned long : 20;
        } BIT;
    } CHEXT_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_1;
    char           wk38[384];
    union {
        unsigned long LONG;
        struct {
            unsigned long SCNT: 32;
        } BIT;
    } SCNT_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SSKP: 32;
        } BIT;
    } SSKP_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DCNT: 32;
        } BIT;
    } DCNT_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DSKP: 32;
        } BIT;
    } DSKP_0;
    char           wk39[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SCNT: 32;
        } BIT;
    } SCNT_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SSKP: 32;
        } BIT;
    } SSKP_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DCNT: 32;
        } BIT;
    } DCNT_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DSKP: 32;
        } BIT;
    } DSKP_1;
    char           wk40[208];
    union {
        unsigned long LONG;
        struct {
            unsigned long PR: 1;
            unsigned long LVINT: 1;
            unsigned long : 14;
            unsigned long LDPR: 4;
            unsigned long : 4;
            unsigned long LWPR: 4;
            unsigned long : 4;
        } BIT;
    } DCTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long DITVL: 8;
            unsigned long : 16;
        } BIT;
    } DSCITVL;
    char           wk41[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long EN0: 1;
            unsigned long EN1: 1;
            unsigned long : 30;
        } BIT;
    } DST_EN;
    union {
        unsigned long LONG;
        struct {
            unsigned long ER0: 1;
            unsigned long ER1: 1;
            unsigned long : 30;
        } BIT;
    } DST_ER;
    union {
        unsigned long LONG;
        struct {
            unsigned long END0: 1;
            unsigned long END1: 1;
            unsigned long : 30;
        } BIT;
    } DST_END;
    union {
        unsigned long LONG;
        struct {
            unsigned long TC0: 1;
            unsigned long TC1: 1;
            unsigned long : 30;
        } BIT;
    } DST_TC;
    union {
        unsigned long LONG;
        struct {
            unsigned long SUS0: 1;
            unsigned long SUS1: 1;
            unsigned long : 30;
        } BIT;
    } DST_SUS;
};

#define USB00     (*(volatile struct st_usb00     *)0xE8218000)
#define USB10     (*(volatile struct st_usb00     *)0xE821A000)
#define USB01     (*(volatile struct st_usb01     *)0xE8219000)
#define USB11     (*(volatile struct st_usb01     *)0xE821B000)

#endif
