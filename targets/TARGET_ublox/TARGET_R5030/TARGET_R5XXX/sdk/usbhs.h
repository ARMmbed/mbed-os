#ifndef INCLUDED_USBHS
#define INCLUDED_USBHS
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** usbhs HAL Spreadsheet version number */
#define USBHS_HAL_VERSION 1

/** USB Module
*/
struct usbhs_s {
   /**  at address offset 0x000, read-write */
   uint32_t ep0;
   /**  at address offset 0x004, read-write */
   uint32_t lpm;
   /**  at address offset 0x008, read-write */
   uint32_t ep1out;
   /**  at address offset 0x00C, read-write */
   uint32_t ep1in;
   /**  at address offset 0x010, read-write */
   uint32_t ep2out;
   /**  at address offset 0x014, read-write */
   uint32_t ep2in;
   /**  at address offset 0x018, read-write */
   uint32_t ep3out;
   /**  at address offset 0x01C, read-write */
   uint32_t ep3in;
   /**  at address offset 0x020, read-write */
   uint32_t ep4out;
   /**  at address offset 0x024, read-write */
   uint32_t ep4in;
   /**  at address offset 0x028, read-write */
   uint32_t ep5out;
   /**  at address offset 0x02C, read-write */
   uint32_t ep5in;
   /** Reserved space */
   uint8_t fill0[4];
   /**  at address offset 0x034, read-write */
   uint32_t ep6in;
   /** Reserved space */
   uint8_t fill1[4];
   /**  at address offset 0x03C, read-write */
   uint32_t ep7in;
   /** Reserved space */
   uint8_t fill2[4];
   /**  at address offset 0x044, read-write */
   uint32_t ep8in;
   /** Reserved space */
   uint8_t fill3[60];
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x084, read-write */
   uint32_t fifo1dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x088, read-write */
   uint32_t fifo2dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x08C, read-write */
   uint32_t fifo3dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x090, read-write */
   uint32_t fifo4dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x094, read-write */
   uint32_t fifo5dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x098, read-write */
   uint32_t fifo6dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x09C, read-write */
   uint32_t fifo7dat;
   /** The fifoxdat register location is used for both endpoint directions: IN and OUT.  When the fifoxdat is written, the access is to the FIFO X buffer which is associated with the IN x endpoint.  When the fifoxdat is read, the access is to the FIFO X buffer that is associated with the OUT x endpoint. at address offset 0x0A0, read-write */
   uint32_t fifo8dat;
   /** Reserved space */
   uint8_t fill4[92];
   /** The 64 bytes of the IN 0 endpoint buffer are accessible from address 0x100H to 0x13FH. at address offset 0x100, write-only */
   uint32_t ep0indat_1;
   /**  at address offset 0x104, write-only */
   uint32_t ep0indat_2;
   /**  at address offset 0x108, write-only */
   uint32_t ep0indat_3;
   /**  at address offset 0x10C, write-only */
   uint32_t ep0indat_4;
   /**  at address offset 0x110, write-only */
   uint32_t ep0indat_5;
   /**  at address offset 0x114, write-only */
   uint32_t ep0indat_6;
   /**  at address offset 0x118, write-only */
   uint32_t ep0indat_7;
   /**  at address offset 0x11C, write-only */
   uint32_t ep0indat_8;
   /**  at address offset 0x120, write-only */
   uint32_t ep0indat_9;
   /**  at address offset 0x124, write-only */
   uint32_t ep0indat_10;
   /**  at address offset 0x128, write-only */
   uint32_t ep0indat_11;
   /**  at address offset 0x12C, write-only */
   uint32_t ep0indat_12;
   /**  at address offset 0x130, write-only */
   uint32_t ep0indat_13;
   /**  at address offset 0x134, write-only */
   uint32_t ep0indat_14;
   /**  at address offset 0x138, write-only */
   uint32_t ep0indat_15;
   /**  at address offset 0x13C, write-only */
   uint32_t ep0indat_16;
   /** The 64 bytes of the OUT 0 endpoint buffer are accessible from address 0x140H to 0x17FH. at address offset 0x140, read-only */
   uint32_t ep0outdat_1;
   /**  at address offset 0x144, read-only */
   uint32_t ep0outdat_2;
   /**  at address offset 0x148, read-only */
   uint32_t ep0outdat_3;
   /**  at address offset 0x14C, read-only */
   uint32_t ep0outdat_4;
   /**  at address offset 0x150, read-only */
   uint32_t ep0outdat_5;
   /**  at address offset 0x154, read-only */
   uint32_t ep0outdat_6;
   /**  at address offset 0x158, read-only */
   uint32_t ep0outdat_7;
   /**  at address offset 0x15C, read-only */
   uint32_t ep0outdat_8;
   /**  at address offset 0x160, read-only */
   uint32_t ep0outdat_9;
   /**  at address offset 0x164, read-only */
   uint32_t ep0outdat_10;
   /**  at address offset 0x168, read-only */
   uint32_t ep0outdat_11;
   /**  at address offset 0x16C, read-only */
   uint32_t ep0outdat_12;
   /**  at address offset 0x170, read-only */
   uint32_t ep0outdat_13;
   /**  at address offset 0x174, read-only */
   uint32_t ep0outdat_14;
   /**  at address offset 0x178, read-only */
   uint32_t ep0outdat_15;
   /**  at address offset 0x17C, read-only */
   uint32_t ep0outdat_16;
   /** The setupdat contains the 8 bytes of the SETUP data packet from the latest CONTROL transfer at address offset 0x180, read-only */
   uint32_t setupdat_0;
   /**  at address offset 0x184, read-only */
   uint32_t setupdat_1;
   /** IN x interrupt request - The CUSB2 sets the inxirq bit to 1 when it transmits an IN x data packet, receives an ACK from the host, and the "busy" bit (inxcs[1]) changes from 1 to 0 (all sub-buffers are full and one sub-buffer is becoming empty). - Write a 1 to this bit to clear the interrupt request at address offset 0x188, read-write */
   uint32_t endpoint_interrupt_request;
   /**  at address offset 0x18C, read-write */
   uint32_t usb_interrupt_request;
   /**  at address offset 0x190, read-write */
   uint32_t fifo_interrupt_request;
   /**  at address offset 0x194, read-write */
   uint32_t endpoint_interrupt_enable;
   /**  at address offset 0x198, read-write */
   uint32_t usb_interrupt_enable;
   /**  at address offset 0x19C, read-write */
   uint32_t fifo_interrupt_enable;
   /**  at address offset 0x1A0, read-write */
   uint32_t interrupt_vector_and_reset;
   /**  at address offset 0x1A4, read-only */
   uint32_t usb_csr;
   /**  at address offset 0x1A8, read-write */
   uint32_t fifoctrl;
   /** Reserved space */
   uint8_t fill5[52];
   /**  at address offset 0x1E0, read-write */
   uint32_t outmaxpck_1_0;
   /**  at address offset 0x1E4, read-write */
   uint32_t outmaxpck_3_2;
   /**  at address offset 0x1E8, read-write */
   uint32_t outmaxpck_5_4;
   /** Reserved space */
   uint8_t fill6[280];
   /**  at address offset 0x304, read-write */
   uint32_t out1staddr;
   /**  at address offset 0x308, read-write */
   uint32_t out2staddr;
   /**  at address offset 0x30C, read-write */
   uint32_t out3staddr;
   /**  at address offset 0x310, read-write */
   uint32_t out4staddr;
   /**  at address offset 0x314, read-write */
   uint32_t out5staddr;
   /** Reserved space */
   uint8_t fill7[44];
   /**  at address offset 0x344, read-write */
   uint32_t in1staddr;
   /**  at address offset 0x348, read-write */
   uint32_t in2staddr;
   /**  at address offset 0x34C, read-write */
   uint32_t in3staddr;
   /**  at address offset 0x350, read-write */
   uint32_t in4staddr;
   /**  at address offset 0x354, read-write */
   uint32_t in5staddr;
   /**  at address offset 0x358, read-write */
   uint32_t in6staddr;
   /**  at address offset 0x35C, read-write */
   uint32_t in7staddr;
   /**  at address offset 0x360, read-write */
   uint32_t in8staddr;
   /** Reserved space */
   uint8_t fill8[96];
   /** The processor writes to the cpuctrl 2 register to support single interrupt request. at address offset 0x3C4, read-write */
   uint32_t cpuctrl2;
   /** Reserved space */
   uint8_t fill9[20];
   /**  at address offset 0x3DC, read-only */
   uint32_t endian_sfr;
   /** The maximum packet size for IN endpoints is reported during the USB enumeration procedure at address offset 0x3E0, read-write */
   uint32_t inmaxpck_1;
   /**  at address offset 0x3E4, read-write */
   uint32_t inmaxpck_3_2;
   /**  at address offset 0x3E8, read-write */
   uint32_t inmaxpck_5_4;
   /**  at address offset 0x3EC, read-write */
   uint32_t inmaxpck_7_6;
   /**  at address offset 0x3F0, read-write */
   uint32_t inmaxpck_9_8;
   /** Reserved space */
   uint8_t fill10[12];
   /** Global Configuration Register at address offset 0x400, read-write */
   uint32_t dma_conf;
   /** Global Status Register at address offset 0x404, read-only */
   uint32_t dma_sts;
   /** Reserved space */
   uint8_t fill11[20];
   /** Endpoint Select Register at address offset 0x41C, read-write */
   uint32_t dma_ep_sel;
   /**  at address offset 0x420, read-write */
   uint32_t dma_ep_traddr;
   /** Endpoint Configuration Register at address offset 0x424, read-write */
   uint32_t ep_cfg;
   /** Endpoint Command Register at address offset 0x428, read-write */
   uint32_t dma_ep_cmd;
   /** Endpoint Status Register at address offset 0x42C, read-write */
   uint32_t dma_ep_sts;
   /** Reserved space */
   uint8_t fill12[4];
   /** Endpoint Status Register Enable at address offset 0x434, read-write */
   uint32_t dma_ep_sts_en;
   /** Doorbell Register at address offset 0x438, read-write */
   uint32_t dma_drbl;
   /** EP Interrupt Enable Register at address offset 0x43C, read-write */
   uint32_t dma_ep_ien;
   /** EP Interrupt Status Register at address offset 0x440, read-only */
   uint32_t dma_ep_ists;
   /** AXI Master Control register at address offset 0x444, read-write */
   uint32_t dma_axim_ctrl;
   /** AXI Master ID register at address offset 0x448, read-write */
   uint32_t dma_axim_id;
   /** Reserved space */
   uint8_t fill13[4];
   /** AXI Master Wrapper Extended Capability at address offset 0x450, read-write */
   uint32_t dma_axim_cap;
   /** Reserved space */
   uint8_t fill14[4];
   /** AXI Master Wrapper Extended Capability Control Register 0 at address offset 0x458, read-write */
   uint32_t dma_axim_ctrl0;
   /** AXI Master Wrapper Extended Capability Control Register 1 at address offset 0x45C, read-write */
   uint32_t dma_axim_ctrl1;
};

