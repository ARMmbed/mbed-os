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
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifndef _HOST_WLAN_CONFIG_H_
#define _HOST_WLAN_CONFIG_H_

/* Include definitions here that can be used to tune the WLAN module behavior.
 * Different customers can tune the behavior as per their needs, here.
 */

/* This configuration item when defined will consider the barker preamble
 * mentioned in the ERP IE of the beacons from the AP to determine the short
 * preamble support sent in the (Re)Assoc request frames.
 */
#define WLAN_CONFIG_DONOT_IGNORE_BARKER_IN_ERP 0

/* This config item when defined will not send the power module state transition
 * failure events that happen during scan, to the host.
 */
#define WLAN_CONFIG_IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN 0

/*
 * This configuration item enable/disable keepalive support.
 * Keepalive support: In the absence of any data traffic to AP, null
 * frames will be sent to the AP at periodic interval, to keep the association
 * active. This configuration item defines the periodic interval.
 * Use value of zero to disable keepalive support
 * Default: 60 seconds
 */
#define WLAN_CONFIG_KEEP_ALIVE_INTERVAL 60

/*
 * This configuration item disables 11n support.
 * The wifi device will connect using 11g, 11b
 * or 11a(if supported by chipset) only.
 * The consequence of allowing
 * 11n is that aggregate packets must be
 * supported by the host which requires more
 * RAM for received buffers.  Because
 * throughput will anyways be limited by the
 * SPI interface 11n support will not provide
 * greater throughput on slower host systems. However some
 * access points can be configured for 11n only connections
 * in which case disabling 11n would prevent the system from
 * being able to connect to such AP's.
 * 0 - Enable
 * 1 - Disable
 */
#define WLAN_CONFIG_DISABLE_11N 0

/*
 * This configuration is used to support 11N aggregation
 * inside the driver.  If aggregation is intended
 * to be used then this must be set to 1.  The code
 * and RAM footprints for the driver will increase
 * by enabling this feature.  This feature can
 * improve throughput performance if the RF is
 * the throughput bottle-neck for the system.
 *
 */
#define WLAN_CONFIG_11N_AGGR_SUPPORT 1

/*
 * This configuration controls whether statistics
 * from the wifi device are stored in the driver.
 * This takes a fair amount of RAM and so should
 * only be activated if needed.
 */
#define WLAN_CONFIG_CAPTURE_TARGET_STATS 0

/*
 * This configuration controls whether the
 * wifi driver accepts 802.11 frames from
 * the stack.  If 0 then 802.3 frames
 * are required.
 * ****** DO NOT ADJUST ******
 */
#define WLAN_CONFIG_TX_DOT11_HDR 0

/*
 * This configuration enables/disables internal
 * driver stat tracking using the AR_SOFTC_T.
 * Storage cost is ~100 bytes.
 */
#define WLAN_CONFIG_NET_STATS 0

/*
 * This configuration reduces the number of
 * virtual endpoints managed by the driver
 * to what would be used if WMM were active
 * rather than what the device is capable
 * of supporting.  enabling this configuration
 * saves some RAM and reduces execution time
 * modestly.
 * ****** DO NOT ADJUST ******
 */
#define WLAN_CONFIG_TRUNCATED_ENDPOINTS 1

/*
 * This configuration enables/disables use
 * of a GPIO to reset/power down the WiFi device.
 * If this value is 0 then the wifi board should
 * be configured (via jumpers) so that the PWD line
 * is asserted low through some other appropriate
 * method.
 */
#define WLAN_CONFIG_ENABLE_CHIP_PWD_GPIO 1

/*
 * This configuration enable/disables use of
 * WMI command synchronization. It is only
 * needed if wmi commands which use a flag
 * other than NO_SYNC_WMIFLAG are introduced.
 * ****** DO NOT ADJUST ******
 */
#define WLAN_CONFIG_ENABLE_WMI_SYNC 0

/*
 * This configuration should be set to 1 when
 * it is desired to allow AMSDU's to be sliced
 * by the host system.  This should only be
 * enabled (set to 1) by Atheros.
 * ****** DO NOT ADJUST ******
 */
#define WLAN_CONFIG_AMSDU_ON_HOST 0

#define WLAN_CONFIG_REDUCE_CREDIT_DRIBBLE 1

#define WLAN_CONFIG_NUM_PRE_ALLOC_RX_BUFFERS 2

/* compile time test to ensure a valid 11N configuration */
#if WLAN_CONFIG_DISABLE_11N
#if (WLAN_CONFIG_11N_AGGR_SUPPORT || WLAN_CONFIG_AMSDU_ON_HOST)
#error INVALID 11N COMPILE CONFIGURATION
#endif
#endif

#endif /* _HOST_WLAN_CONFIG_H_ */
