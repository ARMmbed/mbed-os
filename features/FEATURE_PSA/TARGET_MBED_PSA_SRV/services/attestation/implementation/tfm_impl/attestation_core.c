/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "tfm_client.h"
#include "attestation.h"
#include "tfm_impl/tfm_boot_status.h"
#include "tfm_plat_defs.h"
#include "tfm_plat_device_id.h"
#include "tfm_plat_boot_seed.h"
#include "tfm_attest_hal.h"
#include "attest_token.h"
#include "attest_eat_defines.h"
#include "t_cose_defines.h"
#include "tfm_memory_utils.h"

#define MAX_BOOT_STATUS 512

/* Indicates how to encode SW components' measurements in the CBOR map */
#define EAT_SW_COMPONENT_NESTED     1  /* Nested map */
#define EAT_SW_COMPONENT_NOT_NESTED 0  /* Flat structure */

/* Indicates that the boot status does not contain any SW components'
 * measurement
 */
#define NO_SW_COMPONENT_FIXED_VALUE 1

/*!
 * \var boot_status
 *
 * \brief Array variable to store the boot status in service's memory.
 *
 * \details Boot status comes from the secure bootloader and primarily stored
 *          on a memory area which is shared between bootloader and SPM.
 *          SPM provides the \ref tfm_core_get_boot_data() API to retrieve
 *          the service related data from shared area.
 */

/* Enforcement of 4 byte alignment, which is checked by TF-M SPM */
__attribute__ ((aligned(4)))
static uint8_t boot_status[MAX_BOOT_STATUS];

enum psa_attest_err_t attest_init(void)
{
    enum psa_attest_err_t res;

    res = attest_get_boot_data(TLV_MAJOR_IAS, boot_status, MAX_BOOT_STATUS);

    return res;
}

/*!
 * \brief Static function to map return values between \ref attest_token_err_t
 *        and \ref psa_attest_err_t
 *
 * \param[in]  token_err  Token encoding return value
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static inline enum psa_attest_err_t
error_mapping(enum attest_token_err_t token_err)
{
    switch (token_err) {
    case ATTEST_TOKEN_ERR_SUCCESS:
        return PSA_ATTEST_ERR_SUCCESS;
        break;
    case ATTEST_TOKEN_ERR_TOO_SMALL:
        return PSA_ATTEST_ERR_TOKEN_BUFFER_OVERFLOW;
        break;
    default:
        return PSA_ATTEST_ERR_GENERAL;
    }
}

/*!
 * \brief Static function to convert a pointer and size info to unsigned
 *        integer number. Max 32bits unsigned integers are supported.
 *
 * This implementation assumes that the endianness of the sender and receiver
 * of the data is the same because they are actually running on the same CPU
 * instance. If this assumption is not true than this function must be
 * refactored accordingly.
 *
 * \param[in]  int_ptr  Pointer to the unsigned integer
 * \param[in]  len      Size of the unsigned integers in bytes
 * \param[in]  value    Pointer where to store the converted value
 *
 * \return Returns 0 on success and -1 on error.
 */
static inline int32_t get_uint(const void *int_ptr,
                               size_t len,
                               uint32_t *value)
{
    uint16_t uint16;

    switch (len) {
    case 1:
        *value = (uint32_t)(*(uint8_t  *)(int_ptr));
        break;
    case 2:
        /* Avoid unaligned access */
        tfm_memcpy(&uint16, int_ptr, sizeof(uint16));
        *value = (uint32_t)uint16;
        break;
    case 4:
        /* Avoid unaligned access */
        tfm_memcpy(value, int_ptr, sizeof(uint32_t));
        break;
    default:
        return -1;
    }

    return 0;
}
/*!
 * \brief Static function to look up all entires in the shared data area
 *       (boot status) which belong to a specific module.
 *
 * \param[in]     module  The identifier of SW module to look up based on this
 * \param[out]    claim   The type of SW module's attribute
 * \param[out]    tlv_len Length of the shared data entry
 * \param[in/out] tlv_ptr Pointer to the shared data entry. If its value NULL as
 *                        input then it will starts the look up from the
 *                        beginning of the shared data section. If not NULL then
 *                        it continue look up from the next entry. It returns
 *                        the address of next found entry which belongs to
 *                        module.
 *
 * \retval    -1          Error, boot status is malformed
 * \retval     0          Entry not found
 * \retval     1          Entry found
 */