/** bit field defines for usbhs_s#ep0 */
#define USBHS_EP0_OUT0BC_OFFSET 0
#define USBHS_EP0_OUT0BC_SIZE 7
#define USBHS_EP0_IN0BC_OFFSET 8
#define USBHS_EP0_IN0BC_SIZE 7
#define USBHS_EP0_EP0CS_STALL_OFFSET 16
#define USBHS_EP0_EP0CS_STALL_SIZE 1
#define USBHS_EP0_EP0CS_HSNAK_OFFSET 17
#define USBHS_EP0_EP0CS_HSNAK_SIZE 1
#define USBHS_EP0_EP0CS_INBSY_OFFSET 18
#define USBHS_EP0_EP0CS_INBSY_SIZE 1
#define USBHS_EP0_EP0CS_OUTBSY_OFFSET 19
#define USBHS_EP0_EP0CS_OUTBSY_SIZE 1
#define USBHS_EP0_EP0CS_DSTALL_OFFSET 20
#define USBHS_EP0_EP0CS_DSTALL_SIZE 1
#define USBHS_EP0_EP0CS_CHGSET_OFFSET 23
#define USBHS_EP0_EP0CS_CHGSET_SIZE 1

/** bit field defines for usbhs_s#lpm */
#define USBHS_LPM_LPMCTRLL_HIRD_OFFSET 4
#define USBHS_LPM_LPMCTRLL_HIRD_SIZE 4
#define USBHS_LPM_LPMCTRLH_BREMOTEWAKEUP_OFFSET 8
#define USBHS_LPM_LPMCTRLH_BREMOTEWAKEUP_SIZE 1
#define USBHS_LPM_LPMCTRLH_LPMNYET_OFFSET 15
#define USBHS_LPM_LPMCTRLH_LPMNYET_SIZE 1
#define USBHS_LPM_LPMCLK_SLEEPENTRY_OFFSET 23
#define USBHS_LPM_LPMCLK_SLEEPENTRY_SIZE 1
#define USBHS_LPM_EP0FIFO_FIFOAUTOOUT_OFFSET 24
#define USBHS_LPM_EP0FIFO_FIFOAUTOOUT_SIZE 1
#define USBHS_LPM_EP0FIFO_FIFOAUTOIN_OFFSET 25
#define USBHS_LPM_EP0FIFO_FIFOAUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep1out */
#define USBHS_EP1OUT_OUT1BC_OFFSET 0
#define USBHS_EP1OUT_OUT1BC_SIZE 11
#define USBHS_EP1OUT_OUT1CON_BUF_OFFSET 16
#define USBHS_EP1OUT_OUT1CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP1OUT_OUT1CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP1OUT_OUT1CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00"-- single buffering */
#define USBHS_EP1OUT_OUT1CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10"-- triple buffering */
#define USBHS_EP1OUT_OUT1CON_BUF_TB_VALUE 2
#define USBHS_EP1OUT_OUT1CON_TYPE_OFFSET 18
#define USBHS_EP1OUT_OUT1CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP1OUT_OUT1CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP1OUT_OUT1CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP1OUT_OUT1CON_TYPE_ISE_VALUE 1
#define USBHS_EP1OUT_OUT1CON_STALL_OFFSET 22
#define USBHS_EP1OUT_OUT1CON_STALL_SIZE 1
#define USBHS_EP1OUT_OUT1CON_VAL_OFFSET 23
#define USBHS_EP1OUT_OUT1CON_VAL_SIZE 1
#define USBHS_EP1OUT_OUT1CS_ERR_OFFSET 24
#define USBHS_EP1OUT_OUT1CS_ERR_SIZE 1
#define USBHS_EP1OUT_OUT1CS_BUSY_OFFSET 25
#define USBHS_EP1OUT_OUT1CS_BUSY_SIZE 1
#define USBHS_EP1OUT_OUT1CS_NPAK_OFFSET 26
#define USBHS_EP1OUT_OUT1CS_NPAK_SIZE 2
#define USBHS_EP1OUT_OUT1CS_AUTOOUT_OFFSET 28
#define USBHS_EP1OUT_OUT1CS_AUTOOUT_SIZE 1

/** bit field defines for usbhs_s#ep1in */
#define USBHS_EP1IN_IN1BC_OFFSET 0
#define USBHS_EP1IN_IN1BC_SIZE 11
#define USBHS_EP1IN_IN1CON_BUF_OFFSET 16
#define USBHS_EP1IN_IN1CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP1IN_IN1CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP1IN_IN1CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP1IN_IN1CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP1IN_IN1CON_BUF_TB_VALUE 2
#define USBHS_EP1IN_IN1CON_TYPE_OFFSET 18
#define USBHS_EP1IN_IN1CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP1IN_IN1CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP1IN_IN1CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP1IN_IN1CON_TYPE_ISE_VALUE 1
#define USBHS_EP1IN_IN1CON_ISOD_OFFSET 20
#define USBHS_EP1IN_IN1CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP1IN_IN1CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP1IN_IN1CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP1IN_IN1CON_ISOD_TP_VALUE 2
#define USBHS_EP1IN_IN1CON_STALL_OFFSET 22
#define USBHS_EP1IN_IN1CON_STALL_SIZE 1
#define USBHS_EP1IN_IN1CON_VAL_OFFSET 23
#define USBHS_EP1IN_IN1CON_VAL_SIZE 1
#define USBHS_EP1IN_IN1CS_ERR_OFFSET 24
#define USBHS_EP1IN_IN1CS_ERR_SIZE 1
#define USBHS_EP1IN_IN1CS_BUSY_OFFSET 25
#define USBHS_EP1IN_IN1CS_BUSY_SIZE 1
#define USBHS_EP1IN_IN1CS_NPAK_OFFSET 26
#define USBHS_EP1IN_IN1CS_NPAK_SIZE 2
#define USBHS_EP1IN_IN1CS_AUTOIN_OFFSET 28
#define USBHS_EP1IN_IN1CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep2out */
#define USBHS_EP2OUT_OUT2BC_OFFSET 0
#define USBHS_EP2OUT_OUT2BC_SIZE 11
#define USBHS_EP2OUT_OUT2CON_BUF_OFFSET 16
#define USBHS_EP2OUT_OUT2CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP2OUT_OUT2CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP2OUT_OUT2CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00"-- single buffering */
#define USBHS_EP2OUT_OUT2CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10"-- triple buffering */
#define USBHS_EP2OUT_OUT2CON_BUF_TB_VALUE 2
#define USBHS_EP2OUT_OUT2CON_TYPE_OFFSET 18
#define USBHS_EP2OUT_OUT2CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP2OUT_OUT2CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP2OUT_OUT2CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP2OUT_OUT2CON_TYPE_ISE_VALUE 1
#define USBHS_EP2OUT_OUT2CON_STALL_OFFSET 22
#define USBHS_EP2OUT_OUT2CON_STALL_SIZE 1
#define USBHS_EP2OUT_OUT2CON_VAL_OFFSET 23
#define USBHS_EP2OUT_OUT2CON_VAL_SIZE 1
#define USBHS_EP2OUT_OUT2CS_ERR_OFFSET 24
#define USBHS_EP2OUT_OUT2CS_ERR_SIZE 1
#define USBHS_EP2OUT_OUT2CS_BUSY_OFFSET 25
#define USBHS_EP2OUT_OUT2CS_BUSY_SIZE 1
#define USBHS_EP2OUT_OUT2CS_NPAK_OFFSET 26
#define USBHS_EP2OUT_OUT2CS_NPAK_SIZE 2
#define USBHS_EP2OUT_OUT2CS_AUTOOUT_OFFSET 28
#define USBHS_EP2OUT_OUT2CS_AUTOOUT_SIZE 1

