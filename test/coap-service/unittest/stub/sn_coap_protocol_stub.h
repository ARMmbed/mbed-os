
#ifndef __SN_COAP_PROTOCOL_STUB_H__
#define __SN_COAP_PROTOCOL_STUB_H__

#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"

typedef struct {
    int8_t expectedInt8;
    int16_t expectedInt16;
    struct coap_s *expectedCoap;
    sn_coap_hdr_s *expectedHeader;
    coap_send_msg_s *expectedSendMsg;
} sn_coap_protocol_stub_def;

extern sn_coap_protocol_stub_def sn_coap_protocol_stub;

#endif //__SN_COAP_PROTOCOL_STUB_H__
