#ifndef INCLUDED_SMURF_IF_HWACC_CONFIG
#define INCLUDED_SMURF_IF_HWACC_CONFIG
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

#define SMURF_IF_HWACC_CONFIG_PERIPHERALID4 0x07
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID5 0x00
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID6 0x00
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID7 0x00
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID0 0x65
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID1 0xDA
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID2 0x08
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID3 0x00
#define SMURF_IF_HWACC_CONFIG_COMPONENTID0 0x0D
#define SMURF_IF_HWACC_CONFIG_COMPONENTID1 0xF0
#define SMURF_IF_HWACC_CONFIG_COMPONENTID2 0x05
#define SMURF_IF_HWACC_CONFIG_COMPONENTID3 0xB1

/** Simple MUltichannel RF interface (SMURF).  The SMURF is the physical interface between the baseband and the transceiver and consists of 2 separate components that need to be used in pairs. This HAL contains extra registers specific for KM Modem HWAcc.
*/
struct smurf_if_hwacc_config_s {
   /** SMURF Control and Configuration Register at address offset 0x000, read-write */
   uint32_t ctrl;
   /** SMURF Control and Configuration Register at address offset 0x004, read-write */
   uint32_t ctrl_local;
   /** Reserved space */
   uint8_t fill0[120];
   /** PHY FIFO abort request at address offset 0x080, write-only */
   uint32_t phy_rxabort_req;
   /** PHY FIFO abort status at address offset 0x084, read-only */
   uint32_t phy_rxabort_stat;
   /** Reserved space */
   uint8_t fill1[120];
   /**  at address offset 0x100, write-only */
   uint32_t ofdm_demux_comd_0;
   /**  at address offset 0x104, read-only */
   uint32_t ofdm_demux_stat_0;
   /**  at address offset 0x108, write-only */
   uint32_t ofdm_demux_comd_1;
   /**  at address offset 0x10C, read-only */
   uint32_t ofdm_demux_stat_1;
   /** Reserved space */
   uint8_t fill2[112];
   /**  at address offset 0x180, write-only */
   uint32_t ofdm_demux_fltclear;
   /**  at address offset 0x184, read-only */
   uint32_t ofdm_demux_fltstatus;
   /**  at address offset 0x188, read-only */
   uint32_t ofdm_demux_fltrawstatus;
   /**  at address offset 0x18C, write-only */
   uint32_t ofdm_demux_fltmaskclr;
   /**  at address offset 0x190, write-only */
   uint32_t ofdm_demux_fltmaskset;
   /**  at address offset 0x194, read-only */
   uint32_t ofdm_demux_fltmaskstatus;
   /** Reserved space */
   uint8_t fill3[104];
   /**  at address offset 0x200, write-only */
   uint32_t pss_comd;
   /**  at address offset 0x204, read-only */
   uint32_t pss_stat;
   /** Reserved space */
   uint8_t fill4[120];
   /** Clear Fault Status at address offset 0x280, write-only */
   uint32_t pss_fltclear;
   /** Fault Status Register at address offset 0x284, read-only */
   uint32_t pss_fltstatus;
   /** Raw Fault Status Register at address offset 0x288, read-only */
   uint32_t pss_fltrawstatus;
   /** Fault Mask Clear at address offset 0x28C, write-only */
   uint32_t pss_fltmaskclr;
   /** Fault Mask Set at address offset 0x290, write-only */
   uint32_t pss_fltmaskset;
   /** Fault Mask Status at address offset 0x294, read-only */
   uint32_t pss_fltmaskstatus;
   /** Reserved space */
   uint8_t fill5[104];
   /**  at address offset 0x300, write-only */
   uint32_t sss_comd;
   /**  at address offset 0x304, read-only */
   uint32_t sss_stat;
   /** Reserved space */
   uint8_t fill6[120];
   /** Clear Fault Status at address offset 0x380, write-only */
   uint32_t sss_fltclear;
   /** Fault Status Register at address offset 0x384, read-only */
   uint32_t sss_fltstatus;
   /** Raw Fault Status Register at address offset 0x388, read-only */
   uint32_t sss_fltrawstatus;
   /** Fault Mask Clear at address offset 0x38C, write-only */
   uint32_t sss_fltmaskclr;
   /** Fault Mask Set at address offset 0x390, write-only */
   uint32_t sss_fltmaskset;
   /** Fault Mask Status at address offset 0x394, read-only */
   uint32_t sss_fltmaskstatus;
   /** Reserved space */
   uint8_t fill7[104];
   /** Circular buffer RFDMA access read client control at address offset 0x400, read-write */
   uint32_t fbuf_rfdma_rdptr;
   /** Reserved space */
   uint8_t fill8[252];
   /** 3-bit read-write register. Fabric bridge RFDMA oustanding read transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x500, read-write */
   uint32_t fabric_bridge_rfdma_outst_read_xactions;
   /** 3-bit read-write register. Fabric bridge ULBPE oustanding read transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x504, read-write */
   uint32_t fabric_bridge_ul_bpe_outst_read_xactions;
   /** 3-bit read-write register. Fabric bridge DLBPE oustanding read transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x508, read-write */
   uint32_t fabric_bridge_dl_bpe_outst_read_xactions;
   /** 3-bit read-write register. Fabric bridge PSS oustanding read transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x4 at address offset 0x50C, read-write */
   uint32_t fabric_bridge_pss_outst_read_xactions;
   /** 3-bit read-write register. Fabric bridge SSS oustanding read transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x510, read-write */
   uint32_t fabric_bridge_sss_outst_read_xactions;
   /** 3-bit read-write register. Fabric bridge RFDMA oustanding write transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x514, read-write */
   uint32_t fabric_bridge_rfdma_outst_write_xactions;
   /** 3-bit read-write register. Fabric bridge ULBPE oustanding write transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x518, read-write */
   uint32_t fabric_bridge_ul_bpe_outst_write_xactions;
   /** 3-bit read-write register. Fabric bridge DLBPE oustanding write transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x51C, read-write */
   uint32_t fabric_bridge_dl_bpe_outst_write_xactions;
   /** 3-bit read-write register. Fabric bridge OFDMDEMUX oustanding write transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x520, read-write */
   uint32_t fabric_bridge_ofdm_demux_outst_write_xactions;
   /** 3-bit read-write register. Fabric bridge PSS oustanding write transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x4 at address offset 0x524, read-write */
   uint32_t fabric_bridge_pss_outst_write_xactions;
   /** 3-bit read-write register. Fabric bridge SSS oustanding write transactions configuration. Only the following values are valid : 0x6 - 1 transaction, 0x4 - 3 transactions, 0x0 - 7 transactions. Default value 0x6 at address offset 0x528, read-write */
   uint32_t fabric_bridge_sss_outst_write_xactions;
   /** Reserved space */
   uint8_t fill9[212];
   /** FBUF Light-sleep controller recovery threshold at address offset 0x600, read-write */
   uint32_t fbuf_ls_ctrl_thresh;
   /** UL BPE Light-sleep controller recovery threshold at address offset 0x604, read-write */
   uint32_t ul_bpe_ls_ctrl_thresh;
   /** DL BPE Light-sleep controller recovery threshold at address offset 0x608, read-write */
   uint32_t dl_bpe_ls_ctrl_thresh;
   /** OFDM Demux Light-sleep controller recovery threshold at address offset 0x60C, read-write */
   uint32_t ofdm_demux_ls_ctrl_thresh;
   /** PSS Light-sleep controller recovery threshold at address offset 0x610, read-write */
   uint32_t pss_ls_ctrl_thresh;
   /** SSS Light-sleep controller recovery threshold at address offset 0x614, read-write */
   uint32_t sss_ls_ctrl_thresh;
   /** Reserved space */
   uint8_t fill10[2488];
   /** SMURF Data IF Peripheral ID4 Register at address offset 0xFD0, read-only constant 0x00000007 */
   uint32_t peripheralid4;
   /** SMURF Data IF Peripheral ID5 Register at address offset 0xFD4, read-only constant 0x00000000 */
   uint32_t peripheralid5;
   /** SMURF Data IF Peripheral ID6 Register at address offset 0xFD8, read-only constant 0x00000000 */
   uint32_t peripheralid6;
   /** SMURF Data IF Peripheral ID7 Register at address offset 0xFDC, read-only constant 0x00000000 */
   uint32_t peripheralid7;
   /** SMURF Data IF Peripheral ID0 Register at address offset 0xFE0, read-only constant 0x00000065 */
   uint32_t peripheralid0;
   /** SMURF Data IF Peripheral ID1 Register at address offset 0xFE4, read-only constant 0x000000DA */
   uint32_t peripheralid1;
   /** SMURF Data IF Peripheral ID2 Register at address offset 0xFE8, read-only constant 0x00000008 */
   uint32_t peripheralid2;
   /** SMURF Data IF Peripheral ID3 Register at address offset 0xFEC, read-only constant 0x00000000 */
   uint32_t peripheralid3;
   /** SMURF Data IF Component ID0 Register at address offset 0xFF0, read-only constant 0x0000000D */
   uint32_t componentid0;
   /** SMURF Data IF Component ID1 Register at address offset 0xFF4, read-only constant 0x000000F0 */
   uint32_t componentid1;
   /** SMURF Data IF Component ID2 Register at address offset 0xFF8, read-only constant 0x00000005 */
   uint32_t componentid2;
   /** SMURF Data IF Component ID3 Register at address offset 0xFFC, read-only constant 0x000000B1 */
   uint32_t componentid3;
};

