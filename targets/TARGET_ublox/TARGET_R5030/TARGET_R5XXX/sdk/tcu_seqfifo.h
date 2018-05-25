#ifndef INCLUDED_TCU_SEQFIFO
#define INCLUDED_TCU_SEQFIFO
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** TCU sequence FIFO write address */
#define NR_TCU 2

/** TCU sequence FIFO write address
*/
struct tcu_seqfifo_seqfifo_s {
   /** TCU sequence FIFO write address at address offset 0x000, write-only */
   uint32_t seqfifo;
   /** TCU fill address at address offset 0x004, write-only */
   uint32_t fill[1023];
};

/** bit field defines for tcu_seqfifo_seqfifo_s#seqfifo */
#define TCU_SEQFIFO_SEQFIFO_SEQFIFO_SEQFIFO_OFFSET 0
#define TCU_SEQFIFO_SEQFIFO_SEQFIFO_SEQFIFO_SIZE 32

/** Timping Control Unit Sequence FIFOs
*/
struct tcu_seqfifo_s {
   /** TCU sequence FIFO write address at address offset 0x000 size: 8192 */
   struct tcu_seqfifo_seqfifo_s seqfifo[NR_TCU];
};

/* EOF tcu_seqfifo.h */
#endif
