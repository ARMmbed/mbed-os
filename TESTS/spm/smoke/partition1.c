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
#define SERVER_RESPONSE_TEXT            "Response1"
#define WRITE_OFFSET                    5
#define ACTUAL_MSG_SIZE                 22

// ---------------------------------- Global Variables -------------------------------

const char SERVER_EXPECTED_READ_MSG[] = "Hello and welcome SPM";

// ------------------------------ Partition's Main Thread ----------------------------

void part1_main(void *ptr)
{
    uint32_t  signals   = 0;
    int32_t   client_id = 0;
    psa_msg_t msg       = {0};

    char read_msg_buf[SERVER_READ_MSG_BUF_SIZE] = {0};
    const char write_msg_buf[SERVER_RSP_BUF_SIZE] = SERVER_RESPONSE_TEXT;


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
                MBED_ASSERT(msg.size == ACTUAL_MSG_SIZE);
                MBED_ASSERT(msg.response_size == 20);

                uint32_t offset = 0;
                uint32_t bytes_to_read = msg.size - offset;
                size_t bytes_read = psa_read(msg.handle, offset, read_msg_buf, bytes_to_read);
                MBED_ASSERT(bytes_read == bytes_to_read);

                int cmp_res = strcmp(SERVER_EXPECTED_READ_MSG + offset, read_msg_buf);
                if(cmp_res != 0) {
                    error("psa_read() - Bad reading with offset %u!!", (unsigned int)offset);
                }

                offset = 10;
                bytes_to_read = SERVER_READ_MSG_BUF_SIZE;
                bytes_read = psa_read(msg.handle, offset, read_msg_buf, bytes_to_read);
                MBED_ASSERT(bytes_read == msg.size - offset);

                cmp_res = strcmp(SERVER_EXPECTED_READ_MSG + offset, read_msg_buf);
                if(cmp_res != 0) {
                    error("psa_read() - Bad reading with offset %u!!", offset);
                }

                offset = ACTUAL_MSG_SIZE;
                bytes_to_read = 5;
                bytes_read = psa_read(msg.handle, offset, read_msg_buf, bytes_to_read);
                MBED_ASSERT(bytes_read == 0);
                PSA_UNUSED(bytes_read);

                psa_write(msg.handle, WRITE_OFFSET, write_msg_buf, strlen(write_msg_buf) + 1);

                break;
            }

            case PSA_IPC_MSG_TYPE_CONNECT:
            case PSA_IPC_MSG_TYPE_DISCONNECT:
            {
                MBED_ASSERT(msg.size == 0);
                MBED_ASSERT(msg.response_size == 0);
                break;
            }
        }

        psa_end(msg.handle, PSA_SUCCESS);
    }

    return;
}
