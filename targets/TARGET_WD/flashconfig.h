/*
 * flashconfig.h
 *
 * Created: 24.08.2017 20:20:02
 *  Author: Guenter.Prossliner
 */


#ifndef FLASHCONFIG_H_
#define FLASHCONFIG_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    flashconfig_success,
    flashconfig_args_error,
    flashconfig_not_found,
    flashconfig_overrun
} flashconfig_result;

flashconfig_result flashconfig_get_value(const char *name, const char **value);

flashconfig_result flashconfig_set_value(const char *name, char *value);

#define FLASHCONFIG_NAME_MAX    32
#define FLASHCONFIG_VALUE_MAX   200

#ifdef __cplusplus
}
#endif


#endif /* FLASHCONFIG_H_ */