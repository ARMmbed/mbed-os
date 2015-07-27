#ifndef USART_INTERRUPT_H_INCLUDED
#define USART_INTERRUPT_H_INCLUDED

#include "usart.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__DOXYGEN__)
void _usart_write_buffer(
    struct usart_module *const module,
    uint8_t *tx_data,
    uint16_t length);

void _usart_read_buffer(
    struct usart_module *const module,
    uint8_t *rx_data,
    uint16_t length);

void _usart_interrupt_handler(
    uint8_t instance);
#endif

/**
 * \addtogroup asfdoc_sam0_sercom_usart_group
 *
 * @{
 */

/**
 * \name Callback Management
 * @{
 */
void usart_register_callback(
    struct usart_module *const module,
    usart_callback_t callback_func,
    enum usart_callback callback_type);

void usart_unregister_callback(
    struct usart_module *module,
    enum usart_callback callback_type);

/**
 * \brief Enables callback
 *
 * Enables the callback function registered by the \ref usart_register_callback.
 * The callback function will be called from the interrupt handler when the
 * conditions for the callback type are met.
 *
 * \param[in]  module         Pointer to USART software instance struct
 * \param[in]  callback_type  Callback type given by an enum
 */
static inline void usart_enable_callback(
    struct usart_module *const module,
    enum usart_callback callback_type)
{
    /* Sanity check arguments */
    Assert(module);

    /* Enable callback */
    module->callback_enable_mask |= (1 << callback_type);

}

/**
 * \brief Disable callback
 *
 * Disables the callback function registered by the \ref usart_register_callback,
 * and the callback will not be called from the interrupt routine.
 *
 * \param[in]  module         Pointer to USART software instance struct
 * \param[in]  callback_type  Callback type given by an enum
 */
static inline void usart_disable_callback(
    struct usart_module *const module,
    enum usart_callback callback_type)
{
    /* Sanity check arguments */
    Assert(module);

    /* Disable callback */
    module->callback_enable_mask &= ~(1 << callback_type);
}

/**
 * @}
 */

/**
 * \name Writing and Reading
 * @{
 */
enum status_code usart_write_job(
    struct usart_module *const module,
    const uint16_t *tx_data);

enum status_code usart_read_job(
    struct usart_module *const module,
    uint16_t *const rx_data);

enum status_code usart_write_buffer_job(
    struct usart_module *const module,
    uint8_t *tx_data,
    uint16_t length);

enum status_code usart_read_buffer_job(
    struct usart_module *const module,
    uint8_t *rx_data,
    uint16_t length);

void usart_abort_job(
    struct usart_module *const module,
    enum usart_transceiver_type transceiver_type);

enum status_code usart_get_job_status(
    struct usart_module *const module,
    enum usart_transceiver_type transceiver_type);
/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* USART_INTERRUPT_H_INCLUDED */