/** bit field defines for usbhs_s#ep2in */
#define USBHS_EP2IN_IN2BC_OFFSET 0
#define USBHS_EP2IN_IN2BC_SIZE 11
#define USBHS_EP2IN_IN2CON_BUF_OFFSET 16
#define USBHS_EP2IN_IN2CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP2IN_IN2CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP2IN_IN2CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP2IN_IN2CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP2IN_IN2CON_BUF_TB_VALUE 2
#define USBHS_EP2IN_IN2CON_TYPE_OFFSET 18
#define USBHS_EP2IN_IN2CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP2IN_IN2CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP2IN_IN2CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP2IN_IN2CON_TYPE_ISE_VALUE 1
#define USBHS_EP2IN_IN2CON_ISOD_OFFSET 20
#define USBHS_EP2IN_IN2CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP2IN_IN2CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP2IN_IN2CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP2IN_IN2CON_ISOD_TP_VALUE 2
#define USBHS_EP2IN_IN2CON_STALL_OFFSET 22
#define USBHS_EP2IN_IN2CON_STALL_SIZE 1
#define USBHS_EP2IN_IN2CON_VAL_OFFSET 23
#define USBHS_EP2IN_IN2CON_VAL_SIZE 1
#define USBHS_EP2IN_IN2CS_ERR_OFFSET 24
#define USBHS_EP2IN_IN2CS_ERR_SIZE 1
#define USBHS_EP2IN_IN2CS_BUSY_OFFSET 25
#define USBHS_EP2IN_IN2CS_BUSY_SIZE 1
#define USBHS_EP2IN_IN2CS_NPAK_OFFSET 26
#define USBHS_EP2IN_IN2CS_NPAK_SIZE 2
#define USBHS_EP2IN_IN2CS_AUTOIN_OFFSET 28
#define USBHS_EP2IN_IN2CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep3out */
#define USBHS_EP3OUT_OUT3BC_OFFSET 0
#define USBHS_EP3OUT_OUT3BC_SIZE 11
#define USBHS_EP3OUT_OUT3CON_BUF_OFFSET 16
#define USBHS_EP3OUT_OUT3CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP3OUT_OUT3CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP3OUT_OUT3CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00"-- single buffering */
#define USBHS_EP3OUT_OUT3CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10"-- triple buffering */
#define USBHS_EP3OUT_OUT3CON_BUF_TB_VALUE 2
#define USBHS_EP3OUT_OUT3CON_TYPE_OFFSET 18
#define USBHS_EP3OUT_OUT3CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP3OUT_OUT3CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP3OUT_OUT3CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP3OUT_OUT3CON_TYPE_ISE_VALUE 1
#define USBHS_EP3OUT_OUT3CON_STALL_OFFSET 22
#define USBHS_EP3OUT_OUT3CON_STALL_SIZE 1
#define USBHS_EP3OUT_OUT3CON_VAL_OFFSET 23
#define USBHS_EP3OUT_OUT3CON_VAL_SIZE 1
#define USBHS_EP3OUT_OUT3CS_ERR_OFFSET 24
#define USBHS_EP3OUT_OUT3CS_ERR_SIZE 1
#define USBHS_EP3OUT_OUT3CS_BUSY_OFFSET 25
#define USBHS_EP3OUT_OUT3CS_BUSY_SIZE 1
#define USBHS_EP3OUT_OUT3CS_NPAK_OFFSET 26
#define USBHS_EP3OUT_OUT3CS_NPAK_SIZE 2
#define USBHS_EP3OUT_OUT3CS_AUTOOUT_OFFSET 28
#define USBHS_EP3OUT_OUT3CS_AUTOOUT_SIZE 1

/** bit field defines for usbhs_s#ep3in */
#define USBHS_EP3IN_IN3BC_OFFSET 0
#define USBHS_EP3IN_IN3BC_SIZE 11
#define USBHS_EP3IN_IN3CON_BUF_OFFSET 16
#define USBHS_EP3IN_IN3CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP3IN_IN3CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP3IN_IN3CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP3IN_IN3CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP3IN_IN3CON_BUF_TB_VALUE 2
#define USBHS_EP3IN_IN3CON_TYPE_OFFSET 18
#define USBHS_EP3IN_IN3CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP3IN_IN3CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP3IN_IN3CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP3IN_IN3CON_TYPE_ISE_VALUE 1
#define USBHS_EP3IN_IN3CON_ISOD_OFFSET 20
#define USBHS_EP3IN_IN3CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP3IN_IN3CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP3IN_IN3CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP3IN_IN3CON_ISOD_TP_VALUE 2
#define USBHS_EP3IN_IN3CON_STALL_OFFSET 22
#define USBHS_EP3IN_IN3CON_STALL_SIZE 1
#define USBHS_EP3IN_IN3CON_VAL_OFFSET 23
#define USBHS_EP3IN_IN3CON_VAL_SIZE 1
#define USBHS_EP3IN_IN3CS_ERR_OFFSET 24
#define USBHS_EP3IN_IN3CS_ERR_SIZE 1
#define USBHS_EP3IN_IN3CS_BUSY_OFFSET 25
#define USBHS_EP3IN_IN3CS_BUSY_SIZE 1
#define USBHS_EP3IN_IN3CS_NPAK_OFFSET 26
#define USBHS_EP3IN_IN3CS_NPAK_SIZE 2
#define USBHS_EP3IN_IN3CS_AUTOIN_OFFSET 28
#define USBHS_EP3IN_IN3CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep4out */
#define USBHS_EP4OUT_OUT4BC_OFFSET 0
#define USBHS_EP4OUT_OUT4BC_SIZE 11
#define USBHS_EP4OUT_OUT4CON_BUF_OFFSET 16
#define USBHS_EP4OUT_OUT4CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP4OUT_OUT4CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP4OUT_OUT4CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00"-- single buffering */
#define USBHS_EP4OUT_OUT4CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10"-- triple buffering */
#define USBHS_EP4OUT_OUT4CON_BUF_TB_VALUE 2
#define USBHS_EP4OUT_OUT4CON_TYPE_OFFSET 18
#define USBHS_EP4OUT_OUT4CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP4OUT_OUT4CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP4OUT_OUT4CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP4OUT_OUT4CON_TYPE_ISE_VALUE 1
#define USBHS_EP4OUT_OUT4CON_STALL_OFFSET 22
#define USBHS_EP4OUT_OUT4CON_STALL_SIZE 1
#define USBHS_EP4OUT_OUT4CON_VAL_OFFSET 23
#define USBHS_EP4OUT_OUT4CON_VAL_SIZE 1
#define USBHS_EP4OUT_OUT4CS_ERR_OFFSET 24
#define USBHS_EP4OUT_OUT4CS_ERR_SIZE 1
#define USBHS_EP4OUT_OUT4CS_BUSY_OFFSET 25
#define USBHS_EP4OUT_OUT4CS_BUSY_SIZE 1
#define USBHS_EP4OUT_OUT4CS_NPAK_OFFSET 26
#define USBHS_EP4OUT_OUT4CS_NPAK_SIZE 2
#define USBHS_EP4OUT_OUT4CS_AUTOOUT_OFFSET 28
#define USBHS_EP4OUT_OUT4CS_AUTOOUT_SIZE 1

