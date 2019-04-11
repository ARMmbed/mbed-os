/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
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

/* Major numbers (4 bit) to identify
 * the consumer of shared data in runtime SW
 */
#define TLV_MAJOR_CORE     0x0
#define TLV_MAJOR_IAS      0x1

/**
 * The shared data between boot loader and runtime SW is TLV encoded. The
 * shared data is stored in a well known location in secure memory and this is
 * a contract between boot loader and runtime SW.
 *
 * The structure of shared data must be the following:
 *  - At the beginning there must be a header: struct shared_data_tlv_header
 *    This contains a magic number and a size field which covers the entire
 *    size of the shared data area including this header.
 *  - After the header there come the entries which are composed from an entry
 *    header structure: struct shared_data_tlv_entry and the data. In the entry
 *    header is a type field (tly_type) which identify the consumer of the
 *    entry in the runtime SW and specify the subtype of that data item. There
 *    is a size field (tlv_len) which covers the size of the entry header and
 *    the data. After this structure comes the actual data.
 *  - Arbitrary number and size of data entry can be in the shared memory area.
 *
 * This table gives of overview about the tlv_type field in the entry header.
 * The tlv_type always composed from a major and minor number. Major number
 * identifies the addressee in runtime SW, who should process the data entry.
 * Minor number used to encode more info about the data entry. The actual
 * definition of minor number could change per major number. In case of boot
 * status data, which is going to be processed by initial attestation service
 * the minor number is split further to two part: sw_module and claim. The
 * sw_module identifies the SW component in the system which the data item
 * belongs to and the claim part identifies the exact type of the data.
 *
 * |---------------------------------------|
 * |            tlv_type (16)              |
 * |---------------------------------------|
 * | tlv_major(4)|      tlv_minor(12)      |
 * |---------------------------------------|
 * | MAJOR_IAS   | sw_module(6) | claim(6) |
 * |---------------------------------------|
 * | MAJOR_CORE  |          TBD            |
 * |---------------------------------------|
 */

/* Initial attestation: SW components / SW modules
 * This list is intended to be adjusted per device. It contains more SW
 * components than currently available in TF-M project. It serves as an example,
 * what kind of SW components might be available.
 */
#define SW_GENERAL     0x00
#define SW_BL2         0x01
#define SW_PROT        0x02
#define SW_AROT        0x03
#define SW_SPE         0x04
#define SW_NSPE        0x05
#define SW_S_NS        0x06
#define SW_MAX         0x07

/* Initial attestation: Claim per SW components / SW modules */
/* Bits: 0-2 */
#define SW_VERSION       0x00
#define SW_SIGNER_ID     0x01
#define SW_EPOCH         0x02
#define SW_TYPE          0x03
/* Bits: 3-5 */
#define SW_MEASURE_VALUE 0x08
#define SW_MEASURE_TYPE  0x09

/* Initial attestation: General claim does not belong any particular SW
 * component. But they might be part of the boot status.
 */
#define BOOT_SEED          0x00
#define HW_VERSION         0x01
#define SECURITY_LIFECYCLE 0x02

/* Minor numbers (12 bit) to identify attestation service related data */
#define TLV_MINOR_IAS_BOOT_SEED       ((SW_GENERAL << 6) | BOOT_SEED)
#define TLV_MINOR_IAS_HW_VERSION      ((SW_GENERAL << 6) | HW_VERSION)
#define TLV_MINOR_IAS_SLC             ((SW_GENERAL << 6) | SECURITY_LIFECYCLE)

/* Bootloader - It can be more stage */
#define TLV_MINOR_IAS_BL2_MEASURE_VALUE  ((SW_BL2  << 6) | SW_MEASURE_VALUE)
#define TLV_MINOR_IAS_BL2_MEASURE_TYPE   ((SW_BL2  << 6) | SW_MEASURE_TYPE)
#define TLV_MINOR_IAS_BL2_VERSION        ((SW_BL2  << 6) | SW_VERSION)
#define TLV_MINOR_IAS_BL2_SIGNER_ID      ((SW_BL2  << 6) | SW_SIGNER_ID)
#define TLV_MINOR_IAS_BL2_EPOCH          ((SW_BL2  << 6) | SW_EPOCH)
#define TLV_MINOR_IAS_BL2_TYPE           ((SW_BL2  << 6) | SW_TYPE)

/* PROT: PSA Root of Trust */
#define TLV_MINOR_IAS_PROT_MEASURE_VALUE ((SW_PROT << 6) | SW_MEASURE_VALUE)
#define TLV_MINOR_IAS_PROT_MEASURE_TYPE  ((SW_PROT << 6) | SW_MEASURE_TYPE)
#define TLV_MINOR_IAS_PROT_VERSION       ((SW_PROT << 6) | SW_VERSION)
#define TLV_MINOR_IAS_PROT_SIGNER_ID     ((SW_PROT << 6) | SW_SIGNER_ID)
#define TLV_MINOR_IAS_PROT_EPOCH         ((SW_PROT << 6) | SW_EPOCH)
#define TLV_MINOR_IAS_PROT_TYPE          ((SW_PROT << 6) | SW_TYPE)

