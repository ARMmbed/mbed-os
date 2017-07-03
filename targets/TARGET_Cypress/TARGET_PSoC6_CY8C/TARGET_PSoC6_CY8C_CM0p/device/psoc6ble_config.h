/***************************************************************************//**
* \file psoc6ble_config.h
*
* \brief
* PSoC 6 BLE device configuration header
*
* \note
* Generated 2/9/2017 by CyDeviceHeaderGenerator v1.1.0.47
* from the register map configuration rev#961378
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _PSOC6BLE_CONFIG_H_
#define _PSOC6BLE_CONFIG_H_

/* Clock Connections */
typedef enum
{
    PCLK_SCB0_CLOCK                 =  0,       /* scb[0].clock */
    PCLK_SCB1_CLOCK                 =  1,       /* scb[1].clock */
    PCLK_SCB2_CLOCK                 =  2,       /* scb[2].clock */
    PCLK_SCB3_CLOCK                 =  3,       /* scb[3].clock */
    PCLK_SCB4_CLOCK                 =  4,       /* scb[4].clock */
    PCLK_SCB5_CLOCK                 =  5,       /* scb[5].clock */
    PCLK_SCB6_CLOCK                 =  6,       /* scb[6].clock */
    PCLK_SCB7_CLOCK                 =  7,       /* scb[7].clock */
    PCLK_SCB8_CLOCK                 =  8,       /* scb[8].clock */
    PCLK_UDB_CLOCKS0                =  9,       /* udb.clocks[0] */
    PCLK_UDB_CLOCKS1                = 10,       /* udb.clocks[1] */
    PCLK_UDB_CLOCKS2                = 11,       /* udb.clocks[2] */
    PCLK_UDB_CLOCKS3                = 12,       /* udb.clocks[3] */
    PCLK_UDB_CLOCKS4                = 13,       /* udb.clocks[4] */
    PCLK_UDB_CLOCKS5                = 14,       /* udb.clocks[5] */
    PCLK_UDB_CLOCKS6                = 15,       /* udb.clocks[6] */
    PCLK_UDB_CLOCKS7                = 16,       /* udb.clocks[7] */
    PCLK_SMARTIO8_CLOCK             = 17,       /* smartio[8].clock */
    PCLK_SMARTIO9_CLOCK             = 18,       /* smartio[9].clock */
    PCLK_TCPWM0_CLOCKS0             = 19,       /* tcpwm[0].clocks[0] */
    PCLK_TCPWM0_CLOCKS1             = 20,       /* tcpwm[0].clocks[1] */
    PCLK_TCPWM0_CLOCKS2             = 21,       /* tcpwm[0].clocks[2] */
    PCLK_TCPWM0_CLOCKS3             = 22,       /* tcpwm[0].clocks[3] */
    PCLK_TCPWM0_CLOCKS4             = 23,       /* tcpwm[0].clocks[4] */
    PCLK_TCPWM0_CLOCKS5             = 24,       /* tcpwm[0].clocks[5] */
    PCLK_TCPWM0_CLOCKS6             = 25,       /* tcpwm[0].clocks[6] */
    PCLK_TCPWM0_CLOCKS7             = 26,       /* tcpwm[0].clocks[7] */
    PCLK_TCPWM1_CLOCKS0             = 27,       /* tcpwm[1].clocks[0] */
    PCLK_TCPWM1_CLOCKS1             = 28,       /* tcpwm[1].clocks[1] */
    PCLK_TCPWM1_CLOCKS2             = 29,       /* tcpwm[1].clocks[2] */
    PCLK_TCPWM1_CLOCKS3             = 30,       /* tcpwm[1].clocks[3] */
    PCLK_TCPWM1_CLOCKS4             = 31,       /* tcpwm[1].clocks[4] */
    PCLK_TCPWM1_CLOCKS5             = 32,       /* tcpwm[1].clocks[5] */
    PCLK_TCPWM1_CLOCKS6             = 33,       /* tcpwm[1].clocks[6] */
    PCLK_TCPWM1_CLOCKS7             = 34,       /* tcpwm[1].clocks[7] */
    PCLK_TCPWM1_CLOCKS8             = 35,       /* tcpwm[1].clocks[8] */
    PCLK_TCPWM1_CLOCKS9             = 36,       /* tcpwm[1].clocks[9] */
    PCLK_TCPWM1_CLOCKS10            = 37,       /* tcpwm[1].clocks[10] */
    PCLK_TCPWM1_CLOCKS11            = 38,       /* tcpwm[1].clocks[11] */
    PCLK_TCPWM1_CLOCKS12            = 39,       /* tcpwm[1].clocks[12] */
    PCLK_TCPWM1_CLOCKS13            = 40,       /* tcpwm[1].clocks[13] */
    PCLK_TCPWM1_CLOCKS14            = 41,       /* tcpwm[1].clocks[14] */
    PCLK_TCPWM1_CLOCKS15            = 42,       /* tcpwm[1].clocks[15] */
    PCLK_TCPWM1_CLOCKS16            = 43,       /* tcpwm[1].clocks[16] */
    PCLK_TCPWM1_CLOCKS17            = 44,       /* tcpwm[1].clocks[17] */
    PCLK_TCPWM1_CLOCKS18            = 45,       /* tcpwm[1].clocks[18] */
    PCLK_TCPWM1_CLOCKS19            = 46,       /* tcpwm[1].clocks[19] */
    PCLK_TCPWM1_CLOCKS20            = 47,       /* tcpwm[1].clocks[20] */
    PCLK_TCPWM1_CLOCKS21            = 48,       /* tcpwm[1].clocks[21] */
    PCLK_TCPWM1_CLOCKS22            = 49,       /* tcpwm[1].clocks[22] */
    PCLK_TCPWM1_CLOCKS23            = 50,       /* tcpwm[1].clocks[23] */
    PCLK_CSD_CLOCK                  = 51,       /* csd.clock */
    PCLK_LCD_CLOCK                  = 52,       /* lcd.clock */
    PCLK_PROFILE_CLOCK_PROFILE      = 53,       /* profile.clock_profile */
    PCLK_CPUSS_CLOCK_TRACE_IN       = 54,       /* cpuss.clock_trace_in */
    PCLK_PASS_CLOCK_CTDAC           = 55,       /* pass.clock_ctdac */
    PCLK_PASS_CLOCK_PUMP_PERI       = 56,       /* pass.clock_pump_peri */
    PCLK_PASS_CLOCK_SAR             = 57,       /* pass.clock_sar */
    PCLK_USB_CLOCK_DEV_BRS          = 58        /* usb.clock_dev_brs */
} en_clk_dst_t;

/* Trigger Group Inputs */
/* Trigger Input Group 0 - DMA Request Assignments */
typedef enum
{
    TRIG0_IN_TR_GROUP10_OUTPUT0     = 0x0000u,  /* tr_group[10].output[0] */
    TRIG0_IN_TR_GROUP10_OUTPUT1     = 0x0001u,  /* tr_group[10].output[1] */
    TRIG0_IN_TR_GROUP10_OUTPUT2     = 0x0002u,  /* tr_group[10].output[2] */
    TRIG0_IN_TR_GROUP10_OUTPUT3     = 0x0003u,  /* tr_group[10].output[3] */
    TRIG0_IN_TR_GROUP10_OUTPUT4     = 0x0004u,  /* tr_group[10].output[4] */
    TRIG0_IN_TR_GROUP10_OUTPUT5     = 0x0005u,  /* tr_group[10].output[5] */
    TRIG0_IN_TR_GROUP10_OUTPUT6     = 0x0006u,  /* tr_group[10].output[6] */
    TRIG0_IN_TR_GROUP10_OUTPUT7     = 0x0007u,  /* tr_group[10].output[7] */
    TRIG0_IN_TR_GROUP11_OUTPUT0     = 0x0008u,  /* tr_group[11].output[0] */
    TRIG0_IN_TR_GROUP11_OUTPUT1     = 0x0009u,  /* tr_group[11].output[1] */
    TRIG0_IN_TR_GROUP11_OUTPUT2     = 0x000Au,  /* tr_group[11].output[2] */
    TRIG0_IN_TR_GROUP11_OUTPUT3     = 0x000Bu,  /* tr_group[11].output[3] */
    TRIG0_IN_TR_GROUP11_OUTPUT4     = 0x000Cu,  /* tr_group[11].output[4] */
    TRIG0_IN_TR_GROUP11_OUTPUT5     = 0x000Du,  /* tr_group[11].output[5] */
    TRIG0_IN_TR_GROUP11_OUTPUT6     = 0x000Eu,  /* tr_group[11].output[6] */
    TRIG0_IN_TR_GROUP11_OUTPUT7     = 0x000Fu,  /* tr_group[11].output[7] */
    TRIG0_IN_TR_GROUP11_OUTPUT8     = 0x0010u,  /* tr_group[11].output[8] */
    TRIG0_IN_TR_GROUP11_OUTPUT9     = 0x0011u,  /* tr_group[11].output[9] */
    TRIG0_IN_TR_GROUP11_OUTPUT10    = 0x0012u,  /* tr_group[11].output[10] */
    TRIG0_IN_TR_GROUP11_OUTPUT11    = 0x0013u,  /* tr_group[11].output[11] */
    TRIG0_IN_TR_GROUP11_OUTPUT12    = 0x0014u,  /* tr_group[11].output[12] */
    TRIG0_IN_TR_GROUP11_OUTPUT13    = 0x0015u,  /* tr_group[11].output[13] */
    TRIG0_IN_TR_GROUP11_OUTPUT14    = 0x0016u,  /* tr_group[11].output[14] */
    TRIG0_IN_TR_GROUP11_OUTPUT15    = 0x0017u,  /* tr_group[11].output[15] */
    TRIG0_IN_TR_GROUP12_OUTPUT8     = 0x0018u,  /* tr_group[12].output[8] */
    TRIG0_IN_TR_GROUP12_OUTPUT9     = 0x0019u,  /* tr_group[12].output[9] */
    TRIG0_IN_TR_GROUP13_OUTPUT0     = 0x001Au,  /* tr_group[13].output[0] */
    TRIG0_IN_TR_GROUP13_OUTPUT1     = 0x001Bu,  /* tr_group[13].output[1] */
    TRIG0_IN_TR_GROUP13_OUTPUT2     = 0x001Cu,  /* tr_group[13].output[2] */
    TRIG0_IN_TR_GROUP13_OUTPUT3     = 0x001Du,  /* tr_group[13].output[3] */
    TRIG0_IN_TR_GROUP13_OUTPUT4     = 0x001Eu,  /* tr_group[13].output[4] */
    TRIG0_IN_TR_GROUP13_OUTPUT5     = 0x001Fu,  /* tr_group[13].output[5] */
    TRIG0_IN_TR_GROUP13_OUTPUT6     = 0x0020u,  /* tr_group[13].output[6] */
    TRIG0_IN_TR_GROUP13_OUTPUT7     = 0x0021u,  /* tr_group[13].output[7] */
    TRIG0_IN_TR_GROUP13_OUTPUT8     = 0x0022u,  /* tr_group[13].output[8] */
    TRIG0_IN_TR_GROUP13_OUTPUT9     = 0x0023u,  /* tr_group[13].output[9] */
    TRIG0_IN_TR_GROUP13_OUTPUT10    = 0x0024u,  /* tr_group[13].output[10] */
    TRIG0_IN_TR_GROUP13_OUTPUT11    = 0x0025u,  /* tr_group[13].output[11] */
    TRIG0_IN_TR_GROUP13_OUTPUT12    = 0x0026u,  /* tr_group[13].output[12] */
    TRIG0_IN_TR_GROUP13_OUTPUT13    = 0x0027u,  /* tr_group[13].output[13] */
    TRIG0_IN_TR_GROUP13_OUTPUT14    = 0x0028u,  /* tr_group[13].output[14] */
    TRIG0_IN_TR_GROUP13_OUTPUT15    = 0x0029u,  /* tr_group[13].output[15] */
    TRIG0_IN_TR_GROUP14_OUTPUT0     = 0x002Au,  /* tr_group[14].output[0] */
    TRIG0_IN_TR_GROUP14_OUTPUT1     = 0x002Bu,  /* tr_group[14].output[1] */
    TRIG0_IN_TR_GROUP14_OUTPUT2     = 0x002Cu,  /* tr_group[14].output[2] */
    TRIG0_IN_TR_GROUP14_OUTPUT3     = 0x002Du,  /* tr_group[14].output[3] */
    TRIG0_IN_TR_GROUP14_OUTPUT4     = 0x002Eu,  /* tr_group[14].output[4] */
    TRIG0_IN_TR_GROUP14_OUTPUT5     = 0x002Fu,  /* tr_group[14].output[5] */
    TRIG0_IN_TR_GROUP14_OUTPUT6     = 0x0030u,  /* tr_group[14].output[6] */
    TRIG0_IN_TR_GROUP14_OUTPUT7     = 0x0031u   /* tr_group[14].output[7] */
} en_trig_input_grp0_t;

/* Trigger Input Group 1 - DMA Request Assignments */
typedef enum
{
    TRIG1_IN_TR_GROUP10_OUTPUT0     = 0x0100u,  /* tr_group[10].output[0] */
    TRIG1_IN_TR_GROUP10_OUTPUT1     = 0x0101u,  /* tr_group[10].output[1] */
    TRIG1_IN_TR_GROUP10_OUTPUT2     = 0x0102u,  /* tr_group[10].output[2] */
    TRIG1_IN_TR_GROUP10_OUTPUT3     = 0x0103u,  /* tr_group[10].output[3] */
    TRIG1_IN_TR_GROUP10_OUTPUT4     = 0x0104u,  /* tr_group[10].output[4] */
    TRIG1_IN_TR_GROUP10_OUTPUT5     = 0x0105u,  /* tr_group[10].output[5] */
    TRIG1_IN_TR_GROUP10_OUTPUT6     = 0x0106u,  /* tr_group[10].output[6] */
    TRIG1_IN_TR_GROUP10_OUTPUT7     = 0x0107u,  /* tr_group[10].output[7] */
    TRIG1_IN_TR_GROUP11_OUTPUT0     = 0x0108u,  /* tr_group[11].output[0] */
    TRIG1_IN_TR_GROUP11_OUTPUT1     = 0x0109u,  /* tr_group[11].output[1] */
    TRIG1_IN_TR_GROUP11_OUTPUT2     = 0x010Au,  /* tr_group[11].output[2] */
    TRIG1_IN_TR_GROUP11_OUTPUT3     = 0x010Bu,  /* tr_group[11].output[3] */
    TRIG1_IN_TR_GROUP11_OUTPUT4     = 0x010Cu,  /* tr_group[11].output[4] */
    TRIG1_IN_TR_GROUP11_OUTPUT5     = 0x010Du,  /* tr_group[11].output[5] */
    TRIG1_IN_TR_GROUP11_OUTPUT6     = 0x010Eu,  /* tr_group[11].output[6] */
    TRIG1_IN_TR_GROUP11_OUTPUT7     = 0x010Fu,  /* tr_group[11].output[7] */
    TRIG1_IN_TR_GROUP11_OUTPUT8     = 0x0110u,  /* tr_group[11].output[8] */
    TRIG1_IN_TR_GROUP11_OUTPUT9     = 0x0111u,  /* tr_group[11].output[9] */
    TRIG1_IN_TR_GROUP11_OUTPUT10    = 0x0112u,  /* tr_group[11].output[10] */
    TRIG1_IN_TR_GROUP11_OUTPUT11    = 0x0113u,  /* tr_group[11].output[11] */
    TRIG1_IN_TR_GROUP11_OUTPUT12    = 0x0114u,  /* tr_group[11].output[12] */
    TRIG1_IN_TR_GROUP11_OUTPUT13    = 0x0115u,  /* tr_group[11].output[13] */
    TRIG1_IN_TR_GROUP11_OUTPUT14    = 0x0116u,  /* tr_group[11].output[14] */
    TRIG1_IN_TR_GROUP11_OUTPUT15    = 0x0117u,  /* tr_group[11].output[15] */
    TRIG1_IN_TR_GROUP12_OUTPUT8     = 0x0118u,  /* tr_group[12].output[8] */
    TRIG1_IN_TR_GROUP12_OUTPUT9     = 0x0119u,  /* tr_group[12].output[9] */
    TRIG1_IN_TR_GROUP13_OUTPUT0     = 0x011Au,  /* tr_group[13].output[0] */
    TRIG1_IN_TR_GROUP13_OUTPUT1     = 0x011Bu,  /* tr_group[13].output[1] */
    TRIG1_IN_TR_GROUP13_OUTPUT2     = 0x011Cu,  /* tr_group[13].output[2] */
    TRIG1_IN_TR_GROUP13_OUTPUT3     = 0x011Du,  /* tr_group[13].output[3] */
    TRIG1_IN_TR_GROUP13_OUTPUT4     = 0x011Eu,  /* tr_group[13].output[4] */
    TRIG1_IN_TR_GROUP13_OUTPUT5     = 0x011Fu,  /* tr_group[13].output[5] */
    TRIG1_IN_TR_GROUP13_OUTPUT6     = 0x0120u,  /* tr_group[13].output[6] */
    TRIG1_IN_TR_GROUP13_OUTPUT7     = 0x0121u,  /* tr_group[13].output[7] */
    TRIG1_IN_TR_GROUP13_OUTPUT8     = 0x0122u,  /* tr_group[13].output[8] */
    TRIG1_IN_TR_GROUP13_OUTPUT9     = 0x0123u,  /* tr_group[13].output[9] */
    TRIG1_IN_TR_GROUP13_OUTPUT10    = 0x0124u,  /* tr_group[13].output[10] */
    TRIG1_IN_TR_GROUP13_OUTPUT11    = 0x0125u,  /* tr_group[13].output[11] */
    TRIG1_IN_TR_GROUP13_OUTPUT12    = 0x0126u,  /* tr_group[13].output[12] */
    TRIG1_IN_TR_GROUP13_OUTPUT13    = 0x0127u,  /* tr_group[13].output[13] */
    TRIG1_IN_TR_GROUP13_OUTPUT14    = 0x0128u,  /* tr_group[13].output[14] */
    TRIG1_IN_TR_GROUP13_OUTPUT15    = 0x0129u,  /* tr_group[13].output[15] */
    TRIG1_IN_TR_GROUP14_OUTPUT0     = 0x012Au,  /* tr_group[14].output[0] */
    TRIG1_IN_TR_GROUP14_OUTPUT1     = 0x012Bu,  /* tr_group[14].output[1] */
    TRIG1_IN_TR_GROUP14_OUTPUT2     = 0x012Cu,  /* tr_group[14].output[2] */
    TRIG1_IN_TR_GROUP14_OUTPUT3     = 0x012Du,  /* tr_group[14].output[3] */
    TRIG1_IN_TR_GROUP14_OUTPUT4     = 0x012Eu,  /* tr_group[14].output[4] */
    TRIG1_IN_TR_GROUP14_OUTPUT5     = 0x012Fu,  /* tr_group[14].output[5] */
    TRIG1_IN_TR_GROUP14_OUTPUT6     = 0x0130u,  /* tr_group[14].output[6] */
    TRIG1_IN_TR_GROUP14_OUTPUT7     = 0x0131u   /* tr_group[14].output[7] */
} en_trig_input_grp1_t;

/* Trigger Input Group 2 - TCPWM trigger inputs */
typedef enum
{
    TRIG2_IN_TR_GROUP10_OUTPUT0     = 0x0200u,  /* tr_group[10].output[0] */
    TRIG2_IN_TR_GROUP10_OUTPUT1     = 0x0201u,  /* tr_group[10].output[1] */
    TRIG2_IN_TR_GROUP10_OUTPUT2     = 0x0202u,  /* tr_group[10].output[2] */
    TRIG2_IN_TR_GROUP10_OUTPUT3     = 0x0203u,  /* tr_group[10].output[3] */
    TRIG2_IN_TR_GROUP10_OUTPUT4     = 0x0204u,  /* tr_group[10].output[4] */
    TRIG2_IN_TR_GROUP10_OUTPUT5     = 0x0205u,  /* tr_group[10].output[5] */
    TRIG2_IN_TR_GROUP10_OUTPUT6     = 0x0206u,  /* tr_group[10].output[6] */
    TRIG2_IN_TR_GROUP10_OUTPUT7     = 0x0207u,  /* tr_group[10].output[7] */
    TRIG2_IN_TR_GROUP11_OUTPUT0     = 0x0208u,  /* tr_group[11].output[0] */
    TRIG2_IN_TR_GROUP11_OUTPUT1     = 0x0209u,  /* tr_group[11].output[1] */
    TRIG2_IN_TR_GROUP11_OUTPUT2     = 0x020Au,  /* tr_group[11].output[2] */
    TRIG2_IN_TR_GROUP11_OUTPUT3     = 0x020Bu,  /* tr_group[11].output[3] */
    TRIG2_IN_TR_GROUP11_OUTPUT4     = 0x020Cu,  /* tr_group[11].output[4] */
    TRIG2_IN_TR_GROUP11_OUTPUT5     = 0x020Du,  /* tr_group[11].output[5] */
    TRIG2_IN_TR_GROUP11_OUTPUT6     = 0x020Eu,  /* tr_group[11].output[6] */
    TRIG2_IN_TR_GROUP11_OUTPUT7     = 0x020Fu,  /* tr_group[11].output[7] */
    TRIG2_IN_TR_GROUP11_OUTPUT8     = 0x0210u,  /* tr_group[11].output[8] */
    TRIG2_IN_TR_GROUP11_OUTPUT9     = 0x0211u,  /* tr_group[11].output[9] */
    TRIG2_IN_TR_GROUP11_OUTPUT10    = 0x0212u,  /* tr_group[11].output[10] */
    TRIG2_IN_TR_GROUP11_OUTPUT11    = 0x0213u,  /* tr_group[11].output[11] */
    TRIG2_IN_TR_GROUP11_OUTPUT12    = 0x0214u,  /* tr_group[11].output[12] */
    TRIG2_IN_TR_GROUP11_OUTPUT13    = 0x0215u,  /* tr_group[11].output[13] */
    TRIG2_IN_TR_GROUP11_OUTPUT14    = 0x0216u,  /* tr_group[11].output[14] */
    TRIG2_IN_TR_GROUP11_OUTPUT15    = 0x0217u,  /* tr_group[11].output[15] */
    TRIG2_IN_TR_GROUP12_OUTPUT0     = 0x0218u,  /* tr_group[12].output[0] */
    TRIG2_IN_TR_GROUP12_OUTPUT1     = 0x0219u,  /* tr_group[12].output[1] */
    TRIG2_IN_TR_GROUP12_OUTPUT2     = 0x021Au,  /* tr_group[12].output[2] */
    TRIG2_IN_TR_GROUP12_OUTPUT3     = 0x021Bu,  /* tr_group[12].output[3] */
    TRIG2_IN_TR_GROUP12_OUTPUT4     = 0x021Cu,  /* tr_group[12].output[4] */
    TRIG2_IN_TR_GROUP12_OUTPUT5     = 0x021Du,  /* tr_group[12].output[5] */
    TRIG2_IN_TR_GROUP12_OUTPUT6     = 0x021Eu,  /* tr_group[12].output[6] */
    TRIG2_IN_TR_GROUP12_OUTPUT7     = 0x021Fu,  /* tr_group[12].output[7] */
    TRIG2_IN_TR_GROUP13_OUTPUT16    = 0x0220u,  /* tr_group[13].output[16] */
    TRIG2_IN_TR_GROUP13_OUTPUT17    = 0x0221u,  /* tr_group[13].output[17] */
    TRIG2_IN_TR_GROUP14_OUTPUT8     = 0x0222u,  /* tr_group[14].output[8] */
    TRIG2_IN_TR_GROUP14_OUTPUT9     = 0x0223u,  /* tr_group[14].output[9] */
    TRIG2_IN_TR_GROUP14_OUTPUT10    = 0x0224u,  /* tr_group[14].output[10] */
    TRIG2_IN_TR_GROUP14_OUTPUT11    = 0x0225u,  /* tr_group[14].output[11] */
    TRIG2_IN_TR_GROUP14_OUTPUT12    = 0x0226u,  /* tr_group[14].output[12] */
    TRIG2_IN_TR_GROUP14_OUTPUT13    = 0x0227u,  /* tr_group[14].output[13] */
    TRIG2_IN_TR_GROUP14_OUTPUT14    = 0x0228u,  /* tr_group[14].output[14] */
    TRIG2_IN_TR_GROUP14_OUTPUT15    = 0x0229u   /* tr_group[14].output[15] */
} en_trig_input_grp2_t;

