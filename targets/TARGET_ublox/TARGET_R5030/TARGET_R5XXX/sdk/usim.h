#ifndef INCLUDED_USIM
#define INCLUDED_USIM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Smart Card Reader control registers.
*/
struct usim_s {
   /** This is a 16-bit, read/write register that provides the control of the Smart Card Reader interface. at address offset 0x000, read-write */
   uint32_t ctrl1;
   /** This is a 16-bit, read/write register that provides the control of the Smart Card Reader interface. at address offset 0x004, read-write */
   uint32_t ctrl2;
   /** This is a 16-bit, read/write register that provides direct access to Smart Card pads without serial interface assistance. You can use this register feature with synchronous and any other non-ISO 7816 and non-EMV cards. at address offset 0x008, read-write */
   uint32_t scpads;
   /** This is a 16-bit read/write register that enables assertion of each interrupt bit individually in the Interrupt Status Register. at address offset 0x00C, read-write */
   uint32_t inten1;
   /** This 16-bit read/clear register provides information about the state of each interrupt bit. You can clear the register bits individually by writing 1 to a bit you intend to clear. If the global interrupt is enabled in the GINTEN bit in Control Register 1, then each interrupt bit assertion in the Interrupt Status Register invokes the global interrupt. at address offset 0x010, read-write */
   uint32_t intstat1;
   /** This is a 16-bit read/write register that provides the control and status of RX and TX FIFO. at address offset 0x014, read-write */
   uint32_t fifoctrl;
   /** This read-only register consists of two 8-bit values that provide the lower part of TX/RX FIFO Counters. They are equal to TX/RX FIFO Counters up to value 255. All values above 255 are read as 255. It is recommended to use the 16-bit TX/RX FIFO Counters instead of this register. at address offset 0x018, read-write */
   uint32_t legacyfifocnt;
   /** This is a 4-bit, read/write register that sets the interrupt threshold. The interrupt is asserted when the number of bytes it receives is equal to, or exceeds the threshold (Width of this register is given by FIFODEPTH local parameter - default value is 4). at address offset 0x01C, read-write */
   uint32_t rxfifothres;
   /** TX/RX Repeat at address offset 0x020, read-write */
   uint32_t txrxrept;
   /** This is a 16-bit, read/write register that defines the divisor value used to generate the Smart Card Clock from the system clock. at address offset 0x024, read-write */
   uint32_t sccdiv;
   /** This is a 16-bit, read/write register that defines a divisor value used to generate the Baud Clock impulses from the system clock. at address offset 0x028, read-write */
   uint32_t bauddiv;
   /** This is an 8-bit, read/write register that sets a delay at the end of each character transmitted from the Smart Card Reader to the Smart Card. The value is in Elementary Time Units (ETU). The parity error is besides signaled during the guardtime. at address offset 0x02C, read-write */
   uint32_t scgrdt;
   /** This is a 16-bit, read/write register that sets the duration of each part of the activation and deactivation sequence. The value is in Smart Card Clock Cycles. Bits (7:0) of this register are hard-wired to zero. at address offset 0x030, read-write */
   uint32_t adeatime;
   /** This is a 16-bit read/write register that sets the duration of the smart card reset sequence. This value is same for the cold and warm reset. The value is in terms of smart card clock cycles. Bits (7:0) of this register are hard-wired to zero. at address offset 0x034, read-write */
   uint32_t lowrsttime;
   /** This is a 16-bit read/write register that defines the maximum time between the rising edge of the SCRSTN signal and the start of ATR response. The value is in terms of smart card clock cycles. Bits (7:0) of this register are hard-wired to zero. at address offset 0x038, read-write */
   uint32_t atrstartlimit;
   /** This is a 16-bit, read/write register that sets the maximum time between the leading edges of two, consecutive characters. Sets LSBs c2clim 15:0. See also c2climhi for c2clim 23:16. at address offset 0x03C, read-write */
   uint32_t c2clim;
   /** This is a 2-bit read/write register that enables assertion of each interrupt bit individually in the Interrupt Status Register. at address offset 0x040, read-write */
   uint32_t inten2;
   /** This 2-bit read/clear register provides information about the state of each interrupt bit. You can clear the register bits individually by writing 1 to a bit you intend to clear. If the global interrupt is enabled in the GINTEN bit in Control Register 1, then each interrupt bit assertion in the Interrupt Status Register invokes the global interrupt. at address offset 0x044, read-write */
   uint32_t intstat2;
   /** This is a 4-bit, read/write register that sets the interrupt threshold. The interrupt is asserted when the number of bytes in TX FIFO is equal or less than the threshold (Width of this register is given by FIFODEPTH local parameter - default value is 4). at address offset 0x048, read-write */
   uint32_t txfifothres;
   /** This is a 16-bit, read-only register that provides the number of bytes stored in the TX FIFO. at address offset 0x04C, read-write */
   uint32_t txfifocnt;
   /** This is a 16-bit, read-only register that provides the number of bytes stored in the RX FIFO. at address offset 0x050, read-write */
   uint32_t rxfifocnt;
   /** Reserved space */
   uint8_t fill0[4];
   /** This is a 16-bit, read/write register that sets the maximum time between the leading edges of two, consecutive characters. Sets MSBs c2clim[23:16]. See also c2clim for c2clim[15:0] at address offset 0x058, read-write */
   uint32_t c2climhi;
   /** Reserved space */
   uint8_t fill1[420];
   /** This is an 8-bit, read/write register that provides access to the receive and transmit FIFO buffers. The TX FIFO is accessed during the APB write transfer. The RX FIFO is accessed during the APB read transfer. All read/write accesses at address range 200h-3ffh are redirected to the FIFO. at address offset 0x200, read-write */
   uint32_t fifo[64];
};

