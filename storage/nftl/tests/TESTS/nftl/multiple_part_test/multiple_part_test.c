#include <nftl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <nftl.h>
#include <inc/nftl_conf.h>
#include <inc/nftl_util.h>
#include <aos/hal/flash.h>
#include <aos/hal/nand_flash.h>

#define DEFAULT_BYTE_VALUE_IN_IMG 0x00
#define USER_BLOCK_SIZE NFTL_PAGE_SIZE

#define TEST_ROUNDS 100000
#define BLOCK_TOTAL (NFTL_TOTAL_RAW_BLOCK_NUMS * NFTL_PAGE_PER_BLOCK_NUMS)
#define CHANGES_MAX (BLOCK_TOTAL >> 2)

static uint8_t rands_in_block[BLOCK_TOTAL];

extern int32_t hal_flash_init();

int main(int argc, char *argv[])
{
    int round, idx, changes, change_idx;
    uint8_t value_to_change, block_buf[USER_BLOCK_SIZE];
    nftl_partition_t p;
    nftl_block_t b;
    int err;
    //unsigned int seed = time(NULL);
    //srand(seed);

    hal_flash_init();
    nftl_init();
    memset(rands_in_block, DEFAULT_BYTE_VALUE_IN_IMG, sizeof(rands_in_block));

#ifdef NFTL_YES_TRACE
    printf("Dump after nftl init:\n");
    nftl_dump(NULL);
#endif

    /* init rands_in_block with values read from flash. */
    for (idx = 0; idx < BLOCK_TOTAL; idx++) {
        memset(block_buf, 0, sizeof(change_idx));
        if (idx >= ((NFTL_PART0_RAW_BLOCK_NUMS +
            NFTL_PART1_RAW_BLOCK_NUMS) * NFTL_PAGE_PER_BLOCK_NUMS)) {
            p = NFTL_PARTITION2;
            b = idx - (NFTL_PART0_RAW_BLOCK_NUMS +
                NFTL_PART1_RAW_BLOCK_NUMS) * NFTL_PAGE_PER_BLOCK_NUMS;
        } else if (idx >= (NFTL_PART0_RAW_BLOCK_NUMS *
                   NFTL_PAGE_PER_BLOCK_NUMS)) {
            p = NFTL_PARTITION1;
            b = idx - NFTL_PART0_RAW_BLOCK_NUMS * NFTL_PAGE_PER_BLOCK_NUMS;
        } else {
            /* partition 0 */
            p = NFTL_PARTITION0;
            b = idx;
        }

        err = nftl_flash_read(p, b, 0, block_buf, sizeof(block_buf));
        /* read back check */
        if (err != NFTL_ERR_OK) {
            printf("%s %d Failed to read, idx: %d\r\n", __func__, __LINE__, idx);
            return -1;
        } else {
            rands_in_block[idx] = block_buf[0];
        }
    }

    for (round = 0; round < TEST_ROUNDS; round++) {
        changes = rand() % CHANGES_MAX;
        for (idx = 0; idx < changes; idx++) {
            value_to_change = (unsigned char)(rand() % 0xff);
            change_idx = rand() % BLOCK_TOTAL;
            if ((round % 3 == 0) && (change_idx % 2 == 0)) continue;
            if (change_idx >= ((NFTL_PART0_RAW_BLOCK_NUMS +
                NFTL_PART1_RAW_BLOCK_NUMS) * NFTL_PAGE_PER_BLOCK_NUMS)) {
                /* partition 2, TODO: used logical partition index */
                p = NFTL_PARTITION2;
                b = change_idx - (NFTL_PART0_RAW_BLOCK_NUMS +
                    NFTL_PART1_RAW_BLOCK_NUMS) * NFTL_PAGE_PER_BLOCK_NUMS;
            } else if (change_idx >= (NFTL_PART0_RAW_BLOCK_NUMS *
                       NFTL_PAGE_PER_BLOCK_NUMS)) {
                /* partition 1 */
                p = NFTL_PARTITION1;
                b = change_idx - NFTL_PART0_RAW_BLOCK_NUMS * NFTL_PAGE_PER_BLOCK_NUMS;
            } else {
                /* partition 0 */
                p = NFTL_PARTITION0;
                b = change_idx;
            }
            memset(block_buf, value_to_change, sizeof(block_buf));
            err = nftl_flash_erase(p, b);
            if (err == NFTL_ERR_OK) {
                err == nftl_flash_write(p, b, 0, block_buf, sizeof(block_buf));
                if (err == NFTL_ERR_OK) {
                    rands_in_block[change_idx] = value_to_change;
                    printf("value %02x programmed into partition %d "
                           "block %d\r\n", value_to_change, p, b);
                    memset(block_buf, change_idx, sizeof(change_idx));
                    err = nftl_flash_read(p, b, 0, block_buf, sizeof(block_buf));
                    /* read back check */
                    if (err == NFTL_ERR_OK) {
                        for (int j = 0; j < USER_BLOCK_SIZE; j++) {
                            assert(block_buf[j] == value_to_change);
                        }
                    } else {
                        printf("Failed to read back programmed value "
                               "partition %d block %d value: %02x\r\n",
                               p, b, value_to_change);
                    }
                } else {
                    printf("\033[33;33m value %02x failed into partition "
                           "%d block %d, err: %d \033[0m\r\n", 
                           value_to_change, p, b, err);
                }
            } else {
                printf("\033[33;33m Failed to erase partition %d "
                       "block %d, err: %d \033[0m\r\n", p, b, err);
            }

            usleep(rand() % 300000);
        }

        // do a round check
        int err_cnt = 0;
        for (idx = 0; idx < BLOCK_TOTAL; idx++) {
            if (idx >= ((NFTL_PART0_RAW_BLOCK_NUMS +
                NFTL_PART1_RAW_BLOCK_NUMS) * NFTL_PAGE_PER_BLOCK_NUMS)) {
                /* partition 2 */
                p = NFTL_PARTITION2;
                b = idx - (NFTL_PART0_RAW_BLOCK_NUMS +
                    NFTL_PART1_RAW_BLOCK_NUMS) * NFTL_PAGE_PER_BLOCK_NUMS;
            } else if (idx >= (NFTL_PART0_RAW_BLOCK_NUMS *
                       NFTL_PAGE_PER_BLOCK_NUMS)) {
                /* partition 1 */
                p = NFTL_PARTITION1;
                b = idx - NFTL_PART0_RAW_BLOCK_NUMS * NFTL_PAGE_PER_BLOCK_NUMS;
            } else {
                /* partition 0 */
                p = NFTL_PARTITION0;
                b = idx;
            }

            memset(block_buf, change_idx, sizeof(change_idx));
            err = nftl_flash_read(p, b, 0, block_buf, sizeof(block_buf));
            if (err == NFTL_ERR_OK) {
                    int j = 0;
                //for (int j = 0; j < USER_BLOCK_SIZE; j++) {
                    if (block_buf[j] != rands_in_block[idx]) {
                        printf("\033[41;33m Non match found: partition - %u "
                               "block - %u offset - %u \033[0m\r\n", p, b, j);
                        printf("The value in block is %02x, the expected value is "
                               "%02x\r\n", block_buf[j], rands_in_block[idx]);
                        err_cnt++;
                        break;
                        //return -1;
                    }
                //}
            } else {
                printf("\033[33;33m Failed to read partition %d "
                       "block %d, err: %d \033[0m\r\n", p, b, err);
            }
        }
        assert(err_cnt == 0);
    }

    return 0;
}