/* Trigger Input Group 3 - TCPWM trigger inputs */
typedef enum
{
    TRIG3_IN_TR_GROUP10_OUTPUT0     = 0x0300u,  /* tr_group[10].output[0] */
    TRIG3_IN_TR_GROUP10_OUTPUT1     = 0x0301u,  /* tr_group[10].output[1] */
    TRIG3_IN_TR_GROUP10_OUTPUT2     = 0x0302u,  /* tr_group[10].output[2] */
    TRIG3_IN_TR_GROUP10_OUTPUT3     = 0x0303u,  /* tr_group[10].output[3] */
    TRIG3_IN_TR_GROUP10_OUTPUT4     = 0x0304u,  /* tr_group[10].output[4] */
    TRIG3_IN_TR_GROUP10_OUTPUT5     = 0x0305u,  /* tr_group[10].output[5] */
    TRIG3_IN_TR_GROUP10_OUTPUT6     = 0x0306u,  /* tr_group[10].output[6] */
    TRIG3_IN_TR_GROUP10_OUTPUT7     = 0x0307u,  /* tr_group[10].output[7] */
    TRIG3_IN_TR_GROUP11_OUTPUT0     = 0x0308u,  /* tr_group[11].output[0] */
    TRIG3_IN_TR_GROUP11_OUTPUT1     = 0x0309u,  /* tr_group[11].output[1] */
    TRIG3_IN_TR_GROUP11_OUTPUT2     = 0x030Au,  /* tr_group[11].output[2] */
    TRIG3_IN_TR_GROUP11_OUTPUT3     = 0x030Bu,  /* tr_group[11].output[3] */
    TRIG3_IN_TR_GROUP11_OUTPUT4     = 0x030Cu,  /* tr_group[11].output[4] */
    TRIG3_IN_TR_GROUP11_OUTPUT5     = 0x030Du,  /* tr_group[11].output[5] */
    TRIG3_IN_TR_GROUP11_OUTPUT6     = 0x030Eu,  /* tr_group[11].output[6] */
    TRIG3_IN_TR_GROUP11_OUTPUT7     = 0x030Fu,  /* tr_group[11].output[7] */
    TRIG3_IN_TR_GROUP11_OUTPUT8     = 0x0310u,  /* tr_group[11].output[8] */
    TRIG3_IN_TR_GROUP11_OUTPUT9     = 0x0311u,  /* tr_group[11].output[9] */
    TRIG3_IN_TR_GROUP11_OUTPUT10    = 0x0312u,  /* tr_group[11].output[10] */
    TRIG3_IN_TR_GROUP11_OUTPUT11    = 0x0313u,  /* tr_group[11].output[11] */
    TRIG3_IN_TR_GROUP11_OUTPUT12    = 0x0314u,  /* tr_group[11].output[12] */
    TRIG3_IN_TR_GROUP11_OUTPUT13    = 0x0315u,  /* tr_group[11].output[13] */
    TRIG3_IN_TR_GROUP11_OUTPUT14    = 0x0316u,  /* tr_group[11].output[14] */
    TRIG3_IN_TR_GROUP11_OUTPUT15    = 0x0317u,  /* tr_group[11].output[15] */
    TRIG3_IN_TR_GROUP12_OUTPUT0     = 0x0318u,  /* tr_group[12].output[0] */
    TRIG3_IN_TR_GROUP12_OUTPUT1     = 0x0319u,  /* tr_group[12].output[1] */
    TRIG3_IN_TR_GROUP12_OUTPUT2     = 0x031Au,  /* tr_group[12].output[2] */
    TRIG3_IN_TR_GROUP12_OUTPUT3     = 0x031Bu,  /* tr_group[12].output[3] */
    TRIG3_IN_TR_GROUP12_OUTPUT4     = 0x031Cu,  /* tr_group[12].output[4] */
    TRIG3_IN_TR_GROUP12_OUTPUT5     = 0x031Du,  /* tr_group[12].output[5] */
    TRIG3_IN_TR_GROUP12_OUTPUT6     = 0x031Eu,  /* tr_group[12].output[6] */
    TRIG3_IN_TR_GROUP12_OUTPUT7     = 0x031Fu,  /* tr_group[12].output[7] */
    TRIG3_IN_TR_GROUP13_OUTPUT16    = 0x0320u,  /* tr_group[13].output[16] */
    TRIG3_IN_TR_GROUP13_OUTPUT17    = 0x0321u,  /* tr_group[13].output[17] */
    TRIG3_IN_TR_GROUP14_OUTPUT8     = 0x0322u,  /* tr_group[14].output[8] */
    TRIG3_IN_TR_GROUP14_OUTPUT9     = 0x0323u,  /* tr_group[14].output[9] */
    TRIG3_IN_TR_GROUP14_OUTPUT10    = 0x0324u,  /* tr_group[14].output[10] */
    TRIG3_IN_TR_GROUP14_OUTPUT11    = 0x0325u,  /* tr_group[14].output[11] */
    TRIG3_IN_TR_GROUP14_OUTPUT12    = 0x0326u,  /* tr_group[14].output[12] */
    TRIG3_IN_TR_GROUP14_OUTPUT13    = 0x0327u,  /* tr_group[14].output[13] */
    TRIG3_IN_TR_GROUP14_OUTPUT14    = 0x0328u,  /* tr_group[14].output[14] */
    TRIG3_IN_TR_GROUP14_OUTPUT15    = 0x0329u   /* tr_group[14].output[15] */
} en_trig_input_grp3_t;

/* Trigger Input Group 4 - PROFILE trigger multiplexer */
typedef enum
{
    TRIG4_IN_TR_GROUP10_OUTPUT0     = 0x0400u,  /* tr_group[10].output[0] */
    TRIG4_IN_TR_GROUP10_OUTPUT1     = 0x0401u,  /* tr_group[10].output[1] */
    TRIG4_IN_TR_GROUP10_OUTPUT2     = 0x0402u,  /* tr_group[10].output[2] */
    TRIG4_IN_TR_GROUP10_OUTPUT3     = 0x0403u,  /* tr_group[10].output[3] */
    TRIG4_IN_TR_GROUP10_OUTPUT4     = 0x0404u,  /* tr_group[10].output[4] */
    TRIG4_IN_TR_GROUP10_OUTPUT5     = 0x0405u,  /* tr_group[10].output[5] */
    TRIG4_IN_TR_GROUP10_OUTPUT6     = 0x0406u,  /* tr_group[10].output[6] */
    TRIG4_IN_TR_GROUP10_OUTPUT7     = 0x0407u,  /* tr_group[10].output[7] */
    TRIG4_IN_TR_GROUP11_OUTPUT0     = 0x0408u,  /* tr_group[11].output[0] */
    TRIG4_IN_TR_GROUP11_OUTPUT1     = 0x0409u,  /* tr_group[11].output[1] */
    TRIG4_IN_TR_GROUP11_OUTPUT2     = 0x040Au,  /* tr_group[11].output[2] */
    TRIG4_IN_TR_GROUP11_OUTPUT3     = 0x040Bu,  /* tr_group[11].output[3] */
    TRIG4_IN_TR_GROUP11_OUTPUT4     = 0x040Cu,  /* tr_group[11].output[4] */
    TRIG4_IN_TR_GROUP11_OUTPUT5     = 0x040Du,  /* tr_group[11].output[5] */
    TRIG4_IN_TR_GROUP11_OUTPUT6     = 0x040Eu,  /* tr_group[11].output[6] */
    TRIG4_IN_TR_GROUP11_OUTPUT7     = 0x040Fu,  /* tr_group[11].output[7] */
    TRIG4_IN_TR_GROUP11_OUTPUT8     = 0x0410u,  /* tr_group[11].output[8] */
    TRIG4_IN_TR_GROUP11_OUTPUT9     = 0x0411u,  /* tr_group[11].output[9] */
    TRIG4_IN_TR_GROUP11_OUTPUT10    = 0x0412u,  /* tr_group[11].output[10] */
    TRIG4_IN_TR_GROUP11_OUTPUT11    = 0x0413u,  /* tr_group[11].output[11] */
    TRIG4_IN_TR_GROUP11_OUTPUT12    = 0x0414u,  /* tr_group[11].output[12] */
    TRIG4_IN_TR_GROUP11_OUTPUT13    = 0x0415u,  /* tr_group[11].output[13] */
    TRIG4_IN_TR_GROUP11_OUTPUT14    = 0x0416u,  /* tr_group[11].output[14] */
    TRIG4_IN_TR_GROUP11_OUTPUT15    = 0x0417u,  /* tr_group[11].output[15] */
    TRIG4_IN_TR_GROUP12_OUTPUT0     = 0x0418u,  /* tr_group[12].output[0] */
    TRIG4_IN_TR_GROUP12_OUTPUT1     = 0x0419u,  /* tr_group[12].output[1] */
    TRIG4_IN_TR_GROUP12_OUTPUT2     = 0x041Au,  /* tr_group[12].output[2] */
    TRIG4_IN_TR_GROUP12_OUTPUT3     = 0x041Bu,  /* tr_group[12].output[3] */
    TRIG4_IN_TR_GROUP12_OUTPUT4     = 0x041Cu,  /* tr_group[12].output[4] */
    TRIG4_IN_TR_GROUP12_OUTPUT5     = 0x041Du,  /* tr_group[12].output[5] */
    TRIG4_IN_TR_GROUP12_OUTPUT6     = 0x041Eu,  /* tr_group[12].output[6] */
    TRIG4_IN_TR_GROUP12_OUTPUT7     = 0x041Fu,  /* tr_group[12].output[7] */
    TRIG4_IN_TR_GROUP13_OUTPUT16    = 0x0420u,  /* tr_group[13].output[16] */
    TRIG4_IN_TR_GROUP13_OUTPUT17    = 0x0421u,  /* tr_group[13].output[17] */
    TRIG4_IN_TR_GROUP14_OUTPUT8     = 0x0422u,  /* tr_group[14].output[8] */
    TRIG4_IN_TR_GROUP14_OUTPUT9     = 0x0423u,  /* tr_group[14].output[9] */
    TRIG4_IN_TR_GROUP14_OUTPUT10    = 0x0424u,  /* tr_group[14].output[10] */
    TRIG4_IN_TR_GROUP14_OUTPUT11    = 0x0425u,  /* tr_group[14].output[11] */
    TRIG4_IN_TR_GROUP14_OUTPUT12    = 0x0426u,  /* tr_group[14].output[12] */
    TRIG4_IN_TR_GROUP14_OUTPUT13    = 0x0427u,  /* tr_group[14].output[13] */
    TRIG4_IN_TR_GROUP14_OUTPUT14    = 0x0428u,  /* tr_group[14].output[14] */
    TRIG4_IN_TR_GROUP14_OUTPUT15    = 0x0429u   /* tr_group[14].output[15] */
} en_trig_input_grp4_t;

/* Trigger Input Group 5 - CPUSS.CTI trigger multiplexer */
typedef enum
{
    TRIG5_IN_TR_GROUP10_OUTPUT0     = 0x0500u,  /* tr_group[10].output[0] */
    TRIG5_IN_TR_GROUP10_OUTPUT1     = 0x0501u,  /* tr_group[10].output[1] */
    TRIG5_IN_TR_GROUP10_OUTPUT2     = 0x0502u,  /* tr_group[10].output[2] */
    TRIG5_IN_TR_GROUP10_OUTPUT3     = 0x0503u,  /* tr_group[10].output[3] */
    TRIG5_IN_TR_GROUP10_OUTPUT4     = 0x0504u,  /* tr_group[10].output[4] */
    TRIG5_IN_TR_GROUP10_OUTPUT5     = 0x0505u,  /* tr_group[10].output[5] */
    TRIG5_IN_TR_GROUP10_OUTPUT6     = 0x0506u,  /* tr_group[10].output[6] */
    TRIG5_IN_TR_GROUP10_OUTPUT7     = 0x0507u,  /* tr_group[10].output[7] */
    TRIG5_IN_TR_GROUP11_OUTPUT0     = 0x0508u,  /* tr_group[11].output[0] */
    TRIG5_IN_TR_GROUP11_OUTPUT1     = 0x0509u,  /* tr_group[11].output[1] */
    TRIG5_IN_TR_GROUP11_OUTPUT2     = 0x050Au,  /* tr_group[11].output[2] */
    TRIG5_IN_TR_GROUP11_OUTPUT3     = 0x050Bu,  /* tr_group[11].output[3] */
    TRIG5_IN_TR_GROUP11_OUTPUT4     = 0x050Cu,  /* tr_group[11].output[4] */
    TRIG5_IN_TR_GROUP11_OUTPUT5     = 0x050Du,  /* tr_group[11].output[5] */
    TRIG5_IN_TR_GROUP11_OUTPUT6     = 0x050Eu,  /* tr_group[11].output[6] */
    TRIG5_IN_TR_GROUP11_OUTPUT7     = 0x050Fu,  /* tr_group[11].output[7] */
    TRIG5_IN_TR_GROUP11_OUTPUT8     = 0x0510u,  /* tr_group[11].output[8] */
    TRIG5_IN_TR_GROUP11_OUTPUT9     = 0x0511u,  /* tr_group[11].output[9] */
    TRIG5_IN_TR_GROUP11_OUTPUT10    = 0x0512u,  /* tr_group[11].output[10] */
    TRIG5_IN_TR_GROUP11_OUTPUT11    = 0x0513u,  /* tr_group[11].output[11] */
    TRIG5_IN_TR_GROUP11_OUTPUT12    = 0x0514u,  /* tr_group[11].output[12] */
    TRIG5_IN_TR_GROUP11_OUTPUT13    = 0x0515u,  /* tr_group[11].output[13] */
    TRIG5_IN_TR_GROUP11_OUTPUT14    = 0x0516u,  /* tr_group[11].output[14] */
    TRIG5_IN_TR_GROUP11_OUTPUT15    = 0x0517u,  /* tr_group[11].output[15] */
    TRIG5_IN_TR_GROUP12_OUTPUT0     = 0x0518u,  /* tr_group[12].output[0] */
    TRIG5_IN_TR_GROUP12_OUTPUT1     = 0x0519u,  /* tr_group[12].output[1] */
    TRIG5_IN_TR_GROUP12_OUTPUT2     = 0x051Au,  /* tr_group[12].output[2] */
    TRIG5_IN_TR_GROUP12_OUTPUT3     = 0x051Bu,  /* tr_group[12].output[3] */
    TRIG5_IN_TR_GROUP12_OUTPUT4     = 0x051Cu,  /* tr_group[12].output[4] */
    TRIG5_IN_TR_GROUP12_OUTPUT5     = 0x051Du,  /* tr_group[12].output[5] */
    TRIG5_IN_TR_GROUP12_OUTPUT6     = 0x051Eu,  /* tr_group[12].output[6] */
    TRIG5_IN_TR_GROUP12_OUTPUT7     = 0x051Fu,  /* tr_group[12].output[7] */
    TRIG5_IN_TR_GROUP13_OUTPUT16    = 0x0520u,  /* tr_group[13].output[16] */
    TRIG5_IN_TR_GROUP13_OUTPUT17    = 0x0521u,  /* tr_group[13].output[17] */
    TRIG5_IN_TR_GROUP14_OUTPUT8     = 0x0522u,  /* tr_group[14].output[8] */
    TRIG5_IN_TR_GROUP14_OUTPUT9     = 0x0523u,  /* tr_group[14].output[9] */
    TRIG5_IN_TR_GROUP14_OUTPUT10    = 0x0524u,  /* tr_group[14].output[10] */
    TRIG5_IN_TR_GROUP14_OUTPUT11    = 0x0525u,  /* tr_group[14].output[11] */
    TRIG5_IN_TR_GROUP14_OUTPUT12    = 0x0526u,  /* tr_group[14].output[12] */
    TRIG5_IN_TR_GROUP14_OUTPUT13    = 0x0527u,  /* tr_group[14].output[13] */
    TRIG5_IN_TR_GROUP14_OUTPUT14    = 0x0528u,  /* tr_group[14].output[14] */
    TRIG5_IN_TR_GROUP14_OUTPUT15    = 0x0529u   /* tr_group[14].output[15] */
} en_trig_input_grp5_t;

/* Trigger Input Group 6 - PASS trigger multiplexer */
typedef enum
{
    TRIG6_IN_TR_GROUP10_OUTPUT0     = 0x0600u,  /* tr_group[10].output[0] */
    TRIG6_IN_TR_GROUP10_OUTPUT1     = 0x0601u,  /* tr_group[10].output[1] */
    TRIG6_IN_TR_GROUP10_OUTPUT2     = 0x0602u,  /* tr_group[10].output[2] */
    TRIG6_IN_TR_GROUP10_OUTPUT3     = 0x0603u,  /* tr_group[10].output[3] */
    TRIG6_IN_TR_GROUP10_OUTPUT4     = 0x0604u,  /* tr_group[10].output[4] */
    TRIG6_IN_TR_GROUP10_OUTPUT5     = 0x0605u,  /* tr_group[10].output[5] */
    TRIG6_IN_TR_GROUP10_OUTPUT6     = 0x0606u,  /* tr_group[10].output[6] */
    TRIG6_IN_TR_GROUP10_OUTPUT7     = 0x0607u,  /* tr_group[10].output[7] */
    TRIG6_IN_TR_GROUP11_OUTPUT0     = 0x0608u,  /* tr_group[11].output[0] */
    TRIG6_IN_TR_GROUP11_OUTPUT1     = 0x0609u,  /* tr_group[11].output[1] */
    TRIG6_IN_TR_GROUP11_OUTPUT2     = 0x060Au,  /* tr_group[11].output[2] */
    TRIG6_IN_TR_GROUP11_OUTPUT3     = 0x060Bu,  /* tr_group[11].output[3] */
    TRIG6_IN_TR_GROUP11_OUTPUT4     = 0x060Cu,  /* tr_group[11].output[4] */
    TRIG6_IN_TR_GROUP11_OUTPUT5     = 0x060Du,  /* tr_group[11].output[5] */
    TRIG6_IN_TR_GROUP11_OUTPUT6     = 0x060Eu,  /* tr_group[11].output[6] */
    TRIG6_IN_TR_GROUP11_OUTPUT7     = 0x060Fu,  /* tr_group[11].output[7] */
    TRIG6_IN_TR_GROUP11_OUTPUT8     = 0x0610u,  /* tr_group[11].output[8] */
    TRIG6_IN_TR_GROUP11_OUTPUT9     = 0x0611u,  /* tr_group[11].output[9] */
    TRIG6_IN_TR_GROUP11_OUTPUT10    = 0x0612u,  /* tr_group[11].output[10] */
    TRIG6_IN_TR_GROUP11_OUTPUT11    = 0x0613u,  /* tr_group[11].output[11] */
    TRIG6_IN_TR_GROUP11_OUTPUT12    = 0x0614u,  /* tr_group[11].output[12] */
    TRIG6_IN_TR_GROUP11_OUTPUT13    = 0x0615u,  /* tr_group[11].output[13] */
    TRIG6_IN_TR_GROUP11_OUTPUT14    = 0x0616u,  /* tr_group[11].output[14] */
    TRIG6_IN_TR_GROUP11_OUTPUT15    = 0x0617u,  /* tr_group[11].output[15] */
    TRIG6_IN_TR_GROUP12_OUTPUT0     = 0x0618u,  /* tr_group[12].output[0] */
    TRIG6_IN_TR_GROUP12_OUTPUT1     = 0x0619u,  /* tr_group[12].output[1] */
    TRIG6_IN_TR_GROUP12_OUTPUT2     = 0x061Au,  /* tr_group[12].output[2] */
    TRIG6_IN_TR_GROUP12_OUTPUT3     = 0x061Bu,  /* tr_group[12].output[3] */
    TRIG6_IN_TR_GROUP12_OUTPUT4     = 0x061Cu,  /* tr_group[12].output[4] */
    TRIG6_IN_TR_GROUP12_OUTPUT5     = 0x061Du,  /* tr_group[12].output[5] */
    TRIG6_IN_TR_GROUP12_OUTPUT6     = 0x061Eu,  /* tr_group[12].output[6] */
    TRIG6_IN_TR_GROUP12_OUTPUT7     = 0x061Fu,  /* tr_group[12].output[7] */
    TRIG6_IN_TR_GROUP13_OUTPUT16    = 0x0620u,  /* tr_group[13].output[16] */
    TRIG6_IN_TR_GROUP13_OUTPUT17    = 0x0621u,  /* tr_group[13].output[17] */
    TRIG6_IN_TR_GROUP14_OUTPUT8     = 0x0622u,  /* tr_group[14].output[8] */
    TRIG6_IN_TR_GROUP14_OUTPUT9     = 0x0623u,  /* tr_group[14].output[9] */
    TRIG6_IN_TR_GROUP14_OUTPUT10    = 0x0624u,  /* tr_group[14].output[10] */
    TRIG6_IN_TR_GROUP14_OUTPUT11    = 0x0625u,  /* tr_group[14].output[11] */
    TRIG6_IN_TR_GROUP14_OUTPUT12    = 0x0626u,  /* tr_group[14].output[12] */
    TRIG6_IN_TR_GROUP14_OUTPUT13    = 0x0627u,  /* tr_group[14].output[13] */
    TRIG6_IN_TR_GROUP14_OUTPUT14    = 0x0628u,  /* tr_group[14].output[14] */
    TRIG6_IN_TR_GROUP14_OUTPUT15    = 0x0629u   /* tr_group[14].output[15] */
} en_trig_input_grp6_t;

