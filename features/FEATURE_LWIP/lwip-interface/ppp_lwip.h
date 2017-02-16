/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
 */

#ifndef PPP_LWIP_H_
#define PPP_LWIP_H_

#include "FileHandle.h"
#include "NetworkStack.h"

namespace mbed {

void ppp_bringup_test();

NetworkStack *mbed_ppp_get_stack();
nsapi_error_t mbed_ppp_init(FileHandle *stream);

}

#endif /* PPP_LWIP_H_ */
