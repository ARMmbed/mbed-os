#ifndef INCLUDED_USBHS_CFGS
#define INCLUDED_USBHS_CFGS
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** usbhs_cfgs HAL Spreadsheet version number */
#define USBHS_CFGS_HAL_VERSION 1

/** usbhs_cfgs
*/
struct usbhs_cfgs_s {
   /** Always ON register Configuration register for USBHS sub-system at address offset 0x000, read-write */
   uint32_t conf;
   /** Reserved space */
   uint8_t fill0[12];
   /** Always ON register Control register for DP/DM PHY. Will have effect only when usbhs_conf.hsic_sel = 0 at address offset 0x010, read-write */
   uint32_t dpdmphy_ctrl;
   /** Always ON register Override signal register for DP/DM PHY to override controls from USB core IP towards DPDM PHY.  The contents of The contents of USBHS_DPDMPHY_OVR register does not have any effect if USBHS_DPDMPHY_CTRL.override_en register bit is 0 at address offset 0x014, read-write */
   uint32_t dpdmphy_ovr;
   /** Status register for DP/DM PHY                                                                                                                                   at address offset 0x018, read-only */
   uint32_t dpdmphy_rdbk;
   /** Status register for USB core at address offset 0x01C, read-only */
   uint32_t core_rdbk;
   /** Reserved space */
   uint8_t fill1[16];
   /** Always ON register Additional control bits for USB DpDM PHY - to tune various PHY parameters - depends on silicon. Refer to SF spec and USB-PHY datasheet at address offset 0x030, read-write */
   uint32_t dpdmphy_extra_ctrl1;
   /** Always ON register Additional control bits for USB DpDM PHY - to tune various PHY parameters - depends on silicon. Refer to SF spec and USB-PHY datasheet at address offset 0x034, read-write */
   uint32_t dpdmphy_extra_ctrl2;
   /** Always ON register Additional control bits for USB DpDM PHY - to tune various PHY parameters - depends on silicon. Refer to SF spec and USB-PHY datasheet at address offset 0x038, read-write */
   uint32_t dpdmphy_extra_ctrl3;
   /** Additional observation lines from USB DPDM PHY  at address offset 0x03C, read-only */
   uint32_t dpdmphy_extra_rdbk1;
   /** Additional observation lines from USB DPDM PHY  at address offset 0x040, read-only */
   uint32_t dpdmphy_extra_rdbk2;
};

/** bit field defines for usbhs_cfgs_s#conf */
#define USBHS_CFGS_CONF_HS_DISABLE_OFFSET 1
#define USBHS_CFGS_CONF_HS_DISABLE_SIZE 1
#define USBHS_CFGS_CONF_USB_WAKEUP_OFFSET 2
#define USBHS_CFGS_CONF_USB_WAKEUP_SIZE 1
#define USBHS_CFGS_CONF_SLEEP_AUTOCLKGATING_ENABLE_OFFSET 3
#define USBHS_CFGS_CONF_SLEEP_AUTOCLKGATING_ENABLE_SIZE 1
#define USBHS_CFGS_CONF_SUSPEND_AUTOCLKGATING_ENABLE_OFFSET 4
#define USBHS_CFGS_CONF_SUSPEND_AUTOCLKGATING_ENABLE_SIZE 1
#define USBHS_CFGS_CONF_WAKEUP_LINECHANGE_DISABLE_OFFSET 5
#define USBHS_CFGS_CONF_WAKEUP_LINECHANGE_DISABLE_SIZE 1
#define USBHS_CFGS_CONF_WAKEUP_VBUSCHANGE_DISABLE_OFFSET 6
#define USBHS_CFGS_CONF_WAKEUP_VBUSCHANGE_DISABLE_SIZE 1