/* Trigger Input Group 7 - UDB general purpose trigger multiplexer */
typedef enum
{
    TRIG7_IN_TR_GROUP10_OUTPUT0     = 0x0700u,  /* tr_group[10].output[0] */
    TRIG7_IN_TR_GROUP10_OUTPUT1     = 0x0701u,  /* tr_group[10].output[1] */
    TRIG7_IN_TR_GROUP10_OUTPUT2     = 0x0702u,  /* tr_group[10].output[2] */
    TRIG7_IN_TR_GROUP10_OUTPUT3     = 0x0703u,  /* tr_group[10].output[3] */
    TRIG7_IN_TR_GROUP10_OUTPUT4     = 0x0704u,  /* tr_group[10].output[4] */
    TRIG7_IN_TR_GROUP10_OUTPUT5     = 0x0705u,  /* tr_group[10].output[5] */
    TRIG7_IN_TR_GROUP10_OUTPUT6     = 0x0706u,  /* tr_group[10].output[6] */
    TRIG7_IN_TR_GROUP10_OUTPUT7     = 0x0707u,  /* tr_group[10].output[7] */
    TRIG7_IN_TR_GROUP11_OUTPUT0     = 0x0708u,  /* tr_group[11].output[0] */
    TRIG7_IN_TR_GROUP11_OUTPUT1     = 0x0709u,  /* tr_group[11].output[1] */
    TRIG7_IN_TR_GROUP11_OUTPUT2     = 0x070Au,  /* tr_group[11].output[2] */
    TRIG7_IN_TR_GROUP11_OUTPUT3     = 0x070Bu,  /* tr_group[11].output[3] */
    TRIG7_IN_TR_GROUP11_OUTPUT4     = 0x070Cu,  /* tr_group[11].output[4] */
    TRIG7_IN_TR_GROUP11_OUTPUT5     = 0x070Du,  /* tr_group[11].output[5] */
    TRIG7_IN_TR_GROUP11_OUTPUT6     = 0x070Eu,  /* tr_group[11].output[6] */
    TRIG7_IN_TR_GROUP11_OUTPUT7     = 0x070Fu,  /* tr_group[11].output[7] */
    TRIG7_IN_TR_GROUP11_OUTPUT8     = 0x0710u,  /* tr_group[11].output[8] */
    TRIG7_IN_TR_GROUP11_OUTPUT9     = 0x0711u,  /* tr_group[11].output[9] */
    TRIG7_IN_TR_GROUP11_OUTPUT10    = 0x0712u,  /* tr_group[11].output[10] */
    TRIG7_IN_TR_GROUP11_OUTPUT11    = 0x0713u,  /* tr_group[11].output[11] */
    TRIG7_IN_TR_GROUP11_OUTPUT12    = 0x0714u,  /* tr_group[11].output[12] */
    TRIG7_IN_TR_GROUP11_OUTPUT13    = 0x0715u,  /* tr_group[11].output[13] */
    TRIG7_IN_TR_GROUP11_OUTPUT14    = 0x0716u,  /* tr_group[11].output[14] */
    TRIG7_IN_TR_GROUP11_OUTPUT15    = 0x0717u,  /* tr_group[11].output[15] */
    TRIG7_IN_TR_GROUP12_OUTPUT0     = 0x0718u,  /* tr_group[12].output[0] */
    TRIG7_IN_TR_GROUP12_OUTPUT1     = 0x0719u,  /* tr_group[12].output[1] */
    TRIG7_IN_TR_GROUP12_OUTPUT2     = 0x071Au,  /* tr_group[12].output[2] */
    TRIG7_IN_TR_GROUP12_OUTPUT3     = 0x071Bu,  /* tr_group[12].output[3] */
    TRIG7_IN_TR_GROUP12_OUTPUT4     = 0x071Cu,  /* tr_group[12].output[4] */
    TRIG7_IN_TR_GROUP12_OUTPUT5     = 0x071Du,  /* tr_group[12].output[5] */
    TRIG7_IN_TR_GROUP12_OUTPUT6     = 0x071Eu,  /* tr_group[12].output[6] */
    TRIG7_IN_TR_GROUP12_OUTPUT7     = 0x071Fu,  /* tr_group[12].output[7] */
    TRIG7_IN_TR_GROUP13_OUTPUT16    = 0x0720u,  /* tr_group[13].output[16] */
    TRIG7_IN_TR_GROUP13_OUTPUT17    = 0x0721u,  /* tr_group[13].output[17] */
    TRIG7_IN_TR_GROUP14_OUTPUT8     = 0x0722u,  /* tr_group[14].output[8] */
    TRIG7_IN_TR_GROUP14_OUTPUT9     = 0x0723u,  /* tr_group[14].output[9] */
    TRIG7_IN_TR_GROUP14_OUTPUT10    = 0x0724u,  /* tr_group[14].output[10] */
    TRIG7_IN_TR_GROUP14_OUTPUT11    = 0x0725u,  /* tr_group[14].output[11] */
    TRIG7_IN_TR_GROUP14_OUTPUT12    = 0x0726u,  /* tr_group[14].output[12] */
    TRIG7_IN_TR_GROUP14_OUTPUT13    = 0x0727u,  /* tr_group[14].output[13] */
    TRIG7_IN_TR_GROUP14_OUTPUT14    = 0x0728u,  /* tr_group[14].output[14] */
    TRIG7_IN_TR_GROUP14_OUTPUT15    = 0x0729u   /* tr_group[14].output[15] */
} en_trig_input_grp7_t;

/* Trigger Input Group 8 - Trigger multiplexer to pins */
typedef enum
{
    TRIG8_IN_TR_GROUP10_OUTPUT0     = 0x0800u,  /* tr_group[10].output[0] */
    TRIG8_IN_TR_GROUP10_OUTPUT1     = 0x0801u,  /* tr_group[10].output[1] */
    TRIG8_IN_TR_GROUP10_OUTPUT2     = 0x0802u,  /* tr_group[10].output[2] */
    TRIG8_IN_TR_GROUP10_OUTPUT3     = 0x0803u,  /* tr_group[10].output[3] */
    TRIG8_IN_TR_GROUP10_OUTPUT4     = 0x0804u,  /* tr_group[10].output[4] */
    TRIG8_IN_TR_GROUP10_OUTPUT5     = 0x0805u,  /* tr_group[10].output[5] */
    TRIG8_IN_TR_GROUP10_OUTPUT6     = 0x0806u,  /* tr_group[10].output[6] */
    TRIG8_IN_TR_GROUP10_OUTPUT7     = 0x0807u,  /* tr_group[10].output[7] */
    TRIG8_IN_TR_GROUP11_OUTPUT0     = 0x0808u,  /* tr_group[11].output[0] */
    TRIG8_IN_TR_GROUP11_OUTPUT1     = 0x0809u,  /* tr_group[11].output[1] */
    TRIG8_IN_TR_GROUP11_OUTPUT2     = 0x080Au,  /* tr_group[11].output[2] */
    TRIG8_IN_TR_GROUP11_OUTPUT3     = 0x080Bu,  /* tr_group[11].output[3] */
    TRIG8_IN_TR_GROUP11_OUTPUT4     = 0x080Cu,  /* tr_group[11].output[4] */
    TRIG8_IN_TR_GROUP11_OUTPUT5     = 0x080Du,  /* tr_group[11].output[5] */
    TRIG8_IN_TR_GROUP11_OUTPUT6     = 0x080Eu,  /* tr_group[11].output[6] */
    TRIG8_IN_TR_GROUP11_OUTPUT7     = 0x080Fu,  /* tr_group[11].output[7] */
    TRIG8_IN_TR_GROUP11_OUTPUT8     = 0x0810u,  /* tr_group[11].output[8] */
    TRIG8_IN_TR_GROUP11_OUTPUT9     = 0x0811u,  /* tr_group[11].output[9] */
    TRIG8_IN_TR_GROUP11_OUTPUT10    = 0x0812u,  /* tr_group[11].output[10] */
    TRIG8_IN_TR_GROUP11_OUTPUT11    = 0x0813u,  /* tr_group[11].output[11] */
    TRIG8_IN_TR_GROUP11_OUTPUT12    = 0x0814u,  /* tr_group[11].output[12] */
    TRIG8_IN_TR_GROUP11_OUTPUT13    = 0x0815u,  /* tr_group[11].output[13] */
    TRIG8_IN_TR_GROUP11_OUTPUT14    = 0x0816u,  /* tr_group[11].output[14] */
    TRIG8_IN_TR_GROUP11_OUTPUT15    = 0x0817u,  /* tr_group[11].output[15] */
    TRIG8_IN_TR_GROUP12_OUTPUT0     = 0x0818u,  /* tr_group[12].output[0] */
    TRIG8_IN_TR_GROUP12_OUTPUT1     = 0x0819u,  /* tr_group[12].output[1] */
    TRIG8_IN_TR_GROUP12_OUTPUT2     = 0x081Au,  /* tr_group[12].output[2] */
    TRIG8_IN_TR_GROUP12_OUTPUT3     = 0x081Bu,  /* tr_group[12].output[3] */
    TRIG8_IN_TR_GROUP12_OUTPUT4     = 0x081Cu,  /* tr_group[12].output[4] */
    TRIG8_IN_TR_GROUP12_OUTPUT5     = 0x081Du,  /* tr_group[12].output[5] */
    TRIG8_IN_TR_GROUP12_OUTPUT6     = 0x081Eu,  /* tr_group[12].output[6] */
    TRIG8_IN_TR_GROUP12_OUTPUT7     = 0x081Fu,  /* tr_group[12].output[7] */
    TRIG8_IN_TR_GROUP13_OUTPUT16    = 0x0820u,  /* tr_group[13].output[16] */
    TRIG8_IN_TR_GROUP13_OUTPUT17    = 0x0821u,  /* tr_group[13].output[17] */
    TRIG8_IN_TR_GROUP14_OUTPUT8     = 0x0822u,  /* tr_group[14].output[8] */
    TRIG8_IN_TR_GROUP14_OUTPUT9     = 0x0823u,  /* tr_group[14].output[9] */
    TRIG8_IN_TR_GROUP14_OUTPUT10    = 0x0824u,  /* tr_group[14].output[10] */
    TRIG8_IN_TR_GROUP14_OUTPUT11    = 0x0825u,  /* tr_group[14].output[11] */
    TRIG8_IN_TR_GROUP14_OUTPUT12    = 0x0826u,  /* tr_group[14].output[12] */
    TRIG8_IN_TR_GROUP14_OUTPUT13    = 0x0827u,  /* tr_group[14].output[13] */
    TRIG8_IN_TR_GROUP14_OUTPUT14    = 0x0828u,  /* tr_group[14].output[14] */
    TRIG8_IN_TR_GROUP14_OUTPUT15    = 0x0829u   /* tr_group[14].output[15] */
} en_trig_input_grp8_t;

/* Trigger Input Group 9 - Feedback mux to USB DMA interface */
typedef enum
{
    TRIG9_IN_CPUSS_DW0_TR_OUT0      = 0x0900u,  /* cpuss.dw0_tr_out[0] */
    TRIG9_IN_CPUSS_DW0_TR_OUT1      = 0x0901u,  /* cpuss.dw0_tr_out[1] */
    TRIG9_IN_CPUSS_DW0_TR_OUT2      = 0x0902u,  /* cpuss.dw0_tr_out[2] */
    TRIG9_IN_CPUSS_DW0_TR_OUT3      = 0x0903u,  /* cpuss.dw0_tr_out[3] */
    TRIG9_IN_CPUSS_DW0_TR_OUT4      = 0x0904u,  /* cpuss.dw0_tr_out[4] */
    TRIG9_IN_CPUSS_DW0_TR_OUT5      = 0x0905u,  /* cpuss.dw0_tr_out[5] */
    TRIG9_IN_CPUSS_DW0_TR_OUT6      = 0x0906u,  /* cpuss.dw0_tr_out[6] */
    TRIG9_IN_CPUSS_DW0_TR_OUT7      = 0x0907u,  /* cpuss.dw0_tr_out[7] */
    TRIG9_IN_CPUSS_DW0_TR_OUT8      = 0x0908u,  /* cpuss.dw0_tr_out[8] */
    TRIG9_IN_CPUSS_DW0_TR_OUT9      = 0x0909u,  /* cpuss.dw0_tr_out[9] */
    TRIG9_IN_CPUSS_DW0_TR_OUT10     = 0x090Au,  /* cpuss.dw0_tr_out[10] */
    TRIG9_IN_CPUSS_DW0_TR_OUT11     = 0x090Bu,  /* cpuss.dw0_tr_out[11] */
    TRIG9_IN_CPUSS_DW0_TR_OUT12     = 0x090Cu,  /* cpuss.dw0_tr_out[12] */
    TRIG9_IN_CPUSS_DW0_TR_OUT13     = 0x090Du,  /* cpuss.dw0_tr_out[13] */
    TRIG9_IN_CPUSS_DW0_TR_OUT14     = 0x090Eu,  /* cpuss.dw0_tr_out[14] */
    TRIG9_IN_CPUSS_DW0_TR_OUT15     = 0x090Fu,  /* cpuss.dw0_tr_out[15] */
    TRIG9_IN_CPUSS_DW1_TR_OUT0      = 0x0910u,  /* cpuss.dw1_tr_out[0] */
    TRIG9_IN_CPUSS_DW1_TR_OUT1      = 0x0911u,  /* cpuss.dw1_tr_out[1] */
    TRIG9_IN_CPUSS_DW1_TR_OUT2      = 0x0912u,  /* cpuss.dw1_tr_out[2] */
    TRIG9_IN_CPUSS_DW1_TR_OUT3      = 0x0913u,  /* cpuss.dw1_tr_out[3] */
    TRIG9_IN_CPUSS_DW1_TR_OUT4      = 0x0914u,  /* cpuss.dw1_tr_out[4] */
    TRIG9_IN_CPUSS_DW1_TR_OUT5      = 0x0915u,  /* cpuss.dw1_tr_out[5] */
    TRIG9_IN_CPUSS_DW1_TR_OUT6      = 0x0916u,  /* cpuss.dw1_tr_out[6] */
    TRIG9_IN_CPUSS_DW1_TR_OUT7      = 0x0917u,  /* cpuss.dw1_tr_out[7] */
    TRIG9_IN_CPUSS_DW1_TR_OUT8      = 0x0918u,  /* cpuss.dw1_tr_out[8] */
    TRIG9_IN_CPUSS_DW1_TR_OUT9      = 0x0919u,  /* cpuss.dw1_tr_out[9] */
    TRIG9_IN_CPUSS_DW1_TR_OUT10     = 0x091Au,  /* cpuss.dw1_tr_out[10] */
    TRIG9_IN_CPUSS_DW1_TR_OUT11     = 0x091Bu,  /* cpuss.dw1_tr_out[11] */
    TRIG9_IN_CPUSS_DW1_TR_OUT12     = 0x091Cu,  /* cpuss.dw1_tr_out[12] */
    TRIG9_IN_CPUSS_DW1_TR_OUT13     = 0x091Du,  /* cpuss.dw1_tr_out[13] */
    TRIG9_IN_CPUSS_DW1_TR_OUT14     = 0x091Eu,  /* cpuss.dw1_tr_out[14] */
    TRIG9_IN_CPUSS_DW1_TR_OUT15     = 0x091Fu   /* cpuss.dw1_tr_out[15] */
} en_trig_input_grp9_t;

/* Trigger Input Group 10 - Reduces 32 datawire output triggers to 8 signals, used by all except USB */
typedef enum
{
    TRIG10_IN_CPUSS_DW0_TR_OUT0     = 0x0A00u,  /* cpuss.dw0_tr_out[0] */
    TRIG10_IN_CPUSS_DW0_TR_OUT1     = 0x0A01u,  /* cpuss.dw0_tr_out[1] */
    TRIG10_IN_CPUSS_DW0_TR_OUT2     = 0x0A02u,  /* cpuss.dw0_tr_out[2] */
    TRIG10_IN_CPUSS_DW0_TR_OUT3     = 0x0A03u,  /* cpuss.dw0_tr_out[3] */
    TRIG10_IN_CPUSS_DW0_TR_OUT4     = 0x0A04u,  /* cpuss.dw0_tr_out[4] */
    TRIG10_IN_CPUSS_DW0_TR_OUT5     = 0x0A05u,  /* cpuss.dw0_tr_out[5] */
    TRIG10_IN_CPUSS_DW0_TR_OUT6     = 0x0A06u,  /* cpuss.dw0_tr_out[6] */
    TRIG10_IN_CPUSS_DW0_TR_OUT7     = 0x0A07u,  /* cpuss.dw0_tr_out[7] */
    TRIG10_IN_CPUSS_DW0_TR_OUT8     = 0x0A08u,  /* cpuss.dw0_tr_out[8] */
    TRIG10_IN_CPUSS_DW0_TR_OUT9     = 0x0A09u,  /* cpuss.dw0_tr_out[9] */
    TRIG10_IN_CPUSS_DW0_TR_OUT10    = 0x0A0Au,  /* cpuss.dw0_tr_out[10] */
    TRIG10_IN_CPUSS_DW0_TR_OUT11    = 0x0A0Bu,  /* cpuss.dw0_tr_out[11] */
    TRIG10_IN_CPUSS_DW0_TR_OUT12    = 0x0A0Cu,  /* cpuss.dw0_tr_out[12] */
    TRIG10_IN_CPUSS_DW0_TR_OUT13    = 0x0A0Du,  /* cpuss.dw0_tr_out[13] */
    TRIG10_IN_CPUSS_DW0_TR_OUT14    = 0x0A0Eu,  /* cpuss.dw0_tr_out[14] */
    TRIG10_IN_CPUSS_DW0_TR_OUT15    = 0x0A0Fu,  /* cpuss.dw0_tr_out[15] */
    TRIG10_IN_CPUSS_DW1_TR_OUT0     = 0x0A10u,  /* cpuss.dw1_tr_out[0] */
    TRIG10_IN_CPUSS_DW1_TR_OUT1     = 0x0A11u,  /* cpuss.dw1_tr_out[1] */
    TRIG10_IN_CPUSS_DW1_TR_OUT2     = 0x0A12u,  /* cpuss.dw1_tr_out[2] */
    TRIG10_IN_CPUSS_DW1_TR_OUT3     = 0x0A13u,  /* cpuss.dw1_tr_out[3] */
    TRIG10_IN_CPUSS_DW1_TR_OUT4     = 0x0A14u,  /* cpuss.dw1_tr_out[4] */
    TRIG10_IN_CPUSS_DW1_TR_OUT5     = 0x0A15u,  /* cpuss.dw1_tr_out[5] */
    TRIG10_IN_CPUSS_DW1_TR_OUT6     = 0x0A16u,  /* cpuss.dw1_tr_out[6] */
    TRIG10_IN_CPUSS_DW1_TR_OUT7     = 0x0A17u,  /* cpuss.dw1_tr_out[7] */
    TRIG10_IN_CPUSS_DW1_TR_OUT8     = 0x0A18u,  /* cpuss.dw1_tr_out[8] */
    TRIG10_IN_CPUSS_DW1_TR_OUT9     = 0x0A19u,  /* cpuss.dw1_tr_out[9] */
    TRIG10_IN_CPUSS_DW1_TR_OUT10    = 0x0A1Au,  /* cpuss.dw1_tr_out[10] */
    TRIG10_IN_CPUSS_DW1_TR_OUT11    = 0x0A1Bu,  /* cpuss.dw1_tr_out[11] */
    TRIG10_IN_CPUSS_DW1_TR_OUT12    = 0x0A1Cu,  /* cpuss.dw1_tr_out[12] */
    TRIG10_IN_CPUSS_DW1_TR_OUT13    = 0x0A1Du,  /* cpuss.dw1_tr_out[13] */
    TRIG10_IN_CPUSS_DW1_TR_OUT14    = 0x0A1Eu,  /* cpuss.dw1_tr_out[14] */
    TRIG10_IN_CPUSS_DW1_TR_OUT15    = 0x0A1Fu   /* cpuss.dw1_tr_out[15] */
} en_trig_input_grp10_t;