/* AROT: Application Root of Trust */
#define TLV_MINOR_IAS_AROT_MEASURE_VALUE ((SW_AROT << 6) | SW_MEASURE_VALUE)
#define TLV_MINOR_IAS_AROT_MEASURE_TYPE  ((SW_AROT << 6) | SW_MEASURE_TYPE)
#define TLV_MINOR_IAS_AROT_VERSION       ((SW_AROT << 6) | SW_VERSION)
#define TLV_MINOR_IAS_AROT_SIGNER_ID     ((SW_AROT << 6) | SW_SIGNER_ID)
#define TLV_MINOR_IAS_AROT_EPOCH         ((SW_AROT << 6) | SW_EPOCH)
#define TLV_MINOR_IAS_AROT_TYPE          ((SW_AROT << 6) | SW_TYPE)

/* Non-secure processing environment - single non-secure image */
#define TLV_MINOR_IAS_NSPE_MEASURE_VALUE ((SW_NSPE << 6) | SW_MEASURE_VALUE)
#define TLV_MINOR_IAS_NSPE_MEASURE_TYPE  ((SW_NSPE << 6) | SW_MEASURE_TYPE)
#define TLV_MINOR_IAS_NSPE_VERSION       ((SW_NSPE << 6) | SW_VERSION)
#define TLV_MINOR_IAS_NSPE_SIGNER_ID     ((SW_NSPE << 6) | SW_SIGNER_ID)
#define TLV_MINOR_IAS_NSPE_EPOCH         ((SW_NSPE << 6) | SW_EPOCH)
#define TLV_MINOR_IAS_NSPE_TYPE          ((SW_NSPE << 6) | SW_TYPE)

/* Secure processing environment (ARoT + PRoT) - single secure image */
#define TLV_MINOR_IAS_SPE_MEASURE_VALUE  ((SW_SPE  << 6) | SW_MEASURE_VALUE)
#define TLV_MINOR_IAS_SPE_MEASURE_TYPE   ((SW_SPE  << 6) | SW_MEASURE_TYPE)
#define TLV_MINOR_IAS_SPE_VERSION        ((SW_SPE  << 6) | SW_VERSION)
#define TLV_MINOR_IAS_SPE_SIGNER_ID      ((SW_SPE  << 6) | SW_SIGNER_ID)
#define TLV_MINOR_IAS_SPE_EPOCH          ((SW_SPE  << 6) | SW_EPOCH)
#define TLV_MINOR_IAS_SPE_TYPE           ((SW_SPE  << 6) | SW_TYPE)

/* SPE + NSPE - combined secure and non-secure image */
#define TLV_MINOR_IAS_S_NS_MEASURE_VALUE ((SW_S_NS << 6) | SW_MEASURE_VALUE)
#define TLV_MINOR_IAS_S_NS_MEASURE_TYPE  ((SW_S_NS << 6) | SW_MEASURE_TYPE)
#define TLV_MINOR_IAS_S_NS_VERSION       ((SW_S_NS << 6) | SW_VERSION)
#define TLV_MINOR_IAS_S_NS_SIGNER_ID     ((SW_S_NS << 6) | SW_SIGNER_ID)
#define TLV_MINOR_IAS_S_NS_EPOCH         ((SW_S_NS << 6) | SW_EPOCH)
#define TLV_MINOR_IAS_S_NS_TYPE          ((SW_S_NS << 6) | SW_TYPE)

/* General macros to handle TLV type */
#define MAJOR_MASK 0xF     /* 4  bit */
#define MAJOR_POS  12      /* 12 bit */
#define MINOR_MASK 0xFFF   /* 12 bit */

#define SET_TLV_TYPE(major, minor) \
        ((((major) & MAJOR_MASK) << MAJOR_POS) | ((minor) & MINOR_MASK))
#define GET_MAJOR(tlv_type) ((tlv_type) >> MAJOR_POS)
#define GET_MINOR(tlv_type) ((tlv_type) &  MINOR_MASK)

/* Initial attestation specific macros */
#define MODULE_POS 6               /* 6 bit */
#define CLAIM_MASK 0x3F            /* 6 bit */
#define MEASUREMENT_CLAIM_POS 3    /* 3 bit */

#define GET_IAS_MODULE(tlv_type) (GET_MINOR(tlv_type) >> MODULE_POS)
#define GET_IAS_CLAIM(tlv_type)  (GET_MINOR(tlv_type)  & CLAIM_MASK)
#define SET_IAS_MINOR(sw_module, claim) (((sw_module) << 6) | (claim))

#define GET_IAS_MEASUREMENT_CLAIM(ias_claim) ((ias_claim) >> \
                                              MEASUREMENT_CLAIM_POS)

/* Magic value which marks the beginning of shared data area in memory */
#define SHARED_DATA_TLV_INFO_MAGIC    0x2016

/**
 * Shared data TLV header.  All fields in little endian.
 *
 *    -----------------------------------
 *    | tlv_magic(16) | tlv_tot_len(16) |
 *    -----------------------------------
 */
struct shared_data_tlv_header {
    uint16_t tlv_magic;
    uint16_t tlv_tot_len; /* size of whole TLV area (including this header) */
};

#define SHARED_DATA_HEADER_SIZE sizeof(struct shared_data_tlv_header)

/**
 * Shared data TLV entry header format. All fields in little endian.
 *
 *    -------------------------------
 *    | tlv_type(16) |  tlv_len(16) |
 *    -------------------------------
 *    |         Raw data            |
 *    -------------------------------
 */
struct shared_data_tlv_entry {
    uint16_t tlv_type;
    uint16_t tlv_len; /* size of single TLV entry (including this header). */
};

#define SHARED_DATA_ENTRY_HEADER_SIZE sizeof(struct shared_data_tlv_entry)
#define SHARED_DATA_ENTRY_SIZE(size) (size + SHARED_DATA_ENTRY_HEADER_SIZE)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_BOOT_STATUS_H__ */