/** bit field defines for usbhs_cfgs_s#dpdmphy_ctrl */
#define USBHS_CFGS_DPDMPHY_CTRL_RESET_OFFSET 0
#define USBHS_CFGS_DPDMPHY_CTRL_RESET_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_SUSPEND_N_OFFSET 1
#define USBHS_CFGS_DPDMPHY_CTRL_SUSPEND_N_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_SLEEP_N_OFFSET 2
#define USBHS_CFGS_DPDMPHY_CTRL_SLEEP_N_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_REF_CLK_SEL_OFFSET 3
#define USBHS_CFGS_DPDMPHY_CTRL_REF_CLK_SEL_SIZE 2
#define USBHS_CFGS_DPDMPHY_CTRL_DATABUS16_8_OFFSET 5
#define USBHS_CFGS_DPDMPHY_CTRL_DATABUS16_8_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_TX_BITSTUFF_ENABLE_OFFSET 6
#define USBHS_CFGS_DPDMPHY_CTRL_TX_BITSTUFF_ENABLE_SIZE 2
#define USBHS_CFGS_DPDMPHY_CTRL_DP_PULLDOWN_OFFSET 8
#define USBHS_CFGS_DPDMPHY_CTRL_DP_PULLDOWN_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_DM_PULLDOWN_OFFSET 9
#define USBHS_CFGS_DPDMPHY_CTRL_DM_PULLDOWN_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_FSLS_SERIAL_MODE_OFFSET 10
#define USBHS_CFGS_DPDMPHY_CTRL_FSLS_SERIAL_MODE_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_PHY_PLLOE_OFFSET 11
#define USBHS_CFGS_DPDMPHY_CTRL_PHY_PLLOE_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_PHY_BGOE_OFFSET 12
#define USBHS_CFGS_DPDMPHY_CTRL_PHY_BGOE_SIZE 1
#define USBHS_CFGS_DPDMPHY_CTRL_OVERRIDE_EN_OFFSET 15
#define USBHS_CFGS_DPDMPHY_CTRL_OVERRIDE_EN_SIZE 1

/** bit field defines for usbhs_cfgs_s#dpdmphy_ovr */
#define USBHS_CFGS_DPDMPHY_OVR_SUSPEND_N_OVR_OFFSET 1
#define USBHS_CFGS_DPDMPHY_OVR_SUSPEND_N_OVR_SIZE 1
#define USBHS_CFGS_DPDMPHY_OVR_SLEEP_N_OVR_OFFSET 2
#define USBHS_CFGS_DPDMPHY_OVR_SLEEP_N_OVR_SIZE 1
#define USBHS_CFGS_DPDMPHY_OVR_OPMODE_OVR_OFFSET 3
#define USBHS_CFGS_DPDMPHY_OVR_OPMODE_OVR_SIZE 2
#define USBHS_CFGS_DPDMPHY_OVR_TERMSELECT_OVR_OFFSET 5
#define USBHS_CFGS_DPDMPHY_OVR_TERMSELECT_OVR_SIZE 1
#define USBHS_CFGS_DPDMPHY_OVR_XCVRSELECT_OVR_OFFSET 6
#define USBHS_CFGS_DPDMPHY_OVR_XCVRSELECT_OVR_SIZE 2
#define USBHS_CFGS_DPDMPHY_OVR_TXVALID_OVR_OFFSET 8
#define USBHS_CFGS_DPDMPHY_OVR_TXVALID_OVR_SIZE 2
#define USBHS_CFGS_DPDMPHY_OVR_DATAIN_OVR_OFFSET 16
#define USBHS_CFGS_DPDMPHY_OVR_DATAIN_OVR_SIZE 16

