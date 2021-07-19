#if 0
#include "inc/nftl_conf.h"
#include "inc/nftl_partition.h"

const nftl_partition_info_t nftl_partition_table[] = {
    [NFTL_PARTITION0] =
    {
        .desc   = "NFTL0",
        .blkoff = 0,
        .blkcnt = NFTL_PART0_BLOCK_NUMS,
    },
    [NFTL_PARTITION1] =
    {
        .desc   = "NFTL1",
        .blkoff = NFTL_PART0_BLOCK_NUMS,
        .blkcnt = NFTL_PART1_BLOCK_NUMS,
    },
    [NFTL_PARTITION2] =
    {
        .desc   = "NFTL2",
        .blkoff = NFTL_PART0_BLOCK_NUMS + NFTL_PART1_BLOCK_NUMS,
        .blkcnt = NFTL_PART2_BLOCK_NUMS,
    },
};
#endif
