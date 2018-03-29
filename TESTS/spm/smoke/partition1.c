// -------------------------------------- Includes -----------------------------------

#include <string.h>

#include "cmsis_os2.h"
#include "spm_server.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "psa_part1_partition.h"

// ------------------------------------ Definitions ----------------------------------

#define SERVER_READ_MSG_BUF_SIZE        30
#define SERVER_RSP_BUF_SIZE             20
#define WRITE_OFFSET                    5
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
        MBED_ASSERT(signals == SF1_MSK);
        PSA_UNUSED(signals);

        osDelay(500);

        psa_get(SF1_MSK, &msg);
        client_id = psa_identity(msg.handle);
        MBED_ASSERT(client_id == PSA_NSPE_IDENTIFIER);
        PSA_UNUSED(client_id);

        switch (msg.type) {

            case PSA_IPC_MSG_TYPE_CALL:
            {

                MBED_ASSERT((msg.size[0] + msg.size[1] + msg.size[2]) == ACTUAL_MSG_SIZE);
                MBED_ASSERT(msg.response_size == SERVER_RSP_BUF_SIZE);
                uint32_t bytes_read = 0;
                char *read_msg_buf = malloc(sizeof(char) * SERVER_READ_MSG_BUF_SIZE);
                memset(read_msg_buf, 0, SERVER_READ_MSG_BUF_SIZE);
                char *read_ptr = read_msg_buf;

                for (size_t i = 0; i < PSA_MAX_INVEC_LEN; i++) {

                    bytes_read += psa_read(msg.handle, i, read_ptr, msg.size[i]);
                    read_ptr = read_msg_buf + bytes_read;
                }

                MBED_ASSERT(bytes_read == (msg.size[0] + msg.size[1] + msg.size[2]));

                int cmp_res = strcmp(SERVER_EXPECTED_READ_MSG, read_msg_buf);
                if(cmp_res != 0) {
                    error("psa_read() - Bad reading!!");
                }

                psa_write(msg.handle, WRITE_OFFSET, WRITE_MSG_BUF, strlen(WRITE_MSG_BUF) + 1);
                free(read_msg_buf);
                read_msg_buf = NULL;
                read_ptr = NULL;

                break;
            }

            case PSA_IPC_MSG_TYPE_CONNECT:
            case PSA_IPC_MSG_TYPE_DISCONNECT:
            {
                MBED_ASSERT(msg.response_size == 0);
                break;
            }
        }

        psa_end(msg.handle, PSA_SUCCESS);
    }

    return;
}
