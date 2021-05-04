/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef PSA_UPDATE_H
#define PSA_UPDATE_H

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"
#include "tfm_fwu_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PSA_FWU_MAX_BLOCK_SIZE               1024
#define TFM_FWU_INVALID_IMAGE_ID             0

/* The maximum size of an image digest in bytes. This is dependent
 * on the hash algorithm used.
 */
#define PSA_FWU_MAX_DIGEST_SIZE              32
#define TFM_IMAGE_INFO_INVALID_DIGEST        0xFF

/* The image ID macros. */
#define FWU_IMAGE_ID_SLOT_POSITION           0U

/* The area where the image is running. */
#define FWU_IMAGE_ID_SLOT_ACTIVE             0x01U

/* The area to stage the image. */
#define FWU_IMAGE_ID_SLOT_STAGE              0x02U
#define FWU_IMAGE_ID_SLOT_MASK               0x00FF

#define FWU_IMAGE_ID_TYPE_POSITION           8U
#define FWU_IMAGE_ID_SPECIFIC_ID_POSITION    16U

#define FWU_CALCULATE_IMAGE_ID(slot, type, specific_id)             \
                             ((slot & FWU_IMAGE_ID_SLOT_MASK) |     \
                             (type << FWU_IMAGE_ID_TYPE_POSITION) | \
                             (specific_id << FWU_IMAGE_ID_SPECIFIC_ID_POSITION))
#define FWU_IMAGE_ID_GET_TYPE(image_id) (image_id >> FWU_IMAGE_ID_TYPE_POSITION)
#define FWU_IMAGE_ID_GET_SLOT(image_id) (image_id & FWU_IMAGE_ID_SLOT_MASK)

/* Image state macros. */
#define PSA_IMAGE_UNDEFINED            0
#define PSA_IMAGE_CANDIDATE            1
#define PSA_IMAGE_INSTALLED            2
#define PSA_IMAGE_REJECTED             3
#define PSA_IMAGE_PENDING_INSTALL      4
#define PSA_IMAGE_REBOOT_NEEDED        5

/*
 * image_id[7:0]: slot.
 * image_id[15:8]: image type.
 * image_id[31:16]: specific image ID.
 */
typedef uint32_t psa_image_id_t;

typedef struct tfm_image_version_s {
    uint8_t iv_major;
    uint8_t iv_minor;
    uint16_t iv_revision;
    uint32_t iv_build_num;
} psa_image_version_t;

typedef struct tfm_image_info_s {
    psa_image_version_t version;
    uint8_t state;
    uint8_t digest[PSA_FWU_MAX_DIGEST_SIZE];
} psa_image_info_t;

typedef struct psa_hash_s {
    uint8_t value[PSA_FWU_MAX_DIGEST_SIZE];
} psa_hash_t;

/**
 * \brief Writes an image to its staging area.
 *
 * Writes the image data 'block' with length 'block_size' to its staging area.
 * If the image size is less than or equal to PSA_FWU_MAX_BLOCK_SIZE, the
 * caller can send the entire image in one call.
 * If the image size is greater than PSA_FWU_MAX_BLOCK_SIZE, the caller must
 * send parts of the image by calling this API multiple times with different
 * data blocks.
 *
 * \param[in] image_id        The identifier of the image
 * \param[in] block_offset    The offset of the block being passed into block,
 *                            in bytes
 * \param[in] block           A buffer containing a block of image data. This
 *                            might be a complete image or a subset.
 * \param[in] block_size      Size of block. The size must not be greater than
 *                            PSA_FWU_MAX_BLOCK_SIZE.
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     The data in block has been
 *                                         successfully stored.
 * \retval PSA_ERROR_INVALID_ARGUMENT      One of the following error
 *                                         conditions occurred:
 *                                         The parameter size is greater than
 *                                         PSA_FWU_MAX_BLOCK_SIZE;
 *                                         The parameter size is 0;
 *                                         The combination of offset and size
 *                                         is out of bounds.
 *
 * \retval PSA_ERROR_INSUFFICIENT_MEMORY   There is not enough memory to
 *                                         process the update
 * \retval PSA_ERROR_INSUFFICIENT_STORAGE  There is not enough storage to
 *                                         process the update
 * \retval PSA_ERROR_GENERAL_ERROR         A fatal error occurred
 * \retval PSA_ERROR_CURRENTLY_INSTALLING        The image is currently locked for
 *                                         writing.
 */
psa_status_t psa_fwu_write(psa_image_id_t image_id,
                           size_t block_offset,
                           const void *block,
                           size_t block_size);


