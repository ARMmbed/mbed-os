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
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name    : spibsc.h
* $Rev: 12 $
* $Date:: 2016-05-19 17:26:37 +0900#$
* Description  : 
******************************************************************************/
#ifndef _SPIBSC_H_
#define _SPIBSC_H_

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "iodefine.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define SPIBSC_CMNCR_MD_EXTRD       (0u)
#define SPIBSC_CMNCR_MD_SPI         (1u)

#define SPIBSC_OUTPUT_LOW           (0u)
#define SPIBSC_OUTPUT_HIGH          (1u)
#define SPIBSC_OUTPUT_LAST          (2u)
#define SPIBSC_OUTPUT_HiZ           (3u)

#define SPIBSC_CMNCR_CPHAT_EVEN     (0u)
#define SPIBSC_CMNCR_CPHAT_ODD      (1u)

#define SPIBSC_CMNCR_CPHAR_ODD      (0u)
#define SPIBSC_CMNCR_CPHAR_EVEN     (1u)

#define SPIBSC_CMNCR_SSLP_LOW       (0u)
#define SPIBSC_CMNCR_SSLP_HIGH      (1u)

#define SPIBSC_CMNCR_CPOL_LOW       (0u)
#define SPIBSC_CMNCR_CPOL_HIGH      (1u)

#define SPIBSC_CMNCR_BSZ_SINGLE     (0u)
#define SPIBSC_CMNCR_BSZ_DUAL       (1u)

#define SPIBSC_DELAY_1SPBCLK        (0u)
#define SPIBSC_DELAY_2SPBCLK        (1u)
#define SPIBSC_DELAY_3SPBCLK        (2u)
#define SPIBSC_DELAY_4SPBCLK        (3u)
#define SPIBSC_DELAY_5SPBCLK        (4u)
#define SPIBSC_DELAY_6SPBCLK        (5u)
#define SPIBSC_DELAY_7SPBCLK        (6u)
#define SPIBSC_DELAY_8SPBCLK        (7u)


#define SPIBSC_BURST_1              (0x00u)
#define SPIBSC_BURST_2              (0x01u)
#define SPIBSC_BURST_3              (0x02u)
#define SPIBSC_BURST_4              (0x03u)
#define SPIBSC_BURST_5              (0x04u)
#define SPIBSC_BURST_6              (0x05u)
#define SPIBSC_BURST_7              (0x06u)
#define SPIBSC_BURST_8              (0x07u)
#define SPIBSC_BURST_9              (0x08u)
#define SPIBSC_BURST_10             (0x09u)
#define SPIBSC_BURST_11             (0x0au)
#define SPIBSC_BURST_12             (0x0bu)
#define SPIBSC_BURST_13             (0x0cu)
#define SPIBSC_BURST_14             (0x0du)
#define SPIBSC_BURST_15             (0x0eu)
#define SPIBSC_BURST_16             (0x0fu)

#define SPIBSC_BURST_DISABLE        (0u)
#define SPIBSC_BURST_ENABLE         (1u)

#define SPIBSC_DRCR_RCF_EXE         (1u)

#define SPIBSC_SSL_NEGATE           (0u)
#define SPIBSC_TRANS_END            (1u)

#define SPIBSC_1BIT                 (0u)
#define SPIBSC_2BIT                 (1u)
#define SPIBSC_4BIT                 (2u)

#define SPIBSC_OUTPUT_DISABLE       (0u)
#define SPIBSC_OUTPUT_ENABLE        (1u)
#define SPIBSC_OUTPUT_ADDR_24       (0x07u)
#define SPIBSC_OUTPUT_ADDR_32       (0x0fu)
#define SPIBSC_OUTPUT_OPD_3         (0x08u)
#define SPIBSC_OUTPUT_OPD_32        (0x0cu)
#define SPIBSC_OUTPUT_OPD_321       (0x0eu)
#define SPIBSC_OUTPUT_OPD_3210      (0x0fu)

#define SPIBSC_OUTPUT_SPID_8        (0x08u)
#define SPIBSC_OUTPUT_SPID_16       (0x0cu)
#define SPIBSC_OUTPUT_SPID_32       (0x0fu)

#define SPIBSC_SPISSL_NEGATE        (0u)
#define SPIBSC_SPISSL_KEEP          (1u)

#define SPIBSC_SPIDATA_DISABLE      (0u)
#define SPIBSC_SPIDATA_ENABLE       (1u)

#define SPIBSC_SPI_DISABLE          (0u)
#define SPIBSC_SPI_ENABLE           (1u)


/* Use for setting of the DME bit of "data read enable register"(DRENR) */
#define SPIBSC_DUMMY_CYC_DISABLE    (0u)
#define SPIBSC_DUMMY_CYC_ENABLE     (1u)

/* Use for setting of the DMCYC [2:0] bit of "data read dummy cycle register"(DRDMCR) */
#define SPIBSC_DUMMY_1CYC           (0u)
#define SPIBSC_DUMMY_2CYC           (1u)
#define SPIBSC_DUMMY_3CYC           (2u)
#define SPIBSC_DUMMY_4CYC           (3u)
#define SPIBSC_DUMMY_5CYC           (4u)
#define SPIBSC_DUMMY_6CYC           (5u)
#define SPIBSC_DUMMY_7CYC           (6u)
#define SPIBSC_DUMMY_8CYC           (7u)

/* Use for setting of "data read DDR enable register"(DRDRENR) */
#define SPIBSC_SDR_TRANS            (0u)
#define SPIBSC_DDR_TRANS            (1u)

/* Use for setting the CKDLY regsiter */
#define SPIBSC_CKDLY_DEFAULT        (0x0000A504uL)    /* Initial value */
#define SPIBSC_CKDLY_TUNING         (0x0000A50AuL)    /* Shorten the data input setup time and extend the data hold time */

/* Use for setting the SPODLY regsiter */
#define SPIBSC_SPODLY_DEFAULT       (0xA5000000uL)    /* Initial value */
#define SPIBSC_SPODLY_TUNING        (0xA5001111uL)    /* Delay the data output delay/hold/buffer-on/buffer-off time */

#endif /* _SPIBSC_H_ */

/* End of File */