/** bit field defines for usim_s#ctrl1 */
#define USIM_CTRL1_INVLEV_OFFSET 0
#define USIM_CTRL1_INVLEV_SIZE 1
#define USIM_CTRL1_INVORD_OFFSET 1
#define USIM_CTRL1_INVORD_SIZE 1
#define USIM_CTRL1_PECH2FIFO_OFFSET 2
#define USIM_CTRL1_PECH2FIFO_SIZE 1
#define USIM_CTRL1_CLKSTOP_OFFSET 6
#define USIM_CTRL1_CLKSTOP_SIZE 1
#define USIM_CTRL1_CLKSTOPVAL_OFFSET 7
#define USIM_CTRL1_CLKSTOPVAL_SIZE 1
#define USIM_CTRL1_TXEN_OFFSET 8
#define USIM_CTRL1_TXEN_SIZE 1
#define USIM_CTRL1_RXEN_OFFSET 9
#define USIM_CTRL1_RXEN_SIZE 1
#define USIM_CTRL1_TS2FIFO_OFFSET 10
#define USIM_CTRL1_TS2FIFO_SIZE 1
#define USIM_CTRL1_T0T1_OFFSET 11
#define USIM_CTRL1_T0T1_SIZE 1
#define USIM_CTRL1_ATRSTFLUSH_OFFSET 12
#define USIM_CTRL1_ATRSTFLUSH_SIZE 1
#define USIM_CTRL1_TCKEN_OFFSET 13
#define USIM_CTRL1_TCKEN_SIZE 1
#define USIM_CTRL1_RESERVED_OFFSET 14
#define USIM_CTRL1_RESERVED_SIZE 1
#define USIM_CTRL1_GINTEN_OFFSET 15
#define USIM_CTRL1_GINTEN_SIZE 1

/** bit field defines for usim_s#ctrl2 */
#define USIM_CTRL2_WARMRST_OFFSET 2
#define USIM_CTRL2_WARMRST_SIZE 1
#define USIM_CTRL2_ACT_OFFSET 3
#define USIM_CTRL2_ACT_SIZE 1
#define USIM_CTRL2_DEACT_OFFSET 4
#define USIM_CTRL2_DEACT_SIZE 1
#define USIM_CTRL2_VCC18_OFFSET 5
#define USIM_CTRL2_VCC18_SIZE 1
#define USIM_CTRL2_VCC33_OFFSET 6
#define USIM_CTRL2_VCC33_SIZE 1
#define USIM_CTRL2_VCC50_OFFSET 7
#define USIM_CTRL2_VCC50_SIZE 1
#define USIM_CTRL2_UBX_FAST_ACTIVATE_OFFSET 8
#define USIM_CTRL2_UBX_FAST_ACTIVATE_SIZE 1
#define USIM_CTRL2_RESERVED_OFFSET 9
#define USIM_CTRL2_RESERVED_SIZE 7

