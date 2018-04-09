#include <string.h>
#include "cmsis_os2.h"
#include "unity.h"
#include "spm_panic.h"
#include "spm_server.h"
#include "spm_client.h"
#include "spm_panic.h"
#include "psa_part2_partition.h"
#include "psa_client_common.h"

#define MINOR_VER 5

void server_main2(void *ptr)
{
    uint32_t  signals   = 0;
    psa_msg_t msg       = {0};

    while (1) {
        signals = psa_wait_any(PSA_WAIT_BLOCK);
        if (signals & DUMMY_MSK) {
            psa_get(DUMMY_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CALL:
                case PSA_IPC_MSG_TYPE_CONNECT:
                case PSA_IPC_MSG_TYPE_DISCONNECT:
                    psa_end(msg.handle, PSA_SUCCESS);
                    break;
                default:
                SPM_PANIC("Invalid msg type");
            }
        }
        else if (signals & PART2_CALL_INSIDE_PARTITION_MSK) {
            psa_get(PART2_CALL_INSIDE_PARTITION_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    psa_end(msg.handle, PSA_SUCCESS);
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    psa_connect(DUMMY, MINOR_VER);
                    TEST_FAIL_MESSAGE("server_call_sfid_in_same_partition negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_call_sfid_in_same_partition msg type failure");
                }
            }
        } else {
            SPM_ASSERT(0);
        }
    }
}