/** bit field defines for usbhs_s#ep4in */
#define USBHS_EP4IN_IN4BC_OFFSET 0
#define USBHS_EP4IN_IN4BC_SIZE 11
#define USBHS_EP4IN_IN4CON_BUF_OFFSET 16
#define USBHS_EP4IN_IN4CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP4IN_IN4CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP4IN_IN4CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP4IN_IN4CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP4IN_IN4CON_BUF_TB_VALUE 2
#define USBHS_EP4IN_IN4CON_TYPE_OFFSET 18
#define USBHS_EP4IN_IN4CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP4IN_IN4CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP4IN_IN4CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP4IN_IN4CON_TYPE_ISE_VALUE 1
#define USBHS_EP4IN_IN4CON_ISOD_OFFSET 20
#define USBHS_EP4IN_IN4CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP4IN_IN4CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP4IN_IN4CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP4IN_IN4CON_ISOD_TP_VALUE 2
#define USBHS_EP4IN_IN4CON_STALL_OFFSET 22
#define USBHS_EP4IN_IN4CON_STALL_SIZE 1
#define USBHS_EP4IN_IN4CON_VAL_OFFSET 23
#define USBHS_EP4IN_IN4CON_VAL_SIZE 1
#define USBHS_EP4IN_IN4CS_ERR_OFFSET 24
#define USBHS_EP4IN_IN4CS_ERR_SIZE 1
#define USBHS_EP4IN_IN4CS_BUSY_OFFSET 25
#define USBHS_EP4IN_IN4CS_BUSY_SIZE 1
#define USBHS_EP4IN_IN4CS_NPAK_OFFSET 26
#define USBHS_EP4IN_IN4CS_NPAK_SIZE 2
#define USBHS_EP4IN_IN4CS_AUTOIN_OFFSET 28
#define USBHS_EP4IN_IN4CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep5out */
#define USBHS_EP5OUT_OUT5BC_OFFSET 0
#define USBHS_EP5OUT_OUT5BC_SIZE 11
#define USBHS_EP5OUT_OUT5CON_BUF_OFFSET 16
#define USBHS_EP5OUT_OUT5CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP5OUT_OUT5CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP5OUT_OUT5CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00"-- single buffering */
#define USBHS_EP5OUT_OUT5CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10"-- triple buffering */
#define USBHS_EP5OUT_OUT5CON_BUF_TB_VALUE 2
#define USBHS_EP5OUT_OUT5CON_TYPE_OFFSET 18
#define USBHS_EP5OUT_OUT5CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP5OUT_OUT5CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP5OUT_OUT5CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP5OUT_OUT5CON_TYPE_ISE_VALUE 1
#define USBHS_EP5OUT_OUT5CON_STALL_OFFSET 22
#define USBHS_EP5OUT_OUT5CON_STALL_SIZE 1
#define USBHS_EP5OUT_OUT5CON_VAL_OFFSET 23
#define USBHS_EP5OUT_OUT5CON_VAL_SIZE 1
#define USBHS_EP5OUT_OUT5CS_ERR_OFFSET 24
#define USBHS_EP5OUT_OUT5CS_ERR_SIZE 1
#define USBHS_EP5OUT_OUT5CS_BUSY_OFFSET 25
#define USBHS_EP5OUT_OUT5CS_BUSY_SIZE 1
#define USBHS_EP5OUT_OUT5CS_NPAK_OFFSET 26
#define USBHS_EP5OUT_OUT5CS_NPAK_SIZE 2
#define USBHS_EP5OUT_OUT5CS_AUTOOUT_OFFSET 28
#define USBHS_EP5OUT_OUT5CS_AUTOOUT_SIZE 1

/** bit field defines for usbhs_s#ep5in */
#define USBHS_EP5IN_IN5BC_OFFSET 0
#define USBHS_EP5IN_IN5BC_SIZE 11
#define USBHS_EP5IN_IN5CON_BUF_OFFSET 16
#define USBHS_EP5IN_IN5CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP5IN_IN5CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP5IN_IN5CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP5IN_IN5CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP5IN_IN5CON_BUF_TB_VALUE 2
#define USBHS_EP5IN_IN5CON_TYPE_OFFSET 18
#define USBHS_EP5IN_IN5CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP5IN_IN5CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP5IN_IN5CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP5IN_IN5CON_TYPE_ISE_VALUE 1
#define USBHS_EP5IN_IN5CON_ISOD_OFFSET 20
#define USBHS_EP5IN_IN5CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP5IN_IN5CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP5IN_IN5CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP5IN_IN5CON_ISOD_TP_VALUE 2
#define USBHS_EP5IN_IN5CON_STALL_OFFSET 22
#define USBHS_EP5IN_IN5CON_STALL_SIZE 1
#define USBHS_EP5IN_IN5CON_VAL_OFFSET 23
#define USBHS_EP5IN_IN5CON_VAL_SIZE 1
#define USBHS_EP5IN_IN5CS_ERR_OFFSET 24
#define USBHS_EP5IN_IN5CS_ERR_SIZE 1
#define USBHS_EP5IN_IN5CS_BUSY_OFFSET 25
#define USBHS_EP5IN_IN5CS_BUSY_SIZE 1
#define USBHS_EP5IN_IN5CS_NPAK_OFFSET 26
#define USBHS_EP5IN_IN5CS_NPAK_SIZE 2
#define USBHS_EP5IN_IN5CS_AUTOIN_OFFSET 28
#define USBHS_EP5IN_IN5CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep6in */
#define USBHS_EP6IN_IN6BC_OFFSET 0
#define USBHS_EP6IN_IN6BC_SIZE 11
#define USBHS_EP6IN_IN6CON_BUF_OFFSET 16
#define USBHS_EP6IN_IN6CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP6IN_IN6CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP6IN_IN6CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP6IN_IN6CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP6IN_IN6CON_BUF_TB_VALUE 2
#define USBHS_EP6IN_IN6CON_TYPE_OFFSET 18
#define USBHS_EP6IN_IN6CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP6IN_IN6CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP6IN_IN6CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP6IN_IN6CON_TYPE_ISE_VALUE 1
#define USBHS_EP6IN_IN6CON_ISOD_OFFSET 20
#define USBHS_EP6IN_IN6CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP6IN_IN6CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP6IN_IN6CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP6IN_IN6CON_ISOD_TP_VALUE 2
#define USBHS_EP6IN_IN6CON_STALL_OFFSET 22
#define USBHS_EP6IN_IN6CON_STALL_SIZE 1
#define USBHS_EP6IN_IN6CON_VAL_OFFSET 23
#define USBHS_EP6IN_IN6CON_VAL_SIZE 1
#define USBHS_EP6IN_IN6CS_ERR_OFFSET 24
#define USBHS_EP6IN_IN6CS_ERR_SIZE 1
#define USBHS_EP6IN_IN6CS_BUSY_OFFSET 25
#define USBHS_EP6IN_IN6CS_BUSY_SIZE 1
#define USBHS_EP6IN_IN6CS_NPAK_OFFSET 26
#define USBHS_EP6IN_IN6CS_NPAK_SIZE 2
#define USBHS_EP6IN_IN6CS_AUTOIN_OFFSET 28
#define USBHS_EP6IN_IN6CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep7in */
#define USBHS_EP7IN_IN7BC_OFFSET 0
#define USBHS_EP7IN_IN7BC_SIZE 11
#define USBHS_EP7IN_IN7CON_BUF_OFFSET 16
#define USBHS_EP7IN_IN7CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP7IN_IN7CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP7IN_IN7CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP7IN_IN7CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP7IN_IN7CON_BUF_TB_VALUE 2
#define USBHS_EP7IN_IN7CON_TYPE_OFFSET 18
#define USBHS_EP7IN_IN7CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP7IN_IN7CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP7IN_IN7CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP7IN_IN7CON_TYPE_ISE_VALUE 1
#define USBHS_EP7IN_IN7CON_ISOD_OFFSET 20
#define USBHS_EP7IN_IN7CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP7IN_IN7CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP7IN_IN7CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP7IN_IN7CON_ISOD_TP_VALUE 2
#define USBHS_EP7IN_IN7CON_STALL_OFFSET 22
#define USBHS_EP7IN_IN7CON_STALL_SIZE 1
#define USBHS_EP7IN_IN7CON_VAL_OFFSET 23
#define USBHS_EP7IN_IN7CON_VAL_SIZE 1
#define USBHS_EP7IN_IN7CS_ERR_OFFSET 24
#define USBHS_EP7IN_IN7CS_ERR_SIZE 1
#define USBHS_EP7IN_IN7CS_BUSY_OFFSET 25
#define USBHS_EP7IN_IN7CS_BUSY_SIZE 1
#define USBHS_EP7IN_IN7CS_NPAK_OFFSET 26
#define USBHS_EP7IN_IN7CS_NPAK_SIZE 2
#define USBHS_EP7IN_IN7CS_AUTOIN_OFFSET 28
#define USBHS_EP7IN_IN7CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#ep8in */
#define USBHS_EP8IN_IN8BC_OFFSET 0
#define USBHS_EP8IN_IN8BC_SIZE 11
#define USBHS_EP8IN_IN8CON_BUF_OFFSET 16
#define USBHS_EP8IN_IN8CON_BUF_SIZE 2
/** buf1,buf0 = "01"-- double buffering */
#define USBHS_EP8IN_IN8CON_BUF_DB_VALUE 1
/** buf1,buf0 = "11"-- quad buffering */
#define USBHS_EP8IN_IN8CON_BUF_QB_VALUE 3
/** buf1,buf0 = "00" --single buffering */
#define USBHS_EP8IN_IN8CON_BUF_SB_VALUE 0
/** buf1,buf0 = "10" -- triple buffering */
#define USBHS_EP8IN_IN8CON_BUF_TB_VALUE 2
#define USBHS_EP8IN_IN8CON_TYPE_OFFSET 18
#define USBHS_EP8IN_IN8CON_TYPE_SIZE 2
/** type1, type0 = "10" -- bulk endpoint */
#define USBHS_EP8IN_IN8CON_TYPE_BUE_VALUE 2
/** type1, type0 = "11" -- interrupt endpoint */
#define USBHS_EP8IN_IN8CON_TYPE_INE_VALUE 3
/** type1, type0 = "01" -- isochronous endpoint */
#define USBHS_EP8IN_IN8CON_TYPE_ISE_VALUE 1
#define USBHS_EP8IN_IN8CON_ISOD_OFFSET 20
#define USBHS_EP8IN_IN8CON_ISOD_SIZE 2
/** Isod1,isod0="01" -- 2 ISO packets per microframe */
#define USBHS_EP8IN_IN8CON_ISOD_DP_VALUE 1
/** Isod1,isod0="00" -- 1 ISO packet per microframe */
#define USBHS_EP8IN_IN8CON_ISOD_SP_VALUE 0
/** Isod1,isod0="10" -- 3 ISO packets per microframe */
#define USBHS_EP8IN_IN8CON_ISOD_TP_VALUE 2
#define USBHS_EP8IN_IN8CON_STALL_OFFSET 22
#define USBHS_EP8IN_IN8CON_STALL_SIZE 1
#define USBHS_EP8IN_IN8CON_VAL_OFFSET 23
#define USBHS_EP8IN_IN8CON_VAL_SIZE 1
#define USBHS_EP8IN_IN8CS_ERR_OFFSET 24
#define USBHS_EP8IN_IN8CS_ERR_SIZE 1
#define USBHS_EP8IN_IN8CS_BUSY_OFFSET 25
#define USBHS_EP8IN_IN8CS_BUSY_SIZE 1
#define USBHS_EP8IN_IN8CS_NPAK_OFFSET 26
#define USBHS_EP8IN_IN8CS_NPAK_SIZE 2
#define USBHS_EP8IN_IN8CS_AUTOIN_OFFSET 28
#define USBHS_EP8IN_IN8CS_AUTOIN_SIZE 1

