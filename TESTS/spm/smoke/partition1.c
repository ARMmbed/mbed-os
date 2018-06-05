// -------------------------------------- Includes -----------------------------------

#include <string.h>

#include "cmsis_os2.h"
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_smoke_test_part1_partition.h"

// ------------------------------------ Definitions ----------------------------------

#define SERVER_READ_MSG_BUF_SIZE        30
#define SERVER_RSP_BUF_SIZE             20
#define ACTUAL_MSG_SIZE                 22

// ---------------------------------- Global Variables -------------------------------

const char SERVER_EXPECTED_READ_MSG[] = "Hello and welcome SPM";
const char WRITE_MSG_BUF[] = "Response1";

// ------------------------------ Partition's Main Thread ----------------------------

void part1_main(void *ptr)
{
    uint32_t  signals   = 0;
    int32_t   client_id = 0;
    psa_msg_t msg       = {0};

    while (1) {

        signals = psa_wait_any(PSA_WAIT_BLOCK);
        SPM_ASSERT(signals == SF1_MSK);
        PSA_UNUSED(signals);

        osDelay(500);

        psa_get(SF1_MSK, &msg);
        if (msg.handle != PSA_NULL_HANDLE) {
            client_id = psa_identity(msg.handle);
            SPM_ASSERT(client_id == PSA_NSPE_IDENTIFIER);
            PSA_UNUSED(client_id);
        }

        switch (msg.type) {
            case PSA_IPC_MSG_TYPE_CALL:
            {
                SPM_ASSERT((msg.in_size[0] + msg.in_size[1] + msg.in_size[2]) == ACTUAL_MSG_SIZE);
                SPM_ASSERT(msg.out_size[0] == SERVER_RSP_BUF_SIZE);
                uint32_t bytes_read = 0;
                char *read_msg_buf = malloc(sizeof(char) * SERVER_READ_MSG_BUF_SIZE);
                if (NULL == read_msg_buf) {
                    SPM_PANIC("Failed to allocate Memory");
                }
                memset(read_msg_buf, 0, SERVER_READ_MSG_BUF_SIZE);
                char *read_ptr = read_msg_buf;

                for (size_t i = 0; i < PSA_MAX_INVEC_LEN; i++) {

                    bytes_read += psa_read(msg.handle, i, read_ptr, msg.in_size[i]);
                    read_ptr = read_msg_buf + bytes_read;
                }

                SPM_ASSERT(bytes_read == (msg.in_size[0] + msg.in_size[1] + msg.in_size[2]));

                int cmp_res = strcmp(SERVER_EXPECTED_READ_MSG, read_msg_buf);
                if(cmp_res != 0) {
                    SPM_PANIC("psa_read() - Bad reading!!");
                }

                psa_write(msg.handle, 0, WRITE_MSG_BUF, strlen(WRITE_MSG_BUF) + 1);
                free(read_msg_buf);
                read_msg_buf = NULL;
                read_ptr = NULL;
                break;
            }
            case PSA_IPC_MSG_TYPE_DISCONNECT:
                SPM_ASSERT(msg.handle == PSA_NULL_HANDLE);
                // Fallthrough
            case PSA_IPC_MSG_TYPE_CONNECT:
            {
                SPM_ASSERT(msg.out_size[0] == 0);
                SPM_ASSERT(msg.out_size[1] == 0);
                SPM_ASSERT(msg.out_size[2] == 0);

                break;
            }
            default:
                SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
                break;
        }

        psa_end(msg.handle, PSA_SUCCESS);
    }

    return;
}
