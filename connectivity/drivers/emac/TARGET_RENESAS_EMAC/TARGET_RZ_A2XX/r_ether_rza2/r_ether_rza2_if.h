/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018-2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/* Copyright (c) 2018-2020 Renesas Electronics Corporation.
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
/***********************************************************************************************************************
 * File Name    : r_ether_rza2_if.h
 * Version      : 1.00
 * Description  : Ethernet module device driver
 ***********************************************************************************************************************/

/* Guards against multiple inclusion */
#ifndef R_ETHER_RZA2_IF_H
#define R_ETHER_RZA2_IF_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <stdint.h>

#include "r_ether_rza2_config.h"
#include "src/phy/phy.h"

#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* Version Number of API. */
#define ETHER_RZA2_VERSION_MAJOR  (1)
#define ETHER_RZA2_VERSION_MINOR  (0)

/* When using the Read functions, ETHER_NO_DATA is the return value that indicates that no received data. */
#define ETHER_NO_DATA             (0)

/* The value of flag which indicates that the interrupt of Ethernet occur. */
#define ETHER_FLAG_OFF            (0)
#define ETHER_FLAG_ON             (1)
#define ETHER_FLAG_ON_LINK_ON     (3)
#define ETHER_FLAG_ON_LINK_OFF    (2)

/* Channel definition of Ethernet */
#define ETHER_CHANNEL_0           (0)
#define ETHER_CHANNEL_1           (1)

#define ETHER_CHANNEL_MAX         (ETHER_CH0_EN + ETHER_CH1_EN)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
/* ETHER API error codes */
typedef enum {
    ETHER_SUCCESS = 0,            /* Processing completed successfully */
    ETHER_ERR_INVALID_PTR = -1,   /* Value of the pointer is NULL */
    ETHER_ERR_INVALID_DATA = -2,  /* Value of the argument is out of range */
    ETHER_ERR_INVALID_CHAN = -3,  /* Nonexistent channel number */
    ETHER_ERR_INVALID_ARG = -4,   /* Invalid argument */
    ETHER_ERR_LINK = -5,          /* Auto-negotiation is not completed, and transmission/reception is not enabled. */
    ETHER_ERR_MPDE = -6,          /* As a Magic Packet is being detected, and transmission/reception is not enabled. */
    ETHER_ERR_TACT = -7,          /* Transmit buffer is not empty. */
    ETHER_ERR_CHAN_OPEN = -8,     /* Indicates the Ethernet cannot be opened because it is being used by another application */
    ETHER_ERR_MC_FRAME = -9,      /* Detect multicast frame when multicast frame filtering enable */
    ETHER_ERR_RECV_ENABLE = -10,  /* Enable receive function in ETHERC */
    ETHER_ERR_OTHER = -11         /* Other error */
} ether_return_t;

/* Event code of callback function */
typedef enum {
    ETHER_CB_EVENT_ID_WAKEON_LAN, /* Magic packet detection */
    ETHER_CB_EVENT_ID_LINK_ON,    /* Link up detection */
    ETHER_CB_EVENT_ID_LINK_OFF,   /* Link down detection */
} ether_cb_event_t;

/* Structure of the callback function pointer */
typedef struct {
    void (*pcb_func)(void *);     /* Callback function pointer */
    void (*pcb_int_hnd)(void *);  /* Interrupt handler function pointer */
} ether_cb_t;

/* Structure to be used when decoding the argument of the callback function */
typedef struct {
    uint32_t         channel;     /* ETHERC channel */
    ether_cb_event_t event_id;    /* Event code for callback function */
    uint32_t         status_ecsr; /* ETHERC status register for interrupt handler */
    uint32_t         status_eesr; /* ETHERC/EDMAC status register for interrupt handler */
} ether_cb_arg_t;

/* Parameters of the control function (1st argument) */
typedef enum {
    CONTROL_SET_CALLBACK,         /* Callback function registration */
    CONTROL_SET_PROMISCUOUS_MODE, /* Promiscuous mode setting */
    CONTROL_SET_INT_HANDLER,      /* Interrupt handler function registration */
    CONTROL_POWER_ON,             /* Cancel ETHERC/EDMAC module stop */
    CONTROL_POWER_OFF,            /* Transition to ETHERC/EDMAC module stop */
    CONTROL_MULTICASTFRAME_FILTER,/* Multicast frame filter setting*/
    CONTROL_BROADCASTFRAME_FILTER /* Broadcast frame filter setting*/
} ether_cmd_t;

typedef enum {
    ETHER_PROMISCUOUS_OFF,        /* ETHERC operates in standard mode */
    ETHER_PROMISCUOUS_ON          /* ETHERC operates in promiscuous mode */
} ether_promiscuous_bit_t;

typedef enum {
    ETHER_MC_FILTER_OFF,          /* Multicast frame filter disable */
    ETHER_MC_FILTER_ON            /* Multicast frame filter enable */
} ether_mc_filter_t;

typedef struct {
    uint32_t                channel; /* ETHERC channel */
    ether_promiscuous_bit_t bit;     /* Promiscuous mode */
} ether_promiscuous_t;

typedef struct {
    uint32_t          channel;    /* ETHERC channel */
    ether_mc_filter_t flag;       /* Multicast frame filter */
} ether_multicast_t;

typedef struct {
    uint32_t channel;             /* ETHERC channel */
    uint32_t counter;             /* Continuous reception number of Broadcast frame */
} ether_broadcast_t;

/* Parameters of the control function (2nd argument) */
typedef union {
    ether_cb_t          ether_callback;        /* Callback function pointer */
    ether_promiscuous_t *p_ether_promiscuous;  /* Promiscuous mode setting */
    ether_cb_t          ether_int_hnd;         /* Interrupt handler function pointer */
    uint32_t            channel;               /* ETHERC channel number */
    ether_multicast_t    *p_ether_multicast;   /* Multicast frame filter setting */
    ether_broadcast_t    *p_ether_broadcast;   /* Broadcast frame filter setting */
} ether_param_t;

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
extern void R_ETHER_Initial(void);
extern ether_return_t R_ETHER_Open_ZC2(uint32_t channel, const uint8_t mac_addr[], uint8_t pause);
extern ether_return_t R_ETHER_Close_ZC2(uint32_t channel);
extern int32_t R_ETHER_Read(uint32_t channel, void *pbuf);
extern int32_t R_ETHER_Read_ZC2(uint32_t channel, void **pbuf);
extern int32_t R_ETHER_Read_ZC2_BufRelease(uint32_t channel);
extern ether_return_t R_ETHER_Write(uint32_t channel, void *pbuf, uint32_t len);
extern ether_return_t R_ETHER_Write_ZC2_GetBuf(uint32_t channel, void **pbuf, uint16_t *pbuf_size);
extern ether_return_t R_ETHER_Write_ZC2_SetBuf(uint32_t channel, const uint32_t len);
extern ether_return_t R_ETHER_CheckLink_ZC(uint32_t channel);
extern void R_ETHER_LinkProcess(uint32_t channel);
extern ether_return_t R_ETHER_WakeOnLAN(uint32_t channel);
extern ether_return_t R_ETHER_CheckWrite(uint32_t channel);
extern ether_return_t R_ETHER_Control(ether_cmd_t const cmd, ether_param_t const control);
extern uint32_t R_ETHER_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif  /* R_ETHER_RZA2_IF_H*/