/** bit field defines for usbhs_s#fifo1dat */
#define USBHS_FIFO1DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO1DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo2dat */
#define USBHS_FIFO2DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO2DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo3dat */
#define USBHS_FIFO3DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO3DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo4dat */
#define USBHS_FIFO4DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO4DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo5dat */
#define USBHS_FIFO5DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO5DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo6dat */
#define USBHS_FIFO6DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO6DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo7dat */
#define USBHS_FIFO7DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO7DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#fifo8dat */
#define USBHS_FIFO8DAT_FIFOXDAT_OFFSET 0
#define USBHS_FIFO8DAT_FIFOXDAT_SIZE 32

/** bit field defines for usbhs_s#ep0indat_1 */
#define USBHS_EP0INDAT_1_NONE_OFFSET 0
#define USBHS_EP0INDAT_1_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_2 */
#define USBHS_EP0INDAT_2_NONE_OFFSET 0
#define USBHS_EP0INDAT_2_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_3 */
#define USBHS_EP0INDAT_3_NONE_OFFSET 0
#define USBHS_EP0INDAT_3_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_4 */
#define USBHS_EP0INDAT_4_NONE_OFFSET 0
#define USBHS_EP0INDAT_4_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_5 */
#define USBHS_EP0INDAT_5_NONE_OFFSET 0
#define USBHS_EP0INDAT_5_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_6 */
#define USBHS_EP0INDAT_6_NONE_OFFSET 0
#define USBHS_EP0INDAT_6_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_7 */
#define USBHS_EP0INDAT_7_NONE_OFFSET 0
#define USBHS_EP0INDAT_7_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_8 */
#define USBHS_EP0INDAT_8_NONE_OFFSET 0
#define USBHS_EP0INDAT_8_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_9 */
#define USBHS_EP0INDAT_9_NONE_OFFSET 0
#define USBHS_EP0INDAT_9_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_10 */
#define USBHS_EP0INDAT_10_NONE_OFFSET 0
#define USBHS_EP0INDAT_10_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_11 */
#define USBHS_EP0INDAT_11_NONE_OFFSET 0
#define USBHS_EP0INDAT_11_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_12 */
#define USBHS_EP0INDAT_12_NONE_OFFSET 0
#define USBHS_EP0INDAT_12_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_13 */
#define USBHS_EP0INDAT_13_NONE_OFFSET 0
#define USBHS_EP0INDAT_13_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_14 */
#define USBHS_EP0INDAT_14_NONE_OFFSET 0
#define USBHS_EP0INDAT_14_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_15 */
#define USBHS_EP0INDAT_15_NONE_OFFSET 0
#define USBHS_EP0INDAT_15_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0indat_16 */
#define USBHS_EP0INDAT_16_NONE_OFFSET 0
#define USBHS_EP0INDAT_16_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_1 */
#define USBHS_EP0OUTDAT_1_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_1_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_2 */
#define USBHS_EP0OUTDAT_2_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_2_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_3 */
#define USBHS_EP0OUTDAT_3_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_3_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_4 */
#define USBHS_EP0OUTDAT_4_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_4_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_5 */
#define USBHS_EP0OUTDAT_5_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_5_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_6 */
#define USBHS_EP0OUTDAT_6_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_6_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_7 */
#define USBHS_EP0OUTDAT_7_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_7_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_8 */
#define USBHS_EP0OUTDAT_8_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_8_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_9 */
#define USBHS_EP0OUTDAT_9_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_9_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_10 */
#define USBHS_EP0OUTDAT_10_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_10_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_11 */
#define USBHS_EP0OUTDAT_11_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_11_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_12 */
#define USBHS_EP0OUTDAT_12_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_12_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_13 */
#define USBHS_EP0OUTDAT_13_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_13_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_14 */
#define USBHS_EP0OUTDAT_14_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_14_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_15 */
#define USBHS_EP0OUTDAT_15_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_15_NONE_SIZE 32

/** bit field defines for usbhs_s#ep0outdat_16 */
#define USBHS_EP0OUTDAT_16_NONE_OFFSET 0
#define USBHS_EP0OUTDAT_16_NONE_SIZE 32

/** bit field defines for usbhs_s#setupdat_0 */
#define USBHS_SETUPDAT_0_NONE_OFFSET 0
#define USBHS_SETUPDAT_0_NONE_SIZE 32

/** bit field defines for usbhs_s#setupdat_1 */
#define USBHS_SETUPDAT_1_NONE_OFFSET 0
#define USBHS_SETUPDAT_1_NONE_SIZE 32

/** bit field defines for usbhs_s#endpoint_interrupt_request */
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN0IRQ_OFFSET 0
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN0IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN1IRQ_OFFSET 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN1IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN2IRQ_OFFSET 2
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN2IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN3IRQ_OFFSET 3
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN3IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN4IRQ_OFFSET 4
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN4IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN5IRQ_OFFSET 5
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN5IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN6IRQ_OFFSET 6
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN6IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN7IRQ_OFFSET 7
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN7IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN8IRQ_OFFSET 8
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_IN8IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT0IRQ_OFFSET 16
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT0IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT1IRQ_OFFSET 17
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT1IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT2IRQ_OFFSET 18
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT2IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT3IRQ_OFFSET 19
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT3IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT4IRQ_OFFSET 20
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT4IRQ_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT5IRQ_OFFSET 21
#define USBHS_ENDPOINT_INTERRUPT_REQUEST_OUT5IRQ_SIZE 1