/* Trigger Input Group 11 - Reduces 96 tcpwm output triggers to 16 signals, used by all sinks */
typedef enum
{
    TRIG11_IN_TCPWM0_TR_OVERFLOW0   = 0x0B00u,  /* tcpwm[0].tr_overflow[0] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW1   = 0x0B01u,  /* tcpwm[0].tr_overflow[1] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW2   = 0x0B02u,  /* tcpwm[0].tr_overflow[2] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW3   = 0x0B03u,  /* tcpwm[0].tr_overflow[3] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW4   = 0x0B04u,  /* tcpwm[0].tr_overflow[4] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW5   = 0x0B05u,  /* tcpwm[0].tr_overflow[5] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW6   = 0x0B06u,  /* tcpwm[0].tr_overflow[6] */
    TRIG11_IN_TCPWM0_TR_OVERFLOW7   = 0x0B07u,  /* tcpwm[0].tr_overflow[7] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH0 = 0x0B08u, /* tcpwm[0].tr_compare_match[0] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH1 = 0x0B09u, /* tcpwm[0].tr_compare_match[1] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH2 = 0x0B0Au, /* tcpwm[0].tr_compare_match[2] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH3 = 0x0B0Bu, /* tcpwm[0].tr_compare_match[3] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH4 = 0x0B0Cu, /* tcpwm[0].tr_compare_match[4] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH5 = 0x0B0Du, /* tcpwm[0].tr_compare_match[5] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH6 = 0x0B0Eu, /* tcpwm[0].tr_compare_match[6] */
    TRIG11_IN_TCPWM0_TR_COMPARE_MATCH7 = 0x0B0Fu, /* tcpwm[0].tr_compare_match[7] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW0  = 0x0B10u,  /* tcpwm[0].tr_underflow[0] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW1  = 0x0B11u,  /* tcpwm[0].tr_underflow[1] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW2  = 0x0B12u,  /* tcpwm[0].tr_underflow[2] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW3  = 0x0B13u,  /* tcpwm[0].tr_underflow[3] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW4  = 0x0B14u,  /* tcpwm[0].tr_underflow[4] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW5  = 0x0B15u,  /* tcpwm[0].tr_underflow[5] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW6  = 0x0B16u,  /* tcpwm[0].tr_underflow[6] */
    TRIG11_IN_TCPWM0_TR_UNDERFLOW7  = 0x0B17u,  /* tcpwm[0].tr_underflow[7] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW0   = 0x0B18u,  /* tcpwm[1].tr_overflow[0] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW1   = 0x0B19u,  /* tcpwm[1].tr_overflow[1] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW2   = 0x0B1Au,  /* tcpwm[1].tr_overflow[2] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW3   = 0x0B1Bu,  /* tcpwm[1].tr_overflow[3] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW4   = 0x0B1Cu,  /* tcpwm[1].tr_overflow[4] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW5   = 0x0B1Du,  /* tcpwm[1].tr_overflow[5] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW6   = 0x0B1Eu,  /* tcpwm[1].tr_overflow[6] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW7   = 0x0B1Fu,  /* tcpwm[1].tr_overflow[7] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW8   = 0x0B20u,  /* tcpwm[1].tr_overflow[8] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW9   = 0x0B21u,  /* tcpwm[1].tr_overflow[9] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW10  = 0x0B22u,  /* tcpwm[1].tr_overflow[10] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW11  = 0x0B23u,  /* tcpwm[1].tr_overflow[11] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW12  = 0x0B24u,  /* tcpwm[1].tr_overflow[12] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW13  = 0x0B25u,  /* tcpwm[1].tr_overflow[13] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW14  = 0x0B26u,  /* tcpwm[1].tr_overflow[14] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW15  = 0x0B27u,  /* tcpwm[1].tr_overflow[15] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW16  = 0x0B28u,  /* tcpwm[1].tr_overflow[16] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW17  = 0x0B29u,  /* tcpwm[1].tr_overflow[17] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW18  = 0x0B2Au,  /* tcpwm[1].tr_overflow[18] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW19  = 0x0B2Bu,  /* tcpwm[1].tr_overflow[19] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW20  = 0x0B2Cu,  /* tcpwm[1].tr_overflow[20] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW21  = 0x0B2Du,  /* tcpwm[1].tr_overflow[21] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW22  = 0x0B2Eu,  /* tcpwm[1].tr_overflow[22] */
    TRIG11_IN_TCPWM1_TR_OVERFLOW23  = 0x0B2Fu,  /* tcpwm[1].tr_overflow[23] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH0 = 0x0B30u, /* tcpwm[1].tr_compare_match[0] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH1 = 0x0B31u, /* tcpwm[1].tr_compare_match[1] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH2 = 0x0B32u, /* tcpwm[1].tr_compare_match[2] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH3 = 0x0B33u, /* tcpwm[1].tr_compare_match[3] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH4 = 0x0B34u, /* tcpwm[1].tr_compare_match[4] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH5 = 0x0B35u, /* tcpwm[1].tr_compare_match[5] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH6 = 0x0B36u, /* tcpwm[1].tr_compare_match[6] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH7 = 0x0B37u, /* tcpwm[1].tr_compare_match[7] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH8 = 0x0B38u, /* tcpwm[1].tr_compare_match[8] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH9 = 0x0B39u, /* tcpwm[1].tr_compare_match[9] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH10 = 0x0B3Au, /* tcpwm[1].tr_compare_match[10] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH11 = 0x0B3Bu, /* tcpwm[1].tr_compare_match[11] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH12 = 0x0B3Cu, /* tcpwm[1].tr_compare_match[12] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH13 = 0x0B3Du, /* tcpwm[1].tr_compare_match[13] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH14 = 0x0B3Eu, /* tcpwm[1].tr_compare_match[14] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH15 = 0x0B3Fu, /* tcpwm[1].tr_compare_match[15] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH16 = 0x0B40u, /* tcpwm[1].tr_compare_match[16] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH17 = 0x0B41u, /* tcpwm[1].tr_compare_match[17] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH18 = 0x0B42u, /* tcpwm[1].tr_compare_match[18] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH19 = 0x0B43u, /* tcpwm[1].tr_compare_match[19] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH20 = 0x0B44u, /* tcpwm[1].tr_compare_match[20] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH21 = 0x0B45u, /* tcpwm[1].tr_compare_match[21] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH22 = 0x0B46u, /* tcpwm[1].tr_compare_match[22] */
    TRIG11_IN_TCPWM1_TR_COMPARE_MATCH23 = 0x0B47u, /* tcpwm[1].tr_compare_match[23] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW0  = 0x0B48u,  /* tcpwm[1].tr_underflow[0] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW1  = 0x0B49u,  /* tcpwm[1].tr_underflow[1] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW2  = 0x0B4Au,  /* tcpwm[1].tr_underflow[2] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW3  = 0x0B4Bu,  /* tcpwm[1].tr_underflow[3] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW4  = 0x0B4Cu,  /* tcpwm[1].tr_underflow[4] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW5  = 0x0B4Du,  /* tcpwm[1].tr_underflow[5] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW6  = 0x0B4Eu,  /* tcpwm[1].tr_underflow[6] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW7  = 0x0B4Fu,  /* tcpwm[1].tr_underflow[7] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW8  = 0x0B50u,  /* tcpwm[1].tr_underflow[8] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW9  = 0x0B51u,  /* tcpwm[1].tr_underflow[9] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW10 = 0x0B52u,  /* tcpwm[1].tr_underflow[10] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW11 = 0x0B53u,  /* tcpwm[1].tr_underflow[11] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW12 = 0x0B54u,  /* tcpwm[1].tr_underflow[12] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW13 = 0x0B55u,  /* tcpwm[1].tr_underflow[13] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW14 = 0x0B56u,  /* tcpwm[1].tr_underflow[14] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW15 = 0x0B57u,  /* tcpwm[1].tr_underflow[15] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW16 = 0x0B58u,  /* tcpwm[1].tr_underflow[16] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW17 = 0x0B59u,  /* tcpwm[1].tr_underflow[17] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW18 = 0x0B5Au,  /* tcpwm[1].tr_underflow[18] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW19 = 0x0B5Bu,  /* tcpwm[1].tr_underflow[19] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW20 = 0x0B5Cu,  /* tcpwm[1].tr_underflow[20] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW21 = 0x0B5Du,  /* tcpwm[1].tr_underflow[21] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW22 = 0x0B5Eu,  /* tcpwm[1].tr_underflow[22] */
    TRIG11_IN_TCPWM1_TR_UNDERFLOW23 = 0x0B5Fu   /* tcpwm[1].tr_underflow[23] */
} en_trig_input_grp11_t;

/* Trigger Input Group 12 - Reduces 28 pin input signals to 10 triggers used by all sinks */
typedef enum
{
    TRIG12_IN_PERI_TR_IO_INPUT0     = 0x0C00u,  /* peri.tr_io_input[0] */
    TRIG12_IN_PERI_TR_IO_INPUT1     = 0x0C01u,  /* peri.tr_io_input[1] */
    TRIG12_IN_PERI_TR_IO_INPUT2     = 0x0C02u,  /* peri.tr_io_input[2] */
    TRIG12_IN_PERI_TR_IO_INPUT3     = 0x0C03u,  /* peri.tr_io_input[3] */
    TRIG12_IN_PERI_TR_IO_INPUT4     = 0x0C04u,  /* peri.tr_io_input[4] */
    TRIG12_IN_PERI_TR_IO_INPUT5     = 0x0C05u,  /* peri.tr_io_input[5] */
    TRIG12_IN_PERI_TR_IO_INPUT6     = 0x0C06u,  /* peri.tr_io_input[6] */
    TRIG12_IN_PERI_TR_IO_INPUT7     = 0x0C07u,  /* peri.tr_io_input[7] */
    TRIG12_IN_PERI_TR_IO_INPUT8     = 0x0C08u,  /* peri.tr_io_input[8] */
    TRIG12_IN_PERI_TR_IO_INPUT9     = 0x0C09u,  /* peri.tr_io_input[9] */
    TRIG12_IN_PERI_TR_IO_INPUT10    = 0x0C0Au,  /* peri.tr_io_input[10] */
    TRIG12_IN_PERI_TR_IO_INPUT11    = 0x0C0Bu,  /* peri.tr_io_input[11] */
    TRIG12_IN_PERI_TR_IO_INPUT12    = 0x0C0Cu,  /* peri.tr_io_input[12] */
    TRIG12_IN_PERI_TR_IO_INPUT13    = 0x0C0Du,  /* peri.tr_io_input[13] */
    TRIG12_IN_PERI_TR_IO_INPUT14    = 0x0C0Eu,  /* peri.tr_io_input[14] */
    TRIG12_IN_PERI_TR_IO_INPUT15    = 0x0C0Fu,  /* peri.tr_io_input[15] */
    TRIG12_IN_PERI_TR_IO_INPUT16    = 0x0C10u,  /* peri.tr_io_input[16] */
    TRIG12_IN_PERI_TR_IO_INPUT17    = 0x0C11u,  /* peri.tr_io_input[17] */
    TRIG12_IN_PERI_TR_IO_INPUT18    = 0x0C12u,  /* peri.tr_io_input[18] */
    TRIG12_IN_PERI_TR_IO_INPUT19    = 0x0C13u,  /* peri.tr_io_input[19] */
    TRIG12_IN_PERI_TR_IO_INPUT20    = 0x0C14u,  /* peri.tr_io_input[20] */
    TRIG12_IN_PERI_TR_IO_INPUT21    = 0x0C15u,  /* peri.tr_io_input[21] */
    TRIG12_IN_PERI_TR_IO_INPUT22    = 0x0C16u,  /* peri.tr_io_input[22] */
    TRIG12_IN_PERI_TR_IO_INPUT23    = 0x0C17u,  /* peri.tr_io_input[23] */
    TRIG12_IN_PERI_TR_IO_INPUT24    = 0x0C18u,  /* peri.tr_io_input[24] */
    TRIG12_IN_PERI_TR_IO_INPUT25    = 0x0C19u,  /* peri.tr_io_input[25] */
    TRIG12_IN_PERI_TR_IO_INPUT26    = 0x0C1Au,  /* peri.tr_io_input[26] */
    TRIG12_IN_PERI_TR_IO_INPUT27    = 0x0C1Bu   /* peri.tr_io_input[27] */
} en_trig_input_grp12_t;

/* Trigger Input Group 13 - Reduces DMA requests to 16+2 outputs used by all sinks */
typedef enum
{
    TRIG13_IN_SCB0_TR_TX_REQ        = 0x0D00u,  /* scb[0].tr_tx_req */
    TRIG13_IN_SCB0_TR_RX_REQ        = 0x0D01u,  /* scb[0].tr_rx_req */
    TRIG13_IN_SCB1_TR_TX_REQ        = 0x0D02u,  /* scb[1].tr_tx_req */
    TRIG13_IN_SCB1_TR_RX_REQ        = 0x0D03u,  /* scb[1].tr_rx_req */
    TRIG13_IN_SCB2_TR_TX_REQ        = 0x0D04u,  /* scb[2].tr_tx_req */
    TRIG13_IN_SCB2_TR_RX_REQ        = 0x0D05u,  /* scb[2].tr_rx_req */
    TRIG13_IN_SCB3_TR_TX_REQ        = 0x0D06u,  /* scb[3].tr_tx_req */
    TRIG13_IN_SCB3_TR_RX_REQ        = 0x0D07u,  /* scb[3].tr_rx_req */
    TRIG13_IN_SCB4_TR_TX_REQ        = 0x0D08u,  /* scb[4].tr_tx_req */
    TRIG13_IN_SCB4_TR_RX_REQ        = 0x0D09u,  /* scb[4].tr_rx_req */
    TRIG13_IN_SCB5_TR_TX_REQ        = 0x0D0Au,  /* scb[5].tr_tx_req */
    TRIG13_IN_SCB5_TR_RX_REQ        = 0x0D0Bu,  /* scb[5].tr_rx_req */
    TRIG13_IN_SCB6_TR_TX_REQ        = 0x0D0Cu,  /* scb[6].tr_tx_req */
    TRIG13_IN_SCB6_TR_RX_REQ        = 0x0D0Du,  /* scb[6].tr_rx_req */
    TRIG13_IN_SCB7_TR_TX_REQ        = 0x0D0Eu,  /* scb[7].tr_tx_req */
    TRIG13_IN_SCB7_TR_RX_REQ        = 0x0D0Fu,  /* scb[7].tr_rx_req */
    TRIG13_IN_SCB8_TR_TX_REQ        = 0x0D10u,  /* scb[8].tr_tx_req */
    TRIG13_IN_SCB8_TR_RX_REQ        = 0x0D11u,  /* scb[8].tr_rx_req */
    TRIG13_IN_AUDIOSS_TR_PDM_RX_REQ = 0x0D12u,  /* audioss.tr_pdm_rx_req */
    TRIG13_IN_AUDIOSS_TR_I2S_TX_REQ = 0x0D13u,  /* audioss.tr_i2s_tx_req */
    TRIG13_IN_AUDIOSS_TR_I2S_RX_REQ = 0x0D14u,  /* audioss.tr_i2s_rx_req */
    TRIG13_IN_SMIF_TR_TX_REQ        = 0x0D15u,  /* smif.tr_tx_req */
    TRIG13_IN_SMIF_TR_RX_REQ        = 0x0D16u,  /* smif.tr_rx_req */
    TRIG13_IN_USB_DMA_REQ0          = 0x0D17u,  /* usb.dma_req[0] */
    TRIG13_IN_USB_DMA_REQ1          = 0x0D18u,  /* usb.dma_req[1] */
    TRIG13_IN_USB_DMA_REQ2          = 0x0D19u,  /* usb.dma_req[2] */
    TRIG13_IN_USB_DMA_REQ3          = 0x0D1Au,  /* usb.dma_req[3] */
    TRIG13_IN_USB_DMA_REQ4          = 0x0D1Bu,  /* usb.dma_req[4] */
    TRIG13_IN_USB_DMA_REQ5          = 0x0D1Cu,  /* usb.dma_req[5] */
    TRIG13_IN_USB_DMA_REQ6          = 0x0D1Du,  /* usb.dma_req[6] */
    TRIG13_IN_USB_DMA_REQ7          = 0x0D1Eu   /* usb.dma_req[7] */
} en_trig_input_grp13_t;

/* Trigger Input Group 14 - Reduces general purpose trigger inputs to 8+8 outputs used by all sinks */
typedef enum
{
    TRIG14_IN_UDB_TR_UDB0           = 0x0E00u,  /* udb.tr_udb[0] */
    TRIG14_IN_UDB_TR_UDB1           = 0x0E01u,  /* udb.tr_udb[1] */
    TRIG14_IN_UDB_TR_UDB2           = 0x0E02u,  /* udb.tr_udb[2] */
    TRIG14_IN_UDB_TR_UDB3           = 0x0E03u,  /* udb.tr_udb[3] */
    TRIG14_IN_UDB_TR_UDB4           = 0x0E04u,  /* udb.tr_udb[4] */
    TRIG14_IN_UDB_TR_UDB5           = 0x0E05u,  /* udb.tr_udb[5] */
    TRIG14_IN_UDB_TR_UDB6           = 0x0E06u,  /* udb.tr_udb[6] */
    TRIG14_IN_UDB_TR_UDB7           = 0x0E07u,  /* udb.tr_udb[7] */
    TRIG14_IN_UDB_TR_UDB8           = 0x0E08u,  /* udb.tr_udb[8] */
    TRIG14_IN_UDB_TR_UDB9           = 0x0E09u,  /* udb.tr_udb[9] */
    TRIG14_IN_UDB_TR_UDB10          = 0x0E0Au,  /* udb.tr_udb[10] */
    TRIG14_IN_UDB_TR_UDB11          = 0x0E0Bu,  /* udb.tr_udb[11] */
    TRIG14_IN_UDB_TR_UDB12          = 0x0E0Cu,  /* udb.tr_udb[12] */
    TRIG14_IN_UDB_TR_UDB13          = 0x0E0Du,  /* udb.tr_udb[13] */
    TRIG14_IN_UDB_TR_UDB14          = 0x0E0Eu,  /* udb.tr_udb[14] */
    TRIG14_IN_UDB_TR_UDB15          = 0x0E0Fu,  /* udb.tr_udb[15] */
    TRIG14_IN_UDB_DSI_OUT_TR0       = 0x0E10u,  /* udb.dsi_out_tr[0] */
    TRIG14_IN_UDB_DSI_OUT_TR1       = 0x0E11u,  /* udb.dsi_out_tr[1] */
    TRIG14_IN_CPUSS_CTI_TR_OUT0     = 0x0E12u,  /* cpuss.cti_tr_out[0] */
    TRIG14_IN_CPUSS_CTI_TR_OUT1     = 0x0E13u,  /* cpuss.cti_tr_out[1] */
    TRIG14_IN_PASS_TR_SAR_OUT       = 0x0E14u,  /* pass.tr_sar_out */
    TRIG14_IN_PASS_TR_CTDAC_EMPTY   = 0x0E15u,  /* pass.tr_ctdac_empty */
    TRIG14_IN_PASS_DSI_CTB_CMP0     = 0x0E16u,  /* pass.dsi_ctb_cmp0 */
    TRIG14_IN_PASS_DSI_CTB_CMP1     = 0x0E17u,  /* pass.dsi_ctb_cmp1 */
    TRIG14_IN_LPCOMP_DSI_COMP0      = 0x0E18u,  /* lpcomp.dsi_comp0 */
    TRIG14_IN_LPCOMP_DSI_COMP1      = 0x0E19u,  /* lpcomp.dsi_comp1 */
    TRIG14_IN_SCB0_TR_I2C_SCL_FILTERED = 0x0E1Au, /* scb[0].tr_i2c_scl_filtered */
    TRIG14_IN_SCB1_TR_I2C_SCL_FILTERED = 0x0E1Bu, /* scb[1].tr_i2c_scl_filtered */
    TRIG14_IN_SCB2_TR_I2C_SCL_FILTERED = 0x0E1Cu, /* scb[2].tr_i2c_scl_filtered */
    TRIG14_IN_SCB3_TR_I2C_SCL_FILTERED = 0x0E1Du, /* scb[3].tr_i2c_scl_filtered */
    TRIG14_IN_SCB4_TR_I2C_SCL_FILTERED = 0x0E1Eu, /* scb[4].tr_i2c_scl_filtered */
    TRIG14_IN_SCB5_TR_I2C_SCL_FILTERED = 0x0E1Fu, /* scb[5].tr_i2c_scl_filtered */
    TRIG14_IN_SCB6_TR_I2C_SCL_FILTERED = 0x0E20u, /* scb[6].tr_i2c_scl_filtered */
    TRIG14_IN_SCB7_TR_I2C_SCL_FILTERED = 0x0E21u, /* scb[7].tr_i2c_scl_filtered */
    TRIG14_IN_SCB8_TR_I2C_SCL_FILTERED = 0x0E22u, /* scb[8].tr_i2c_scl_filtered */
    TRIG14_IN_CPUSS_TR_FAULT0       = 0x0E23u,  /* cpuss.tr_fault[0] */
    TRIG14_IN_CPUSS_TR_FAULT1       = 0x0E24u   /* cpuss.tr_fault[1] */
} en_trig_input_grp14_t;

/* Trigger Group Outputs */
/* Trigger Output Group 0 - DMA Request Assignments */
typedef enum
{
    TRIG0_OUT_CPUSS_DW0_TR_IN0      = 0x1000u,  /* cpuss.dw0_tr_in[0] */
    TRIG0_OUT_CPUSS_DW0_TR_IN1      = 0x1001u,  /* cpuss.dw0_tr_in[1] */
    TRIG0_OUT_CPUSS_DW0_TR_IN2      = 0x1002u,  /* cpuss.dw0_tr_in[2] */
    TRIG0_OUT_CPUSS_DW0_TR_IN3      = 0x1003u,  /* cpuss.dw0_tr_in[3] */
    TRIG0_OUT_CPUSS_DW0_TR_IN4      = 0x1004u,  /* cpuss.dw0_tr_in[4] */
    TRIG0_OUT_CPUSS_DW0_TR_IN5      = 0x1005u,  /* cpuss.dw0_tr_in[5] */
    TRIG0_OUT_CPUSS_DW0_TR_IN6      = 0x1006u,  /* cpuss.dw0_tr_in[6] */
    TRIG0_OUT_CPUSS_DW0_TR_IN7      = 0x1007u,  /* cpuss.dw0_tr_in[7] */
    TRIG0_OUT_CPUSS_DW0_TR_IN8      = 0x1008u,  /* cpuss.dw0_tr_in[8] */
    TRIG0_OUT_CPUSS_DW0_TR_IN9      = 0x1009u,  /* cpuss.dw0_tr_in[9] */
    TRIG0_OUT_CPUSS_DW0_TR_IN10     = 0x100Au,  /* cpuss.dw0_tr_in[10] */
    TRIG0_OUT_CPUSS_DW0_TR_IN11     = 0x100Bu,  /* cpuss.dw0_tr_in[11] */
    TRIG0_OUT_CPUSS_DW0_TR_IN12     = 0x100Cu,  /* cpuss.dw0_tr_in[12] */
    TRIG0_OUT_CPUSS_DW0_TR_IN13     = 0x100Du,  /* cpuss.dw0_tr_in[13] */
    TRIG0_OUT_CPUSS_DW0_TR_IN14     = 0x100Eu,  /* cpuss.dw0_tr_in[14] */
    TRIG0_OUT_CPUSS_DW0_TR_IN15     = 0x100Fu   /* cpuss.dw0_tr_in[15] */
} en_trig_output_grp0_t;

