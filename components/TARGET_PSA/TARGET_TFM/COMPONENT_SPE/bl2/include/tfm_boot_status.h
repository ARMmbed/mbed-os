/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_BOOT_STATUS_H__
#define __TFM_BOOT_STATUS_H__

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Major numbers to identify the consumer of shared data in runtime SW */
#define TLV_MAJOR_CORE  0x0
#define TLV_MAJOR_IAS   0x1

/* PSA Root of Trust */
#define TLV_MINOR_IAS_PRoT_SHA256     0x00
#define TLV_MINOR_IAS_PRoT_SW_VERSION 0x01
#define TLV_MINOR_IAS_PRoT_EPOCH      0x02

/* Application Root of Trust */
#define TLV_MINOR_IAS_ARoT_SHA256     0x03
#define TLV_MINOR_IAS_ARoT_SW_VERSION 0x04
#define TLV_MINOR_IAS_ARoT_EPOCH      0x05

/* Non-secure processing environment: single non-secure image */
#define TLV_MINOR_IAS_NSPE_SHA256     0x06
#define TLV_MINOR_IAS_NSPE_SW_VERSION 0x07
#define TLV_MINOR_IAS_NSPE_EPOCH      0x08

/* ARoT + PRoT:  single secure image */
#define TLV_MINOR_IAS_S_SHA256        0x09
#define TLV_MINOR_IAS_S_SW_VERSION    0x0a
#define TLV_MINOR_IAS_S_EPOCH         0x0b

/* S + NS: combined secure and non-secure image */
#define TLV_MINOR_IAS_S_NS_SHA256     0x0c
#define TLV_MINOR_IAS_S_NS_SW_VERSION 0x0d
#define TLV_MINOR_IAS_S_NS_EPOCH      0x0e

#define SHARED_DATA_TLV_INFO_MAGIC    0x2016

/**
 * Shared data TLV header.  All fields in little endian.
 *
 *    ---------------------------
 *    | tlv_magic | tlv_tot_len |
 *    ---------------------------
 */
struct shared_data_tlv_header {
    uint16_t tlv_magic;
    uint16_t tlv_tot_len; /* size of whole TLV area (including this header) */
};

#define SHARED_DATA_HEADER_SIZE sizeof(struct shared_data_tlv_header)

/**
 * Shared data TLV entry header format. All fields in little endian.
 *
 *    ---------------------------------------------
 *    | tlv_major_type | tlv_minor_type | tlv_len |
 *    ---------------------------------------------
 *    |                    Raw data               |
 *    ---------------------------------------------
 */
struct shared_data_tlv_entry {
    uint8_t  tlv_major_type;
    uint8_t  tlv_minor_type;
    uint16_t tlv_len; /* size of single TLV entry (including this header). */
};

#define SHARED_DATA_ENTRY_HEADER_SIZE sizeof(struct shared_data_tlv_entry)
#define SHARED_DATA_ENTRY_SIZE(size) (size + SHARED_DATA_ENTRY_HEADER_SIZE)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_BOOT_STATUS_H__ */