/** bit field defines for usbhs_s#usb_interrupt_request */
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SUDAVIR_OFFSET 0
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SUDAVIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SOFIR_OFFSET 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SOFIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SUTOKIR_OFFSET 2
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SUTOKIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SUSPIR_OFFSET 3
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_SUSPIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_URESIR_OFFSET 4
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_URESIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_HSPEEDIR_OFFSET 5
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_HSPEEDIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_LPMIR_OFFSET 7
#define USBHS_USB_INTERRUPT_REQUEST_USBIRQ_LPMIR_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_EXTIRQ_WAKEUPIRQ_OFFSET 15
#define USBHS_USB_INTERRUPT_REQUEST_EXTIRQ_WAKEUPIRQ_SIZE 1
#define USBHS_USB_INTERRUPT_REQUEST_OUT0PNGIRQ_OFFSET 16
#define USBHS_USB_INTERRUPT_REQUEST_OUT0PNGIRQ_SIZE 1

/** bit field defines for usbhs_s#fifo_interrupt_request */
#define USBHS_FIFO_INTERRUPT_REQUEST_IN1FULLIRQ_OFFSET 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN1FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN2FULLIRQ_OFFSET 2
#define USBHS_FIFO_INTERRUPT_REQUEST_IN2FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN3FULLIRQ_OFFSET 3
#define USBHS_FIFO_INTERRUPT_REQUEST_IN3FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN4FULLIRQ_OFFSET 4
#define USBHS_FIFO_INTERRUPT_REQUEST_IN4FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN5FULLIRQ_OFFSET 5
#define USBHS_FIFO_INTERRUPT_REQUEST_IN5FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN6FULLIRQ_OFFSET 6
#define USBHS_FIFO_INTERRUPT_REQUEST_IN6FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN7FULLIRQ_OFFSET 7
#define USBHS_FIFO_INTERRUPT_REQUEST_IN7FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN8FULLIRQ_OFFSET 8
#define USBHS_FIFO_INTERRUPT_REQUEST_IN8FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_IN9FULLIRQ_OFFSET 9
#define USBHS_FIFO_INTERRUPT_REQUEST_IN9FULLIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT1EMPTIRQ_OFFSET 17
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT1EMPTIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT2EMPTIRQ_OFFSET 18
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT2EMPTIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT3EMPTIRQ_OFFSET 19
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT3EMPTIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT4EMPTIRQ_OFFSET 20
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT4EMPTIRQ_SIZE 1
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT5EMPTIRQ_OFFSET 21
#define USBHS_FIFO_INTERRUPT_REQUEST_OUT5EMPTIRQ_SIZE 1

/** bit field defines for usbhs_s#endpoint_interrupt_enable */
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN0IEN_OFFSET 0
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN0IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN1IEN_OFFSET 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN1IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN2IEN_OFFSET 2
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN2IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN3IEN_OFFSET 3
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN3IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN4IEN_OFFSET 4
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN4IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN5IEN_OFFSET 5
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN5IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN6IEN_OFFSET 6
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN6IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN7IEN_OFFSET 7
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN7IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN8IEN_OFFSET 8
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_IN8IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT0IEN_OFFSET 16
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT0IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT1IEN_OFFSET 17
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT1IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT2IEN_OFFSET 18
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT2IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT3IEN_OFFSET 19
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT3IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT4IEN_OFFSET 20
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT4IEN_SIZE 1
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT5IEN_OFFSET 21
#define USBHS_ENDPOINT_INTERRUPT_ENABLE_OUT5IEN_SIZE 1

/** bit field defines for usbhs_s#usb_interrupt_enable */
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SUDAVIR_OFFSET 0
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SUDAVIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SOFIR_OFFSET 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SOFIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SUTOKIR_OFFSET 2
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SUTOKIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SUSPIR_OFFSET 3
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_SUSPIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_URESIR_OFFSET 4
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_URESIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_HSPEEDIR_OFFSET 5
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_HSPEEDIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_LPMIR_OFFSET 7
#define USBHS_USB_INTERRUPT_ENABLE_USBIEN_LPMIR_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_EXTIEN_WAKEUPIEN_OFFSET 15
#define USBHS_USB_INTERRUPT_ENABLE_EXTIEN_WAKEUPIEN_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_OUT0PNGIEN_OFFSET 16
#define USBHS_USB_INTERRUPT_ENABLE_OUT0PNGIEN_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_OUT1PNGIEN_OFFSET 17
#define USBHS_USB_INTERRUPT_ENABLE_OUT1PNGIEN_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_OUT2PNGIEN_OFFSET 18
#define USBHS_USB_INTERRUPT_ENABLE_OUT2PNGIEN_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_OUT3PNGIEN_OFFSET 19
#define USBHS_USB_INTERRUPT_ENABLE_OUT3PNGIEN_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_OUT4PNGIEN_OFFSET 20
#define USBHS_USB_INTERRUPT_ENABLE_OUT4PNGIEN_SIZE 1
#define USBHS_USB_INTERRUPT_ENABLE_OUT5PNGIEN_OFFSET 21
#define USBHS_USB_INTERRUPT_ENABLE_OUT5PNGIEN_SIZE 1

/** bit field defines for usbhs_s#fifo_interrupt_enable */
#define USBHS_FIFO_INTERRUPT_ENABLE_IN1FULLIEN_OFFSET 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN1FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN2FULLIEN_OFFSET 2
#define USBHS_FIFO_INTERRUPT_ENABLE_IN2FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN3FULLIEN_OFFSET 3
#define USBHS_FIFO_INTERRUPT_ENABLE_IN3FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN4FULLIEN_OFFSET 4
#define USBHS_FIFO_INTERRUPT_ENABLE_IN4FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN5FULLIEN_OFFSET 5
#define USBHS_FIFO_INTERRUPT_ENABLE_IN5FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN6FULLIEN_OFFSET 6
#define USBHS_FIFO_INTERRUPT_ENABLE_IN6FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN7FULLIEN_OFFSET 7
#define USBHS_FIFO_INTERRUPT_ENABLE_IN7FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_IN8FULLIEN_OFFSET 8
#define USBHS_FIFO_INTERRUPT_ENABLE_IN8FULLIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT1EMPTIEN_OFFSET 17
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT1EMPTIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT2EMPTIEN_OFFSET 18
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT2EMPTIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT3EMPTIEN_OFFSET 19
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT3EMPTIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT4EMPTIEN_OFFSET 20
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT4EMPTIEN_SIZE 1
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT5EMPTIEN_OFFSET 21
#define USBHS_FIFO_INTERRUPT_ENABLE_OUT5EMPTIEN_SIZE 1

/** bit field defines for usbhs_s#interrupt_vector_and_reset */
#define USBHS_INTERRUPT_VECTOR_AND_RESET_IVECT_OFFSET 0
#define USBHS_INTERRUPT_VECTOR_AND_RESET_IVECT_SIZE 8
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_EP_OFFSET 16
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_EP_SIZE 4
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_IO_OFFSET 20
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_IO_SIZE 1
/** Io=1 -- IN endpoint selected */
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_IO_IN_VALUE 1
/** Io=0 -- OUT endpoint selected */
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_IO_OUT_VALUE 0
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_TOGRST_OFFSET 21
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_TOGRST_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_FIFORST_OFFSET 22
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_FIFORST_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_TOGSETQ_OFFSET 23
#define USBHS_INTERRUPT_VECTOR_AND_RESET_ENDPRST_TOGSETQ_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_LPMNYET_OFFSET 25
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_LPMNYET_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_SLFPWR_OFFSET 26
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_SLFPWR_SIZE 1
/** slfpwr=0 - device is bus powered */
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_SLFPWR_BP_VALUE 0
/** slfpwr=1 - device is self powered */
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_SLFPWR_SP_VALUE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_RWAKEN_OFFSET 27
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_RWAKEN_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_ENUM_OFFSET 28
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_ENUM_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_SIGRSUME_OFFSET 29
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_SIGRSUME_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_DISCON_OFFSET 30
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_DISCON_SIZE 1
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_WAKESRC_OFFSET 31
#define USBHS_INTERRUPT_VECTOR_AND_RESET_USBCS_WAKESRC_SIZE 1

/** bit field defines for usbhs_s#usb_csr */
#define USBHS_USB_CSR_MFRMNR_OFFSET 0
#define USBHS_USB_CSR_MFRMNR_SIZE 3
#define USBHS_USB_CSR_FRMNR_OFFSET 3
#define USBHS_USB_CSR_FRMNR_SIZE 11
#define USBHS_USB_CSR_FNADDR_OFFSET 16
#define USBHS_USB_CSR_FNADDR_SIZE 7