/** bit field defines for usim_s#scpads */
#define USIM_SCPADS_DIRACCPADS_OFFSET 0
#define USIM_SCPADS_DIRACCPADS_SIZE 1
#define USIM_SCPADS_DSCIO_OFFSET 1
#define USIM_SCPADS_DSCIO_SIZE 1
#define USIM_SCPADS_DSCCLK_OFFSET 2
#define USIM_SCPADS_DSCCLK_SIZE 1
#define USIM_SCPADS_DSCRST_OFFSET 3
#define USIM_SCPADS_DSCRST_SIZE 1
#define USIM_SCPADS_DSCVCC_OFFSET 4
#define USIM_SCPADS_DSCVCC_SIZE 1
#define USIM_SCPADS_AUTOADEAVPP_OFFSET 5
#define USIM_SCPADS_AUTOADEAVPP_SIZE 1
#define USIM_SCPADS_DSCVPPEN_OFFSET 6
#define USIM_SCPADS_DSCVPPEN_SIZE 1
#define USIM_SCPADS_DSCVPPPP_OFFSET 7
#define USIM_SCPADS_DSCVPPPP_SIZE 1
#define USIM_SCPADS_DSCFCB_OFFSET 8
#define USIM_SCPADS_DSCFCB_SIZE 1
#define USIM_SCPADS_SCPRESENT_OFFSET 9
#define USIM_SCPADS_SCPRESENT_SIZE 1
#define USIM_SCPADS_RESERVED_OFFSET 10
#define USIM_SCPADS_RESERVED_SIZE 6

/** bit field defines for usim_s#inten1 */
#define USIM_INTEN1_TXFIDONE_OFFSET 0
#define USIM_INTEN1_TXFIDONE_SIZE 1
#define USIM_INTEN1_TXFIEMPTY_OFFSET 1
#define USIM_INTEN1_TXFIEMPTY_SIZE 1
#define USIM_INTEN1_RXFIFULL_OFFSET 2
#define USIM_INTEN1_RXFIFULL_SIZE 1
#define USIM_INTEN1_CLKSTOPRUN_OFFSET 3
#define USIM_INTEN1_CLKSTOPRUN_SIZE 1
#define USIM_INTEN1_TXDONE_OFFSET 4
#define USIM_INTEN1_TXDONE_SIZE 1
#define USIM_INTEN1_RXDONE_OFFSET 5
#define USIM_INTEN1_RXDONE_SIZE 1
#define USIM_INTEN1_TXPERR_OFFSET 6
#define USIM_INTEN1_TXPERR_SIZE 1
#define USIM_INTEN1_RXPERR_OFFSET 7
#define USIM_INTEN1_RXPERR_SIZE 1
#define USIM_INTEN1_C2CFULL_OFFSET 8
#define USIM_INTEN1_C2CFULL_SIZE 1
#define USIM_INTEN1_RXTHRESHOLD_OFFSET 9
#define USIM_INTEN1_RXTHRESHOLD_SIZE 1
#define USIM_INTEN1_ATRFAIL_OFFSET 10
#define USIM_INTEN1_ATRFAIL_SIZE 1
#define USIM_INTEN1_ATRDONE_OFFSET 11
#define USIM_INTEN1_ATRDONE_SIZE 1
#define USIM_INTEN1_SCREM_OFFSET 12
#define USIM_INTEN1_SCREM_SIZE 1
#define USIM_INTEN1_SCINS_OFFSET 13
#define USIM_INTEN1_SCINS_SIZE 1
#define USIM_INTEN1_SCACT_OFFSET 14
#define USIM_INTEN1_SCACT_SIZE 1
#define USIM_INTEN1_SCDEACT_OFFSET 15
#define USIM_INTEN1_SCDEACT_SIZE 1

