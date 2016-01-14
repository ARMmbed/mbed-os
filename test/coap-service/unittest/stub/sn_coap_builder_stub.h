
#ifndef __SN_COAP_BUILDER_STUB_H__
#define __SN_COAP_BUILDER_STUB_H__

typedef struct {
    int16_t expectedInt16;
    uint16_t expectedUint16;
    sn_coap_hdr_s *expectedHeader;
} sn_coap_builder_stub_def;

extern sn_coap_builder_stub_def sn_coap_builder_stub;

#endif //__SN_COAP_BUILDER_STUB_H__
