/*
 * bootloader.h
 *
 * Created: 17.08.2017 17:25:55
 *  Author: Guenter.Prossliner
 */


#ifndef BLSRV_H_
#define BLSRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "WD_ABI.h"
#include "assert.h"

#define blsrv_erase_update_region       0x01
#define blsrv_write_update_region       0x02
#define blsrv_get_status                0x03
#define blsrv_apply_update              0x04
#define blsrv_write_config_data         0x05

struct blsrv_desc {

    // operation is a selector for the following fields
    int operation;
    union {

        struct {
            size_t offset;
            intptr_t src;
            size_t size;
        } write_update_region;

        struct {
            int force_dsa_validation;       // bitmask: 1:validate application image, 2: validate update image
            // output fields
            int bootloader_image_status;    // the image verification status
            int application_image_status;   // the image verification status
            int update_image_status;        // the image verification status
            int command_word;               // the word following the image
        } get_status;

        struct {
            int validation_result;  // output field, will be set by the service but only if != 0 , otherwise the call will not return but reset and apply!
        } apply_update;

        struct {
            intptr_t address;   // address within the config section
            intptr_t buffer;    // data to write
            size_t buffer_size;
        } write_config_data;

    } args;
};

static inline int blsrv_call(struct blsrv_desc *descriptor)
{
    int retcode;

    // validate the bootloader magic
    uint32_t bl_abihdr = *((uint32_t *)(WD_FLASH_BASE + WD_ABI_HDR_OFFSET));
    bl_abihdr &= 0xFFFFFF00;    // clear flags
    assert(bl_abihdr == (WD_ABI_HDR_MAGIC << 16 | WD_ABI_HDR_BL << 8)); // assert bootloader header

    typedef int (*ct)(void *);
    void **vectable = (void **)(WD_FLASH_BASE + WD_ABI_HDR_OFFSET + WD_ABI_BLHDR_OFFSET_SRVCALL);
    void *fnptr = *vectable;
    return ((ct)fnptr)(descriptor);
}

#ifdef __cplusplus
}
#endif


#endif /* BLSRV_H_ */