/** bit field defines for smurf_if_hwacc_config_s#ctrl */
#define SMURF_IF_HWACC_CONFIG_CTRL_LOOPBACK_MODE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_CTRL_LOOPBACK_MODE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ctrl_local */
#define SMURF_IF_HWACC_CONFIG_CTRL_LOCAL_LOCAL_LOOPBACK_MODE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_CTRL_LOCAL_LOCAL_LOOPBACK_MODE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#phy_rxabort_req */
#define SMURF_IF_HWACC_CONFIG_PHY_RXABORT_REQ_PHY_RXABORT_REQ_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PHY_RXABORT_REQ_PHY_RXABORT_REQ_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#phy_rxabort_stat */
#define SMURF_IF_HWACC_CONFIG_PHY_RXABORT_STAT_PHY_RXABORT_STAT_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PHY_RXABORT_STAT_PHY_RXABORT_STAT_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_comd_0 */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_PENDING_SUSPEND_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_PENDING_SUSPEND_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_PENDING_ABORT_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_PENDING_ABORT_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_RUNNING_SUSPEND_OFFSET 2
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_RUNNING_SUSPEND_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_RUNNING_ABORT_OFFSET 3
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_0_RUNNING_ABORT_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_stat_0 */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_PENDING_SUSPENDED_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_PENDING_SUSPENDED_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_PENDING_VALID_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_PENDING_VALID_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_RUNNING_SUSPENDED_OFFSET 2
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_RUNNING_SUSPENDED_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_RUNNING_ABORT_BUSY_OFFSET 3
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_RUNNING_ABORT_BUSY_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_RUNNING_BUSY_OFFSET 4
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_0_RUNNING_BUSY_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_comd_1 */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_PENDING_SUSPEND_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_PENDING_SUSPEND_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_PENDING_ABORT_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_PENDING_ABORT_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_RUNNING_SUSPEND_OFFSET 2
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_RUNNING_SUSPEND_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_RUNNING_ABORT_OFFSET 3
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_COMD_1_RUNNING_ABORT_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_stat_1 */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_PENDING_SUSPENDED_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_PENDING_SUSPENDED_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_PENDING_VALID_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_PENDING_VALID_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_RUNNING_SUSPENDED_OFFSET 2
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_RUNNING_SUSPENDED_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_RUNNING_ABORT_BUSY_OFFSET 3
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_RUNNING_ABORT_BUSY_SIZE 1
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_RUNNING_BUSY_OFFSET 4
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_STAT_1_RUNNING_BUSY_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_fltclear */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTCLEAR_NONE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTCLEAR_NONE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_fltstatus */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTSTATUS_NONE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTSTATUS_NONE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_fltrawstatus */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTRAWSTATUS_NONE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTRAWSTATUS_NONE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_fltmaskclr */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTMASKCLR_NONE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTMASKCLR_NONE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_fltmaskset */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTMASKSET_NONE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTMASKSET_NONE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_fltmaskstatus */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTMASKSTATUS_NONE_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_FLTMASKSTATUS_NONE_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_comd */
#define SMURF_IF_HWACC_CONFIG_PSS_COMD_RUNNING_SUSPEND_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_COMD_RUNNING_SUSPEND_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_stat */
#define SMURF_IF_HWACC_CONFIG_PSS_STAT_RUNNING_SUSPENDED_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_STAT_RUNNING_SUSPENDED_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_fltclear */
#define SMURF_IF_HWACC_CONFIG_PSS_FLTCLEAR_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_FLTCLEAR_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTCLEAR_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTCLEAR_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_fltstatus */
#define SMURF_IF_HWACC_CONFIG_PSS_FLTSTATUS_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_FLTSTATUS_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTSTATUS_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTSTATUS_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_fltrawstatus */
#define SMURF_IF_HWACC_CONFIG_PSS_FLTRAWSTATUS_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_FLTRAWSTATUS_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTRAWSTATUS_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTRAWSTATUS_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_fltmaskclr */
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKCLR_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKCLR_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKCLR_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKCLR_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_fltmaskset */
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSET_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSET_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSET_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSET_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#pss_fltmaskstatus */
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSTATUS_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSTATUS_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSTATUS_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_PSS_FLTMASKSTATUS_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_comd */
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_PENDING_SUSPEND_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_PENDING_SUSPEND_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_PENDING_ABORT_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_PENDING_ABORT_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_RUNNING_SUSPEND_OFFSET 2
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_RUNNING_SUSPEND_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_RUNNING_ABORT_OFFSET 3
#define SMURF_IF_HWACC_CONFIG_SSS_COMD_RUNNING_ABORT_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_stat */
#define SMURF_IF_HWACC_CONFIG_SSS_STAT_PENDING_SUSPENDED_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_STAT_PENDING_SUSPENDED_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_STAT_RUNNING_SUSPENDED_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_STAT_RUNNING_SUSPENDED_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_STAT_RUNNING_ABORT_BUSY_OFFSET 2
#define SMURF_IF_HWACC_CONFIG_SSS_STAT_RUNNING_ABORT_BUSY_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_fltclear */
#define SMURF_IF_HWACC_CONFIG_SSS_FLTCLEAR_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_FLTCLEAR_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTCLEAR_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTCLEAR_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_fltstatus */
#define SMURF_IF_HWACC_CONFIG_SSS_FLTSTATUS_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_FLTSTATUS_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTSTATUS_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTSTATUS_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_fltrawstatus */
#define SMURF_IF_HWACC_CONFIG_SSS_FLTRAWSTATUS_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_FLTRAWSTATUS_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTRAWSTATUS_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTRAWSTATUS_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_fltmaskclr */
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKCLR_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKCLR_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKCLR_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKCLR_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_fltmaskset */
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSET_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSET_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSET_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSET_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#sss_fltmaskstatus */
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSTATUS_AXI_READ_ERROR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSTATUS_AXI_READ_ERROR_SIZE 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSTATUS_AXI_WRITE_ERROR_OFFSET 1
#define SMURF_IF_HWACC_CONFIG_SSS_FLTMASKSTATUS_AXI_WRITE_ERROR_SIZE 1

