#ifndef CMD_SOCKET_H_
#define CMD_SOCKET_H_

#include "nsapi_types.h"

int handle_nsapi_error(const char *function, nsapi_error_t ret);
int handle_nsapi_size_or_error(const char *function, nsapi_size_or_error_t ret);
void cmd_socket_init(void);

#endif