static int32_t attest_get_tlv_by_module(uint8_t    module,
                                        uint8_t   *claim,
                                        uint16_t  *tlv_len,
                                        uint8_t  **tlv_ptr)
{
    struct shared_data_tlv_header *tlv_header;
    struct shared_data_tlv_entry tlv_entry;
    uint8_t *tlv_end;
    uint8_t *tlv_curr;

    tlv_header = (struct shared_data_tlv_header *)boot_status;
    if (tlv_header->tlv_magic != SHARED_DATA_TLV_INFO_MAGIC) {
        return -1;
    }

    /* Get the boundaries of TLV section where to lookup*/
    tlv_end  = (uint8_t *)boot_status + tlv_header->tlv_tot_len;
    if (*tlv_ptr == NULL) {
        /* At first call set to the beginning of the TLV section */
        tlv_curr = (uint8_t *)boot_status + SHARED_DATA_HEADER_SIZE;
    } else {
        /* Any subsequent call set to the next TLV entry */
        tfm_memcpy(&tlv_entry, *tlv_ptr, SHARED_DATA_ENTRY_HEADER_SIZE);
        tlv_curr  = (*tlv_ptr) + tlv_entry.tlv_len;
    }

    /* Iterates over the TLV section and returns the address and size of TLVs
     * with requested module identifier
     */
    for (; tlv_curr < tlv_end; tlv_curr += tlv_entry.tlv_len) {
        /* Create local copy to avoid unaligned access */
        tfm_memcpy(&tlv_entry, tlv_curr, SHARED_DATA_ENTRY_HEADER_SIZE);
        if (GET_IAS_MODULE(tlv_entry.tlv_type) == module) {
            *claim   = GET_IAS_CLAIM(tlv_entry.tlv_type);
            *tlv_ptr = tlv_curr;
            *tlv_len = tlv_entry.tlv_len;
            return 1;
        }
    }

    return 0;
}

/*!
 * \brief Static function to look up specific claim belongs to SW_GENERAL module
 *
 * \param[in]   claim    The claim ID to look for
 * \param[out]  tlv_len  Length of the shared data entry
 * \param[out]  tlv_ptr  Pointer to a shared data entry which belongs to the
 *                       SW_GENERAL module.
 *
 * \retval    -1          Error, boot status is malformed
 * \retval     0          Entry not found
 * \retval     1          Entry found
 */
static int32_t attest_get_tlv_by_id(uint8_t    claim,
                                    uint16_t  *tlv_len,
                                    uint8_t  **tlv_ptr)
{
    uint8_t tlv_id;
    uint8_t module = SW_GENERAL;
    int32_t found;

    /* Ensure that look up starting from the beginning of the boot status */
    *tlv_ptr = NULL;

    /* Look up specific TLV entry which belongs to SW_GENERAL module */
    do {
        /* Look up next entry */
        found = attest_get_tlv_by_module(module, &tlv_id,
                                         tlv_len, tlv_ptr);
        if (found != 1) {
            break;
        }
        /* At least one entry was found which belongs to SW_GENERAL,
         * check whether this one is looked for
         */
        if (claim == tlv_id) {
            break;
        }
    } while (found == 1);

    return found;
}