/** bit field defines for smurf_if_hwacc_config_s#fbuf_rfdma_rdptr */
#define SMURF_IF_HWACC_CONFIG_FBUF_RFDMA_RDPTR_START_ADDR_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FBUF_RFDMA_RDPTR_START_ADDR_SIZE 15
#define SMURF_IF_HWACC_CONFIG_FBUF_RFDMA_RDPTR_RD_PTR_OFFSET 16
#define SMURF_IF_HWACC_CONFIG_FBUF_RFDMA_RDPTR_RD_PTR_SIZE 15

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_rfdma_outst_read_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_RFDMA_OUTST_READ_XACTIONS_FABRIC_BRIDGE_RFDMA_OUTST_READ_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_RFDMA_OUTST_READ_XACTIONS_FABRIC_BRIDGE_RFDMA_OUTST_READ_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_ul_bpe_outst_read_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_UL_BPE_OUTST_READ_XACTIONS_FABRIC_BRIDGE_UL_BPE_OUTST_READ_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_UL_BPE_OUTST_READ_XACTIONS_FABRIC_BRIDGE_UL_BPE_OUTST_READ_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_dl_bpe_outst_read_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_DL_BPE_OUTST_READ_XACTIONS_FABRIC_BRIDGE_DL_BPE_OUTST_READ_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_DL_BPE_OUTST_READ_XACTIONS_FABRIC_BRIDGE_DL_BPE_OUTST_READ_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_pss_outst_read_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_PSS_OUTST_READ_XACTIONS_FABRIC_BRIDGE_PSS_OUTST_READ_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_PSS_OUTST_READ_XACTIONS_FABRIC_BRIDGE_PSS_OUTST_READ_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_sss_outst_read_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_SSS_OUTST_READ_XACTIONS_FABRIC_BRIDGE_SSS_OUTST_READ_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_SSS_OUTST_READ_XACTIONS_FABRIC_BRIDGE_SSS_OUTST_READ_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_rfdma_outst_write_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_RFDMA_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_RFDMA_OUTST_WRITE_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_RFDMA_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_RFDMA_OUTST_WRITE_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_ul_bpe_outst_write_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_UL_BPE_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_UL_BPE_OUTST_WRITE_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_UL_BPE_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_UL_BPE_OUTST_WRITE_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_dl_bpe_outst_write_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_DL_BPE_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_DL_BPE_OUTST_WRITE_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_DL_BPE_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_DL_BPE_OUTST_WRITE_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_ofdm_demux_outst_write_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_OFDM_DEMUX_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_OFDM_DEMUX_OUTST_WRITE_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_OFDM_DEMUX_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_OFDM_DEMUX_OUTST_WRITE_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_pss_outst_write_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_PSS_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_PSS_OUTST_WRITE_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_PSS_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_PSS_OUTST_WRITE_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fabric_bridge_sss_outst_write_xactions */
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_SSS_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_SSS_OUTST_WRITE_XACTIONS_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FABRIC_BRIDGE_SSS_OUTST_WRITE_XACTIONS_FABRIC_BRIDGE_SSS_OUTST_WRITE_XACTIONS_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#fbuf_ls_ctrl_thresh */
#define SMURF_IF_HWACC_CONFIG_FBUF_LS_CTRL_THRESH_FBUF_LS_CTRL_THRESH_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_FBUF_LS_CTRL_THRESH_FBUF_LS_CTRL_THRESH_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#ul_bpe_ls_ctrl_thresh */
#define SMURF_IF_HWACC_CONFIG_UL_BPE_LS_CTRL_THRESH_UL_BPE_LS_CTRL_THRESH_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_UL_BPE_LS_CTRL_THRESH_UL_BPE_LS_CTRL_THRESH_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#dl_bpe_ls_ctrl_thresh */
#define SMURF_IF_HWACC_CONFIG_DL_BPE_LS_CTRL_THRESH_DL_BPE_LS_CTRL_THRESH_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_DL_BPE_LS_CTRL_THRESH_DL_BPE_LS_CTRL_THRESH_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#ofdm_demux_ls_ctrl_thresh */
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_LS_CTRL_THRESH_OFDM_DEMUX_LS_CTRL_THRESH_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_OFDM_DEMUX_LS_CTRL_THRESH_OFDM_DEMUX_LS_CTRL_THRESH_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#pss_ls_ctrl_thresh */
#define SMURF_IF_HWACC_CONFIG_PSS_LS_CTRL_THRESH_PSS_LS_CTRL_THRESH_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PSS_LS_CTRL_THRESH_PSS_LS_CTRL_THRESH_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#sss_ls_ctrl_thresh */
#define SMURF_IF_HWACC_CONFIG_SSS_LS_CTRL_THRESH_SSS_LS_CTRL_THRESH_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_SSS_LS_CTRL_THRESH_SSS_LS_CTRL_THRESH_SIZE 3

