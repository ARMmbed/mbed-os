#ifndef EXTINT_CALLBACK_H_INCLUDED
#define EXTINT_CALLBACK_H_INCLUDED

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup asfdoc_sam0_extint_group
 *
 * @{
 */

/** \name Callback Configuration and Initialization
 * @{
 */

/** Enum for the possible callback types for the EXTINT module. */
enum extint_callback_type {
    /** Callback type for when an external interrupt detects the configured
     *  channel criteria (i.e. edge or level detection)
     */
    EXTINT_CALLBACK_TYPE_DETECT,
};

enum status_code extint_register_callback(
    const extint_callback_t callback,
    const uint8_t channel,
    const enum extint_callback_type type);

enum status_code extint_unregister_callback(
    const extint_callback_t callback,
    const uint8_t channel,
    const enum extint_callback_type type);

uint8_t extint_get_current_channel(void);

/** @} */

/** \name Callback Enabling and Disabling (Channel)
 * @{
 */

enum status_code extint_chan_enable_callback(
    const uint8_t channel,
    const enum extint_callback_type type);

enum status_code extint_chan_disable_callback(
    const uint8_t channel,
    const enum extint_callback_type type);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
