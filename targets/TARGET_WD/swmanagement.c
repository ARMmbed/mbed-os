#include "swmanagement.h"
#include "blsrv.h"

static char m_null_termination;
static size_t m_download_offset;
static time_t m_download_timestamp;

static inline void m_set_valid_md(intptr_t mdroot, swmanagement_image_information *info)
{
    info->image_total_length = *((size_t *)(mdroot + WD_ABI_HDR_OFFSET_SIZE));
    info->image_application_name = (const char *)(mdroot + WD_ABI_APPHDR_OFFSET_NAME);
    info->image_application_version = (const char *)(mdroot + WD_ABI_APPHDR_OFFSET_VERSION);
}

static inline m_set_invalid_md(swmanagement_image_information *info)
{
    info->image_total_length = 0;
    info->image_application_name = &m_null_termination;
    info->image_application_version = &m_null_termination;
}

static inline void m_set_md(intptr_t mdroot, swmanagement_image_information *info)
{
    if (info->image_validation_result < 0) {
        m_set_invalid_md(info);
    } else {
        m_set_valid_md(mdroot, info);
    }
}

// linker-script imports
extern int __image_start;
extern int __update_image_start;

// we need to use the address of the symbols, because the C compiler dereferences the extern vars automatically
#define image_start_intptr ((intptr_t)(&__image_start))
#define update_image_start_intptr ((intptr_t)(&__update_image_start))

void swmanagement_get_status(swmanagement_status *status)
{
    struct blsrv_desc desc;

    //  get status from bl
    //////////////////////////////////////////////////////////////////////////
    desc.operation = blsrv_get_status;
    desc.args.get_status.force_dsa_validation = 0;
    blsrv_call(&desc);

    status->boot_image_information.image_validation_result = desc.args.get_status.application_image_status;
    status->update_image_information.image_validation_result = desc.args.get_status.update_image_status;
    status->update_status = desc.args.get_status.command_word;

    // extract metadata
    //////////////////////////////////////////////////////////////////////////
    m_set_md(image_start_intptr + WD_ABI_HDR_OFFSET, &status->boot_image_information);
    m_set_md(update_image_start_intptr + WD_ABI_HDR_OFFSET, &status->update_image_information);

}

void swmanagement_prepare_new_download(void)
{
    struct blsrv_desc desc;
    desc.operation = blsrv_erase_update_region;

    blsrv_call(&desc);

    m_download_offset = 0;
}

uint8_t swmanagement_is_prepared_for_download(void)
{

    swmanagement_status status;
    swmanagement_get_status(&status);
    if (m_download_offset == 0 &&
            status.update_status == Default &&
            status.update_image_information.image_validation_result == Empty) {
        return 1;
    }
    return 0;
}

void swmangement_append_download_data(const void *buffer, size_t buffer_size)
{
    intptr_t src = (intptr_t)buffer;

    struct blsrv_desc desc;
    desc.operation = blsrv_write_update_region;
    desc.args.write_update_region.src = src;
    desc.args.write_update_region.offset = m_download_offset;
    desc.args.write_update_region.size = buffer_size;

    blsrv_call(&desc);

    m_download_offset += buffer_size;

    if (rtc_isenabled()) {
        m_download_timestamp = rtc_read();
    } else {
        m_download_timestamp = 0;
    }

}

uint8_t swmanagement_is_in_download_state(void)
{
    if (rtc_isenabled()) {
        if (m_download_offset > 0 &&
                rtc_read() < (m_download_timestamp + 20)) { // window of 20 sec. for check
            return 1;
        }
    }
    return 0;
}

image_validation_result swmanagement_update_apply(void)
{
    struct blsrv_desc desc;
    desc.operation = blsrv_apply_update;
    blsrv_call(&desc);

    return desc.args.apply_update.validation_result;
}