/** bit field defines for usbhs_s#fifoctrl */
#define USBHS_FIFOCTRL_FIFOCTRL_EP_OFFSET 0
#define USBHS_FIFOCTRL_FIFOCTRL_EP_SIZE 4
#define USBHS_FIFOCTRL_FIFOCTRL_IO_OFFSET 4
#define USBHS_FIFOCTRL_FIFOCTRL_IO_SIZE 1
#define USBHS_FIFOCTRL_FIFOCTRL_FIFOAUTO_OFFSET 5
#define USBHS_FIFOCTRL_FIFOCTRL_FIFOAUTO_SIZE 1
#define USBHS_FIFOCTRL_FIFOCTRL_FIFOCMIT_OFFSET 6
#define USBHS_FIFOCTRL_FIFOCTRL_FIFOCMIT_SIZE 1
#define USBHS_FIFOCTRL_FIFOCTRL_FIFOACC_OFFSET 7
#define USBHS_FIFOCTRL_FIFOCTRL_FIFOACC_SIZE 1
#define USBHS_FIFOCTRL_SPEEDCTRL_FS_OFFSET 9
#define USBHS_FIFOCTRL_SPEEDCTRL_FS_SIZE 1
#define USBHS_FIFOCTRL_SPEEDCTRL_HS_OFFSET 10
#define USBHS_FIFOCTRL_SPEEDCTRL_HS_SIZE 1
#define USBHS_FIFOCTRL_SPEEDCTRL_HSDISABLE_OFFSET 15
#define USBHS_FIFOCTRL_SPEEDCTRL_HSDISABLE_SIZE 1
#define USBHS_FIFOCTRL_SLEEPCLKGATE_OFFSET 16
#define USBHS_FIFOCTRL_SLEEPCLKGATE_SIZE 8

/** bit field defines for usbhs_s#outmaxpck_1_0 */
#define USBHS_OUTMAXPCK_1_0_OUT0MAXPCK_OFFSET 0
#define USBHS_OUTMAXPCK_1_0_OUT0MAXPCK_SIZE 7
#define USBHS_OUTMAXPCK_1_0_OUT1MAXPCK_OFFSET 16
#define USBHS_OUTMAXPCK_1_0_OUT1MAXPCK_SIZE 11

/** bit field defines for usbhs_s#outmaxpck_3_2 */
#define USBHS_OUTMAXPCK_3_2_OUT2MAXPCK_OFFSET 0
#define USBHS_OUTMAXPCK_3_2_OUT2MAXPCK_SIZE 11
#define USBHS_OUTMAXPCK_3_2_OUT3MAXPCK_OFFSET 16
#define USBHS_OUTMAXPCK_3_2_OUT3MAXPCK_SIZE 11

/** bit field defines for usbhs_s#outmaxpck_5_4 */
#define USBHS_OUTMAXPCK_5_4_OUT4MAXPCK_OFFSET 0
#define USBHS_OUTMAXPCK_5_4_OUT4MAXPCK_SIZE 11
#define USBHS_OUTMAXPCK_5_4_OUT5MAXPCK_OFFSET 16
#define USBHS_OUTMAXPCK_5_4_OUT5MAXPCK_SIZE 11

/** bit field defines for usbhs_s#out1staddr */
#define USBHS_OUT1STADDR_OUT1STADDR_OFFSET 2
#define USBHS_OUT1STADDR_OUT1STADDR_SIZE 14

/** bit field defines for usbhs_s#out2staddr */
#define USBHS_OUT2STADDR_OUT2STADDR_OFFSET 2
#define USBHS_OUT2STADDR_OUT2STADDR_SIZE 14

/** bit field defines for usbhs_s#out3staddr */
#define USBHS_OUT3STADDR_OUT3STADDR_OFFSET 2
#define USBHS_OUT3STADDR_OUT3STADDR_SIZE 14

/** bit field defines for usbhs_s#out4staddr */
#define USBHS_OUT4STADDR_OUT4STADDR_OFFSET 2
#define USBHS_OUT4STADDR_OUT4STADDR_SIZE 14

/** bit field defines for usbhs_s#out5staddr */
#define USBHS_OUT5STADDR_OUT5STADDR_OFFSET 2
#define USBHS_OUT5STADDR_OUT5STADDR_SIZE 14

/** bit field defines for usbhs_s#in1staddr */
#define USBHS_IN1STADDR_IN1STADDR_OFFSET 2
#define USBHS_IN1STADDR_IN1STADDR_SIZE 14

/** bit field defines for usbhs_s#in2staddr */
#define USBHS_IN2STADDR_IN2STADDR_OFFSET 2
#define USBHS_IN2STADDR_IN2STADDR_SIZE 14

/** bit field defines for usbhs_s#in3staddr */
#define USBHS_IN3STADDR_IN3STADDR_OFFSET 2
#define USBHS_IN3STADDR_IN3STADDR_SIZE 14

/** bit field defines for usbhs_s#in4staddr */
#define USBHS_IN4STADDR_IN4STADDR_OFFSET 2
#define USBHS_IN4STADDR_IN4STADDR_SIZE 14

/** bit field defines for usbhs_s#in5staddr */
#define USBHS_IN5STADDR_IN5STADDR_OFFSET 2
#define USBHS_IN5STADDR_IN5STADDR_SIZE 14

/** bit field defines for usbhs_s#in6staddr */
#define USBHS_IN6STADDR_IN6STADDR_OFFSET 2
#define USBHS_IN6STADDR_IN6STADDR_SIZE 14

/** bit field defines for usbhs_s#in7staddr */
#define USBHS_IN7STADDR_IN7STADDR_OFFSET 2
#define USBHS_IN7STADDR_IN7STADDR_SIZE 14

/** bit field defines for usbhs_s#in8staddr */
#define USBHS_IN8STADDR_IN8STADDR_OFFSET 2
#define USBHS_IN8STADDR_IN8STADDR_SIZE 14

/** bit field defines for usbhs_s#cpuctrl2 */
#define USBHS_CPUCTRL2_NONE_OFFSET 0
#define USBHS_CPUCTRL2_NONE_SIZE 1

/** bit field defines for usbhs_s#endian_sfr */
#define USBHS_ENDIAN_SFR_ENDIAN_SFR_CS_OFFSET 7
#define USBHS_ENDIAN_SFR_ENDIAN_SFR_CS_SIZE 1
#define USBHS_ENDIAN_SFR_ENDIAN_SFR_S_OFFSET 31
#define USBHS_ENDIAN_SFR_ENDIAN_SFR_S_SIZE 1

/** bit field defines for usbhs_s#inmaxpck_1 */
#define USBHS_INMAXPCK_1_IN1MAXPCK_OFFSET 16
#define USBHS_INMAXPCK_1_IN1MAXPCK_SIZE 11

/** bit field defines for usbhs_s#inmaxpck_3_2 */
#define USBHS_INMAXPCK_3_2_IN2MAXPCK_OFFSET 0
#define USBHS_INMAXPCK_3_2_IN2MAXPCK_SIZE 11
#define USBHS_INMAXPCK_3_2_IN3MAXPCK_OFFSET 16
#define USBHS_INMAXPCK_3_2_IN3MAXPCK_SIZE 11

/** bit field defines for usbhs_s#inmaxpck_5_4 */
#define USBHS_INMAXPCK_5_4_IN4MAXPCK_OFFSET 0
#define USBHS_INMAXPCK_5_4_IN4MAXPCK_SIZE 11
#define USBHS_INMAXPCK_5_4_IN5MAXPCK_OFFSET 16
#define USBHS_INMAXPCK_5_4_IN5MAXPCK_SIZE 11

/** bit field defines for usbhs_s#inmaxpck_7_6 */
#define USBHS_INMAXPCK_7_6_IN6MAXPCK_OFFSET 0
#define USBHS_INMAXPCK_7_6_IN6MAXPCK_SIZE 11
#define USBHS_INMAXPCK_7_6_IN7MAXPCK_OFFSET 16
#define USBHS_INMAXPCK_7_6_IN7MAXPCK_SIZE 11

/** bit field defines for usbhs_s#inmaxpck_9_8 */
#define USBHS_INMAXPCK_9_8_IN8MAXPCK_OFFSET 0
#define USBHS_INMAXPCK_9_8_IN8MAXPCK_SIZE 11

/** bit field defines for usbhs_s#dma_conf */
#define USBHS_DMA_CONF_DMA_CONF_CFGRST_OFFSET 0
#define USBHS_DMA_CONF_DMA_CONF_CFGRST_SIZE 1
#define USBHS_DMA_CONF_DMA_CONF_DSING_OFFSET 8
#define USBHS_DMA_CONF_DMA_CONF_DSING_SIZE 1
#define USBHS_DMA_CONF_DMA_CONF_DMULT_OFFSET 9
#define USBHS_DMA_CONF_DMA_CONF_DMULT_SIZE 1