/** bit field defines for smurf_if_hwacc_config_s#peripheralid4 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID4_PERIPHERALID4_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID4_PERIPHERALID4_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid5 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID5_PERIPHERALID5_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID5_PERIPHERALID5_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid6 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID6_PERIPHERALID6_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID6_PERIPHERALID6_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid7 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID7_PERIPHERALID7_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID7_PERIPHERALID7_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid0 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID0_PERIPHERALID0_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID0_PERIPHERALID0_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid1 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID1_PERIPHERALID1_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID1_PERIPHERALID1_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid2 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID2_PERIPHERALID2_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID2_PERIPHERALID2_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#peripheralid3 */
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID3_PERIPHERALID3_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_PERIPHERALID3_PERIPHERALID3_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#componentid0 */
#define SMURF_IF_HWACC_CONFIG_COMPONENTID0_COMPONENTID0_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_COMPONENTID0_COMPONENTID0_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#componentid1 */
#define SMURF_IF_HWACC_CONFIG_COMPONENTID1_COMPONENTID1_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_COMPONENTID1_COMPONENTID1_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#componentid2 */
#define SMURF_IF_HWACC_CONFIG_COMPONENTID2_COMPONENTID2_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_COMPONENTID2_COMPONENTID2_SIZE 8

/** bit field defines for smurf_if_hwacc_config_s#componentid3 */
#define SMURF_IF_HWACC_CONFIG_COMPONENTID3_COMPONENTID3_OFFSET 0
#define SMURF_IF_HWACC_CONFIG_COMPONENTID3_COMPONENTID3_SIZE 8

/* EOF smurf_if_hwacc_config.h */
#endif
