#ifndef __COAP_CONNECTION_HANDLER_STUB_H__
#define __COAP_CONNECTION_HANDLER_STUB_H__

#include <inttypes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "coap_connection_handler.h"

typedef struct {
    int int_value;
    bool bool_value;
    thread_conn_handler_t *handler_obj;

    int (*send_to_sock_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, const unsigned char *, int);
    int (*receive_from_sock_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, unsigned char *, int);
    int (*get_passwd_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t *pw_ptr, uint8_t *pw_len);
    void (*sec_done_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static 40]);

} thread_conn_handler_stub_def;

extern thread_conn_handler_stub_def thread_conn_handler_stub;

#ifdef __cplusplus
}
#endif


#endif
