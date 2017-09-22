#ifndef INCLUDED_STM_STIM
#define INCLUDED_STM_STIM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** Number of Extended Stimulus Ports */
#define STM_STIM_NR_PORTS 65536
/** Number of bytes per Stimulus Port (channel) */
#define STM_STIM_WIDTH 256

/** Extended Stimulus Ports
*/
struct stm_stim_ports_s {
   /** Data, marked with timestamp, guaranteed at address offset 0x000, write-only */
   uint32_t g_dmts;
   /** Reserved space */
   uint8_t fill0[4];
   /** Data, marked, guaranteed at address offset 0x008, write-only */
   uint32_t g_dm;
   /** Reserved space */
   uint8_t fill1[4];
   /** Data, with timestamp, guaranteed at address offset 0x010, write-only */
   uint32_t g_dts;
   /** Reserved space */
   uint8_t fill2[4];
   /** Data, guaranteed at address offset 0x018, write-only */
   uint32_t g_d;
   /** Reserved space */
   uint8_t fill3[68];
   /** Flag with timestamp, guaranteed at address offset 0x060, write-only */
   uint32_t g_flagts;
   /** Reserved space */
   uint8_t fill4[4];
   /** Flag, guaranteed at address offset 0x068, write-only */
   uint32_t g_flag;
   /** Reserved space */
   uint8_t fill5[4];
   /** Trigger with timestamp, guaranteed at address offset 0x070, write-only */
   uint32_t g_trigts;
   /** Reserved space */
   uint8_t fill6[4];
   /** Trigger, guaranteed at address offset 0x078, write-only */
   uint32_t g_trig;
   /** Reserved space */
   uint8_t fill7[4];
   /** Data, marked with timestamp, invariant timing at address offset 0x080, write-only */
   uint32_t i_dmts;
   /** Reserved space */
   uint8_t fill8[4];
   /** Data, marked, invariant timing at address offset 0x088, write-only */
   uint32_t i_dm;
   /** Reserved space */
   uint8_t fill9[4];
   /** Data, with timestamp, invariant timing at address offset 0x090, write-only */
   uint32_t i_dts;
   /** Reserved space */
   uint8_t fill10[4];
   /** Data, invariant timing at address offset 0x098, write-only */
   uint32_t i_d;
   /** Reserved space */
   uint8_t fill11[68];
   /** Flag with timestamp, invariant timing at address offset 0x0E0, write-only */
   uint32_t i_flagts;
   /** Reserved space */
   uint8_t fill12[4];
   /** Flag, invariant timing at address offset 0x0E8, write-only */
   uint32_t i_flag;
   /** Reserved space */
   uint8_t fill13[4];
   /** Trigger with timestamp, invariant timing at address offset 0x0F0, write-only */
   uint32_t i_trigts;
   /** Reserved space */
   uint8_t fill14[4];
   /** Trigger, invariant timing at address offset 0x0F8, write-only */
   uint32_t i_trig;
   /** Reserved space \private at address offset 0x0FC, read-write */
   uint32_t fill;
};

/** bit field defines for stm_stim_ports_s#g_dmts */
#define STM_STIM_PORTS_G_DMTS_G_DMTS_OFFSET 0
#define STM_STIM_PORTS_G_DMTS_G_DMTS_SIZE 32

/** bit field defines for stm_stim_ports_s#g_dm */
#define STM_STIM_PORTS_G_DM_G_DM_OFFSET 0
#define STM_STIM_PORTS_G_DM_G_DM_SIZE 32

/** bit field defines for stm_stim_ports_s#g_dts */
#define STM_STIM_PORTS_G_DTS_G_DTS_OFFSET 0
#define STM_STIM_PORTS_G_DTS_G_DTS_SIZE 32

/** bit field defines for stm_stim_ports_s#g_d */
#define STM_STIM_PORTS_G_D_G_D_OFFSET 0
#define STM_STIM_PORTS_G_D_G_D_SIZE 32

/** bit field defines for stm_stim_ports_s#g_flagts */
#define STM_STIM_PORTS_G_FLAGTS_G_FLAGTS_OFFSET 0
#define STM_STIM_PORTS_G_FLAGTS_G_FLAGTS_SIZE 32

/** bit field defines for stm_stim_ports_s#g_flag */
#define STM_STIM_PORTS_G_FLAG_G_FLAG_OFFSET 0
#define STM_STIM_PORTS_G_FLAG_G_FLAG_SIZE 32

/** bit field defines for stm_stim_ports_s#g_trigts */
#define STM_STIM_PORTS_G_TRIGTS_G_TRIGTS_OFFSET 0
#define STM_STIM_PORTS_G_TRIGTS_G_TRIGTS_SIZE 32

/** bit field defines for stm_stim_ports_s#g_trig */
#define STM_STIM_PORTS_G_TRIG_G_TRIG_OFFSET 0
#define STM_STIM_PORTS_G_TRIG_G_TRIG_SIZE 32

/** bit field defines for stm_stim_ports_s#i_dmts */
#define STM_STIM_PORTS_I_DMTS_I_DMTS_OFFSET 0
#define STM_STIM_PORTS_I_DMTS_I_DMTS_SIZE 32

/** bit field defines for stm_stim_ports_s#i_dm */
#define STM_STIM_PORTS_I_DM_I_DM_OFFSET 0
#define STM_STIM_PORTS_I_DM_I_DM_SIZE 32

/** bit field defines for stm_stim_ports_s#i_dts */
#define STM_STIM_PORTS_I_DTS_I_DTS_OFFSET 0
#define STM_STIM_PORTS_I_DTS_I_DTS_SIZE 32

/** bit field defines for stm_stim_ports_s#i_d */
#define STM_STIM_PORTS_I_D_I_D_OFFSET 0
#define STM_STIM_PORTS_I_D_I_D_SIZE 32

/** bit field defines for stm_stim_ports_s#i_flagts */
#define STM_STIM_PORTS_I_FLAGTS_I_FLAGTS_OFFSET 0
#define STM_STIM_PORTS_I_FLAGTS_I_FLAGTS_SIZE 32

/** bit field defines for stm_stim_ports_s#i_flag */
#define STM_STIM_PORTS_I_FLAG_I_FLAG_OFFSET 0
#define STM_STIM_PORTS_I_FLAG_I_FLAG_SIZE 32

/** bit field defines for stm_stim_ports_s#i_trigts */
#define STM_STIM_PORTS_I_TRIGTS_I_TRIGTS_OFFSET 0
#define STM_STIM_PORTS_I_TRIGTS_I_TRIGTS_SIZE 32

/** bit field defines for stm_stim_ports_s#i_trig */
#define STM_STIM_PORTS_I_TRIG_I_TRIG_OFFSET 0
#define STM_STIM_PORTS_I_TRIG_I_TRIG_SIZE 32

/** Extended Stimulus Ports
*/
struct stm_stim_s {
   /** Extended Stimulus Ports at address offset 0x000 size: 16777216 */
   struct stm_stim_ports_s ports[STM_STIM_NR_PORTS];
};

/* EOF stm_stim.h */
#endif