/** bit field defines for usbhs_s#dma_sts */
#define USBHS_DMA_STS_DTRANS_OFFSET 3
#define USBHS_DMA_STS_DTRANS_SIZE 1

/** bit field defines for usbhs_s#dma_ep_sel */
#define USBHS_DMA_EP_SEL_EPNO_OFFSET 0
#define USBHS_DMA_EP_SEL_EPNO_SIZE 4
#define USBHS_DMA_EP_SEL_DIR_OFFSET 7
#define USBHS_DMA_EP_SEL_DIR_SIZE 1

/** bit field defines for usbhs_s#dma_ep_traddr */
#define USBHS_DMA_EP_TRADDR_TRADDR_OFFSET 0
#define USBHS_DMA_EP_TRADDR_TRADDR_SIZE 32

/** bit field defines for usbhs_s#ep_cfg */
#define USBHS_EP_CFG_ENABLE_OFFSET 0
#define USBHS_EP_CFG_ENABLE_SIZE 1
#define USBHS_EP_CFG_EPENDIAN_OFFSET 7
#define USBHS_EP_CFG_EPENDIAN_SIZE 1
#define USBHS_EP_CFG_DSING_OFFSET 12
#define USBHS_EP_CFG_DSING_SIZE 1
#define USBHS_EP_CFG_DMULT_OFFSET 13
#define USBHS_EP_CFG_DMULT_SIZE 1

/** bit field defines for usbhs_s#dma_ep_cmd */
#define USBHS_DMA_EP_CMD_EPRST_OFFSET 0
#define USBHS_DMA_EP_CMD_EPRST_SIZE 1
#define USBHS_DMA_EP_CMD_DRDY_OFFSET 6
#define USBHS_DMA_EP_CMD_DRDY_SIZE 1
#define USBHS_DMA_EP_CMD_DFLUSH_OFFSET 7
#define USBHS_DMA_EP_CMD_DFLUSH_SIZE 1

/** bit field defines for usbhs_s#dma_ep_sts */
#define USBHS_DMA_EP_STS_IOC_OFFSET 2
#define USBHS_DMA_EP_STS_IOC_SIZE 1
#define USBHS_DMA_EP_STS_ISP_OFFSET 3
#define USBHS_DMA_EP_STS_ISP_SIZE 1
#define USBHS_DMA_EP_STS_DESCMIS_OFFSET 4
#define USBHS_DMA_EP_STS_DESCMIS_SIZE 1
#define USBHS_DMA_EP_STS_TRBERR_OFFSET 7
#define USBHS_DMA_EP_STS_TRBERR_SIZE 1
#define USBHS_DMA_EP_STS_DBUSY_OFFSET 9
#define USBHS_DMA_EP_STS_DBUSY_SIZE 1
#define USBHS_DMA_EP_STS_CCS_OFFSET 11
#define USBHS_DMA_EP_STS_CCS_SIZE 1
#define USBHS_DMA_EP_STS_OUTSMM_OFFSET 14
#define USBHS_DMA_EP_STS_OUTSMM_SIZE 1
#define USBHS_DMA_EP_STS_ISOERR_OFFSET 15
#define USBHS_DMA_EP_STS_ISOERR_SIZE 1
#define USBHS_DMA_EP_STS_IDTRANS_OFFSET 31
#define USBHS_DMA_EP_STS_IDTRANS_SIZE 1

/** bit field defines for usbhs_s#dma_ep_sts_en */
#define USBHS_DMA_EP_STS_EN_DESCMISEN_OFFSET 5
#define USBHS_DMA_EP_STS_EN_DESCMISEN_SIZE 1
#define USBHS_DMA_EP_STS_EN_TRBERREN_OFFSET 7
#define USBHS_DMA_EP_STS_EN_TRBERREN_SIZE 1
#define USBHS_DMA_EP_STS_EN_OUTSMMEN_OFFSET 14
#define USBHS_DMA_EP_STS_EN_OUTSMMEN_SIZE 1
#define USBHS_DMA_EP_STS_EN_ISOERREN_OFFSET 15
#define USBHS_DMA_EP_STS_EN_ISOERREN_SIZE 1

/** bit field defines for usbhs_s#dma_drbl */
#define USBHS_DMA_DRBL_DRBLNO_OFFSET 0
#define USBHS_DMA_DRBL_DRBLNO_SIZE 16
#define USBHS_DMA_DRBL_DRBLNI_OFFSET 16
#define USBHS_DMA_DRBL_DRBLNI_SIZE 16

/** bit field defines for usbhs_s#dma_ep_ien */
#define USBHS_DMA_EP_IEN_EOUT_OFFSET 0
#define USBHS_DMA_EP_IEN_EOUT_SIZE 16
#define USBHS_DMA_EP_IEN_EIN_OFFSET 16
#define USBHS_DMA_EP_IEN_EIN_SIZE 16

/** bit field defines for usbhs_s#dma_ep_ists */
#define USBHS_DMA_EP_ISTS_EOUT_OFFSET 0
#define USBHS_DMA_EP_ISTS_EOUT_SIZE 16
#define USBHS_DMA_EP_ISTS_EIN_OFFSET 16
#define USBHS_DMA_EP_ISTS_EIN_SIZE 16

/** bit field defines for usbhs_s#dma_axim_ctrl */
#define USBHS_DMA_AXIM_CTRL_MAWPROT_OFFSET 0
#define USBHS_DMA_AXIM_CTRL_MAWPROT_SIZE 4
#define USBHS_DMA_AXIM_CTRL_MAWCACHE_OFFSET 4
#define USBHS_DMA_AXIM_CTRL_MAWCACHE_SIZE 4
#define USBHS_DMA_AXIM_CTRL_MAWLOCK_OFFSET 8
#define USBHS_DMA_AXIM_CTRL_MAWLOCK_SIZE 2
#define USBHS_DMA_AXIM_CTRL_MARPROT_OFFSET 16
#define USBHS_DMA_AXIM_CTRL_MARPROT_SIZE 4
#define USBHS_DMA_AXIM_CTRL_MARCACHE_OFFSET 20
#define USBHS_DMA_AXIM_CTRL_MARCACHE_SIZE 4
#define USBHS_DMA_AXIM_CTRL_MARLCOCK_OFFSET 24
#define USBHS_DMA_AXIM_CTRL_MARLCOCK_SIZE 2

/** bit field defines for usbhs_s#dma_axim_id */
#define USBHS_DMA_AXIM_ID_MAW_ID_OFFSET 0
#define USBHS_DMA_AXIM_ID_MAW_ID_SIZE 16
#define USBHS_DMA_AXIM_ID_MAR_ID_OFFSET 16
#define USBHS_DMA_AXIM_ID_MAR_ID_SIZE 16

/** bit field defines for usbhs_s#dma_axim_cap */
#define USBHS_DMA_AXIM_CAP_AXI_DECERR_EN_OFFSET 20
#define USBHS_DMA_AXIM_CAP_AXI_DECERR_EN_SIZE 1
#define USBHS_DMA_AXIM_CAP_AXI_SLVERR_EN_OFFSET 21
#define USBHS_DMA_AXIM_CAP_AXI_SLVERR_EN_SIZE 1
#define USBHS_DMA_AXIM_CAP_AXI_DECERR_OFFSET 28
#define USBHS_DMA_AXIM_CAP_AXI_DECERR_SIZE 1
#define USBHS_DMA_AXIM_CAP_AXI_SLVERR_OFFSET 29
#define USBHS_DMA_AXIM_CAP_AXI_SLVERR_SIZE 1
#define USBHS_DMA_AXIM_CAP_AXI_IDLE_OFFSET 30
#define USBHS_DMA_AXIM_CAP_AXI_IDLE_SIZE 1

/** bit field defines for usbhs_s#dma_axim_ctrl0 */
#define USBHS_DMA_AXIM_CTRL0_B_MAX_OFFSET 0
#define USBHS_DMA_AXIM_CTRL0_B_MAX_SIZE 8

/** bit field defines for usbhs_s#dma_axim_ctrl1 */
#define USBHS_DMA_AXIM_CTRL1_ROT_OFFSET 0
#define USBHS_DMA_AXIM_CTRL1_ROT_SIZE 8
#define USBHS_DMA_AXIM_CTRL1_WOT_OFFSET 16
#define USBHS_DMA_AXIM_CTRL1_WOT_SIZE 8

/* EOF usbhs.h */
#endif
