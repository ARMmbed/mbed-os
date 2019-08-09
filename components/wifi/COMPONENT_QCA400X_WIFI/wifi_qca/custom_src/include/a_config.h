//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
// Author(s): ="Atheros"
//==============================================================================

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "wifi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Debug and assert printing
// ============================================================================

// KSDK printf defined in fsl_debug_console.h
// extern int debug_printf(const char  *fmt_s, ...);

// Define this to a printf-like function to print asserts via qcadrv_assert_func()
//#define ASSERT_PRINTF(...)
//#define ASSERT_PRINTF(...)  debug_printf(__VA_ARGS__)

// Define these to enable WMI RX event tracing
#define WMI_DEBUG_RX_EVENT(cmdId, pReq)
//#define WMI_DEBUG_RX_EVENT(cmdId, pReq)  debug_printf("wmiRx=%x\r\n", id)

// Define this to enable WMI TX command tracing
#define WMI_DEBUG_TX_COMMAND(cmdId, pReq)
//#define WMI_DEBUG_TX_COMMAND(cmdId, pReq)  debug_printf("wmiTx=%x\r\n", cmdId)

// The original QCA driver developers have sprinkled printf statements at more
// or less random places and if you want to see thes, then define this macro
#define QCADRV_PRINTF(...)
//#define QCADRV_PRINTF(...)  debug_printf(__VA_ARGS__)

// NOTE: wmi.c contains a plethora of debug definitions and they are pretty hard
// to make head or tails out of but if you define AR_DEBUG_PRINTF() in a_debug.h
// to some printf() function then you enable some additional WMI tracing that
// were put in place by the original QCA driver developers.

// ============================================================================
//
// ============================================================================

#define ENABLE_FPGA_BUILD 0

#define USE_4BYTE_REGISTER_ACCESS

#define TARGET_AR4100_REV2 0x30000582
#define TARGET_AR400X_REV1 0x31C80997
#define TARGET_AR400X_REV2 0x31C80002
#define TARGET_AR400X_REV3 0x31C8270E
#define TARGET_AR400X_REV4 0x32000052

#define ATH_FIRMWARE_TARGET TARGET_AR400X_REV1 // TARGET_AR400X_REV2

#ifndef SUPPORT_11N
#define SUPPORT_11N 1
#endif

/* PORT_NOTE: Set the following DRIVER_CONFIG_... defines according to
 * 	the requirements/attributes of the OS and MCU. */

/* DRIVER_CONFIG_INCLUDE_BMI - optionally set to 0 if the driver will
 * 	not be used in BMI mode and code space needs to be saved.
 *  If the driver will be used in BMI for flash reprogram or other then
 *	this option must be 1.
 */
#define DRIVER_CONFIG_INCLUDE_BMI 1 // 1 or 0

/* DRIVER_CONFIG_DISABLE_ASSERT - set to 1 if assert NOP behavior is
 * preferred <not recommended> */
#define DRIVER_CONFIG_DISABLE_ASSERT 0 // 1 or 0
/* DRIVER_CONFIG_PROFILE_BOOT - can be used to profile driver boot
 * process with GPIO's + logic analyzer */
#define DRIVER_CONFIG_PROFILE_BOOT 0 // 1 or 0

/* DRIVER_CONFIG_IMPLEMENT_RX_FREE_QUEUE - set if driver should
 * implement a RX Free Queue */
#define DRIVER_CONFIG_IMPLEMENT_RX_FREE_QUEUE 1 // 1 or 0

/*TCPIP stack offload for AR4001*/
#define ENABLE_STACK_OFFLOAD 1 // 1 or 0
#if !ENABLE_STACK_OFFLOAD
#   error "WiFi driver works only with ENABLE_STACK_OFFLOAD enabled ! "
#endif

#define T_SELECT_VER1 1 // 1 or 0

/* DRIVER_CONFIG_ENABLE_STORE_RECALL - optionally set to zero to reduce
 * driver memory footprint.
 */
#define DRIVER_CONFIG_ENABLE_STORE_RECALL 1 // 1 or 0

#define ENABLE_AP_MODE 1 // 1 or 0

#define MULTI_SOCKET_SUPPORT 1 // 1 or 0

#define MANUFACTURING_SUPPORT 1
#define ENABLE_P2P_MODE 1 // 1 or 0

/* DRIVER_CONFIG_PROGRAM_MAC_ADDR - optionally add code to allow an application
 * to program a new mac address to the wifi device.  this code should only
 * be included when programming a mac address is required such as during
 * production.  otherwise code space can be saved by setting this to 0.
 */
#define DRIVER_CONFIG_PROGRAM_MAC_ADDR 1 // 1 or 0

/* DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD is used to enable Firmware download
   from the host. This feature is implemented but kept disable and can be enabled
   on need basis. DO NOT EDIT NEXT LINE INCLUDING THE SPACE. BUILD SCRIPT SEARCHES
   FOR THIS PATTERN AND UPDATES IT IN BUILD TIME. */
#define DRIVER_CONFIG_ENABLE_HOST_FW_DOWNLOAD 0 // 1 or 0

#define ENABLE_HTTP_SERVER 1 // 1 or 0

#define ENABLE_HTTP_CLIENT 1 // 1 or 0

#define ENABLE_DNS_SERVER 1 // 1 or 0

#define ENABLE_DNS_CLIENT 1 // 1 or 0

#define ENABLE_SNTP_CLIENT 1 // 1 or 0

#define ENABLE_RAW_SOCKET_DEMO 1 // 1 or 0

#define WLAN_NUM_OF_DEVICES 1 // 1 or 2

#define ENABLE_ROUTING_CMDS 1 // 1 or 0
#define ENABLE_HTTPS_SERVER 1 // 1 or 0

#define ENABLE_HTTPS_CLIENT 1 // 1 or 0

#define ENABLE_SSL 1 // 1 or 0

#define ENABLE_SCC_MODE 1 // 1 or 0
#define ENABLE_KF_PERFORMANCE 1

#define ENABLE_LARGE_DSET 1 // 1 or 0

#include "wlan_config.h"

#ifdef __cplusplus
}
#endif

#endif
