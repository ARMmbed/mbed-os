//*****************************************************************************
//
//  am_hal_secure_ota.h
//! @file
//!
//! @brief Functions for secure over-the-air.
//!
//! @addtogroup
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#ifndef AM_HAL_SECURE_OTA_H
#define AM_HAL_SECURE_OTA_H
// Ambiq Standard Image Format related definitions
// Magic Numbers
#define AM_IMAGE_MAGIC_SBL                0xA3
#define AM_IMAGE_MAGIC_AM3P               0x3A
#define AM_IMAGE_MAGIC_PATCH              0xAF
#define AM_IMAGE_MAGIC_MAIN               0xC0
#define AM_IMAGE_MAGIC_CHILD              0xCC
#define AM_IMAGE_MAGIC_NONSECURE          0xCB
#define AM_IMAGE_MAGIC_INFO0              0xCF

// First 30 words of the image headers follow similar pattern
typedef struct
{
    union
    {
        uint32_t     ui32;
        struct
        {
            uint32_t    blobSize     : 20;
            uint32_t    resvd        : 3;
            uint32_t    encrypted    : 1;
            uint32_t    magicNum     : 8;
        } s;
    } w0;
    uint32_t        crc;
    union
    {
        uint32_t    ui32;
        struct
        {
            uint32_t    authAlgo     : 4;
            uint32_t    authKeyIdx   : 4;
            uint32_t    encAlgo      : 4;
            uint32_t    keyIdx       : 4;
            uint32_t    resvd3       : 8;
            uint32_t    crcBoot      : 1;
            uint32_t    authBoot     : 1;
            uint32_t    resvd2       : 2;
            uint32_t    crcInstall   : 1;
            uint32_t    authInstall  : 1;
            uint32_t    resvd1       : 2;
        } s;
    } w2;
    uint32_t        w3;
    uint32_t        signature[8]; // w4-11
    uint32_t        iv[4]; // w12-w15
    uint32_t        kek[4]; // w16-w19
    uint32_t        signatureClear[8]; // w20-w27
    union
    {
        uint32_t   ui32;
        struct
        {
            uint32_t    offsetWords     : 16;
            uint32_t    sizeWords       : 16;
        } info0;
        struct
        {
            uint32_t    encap        : 1;
            uint32_t    resvd        : 1;
            uint32_t    loadAddrMsb  : 30;
        } s1;
        struct
        {
            uint32_t    writeProtect : 1;
            uint32_t    copyProtect  : 1;
            uint32_t    loadAddrMsb  : 30;
        } s;
    } addrWord; // w28
    union
    {
        uint32_t   ui32;
        uint32_t   resv;
        uint32_t   key; // For info
        struct
        {
            uint32_t    version         : 15;
            uint32_t    erasePrev       : 1;
            uint32_t    resv            : 16;
        } s;
    } versionKeyWord; // w29
} am_image_hdr_common_t;

// Bitmask used to clear encrypted status in flash
#define AM_IMAGE_BITMASK_ENCRYPTED            0x00800000
// Number of most significant bits in w28 used for load address
#define AM_IMAGE_LOAD_ADDR_MSB_BITS           30

#define AM_IMAGE_GET_LOADADDR(common)         (((am_image_hdr_common_t *)(common))->addrWord.s.loadAddrMsb << (32 - AM_IMAGE_LOAD_ADDR_MSB_BITS))

#define AM_IMAGE_NUM_TRAILING_WORDS_TO_256    ((256 - sizeof(am_image_hdr_common_t))/4)
#define AM_IMAGE_MAX_CHILD_IMAGE              AM_IMAGE_NUM_TRAILING_WORDS_TO_256

typedef struct
{
    am_image_hdr_common_t common;
    uint32_t        childPtr[AM_IMAGE_MAX_CHILD_IMAGE];
} am_main_image_hdr_t;

typedef struct
{
    am_image_hdr_common_t common;
    uint32_t        featureKey;
    uint32_t        resvd[1];
} am_thirdparty_image_hdr_t;