/*!
 * \brief Static function to add SW component related claims to attestation
 *        token in CBOR format.
 *
 *  This function translates between TLV  and CBOR encoding.
 *
 * \param[in]  token_ctx    Attestation token encoding context
 * \param[in]  tlv_id       The ID of claim
 * \param[in]  claim_value  A structure which carries a pointer and size about
 *                          the data item to be added to the token
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_sw_component_claim(struct attest_token_ctx *token_ctx,
                              uint8_t tlv_id,
                              const struct useful_buf_c *claim_value)
{
    int32_t res;
    uint32_t value;

    switch (tlv_id) {
    case SW_MEASURE_VALUE:
        attest_token_add_bstr(token_ctx,
                              EAT_CBOR_SW_COMPONENT_MEASUREMENT_VALUE,
                              claim_value);
        break;
    case SW_MEASURE_TYPE:
        attest_token_add_tstr(token_ctx,
                              EAT_CBOR_SW_COMPONENT_MEASUREMENT_DESC,
                              claim_value);
        break;
    case SW_VERSION:
        attest_token_add_tstr(token_ctx,
                              EAT_CBOR_SW_COMPONENT_VERSION,
                              claim_value);
        break;
    case SW_SIGNER_ID:
        attest_token_add_bstr(token_ctx,
                              EAT_CBOR_SW_COMPONENT_SIGNER_ID,
                              claim_value);
        break;
    case SW_EPOCH:
        res = get_uint(claim_value->ptr, claim_value->len, &value);
        if (res) {
            return PSA_ATTEST_ERR_GENERAL;
        }
        attest_token_add_integer(token_ctx,
                                 EAT_CBOR_SW_COMPONENT_SECURITY_EPOCH,
                                 (int64_t)value);
        break;
    case SW_TYPE:
        attest_token_add_tstr(token_ctx,
                              EAT_CBOR_SW_COMPONENT_MEASUREMENT_TYPE,
                              claim_value);
        break;
    default:
        return PSA_ATTEST_ERR_GENERAL;
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add the measurement data of a single SW components
 *        to the attestation token.
 *
 * \param[in]  token_ctx    Token encoding context
 * \param[in]  module       SW component identifier
 * \param[in]  tlv_address  Address of the first TLV entry in the boot status,
 *                          which belongs to this SW component.
 * \param[in]  nested_map   Flag to indicate that how to encode the SW component
 *                          measurement data: nested map or non-nested map.
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_single_sw_measurment(struct attest_token_ctx *token_ctx,
                                uint32_t module,
                                uint8_t *tlv_address,
                                uint32_t nested_map)
{
    struct shared_data_tlv_entry tlv_entry;
    uint16_t tlv_len;
    uint8_t  tlv_id;
    uint8_t *tlv_ptr = tlv_address;
    int32_t found = 1;
    struct useful_buf_c claim_value;
    enum psa_attest_err_t res;
    QCBOREncodeContext *cbor_encode_ctx;

    /* Create local copy to avoid unaligned access */
    tfm_memcpy(&tlv_entry, tlv_address, SHARED_DATA_ENTRY_HEADER_SIZE);
    tlv_len = tlv_entry.tlv_len;
    tlv_id = GET_IAS_CLAIM(tlv_entry.tlv_type);

    cbor_encode_ctx = attest_token_borrow_cbor_cntxt(token_ctx);

    /* Open nested map for SW component measurement claims */
    if (nested_map) {
        QCBOREncode_OpenMapInMapN(cbor_encode_ctx,
                                 EAT_CBOR_SW_COMPONENT_MEASUREMENT_VALUE);
    }

    /* Look up all measurement TLV entry which belongs to the SW component */
    while (found) {
         /* Here only measurement claims are added to the token */
         if (GET_IAS_MEASUREMENT_CLAIM(tlv_id)) {
            claim_value.ptr = tlv_ptr + SHARED_DATA_ENTRY_HEADER_SIZE;
            claim_value.len  = tlv_len - SHARED_DATA_ENTRY_HEADER_SIZE;
            res = attest_add_sw_component_claim(token_ctx,
                                                tlv_id,
                                                &claim_value);
            if (res != PSA_ATTEST_ERR_SUCCESS) {
                return res;
            }
        }

        /* Look up next entry it can be non-measurement claim*/
        found = attest_get_tlv_by_module(module, &tlv_id,
                                         &tlv_len, &tlv_ptr);
        if (found == -1) {
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        }
    }

    if (nested_map) {
        QCBOREncode_CloseMap(cbor_encode_ctx);
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add the claims of a single SW components to the
 *        attestation token.
 *
 * \param[in]  token_ctx    Token encoding context
 * \param[in]  module       SW component identifier
 * \param[in]  tlv_address  Address of the first TLV entry in the boot status,
 *                          which belongs to this SW component.
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_single_sw_component(struct attest_token_ctx *token_ctx,
                               uint32_t module,
                               uint8_t *tlv_address)
{
    struct shared_data_tlv_entry tlv_entry;
    uint16_t tlv_len;
    uint8_t  tlv_id;
    uint8_t *tlv_ptr = tlv_address;
    int32_t found = 1;
    uint32_t measurement_claim_cnt = 0;
    struct useful_buf_c claim_value;
    QCBOREncodeContext *cbor_encode_ctx;

    /* Create local copy to avoid unaligned access */
    tfm_memcpy(&tlv_entry, tlv_address, SHARED_DATA_ENTRY_HEADER_SIZE);
    tlv_len = tlv_entry.tlv_len;
    tlv_id = GET_IAS_CLAIM(tlv_entry.tlv_type);

    /* Open map which stores claims belong to a SW component */
    cbor_encode_ctx = attest_token_borrow_cbor_cntxt(token_ctx);
    QCBOREncode_OpenMap(cbor_encode_ctx);

    /*Look up all TLV entry which belongs to the same SW component */
    while (found) {
        /* Check whether claim is measurement claim */
        if (GET_IAS_MEASUREMENT_CLAIM(tlv_id)) {
            if (measurement_claim_cnt == 0) {
                /* Call only once when first measurement claim found */
                measurement_claim_cnt++;
                attest_add_single_sw_measurment(token_ctx,
                                                module,
                                                tlv_ptr,
                                                EAT_SW_COMPONENT_NOT_NESTED);
            }
        } else {
            /* Adding top level claims */
            claim_value.ptr = tlv_ptr + SHARED_DATA_ENTRY_HEADER_SIZE;
            claim_value.len  = tlv_len - SHARED_DATA_ENTRY_HEADER_SIZE;
            attest_add_sw_component_claim(token_ctx, tlv_id, &claim_value);
        }

        /* Look up next entry which belongs to SW component */
        found = attest_get_tlv_by_module(module, &tlv_id,
                                         &tlv_len, &tlv_ptr);
        if (found == -1) {
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        }
    }

    /* Close map which stores claims belong to a SW component */
    QCBOREncode_CloseMap(cbor_encode_ctx);

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add the claims of all SW components to the
 *        attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_all_sw_components(struct attest_token_ctx *token_ctx)
{
    uint16_t tlv_len;
    uint8_t *tlv_ptr;
    uint8_t  tlv_id;
    int32_t found;
    uint32_t cnt = 0;
    uint32_t module;
    QCBOREncodeContext *cbor_encode_ctx;

    /* Starting from module 1, because module 0 contains general claims which
     * are not related to SW module(i.e: boot_seed, etc.)
     */
    for (module = 1; module < SW_MAX; ++module) {
        /* Indicates to restart the look up from the beginning of the shared
         * data section
         */
        tlv_ptr = NULL;

        /* Look up the first TLV entry which belongs to the SW module */
        found = attest_get_tlv_by_module(module, &tlv_id,
                                         &tlv_len, &tlv_ptr);
        if (found == -1) {
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        }

        if (found == 1) {
            cnt++;
            if (cnt == 1) {
                /* Open array which stores SW components claims */
                cbor_encode_ctx = attest_token_borrow_cbor_cntxt(token_ctx);
                QCBOREncode_OpenArrayInMapN(cbor_encode_ctx,
                                            EAT_CBOR_ARM_LABEL_SW_COMPONENTS);
            }
            attest_add_single_sw_component(token_ctx, module, tlv_ptr);
        }
    }

    if (cnt != 0) {
        /* Close array which stores SW components claims*/
        QCBOREncode_CloseArray(cbor_encode_ctx);
    } else {
        /* If there is not any SW components' measurement in the boot status
         * then include this claim to indicate that this state is intentional
         */
        attest_token_add_integer(token_ctx,
                                 EAT_CBOR_ARM_LABEL_NO_SW_COMPONENTS,
                                 (int64_t)NO_SW_COMPONENT_FIXED_VALUE);
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add boot seed claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_boot_seed_claim(struct attest_token_ctx *token_ctx)
{
    /* FixMe: Enforcement of 4 byte alignment can be removed as soon as memory
     *        type is configured in the MPU to be normal, instead of device,
     *        which prohibits unaligned access.
     */
    __attribute__ ((aligned(4)))
    uint8_t boot_seed[BOOT_SEED_SIZE];
    enum tfm_plat_err_t res;
    struct useful_buf_c claim_value = {0};
    uint16_t tlv_len;
    uint8_t *tlv_ptr = NULL;
    int32_t found = 0;

    /* First look up BOOT_SEED in boot status, it might comes from bootloader */
    found = attest_get_tlv_by_id(BOOT_SEED, &tlv_len, &tlv_ptr);
    if (found == 1) {
        claim_value.ptr = tlv_ptr + SHARED_DATA_ENTRY_HEADER_SIZE;
        claim_value.len = tlv_len - SHARED_DATA_ENTRY_HEADER_SIZE;
    } else {
        /* If not found in boot status then use callback function to get it
         * from runtime SW
         */
        res = tfm_plat_get_boot_seed(sizeof(boot_seed), boot_seed);
        if (res != TFM_PLAT_ERR_SUCCESS) {
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        }
        claim_value.ptr = boot_seed;
        claim_value.len = BOOT_SEED_SIZE;
    }

    attest_token_add_bstr(token_ctx,
                          EAT_CBOR_ARM_LABEL_BOOT_SEED,
                          &claim_value);

    return PSA_ATTEST_ERR_SUCCESS;
}

/* FixMe: Remove this #if when MPU will be configured properly. Currently
 *        in case of TFM_LVL == 3 unaligned access triggers a usage fault
 *        exception.
 */
#if !defined(TFM_LVL) || (TFM_LVL == 1)
/*!
 * \brief Static function to add instance id claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_instance_id_claim(struct attest_token_ctx *token_ctx)
{
    /* FixMe: Enforcement of 4 byte alignment can be removed as soon as memory
     *        type is configured in the MPU to be normal, instead of device,
     *        which prohibits unaligned access.
     */
    __attribute__ ((aligned(4)))
    uint8_t instance_id[INSTANCE_ID_MAX_SIZE];
    enum tfm_plat_err_t res_plat;
    uint32_t size = sizeof(instance_id);
    struct useful_buf_c claim_value;

    res_plat = tfm_plat_get_instance_id(&size, instance_id);
    if (res_plat != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    claim_value.ptr = instance_id;
    claim_value.len  = size;
    attest_token_add_bstr(token_ctx,
                          EAT_CBOR_ARM_LABEL_UEID,
                          &claim_value);

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add implementation id claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_implementation_id_claim(struct attest_token_ctx *token_ctx)
{
    /* FixMe: Enforcement of 4 byte alignment can be removed as soon as memory
     *        type is configured in the MPU to be normal, instead of device,
     *        which prohibits unaligned access.
     */
    __attribute__ ((aligned(4)))
    uint8_t implementation_id[IMPLEMENTATION_ID_MAX_SIZE];
    enum tfm_plat_err_t res_plat;
    uint32_t size = sizeof(implementation_id);
    struct useful_buf_c claim_value;

    res_plat = tfm_plat_get_implementation_id(&size, implementation_id);
    if (res_plat != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
    }

    claim_value.ptr = implementation_id;
    claim_value.len  = size;
    attest_token_add_bstr(token_ctx,
                          EAT_CBOR_ARM_LABEL_IMPLEMENTATION_ID,
                          &claim_value);

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add hardware version claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_hw_version_claim(struct attest_token_ctx *token_ctx)
{
    /* FixMe: Enforcement of 4 byte alignment can be removed as soon as memory
     *        type is configured in the MPU to be normal, instead of device,
     *        which prohibits unaligned access.
     */
    __attribute__ ((aligned(4)))
    uint8_t hw_version[HW_VERSION_MAX_SIZE];
    enum tfm_plat_err_t res_plat;
    uint32_t size = sizeof(hw_version);
    struct useful_buf_c claim_value = {0};
    uint16_t tlv_len;
    uint8_t *tlv_ptr = NULL;
    int32_t found = 0;

    /* First look up HW version in boot status, it might comes
     * from bootloader
     */
    found = attest_get_tlv_by_id(HW_VERSION, &tlv_len, &tlv_ptr);
    if (found == 1) {
        claim_value.ptr = tlv_ptr + SHARED_DATA_ENTRY_HEADER_SIZE;
        claim_value.len = tlv_len - SHARED_DATA_ENTRY_HEADER_SIZE;
    } else {
        /* If not found in boot status then use callback function to get it
         * from runtime SW
         */
        res_plat = tfm_plat_get_hw_version(&size, hw_version);
        if (res_plat != TFM_PLAT_ERR_SUCCESS) {
            return PSA_ATTEST_ERR_CLAIM_UNAVAILABLE;
        }
        claim_value.ptr = hw_version;
        claim_value.len = size;
    }

    attest_token_add_tstr(token_ctx,
                          EAT_CBOR_ARM_LABEL_HW_VERSION,
                          &claim_value);

    return PSA_ATTEST_ERR_SUCCESS;
}
#endif

/*!
 * \brief Static function to add caller id claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_caller_id_claim(struct attest_token_ctx *token_ctx)
{
    enum psa_attest_err_t res;
    int32_t  caller_id;

    res = attest_get_caller_client_id(&caller_id);
    if (res != PSA_ATTEST_ERR_SUCCESS) {
        return res;
    }

    attest_token_add_integer(token_ctx,
                             EAT_CBOR_ARM_LABEL_CLIENT_ID,
                             (int64_t)caller_id);

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add security lifecycle claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */

static enum psa_attest_err_t
attest_add_security_lifecycle_claim(struct attest_token_ctx *token_ctx)
{
    enum tfm_security_lifecycle_t security_lifecycle;
    uint32_t slc_value;
    int32_t res;
    struct useful_buf_c claim_value = {0};
    uint16_t tlv_len;
    uint8_t *tlv_ptr = NULL;
    int32_t found = 0;

    /* First look up lifecycle state in boot status, it might comes
     * from bootloader
     */
    found = attest_get_tlv_by_id(SECURITY_LIFECYCLE, &tlv_len, &tlv_ptr);
    if (found == 1) {
        claim_value.ptr = tlv_ptr + SHARED_DATA_ENTRY_HEADER_SIZE;
        claim_value.len = tlv_len - SHARED_DATA_ENTRY_HEADER_SIZE;
        res = get_uint(claim_value.ptr, claim_value.len, &slc_value);
        if (res) {
            return PSA_ATTEST_ERR_GENERAL;
        }
        security_lifecycle = (enum tfm_security_lifecycle_t)slc_value;
    } else {
        /* If not found in boot status then use callback function to get it
         * from runtime SW
         */
        security_lifecycle = tfm_attest_hal_get_security_lifecycle();
    }

    /* Sanity check */
    if (security_lifecycle < TFM_SLC_UNKNOWN ||
        security_lifecycle > TFM_SLC_DECOMMISSIONED) {
        return PSA_ATTEST_ERR_GENERAL;
    }

    attest_token_add_integer(token_ctx,
                             EAT_CBOR_ARM_LABEL_SECURITY_LIFECYCLE,
                             (int64_t)security_lifecycle);

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add challenge claim to attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 * \param[in]  challenge  Pointer to buffer which stores the challenge
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_challenge_claim(struct attest_token_ctx   *token_ctx,
                           const struct useful_buf_c *challenge)
{
    attest_token_add_bstr(token_ctx, EAT_CBOR_ARM_LABEL_CHALLENGE, challenge);

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add the verification service indicator claim
 *        to the attestation token.
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_verification_service(struct attest_token_ctx *token_ctx)
{
    struct useful_buf_c service;
    uint32_t size;

    service.ptr = tfm_attest_hal_get_verification_service(&size);

    if (service.ptr) {
        service.len = size;
        attest_token_add_tstr(token_ctx,
                              EAT_CBOR_ARM_LABEL_ORIGINATION,
                              &service);
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to add the name of the profile definition document
 *
 * \param[in]  token_ctx  Token encoding context
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_add_profile_definition(struct attest_token_ctx *token_ctx)
{
    struct useful_buf_c profile;
    uint32_t size;

    profile.ptr = tfm_attest_hal_get_profile_definition(&size);

    if (profile.ptr) {
        profile.len = size;
        attest_token_add_tstr(token_ctx,
                              EAT_CBOR_ARM_LABEL_PROFILE_DEFINITION,
                              &profile);
    }

    return PSA_ATTEST_ERR_SUCCESS;
}

/*!
 * \brief Static function to verify the input challenge size
 *
 *  Only discrete sizes are accepted.
 *
 * \param[in] challenge_size  Size of challenge object in bytes.
 *
 * \retval  PSA_ATTEST_ERR_SUCCESS
 * \retval  PSA_ATTEST_ERR_INVALID_INPUT
 */
static enum psa_attest_err_t attest_verify_challenge_size(size_t challenge_size)
{
    switch (challenge_size) {
    /* Intentional fall through */
    case PSA_INITIAL_ATTEST_CHALLENGE_SIZE_32:
    case PSA_INITIAL_ATTEST_CHALLENGE_SIZE_48:
    case PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64:
    case (PSA_INITIAL_ATTEST_CHALLENGE_SIZE_32 + 4): /* Test purpose */
        return PSA_ATTEST_ERR_SUCCESS;
    }

    return PSA_ATTEST_ERR_INVALID_INPUT;
}

/*!
 * \brief Static function to create the initial attestation token
 *
 * \param[in]  challenge        Structure to carry the challenge value:
 *                              pointer + challeng's length
 * \param[in]  token            Structure to carry the token info, where to
 *                              create it: pointer + buffer's length
 * \param[out] completed_token  Structure to carry the info about the created
 *                              token: pointer + final token's length
 *
 * \return Returns error code as specified in \ref psa_attest_err_t
 */
static enum psa_attest_err_t
attest_create_token(struct useful_buf_c *challenge,
                    struct useful_buf   *token,
                    struct useful_buf_c *completed_token)
{
    enum psa_attest_err_t attest_err = PSA_ATTEST_ERR_SUCCESS;
    enum attest_token_err_t token_err;
    struct attest_token_ctx attest_token_ctx;
    int32_t key_select;
    uint32_t option_flags = 0;

    if (challenge->len == 36) {
        /* FixMe: Special challenge with option flags appended. This might can
         *        be removed when the public API can take option_flags.
         */
        option_flags = *(uint32_t *)((uint8_t*)challenge->ptr + 32);
        challenge->len = 32;
    }

    /* Lower three bits are the key select */
    key_select = option_flags & 0x7;

    /* Get started creating the token. This sets up the CBOR and COSE contexts
     * which causes the COSE headers to be constructed.
     */
    token_err = attest_token_start(&attest_token_ctx,
                                   option_flags,         /* option_flags */
                                   key_select,           /* key_select   */
                                   COSE_ALGORITHM_ES256, /* alg_select   */
                                   token);

    if (token_err != ATTEST_TOKEN_ERR_SUCCESS) {
        attest_err = error_mapping(token_err);
        goto error;
    }

    attest_err = attest_add_challenge_claim(&attest_token_ctx,
                                            challenge);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    if (!(option_flags & TOKEN_OPT_OMIT_CLAIMS)) {
        attest_err = attest_add_boot_seed_claim(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        attest_err = attest_add_verification_service(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        attest_err = attest_add_profile_definition(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        /* FixMe: Remove this #if when MPU will be configured properly.
         *        Currently in case of TFM_LVL == 3 unaligned access triggers
         *        a usage fault exception.
         */
#if !defined(TFM_LVL) || (TFM_LVL == 1)
        attest_err = attest_add_instance_id_claim(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        attest_err = attest_add_hw_version_claim(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        attest_err = attest_add_implementation_id_claim(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }
#endif

        attest_err = attest_add_caller_id_claim(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        attest_err = attest_add_security_lifecycle_claim(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }

        attest_err = attest_add_all_sw_components(&attest_token_ctx);
        if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
            goto error;
        }
    }

    /* Finish up creating the token. This is where the actual signature
     * is generated. This finishes up the CBOR encoding too.
     */
    token_err = attest_token_finish(&attest_token_ctx, completed_token);
    if (token_err) {
        attest_err = error_mapping(token_err);
        goto error;
    }

error:
    return attest_err;
}

/* Limitations of the current implementation:
 *  - Token is not signed yet properly, just a fake signature is added to the
 *    token due to lack of psa_asymmetric_sign() implementation in crypto
 *    service.
 */
enum psa_attest_err_t
initial_attest_get_token(const psa_invec  *in_vec,  uint32_t num_invec,
                               psa_outvec *out_vec, uint32_t num_outvec)
{
    enum psa_attest_err_t attest_err = PSA_ATTEST_ERR_SUCCESS;
    struct useful_buf_c challenge;
    struct useful_buf token;
    struct useful_buf_c completed_token;

    challenge.ptr = in_vec[0].base;
    challenge.len = in_vec[0].len;
    token.ptr = out_vec[0].base;
    token.len = out_vec[0].len;

    attest_err = attest_verify_challenge_size(challenge.len);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    attest_err = attest_check_memory_access((void *)challenge.ptr,
                                            challenge.len,
                                            TFM_ATTEST_ACCESS_RO);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    attest_err = attest_check_memory_access(token.ptr,
                                            token.len,
                                            TFM_ATTEST_ACCESS_RW);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    attest_err = attest_create_token(&challenge, &token, &completed_token);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    out_vec[0].base = (void *)completed_token.ptr;
    out_vec[0].len  = completed_token.len;

error:
    return attest_err;
}

/* Initial implementation, just returns with hard coded value */
enum psa_attest_err_t
initial_attest_get_token_size(const psa_invec  *in_vec,  uint32_t num_invec,
                                    psa_outvec *out_vec, uint32_t num_outvec)
{
    enum psa_attest_err_t attest_err = PSA_ATTEST_ERR_SUCCESS;
    uint32_t  challenge_size = *(uint32_t *)in_vec[0].base;
    uint32_t *token_buf_size = (uint32_t *)out_vec[0].base;
    struct useful_buf_c challenge;
    struct useful_buf token;
    struct useful_buf_c completed_token;

    /* Only the size of the challenge is needed */
    challenge.ptr = NULL;
    challenge.len = challenge_size;

    /* Special value to get the size of the token, but token is not created */
    token.ptr = NULL;
    token.len = INT32_MAX;

    if (out_vec[0].len < sizeof(uint32_t)) {
        attest_err = PSA_ATTEST_ERR_INVALID_INPUT;
        goto error;
    }

    attest_err = attest_verify_challenge_size(challenge_size);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    attest_err = attest_create_token(&challenge, &token, &completed_token);
    if (attest_err != PSA_ATTEST_ERR_SUCCESS) {
        goto error;
    }

    *token_buf_size = completed_token.len;

error:
    return attest_err;
}