/* Trigger Output Group 1 - DMA Request Assignments */
typedef enum
{
    TRIG1_OUT_CPUSS_DW1_TR_IN0      = 0x1100u,  /* cpuss.dw1_tr_in[0] */
    TRIG1_OUT_CPUSS_DW1_TR_IN1      = 0x1101u,  /* cpuss.dw1_tr_in[1] */
    TRIG1_OUT_CPUSS_DW1_TR_IN2      = 0x1102u,  /* cpuss.dw1_tr_in[2] */
    TRIG1_OUT_CPUSS_DW1_TR_IN3      = 0x1103u,  /* cpuss.dw1_tr_in[3] */
    TRIG1_OUT_CPUSS_DW1_TR_IN4      = 0x1104u,  /* cpuss.dw1_tr_in[4] */
    TRIG1_OUT_CPUSS_DW1_TR_IN5      = 0x1105u,  /* cpuss.dw1_tr_in[5] */
    TRIG1_OUT_CPUSS_DW1_TR_IN6      = 0x1106u,  /* cpuss.dw1_tr_in[6] */
    TRIG1_OUT_CPUSS_DW1_TR_IN7      = 0x1107u,  /* cpuss.dw1_tr_in[7] */
    TRIG1_OUT_CPUSS_DW1_TR_IN8      = 0x1108u,  /* cpuss.dw1_tr_in[8] */
    TRIG1_OUT_CPUSS_DW1_TR_IN9      = 0x1109u,  /* cpuss.dw1_tr_in[9] */
    TRIG1_OUT_CPUSS_DW1_TR_IN10     = 0x110Au,  /* cpuss.dw1_tr_in[10] */
    TRIG1_OUT_CPUSS_DW1_TR_IN11     = 0x110Bu,  /* cpuss.dw1_tr_in[11] */
    TRIG1_OUT_CPUSS_DW1_TR_IN12     = 0x110Cu,  /* cpuss.dw1_tr_in[12] */
    TRIG1_OUT_CPUSS_DW1_TR_IN13     = 0x110Du,  /* cpuss.dw1_tr_in[13] */
    TRIG1_OUT_CPUSS_DW1_TR_IN14     = 0x110Eu,  /* cpuss.dw1_tr_in[14] */
    TRIG1_OUT_CPUSS_DW1_TR_IN15     = 0x110Fu   /* cpuss.dw1_tr_in[15] */
} en_trig_output_grp1_t;

/* Trigger Output Group 2 - TCPWM trigger inputs */
typedef enum
{
    TRIG2_OUT_TCPWM0_TR_IN0         = 0x1200u,  /* tcpwm[0].tr_in[0] */
    TRIG2_OUT_TCPWM0_TR_IN1         = 0x1201u,  /* tcpwm[0].tr_in[1] */
    TRIG2_OUT_TCPWM0_TR_IN2         = 0x1202u,  /* tcpwm[0].tr_in[2] */
    TRIG2_OUT_TCPWM0_TR_IN3         = 0x1203u,  /* tcpwm[0].tr_in[3] */
    TRIG2_OUT_TCPWM0_TR_IN4         = 0x1204u,  /* tcpwm[0].tr_in[4] */
    TRIG2_OUT_TCPWM0_TR_IN5         = 0x1205u,  /* tcpwm[0].tr_in[5] */
    TRIG2_OUT_TCPWM0_TR_IN6         = 0x1206u,  /* tcpwm[0].tr_in[6] */
    TRIG2_OUT_TCPWM0_TR_IN7         = 0x1207u,  /* tcpwm[0].tr_in[7] */
    TRIG2_OUT_TCPWM0_TR_IN8         = 0x1208u,  /* tcpwm[0].tr_in[8] */
    TRIG2_OUT_TCPWM0_TR_IN9         = 0x1209u,  /* tcpwm[0].tr_in[9] */
    TRIG2_OUT_TCPWM0_TR_IN10        = 0x120Au,  /* tcpwm[0].tr_in[10] */
    TRIG2_OUT_TCPWM0_TR_IN11        = 0x120Bu,  /* tcpwm[0].tr_in[11] */
    TRIG2_OUT_TCPWM0_TR_IN12        = 0x120Cu,  /* tcpwm[0].tr_in[12] */
    TRIG2_OUT_TCPWM0_TR_IN13        = 0x120Du   /* tcpwm[0].tr_in[13] */
} en_trig_output_grp2_t;

/* Trigger Output Group 3 - TCPWM trigger inputs */
typedef enum
{
    TRIG3_OUT_TCPWM1_TR_IN0         = 0x1300u,  /* tcpwm[1].tr_in[0] */
    TRIG3_OUT_TCPWM1_TR_IN1         = 0x1301u,  /* tcpwm[1].tr_in[1] */
    TRIG3_OUT_TCPWM1_TR_IN2         = 0x1302u,  /* tcpwm[1].tr_in[2] */
    TRIG3_OUT_TCPWM1_TR_IN3         = 0x1303u,  /* tcpwm[1].tr_in[3] */
    TRIG3_OUT_TCPWM1_TR_IN4         = 0x1304u,  /* tcpwm[1].tr_in[4] */
    TRIG3_OUT_TCPWM1_TR_IN5         = 0x1305u,  /* tcpwm[1].tr_in[5] */
    TRIG3_OUT_TCPWM1_TR_IN6         = 0x1306u,  /* tcpwm[1].tr_in[6] */
    TRIG3_OUT_TCPWM1_TR_IN7         = 0x1307u,  /* tcpwm[1].tr_in[7] */
    TRIG3_OUT_TCPWM1_TR_IN8         = 0x1308u,  /* tcpwm[1].tr_in[8] */
    TRIG3_OUT_TCPWM1_TR_IN9         = 0x1309u,  /* tcpwm[1].tr_in[9] */
    TRIG3_OUT_TCPWM1_TR_IN10        = 0x130Au,  /* tcpwm[1].tr_in[10] */
    TRIG3_OUT_TCPWM1_TR_IN11        = 0x130Bu,  /* tcpwm[1].tr_in[11] */
    TRIG3_OUT_TCPWM1_TR_IN12        = 0x130Cu,  /* tcpwm[1].tr_in[12] */
    TRIG3_OUT_TCPWM1_TR_IN13        = 0x130Du   /* tcpwm[1].tr_in[13] */
} en_trig_output_grp3_t;

/* Trigger Output Group 4 - PROFILE trigger multiplexer */
typedef enum
{
    TRIG4_OUT_PROFILE_TR_START      = 0x1400u,  /* profile.tr_start */
    TRIG4_OUT_PROFILE_TR_STOP       = 0x1401u   /* profile.tr_stop */
} en_trig_output_grp4_t;

/* Trigger Output Group 5 - CPUSS.CTI trigger multiplexer */
typedef enum
{
    TRIG5_OUT_CPUSS_CTI_TR_IN0      = 0x1500u,  /* cpuss.cti_tr_in[0] */
    TRIG5_OUT_CPUSS_CTI_TR_IN1      = 0x1501u   /* cpuss.cti_tr_in[1] */
} en_trig_output_grp5_t;

/* Trigger Output Group 6 - PASS trigger multiplexer */
typedef enum
{
    TRIG6_OUT_PASS_TR_SAR_IN        = 0x1600u   /* pass.tr_sar_in */
} en_trig_output_grp6_t;

/* Trigger Output Group 7 - UDB general purpose trigger multiplexer */
typedef enum
{
    TRIG7_OUT_UDB_TR_IN0            = 0x1700u,  /* udb.tr_in[0] */
    TRIG7_OUT_UDB_TR_IN1            = 0x1701u   /* udb.tr_in[1] */
} en_trig_output_grp7_t;

/* Trigger Output Group 8 - Trigger multiplexer to pins */
typedef enum
{
    TRIG8_OUT_PERI_TR_IO_OUTPUT0    = 0x1800u,  /* peri.tr_io_output[0] */
    TRIG8_OUT_PERI_TR_IO_OUTPUT1    = 0x1801u   /* peri.tr_io_output[1] */
} en_trig_output_grp8_t;

/* Trigger Output Group 9 - Feedback mux to USB DMA interface */
typedef enum
{
    TRIG9_OUT_USB_DMA_BURSTEND0     = 0x1900u,  /* usb.dma_burstend[0] */
    TRIG9_OUT_USB_DMA_BURSTEND1     = 0x1901u,  /* usb.dma_burstend[1] */
    TRIG9_OUT_USB_DMA_BURSTEND2     = 0x1902u,  /* usb.dma_burstend[2] */
    TRIG9_OUT_USB_DMA_BURSTEND3     = 0x1903u,  /* usb.dma_burstend[3] */
    TRIG9_OUT_USB_DMA_BURSTEND4     = 0x1904u,  /* usb.dma_burstend[4] */
    TRIG9_OUT_USB_DMA_BURSTEND5     = 0x1905u,  /* usb.dma_burstend[5] */
    TRIG9_OUT_USB_DMA_BURSTEND6     = 0x1906u,  /* usb.dma_burstend[6] */
    TRIG9_OUT_USB_DMA_BURSTEND7     = 0x1907u   /* usb.dma_burstend[7] */
} en_trig_output_grp9_t;

/* Trigger Output Group 10 - Reduces 32 datawire output triggers to 8 signals, used by all except USB */
typedef enum
{
    TRIG10_OUT_UDB_TR_DW_ACK0       = 0x1A00u,  /* udb.tr_dw_ack[0] */
    TRIG10_OUT_TR_GROUP0_INPUT0     = 0x1A00u,  /* tr_group[0].input[0] */
    TRIG10_OUT_TR_GROUP1_INPUT0     = 0x1A00u,  /* tr_group[1].input[0] */
    TRIG10_OUT_TR_GROUP2_INPUT0     = 0x1A00u,  /* tr_group[2].input[0] */
    TRIG10_OUT_TR_GROUP3_INPUT0     = 0x1A00u,  /* tr_group[3].input[0] */
    TRIG10_OUT_TR_GROUP4_INPUT0     = 0x1A00u,  /* tr_group[4].input[0] */
    TRIG10_OUT_TR_GROUP5_INPUT0     = 0x1A00u,  /* tr_group[5].input[0] */
    TRIG10_OUT_TR_GROUP6_INPUT0     = 0x1A00u,  /* tr_group[6].input[0] */
    TRIG10_OUT_TR_GROUP7_INPUT0     = 0x1A00u,  /* tr_group[7].input[0] */
    TRIG10_OUT_TR_GROUP8_INPUT0     = 0x1A00u,  /* tr_group[8].input[0] */
    TRIG10_OUT_UDB_TR_DW_ACK1       = 0x1A01u,  /* udb.tr_dw_ack[1] */
    TRIG10_OUT_TR_GROUP0_INPUT1     = 0x1A01u,  /* tr_group[0].input[1] */
    TRIG10_OUT_TR_GROUP1_INPUT1     = 0x1A01u,  /* tr_group[1].input[1] */
    TRIG10_OUT_TR_GROUP2_INPUT1     = 0x1A01u,  /* tr_group[2].input[1] */
    TRIG10_OUT_TR_GROUP3_INPUT1     = 0x1A01u,  /* tr_group[3].input[1] */
    TRIG10_OUT_TR_GROUP4_INPUT1     = 0x1A01u,  /* tr_group[4].input[1] */
    TRIG10_OUT_TR_GROUP5_INPUT1     = 0x1A01u,  /* tr_group[5].input[1] */
    TRIG10_OUT_TR_GROUP6_INPUT1     = 0x1A01u,  /* tr_group[6].input[1] */
    TRIG10_OUT_TR_GROUP7_INPUT1     = 0x1A01u,  /* tr_group[7].input[1] */
    TRIG10_OUT_TR_GROUP8_INPUT1     = 0x1A01u,  /* tr_group[8].input[1] */
    TRIG10_OUT_UDB_TR_DW_ACK2       = 0x1A02u,  /* udb.tr_dw_ack[2] */
    TRIG10_OUT_TR_GROUP0_INPUT2     = 0x1A02u,  /* tr_group[0].input[2] */
    TRIG10_OUT_TR_GROUP1_INPUT2     = 0x1A02u,  /* tr_group[1].input[2] */
    TRIG10_OUT_TR_GROUP2_INPUT2     = 0x1A02u,  /* tr_group[2].input[2] */
    TRIG10_OUT_TR_GROUP3_INPUT2     = 0x1A02u,  /* tr_group[3].input[2] */
    TRIG10_OUT_TR_GROUP4_INPUT2     = 0x1A02u,  /* tr_group[4].input[2] */
    TRIG10_OUT_TR_GROUP5_INPUT2     = 0x1A02u,  /* tr_group[5].input[2] */
    TRIG10_OUT_TR_GROUP6_INPUT2     = 0x1A02u,  /* tr_group[6].input[2] */
    TRIG10_OUT_TR_GROUP7_INPUT2     = 0x1A02u,  /* tr_group[7].input[2] */
    TRIG10_OUT_TR_GROUP8_INPUT2     = 0x1A02u,  /* tr_group[8].input[2] */
    TRIG10_OUT_UDB_TR_DW_ACK3       = 0x1A03u,  /* udb.tr_dw_ack[3] */
    TRIG10_OUT_TR_GROUP0_INPUT3     = 0x1A03u,  /* tr_group[0].input[3] */
    TRIG10_OUT_TR_GROUP1_INPUT3     = 0x1A03u,  /* tr_group[1].input[3] */
    TRIG10_OUT_TR_GROUP2_INPUT3     = 0x1A03u,  /* tr_group[2].input[3] */
    TRIG10_OUT_TR_GROUP3_INPUT3     = 0x1A03u,  /* tr_group[3].input[3] */
    TRIG10_OUT_TR_GROUP4_INPUT3     = 0x1A03u,  /* tr_group[4].input[3] */
    TRIG10_OUT_TR_GROUP5_INPUT3     = 0x1A03u,  /* tr_group[5].input[3] */
    TRIG10_OUT_TR_GROUP6_INPUT3     = 0x1A03u,  /* tr_group[6].input[3] */
    TRIG10_OUT_TR_GROUP7_INPUT3     = 0x1A03u,  /* tr_group[7].input[3] */
    TRIG10_OUT_TR_GROUP8_INPUT3     = 0x1A03u,  /* tr_group[8].input[3] */
    TRIG10_OUT_UDB_TR_DW_ACK4       = 0x1A04u,  /* udb.tr_dw_ack[4] */
    TRIG10_OUT_TR_GROUP0_INPUT4     = 0x1A04u,  /* tr_group[0].input[4] */
    TRIG10_OUT_TR_GROUP1_INPUT4     = 0x1A04u,  /* tr_group[1].input[4] */
    TRIG10_OUT_TR_GROUP2_INPUT4     = 0x1A04u,  /* tr_group[2].input[4] */
    TRIG10_OUT_TR_GROUP3_INPUT4     = 0x1A04u,  /* tr_group[3].input[4] */
    TRIG10_OUT_TR_GROUP4_INPUT4     = 0x1A04u,  /* tr_group[4].input[4] */
    TRIG10_OUT_TR_GROUP5_INPUT4     = 0x1A04u,  /* tr_group[5].input[4] */
    TRIG10_OUT_TR_GROUP6_INPUT4     = 0x1A04u,  /* tr_group[6].input[4] */
    TRIG10_OUT_TR_GROUP7_INPUT4     = 0x1A04u,  /* tr_group[7].input[4] */
    TRIG10_OUT_TR_GROUP8_INPUT4     = 0x1A04u,  /* tr_group[8].input[4] */
    TRIG10_OUT_UDB_TR_DW_ACK5       = 0x1A05u,  /* udb.tr_dw_ack[5] */
    TRIG10_OUT_TR_GROUP0_INPUT5     = 0x1A05u,  /* tr_group[0].input[5] */
    TRIG10_OUT_TR_GROUP1_INPUT5     = 0x1A05u,  /* tr_group[1].input[5] */
    TRIG10_OUT_TR_GROUP2_INPUT5     = 0x1A05u,  /* tr_group[2].input[5] */
    TRIG10_OUT_TR_GROUP3_INPUT5     = 0x1A05u,  /* tr_group[3].input[5] */
    TRIG10_OUT_TR_GROUP4_INPUT5     = 0x1A05u,  /* tr_group[4].input[5] */
    TRIG10_OUT_TR_GROUP5_INPUT5     = 0x1A05u,  /* tr_group[5].input[5] */
    TRIG10_OUT_TR_GROUP6_INPUT5     = 0x1A05u,  /* tr_group[6].input[5] */
    TRIG10_OUT_TR_GROUP7_INPUT5     = 0x1A05u,  /* tr_group[7].input[5] */
    TRIG10_OUT_TR_GROUP8_INPUT5     = 0x1A05u,  /* tr_group[8].input[5] */
    TRIG10_OUT_UDB_TR_DW_ACK6       = 0x1A06u,  /* udb.tr_dw_ack[6] */
    TRIG10_OUT_TR_GROUP0_INPUT6     = 0x1A06u,  /* tr_group[0].input[6] */
    TRIG10_OUT_TR_GROUP1_INPUT6     = 0x1A06u,  /* tr_group[1].input[6] */
    TRIG10_OUT_TR_GROUP2_INPUT6     = 0x1A06u,  /* tr_group[2].input[6] */
    TRIG10_OUT_TR_GROUP3_INPUT6     = 0x1A06u,  /* tr_group[3].input[6] */
    TRIG10_OUT_TR_GROUP4_INPUT6     = 0x1A06u,  /* tr_group[4].input[6] */
    TRIG10_OUT_TR_GROUP5_INPUT6     = 0x1A06u,  /* tr_group[5].input[6] */
    TRIG10_OUT_TR_GROUP6_INPUT6     = 0x1A06u,  /* tr_group[6].input[6] */
    TRIG10_OUT_TR_GROUP7_INPUT6     = 0x1A06u,  /* tr_group[7].input[6] */
    TRIG10_OUT_TR_GROUP8_INPUT6     = 0x1A06u,  /* tr_group[8].input[6] */
    TRIG10_OUT_UDB_TR_DW_ACK7       = 0x1A07u,  /* udb.tr_dw_ack[7] */
    TRIG10_OUT_TR_GROUP0_INPUT7     = 0x1A07u,  /* tr_group[0].input[7] */
    TRIG10_OUT_TR_GROUP1_INPUT7     = 0x1A07u,  /* tr_group[1].input[7] */
    TRIG10_OUT_TR_GROUP2_INPUT7     = 0x1A07u,  /* tr_group[2].input[7] */
    TRIG10_OUT_TR_GROUP3_INPUT7     = 0x1A07u,  /* tr_group[3].input[7] */
    TRIG10_OUT_TR_GROUP4_INPUT7     = 0x1A07u,  /* tr_group[4].input[7] */
    TRIG10_OUT_TR_GROUP5_INPUT7     = 0x1A07u,  /* tr_group[5].input[7] */
    TRIG10_OUT_TR_GROUP6_INPUT7     = 0x1A07u,  /* tr_group[6].input[7] */
    TRIG10_OUT_TR_GROUP7_INPUT7     = 0x1A07u,  /* tr_group[7].input[7] */
    TRIG10_OUT_TR_GROUP8_INPUT7     = 0x1A07u   /* tr_group[8].input[7] */
} en_trig_output_grp10_t;

