#ifndef SWMANAGEMENT_H_
#define SWMANAGEMENT_H_

#include <stdint.h>
#include <stddef.h>
#include "mc_res.h"
#include "rtc_api.h"

#ifdef __cplusplus
extern "C" {
#endif

///< Specifies the result of the validation procedure
///< Implementation Note: These values correspond to values hardcoded in the bootloader, so don't alter them!
typedef enum {

    ///< The is no abi header, maybe area is empty
    Empty = -2,

    ///< There is an abi header, but for an incompatible image
    Incompatible = -1,

    ///< The image is compatible, but for development only
    DevelopmentImage = 0,

    ///< The image is CRC validated
    CRCValidatedImage = 1,

    ///< The image is DSA validated
    DSAValidatedImage = 2


} image_validation_result;

typedef struct {

    ///< Returns the total length of the image, as in ABI header
    size_t image_total_length;

    ///< returns a pointer to the name of the downloaded image.
    ///< only available if at least the first 1k of the image, which contains the metadata section, has been downloaded.
    const char *image_application_name;

    ///< returns a pointer to the version string of the downloaded image.
    ///< only available if at least the first 1k of the image, which contains the metadata section, has been downloaded.
    const char *image_application_version;

    ///< returns validation result. This is a not-cached or persistent value.
    image_validation_result image_validation_result;

} swmanagement_image_information;

typedef enum {

    Default = 0xFFFFFFFF,
    UpdatePending = 0xFFFFFF00,
    UpdateSuccessful = 0x00000000

} swmanagement_update_status;

typedef struct {

    swmanagement_image_information boot_image_information;

    swmanagement_image_information update_image_information;

    swmanagement_update_status update_status;

} swmanagement_status;


/**
* @brief
*   Returns the sw-status by scanning flash
*/
void swmanagement_get_status(swmanagement_status *status);

/**
* @brief
*   Prepares a new image to be downloaded (maybe to drop a partial invalid one)
*   The internal stream position (for swmangement_update_append) is set to zero.
*/
void swmanagement_prepare_new_download(void);

/**
* @brief
*   Returns preparation state for new download (prepared / not prepared).
*/
uint8_t swmanagement_is_prepared_for_download(void);

/**
* @brief
*   Appends raw data to the download area.
*/
void swmangement_append_download_data(const void *buffer, size_t buffer_size);

/**
* @brief
*   Returns state of download process (downloading / not downloading).
*   If RTC is not enabled, download state returns false.
*/
uint8_t swmanagement_is_in_download_state(void);

/**
* @brief
*   Finally applies the update. Validation is performed by this method!
*   The image will only be applied if the status is valid.
*
*/
image_validation_result swmanagement_update_apply(void);

#ifdef __cplusplus
}
#endif


#endif  // SWMANAGEMENT_H_