typedef struct
{
    am_image_hdr_common_t common;
    uint32_t        resvd[AM_IMAGE_NUM_TRAILING_WORDS_TO_256];
} am_sbl_image_hdr_t;

// Reserved magic numbers allowed to be used by customer's own bootloader
#define AM_IMAGE_MAGIC_CUST(x)   ((((x) & 0xF0) == 0xC0) && ((x) != 0xC0) && ((x) != 0xCC) && ((x) != 0xCB) && ((x) != 0xCF))

// OTA Upgrade related definitions

// Maximum number of OTAs
#define AM_HAL_SECURE_OTA_MAX_OTA           8

// OTA Protocol between OTA application and SecureBoot
// OTAPOINTER will be initialized as follows:
// Most significant 30 bits will correspond to most significant 30 bits of OTA Descriptor
// Least Significant bit (bit 0) should be initialized to 1 to indicate a valid OTA Descriptor
// bit 1 should be initialized to 1 to indicate that the list contains an SBL OTA
// OTA Descriptor points to a list of entries, each corresponding to an OTA blob, list terminating in 0xFFFFFFFF
// Each list entry word comprises of following:
// Most significant 30 bits will correspond to most significant 30 bits of OTA blob pointer
// Blob pointer needs to be aligned to Flash Page boundary (8K)
// Least Significant 2 bits should be initialized to 1 to indicate a valid OTA Pending
// After Secboot processes an OTA, it clears the least significant bit (bit 0)
// bit 1 indicates the status of the OTA - 0 for Success, 1 for Failure
#define AM_HAL_SECURE_OTA_OTA_VALID_MASK                0x3
#define AM_HAL_SECURE_OTA_OTA_GET_BLOB_PTR(ptr)         ((uint32_t)(ptr) & ~AM_HAL_SECURE_OTA_OTA_VALID_MASK)
#define AM_HAL_SECURE_OTA_OTA_IS_VALID(ptr)             (((uint32_t)(ptr) & AM_HAL_SECURE_OTA_OTA_VALID_MASK) == AM_HAL_SECURE_OTA_OTA_VALID_MASK)
#define AM_HAL_SECURE_OTA_OTA_LIST_END_MARKER           0xFFFFFFFF

// Bitmasks signifying the bit to be cleared for OTA success/failure
#define AM_HAL_SECURE_OTA_OTA_DONE_FAILURE_CLRMASK      0x1
#define AM_HAL_SECURE_OTA_OTA_DONE_SUCCESS_CLRMASK      0x3


// OTA Status
typedef enum
{
    AM_HAL_OTA_STATUS_SUCCESS = 0x0,
    AM_HAL_OTA_STATUS_ERROR = 0x1, // This should never happen
    AM_HAL_OTA_STATUS_FAILURE = 0x2,
    AM_HAL_OTA_STATUS_PENDING = 0x3,
} am_hal_ota_status_e;

// Per Image OTA Status information
typedef struct
{
    uint32_t            *pImage;
    am_hal_ota_status_e status;
} am_hal_ota_status_t;

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

// pOtaDesc should be start of a flash page designated for OTA Descriptor
// This call will erase the flash page, which will then be incrementally populated as OTA's are added
// It will also initialize the OTAPOINTER to point to this descriptor, with LSB indicating it as invalid
uint32_t am_hal_ota_init(uint32_t ui32ProgamKey, uint32_t *pOtaDesc);

// Add a new OTA to descriptor
// This will program the next available entry in OTA descriptor
// Will also set the valid/sbl flags in OTA pointer register
uint32_t am_hal_ota_add(uint32_t ui32ProgamKey, uint8_t imageMagic, uint32_t *pImage);

// Get OTA Status
// Can be called anytime (generally after coming back from reset to check the status of OTA
// Will be also used by sbl_main to identify list of OTA's left for it (would show up as PENDING)
uint32_t am_hal_get_ota_status(uint32_t *pOtaDesc, uint32_t maxOta, am_hal_ota_status_t *pStatus);
    
#ifdef __cplusplus
}
#endif //__cplusplus

#endif // AM_HAL_SECURE_OTA_H
