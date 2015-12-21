/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "string.h"
#include "eventOS_event.h"
#include "ccmLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "shalib.h"
#include "randLIB.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana_nvm.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "common_functions.h"

sec_suite_t *sec_suite_tls_session_get_by_socket_address(sockaddr_t *add, uint8_t request_by_server, int8_t socket_id)
{
    return NULL;
}

int8_t sec_suite_set_chipher_suite_list(int8_t socket_id, uint8_t security_suites)
{
    return -1;
}