/**
 * \brief Starts the installation of an image.
 *
 * The authenticity and integrity of the image is checked during installation.
 * If a reboot is required to complete installation then the implementation
 * can choose to defer the authenticity checks to that point.
 *
 * \param[in] image_id            The identifier of the image to install
 * \param[out] dependency_uuid    If PSA_ERROR_DEPENDENCY_NEEDED is returned,
 *                                this parameter is filled with dependency
 *                                information
 * \param[out] dependency_version If PSA_ERROR_DEPENDENCY_NEEDED is returned,
 *                                this parameter is filled with the minimum
 *                                required version for the dependency
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     The image was successfully
 *                                         installed. The platform does not
 *                                         require a reboot.
 * \retval PSA_SUCCESS_REBOOT              A system reboot is needed to finish
 *                                         installation.
 * \retval PSA_ERROR_INVALID_ARGUMENT      Bad input parameter
 * \retval PSA_ERROR_INVALID_SIGNATURE     The signature is incorrect
 * \retval PSA_ERROR_GENERAL_ERROR         A fatal error occurred
 * \retval PSA_ERROR_DEPENDENCY_NEEDED     A different image requires
 *                                         installation first
 * \retval PSA_ERROR_STORAGE_FAILURE       Some persistent storage could not be
 *                                         read or written by the
 *                                         implementation
 */
psa_status_t psa_fwu_install(psa_image_id_t image_id,
                             psa_image_id_t *dependency_uuid,
                             psa_image_version_t *dependency_version);

/**
 * \brief Aborts an ongoing installation and erases the staging area of the
 *        image.
 *
 * \param[in] image_id      The identifier of the image to abort installation
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                    Installation of the provided image_id
 *                                        has been aborted
 * \retval PSA_ERROR_INVALID_ARGUMENT     No image with the provided image_id
 *                                        is currently being installed
 * \retval PSA_ERROR_NOT_PERMITTED        The caller is not authorized to
 *                                        abort an installation
 */
psa_status_t psa_fwu_abort(psa_image_id_t image_id);

/**
 * \brief Returns information for an image of a particular image_id.
 *
 * \param[in] image_id         The image_id of the image to query
 *
 * \param[out] info            Output parameter for image information
 *                             related to the image_id
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     Image information has been returned
 * \retval PSA_ERROR_NOT_PERMITTED         The caller is not authorized to
 *                                         access platform version information
 */
psa_status_t psa_fwu_query(psa_image_id_t image_id,
                           psa_image_info_t *info);

/**
 * \brief Requests the platform to reboot. On success, the platform initiates
 *        a reboot, and might not return to the caller.
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                   The platform will reboot soon
 * \retval PSA_ERROR_NOT_PERMITTED       The caller is not authorized to
 *                                       reboot the platform
 */
psa_status_t psa_fwu_request_reboot(void);

/**
 * \brief Indicates to the implementation that the upgrade was successful.
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                  The image and its dependencies have
 *                                      transitioned into a PSA_IMAGE_INSTALLED
 *                                      state
 * \retval PSA_ERROR_NOT_SUPPORTED      The implementation does not support a
 *                                      PSA_IMAGE_PENDING_INSTALL state
 * \retval PSA_ERROR_NOT_PERMITTED      The caller is not permitted to make
 *                                      this call
 */
psa_status_t psa_fwu_accept(void);

/**
 * \brief Stores a manifest object and associates it with a particular image ID.
 *
 * \param[in] image_id            The identifier of the image
 *
 * \param[in] manifest            A pointer to a buffer containing a manifest
 *                                object
 *
 * \param[in] manifest_size       The size of the manifest parameter
 *
 * \param[in] manifest_dependency Output parameter containing the hash of a
 *                                required manifest when
 *                                PSA_ERROR_DEPENDENCY_NEEDED is returned
 *
 * \return A status indicating the success/failure of the operation
 *
 * \retval PSA_SUCCESS                     The manifest is persisted
 * \retval PSA_ERROR_NOT_PERMITTED         The manifest is too old to be
 *                                         installed
 * \retval PSA_ERROR_WRONG_DEVICE          The manifest is not intended for this
 *                                         device
 * \retval PSA_ERROR_INVALID_SIGNATURE     The manifest signature is not valid
 * \retval PSA_ERROR_DEPENDENCY_NEEDED     A different manifest is needed
 * \retval PSA_ERROR_INVALID_ARGUMENT      Parameter size is 0 or a pointer
 *                                         parameter is NULL
 * \retval PSA_ERROR_COMMUNICATION_FAILURE The system could not communicate with
 *                                         the installer
 * \retval PSA_ERROR_NOT_SUPPORTED         This function is not implemented
 * \retval PSA_ERROR_CURRENTLY_INSTALLING  An existing manifest for image ID is
 *                                         currently being installed and is
 *                                         locked from writing
 * \retval PSA_ERROR_GENERIC_ERROR         A fatal error occurred
 */
psa_status_t psa_fwu_set_manifest(psa_image_id_t image_id,
                                  const void *manifest,
                                  size_t manifest_size,
                                  psa_hash_t *manifest_dependency);


#ifdef __cplusplus
}
#endif
#endif /* PSA_UPDATE_H */
