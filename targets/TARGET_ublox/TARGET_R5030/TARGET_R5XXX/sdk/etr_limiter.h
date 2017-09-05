#ifndef INCLUDED_ETR_LIMITER
#define INCLUDED_ETR_LIMITER
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define ETR_LIMITER_PERIPHERALID4 0x07
#define ETR_LIMITER_PERIPHERALID5 0x00
#define ETR_LIMITER_PERIPHERALID6 0x00
#define ETR_LIMITER_PERIPHERALID7 0x00
#define ETR_LIMITER_PERIPHERALID0 0x88
#define ETR_LIMITER_PERIPHERALID1 0xDA
#define ETR_LIMITER_PERIPHERALID2 0x08
#define ETR_LIMITER_PERIPHERALID3 0x00
#define ETR_LIMITER_COMPONENTID0 0x0D
#define ETR_LIMITER_COMPONENTID1 0x90
#define ETR_LIMITER_COMPONENTID2 0x05
#define ETR_LIMITER_COMPONENTID3 0xB1

/** ETR Limiter
*/
struct etr_limiter_s {
   /** Control Register at address offset 0x000, read-write */
   uint32_t ctrl;
   /** Read Pointer. Value must be a byte address. Bottom 2 bits are forced zero. at address offset 0x004, read-write */
   uint32_t read_pointer;
   /** Write Pointer. Value must be a byte address. Bottom 2 bits are forced zero. at address offset 0x008, read-write */
   uint32_t write_pointer;
   /** Buffer depth. The value must be a power of 2. at address offset 0x00C, read-write */
   uint32_t bufferdepth_log2;
   /** Threshold register. An IRQ is emitted each time the write pointer passes this threshold. The value is must be specified as log2 of the required threshold size. at address offset 0x010, read-write */
   uint32_t threshold_log2;
   /** This counter is incremented each time the threshold is hit. at address offset 0x014, read-write */
   uint32_t threshold_counter;
   /** Reserved space */
   uint8_t fill0[744];
   /** Event clear at address offset 0x300, write-only */
   uint32_t eventclr;
   /** Event status (raws status masked with the enable register) at address offset 0x304, read-only */
   uint32_t eventstat;
   /** Event raw status at address offset 0x308, read-only */
   uint32_t eventraw;
   /** Event Enable Set at address offset 0x30C, read-write */
   uint32_t eventenableset;
   /** Event Enable Clear at address offset 0x310, read-write */
   uint32_t eventenableclr;
   /** Reserved space */
   uint8_t fill1[3260];
   /** Peripheral ID Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** Peripheral ID Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** Peripheral ID Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** Peripheral ID Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** Peripheral ID Register at address offset 0xFE0, read-only constant 0x00000088 */
   uint32_t peripheralid0;
   /** Peripheral ID Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** Peripheral ID Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** Peripheral ID Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** Primecell Component ID0 at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** Primecell Component ID1 at address offset 0xFF4, read-only constant 0x00000090 */
   uint32_t componentid1;
   /** Primecell Component ID2 at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** Primecell Component ID3 at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for etr_limiter_s#ctrl */
#define ETR_LIMITER_CTRL_LIMITER_ENABLE_OFFSET 0
#define ETR_LIMITER_CTRL_LIMITER_ENABLE_SIZE 1
#define ETR_LIMITER_CTRL_PUSHBACK_ENABLE_OFFSET 1
#define ETR_LIMITER_CTRL_PUSHBACK_ENABLE_SIZE 1

/** bit field defines for etr_limiter_s#read_pointer */
#define ETR_LIMITER_READ_POINTER_READ_POINTER_OFFSET 2
#define ETR_LIMITER_READ_POINTER_READ_POINTER_SIZE 30

/** bit field defines for etr_limiter_s#write_pointer */
#define ETR_LIMITER_WRITE_POINTER_WRITE_POINTER_OFFSET 2
#define ETR_LIMITER_WRITE_POINTER_WRITE_POINTER_SIZE 30

/** bit field defines for etr_limiter_s#bufferdepth_log2 */
#define ETR_LIMITER_BUFFERDEPTH_LOG2_BUFFERDEPTH_LOG2_OFFSET 0
#define ETR_LIMITER_BUFFERDEPTH_LOG2_BUFFERDEPTH_LOG2_SIZE 5

/** bit field defines for etr_limiter_s#threshold_log2 */
#define ETR_LIMITER_THRESHOLD_LOG2_THRESHOLD_LOG2_OFFSET 0
#define ETR_LIMITER_THRESHOLD_LOG2_THRESHOLD_LOG2_SIZE 5

/** bit field defines for etr_limiter_s#threshold_counter */
#define ETR_LIMITER_THRESHOLD_COUNTER_THRESHOLD_COUNTER_OFFSET 0
#define ETR_LIMITER_THRESHOLD_COUNTER_THRESHOLD_COUNTER_SIZE 5

/** bit field defines for etr_limiter_s#eventclr */
#define ETR_LIMITER_EVENTCLR_THRESHOLD_HIT_AND_WRITTEN_OFFSET 0
#define ETR_LIMITER_EVENTCLR_THRESHOLD_HIT_AND_WRITTEN_SIZE 1
#define ETR_LIMITER_EVENTCLR_TRANSFER_EXCEEDS_READ_POINTER_OFFSET 1
#define ETR_LIMITER_EVENTCLR_TRANSFER_EXCEEDS_READ_POINTER_SIZE 1

/** bit field defines for etr_limiter_s#eventstat */
#define ETR_LIMITER_EVENTSTAT_THRESHOLD_HIT_AND_WRITTEN_OFFSET 0
#define ETR_LIMITER_EVENTSTAT_THRESHOLD_HIT_AND_WRITTEN_SIZE 1
#define ETR_LIMITER_EVENTSTAT_TRANSFER_EXCEEDS_READ_POINTER_OFFSET 1
#define ETR_LIMITER_EVENTSTAT_TRANSFER_EXCEEDS_READ_POINTER_SIZE 1

/** bit field defines for etr_limiter_s#eventraw */
#define ETR_LIMITER_EVENTRAW_THRESHOLD_HIT_AND_WRITTEN_OFFSET 0
#define ETR_LIMITER_EVENTRAW_THRESHOLD_HIT_AND_WRITTEN_SIZE 1
#define ETR_LIMITER_EVENTRAW_TRANSFER_EXCEEDS_READ_POINTER_OFFSET 1
#define ETR_LIMITER_EVENTRAW_TRANSFER_EXCEEDS_READ_POINTER_SIZE 1

/** bit field defines for etr_limiter_s#eventenableset */
#define ETR_LIMITER_EVENTENABLESET_THRESHOLD_HIT_AND_WRITTEN_OFFSET 0
#define ETR_LIMITER_EVENTENABLESET_THRESHOLD_HIT_AND_WRITTEN_SIZE 1
#define ETR_LIMITER_EVENTENABLESET_TRANSFER_EXCEEDS_READ_POINTER_OFFSET 1
#define ETR_LIMITER_EVENTENABLESET_TRANSFER_EXCEEDS_READ_POINTER_SIZE 1

/** bit field defines for etr_limiter_s#eventenableclr */
#define ETR_LIMITER_EVENTENABLECLR_THRESHOLD_HIT_AND_WRITTEN_OFFSET 0
#define ETR_LIMITER_EVENTENABLECLR_THRESHOLD_HIT_AND_WRITTEN_SIZE 1
#define ETR_LIMITER_EVENTENABLECLR_TRANSFER_EXCEEDS_READ_POINTER_OFFSET 1
#define ETR_LIMITER_EVENTENABLECLR_TRANSFER_EXCEEDS_READ_POINTER_SIZE 1

/** bit field defines for etr_limiter_s#peripheralid4 */
#define ETR_LIMITER_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define ETR_LIMITER_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid5 */
#define ETR_LIMITER_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define ETR_LIMITER_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid6 */
#define ETR_LIMITER_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define ETR_LIMITER_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid7 */
#define ETR_LIMITER_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define ETR_LIMITER_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid0 */
#define ETR_LIMITER_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define ETR_LIMITER_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid1 */
#define ETR_LIMITER_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define ETR_LIMITER_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid2 */
#define ETR_LIMITER_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define ETR_LIMITER_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for etr_limiter_s#peripheralid3 */
#define ETR_LIMITER_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define ETR_LIMITER_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for etr_limiter_s#componentid0 */
#define ETR_LIMITER_COMPONENTID0_COMPONENTID0_OFFSET 0
#define ETR_LIMITER_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for etr_limiter_s#componentid1 */
#define ETR_LIMITER_COMPONENTID1_COMPONENTID1_OFFSET 0
#define ETR_LIMITER_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for etr_limiter_s#componentid2 */
#define ETR_LIMITER_COMPONENTID2_COMPONENTID2_OFFSET 0
#define ETR_LIMITER_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for etr_limiter_s#componentid3 */
#define ETR_LIMITER_COMPONENTID3_COMPONENTID3_OFFSET 0
#define ETR_LIMITER_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF etr_limiter.h */
#endif
