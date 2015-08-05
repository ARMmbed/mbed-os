#ifndef SERCOM_INTERRUPT_H_INCLUDED
#define SERCOM_INTERRUPT_H_INCLUDED

#include "sercom.h"
#include <system_interrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Look-up table for device instances. */
extern void *_sercom_instances[SERCOM_INST_NUM];

typedef void (*sercom_handler_t)(uint8_t instance);

enum system_interrupt_vector _sercom_get_interrupt_vector(
    Sercom *const sercom_instance);

void _sercom_set_handler(
    const uint8_t instance,
    const sercom_handler_t interrupt_handler);

#ifdef __cplusplus
}
#endif

#endif /* SERCOM_INTERRUPT_H_INCLUDED */