/* Trigger Output Group 11 - Reduces 96 tcpwm output triggers to 16 signals, used by all sinks */
typedef enum
{
    TRIG11_OUT_TR_GROUP0_INPUT8     = 0x1B00u,  /* tr_group[0].input[8] */
    TRIG11_OUT_TR_GROUP1_INPUT8     = 0x1B00u,  /* tr_group[1].input[8] */
    TRIG11_OUT_TR_GROUP2_INPUT8     = 0x1B00u,  /* tr_group[2].input[8] */
    TRIG11_OUT_TR_GROUP3_INPUT8     = 0x1B00u,  /* tr_group[3].input[8] */
    TRIG11_OUT_TR_GROUP4_INPUT8     = 0x1B00u,  /* tr_group[4].input[8] */
    TRIG11_OUT_TR_GROUP5_INPUT8     = 0x1B00u,  /* tr_group[5].input[8] */
    TRIG11_OUT_TR_GROUP6_INPUT8     = 0x1B00u,  /* tr_group[6].input[8] */
    TRIG11_OUT_TR_GROUP7_INPUT8     = 0x1B00u,  /* tr_group[7].input[8] */
    TRIG11_OUT_TR_GROUP8_INPUT8     = 0x1B00u,  /* tr_group[8].input[8] */
    TRIG11_OUT_TR_GROUP0_INPUT9     = 0x1B01u,  /* tr_group[0].input[9] */
    TRIG11_OUT_TR_GROUP1_INPUT9     = 0x1B01u,  /* tr_group[1].input[9] */
    TRIG11_OUT_TR_GROUP2_INPUT9     = 0x1B01u,  /* tr_group[2].input[9] */
    TRIG11_OUT_TR_GROUP3_INPUT9     = 0x1B01u,  /* tr_group[3].input[9] */
    TRIG11_OUT_TR_GROUP4_INPUT9     = 0x1B01u,  /* tr_group[4].input[9] */
    TRIG11_OUT_TR_GROUP5_INPUT9     = 0x1B01u,  /* tr_group[5].input[9] */
    TRIG11_OUT_TR_GROUP6_INPUT9     = 0x1B01u,  /* tr_group[6].input[9] */
    TRIG11_OUT_TR_GROUP7_INPUT9     = 0x1B01u,  /* tr_group[7].input[9] */
    TRIG11_OUT_TR_GROUP8_INPUT9     = 0x1B01u,  /* tr_group[8].input[9] */
    TRIG11_OUT_TR_GROUP0_INPUT10    = 0x1B02u,  /* tr_group[0].input[10] */
    TRIG11_OUT_TR_GROUP1_INPUT10    = 0x1B02u,  /* tr_group[1].input[10] */
    TRIG11_OUT_TR_GROUP2_INPUT10    = 0x1B02u,  /* tr_group[2].input[10] */
    TRIG11_OUT_TR_GROUP3_INPUT10    = 0x1B02u,  /* tr_group[3].input[10] */
    TRIG11_OUT_TR_GROUP4_INPUT10    = 0x1B02u,  /* tr_group[4].input[10] */
    TRIG11_OUT_TR_GROUP5_INPUT10    = 0x1B02u,  /* tr_group[5].input[10] */
    TRIG11_OUT_TR_GROUP6_INPUT10    = 0x1B02u,  /* tr_group[6].input[10] */
    TRIG11_OUT_TR_GROUP7_INPUT10    = 0x1B02u,  /* tr_group[7].input[10] */
    TRIG11_OUT_TR_GROUP8_INPUT10    = 0x1B02u,  /* tr_group[8].input[10] */
    TRIG11_OUT_TR_GROUP0_INPUT11    = 0x1B03u,  /* tr_group[0].input[11] */
    TRIG11_OUT_TR_GROUP1_INPUT11    = 0x1B03u,  /* tr_group[1].input[11] */
    TRIG11_OUT_TR_GROUP2_INPUT11    = 0x1B03u,  /* tr_group[2].input[11] */
    TRIG11_OUT_TR_GROUP3_INPUT11    = 0x1B03u,  /* tr_group[3].input[11] */
    TRIG11_OUT_TR_GROUP4_INPUT11    = 0x1B03u,  /* tr_group[4].input[11] */
    TRIG11_OUT_TR_GROUP5_INPUT11    = 0x1B03u,  /* tr_group[5].input[11] */
    TRIG11_OUT_TR_GROUP6_INPUT11    = 0x1B03u,  /* tr_group[6].input[11] */
    TRIG11_OUT_TR_GROUP7_INPUT11    = 0x1B03u,  /* tr_group[7].input[11] */
    TRIG11_OUT_TR_GROUP8_INPUT11    = 0x1B03u,  /* tr_group[8].input[11] */
    TRIG11_OUT_TR_GROUP0_INPUT12    = 0x1B04u,  /* tr_group[0].input[12] */
    TRIG11_OUT_TR_GROUP1_INPUT12    = 0x1B04u,  /* tr_group[1].input[12] */
    TRIG11_OUT_TR_GROUP2_INPUT12    = 0x1B04u,  /* tr_group[2].input[12] */
    TRIG11_OUT_TR_GROUP3_INPUT12    = 0x1B04u,  /* tr_group[3].input[12] */
    TRIG11_OUT_TR_GROUP4_INPUT12    = 0x1B04u,  /* tr_group[4].input[12] */
    TRIG11_OUT_TR_GROUP5_INPUT12    = 0x1B04u,  /* tr_group[5].input[12] */
    TRIG11_OUT_TR_GROUP6_INPUT12    = 0x1B04u,  /* tr_group[6].input[12] */
    TRIG11_OUT_TR_GROUP7_INPUT12    = 0x1B04u,  /* tr_group[7].input[12] */
    TRIG11_OUT_TR_GROUP8_INPUT12    = 0x1B04u,  /* tr_group[8].input[12] */
    TRIG11_OUT_TR_GROUP0_INPUT13    = 0x1B05u,  /* tr_group[0].input[13] */
    TRIG11_OUT_TR_GROUP1_INPUT13    = 0x1B05u,  /* tr_group[1].input[13] */
    TRIG11_OUT_TR_GROUP2_INPUT13    = 0x1B05u,  /* tr_group[2].input[13] */
    TRIG11_OUT_TR_GROUP3_INPUT13    = 0x1B05u,  /* tr_group[3].input[13] */
    TRIG11_OUT_TR_GROUP4_INPUT13    = 0x1B05u,  /* tr_group[4].input[13] */
    TRIG11_OUT_TR_GROUP5_INPUT13    = 0x1B05u,  /* tr_group[5].input[13] */
    TRIG11_OUT_TR_GROUP6_INPUT13    = 0x1B05u,  /* tr_group[6].input[13] */
    TRIG11_OUT_TR_GROUP7_INPUT13    = 0x1B05u,  /* tr_group[7].input[13] */
    TRIG11_OUT_TR_GROUP8_INPUT13    = 0x1B05u,  /* tr_group[8].input[13] */
    TRIG11_OUT_TR_GROUP0_INPUT14    = 0x1B06u,  /* tr_group[0].input[14] */
    TRIG11_OUT_TR_GROUP1_INPUT14    = 0x1B06u,  /* tr_group[1].input[14] */
    TRIG11_OUT_TR_GROUP2_INPUT14    = 0x1B06u,  /* tr_group[2].input[14] */
    TRIG11_OUT_TR_GROUP3_INPUT14    = 0x1B06u,  /* tr_group[3].input[14] */
    TRIG11_OUT_TR_GROUP4_INPUT14    = 0x1B06u,  /* tr_group[4].input[14] */
    TRIG11_OUT_TR_GROUP5_INPUT14    = 0x1B06u,  /* tr_group[5].input[14] */
    TRIG11_OUT_TR_GROUP6_INPUT14    = 0x1B06u,  /* tr_group[6].input[14] */
    TRIG11_OUT_TR_GROUP7_INPUT14    = 0x1B06u,  /* tr_group[7].input[14] */
    TRIG11_OUT_TR_GROUP8_INPUT14    = 0x1B06u,  /* tr_group[8].input[14] */
    TRIG11_OUT_TR_GROUP0_INPUT15    = 0x1B07u,  /* tr_group[0].input[15] */
    TRIG11_OUT_TR_GROUP1_INPUT15    = 0x1B07u,  /* tr_group[1].input[15] */
    TRIG11_OUT_TR_GROUP2_INPUT15    = 0x1B07u,  /* tr_group[2].input[15] */
    TRIG11_OUT_TR_GROUP3_INPUT15    = 0x1B07u,  /* tr_group[3].input[15] */
    TRIG11_OUT_TR_GROUP4_INPUT15    = 0x1B07u,  /* tr_group[4].input[15] */
    TRIG11_OUT_TR_GROUP5_INPUT15    = 0x1B07u,  /* tr_group[5].input[15] */
    TRIG11_OUT_TR_GROUP6_INPUT15    = 0x1B07u,  /* tr_group[6].input[15] */
    TRIG11_OUT_TR_GROUP7_INPUT15    = 0x1B07u,  /* tr_group[7].input[15] */
    TRIG11_OUT_TR_GROUP8_INPUT15    = 0x1B07u,  /* tr_group[8].input[15] */
    TRIG11_OUT_TR_GROUP0_INPUT16    = 0x1B08u,  /* tr_group[0].input[16] */
    TRIG11_OUT_TR_GROUP1_INPUT16    = 0x1B08u,  /* tr_group[1].input[16] */
    TRIG11_OUT_TR_GROUP2_INPUT16    = 0x1B08u,  /* tr_group[2].input[16] */
    TRIG11_OUT_TR_GROUP3_INPUT16    = 0x1B08u,  /* tr_group[3].input[16] */
    TRIG11_OUT_TR_GROUP4_INPUT16    = 0x1B08u,  /* tr_group[4].input[16] */
    TRIG11_OUT_TR_GROUP5_INPUT16    = 0x1B08u,  /* tr_group[5].input[16] */
    TRIG11_OUT_TR_GROUP6_INPUT16    = 0x1B08u,  /* tr_group[6].input[16] */
    TRIG11_OUT_TR_GROUP7_INPUT16    = 0x1B08u,  /* tr_group[7].input[16] */
    TRIG11_OUT_TR_GROUP8_INPUT16    = 0x1B08u,  /* tr_group[8].input[16] */
    TRIG11_OUT_TR_GROUP0_INPUT17    = 0x1B09u,  /* tr_group[0].input[17] */
    TRIG11_OUT_TR_GROUP1_INPUT17    = 0x1B09u,  /* tr_group[1].input[17] */
    TRIG11_OUT_TR_GROUP2_INPUT17    = 0x1B09u,  /* tr_group[2].input[17] */
    TRIG11_OUT_TR_GROUP3_INPUT17    = 0x1B09u,  /* tr_group[3].input[17] */
    TRIG11_OUT_TR_GROUP4_INPUT17    = 0x1B09u,  /* tr_group[4].input[17] */
    TRIG11_OUT_TR_GROUP5_INPUT17    = 0x1B09u,  /* tr_group[5].input[17] */
    TRIG11_OUT_TR_GROUP6_INPUT17    = 0x1B09u,  /* tr_group[6].input[17] */
    TRIG11_OUT_TR_GROUP7_INPUT17    = 0x1B09u,  /* tr_group[7].input[17] */
    TRIG11_OUT_TR_GROUP8_INPUT17    = 0x1B09u,  /* tr_group[8].input[17] */
    TRIG11_OUT_TR_GROUP0_INPUT18    = 0x1B0Au,  /* tr_group[0].input[18] */
    TRIG11_OUT_TR_GROUP1_INPUT18    = 0x1B0Au,  /* tr_group[1].input[18] */
    TRIG11_OUT_TR_GROUP2_INPUT18    = 0x1B0Au,  /* tr_group[2].input[18] */
    TRIG11_OUT_TR_GROUP3_INPUT18    = 0x1B0Au,  /* tr_group[3].input[18] */
    TRIG11_OUT_TR_GROUP4_INPUT18    = 0x1B0Au,  /* tr_group[4].input[18] */
    TRIG11_OUT_TR_GROUP5_INPUT18    = 0x1B0Au,  /* tr_group[5].input[18] */
    TRIG11_OUT_TR_GROUP6_INPUT18    = 0x1B0Au,  /* tr_group[6].input[18] */
    TRIG11_OUT_TR_GROUP7_INPUT18    = 0x1B0Au,  /* tr_group[7].input[18] */
    TRIG11_OUT_TR_GROUP8_INPUT18    = 0x1B0Au,  /* tr_group[8].input[18] */
    TRIG11_OUT_TR_GROUP0_INPUT19    = 0x1B0Bu,  /* tr_group[0].input[19] */
    TRIG11_OUT_TR_GROUP1_INPUT19    = 0x1B0Bu,  /* tr_group[1].input[19] */
    TRIG11_OUT_TR_GROUP2_INPUT19    = 0x1B0Bu,  /* tr_group[2].input[19] */
    TRIG11_OUT_TR_GROUP3_INPUT19    = 0x1B0Bu,  /* tr_group[3].input[19] */
    TRIG11_OUT_TR_GROUP4_INPUT19    = 0x1B0Bu,  /* tr_group[4].input[19] */
    TRIG11_OUT_TR_GROUP5_INPUT19    = 0x1B0Bu,  /* tr_group[5].input[19] */
    TRIG11_OUT_TR_GROUP6_INPUT19    = 0x1B0Bu,  /* tr_group[6].input[19] */
    TRIG11_OUT_TR_GROUP7_INPUT19    = 0x1B0Bu,  /* tr_group[7].input[19] */
    TRIG11_OUT_TR_GROUP8_INPUT19    = 0x1B0Bu,  /* tr_group[8].input[19] */
    TRIG11_OUT_TR_GROUP0_INPUT20    = 0x1B0Cu,  /* tr_group[0].input[20] */
    TRIG11_OUT_TR_GROUP1_INPUT20    = 0x1B0Cu,  /* tr_group[1].input[20] */
    TRIG11_OUT_TR_GROUP2_INPUT20    = 0x1B0Cu,  /* tr_group[2].input[20] */
    TRIG11_OUT_TR_GROUP3_INPUT20    = 0x1B0Cu,  /* tr_group[3].input[20] */
    TRIG11_OUT_TR_GROUP4_INPUT20    = 0x1B0Cu,  /* tr_group[4].input[20] */
    TRIG11_OUT_TR_GROUP5_INPUT20    = 0x1B0Cu,  /* tr_group[5].input[20] */
    TRIG11_OUT_TR_GROUP6_INPUT20    = 0x1B0Cu,  /* tr_group[6].input[20] */
    TRIG11_OUT_TR_GROUP7_INPUT20    = 0x1B0Cu,  /* tr_group[7].input[20] */
    TRIG11_OUT_TR_GROUP8_INPUT20    = 0x1B0Cu,  /* tr_group[8].input[20] */
    TRIG11_OUT_TR_GROUP0_INPUT21    = 0x1B0Du,  /* tr_group[0].input[21] */
    TRIG11_OUT_TR_GROUP1_INPUT21    = 0x1B0Du,  /* tr_group[1].input[21] */
    TRIG11_OUT_TR_GROUP2_INPUT21    = 0x1B0Du,  /* tr_group[2].input[21] */
    TRIG11_OUT_TR_GROUP3_INPUT21    = 0x1B0Du,  /* tr_group[3].input[21] */
    TRIG11_OUT_TR_GROUP4_INPUT21    = 0x1B0Du,  /* tr_group[4].input[21] */
    TRIG11_OUT_TR_GROUP5_INPUT21    = 0x1B0Du,  /* tr_group[5].input[21] */
    TRIG11_OUT_TR_GROUP6_INPUT21    = 0x1B0Du,  /* tr_group[6].input[21] */
    TRIG11_OUT_TR_GROUP7_INPUT21    = 0x1B0Du,  /* tr_group[7].input[21] */
    TRIG11_OUT_TR_GROUP8_INPUT21    = 0x1B0Du,  /* tr_group[8].input[21] */
    TRIG11_OUT_TR_GROUP0_INPUT22    = 0x1B0Eu,  /* tr_group[0].input[22] */
    TRIG11_OUT_TR_GROUP1_INPUT22    = 0x1B0Eu,  /* tr_group[1].input[22] */
    TRIG11_OUT_TR_GROUP2_INPUT22    = 0x1B0Eu,  /* tr_group[2].input[22] */
    TRIG11_OUT_TR_GROUP3_INPUT22    = 0x1B0Eu,  /* tr_group[3].input[22] */
    TRIG11_OUT_TR_GROUP4_INPUT22    = 0x1B0Eu,  /* tr_group[4].input[22] */
    TRIG11_OUT_TR_GROUP5_INPUT22    = 0x1B0Eu,  /* tr_group[5].input[22] */
    TRIG11_OUT_TR_GROUP6_INPUT22    = 0x1B0Eu,  /* tr_group[6].input[22] */
    TRIG11_OUT_TR_GROUP7_INPUT22    = 0x1B0Eu,  /* tr_group[7].input[22] */
    TRIG11_OUT_TR_GROUP8_INPUT22    = 0x1B0Eu,  /* tr_group[8].input[22] */
    TRIG11_OUT_TR_GROUP0_INPUT23    = 0x1B0Fu,  /* tr_group[0].input[23] */
    TRIG11_OUT_TR_GROUP1_INPUT23    = 0x1B0Fu,  /* tr_group[1].input[23] */
    TRIG11_OUT_TR_GROUP2_INPUT23    = 0x1B0Fu,  /* tr_group[2].input[23] */
    TRIG11_OUT_TR_GROUP3_INPUT23    = 0x1B0Fu,  /* tr_group[3].input[23] */
    TRIG11_OUT_TR_GROUP4_INPUT23    = 0x1B0Fu,  /* tr_group[4].input[23] */
    TRIG11_OUT_TR_GROUP5_INPUT23    = 0x1B0Fu,  /* tr_group[5].input[23] */
    TRIG11_OUT_TR_GROUP6_INPUT23    = 0x1B0Fu,  /* tr_group[6].input[23] */
    TRIG11_OUT_TR_GROUP7_INPUT23    = 0x1B0Fu,  /* tr_group[7].input[23] */
    TRIG11_OUT_TR_GROUP8_INPUT23    = 0x1B0Fu   /* tr_group[8].input[23] */
} en_trig_output_grp11_t;

/* Trigger Output Group 12 - Reduces 28 pin input signals to 10 triggers used by all sinks */
typedef enum
{
    TRIG12_OUT_TR_GROUP2_INPUT24    = 0x1C00u,  /* tr_group[2].input[24] */
    TRIG12_OUT_TR_GROUP3_INPUT24    = 0x1C00u,  /* tr_group[3].input[24] */
    TRIG12_OUT_TR_GROUP4_INPUT24    = 0x1C00u,  /* tr_group[4].input[24] */
    TRIG12_OUT_TR_GROUP5_INPUT24    = 0x1C00u,  /* tr_group[5].input[24] */
    TRIG12_OUT_TR_GROUP6_INPUT24    = 0x1C00u,  /* tr_group[6].input[24] */
    TRIG12_OUT_TR_GROUP7_INPUT24    = 0x1C00u,  /* tr_group[7].input[24] */
    TRIG12_OUT_TR_GROUP8_INPUT24    = 0x1C00u,  /* tr_group[8].input[24] */
    TRIG12_OUT_TR_GROUP2_INPUT25    = 0x1C01u,  /* tr_group[2].input[25] */
    TRIG12_OUT_TR_GROUP3_INPUT25    = 0x1C01u,  /* tr_group[3].input[25] */
    TRIG12_OUT_TR_GROUP4_INPUT25    = 0x1C01u,  /* tr_group[4].input[25] */
    TRIG12_OUT_TR_GROUP5_INPUT25    = 0x1C01u,  /* tr_group[5].input[25] */
    TRIG12_OUT_TR_GROUP6_INPUT25    = 0x1C01u,  /* tr_group[6].input[25] */
    TRIG12_OUT_TR_GROUP7_INPUT25    = 0x1C01u,  /* tr_group[7].input[25] */
    TRIG12_OUT_TR_GROUP8_INPUT25    = 0x1C01u,  /* tr_group[8].input[25] */
    TRIG12_OUT_TR_GROUP2_INPUT26    = 0x1C02u,  /* tr_group[2].input[26] */
    TRIG12_OUT_TR_GROUP3_INPUT26    = 0x1C02u,  /* tr_group[3].input[26] */
    TRIG12_OUT_TR_GROUP4_INPUT26    = 0x1C02u,  /* tr_group[4].input[26] */
    TRIG12_OUT_TR_GROUP5_INPUT26    = 0x1C02u,  /* tr_group[5].input[26] */
    TRIG12_OUT_TR_GROUP6_INPUT26    = 0x1C02u,  /* tr_group[6].input[26] */
    TRIG12_OUT_TR_GROUP7_INPUT26    = 0x1C02u,  /* tr_group[7].input[26] */
    TRIG12_OUT_TR_GROUP8_INPUT26    = 0x1C02u,  /* tr_group[8].input[26] */
    TRIG12_OUT_TR_GROUP2_INPUT27    = 0x1C03u,  /* tr_group[2].input[27] */
    TRIG12_OUT_TR_GROUP3_INPUT27    = 0x1C03u,  /* tr_group[3].input[27] */
    TRIG12_OUT_TR_GROUP4_INPUT27    = 0x1C03u,  /* tr_group[4].input[27] */
    TRIG12_OUT_TR_GROUP5_INPUT27    = 0x1C03u,  /* tr_group[5].input[27] */
    TRIG12_OUT_TR_GROUP6_INPUT27    = 0x1C03u,  /* tr_group[6].input[27] */
    TRIG12_OUT_TR_GROUP7_INPUT27    = 0x1C03u,  /* tr_group[7].input[27] */
    TRIG12_OUT_TR_GROUP8_INPUT27    = 0x1C03u,  /* tr_group[8].input[27] */
    TRIG12_OUT_TR_GROUP2_INPUT28    = 0x1C04u,  /* tr_group[2].input[28] */
    TRIG12_OUT_TR_GROUP3_INPUT28    = 0x1C04u,  /* tr_group[3].input[28] */
    TRIG12_OUT_TR_GROUP4_INPUT28    = 0x1C04u,  /* tr_group[4].input[28] */
    TRIG12_OUT_TR_GROUP5_INPUT28    = 0x1C04u,  /* tr_group[5].input[28] */
    TRIG12_OUT_TR_GROUP6_INPUT28    = 0x1C04u,  /* tr_group[6].input[28] */
    TRIG12_OUT_TR_GROUP7_INPUT28    = 0x1C04u,  /* tr_group[7].input[28] */
    TRIG12_OUT_TR_GROUP8_INPUT28    = 0x1C04u,  /* tr_group[8].input[28] */
    TRIG12_OUT_TR_GROUP2_INPUT29    = 0x1C05u,  /* tr_group[2].input[29] */
    TRIG12_OUT_TR_GROUP3_INPUT29    = 0x1C05u,  /* tr_group[3].input[29] */
    TRIG12_OUT_TR_GROUP4_INPUT29    = 0x1C05u,  /* tr_group[4].input[29] */
    TRIG12_OUT_TR_GROUP5_INPUT29    = 0x1C05u,  /* tr_group[5].input[29] */
    TRIG12_OUT_TR_GROUP6_INPUT29    = 0x1C05u,  /* tr_group[6].input[29] */
    TRIG12_OUT_TR_GROUP7_INPUT29    = 0x1C05u,  /* tr_group[7].input[29] */
    TRIG12_OUT_TR_GROUP8_INPUT29    = 0x1C05u,  /* tr_group[8].input[29] */
    TRIG12_OUT_TR_GROUP2_INPUT30    = 0x1C06u,  /* tr_group[2].input[30] */
    TRIG12_OUT_TR_GROUP3_INPUT30    = 0x1C06u,  /* tr_group[3].input[30] */
    TRIG12_OUT_TR_GROUP4_INPUT30    = 0x1C06u,  /* tr_group[4].input[30] */
    TRIG12_OUT_TR_GROUP5_INPUT30    = 0x1C06u,  /* tr_group[5].input[30] */
    TRIG12_OUT_TR_GROUP6_INPUT30    = 0x1C06u,  /* tr_group[6].input[30] */
    TRIG12_OUT_TR_GROUP7_INPUT30    = 0x1C06u,  /* tr_group[7].input[30] */
    TRIG12_OUT_TR_GROUP8_INPUT30    = 0x1C06u,  /* tr_group[8].input[30] */
    TRIG12_OUT_TR_GROUP2_INPUT31    = 0x1C07u,  /* tr_group[2].input[31] */
    TRIG12_OUT_TR_GROUP3_INPUT31    = 0x1C07u,  /* tr_group[3].input[31] */
    TRIG12_OUT_TR_GROUP4_INPUT31    = 0x1C07u,  /* tr_group[4].input[31] */
    TRIG12_OUT_TR_GROUP5_INPUT31    = 0x1C07u,  /* tr_group[5].input[31] */
    TRIG12_OUT_TR_GROUP6_INPUT31    = 0x1C07u,  /* tr_group[6].input[31] */
    TRIG12_OUT_TR_GROUP7_INPUT31    = 0x1C07u,  /* tr_group[7].input[31] */
    TRIG12_OUT_TR_GROUP8_INPUT31    = 0x1C07u,  /* tr_group[8].input[31] */
    TRIG12_OUT_TR_GROUP0_INPUT24    = 0x1C08u,  /* tr_group[0].input[24] */
    TRIG12_OUT_TR_GROUP1_INPUT24    = 0x1C08u,  /* tr_group[1].input[24] */
    TRIG12_OUT_TR_GROUP0_INPUT25    = 0x1C09u,  /* tr_group[0].input[25] */
    TRIG12_OUT_TR_GROUP1_INPUT25    = 0x1C09u   /* tr_group[1].input[25] */
} en_trig_output_grp12_t;

