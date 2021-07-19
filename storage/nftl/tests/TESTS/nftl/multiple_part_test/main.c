#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <inc/nftl_conf.h>
#include <inc/nftl_util.h>
#include <inc/nftl.h>
#include <aos/hal/flash.h>
#include <aos/hal/nand_flash.h>

uint8_t     write_buf[4096];
uint8_t     read_buf[4096];
uint8_t     write_spare_buf[16];
uint8_t     read_spare_buf[16];

int main(int argc, char *argv[])
{
    uint32_t    block;
    uint32_t    i;
    uint32_t    j;
    uint32_t    w_off;
    uint32_t    r_off;
    uint32_t    total_blk;
    uint8_t     data_record[200];

    hal_flash_init();

    nftl_init();

    total_blk = 200;

    for (i = 0; i < 3; i++) {

        for (j = 0; j < 200; j++) {

            data_record[j] = i*64 + j;

            memset(write_buf, data_record[j], 4096);

            nftl_flash_write(NFTL_PARTITION0, j, 0, write_buf, 4096);
        }
    }

    printf("write finished\r\n");

    for (i = 0; i < 200; i++) {

        nftl_flash_read(NFTL_PARTITION0, i, 0, read_buf, 4096);

        if (read_buf[10] != data_record[i]) {
            printf("failed to check data\r\n");
        } else {
            printf("match: %d, %d\r\n", read_buf[10], data_record[i]);
        }
    }

    printf("test pass\r\n");

    return 0;
}

