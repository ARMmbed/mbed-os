#ifndef __MLE_STUB_H__
#define __MLE_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t expectedUint8;
    uint16_t expectedUint16;
    uint8_t *expectedUint8ptr;
    mle_neigh_table_entry_t *mle_neigh_table_entry_ptr;
} mle_stub_def;

extern mle_stub_def mle_stub;

#ifdef __cplusplus
}
#endif

#endif // __MLE_STUB_H__