/** bit field defines for usbhs_cfgs_s#dpdmphy_rdbk */
#define USBHS_CFGS_DPDMPHY_RDBK_VBUS_VALID_OFFSET 0
#define USBHS_CFGS_DPDMPHY_RDBK_VBUS_VALID_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_CORE_SUSPEND_OFFSET 1
#define USBHS_CFGS_DPDMPHY_RDBK_CORE_SUSPEND_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_CORE_SLEEP_OFFSET 2
#define USBHS_CFGS_DPDMPHY_RDBK_CORE_SLEEP_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_LINESTATE_OFFSET 3
#define USBHS_CFGS_DPDMPHY_RDBK_LINESTATE_SIZE 2
#define USBHS_CFGS_DPDMPHY_RDBK_TX_READY_OFFSET 5
#define USBHS_CFGS_DPDMPHY_RDBK_TX_READY_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_RX_ACTIVE_OFFSET 6
#define USBHS_CFGS_DPDMPHY_RDBK_RX_ACTIVE_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_RX_ERROR_OFFSET 7
#define USBHS_CFGS_DPDMPHY_RDBK_RX_ERROR_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_RX_VALID_OFFSET 8
#define USBHS_CFGS_DPDMPHY_RDBK_RX_VALID_SIZE 2
#define USBHS_CFGS_DPDMPHY_RDBK_TX_VALID_OFFSET 10
#define USBHS_CFGS_DPDMPHY_RDBK_TX_VALID_SIZE 2
#define USBHS_CFGS_DPDMPHY_RDBK_OPMODE_OFFSET 12
#define USBHS_CFGS_DPDMPHY_RDBK_OPMODE_SIZE 2
#define USBHS_CFGS_DPDMPHY_RDBK_XCVRSELECT_OFFSET 14
#define USBHS_CFGS_DPDMPHY_RDBK_XCVRSELECT_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_TERMSELECT_OFFSET 15
#define USBHS_CFGS_DPDMPHY_RDBK_TERMSELECT_SIZE 1
#define USBHS_CFGS_DPDMPHY_RDBK_DATAOUT_OFFSET 16
#define USBHS_CFGS_DPDMPHY_RDBK_DATAOUT_SIZE 16

/** bit field defines for usbhs_cfgs_s#core_rdbk */
#define USBHS_CFGS_CORE_RDBK_WAKEUP_INTR_REQ_OFFSET 0
#define USBHS_CFGS_CORE_RDBK_WAKEUP_INTR_REQ_SIZE 1
#define USBHS_CFGS_CORE_RDBK_USB_UPSTREAM_STATE_OFFSET 1
#define USBHS_CFGS_CORE_RDBK_USB_UPSTREAM_STATE_SIZE 5
#define USBHS_CFGS_CORE_RDBK_AUTOCLKGATINGSTATUS_OFFSET 7
#define USBHS_CFGS_CORE_RDBK_AUTOCLKGATINGSTATUS_SIZE 1

/** bit field defines for usbhs_cfgs_s#dpdmphy_extra_ctrl1 */
#define USBHS_CFGS_DPDMPHY_EXTRA_CTRL1_NONE_OFFSET 0
#define USBHS_CFGS_DPDMPHY_EXTRA_CTRL1_NONE_SIZE 32

/** bit field defines for usbhs_cfgs_s#dpdmphy_extra_ctrl2 */
#define USBHS_CFGS_DPDMPHY_EXTRA_CTRL2_NONE_OFFSET 0
#define USBHS_CFGS_DPDMPHY_EXTRA_CTRL2_NONE_SIZE 32

/** bit field defines for usbhs_cfgs_s#dpdmphy_extra_ctrl3 */
#define USBHS_CFGS_DPDMPHY_EXTRA_CTRL3_NONE_OFFSET 0
#define USBHS_CFGS_DPDMPHY_EXTRA_CTRL3_NONE_SIZE 32

/** bit field defines for usbhs_cfgs_s#dpdmphy_extra_rdbk1 */
#define USBHS_CFGS_DPDMPHY_EXTRA_RDBK1_NONE_OFFSET 0
#define USBHS_CFGS_DPDMPHY_EXTRA_RDBK1_NONE_SIZE 32

/** bit field defines for usbhs_cfgs_s#dpdmphy_extra_rdbk2 */
#define USBHS_CFGS_DPDMPHY_EXTRA_RDBK2_NONE_OFFSET 0
#define USBHS_CFGS_DPDMPHY_EXTRA_RDBK2_NONE_SIZE 32

/* EOF usbhs_cfgs.h */
#endif
