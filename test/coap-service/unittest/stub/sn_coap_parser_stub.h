
#ifndef __SN_COAP_PARSER_STUB_H__
#define __SN_COAP_PARSER_STUB_H__

#include "sn_coap_header.h"

typedef struct {
    sn_coap_hdr_s *expectedHeader;
} sn_coap_parser_def;

extern sn_coap_parser_def sn_coap_parser_stub;

#endif //__SN_COAP_PARSER_STUB_H__
