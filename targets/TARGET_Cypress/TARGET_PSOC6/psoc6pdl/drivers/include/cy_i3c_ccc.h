/***************************************************************************//**
* \file cy_i3c_ccc.h
* \version 1.00
*
* Provides API declarations for the I3C Controller.
*
********************************************************************************
* \copyright
* Copyright 2019-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if defined (CY_IP_MXI3C)

#if !defined (CY_I3C_CCC_H)
#define CY_I3C_CCC_H

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
*                               API Constants
*******************************************************************************/

/**
* \addtogroup group_i3c_macros_common_macros Common Macros
* \{
*/

/** Bits per long */
#define CY_I3C_BITS_PER_LONG (32)

/**
 * Create a contiguous bitmask starting at bit position l and ending at position h. For example
 * CY_I3C_GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000FFFFE00000.
 */
#define CY_I3C_GENMASK(h, l) (((~(0UL)) - ((1UL) << (l)) + 1) & (~(0UL) >> (CY_I3C_BITS_PER_LONG - 1 - (h))))

/** Create a bitmask at bit position @n */
#define CY_I3C_BIT(n) ((1UL) << n)

/** Divide and round up */
#define CY_I3C_DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

/** Swap the bytes of a 16-bit word */
#define CY_I3C_SWAP16(x) ((uint16_t)((((uint16_t)(x) & (uint16_t)0x00FFU) << 8) | (((uint16_t)(x) & (uint16_t)0xFF00U) >> 8) ))

/** Dircet CCC Mask */
#define CY_I3C_CCC_DIRECT                0x80UL

/**Create Broadcast/Direct CCC Id */
#define CY_I3C_CCC_ID(id, broadcast)    ((id) | ((broadcast) ? 0 : CY_I3C_CCC_DIRECT))

/** Enable SLave Events Command: valid in both broadcast and unicast modes */
#define CY_I3C_CCC_ENEC(broadcast)            CY_I3C_CCC_ID(0x0, broadcast)

/** Disable SLave Events Command: valid in both broadcast and unicast modes */
#define CY_I3C_CCC_DISEC(broadcast)            CY_I3C_CCC_ID(0x1, broadcast)

/** Enter Activity State 0-3 Command: valid in both broadcast and unicast modes */
#define CY_I3C_CCC_ENTAS(as, broadcast)        CY_I3C_CCC_ID(0x2 + (as), broadcast)

/** Reset Dynamic Address Assignment Command: valid in both broadcast and unicast modes */
#define CY_I3C_CCC_RSTDAA(broadcast)        CY_I3C_CCC_ID(0x6, broadcast)

/** Set Maaximum Write Length Command: valid in both broadcast and unicast modes */
#define CY_I3C_CCC_SETMWL(broadcast)        CY_I3C_CCC_ID(0x9, broadcast)

/**Set Maximum Read Length Command: valid in both broadcast and unicast modes */
#define CY_I3C_CCC_SETMRL(broadcast)        CY_I3C_CCC_ID(0xA, broadcast)

/** Enter Dynamic Address Assignment Command: Broadcast-only command */
#define CY_I3C_CCC_ENTDAA            CY_I3C_CCC_ID(0x7, true)

/** Define List of Slaves Command: Broadcast-only command */
#define CY_I3C_CCC_DEFSLVS            CY_I3C_CCC_ID(0x8, true)

/** Enter HDR Mode Command: Broadcast-only command */
#define CY_I3C_CCC_ENTHDR(x)        CY_I3C_CCC_ID(0x20 + (x), true)

/** Set Dynamic Address from Static Address Command: Unicast-only command */
#define CY_I3C_CCC_SETDASA            CY_I3C_CCC_ID(0x7, false)

/** Set New Dynamic Address Command: Unicast-only command */
#define CY_I3C_CCC_SETNEWDA            CY_I3C_CCC_ID(0x8, false)

/** Get Maximum Write Length Command: Unicast-only command */
#define CY_I3C_CCC_GETMWL            CY_I3C_CCC_ID(0xB, false)

/** Get Maximum Read Length Command: Unicast-only command */
#define CY_I3C_CCC_GETMRL            CY_I3C_CCC_ID(0xC, false)

/** Get Provisioned ID(PID) Command: Unicast-only command */
#define CY_I3C_CCC_GETPID            CY_I3C_CCC_ID(0xD, false)

/** Get Bus Characteristic Register(BCR) Command: Unicast-only command */
#define CY_I3C_CCC_GETBCR            CY_I3C_CCC_ID(0xE, false)

/** Get Device Characteristic Register(DCR) Command: Unicast-only command */
#define CY_I3C_CCC_GETDCR            CY_I3C_CCC_ID(0xF, false)

/** Get Device Status Command: Unicast-only command */
#define CY_I3C_CCC_GETSTATUS        CY_I3C_CCC_ID(0x10, false)

/** Get Accept Mastership Command: Unicast-only command */
#define CY_I3C_CCC_GETACCMST        CY_I3C_CCC_ID(0x11, false)

/** Get Maximum Data Speed Command: Unicast-only command */
#define CY_I3C_CCC_GETMXDS            CY_I3C_CCC_ID(0x14, false)

/** Get HDR Capability Command: Unicast-only command */
#define CY_I3C_CCC_GETHDRCAP        CY_I3C_CCC_ID(0x15, false)

/** Slave Interrupt Request Event \ref cy_stc_i3c_ccc_events_t */
#define CY_I3C_CCC_EVENT_SIR        CY_I3C_BIT(0)

