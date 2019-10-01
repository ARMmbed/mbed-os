#ifndef _VAL_GREENTEA_H_
#define _VAL_GREENTEA_H_

#ifdef   __cplusplus
extern "C"
{
#endif

#include "val.h"
#include "val_interfaces.h"

void mbed_val_test_init(uint32_t test_num, char8_t *desc, uint32_t test_bitfield);
void mbed_val_test_exit(void);
val_status_t mbed_val_execute_non_secure_tests(uint32_t test_num, client_test_t *tests_list, bool_t server_hs);
val_status_t mbed_val_set_status(uint32_t status);
uint32_t mbed_val_get_status(void);
val_status_t mbed_val_execute_secure_test_func(psa_handle_t *handle, test_info_t test_info, uint32_t sid);
val_status_t mbed_val_print(print_verbosity_t verbosity, const char *string, uint32_t data);
val_status_t mbed_val_get_secure_test_result(psa_handle_t *handle);
val_status_t mbed_val_ipc_connect(uint32_t sid, uint32_t minor_version, psa_handle_t *handle);
val_status_t mbed_val_ipc_call(psa_handle_t handle, psa_invec *in_vec, size_t in_len,
                          psa_outvec *out_vec, size_t out_len);
void mbed_val_ipc_close(psa_handle_t handle);
val_status_t mbed_val_wd_reprogram_timer(wd_timeout_type_t timeout_type);

#ifdef   __cplusplus
}
#endif

#endif // _VAL_GREENTEA_H_