/** bit field defines for usim_s#intstat1 */
#define USIM_INTSTAT1_TXFIDONE_OFFSET 0
#define USIM_INTSTAT1_TXFIDONE_SIZE 1
#define USIM_INTSTAT1_TXFIEMPTY_OFFSET 1
#define USIM_INTSTAT1_TXFIEMPTY_SIZE 1
#define USIM_INTSTAT1_RXFIFULL_OFFSET 2
#define USIM_INTSTAT1_RXFIFULL_SIZE 1
#define USIM_INTSTAT1_CLKSTOPRUN_OFFSET 3
#define USIM_INTSTAT1_CLKSTOPRUN_SIZE 1
#define USIM_INTSTAT1_TXDONE_OFFSET 4
#define USIM_INTSTAT1_TXDONE_SIZE 1
#define USIM_INTSTAT1_RXDONE_OFFSET 5
#define USIM_INTSTAT1_RXDONE_SIZE 1
#define USIM_INTSTAT1_TXPERR_OFFSET 6
#define USIM_INTSTAT1_TXPERR_SIZE 1
#define USIM_INTSTAT1_RXPERR_OFFSET 7
#define USIM_INTSTAT1_RXPERR_SIZE 1
#define USIM_INTSTAT1_C2CFULL_OFFSET 8
#define USIM_INTSTAT1_C2CFULL_SIZE 1
#define USIM_INTSTAT1_RXTHRESHOLD_OFFSET 9
#define USIM_INTSTAT1_RXTHRESHOLD_SIZE 1
#define USIM_INTSTAT1_ATRFAIL_OFFSET 10
#define USIM_INTSTAT1_ATRFAIL_SIZE 1
#define USIM_INTSTAT1_ATRDONE_OFFSET 11
#define USIM_INTSTAT1_ATRDONE_SIZE 1
#define USIM_INTSTAT1_SCREM_OFFSET 12
#define USIM_INTSTAT1_SCREM_SIZE 1
#define USIM_INTSTAT1_SCINS_OFFSET 13
#define USIM_INTSTAT1_SCINS_SIZE 1
#define USIM_INTSTAT1_SCACT_OFFSET 14
#define USIM_INTSTAT1_SCACT_SIZE 1
#define USIM_INTSTAT1_SCDEACT_OFFSET 15
#define USIM_INTSTAT1_SCDEACT_SIZE 1

/** bit field defines for usim_s#fifoctrl */
#define USIM_FIFOCTRL_TXFIEMPTY_OFFSET 0
#define USIM_FIFOCTRL_TXFIEMPTY_SIZE 1
#define USIM_FIFOCTRL_TXFIFULL_OFFSET 1
#define USIM_FIFOCTRL_TXFIFULL_SIZE 1
#define USIM_FIFOCTRL_TXFIFLUSH_OFFSET 2
#define USIM_FIFOCTRL_TXFIFLUSH_SIZE 1
#define USIM_FIFOCTRL_RXFIEMPTY_OFFSET 8
#define USIM_FIFOCTRL_RXFIEMPTY_SIZE 1
#define USIM_FIFOCTRL_RXFIFULL_OFFSET 9
#define USIM_FIFOCTRL_RXFIFULL_SIZE 1
#define USIM_FIFOCTRL_RXFIFLUSH_OFFSET 10
#define USIM_FIFOCTRL_RXFIFLUSH_SIZE 1
#define USIM_FIFOCTRL_RESERVED_OFFSET 11
#define USIM_FIFOCTRL_RESERVED_SIZE 5

/** bit field defines for usim_s#legacyfifocnt */
#define USIM_LEGACYFIFOCNT_TXFIFOCNT_OFFSET 0
#define USIM_LEGACYFIFOCNT_TXFIFOCNT_SIZE 8
#define USIM_LEGACYFIFOCNT_RXFIFOCNT_OFFSET 8
#define USIM_LEGACYFIFOCNT_RXFIFOCNT_SIZE 8

/** bit field defines for usim_s#rxfifothres */
#define USIM_RXFIFOTHRES_RXFIFOTHRES_OFFSET 0
#define USIM_RXFIFOTHRES_RXFIFOTHRES_SIZE 4