/** Mastership Request Event  \ref cy_stc_i3c_ccc_events_t */
#define CY_I3C_CCC_EVENT_MR            CY_I3C_BIT(1)

/**Hot-join Event  \ref cy_stc_i3c_ccc_events_t */
#define CY_I3C_CCC_EVENT_HJ            CY_I3C_BIT(3)

/** HDR Read Command Code */
#define CY_I3C_HDR_IS_READ_CMD        CY_I3C_BIT(7)

/**
* \addtogroup group_i3c_macros_common_macros_getstatus_ccc_fields GetStatus CCC Fields
* \{
*/

/** 
* Contains the interrupt number of any pending interrupt, 
* or 0 if no interrupts are pending 
*/
#define I3C_CCC_STATUS_PENDING_INT(status)    ((status) & CY_I3C_GENMASK(3, 0))

/** Indicates the Slave has detected a protocol error */
#define I3C_CCC_STATUS_PROTOCOL_ERROR        CY_I3C_BIT(5)

/** Indicates the Slave Device's current activity mode */
#define I3C_CCC_STATUS_ACTIVITY_MODE(status) (((status) & CY_I3C_GENMASK(7, 6)) >> 6)

/** \} group_i3c_macros_common_macros_getstatus_ccc_fields */

/** \} group_i3c_macros_common_macros */

/*******************************************************************************
*                              Type Definitions
*******************************************************************************/

/**
* \addtogroup group_i3c_data_structures
* \{
*/

/** ENEC/DISEC CCC payload structure */
typedef struct cy_stc_i3c_ccc_events 
{
    uint8_t events;  /**< bitmask of CY_I3C_CCC_EVENT_xxx events */
}cy_stc_i3c_ccc_events_t;

/** SETMWL/GETMWL CCC payload structure */
typedef struct cy_stc_i3c_ccc_mrwl 
{
    uint16_t len;  /**< maximum write length in bytes */
}cy_stc_i3c_ccc_mrwl_t;

/** SETMRL/GETMRL CCC payload structure */
typedef struct cy_stc_i3c_ccc_mrl 
{
    uint16_t readLen; /**< maximum read length in bytes */
}cy_stc_i3c_ccc_mrl_t;

/** I3C/I2C device descriptor used for DEFSLVS */
typedef struct cy_stc_i3c_ccc_dev_desc 
{
    uint8_t dynAddress; /**< dynamic address assigned to the I3C slave or 0 if the entry is describing an I2C Slave */
    union {
        uint8_t dcr; /**< DCR value (not applicable to entries describing I2C devices) */
        uint8_t lvr; /**< LVR value (not applicable to entries describing I3C devices) */
    };
    uint8_t bcr; /**< BCR value or 0 if this entry is describing an I2C slave */
    uint8_t staticAddress; /**< static address or 0 if the device does not have a static address */
}cy_stc_i3c_ccc_dev_desc_t;

/** DEFSLVS CCC payload structure */
typedef struct cy_stc_i3c_ccc_defslvs 
{
    uint8_t count; /**<  number of dev descriptors */
    cy_stc_i3c_ccc_dev_desc_t master; /**< descriptor describing the current master */
    cy_stc_i3c_ccc_dev_desc_t *slaves; /**< array of descriptors describing slaves controlled by the current master */
}cy_stc_i3c_ccc_defslvs_t ;

/** SETNEWDA and SETDASA CCCs payload structure */
typedef struct cy_stc_i3c_ccc_setda 
{
    uint8_t address; /**<  dynamic address to assign to an I3C device */
}cy_stc_i3c_ccc_setda_t;

/** GETPID CCC payload structure */
typedef struct cy_stc_i3c_ccc_getpid 
{
    uint8_t pid[6]; /**< 48 bits PID in big endian */
}cy_stc_i3c_ccc_getpid_t;

/** GETBCR CCC payload structure */
typedef struct cy_stc_i3c_ccc_getbcr 
{
    uint8_t bcr; /**< BCR (Bus Characteristic Register) value */
}cy_stc_i3c_ccc_getbcr_t;

/** GETDCR CCC payload structure */
typedef struct cy_stc_i3c_ccc_getdcr 
{
    uint8_t dcr; /**< DCR (Device Characteristic Register) value */
}cy_stc_i3c_ccc_getdcr_t;

/** GETSTATUS CCC payload structure */
typedef struct cy_stc_i3c_ccc_getstatus 
{
    uint16_t status; /**< status of the I3C slave (see I3C_CCC_STATUS_xxx macros for more information */
}cy_stc_i3c_ccc_getstatus_t;

/** GETACCMST CCC payload structure */
typedef struct cy_stc_i3c_ccc_getaccmst 
{
    uint8_t newmaster; /**< address of the master taking bus ownership */
}cy_stc_i3c_ccc_getaccmst_t;

/** GETMXDS CCC payload structure */
typedef struct cy_stc_i3c_ccc_getmxds
{
    uint8_t maxwr; /**< write limitations */
    uint8_t maxrd; /**< read limitations */
    uint8_t maxrdturn[3]; /**< maximum read turn-around expressed micro-seconds and little-endian formatted */
}cy_stc_i3c_ccc_getmxds_t;

/** GETHDRCAP CCC payload structure */
typedef struct cy_stc_i3c_ccc_gethdrcap 
{
    uint8_t modes; /**< bitmap of supported HDR modes */
}cy_stc_i3c_ccc_gethdrcap_t;
/** \} group_i3c_data_structures */

#if defined(__cplusplus)
}
#endif

#endif /* (CY_I3C_CCC_H) */

#endif /* CY_IP_MXI3C */
/* [] END OF FILE */