/* Trigger Output Group 13 - Reduces DMA requests to 16+2 outputs used by all sinks */
typedef enum
{
    TRIG13_OUT_TR_GROUP0_INPUT26    = 0x1D00u,  /* tr_group[0].input[26] */
    TRIG13_OUT_TR_GROUP1_INPUT26    = 0x1D00u,  /* tr_group[1].input[26] */
    TRIG13_OUT_TR_GROUP0_INPUT27    = 0x1D01u,  /* tr_group[0].input[27] */
    TRIG13_OUT_TR_GROUP1_INPUT27    = 0x1D01u,  /* tr_group[1].input[27] */
    TRIG13_OUT_TR_GROUP0_INPUT28    = 0x1D02u,  /* tr_group[0].input[28] */
    TRIG13_OUT_TR_GROUP1_INPUT28    = 0x1D02u,  /* tr_group[1].input[28] */
    TRIG13_OUT_TR_GROUP0_INPUT29    = 0x1D03u,  /* tr_group[0].input[29] */
    TRIG13_OUT_TR_GROUP1_INPUT29    = 0x1D03u,  /* tr_group[1].input[29] */
    TRIG13_OUT_TR_GROUP0_INPUT30    = 0x1D04u,  /* tr_group[0].input[30] */
    TRIG13_OUT_TR_GROUP1_INPUT30    = 0x1D04u,  /* tr_group[1].input[30] */
    TRIG13_OUT_TR_GROUP0_INPUT31    = 0x1D05u,  /* tr_group[0].input[31] */
    TRIG13_OUT_TR_GROUP1_INPUT31    = 0x1D05u,  /* tr_group[1].input[31] */
    TRIG13_OUT_TR_GROUP0_INPUT32    = 0x1D06u,  /* tr_group[0].input[32] */
    TRIG13_OUT_TR_GROUP1_INPUT32    = 0x1D06u,  /* tr_group[1].input[32] */
    TRIG13_OUT_TR_GROUP0_INPUT33    = 0x1D07u,  /* tr_group[0].input[33] */
    TRIG13_OUT_TR_GROUP1_INPUT33    = 0x1D07u,  /* tr_group[1].input[33] */
    TRIG13_OUT_TR_GROUP0_INPUT34    = 0x1D08u,  /* tr_group[0].input[34] */
    TRIG13_OUT_TR_GROUP1_INPUT34    = 0x1D08u,  /* tr_group[1].input[34] */
    TRIG13_OUT_TR_GROUP0_INPUT35    = 0x1D09u,  /* tr_group[0].input[35] */
    TRIG13_OUT_TR_GROUP1_INPUT35    = 0x1D09u,  /* tr_group[1].input[35] */
    TRIG13_OUT_TR_GROUP0_INPUT36    = 0x1D0Au,  /* tr_group[0].input[36] */
    TRIG13_OUT_TR_GROUP1_INPUT36    = 0x1D0Au,  /* tr_group[1].input[36] */
    TRIG13_OUT_TR_GROUP0_INPUT37    = 0x1D0Bu,  /* tr_group[0].input[37] */
    TRIG13_OUT_TR_GROUP1_INPUT37    = 0x1D0Bu,  /* tr_group[1].input[37] */
    TRIG13_OUT_TR_GROUP0_INPUT38    = 0x1D0Cu,  /* tr_group[0].input[38] */
    TRIG13_OUT_TR_GROUP1_INPUT38    = 0x1D0Cu,  /* tr_group[1].input[38] */
    TRIG13_OUT_TR_GROUP0_INPUT39    = 0x1D0Du,  /* tr_group[0].input[39] */
    TRIG13_OUT_TR_GROUP1_INPUT39    = 0x1D0Du,  /* tr_group[1].input[39] */
    TRIG13_OUT_TR_GROUP0_INPUT40    = 0x1D0Eu,  /* tr_group[0].input[40] */
    TRIG13_OUT_TR_GROUP1_INPUT40    = 0x1D0Eu,  /* tr_group[1].input[40] */
    TRIG13_OUT_TR_GROUP0_INPUT41    = 0x1D0Fu,  /* tr_group[0].input[41] */
    TRIG13_OUT_TR_GROUP1_INPUT41    = 0x1D0Fu,  /* tr_group[1].input[41] */
    TRIG13_OUT_TR_GROUP2_INPUT32    = 0x1D10u,  /* tr_group[2].input[32] */
    TRIG13_OUT_TR_GROUP3_INPUT32    = 0x1D10u,  /* tr_group[3].input[32] */
    TRIG13_OUT_TR_GROUP4_INPUT32    = 0x1D10u,  /* tr_group[4].input[32] */
    TRIG13_OUT_TR_GROUP5_INPUT32    = 0x1D10u,  /* tr_group[5].input[32] */
    TRIG13_OUT_TR_GROUP6_INPUT32    = 0x1D10u,  /* tr_group[6].input[32] */
    TRIG13_OUT_TR_GROUP7_INPUT32    = 0x1D10u,  /* tr_group[7].input[32] */
    TRIG13_OUT_TR_GROUP8_INPUT32    = 0x1D10u,  /* tr_group[8].input[32] */
    TRIG13_OUT_TR_GROUP2_INPUT33    = 0x1D11u,  /* tr_group[2].input[33] */
    TRIG13_OUT_TR_GROUP3_INPUT33    = 0x1D11u,  /* tr_group[3].input[33] */
    TRIG13_OUT_TR_GROUP4_INPUT33    = 0x1D11u,  /* tr_group[4].input[33] */
    TRIG13_OUT_TR_GROUP5_INPUT33    = 0x1D11u,  /* tr_group[5].input[33] */
    TRIG13_OUT_TR_GROUP6_INPUT33    = 0x1D11u,  /* tr_group[6].input[33] */
    TRIG13_OUT_TR_GROUP7_INPUT33    = 0x1D11u,  /* tr_group[7].input[33] */
    TRIG13_OUT_TR_GROUP8_INPUT33    = 0x1D11u   /* tr_group[8].input[33] */
} en_trig_output_grp13_t;

/* Trigger Output Group 14 - Reduces general purpose trigger inputs to 8+8 outputs used by all sinks */
typedef enum
{
    TRIG14_OUT_TR_GROUP0_INPUT42    = 0x1E00u,  /* tr_group[0].input[42] */
    TRIG14_OUT_TR_GROUP1_INPUT42    = 0x1E00u,  /* tr_group[1].input[42] */
    TRIG14_OUT_TR_GROUP0_INPUT43    = 0x1E01u,  /* tr_group[0].input[43] */
    TRIG14_OUT_TR_GROUP1_INPUT43    = 0x1E01u,  /* tr_group[1].input[43] */
    TRIG14_OUT_TR_GROUP0_INPUT44    = 0x1E02u,  /* tr_group[0].input[44] */
    TRIG14_OUT_TR_GROUP1_INPUT44    = 0x1E02u,  /* tr_group[1].input[44] */
    TRIG14_OUT_TR_GROUP0_INPUT45    = 0x1E03u,  /* tr_group[0].input[45] */
    TRIG14_OUT_TR_GROUP1_INPUT45    = 0x1E03u,  /* tr_group[1].input[45] */
    TRIG14_OUT_TR_GROUP0_INPUT46    = 0x1E04u,  /* tr_group[0].input[46] */
    TRIG14_OUT_TR_GROUP1_INPUT46    = 0x1E04u,  /* tr_group[1].input[46] */
    TRIG14_OUT_TR_GROUP0_INPUT47    = 0x1E05u,  /* tr_group[0].input[47] */
    TRIG14_OUT_TR_GROUP1_INPUT47    = 0x1E05u,  /* tr_group[1].input[47] */
    TRIG14_OUT_TR_GROUP0_INPUT48    = 0x1E06u,  /* tr_group[0].input[48] */
    TRIG14_OUT_TR_GROUP1_INPUT48    = 0x1E06u,  /* tr_group[1].input[48] */
    TRIG14_OUT_TR_GROUP0_INPUT49    = 0x1E07u,  /* tr_group[0].input[49] */
    TRIG14_OUT_TR_GROUP1_INPUT49    = 0x1E07u,  /* tr_group[1].input[49] */
    TRIG14_OUT_TR_GROUP2_INPUT34    = 0x1E08u,  /* tr_group[2].input[34] */
    TRIG14_OUT_TR_GROUP3_INPUT34    = 0x1E08u,  /* tr_group[3].input[34] */
    TRIG14_OUT_TR_GROUP4_INPUT34    = 0x1E08u,  /* tr_group[4].input[34] */
    TRIG14_OUT_TR_GROUP5_INPUT34    = 0x1E08u,  /* tr_group[5].input[34] */
    TRIG14_OUT_TR_GROUP6_INPUT34    = 0x1E08u,  /* tr_group[6].input[34] */
    TRIG14_OUT_TR_GROUP7_INPUT34    = 0x1E08u,  /* tr_group[7].input[34] */
    TRIG14_OUT_TR_GROUP8_INPUT34    = 0x1E08u,  /* tr_group[8].input[34] */
    TRIG14_OUT_TR_GROUP2_INPUT35    = 0x1E09u,  /* tr_group[2].input[35] */
    TRIG14_OUT_TR_GROUP3_INPUT35    = 0x1E09u,  /* tr_group[3].input[35] */
    TRIG14_OUT_TR_GROUP4_INPUT35    = 0x1E09u,  /* tr_group[4].input[35] */
    TRIG14_OUT_TR_GROUP5_INPUT35    = 0x1E09u,  /* tr_group[5].input[35] */
    TRIG14_OUT_TR_GROUP6_INPUT35    = 0x1E09u,  /* tr_group[6].input[35] */
    TRIG14_OUT_TR_GROUP7_INPUT35    = 0x1E09u,  /* tr_group[7].input[35] */
    TRIG14_OUT_TR_GROUP8_INPUT35    = 0x1E09u,  /* tr_group[8].input[35] */
    TRIG14_OUT_TR_GROUP2_INPUT36    = 0x1E0Au,  /* tr_group[2].input[36] */
    TRIG14_OUT_TR_GROUP3_INPUT36    = 0x1E0Au,  /* tr_group[3].input[36] */
    TRIG14_OUT_TR_GROUP4_INPUT36    = 0x1E0Au,  /* tr_group[4].input[36] */
    TRIG14_OUT_TR_GROUP5_INPUT36    = 0x1E0Au,  /* tr_group[5].input[36] */
    TRIG14_OUT_TR_GROUP6_INPUT36    = 0x1E0Au,  /* tr_group[6].input[36] */
    TRIG14_OUT_TR_GROUP7_INPUT36    = 0x1E0Au,  /* tr_group[7].input[36] */
    TRIG14_OUT_TR_GROUP8_INPUT36    = 0x1E0Au,  /* tr_group[8].input[36] */
    TRIG14_OUT_TR_GROUP2_INPUT37    = 0x1E0Bu,  /* tr_group[2].input[37] */
    TRIG14_OUT_TR_GROUP3_INPUT37    = 0x1E0Bu,  /* tr_group[3].input[37] */
    TRIG14_OUT_TR_GROUP4_INPUT37    = 0x1E0Bu,  /* tr_group[4].input[37] */
    TRIG14_OUT_TR_GROUP5_INPUT37    = 0x1E0Bu,  /* tr_group[5].input[37] */
    TRIG14_OUT_TR_GROUP6_INPUT37    = 0x1E0Bu,  /* tr_group[6].input[37] */
    TRIG14_OUT_TR_GROUP7_INPUT37    = 0x1E0Bu,  /* tr_group[7].input[37] */
    TRIG14_OUT_TR_GROUP8_INPUT37    = 0x1E0Bu,  /* tr_group[8].input[37] */
    TRIG14_OUT_TR_GROUP2_INPUT38    = 0x1E0Cu,  /* tr_group[2].input[38] */
    TRIG14_OUT_TR_GROUP3_INPUT38    = 0x1E0Cu,  /* tr_group[3].input[38] */
    TRIG14_OUT_TR_GROUP4_INPUT38    = 0x1E0Cu,  /* tr_group[4].input[38] */
    TRIG14_OUT_TR_GROUP5_INPUT38    = 0x1E0Cu,  /* tr_group[5].input[38] */
    TRIG14_OUT_TR_GROUP6_INPUT38    = 0x1E0Cu,  /* tr_group[6].input[38] */
    TRIG14_OUT_TR_GROUP7_INPUT38    = 0x1E0Cu,  /* tr_group[7].input[38] */
    TRIG14_OUT_TR_GROUP8_INPUT38    = 0x1E0Cu,  /* tr_group[8].input[38] */
    TRIG14_OUT_TR_GROUP2_INPUT39    = 0x1E0Du,  /* tr_group[2].input[39] */
    TRIG14_OUT_TR_GROUP3_INPUT39    = 0x1E0Du,  /* tr_group[3].input[39] */
    TRIG14_OUT_TR_GROUP4_INPUT39    = 0x1E0Du,  /* tr_group[4].input[39] */
    TRIG14_OUT_TR_GROUP5_INPUT39    = 0x1E0Du,  /* tr_group[5].input[39] */
    TRIG14_OUT_TR_GROUP6_INPUT39    = 0x1E0Du,  /* tr_group[6].input[39] */
    TRIG14_OUT_TR_GROUP7_INPUT39    = 0x1E0Du,  /* tr_group[7].input[39] */
    TRIG14_OUT_TR_GROUP8_INPUT39    = 0x1E0Du,  /* tr_group[8].input[39] */
    TRIG14_OUT_TR_GROUP2_INPUT40    = 0x1E0Eu,  /* tr_group[2].input[40] */
    TRIG14_OUT_TR_GROUP3_INPUT40    = 0x1E0Eu,  /* tr_group[3].input[40] */
    TRIG14_OUT_TR_GROUP4_INPUT40    = 0x1E0Eu,  /* tr_group[4].input[40] */
    TRIG14_OUT_TR_GROUP5_INPUT40    = 0x1E0Eu,  /* tr_group[5].input[40] */
    TRIG14_OUT_TR_GROUP6_INPUT40    = 0x1E0Eu,  /* tr_group[6].input[40] */
    TRIG14_OUT_TR_GROUP7_INPUT40    = 0x1E0Eu,  /* tr_group[7].input[40] */
    TRIG14_OUT_TR_GROUP8_INPUT40    = 0x1E0Eu,  /* tr_group[8].input[40] */
    TRIG14_OUT_TR_GROUP2_INPUT41    = 0x1E0Fu,  /* tr_group[2].input[41] */
    TRIG14_OUT_TR_GROUP3_INPUT41    = 0x1E0Fu,  /* tr_group[3].input[41] */
    TRIG14_OUT_TR_GROUP4_INPUT41    = 0x1E0Fu,  /* tr_group[4].input[41] */
    TRIG14_OUT_TR_GROUP5_INPUT41    = 0x1E0Fu,  /* tr_group[5].input[41] */
    TRIG14_OUT_TR_GROUP6_INPUT41    = 0x1E0Fu,  /* tr_group[6].input[41] */
    TRIG14_OUT_TR_GROUP7_INPUT41    = 0x1E0Fu,  /* tr_group[7].input[41] */
    TRIG14_OUT_TR_GROUP8_INPUT41    = 0x1E0Fu   /* tr_group[8].input[41] */
} en_trig_output_grp14_t;

/* Level or edge detection setting for a trigger mux */
typedef enum
{
 /* The trigger is a simple level output */
    TRIGGER_TYPE_LEVEL = 0u,
    /* The trigger is synchronized to the consumer blocks clock
       and a two cycle pulse is generated on this clock */
    TRIGGER_TYPE_EDGE = 1u
} en_trig_type_t;

/* Trigger Type Defines */
/* TCPWM Trigger Types */
#define TRIGGER_TYPE_TCPWM_LINE                 TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_TCPWM_LINE_COMPL           TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_TCPWM_TR_IN__LEVEL         TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_TCPWM_TR_IN__EDGE          TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_TCPWM_TR_OVERFLOW          TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_TCPWM_TR_COMPARE_MATCH     TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_TCPWM_TR_UNDERFLOW         TRIGGER_TYPE_EDGE
/* CSD Trigger Types */
#define TRIGGER_TYPE_CSD_DSI_SAMPLE_OUT         TRIGGER_TYPE_EDGE
/* SCB Trigger Types */
#define TRIGGER_TYPE_SCB_TR_TX_REQ              TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_SCB_TR_RX_REQ              TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_SCB_TR_I2C_SCL_FILTERED    TRIGGER_TYPE_LEVEL
/* PERI Trigger Types */
#define TRIGGER_TYPE_PERI_TR_IO_INPUT__LEVEL    TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_PERI_TR_IO_INPUT__EDGE     TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_PERI_TR_IO_OUTPUT__LEVEL   TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_PERI_TR_IO_OUTPUT__EDGE    TRIGGER_TYPE_EDGE
/* CPUSS Trigger Types */
#define TRIGGER_TYPE_CPUSS_DW0_TR_IN__LEVEL     TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_CPUSS_DW0_TR_IN__EDGE      TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_CPUSS_DW1_TR_IN__LEVEL     TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_CPUSS_DW1_TR_IN__EDGE      TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_CPUSS_CTI_TR_IN            TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_CPUSS_DW0_TR_OUT           TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_CPUSS_DW1_TR_OUT           TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_CPUSS_CTI_TR_OUT           TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_CPUSS_TR_FAULT             TRIGGER_TYPE_EDGE
/* AUDIOSS Trigger Types */
#define TRIGGER_TYPE_AUDIOSS_TR_PDM_RX_REQ      TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_AUDIOSS_TR_I2S_TX_REQ      TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_AUDIOSS_TR_I2S_RX_REQ      TRIGGER_TYPE_LEVEL
/* LPCOMP Trigger Types */
#define TRIGGER_TYPE_LPCOMP_DSI_COMP0           TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_LPCOMP_DSI_COMP1           TRIGGER_TYPE_LEVEL
/* PASS Trigger Types */
#define TRIGGER_TYPE_PASS_DSI_CTB_CMP0          TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_PASS_DSI_CTB_CMP1          TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_PASS_TR_SAR_IN             TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_PASS_TR_SAR_OUT            TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_PASS_TR_CTDAC_EMPTY        TRIGGER_TYPE_EDGE
/* SMIF Trigger Types */
#define TRIGGER_TYPE_SMIF_TR_TX_REQ             TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_SMIF_TR_RX_REQ             TRIGGER_TYPE_LEVEL
/* USB Trigger Types */
#define TRIGGER_TYPE_USB_DMA_BURSTEND           TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_USB_DMA_REQ                TRIGGER_TYPE_EDGE
/* UDB Trigger Types */
#define TRIGGER_TYPE_UDB_TR_IN__LEVEL           TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_UDB_TR_IN__EDGE            TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_UDB_TR_DW_ACK__LEVEL       TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_UDB_TR_DW_ACK__EDGE        TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_UDB_TR_UDB__LEVEL          TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_UDB_TR_UDB__EDGE           TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_UDB_DSI_OUT_TR__LEVEL      TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_UDB_DSI_OUT_TR__EDGE       TRIGGER_TYPE_EDGE
/* PROFILE Trigger Types */
#define TRIGGER_TYPE_PROFILE_TR_START           TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_PROFILE_TR_STOP            TRIGGER_TYPE_EDGE
/* TR_GROUP Trigger Types */
#define TRIGGER_TYPE_TR_GROUP_OUTPUT__LEVEL     TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_TR_GROUP_OUTPUT__EDGE      TRIGGER_TYPE_EDGE
#define TRIGGER_TYPE_TR_GROUP_INPUT__LEVEL      TRIGGER_TYPE_LEVEL
#define TRIGGER_TYPE_TR_GROUP_INPUT__EDGE       TRIGGER_TYPE_EDGE

/* Monitor Signal Defines */
typedef enum
{
    PROFILE_ONE                     =  0,
    CPUSS_MONITOR_CM0               =  1,
    CPUSS_MONITOR_CM4               =  2,
    CPUSS_MONITOR_FLASH             =  3,
    CPUSS_MONITOR_DW0_AHB           =  4,
    CPUSS_MONITOR_DW1_AHB           =  5,
    CPUSS_MONITOR_CRYPTO            =  6,
    USB_MONITOR_AHB                 =  7,
    SCB0_MONITOR_AHB                =  8,
    SCB1_MONITOR_AHB                =  9,
    SCB2_MONITOR_AHB                = 10,
    SCB3_MONITOR_AHB                = 11,
    SCB4_MONITOR_AHB                = 12,
    SCB5_MONITOR_AHB                = 13,
    SCB6_MONITOR_AHB                = 14,
    SCB7_MONITOR_AHB                = 15,
    SCB8_MONITOR_AHB                = 16,
    UDB_MONITOR_UDB0                = 17,
    UDB_MONITOR_UDB1                = 18,
    UDB_MONITOR_UDB2                = 19,
    UDB_MONITOR_UDB3                = 20,
    SMIF_MONITOR_SMIF_SPI_SELECT0   = 21,
    SMIF_MONITOR_SMIF_SPI_SELECT1   = 22,
    SMIF_MONITOR_SMIF_SPI_SELECT2   = 23,
    SMIF_MONITOR_SMIF_SPI_SELECT3   = 24,
    SMIF_MONITOR_SMIF_SPI_SELECT_ANY = 25,
    BLESS_EXT_LNA_RX_CTL_OUT        = 26,
    BLESS_EXT_PA_TX_CTL_OUT         = 27
} en_ep_mon_sel_t;

/* Total count of Energy Profiler monitor signal connections */
#define EP_MONITOR_COUNT            28u

/* Bus masters */
typedef enum
{
    CPUSS_MS_ID_CM0                 =  0,
    CPUSS_MS_ID_CRYPTO              =  1,
    CPUSS_MS_ID_DW0                 =  2,
    CPUSS_MS_ID_DW1                 =  3,
    CPUSS_MS_ID_CM4                 = 14,
    CPUSS_MS_ID_TC                  = 15
} en_prot_master_t;

/* Parameter Defines */
/* Number of regulator modules instantiated within SRSS */
#define SRSS_NUM_ACTREG_PWRMOD      2u
/* Number of shorting switches between vccd and vccact */
#define SRSS_NUM_ACTIVE_SWITCH      3u
/* ULP linear regulator system is present */
#define SRSS_ULPLINREG_PRESENT      1u
/* HT linear regulator system is present */
#define SRSS_HTLINREG_PRESENT       0u
/* SIMO buck core regulator is present. Only compatible with ULP linear
   regulator system (ULPLINREG_PRESENT==1). */
#define SRSS_SIMOBUCK_PRESENT       1u
/* Precision ILO (PILO) is present */
#define SRSS_PILO_PRESENT           1u
/* External Crystal Oscillator is present (high frequency) */
#define SRSS_ECO_PRESENT            1u
/* System Buck-Boost is present */
#define SRSS_SYSBB_PRESENT          0u
/* Number of clock paths. Must be > 0 */
#define SRSS_NUM_CLKPATH            3u
/* Number of PLLs present. Must be <= NUM_CLKPATH */
#define SRSS_NUM_PLL                1u
/* Number of HFCLK roots present. Must be > 0 */
#define SRSS_NUM_HFROOT             5u
/* Number of PWR_HIB_DATA registers */
#define SRSS_NUM_HIBDATA            1u
/* Backup domain is present */
#define SRSS_BACKUP_PRESENT         1u
/* Mask of HFCLK root clock supervisors (CSV). For each clock root i, bit[i] of
   mask indicates presence of a CSV. */