/** bit field defines for usim_s#txrxrept */
#define USIM_TXRXREPT_TXREPT_OFFSET 0
#define USIM_TXRXREPT_TXREPT_SIZE 4
#define USIM_TXRXREPT_RXREPT_OFFSET 4
#define USIM_TXRXREPT_RXREPT_SIZE 4

/** bit field defines for usim_s#sccdiv */
#define USIM_SCCDIV_SCCDIV_OFFSET 0
#define USIM_SCCDIV_SCCDIV_SIZE 16

/** bit field defines for usim_s#bauddiv */
#define USIM_BAUDDIV_BAUDDIV_OFFSET 0
#define USIM_BAUDDIV_BAUDDIV_SIZE 16

/** bit field defines for usim_s#scgrdt */
#define USIM_SCGRDT_BAUDDIV_OFFSET 0
#define USIM_SCGRDT_BAUDDIV_SIZE 8

/** bit field defines for usim_s#adeatime */
#define USIM_ADEATIME_RESERVED_OFFSET 0
#define USIM_ADEATIME_RESERVED_SIZE 8
#define USIM_ADEATIME_ADEATIME_OFFSET 8
#define USIM_ADEATIME_ADEATIME_SIZE 8

/** bit field defines for usim_s#lowrsttime */
#define USIM_LOWRSTTIME_RESERVED_OFFSET 0
#define USIM_LOWRSTTIME_RESERVED_SIZE 8
#define USIM_LOWRSTTIME_ADEATIME_OFFSET 8
#define USIM_LOWRSTTIME_ADEATIME_SIZE 8

/** bit field defines for usim_s#atrstartlimit */
#define USIM_ATRSTARTLIMIT_RESERVED_OFFSET 0
#define USIM_ATRSTARTLIMIT_RESERVED_SIZE 8
#define USIM_ATRSTARTLIMIT_ATRSTARTLIMIT_OFFSET 8
#define USIM_ATRSTARTLIMIT_ATRSTARTLIMIT_SIZE 8

/** bit field defines for usim_s#c2clim */
#define USIM_C2CLIM_ATRSTARTLIMIT_OFFSET 0
#define USIM_C2CLIM_ATRSTARTLIMIT_SIZE 16

/** bit field defines for usim_s#inten2 */
#define USIM_INTEN2_TXTHRESHOLD_OFFSET 0
#define USIM_INTEN2_TXTHRESHOLD_SIZE 2
#define USIM_INTEN2_RESERVED_OFFSET 2
#define USIM_INTEN2_RESERVED_SIZE 14

/** bit field defines for usim_s#intstat2 */
#define USIM_INTSTAT2_TXTHRESHOLD_OFFSET 0
#define USIM_INTSTAT2_TXTHRESHOLD_SIZE 2
#define USIM_INTSTAT2_RESERVED_OFFSET 2
#define USIM_INTSTAT2_RESERVED_SIZE 14

/** bit field defines for usim_s#txfifothres */
#define USIM_TXFIFOTHRES_TXFIFOTHRES_OFFSET 0
#define USIM_TXFIFOTHRES_TXFIFOTHRES_SIZE 4

/** bit field defines for usim_s#txfifocnt */
#define USIM_TXFIFOCNT_TXFIFOCNT_OFFSET 0
#define USIM_TXFIFOCNT_TXFIFOCNT_SIZE 16

/** bit field defines for usim_s#rxfifocnt */
#define USIM_RXFIFOCNT_RXFIFOCNT_OFFSET 0
#define USIM_RXFIFOCNT_RXFIFOCNT_SIZE 16

/** bit field defines for usim_s#c2climhi */
#define USIM_C2CLIMHI_ATRSTARTLIMIT_OFFSET 0
#define USIM_C2CLIMHI_ATRSTARTLIMIT_SIZE 8

/** bit field defines for usim_s#fifo */
#define USIM_FIFO_FIFO_OFFSET 0
#define USIM_FIFO_FIFO_SIZE 8
#define USIM_FIFO_RESERVED_OFFSET 8
#define USIM_FIFO_RESERVED_SIZE 8

/* EOF usim.h */
#endif