#define SRSS_MASK_HFCSV             1u
/* Clock supervisor is present on WCO. Must be 0 if BACKUP_PRESENT==0. */
#define SRSS_WCOCSV_PRESENT         1u
/* Number of software watchdog timers. */
#define SRSS_NUM_MCWDT              2u
/* Number of DSI inputs into clock muxes. This is used for logic optimization. */
#define SRSS_NUM_DSI                2u
/* Alternate high-frequency clock is present. This is used for logic
   optimization. */
#define SRSS_ALTHF_PRESENT          1u
/* Alternate low-frequency clock is present. This is used for logic
   optimization. */
#define SRSS_ALTLF_PRESENT          0u
/* Backup memory is present (only used when BACKUP_PRESENT==1) */
#define SRSS_BACKUP_BMEM_PRESENT    0u
/* Number of Backup registers to include (each is 32b). Only used when
   BACKUP_PRESENT==1. */
#define SRSS_BACKUP_NUM_BREG        8u
/* Number of AMUX splitter cells */
#define IOSS_HSIOM_AMUX_SPLIT_NR    8u
/* Number of HSIOM ports in device (same as GPIO.GPIO_PRT_NR) */
#define IOSS_HSIOM_HSIOM_PORT_NR    15u
/* Number of GPIO ports in range 0..31 */
#define IOSS_GPIO_GPIO_PORT_NR_0_31 15u
/* Number of GPIO ports in range 32..63 */
#define IOSS_GPIO_GPIO_PORT_NR_32_63 0u
/* Number of GPIO ports in range 64..95 */
#define IOSS_GPIO_GPIO_PORT_NR_64_95 0u
/* Number of GPIO ports in range 96..127 */
#define IOSS_GPIO_GPIO_PORT_NR_96_127 0u
/* Number of ports in device */
#define IOSS_GPIO_GPIO_PORT_NR      15u
/* Mask of SMARTIO instances presence */
#define IOSS_SMARTIO_SMARTIO_MASK   768u
/* The number of protection contexts ([2, 16]). */
#define PERI_PC_NR                  8u
/* Master interface presence mask (4 bits) */
#define PERI_MS_PRESENT             15u
/* Master interface PPU combinatorial (1) or registerd (0) */
#define PERI_MS_PPU_COMBINATORIAL   1u
/* The number of programmable PPU structures for PERI (all peripherals) */
#define PERI_MS_PPU_PROG_STRUCT_NR  16u
/* Number of programmable clocks (outputs) */
#define PERI_CLOCK_NR               59u
/* Number of 8.0 dividers */
#define PERI_DIV_8_NR               8u
/* Number of 16.0 dividers */
#define PERI_DIV_16_NR              16u
/* Number of 16.5 (fractional) dividers */
#define PERI_DIV_16_5_NR            4u
/* Number of 24.5 (fractional) dividers */
#define PERI_DIV_24_5_NR            1u
/* Divider number width: max(1,roundup(log2(max(DIV_*_NR))) */
#define PERI_DIV_ADDR_WIDTH         4u
/* Trigger module present (0=No, 1=Yes) */
#define PERI_TR                     1u
/* Number of trigger groups */
#define PERI_TR_GROUP_NR            15u
/* The number of protection contexts minus 1 ([1, 15]). */
#define PERI_PPU_FIXED_STRUCT_PC_NR_MINUS1 7u
/* The number of protection contexts minus 1 ([1, 15]). */
#define PERI_PPU_PROG_STRUCT_PC_NR_MINUS1 7u
/* UDB present or not ('0': no, '1': yes) */
#define CPUSS_UDB_PRESENT           1u
/* System RAM 0 size in kilobytes */
#define CPUSS_SRAM0_SIZE            288u
/* Number of macros used to implement System RAM 0. Example: 8 if 256 KB System
   SRAM0 is implemented with 8 32KB macros. */
#define CPUSS_RAMC0_MACRO_NR        9u
/* System RAM 1 present or not (0=No, 1=Yes) */
#define CPUSS_RAMC1_PRESENT         0u
/* System RAM 1 size in kilobytes */
#define CPUSS_SRAM1_SIZE            32u
/* Number of macros used to implement System RAM 1. Example: 8 if 256 KB System
   RAM 1 is implemented with 8 32KB macros. */
#define CPUSS_RAMC1_MACRO_NR        1u
/* System RAM 2 present or not (0=No, 1=Yes) */
#define CPUSS_RAMC2_PRESENT         0u
/* System RAM 2 size in kilobytes */
#define CPUSS_SRAM2_SIZE            256u
/* Number of macros used to implement System RAM 2. Example: 8 if 256 KB System
   RAM 2 is implemented with 8 32KB macros. */
#define CPUSS_RAMC2_MACRO_NR        16u
/* System ROM size in KB */
#define CPUSS_ROM_SIZE              128u
/* Flash main region size in KB */
#define CPUSS_FLASH_SIZE            1024u
/* Flash work region size in KB (EEPROM emulation, data) */
#define CPUSS_WFLASH_SIZE           32u
/* Flash supervisory region size in KB */
#define CPUSS_SFLASH_SIZE           32u
/* Number of external slaves directly connected to slow AHB-Lite
   infrastructure. Maximum nubmer of slave supported is 4. Width of this
   parameter is 4-bits. 1-bit mask for each slave indicating present or not.
   Example: 4'b0011 - slave 0 and slave 1 are present. Note: The SLOW_SLx_ADDR
   and SLOW_SLx_MASK parameters (for the slaves present) should be derived from
   the Memory Map. */
#define CPUSS_SLOW_SL_PRESENT       1u
/* Number of external slaves directly connected to fast AHB-Lite
   infrastructure. Maximum nubmer of slave supported is 4. Width of this
   parameter is 4-bits. 1-bit mask for each slave indicating present or not.
   Example: 4'b0011 - slave 0 and slave 1 are present. Note: The FAST_SLx_ADDR
   and FAST_SLx_MASK parameters (for the slaves present) should be derived from
   the Memory Map. */
#define CPUSS_FAST_SL_PRESENT       1u
/* Number of external masters driving the slow AHB-Lite infrastructure. Maximum
   number of masters supported is 2. Width of this parameter is 2-bits. 1-bit
   mask for each master indicating present or not. Example: 2'b01 - master 0 is
   present. */
#define CPUSS_SLOW_MS_PRESENT       0u
/* Number of LF clock (which is connected to clk_lf input of CPUSS) cycles to
   make 10ms time period. Example: =327 (or 24'000147) for 32.678 KHz LF clock
   from SRSS. */
#define CPUSS_LF_10MS_TICKS         16777215u
/* Number of total interrupt request inputs to CPUSS */
#define CPUSS_IRQ_NR                139u
/* Number of DeepSleep wakeup interrupt inputs to CPUSS */
#define CPUSS_DPSLP_IRQ_NR          33u
/* Number of DeepSleep wakeup interrupt inputs to CM0+ (product configuration) */
#define CPUSS_CM0_DPSLP_IRQ_NR      8u
/* Width of the CM4 interrupt priority bits. Legal range [3,8] Example: 3 = 8
   levels of priority 8 = 256 levels of priority */
#define CPUSS_CM4_LVL_WIDTH         3u
/* CM4 Floating point unit present or not (0=No, 1=Yes) */
#define CPUSS_CM4_FPU_PRESENT       1u
/* Debug level. Legal range [0,3] */
#define CPUSS_DEBUG_LVL             3u
/* Trace level. Legal range [0,2] Note: CM4 HTM is not supported. Hence vaule 3
   for trace level is not supported in CPUSS. */
#define CPUSS_TRACE_LVL             2u
/* Embedded Trace Buffer present or not (0=No, 1=Yes) */
#define CPUSS_ETB_PRESENT           0u
/* CM0+ MTB SRAM buffer size in kilobytes. Legal vaules 4, 8 or 16 */
#define CPUSS_MTB_SRAM_SIZE         4u
/* CM4 ETB SRAM buffer size in kilobytes. Legal vaules 4, 8 or 16 */
#define CPUSS_ETB_SRAM_SIZE         16u
/* PTM interface present (0=No, 1=Yes) */
#define CPUSS_PTM_PRESENT           1u
/* Width of the PTM interface in bits ([2,32]) */
#define CPUSS_PTM_WIDTH             8u
/* Width of the TPIU interface in bits ([1,32]) */
#define CPUSS_TPIU_WIDTH            4u
/* CoreSight Part Identification Number */
#define CPUSS_JEPID                 52u
/* CoreSight Part Identification Number */
#define CPUSS_JEPCONTINUATION       0u
/* CoreSight Part Identification Number */
#define CPUSS_FAMILYID              256u
/* Cryptography IP present or not (0=No, 1=Yes) */
#define CPUSS_CRYPTO_PRESENT        1u
/* DataWire 0 present or not (0=No, 1=Yes) */
#define CPUSS_DW0_PRESENT           1u
/* Number of DataWire 0 channels (8, 16 or 32) */
#define CPUSS_DW0_CH_NR             16u
/* DataWire 1 present or not (0=No, 1=Yes) */
#define CPUSS_DW1_PRESENT           1u
/* Number of DataWire 1 channels (8, 16 or 32) */
#define CPUSS_DW1_CH_NR             16u
/* AES cipher support (0 = no support, 1 = support */
#define CPUSS_CRYPTO_AES            1u
/* (Tripple) DES cipher support (0 = no support, 1 = support */
#define CPUSS_CRYPTO_DES            1u
/* Pseudo random number generation support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_PR             1u
/* SHA support included */
#define CPUSS_CRYPTO_SHA            1u
/* SHA1 hash support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_SHA1           1u
/* SHA256 hash support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_SHA256         1u
/* SHA512 hash support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_SHA512         1u
/* Cyclic Redundancy Check support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_CRC            1u
/* Vector unit support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_VU             1u
/* True random number generation support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_TR             1u
/* String support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_STR            1u
/* AHB-Lite master interface support (0 = no support, 1 = support) */
#define CPUSS_CRYPTO_MASTER_IF      1u
/* Number of 32-bit words in the IP internal memory buffer (from the set [64,
   128, 256, 512, 1024, 2048, 4096], to allow for a 256 B, 512 B, 1 kB, 2 kB, 4
   kB, 8 kB and 16 kB memory buffer) */
#define CPUSS_CRYPTO_BUFF_SIZE      1024u
/* Number of fault structures. Legal range [1, 4] */
#define CPUSS_FAULT_FAULT_NR        2u
/* Number of IPC structures. Legal range [1, 16] */
#define CPUSS_IPC_IPC_NR            8u
/* Number of IPC interrupt structures. Legal range [1, 16] */
#define CPUSS_IPC_IPC_IRQ_NR        8u
/* Master 0 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS0_PC_NR_MINUS1 7u
/* Master 1 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS1_PC_NR_MINUS1 7u
/* Master 2 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS2_PC_NR_MINUS1 0u
/* Master 3 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS3_PC_NR_MINUS1 0u
/* Master 4 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS4_PC_NR_MINUS1 0u
/* Master 5 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS5_PC_NR_MINUS1 0u
/* Master 6 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS6_PC_NR_MINUS1 0u
/* Master 7 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS7_PC_NR_MINUS1 0u
/* Master 8 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS8_PC_NR_MINUS1 0u
/* Master 9 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS9_PC_NR_MINUS1 0u
/* Master 10 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS10_PC_NR_MINUS1 0u
/* Master 11 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS11_PC_NR_MINUS1 0u
/* Master 12 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS12_PC_NR_MINUS1 0u
/* Master 13 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS13_PC_NR_MINUS1 0u
/* Master 14 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS14_PC_NR_MINUS1 7u
/* Master 15 protect contexts minus one */
#define CPUSS_PROT_SMPU_MS15_PC_NR_MINUS1 7u
/* Number of SMPU protection structures */
#define CPUSS_PROT_SMPU_STRUCT_NR   16u
/* Number of protection contexts supported minus 1. Legal range [1,16] */
#define CPUSS_SMPU_STRUCT_PC_NR_MINUS1 7u
/* Number of DataWire controllers present (max 2) */
#define CPUSS_DW_NR                 2u
/* Number of channels in each DataWire controller (must be the same for now) */
#define CPUSS_DW_CH_NR              16u
/* Page size in # of 32-bit words (1: 4 bytes, 2: 8 bytes, ... */
#define CPUSS_FLASHC_PA_SIZE        128u
/* Number of profiling counters. Legal range [1, 32] */
#define PROFILE_PRFL_CNT_NR         8u
/* Number of monitor event signals. Legal range [1, 128] */
#define PROFILE_PRFL_MONITOR_NR     128u
/* Number of UDB Interrupts */
#define UDB_NUMINT                  16u
/* Number of triggers */
#define UDB_NUMTR                   16u
/* Number of UDB array rows (must be multiple of 2) */
#define UDB_NUMROW                  2u
/* Number of UDB array columns */
#define UDB_NUMCOL                  6u
/* DSI on bottom (1) or on bottom and top (2) of UDB array */
#define UDB_DSISIDES                2u
/* Number of UDBs = NUMROW * NUMCOL */
#define UDB_NUMUDB                  12u
/* Number of UDB pairs = NUMUDB / 2 */
#define UDB_NUMUDBPAIR              6u
/* Number of DSIs = NUMCOL * DSISIDES */
#define UDB_NUMDSI                  12u
/* Number of quad clocks */
#define UDB_NUMQCLK                 3u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB0_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB0_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB0_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB0_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB0_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB0_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB0_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB0_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB0_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB0_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB0_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB0_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB0_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB0_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB0_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB0_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB0_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB0_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB0_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB0_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB0_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB0_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB0_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB1_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB1_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB1_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB1_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB1_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB1_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB1_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB1_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB1_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB1_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB1_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB1_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB1_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB1_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB1_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB1_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB1_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB1_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB1_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB1_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB1_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB1_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB1_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB2_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB2_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB2_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB2_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB2_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB2_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB2_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB2_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB2_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB2_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB2_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB2_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB2_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB2_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB2_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB2_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB2_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB2_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB2_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB2_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB2_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB2_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB2_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB3_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB3_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB3_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB3_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB3_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB3_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB3_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB3_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB3_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB3_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB3_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB3_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB3_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB3_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB3_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB3_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB3_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB3_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB3_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB3_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB3_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB3_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB3_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB4_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB4_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB4_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB4_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB4_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB4_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB4_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB4_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB4_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB4_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB4_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB4_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB4_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB4_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB4_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB4_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB4_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB4_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB4_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB4_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB4_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB4_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB4_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB5_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB5_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB5_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB5_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB5_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB5_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB5_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB5_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB5_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB5_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB5_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB5_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB5_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB5_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB5_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB5_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB5_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB5_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB5_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB5_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB5_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB5_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB5_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB6_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB6_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB6_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB6_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB6_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB6_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB6_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB6_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB6_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB6_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB6_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB6_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB6_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB6_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB6_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB6_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB6_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB6_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB6_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB6_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB6_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB6_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB6_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB7_DEEPSLEEP              0u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB7_EC                     0u
/* I2C master support? ('0': no, '1': yes) */
#define SCB7_I2C_M                  1u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB7_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB7_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB7_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB7_I2C_EC                 0u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB7_I2C_M_S                1u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB7_I2C_S_EC               0u
/* SPI master support? ('0': no, '1': yes) */
#define SCB7_SPI_M                  1u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB7_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB7_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB7_SPI_EC                 0u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB7_SPI_S_EC               0u
/* UART support? ('0': no, '1': yes) */
#define SCB7_UART                   1u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB7_UART_FLOW              1u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB7_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB7_CMD_RESP               0u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB7_EZ                     0u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB7_EZ_CMD_RESP            0u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB7_I2C_S_EZ               0u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB7_SPI_S_EZ               0u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB7_I2C_FAST_PLUS          1u
/* DeepSleep support ('0':no, '1': yes) */
#define SCB8_DEEPSLEEP              1u
/* Externally clocked support? ('0': no, '1': yes) */
#define SCB8_EC                     1u
/* I2C master support? ('0': no, '1': yes) */
#define SCB8_I2C_M                  0u
/* I2C slave support? ('0': no, '1': yes) */
#define SCB8_I2C_S                  1u
/* I2C support? (I2C_M | I2C_S) */
#define SCB8_I2C                    1u
/* I2C glitch filters present? ('0': no, '1': yes) */
#define SCB8_GLITCH                 1u
/* I2C externally clocked support? ('0': no, '1': yes) */
#define SCB8_I2C_EC                 1u
/* I2C master and slave support? (I2C_M & I2C_S) */
#define SCB8_I2C_M_S                0u
/* I2C slave with EC? (I2C_S & I2C_EC) */
#define SCB8_I2C_S_EC               1u
/* SPI master support? ('0': no, '1': yes) */
#define SCB8_SPI_M                  0u
/* SPI slave support? ('0': no, '1': yes) */
#define SCB8_SPI_S                  1u
/* SPI support? (SPI_M | SPI_S) */
#define SCB8_SPI                    1u
/* SPI externally clocked support? ('0': no, '1': yes) */
#define SCB8_SPI_EC                 1u
/* SPI slave with EC? (SPI_S & SPI_EC) */
#define SCB8_SPI_S_EC               1u
/* UART support? ('0': no, '1': yes) */
#define SCB8_UART                   0u
/* UART flow control support? ('0': no, '1': yes) */
#define SCB8_UART_FLOW              0u
/* Number of EZ memory Bytes ([32, 256, 512]). This memory is used in EZ mode,
   CMD_RESP mode and FIFO mode. Note that in EZ mode, if EZ_DATA_NR is 512, only
   256 B are used. This is because the EZ mode uses 8-bit addresses. */
#define SCB8_EZ_DATA_NR             256u
/* Command/response mode support? ('0': no, '1': yes) */
#define SCB8_CMD_RESP               1u
/* EZ mode support? ('0': no, '1': yes) */
#define SCB8_EZ                     1u
/* Command/response mode or EZ mode support? (CMD_RESP | EZ) */
#define SCB8_EZ_CMD_RESP            1u
/* I2C slave with EZ mode (I2C_S & EZ) */
#define SCB8_I2C_S_EZ               1u
/* SPI slave with EZ mode (SPI_S & EZ) */
#define SCB8_SPI_S_EZ               1u
/* Support I2C FM+/1Mbps speed ('0': no, '1': yes) */
#define SCB8_I2C_FAST_PLUS          1u
/* Number of counters per IP (1..8) */
#define TCPWM0_CNT_NR               8u
/* Counter width (in number of bits) */
#define TCPWM0_CNT_CNT_WIDTH        32u
/* Number of counters per IP (1..8) */
#define TCPWM1_CNT_NR               24u
/* Counter width (in number of bits) */
#define TCPWM1_CNT_CNT_WIDTH        16u
/* Max number of LCD commons supported */
#define LCD_COM_NR                  8u
/* Max number of LCD pins (total) supported */
#define LCD_PIN_NR                  62u
/* Number of ports supoprting up to 4 COMs */
#define LCD_NUMPORTS                8u
/* Number of ports supporting up to 8 COMs */
#define LCD_NUMPORTS8               8u
/* Number of ports supporting up to 16 COMs */
#define LCD_NUMPORTS16              0u
/* Number of IREF outputs from AREF */
#define PASS_NR_IREFS               4u
/* Number of CTBs in the Subsystem */
#define PASS_NR_CTBS                1u
/* Number of CTDACs in the Subsystem */
#define PASS_NR_CTDACS              1u
/* CTB0 Exists */
#define PASS_CTB0_EXISTS            1u
/* CTB1 Exists */
#define PASS_CTB1_EXISTS            0u
/* CTB2 Exists */
#define PASS_CTB2_EXISTS            0u
/* CTB3 Exists */
#define PASS_CTB3_EXISTS            0u
/* CTDAC0 Exists */
#define PASS_CTDAC0_EXISTS          1u
/* CTDAC1 Exists */
#define PASS_CTDAC1_EXISTS          0u
/* CTDAC2 Exists */
#define PASS_CTDAC2_EXISTS          0u
/* CTDAC3 Exists */
#define PASS_CTDAC3_EXISTS          0u
/* Number of SAR channels */
#define PASS_SAR_SAR_CHANNELS       16u
/* Averaging logic present in SAR */
#define PASS_SAR_SAR_AVERAGE        1u
/* Range detect logic present in SAR */
#define PASS_SAR_SAR_RANGEDET       1u
/* Support for UAB sampling */
#define PASS_SAR_SAR_UAB            0u
#define PASS_CTBM_CTDAC_PRESENT     1u
/* Number of AHB-Lite "hmaster[]" bits ([1, 8]) */
#define SMIF_MASTER_WIDTH           8u
/* Base address of the SMIF XIP memory region. This address must be a multiple
   of the SMIF XIP memory capacity. This address must be a multiple of 64 KB.
   This address must be in the [0x0000:0000, 0x1fff:ffff] memory region. The XIP
   memory region should NOT overlap with other memory regions. */
#define SMIF_SMIF_XIP_ADDR          402653184u
/* Capacity of the SMIF XIP memory region. The more significant bits of this
   parameter must be '1' and the lesser significant bits of this paramter must
   be '0'. E.g., 0xfff0:0000 specifies a 1 MB memory region. Legal values are
   {0xffff:0000, 0xfffe:0000, 0xfffc:0000, 0xfff8:0000, 0xfff0:0000,
   0xffe0:0000, ..., 0xe000:0000}. */
#define SMIF_SMIF_XIP_MASK          4160749568u
/* Cryptography (AES) support ('0' = no support, '1' = support) */
#define SMIF_CRYPTO                 1u
/* Number of external devices supported ([1,4]) */
#define SMIF_DEVICE_NR              4u
/* External device write support. This is a 4-bit field. Each external device
   has a dedicated bit. E.g., if bit 2 is '1', external device 2 has write
   support. */
#define SMIF_DEVICE_WR_EN           15u
/* AHB bus Master Width */
#define BLE_BLESS_MASTER_WIDTH      8u
/* I2S capable? (0=No,1=Yes) */
#define AUDIOSS_I2S                 1u
/* PDM capable? (0=No,1=Yes) */
#define AUDIOSS_PDM                 1u

#endif /* _PSOC6BLE_CONFIG_H_ */


/* [] END OF FILE */
