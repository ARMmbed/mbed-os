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
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

/*
 * This file contains the definitions of the WMI protocol specified in the
 * Wireless Module Interface (WMI).  It includes definitions of all the
 * commands and events. Commands are messages from the host to the WM.
 * Events and Replies are messages from the WM to the host.
 *
 * Ownership of correctness in regards to commands
 * belongs to the host driver and the WMI is not required to validate
 * parameters for value, proper range, or any other checking.
 *
 */

#ifndef _WMI_H_
#define _WMI_H_

//#include "wmix.h"
#include "wlan_defs.h"
#include <a_osapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTC_PROTOCOL_VERSION 0x0002
#define HTC_PROTOCOL_REVISION 0x0000

#define WMI_PROTOCOL_VERSION 0x0002
#define WMI_PROTOCOL_REVISION 0x0000

#define ATH_MAC_LEN 6 /* length of mac in bytes */
#define WMI_CMD_MAX_LEN 100
#define WMI_CONTROL_MSG_MAX_LEN 256
#define WMI_OPT_CONTROL_MSG_MAX_LEN 1536
#define MAX_EVENT_SIZE 1200
#define IS_ETHERTYPE(_typeOrLen) ((_typeOrLen) >= 0x0600)
#define RFC1042OUI       \
    {                    \
        0x00, 0x00, 0x00 \
    }

#define IP_ETHERTYPE 0x0800

#define WMI_IMPLICIT_PSTREAM 0xFF
#define WMI_MAX_THINSTREAM 15

#define WOW_MAX_PATTERN_LEN 64

#ifdef AR6002_REV2
#define IBSS_MAX_NUM_STA 4
#else
#define IBSS_MAX_NUM_STA 8
#endif

struct host_app_area_s
{
    uint32_t wmi_protocol_ver;
};

typedef PREPACK struct
{
    uint8_t a FIELD_PACKED;
    uint8_t b FIELD_PACKED;
    uint8_t c FIELD_PACKED;
} POSTPACK MY_TEST;

/*
 * Data Path
 */
typedef PREPACK struct
{
    uint8_t dstMac[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t srcMac[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t typeOrLen FIELD_PACKED;
} POSTPACK ATH_MAC_HDR;

typedef PREPACK struct
{
    uint8_t dsap FIELD_PACKED;
    uint8_t ssap FIELD_PACKED;
    uint8_t cntl FIELD_PACKED;
    uint8_t orgCode[3] FIELD_PACKED;
    uint16_t etherType FIELD_PACKED;
} POSTPACK ATH_LLC_SNAP_HDR;

typedef enum
{
    DATA_MSGTYPE = 0x0,
    CNTL_MSGTYPE,
    SYNC_MSGTYPE,
    OPT_MSGTYPE
} WMI_MSG_TYPE;

/*
 * Macros for operating on WMI_DATA_HDR (info) field
 */

#define WMI_DATA_HDR_MSG_TYPE_MASK 0x03
#define WMI_DATA_HDR_MSG_TYPE_SHIFT 0
#define WMI_DATA_HDR_UP_MASK 0x07
#define WMI_DATA_HDR_UP_SHIFT 2
/* In AP mode, the same bit (b5) is used to indicate Power save state in
 * the Rx dir and More data bit state in the tx direction.
 */
#define WMI_DATA_HDR_PS_MASK 0x1
#define WMI_DATA_HDR_PS_SHIFT 5

#define WMI_DATA_HDR_MORE_MASK 0x1
#define WMI_DATA_HDR_MORE_SHIFT 5

typedef enum
{
    WMI_DATA_HDR_DATA_TYPE_802_3 = 0,
    WMI_DATA_HDR_DATA_TYPE_802_11,
    WMI_DATA_HDR_DATA_TYPE_ACL
} WMI_DATA_HDR_DATA_TYPE;

#define WMI_DATA_HDR_DATA_TYPE_MASK 0x3
#define WMI_DATA_HDR_DATA_TYPE_SHIFT 6

#define WMI_DATA_HDR_SET_MORE_BIT(h) ((h)->info |= (WMI_DATA_HDR_MORE_MASK << WMI_DATA_HDR_MORE_SHIFT))

#define WMI_DATA_HDR_IS_MSG_TYPE(h, t) (((h)->info & (WMI_DATA_HDR_MSG_TYPE_MASK)) == (t))
#define WMI_DATA_HDR_SET_MSG_TYPE(h, t)                                                                \
    (h)->info = (uint8_t)(((h)->info & ~(WMI_DATA_HDR_MSG_TYPE_MASK << WMI_DATA_HDR_MSG_TYPE_SHIFT)) | \
                          (t << WMI_DATA_HDR_MSG_TYPE_SHIFT))
#define WMI_DATA_HDR_GET_UP(h) (((h)->info >> WMI_DATA_HDR_UP_SHIFT) & WMI_DATA_HDR_UP_MASK)
#define WMI_DATA_HDR_SET_UP(h, p) \
    (h)->info = (uint8_t)(((h)->info & ~(WMI_DATA_HDR_UP_MASK << WMI_DATA_HDR_UP_SHIFT)) | (p << WMI_DATA_HDR_UP_SHIFT))

#define WMI_DATA_HDR_GET_DATA_TYPE(h) (((h)->info >> WMI_DATA_HDR_DATA_TYPE_SHIFT) & WMI_DATA_HDR_DATA_TYPE_MASK)
#define WMI_DATA_HDR_SET_DATA_TYPE(h, p)                                                                 \
    (h)->info = (uint8_t)(((h)->info & ~(WMI_DATA_HDR_DATA_TYPE_MASK << WMI_DATA_HDR_DATA_TYPE_SHIFT)) | \
                          ((p) << WMI_DATA_HDR_DATA_TYPE_SHIFT))

#define WMI_DATA_HDR_GET_DOT11(h) (WMI_DATA_HDR_GET_DATA_TYPE((h)) == WMI_DATA_HDR_DATA_TYPE_802_11)
#define WMI_DATA_HDR_SET_DOT11(h, p) WMI_DATA_HDR_SET_DATA_TYPE((h), (p))

/* Macros for operating on WMI_DATA_HDR (info2) field */
#define WMI_DATA_HDR_SEQNO_MASK 0xFFF
#define WMI_DATA_HDR_SEQNO_SHIFT 0

#define WMI_DATA_HDR_AMSDU_MASK 0x1
#define WMI_DATA_HDR_AMSDU_SHIFT 12

#define WMI_DATA_HDR_META_MASK 0x7
#define WMI_DATA_HDR_META_SHIFT 13

#define GET_SEQ_NO(_v) ((_v)&WMI_DATA_HDR_SEQNO_MASK)
#define GET_ISMSDU(_v) ((_v)&WMI_DATA_HDR_AMSDU_MASK)

#define WMI_DATA_HDR_GET_SEQNO(h) GET_SEQ_NO((h)->info2 >> WMI_DATA_HDR_SEQNO_SHIFT)
#define WMI_DATA_HDR_SET_SEQNO(h, _v)                                                     \
    ((h)->info2 = ((h)->info2 & ~(WMI_DATA_HDR_SEQNO_MASK << WMI_DATA_HDR_SEQNO_SHIFT)) | \
                  (GET_SEQ_NO(_v) << WMI_DATA_HDR_SEQNO_SHIFT))

#define WMI_DATA_HDR_IS_AMSDU(h) GET_ISMSDU((h)->info2 >> WMI_DATA_HDR_AMSDU_SHIFT)
#define WMI_DATA_HDR_SET_AMSDU(h, _v)                                                               \
    ((h)->info2 = (uint16_t)((h)->info2 & ~(WMI_DATA_HDR_AMSDU_MASK << WMI_DATA_HDR_AMSDU_SHIFT)) | \
                  (GET_ISMSDU(_v) << WMI_DATA_HDR_AMSDU_SHIFT))

#define WMI_DATA_HDR_GET_META(h) (((h)->info2 >> WMI_DATA_HDR_META_SHIFT) & WMI_DATA_HDR_META_MASK)
#define WMI_DATA_HDR_SET_META(h, _v)                                                               \
    ((h)->info2 = (uint16_t)(((h)->info2 & ~(WMI_DATA_HDR_META_MASK << WMI_DATA_HDR_META_SHIFT)) | \
                             ((_v) << WMI_DATA_HDR_META_SHIFT)))

/* Macros for operating on WMI_DATA_HDR (info3) field */
#define WMI_DATA_HDR_DEVID_MASK 0xF
#define WMI_DATA_HDR_DEVID_SHIFT 0
#define GET_DEVID(_v) ((_v)&WMI_DATA_HDR_DEVID_MASK)

#define WMI_DATA_HDR_GET_DEVID(h) (((h)->info3 >> WMI_DATA_HDR_DEVID_SHIFT) & WMI_DATA_HDR_DEVID_MASK)
#define WMI_DATA_HDR_SET_DEVID(h, _v)                                                     \
    ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_DEVID_MASK << WMI_DATA_HDR_DEVID_SHIFT)) | \
                  (GET_DEVID(_v) << WMI_DATA_HDR_DEVID_SHIFT))

#define WMI_DATA_HDR_RXFILT_SHIFT 4
#define WMI_DATA_HDR_RXFILT_MASK (0x1 << WMI_DATA_HDR_RXFILT_SHIFT)

#define WMI_DATA_HDR_IS_RXFILTERED(h) ((h)->info3 & WMI_DATA_HDR_RXFILT_MASK)
#define SET_WMI_DATA_HDR_RXFILTERED(h) (h)->info3 |= WMI_DATA_HDR_RXFILT_MASK

#define WMI_DATA_HDR_TRIGGER_MASK 0x1
#define WMI_DATA_HDR_TRIGGER_SHIFT 4
#define WMI_DATA_HDR_SET_TRIGGER(h, _v)                                                       \
    ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_TRIGGER_MASK << WMI_DATA_HDR_TRIGGER_SHIFT)) | \
                  ((_v) << WMI_DATA_HDR_TRIGGER_SHIFT))
#define WMI_DATA_HDR_IS_TRIGGER(h) \
    ((((h)->info3 >> WMI_DATA_HDR_TRIGGER_SHIFT) & WMI_DATA_HDR_TRIGGER_MASK) == WMI_DATA_HDR_TRIGGER_MASK)

#define WMI_DATA_HDR_EOSP_MASK WMI_DATA_HDR_TRIGGER_MASK
#define WMI_DATA_HDR_EOSP_SHIFT WMI_DATA_HDR_TRIGGER_SHIFT

#define WMI_DATA_HDR_SET_EOSP_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_EOSP_MASK << WMI_DATA_HDR_EOSP_SHIFT))
#define WMI_DATA_HDR_HAS_EOSP_BIT(h) ((h)->info3 & (WMI_DATA_HDR_EOSP_MASK << WMI_DATA_HDR_EOSP_SHIFT))

#define WMI_DATA_HDR_CCX_LINKTEST_MASK 0x1
#define WMI_DATA_HDR_CCX_LINKTEST_SHIFT 5
#define WMI_DATA_HDR_SET_CCX_LINKTEST_BIT(h) \
    ((h)->info3 |= (WMI_DATA_HDR_CCX_LINKTEST_MASK << WMI_DATA_HDR_CCX_LINKTEST_SHIFT))
#define WMI_DATA_HDR_HAS_CCX_LINKTEST_BIT(h) \
    ((h)->info3 & (WMI_DATA_HDR_CCX_LINKTEST_MASK << WMI_DATA_HDR_CCX_LINKTEST_SHIFT))

#define WMI_DATA_HDR_PAD_BEFORE_DATA_MASK 0xFF
#define WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT 0x8
#define GET_PAD_BEFORE_DATA_START(_v) ((_v)&WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)

#define WMI_DATA_HDR_GET_PAD_BEFORE_DATA_START(h) \
    (((h)->info3 >> WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT) & WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)
#define WMI_DATA_HDR_SET_PAD_BEFORE_DATA_START(h, _v)                                                         \
    ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_PAD_BEFORE_DATA_MASK << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT)) | \
                  (GET_PAD_BEFORE_DATA_START(_v) << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT))

#define WMI_DATA_HDR_TRIGGERED_MASK 0x1
#define WMI_DATA_HDR_TRIGGERED_SHIFT 6
#define WMI_DATA_HDR_SET_TRIGGERED_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_TRIGGERED_MASK << WMI_DATA_HDR_TRIGGERED_SHIFT))
#define WMI_DATA_HDR_HAS_TRIGGERED_BIT(h) ((h)->info3 & (WMI_DATA_HDR_TRIGGERED_MASK << WMI_DATA_HDR_TRIGGERED_SHIFT))

#define WMI_DATA_HDR_PSPOLLED_MASK 0x1
#define WMI_DATA_HDR_PSPOLLED_SHIFT 7
#define WMI_DATA_HDR_SET_PSPOLLED_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_PSPOLLED_MASK << WMI_DATA_HDR_PSPOLLED_SHIFT))
#define WMI_DATA_HDR_HAS_PSPOLLED_BIT(h) ((h)->info3 & (WMI_DATA_HDR_PSPOLLED_MASK << WMI_DATA_HDR_PSPOLLED_SHIFT))

#define WMI_DATA_HDR_PAD_BEFORE_DATA_MASK 0xFF
#define WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT 0x8
#define GET_PAD_BEFORE_DATA_START(_v) ((_v)&WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)

#define WMI_DATA_HDR_HOST_RETRY_MASK 0x1
#define WMI_DATA_HDR_GET_HOST_RETRY(h) (WMI_DATA_HDR_GET_PAD_BEFORE_DATA_START(h) & WMI_DATA_HDR_HOST_RETRY_MASK)

#define WMI_DATA_HDR_GET_PAD_BEFORE_DATA_START(h) \
    (((h)->info3 >> WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT) & WMI_DATA_HDR_PAD_BEFORE_DATA_MASK)
#define WMI_DATA_HDR_SET_PAD_BEFORE_DATA_START(h, _v)                                                         \
    ((h)->info3 = ((h)->info3 & ~(WMI_DATA_HDR_PAD_BEFORE_DATA_MASK << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT)) | \
                  (GET_PAD_BEFORE_DATA_START(_v) << WMI_DATA_HDR_PAD_BEFORE_DATA_SHIFT))

#define WMI_DATA_HDR_EXCEPTION_BIT_MASK 0x1
#define WMI_DATA_HDR_EXCEPTION_BIT_SHIFT 8
#define WMI_DATA_HDR_SET_EXCEPTION_BIT(h) \
    ((h)->info3 |= (WMI_DATA_HDR_EXCEPTION_BIT_MASK << WMI_DATA_HDR_EXCEPTION_BIT_SHIFT))

#define WMI_DATA_HDR_AMPDU_FLUSH_BIT_MASK 0x1
#define WMI_DATA_HDR_AMPDU_FLUSH_BIT_SHIFT 9
#define WMI_DATA_HDR_SET_AMPDU_FLUSH_BIT(h) \
    ((h)->info3 |= (WMI_DATA_HDR_AMPDU_FLUSH_BIT_MASK << WMI_DATA_HDR_AMPDU_FLUSH_BIT_SHIFT))

#define WMI_DATA_HDR_WLAN_BIT_MASK 0x0
#define WMI_DATA_HDR_WLAN_BIT_SHIFT 10
#define WMI_DATA_HDR_SET_WLAN_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_WLAN_BIT_MASK << WMI_DATA_HDR_WLAN_BIT_SHIFT))

#define WMI_DATA_HDR_WAN_BIT_MASK 0x1
#define WMI_DATA_HDR_WAN_BIT_SHIFT 10
#define WMI_DATA_HDR_SET_WAN_BIT(h) ((h)->info3 |= (WMI_DATA_HDR_WAN_BIT_MASK << WMI_DATA_HDR_WAN_BIT_SHIFT))

#define WMI_DATA_HDR_OUT_OF_ORDER_BIT_MASK 0x1
#define WMI_DATA_HDR_OUT_OF_ORDER_BIT_SHIFT 11
#define WMI_DATA_HDR_SET_OUT_OF_ORDER_BIT(h) \
    ((h)->info3 |= (WMI_DATA_HDR_OUT_OF_ORDER_BIT_MASK << WMI_DATA_HDR_OUT_OF_ORDER_BIT_SHIFT))

#define WMI_DATA_HDR_FLUSH_ALL_BIT_MASK 0x1
#define WMI_DATA_HDR_FLUSH_ALL_BIT_SHIFT 12
#define WMI_DATA_HDR_SET_FLUSH_ALL_BIT(h) \
    ((h)->info3 |= (WMI_DATA_HDR_FLUSH_ALL_BIT_MASK << WMI_DATA_HDR_FLUSH_ALL_BIT_SHIFT))

#define WMI_DATA_HDR_PARTIAL_FLUSH_BIT_MASK 0x1
#define WMI_DATA_HDR_PARTIAL_FLUSH_BIT_SHIFT 13
#define WMI_DATA_HDR_SET_PARTIAL_FLUSH_BIT(h) \
    ((h)->info3 |= (WMI_DATA_HDR_PARTIAL_FLUSH_BIT_MASK << WMI_DATA_HDR_PARTIAL_FLUSH_BIT_SHIFT))

typedef PREPACK struct
{
    int8_t rssi FIELD_PACKED;
    uint8_t info FIELD_PACKED; /* usage of 'info' field(8-bit):
                   *  b1:b0       - WMI_MSG_TYPE
                   *  b4:b3:b2    - UP(tid)
                   *  b5          - Used in AP mode. More-data in tx dir, PS in rx.
                   *  b7:b6       -  Dot3 header(0),
                   *                 Dot11 Header(1),
                   *                 ACL data(2)
                   */

    uint16_t info2 FIELD_PACKED; /* usage of 'info2' field(16-bit):
                     * b11:b0       - seq_no
                     * b12          - A-MSDU?
                     * b15:b13      - META_DATA_VERSION 0 - 7
                     */
    uint16_t info3 FIELD_PACKED; /* b3:b2:b1:b0  - device id
                                  * b4           - Used in AP mode. uAPSD trigger in rx, EOSP in tx
                                  * b5           - Used by CCX.
                                  * b6           - Used in AP mode. uAPSD triggered frame in tx,
                                  *                meaningless in rx
                                  * b7           - Used in AP mode. PS-POLL triggered frame in tx,
                                  *                meaningless in rx
                                  * b15:b8       - pad before data start(irrespective of meta version)
                                  */
} POSTPACK WMI_DATA_HDR;

/*
 *  TX META VERSION DEFINITIONS
 */
#define WMI_MAX_TX_META_SZ (12)
#define WMI_MAX_TX_META_VERSION (7)
#define WMI_META_VERSION_1 (0x01)
#define WMI_META_VERSION_2 (0x02)
#define WMI_META_VERSION_3 (0x03)
#define WMI_META_VERSION_4 (0x04)
#define WMI_META_VERSION_5 (0x05) // USed for stack offload packets

#define WMI_ACL_TO_DOT11_HEADROOM 36

#if 0 /* removed to prevent compile errors for WM.. */
typedef PREPACK struct {
/* intentionally empty. Default version is no meta data. */
} POSTPACK WMI_TX_META_V0;
#endif

typedef PREPACK struct
{
    uint8_t pktID FIELD_PACKED;        /* The packet ID to identify the tx request */
    uint8_t ratePolicyID FIELD_PACKED; /* The rate policy to be used for the tx of this frame */
} POSTPACK WMI_TX_META_V1;

#define WMI_CSUM_DIR_TX (0x1)
#define TX_CSUM_CALC_FILL (0x1)
typedef PREPACK struct
{
    uint8_t csumStart FIELD_PACKED; /*Offset from start of the WMI header for csum calculation to begin */
    uint8_t csumDest FIELD_PACKED;  /*Offset from start of WMI header where final csum goes*/
    uint8_t csumFlags FIELD_PACKED; /*number of bytes over which csum is calculated*/
} POSTPACK WMI_TX_META_V2;

/* WMI_META_TX_FLAG... are used as TX qualifiers for frames containing WMI_TX_RATE_SCHEDULE in the
 * meta data.  0 or more of these flags should be assigned to the flags member of the schedule. */
#define WMI_META_TX_FLAG_ACK 0x01           // frame needs ACK response from receiver
#define WMI_META_TX_FLAG_SET_RETRY_BIT 0x02 // device will set retry bit in MAC header for retried frames.
#define WMI_META_TX_FLAG_SET_DURATION 0x04  // device will fill duration field in MAC header
/* NOTE: If WMI_META_TX_FLAG_USE_PREFIX == 0 device will NOT use prefix frame.
 *       If WMI_META_TX_FLAG_USE_PREFIX == 1 && WMI_META_TX_FLAG_PREFIX_RTS == 0 device will use CTS prefix.
 *       If WMI_META_TX_FLAG_USE_PREFIX == 1 && WMI_META_TX_FLAG_PREFIX_RTS == 1 device will use RTS prefix.
 */
#define WMI_META_TX_FLAG_USE_PREFIX 0x08 // device will send either RTS or CTS frame prior to subject frame.
#define WMI_META_TX_FLAG_PREFIX_RTS 0x10 // device will send RTS and wait for CTS prior to sending subject frame.
#define WMI_META_TX_LOAD_TSF 0x20 // device will fill the TSF field during transmit procedure. <Beacons/probe responses>

/* WMI_TX_RATE_SCHEDULE - Acts as a host-provided rate schedule to replace what would be normally determined
 * by firmware.  This allows the host to specify what rates and attempts should be used to transmit the
 * frame. */
typedef PREPACK struct
{
#define WMI_TX_MAX_RATE_SERIES (4)
    uint8_t rateSeries[WMI_TX_MAX_RATE_SERIES] FIELD_PACKED; // rate index for each series. first invalid rate
                                                             // terminates series.
    uint8_t trySeries[WMI_TX_MAX_RATE_SERIES] FIELD_PACKED; // number of tries for each series.
    uint8_t flags FIELD_PACKED;          // combination of WMI_META_TX_FLAG...
    uint8_t accessCategory FIELD_PACKED; // should be WMM_AC_BE for managment frames and multicast frames.
    // uint8_t keyIndex FIELD_PACKED;
    //
} POSTPACK WMI_TX_RATE_SCHEDULE;

typedef PREPACK struct
{
    WMI_TX_RATE_SCHEDULE rateSched FIELD_PACKED;
    uint8_t pktID FIELD_PACKED; /* The packet ID to identify the tx request */
} POSTPACK WMI_TX_META_V3;

typedef PREPACK struct
{
    uint8_t reserved FIELD_PACKED;
} POSTPACK WMI_TX_META_V5;

/*
 *  RX META VERSION DEFINITIONS
 */
/* if RX meta data is present at all then the meta data field
 *  will consume WMI_MAX_RX_META_SZ bytes of space between the
 *  WMI_DATA_HDR and the payload. How much of the available
 *  Meta data is actually used depends on which meta data
 *  version is active. */
#define WMI_MAX_RX_META_SZ (12)
#define WMI_MAX_RX_META_VERSION (7)

#define WMI_RX_STATUS_OK 0          /* success */
#define WMI_RX_STATUS_DECRYPT_ERR 1 /* decrypt error */
#define WMI_RX_STATUS_MIC_ERR 2     /* tkip MIC error */
#define WMI_RX_STATUS_ERR 3         /* undefined error */

#define WMI_RX_FLAGS_AGGR 0x0001 /* part of AGGR */
#define WMI_RX_FlAGS_STBC 0x0002 /* used STBC */
#define WMI_RX_FLAGS_SGI 0x0004  /* used SGI */
#define WMI_RX_FLAGS_HT 0x0008   /* is HT packet */
                                 /* the flags field is also used to store the CRYPTO_TYPE of the frame
                                  * that value is shifted by WMI_RX_FLAGS_CRYPTO_SHIFT */
#define WMI_RX_FLAGS_CRYPTO_SHIFT 4
#define WMI_RX_FLAGS_CRYPTO_MASK 0x1f
#define WMI_RX_META_GET_CRYPTO(flags) (((flags) >> WMI_RX_FLAGS_CRYPTO_SHIFT) & WMI_RX_FLAGS_CRYPTO_MASK)

#if 0 /* removed to prevent compile errors for WM.. */
typedef PREPACK struct {
/* intentionally empty. Default version is no meta data. */
} POSTPACK WMI_RX_META_VERSION_0;
#endif

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED;  /* one of WMI_RX_STATUS_... */
    uint8_t rix FIELD_PACKED;     /* rate index mapped to rate at which this packet was received. */
    uint8_t rssi FIELD_PACKED;    /* rssi of packet */
    uint8_t channel FIELD_PACKED; /* rf channel during packet reception */
    uint16_t flags FIELD_PACKED;  /* a combination of WMI_RX_FLAGS_... */
} POSTPACK WMI_RX_META_V1;

#define RX_CSUM_VALID_FLAG (0x1)
typedef PREPACK struct
{
    uint16_t csum FIELD_PACKED;
    uint8_t csumFlags FIELD_PACKED; /* bit 0 set -partial csum valid
                          bit 1 set -test mode */
} POSTPACK WMI_RX_META_V2;

/* Macros for operating on WMI_CMD_HDR (info1) field */
#define WMI_CMD_HDR_DEVID_MASK 0xF
#define WMI_CMD_HDR_DEVID_SHIFT 0

#define WMI_GET_DEVICE_ID(info1) ((info1)&0xF)
#define WMI_SET_DEVICE_ID(h, _v)                                                        \
    ((h)->info1 = ((h)->info1 & ~(WMI_CMD_HDR_DEVID_MASK << WMI_CMD_HDR_DEVID_SHIFT)) | \
                  (WMI_GET_DEVICE_ID(_v) << WMI_CMD_HDR_DEVID_SHIFT))

/*
 * Control Path
 */
typedef PREPACK struct
{
    uint16_t commandId FIELD_PACKED;
    /*
     * info1 - 16 bits
     * b03:b00 - id
     * b15:b04 - unused
     */
    uint16_t info1 FIELD_PACKED;

    uint16_t reserved FIELD_PACKED; /* For alignment */
} POSTPACK WMI_CMD_HDR;             /* used for commands and events */

/*
 * List of Commnands
 */
typedef enum
{
    WMI_CONNECT_CMDID = 0x0001,
    WMI_RECONNECT_CMDID = 0x0002,
    WMI_DISCONNECT_CMDID = 0x0003,
    WMI_SYNCHRONIZE_CMDID = 0x0004,
    WMI_START_SCAN_CMDID = 0x0007,
    WMI_SET_SCAN_PARAMS_CMDID = 0x0008,
    WMI_SET_BSS_FILTER_CMDID = 0x0009,
    WMI_SET_PROBED_SSID_CMDID = 0x000a, /* 10 */
    WMI_SET_LISTEN_INT_CMDID = 0x000b,
    WMI_GET_CHANNEL_LIST_CMDID = 0x000e,
    WMI_SET_BEACON_INT_CMDID = 0x000f,
    WMI_GET_STATISTICS_CMDID = 0x0010,
    WMI_SET_CHANNEL_PARAMS_CMDID = 0x0011,
    WMI_SET_POWER_MODE_CMDID = 0x0012,
    WMI_SET_POWER_PARAMS_CMDID = 0x0014, /* 20 */
    WMI_ADD_CIPHER_KEY_CMDID = 0x0016,
    WMI_DELETE_CIPHER_KEY_CMDID = 0x0017,
    WMI_SET_TX_PWR_CMDID = 0x001b,
    WMI_GET_TX_PWR_CMDID = 0x001c,
    WMI_TARGET_ERROR_REPORT_BITMASK_CMDID = 0x0022,
    WMI_SET_ROAM_CTRL_CMDID = 0x0029,
    WMI_EXTENSION_CMDID = 0x002e,
    WMI_SET_RTS_CMDID = 0x0032, /* 50 */
#if MANUFACTURING_SUPPORT
    WMI_TEST_CMDID = 0x003a,
#endif
    WMI_SET_KEEPALIVE_CMDID = 0x003d,
    WMI_GET_KEEPALIVE_CMDID = 0x003e,
    WMI_SET_APPIE_CMDID = 0x003f,
#if ENABLE_AP_MODE
    WMI_AP_SET_NUM_STA_CMDID = 0xf00c,
    WMI_AP_HIDDEN_SSID_CMDID = 0xf00b,
    WMI_AP_CONFIG_COMMIT_CMDID = 0xf00f,
    WMI_AP_CONN_INACT_CMDID = 0xf012,
    RESERVED = 0xf014,
    WMI_AP_SET_DTIM_CMDID = 0xf015,
    WMI_AP_PSBUF_OFFLOAD_CMDID = 0xf0AF,
#endif
    WMI_ALLOW_AGGR_CMDID = 0xf01b, /* F01B */
    WMI_SET_HT_CAP_CMDID = 0xf01e,
    WMI_SET_PMK_CMDID = 0xf028,
#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2
    WMI_SET_CHANNEL_CMDID = 0xf042,
    WMI_GET_PMK_CMDID = 0xf047,
    WMI_SET_PASSPHRASE_CMDID = 0xf048,
    WMI_STORERECALL_CONFIGURE_CMDID = 0xf04e,
    WMI_STORERECALL_RECALL_CMDID = 0xf04f,
    WMI_STORERECALL_HOST_READY_CMDID = 0xf050,

    /* WPS Commands */
    WMI_WPS_START_CMDID = 0xf053,
    WMI_GET_WPS_STATUS_CMDID = 0xf054,
    /*Socket commands*/
    WMI_SOCKET_CMDID = 0xf055,
    WMI_SET_FILTERED_PROMISCUOUS_MODE_CMDID = 0xf056,
#elif ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
    WMI_SET_CHANNEL_CMDID = 0xf042,
    WMI_GET_PMK_CMDID = 0xf047,
    WMI_SET_PASSPHRASE_CMDID = 0xf048,
    WMI_STORERECALL_CONFIGURE_CMDID = 0xf05e,
    WMI_STORERECALL_RECALL_CMDID = 0xf05f,
    WMI_STORERECALL_HOST_READY_CMDID = 0xf060,
    /* WPS Commands */
    WMI_WPS_START_CMDID = 0xf07a,
    WMI_GET_WPS_STATUS_CMDID = 0xf07b,
    /*Socket commands*/
    WMI_SOCKET_CMDID = 0xf08d,
    WMI_SET_FILTERED_PROMISCUOUS_MODE_CMDID = 0xf099,
#endif

#if ENABLE_P2P_MODE
/* P2P CMDS */

#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2 /* Jade */
    WMI_P2P_SET_CONFIG_CMDID = 0xF038,
    WMI_WPS_SET_CONFIG_CMDID = 0xF039,
    WMI_SET_REQ_DEV_ATTR_CMDID = 0xF03A,
    WMI_P2P_FIND_CMDID = 0xF03B,
    WMI_P2P_STOP_FIND_CMDID = 0xF03C,
    WMI_P2P_LISTEN_CMDID = 0xF03D,

    WMI_P2P_SET_CMDID = 0xF057,

    WMI_P2P_SDPD_TX_CMDID = 0xF058, /* F058 */
    WMI_P2P_STOP_SDPD_CMDID = 0xF059,
    WMI_P2P_CANCEL_CMDID = 0xF05a,
    WMI_P2P_CONNECT_CMDID = 0xf05b,
    WMI_P2P_GET_NODE_LIST_CMDID = 0xf05c,
    WMI_P2P_AUTH_GO_NEG_CMDID = 0xf05d,
    WMI_P2P_FW_PROV_DISC_REQ_CMDID = 0xf05e,
    WMI_P2P_PERSISTENT_PROFILE_CMDID = 0xf05f,
    WMI_P2P_SET_JOIN_PROFILE_CMDID = 0xf060,
    WMI_P2P_GRP_INIT_CMDID = 0xf061,
    WMI_P2P_SET_PROFILE_CMDID = 0xf065,
    WMI_P2P_INVITE_CMDID = 0xF070,
    WMI_P2P_INVITE_REQ_RSP_CMDID = 0xF071,
    WMI_P2P_FW_SET_NOA_CMDID = 0xf072,
    WMI_P2P_FW_GET_NOA_CMDID = 0xf073,
    WMI_P2P_FW_SET_OPPPS_CMDID = 0xf074,
    WMI_P2P_FW_GET_OPPPS_CMDID = 0xf075,

#elif ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
    WMI_P2P_SET_CONFIG_CMDID = 0xF038,
    WMI_WPS_SET_CONFIG_CMDID = 0xF039,
    WMI_SET_REQ_DEV_ATTR_CMDID = 0xF03A,
    WMI_P2P_FIND_CMDID = 0xF03B,
    WMI_P2P_STOP_FIND_CMDID = 0xF03C,
    WMI_P2P_LISTEN_CMDID = 0xF03E,

    WMI_P2P_GRP_INIT_CMDID = 0xF051,
    WMI_P2P_GRP_FORMATION_DONE_CMDID = 0xF052,
    WMI_P2P_INVITE_CMDID = 0xF053,
    WMI_P2P_INVITE_REQ_RSP_CMDID = 0xF054,
    WMI_P2P_SET_CMDID = 0xF056,

    WMI_P2P_SDPD_TX_CMDID = 0xF05B, /* F058 */
    WMI_P2P_STOP_SDPD_CMDID = 0xF05C,
    WMI_P2P_CANCEL_CMDID = 0xF05D,
    WMI_P2P_CONNECT_CMDID = 0xf091,
    WMI_P2P_GET_NODE_LIST_CMDID = 0xf092,
    WMI_P2P_AUTH_GO_NEG_CMDID = 0xf093,
    WMI_P2P_FW_PROV_DISC_REQ_CMDID = 0xf094,
    WMI_P2P_PERSISTENT_PROFILE_CMDID = 0xf09D,
    WMI_P2P_SET_JOIN_PROFILE_CMDID = 0xf09E,
    WMI_P2P_SET_PROFILE_CMDID = 0xf0A8,
    WMI_P2P_FW_SET_NOA_CMDID = 0xf0A9,
    WMI_P2P_FW_GET_NOA_CMDID = 0xf0AA,
    WMI_P2P_FW_SET_OPPPS_CMDID = 0xf0AB,
    WMI_P2P_FW_GET_OPPPS_CMDID = 0xf0AC,
    WMI_P2P_LIST_PERSISTENT_NETWORK_CMDID = 0xf0D7,
#endif

#endif
    WMI_SET_BITRATE_CMDID = 0xf000,
    WMI_GET_BITRATE_CMDID = 0xf001,
    /*GreenTx specific commands*/
    WMI_GREENTX_PARAMS_CMDID = 0xF079,
    WMI_LPL_FORCE_ENABLE_CMDID = 0xF072,
    WMI_ARGOS_CMDID = 0xf0B1,
    WMI_SET_CREDIT_REVERSE_CMDID = 0xF0B6,
    WMI_SET_RCV_DATA_CLASSIFIER_CMDID,
    WMI_SET_TX_POWER_SCALE_CMDID = 0xF0D6,

    WMI_PFM_GET_CMDID = 0xF0DC,

    WMI_DSET_HOST_CFG_CMDID = 0xF0DE,
    WMI_DSET_HOST_READ_CMDID = 0xF0DF,

    WMI_HOST_DSET_LARGE_READ_CMDID = 0xF0EA,
    WMI_HOST_DSET_LARGE_WRITE_CMDID = 0xF0EB,
    WMI_HOST_DSET_RESP_CREATE_CMDID = 0xF0EC,
    WMI_HOST_DSET_SYNC_CMDID = 0xF0ED,
    WMI_HOST_DSET_READBACK_CMDID = 0xF0EE,
    WMI_HOST_DSET_RESP_WRITE_CMDID = 0xF0EF,

    WMI_DSET_OP_CMDID = 0xF0F0,
    WMI_GET_TEMPERATURE_CMDID = 0xF0F1,
    WMI_PARAM_SET_CMDID = 0xF0F3,
    WMI_WLAN_SET_PROBEREQ_ENABLE_CMDID = 0xF0F5,
    WMI_GET_COUNTRY_CODE_CMDID = 0xF0F6,
    WMI_WLAN_WPS_INIT_KEY_CMDID = 0xF0F7,
    WMI_HEATBEAT_CHALLENGE = 0xF0F8,
} WMI_COMMAND_ID;

typedef PREPACK struct
{
    uint8_t enable;
} POSTPACK WMI_PROBE_REQ_REPORT_CMD_STRUCT;

typedef PREPACK struct
{
    int32_t param_val;
} POSTPACK WMI_SET_TX_POWER_SCALE_CMD;

/*
 * Frame Types
 */
typedef enum
{
    WMI_FRAME_BEACON = 0,
    WMI_FRAME_PROBE_REQ,
    WMI_FRAME_PROBE_RESP,
    WMI_FRAME_ASSOC_REQ,
    WMI_FRAME_ASSOC_RESP,
    WMI_NUM_MGMT_FRAME
} WMI_MGMT_FRAME_TYPE;

/*
 * Connect Command
 */
typedef enum
{
    INFRA_NETWORK = 0x01,
    ADHOC_NETWORK = 0x02,
    ADHOC_CREATOR = 0x04,
    AP_NETWORK = 0x10,
    NETWORK_CONNECTED_USING_WPS = 0x20
} NETWORK_TYPE;

typedef enum
{
    SUBTYPE_NONE,
    SUBTYPE_BT,
    SUBTYPE_P2PDEV,
    SUBTYPE_P2PCLIENT,
    SUBTYPE_P2PGO
} NETWORK_SUBTYPE;

typedef enum
{
    OPEN_AUTH = 0x01,
    SHARED_AUTH = 0x02,
    LEAP_AUTH = 0x04 /* different from IEEE_AUTH_MODE definitions */
} DOT11_AUTH_MODE;

typedef enum
{
    NONE_AUTH = 0x01,
    WPA_AUTH = 0x02,
    WPA2_AUTH = 0x04,
    WPA_PSK_AUTH = 0x08,
    WPA2_PSK_AUTH = 0x10,
    WPA_AUTH_CCKM = 0x20,
    WPA2_AUTH_CCKM = 0x40
} AUTH_MODE;

typedef enum
{
    NONE_CRYPT = 0x01,
    WEP_CRYPT = 0x02,
    TKIP_CRYPT = 0x04,
    AES_CRYPT = 0x08
#ifdef WAPI_ENABLE
    ,
    WAPI_CRYPT = 0x10
#endif /*WAPI_ENABLE*/
} CRYPTO_TYPE;

#define WMI_MIN_CRYPTO_TYPE NONE_CRYPT
#define WMI_MAX_CRYPTO_TYPE (AES_CRYPT + 1)

#ifdef WAPI_ENABLE
#undef WMI_MAX_CRYPTO_TYPE
#define WMI_MAX_CRYPTO_TYPE (WAPI_CRYPT + 1)
#endif /* WAPI_ENABLE */

#ifdef WAPI_ENABLE
#define IW_ENCODE_ALG_SM4 0x20
#define IW_AUTH_WAPI_ENABLED 0x20
#endif

#define WMI_MIN_KEY_INDEX 0
#define WMI_MAX_KEY_INDEX 3

#ifdef WAPI_ENABLE
#undef WMI_MAX_KEY_INDEX
#define WMI_MAX_KEY_INDEX 7 /* wapi grpKey 0-3, prwKey 4-7 */
#endif                      /* WAPI_ENABLE */

#define WMI_MAX_KEY_LEN 32

#define WMI_MAX_SSID_LEN 32

typedef enum
{
    CONNECT_ASSOC_POLICY_USER = 0x0001,
    CONNECT_SEND_REASSOC = 0x0002,
    CONNECT_IGNORE_WPAx_GROUP_CIPHER = 0x0004,
    CONNECT_PROFILE_MATCH_DONE = 0x0008,
    CONNECT_IGNORE_AAC_BEACON = 0x0010,
    CONNECT_CSA_FOLLOW_BSS = 0x0020,
    CONNECT_DO_WPA_OFFLOAD = 0x0040,
    CONNECT_DO_NOT_DEAUTH = 0x0080,
    CONNECT_WPS_FLAG = 0x0100
} WMI_CONNECT_CTRL_FLAGS_BITS;

#define DEFAULT_CONNECT_CTRL_FLAGS (CONNECT_CSA_FOLLOW_BSS)

typedef PREPACK struct
{
    uint8_t networkType FIELD_PACKED;
    uint8_t dot11AuthMode FIELD_PACKED;
    uint8_t authMode FIELD_PACKED;
    uint8_t pairwiseCryptoType FIELD_PACKED;
    uint8_t pairwiseCryptoLen FIELD_PACKED;
    uint8_t groupCryptoType FIELD_PACKED;
    uint8_t groupCryptoLen FIELD_PACKED;
    uint8_t ssidLength FIELD_PACKED;
    uint8_t ssid[WMI_MAX_SSID_LEN] FIELD_PACKED;
    uint16_t channel FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint32_t ctrl_flags FIELD_PACKED;
} POSTPACK WMI_CONNECT_CMD;

#if ENABLE_AP_MODE
/*
 * Used with WMI_AP_HIDDEN_SSID_CMDID
 */
#define HIDDEN_SSID_FALSE 0
#define HIDDEN_SSID_TRUE 1

typedef PREPACK struct
{
    uint8_t hidden_ssid FIELD_PACKED;
} POSTPACK WMI_AP_HIDDEN_SSID_CMD;

typedef PREPACK struct
{
    uint32_t period FIELD_PACKED;
} POSTPACK WMI_AP_CONN_INACT_CMD;

typedef PREPACK struct
{
    uint16_t beaconInterval FIELD_PACKED;
} POSTPACK WMI_BEACON_INT_CMD;

typedef PREPACK struct
{
    uint8_t dtim FIELD_PACKED;
} POSTPACK WMI_AP_SET_DTIM_CMD;

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED;     /* enable/disable IoE AP PS Offload */
    uint8_t psBufCount FIELD_PACKED; /* PS Buf count per PS Client */
} POSTPACK WMI_AP_PSBUF_OFFLOAD_CMD;
#endif

/*
 * WMI_RECONNECT_CMDID
 */
typedef PREPACK struct
{
    uint16_t channel FIELD_PACKED;           /* hint */
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED; /* mandatory if set */
} POSTPACK WMI_RECONNECT_CMD;

#define WMI_PMK_LEN 32
typedef PREPACK struct
{
    uint8_t pmk[WMI_PMK_LEN] FIELD_PACKED;
    uint8_t pmk_len FIELD_PACKED;
} POSTPACK WMI_SET_PMK_CMD, WMI_GET_PMK_REPLY;

#define WMI_PASSPHRASE_LEN 64
typedef PREPACK struct
{
    uint8_t ssid[WMI_MAX_SSID_LEN] FIELD_PACKED;
    uint8_t passphrase[WMI_PASSPHRASE_LEN] FIELD_PACKED;
    uint8_t ssid_len FIELD_PACKED;
    uint8_t passphrase_len FIELD_PACKED;
} POSTPACK WMI_SET_PASSPHRASE_CMD;

/*
 * WMI_ADD_CIPHER_KEY_CMDID
 */
typedef enum
{
    PAIRWISE_USAGE = 0x00,
    GROUP_USAGE = 0x01,
    TX_USAGE = 0x02 /* default Tx Key - Static WEP only */
} KEY_USAGE;

/*
 * Bit Flag
 * Bit 0 - Initialise TSC - default is Initialize
 */
#define KEY_OP_INIT_TSC 0x01
#define KEY_OP_INIT_RSC 0x02
#ifdef WAPI_ENABLE
#define KEY_OP_INIT_WAPIPN 0x10
#endif /* WAPI_ENABLE */

#define KEY_OP_INIT_VAL 0x03 /* Default Initialise the TSC & RSC */
#define KEY_OP_VALID_MASK 0x03

typedef PREPACK struct
{
    uint8_t keyIndex FIELD_PACKED;
    uint8_t keyType FIELD_PACKED;
    uint8_t keyUsage FIELD_PACKED; /* KEY_USAGE */
    uint8_t keyLength FIELD_PACKED;
    uint8_t keyRSC[8] FIELD_PACKED; /* key replay sequence counter */
    uint8_t key[WMI_MAX_KEY_LEN] FIELD_PACKED;
    uint8_t key_op_ctrl FIELD_PACKED; /* Additional Key Control information */
    uint8_t key_macaddr[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_ADD_CIPHER_KEY_CMD;

/*
 * WMI_DELETE_CIPHER_KEY_CMDID
 */
typedef PREPACK struct
{
    uint8_t keyIndex FIELD_PACKED;
} POSTPACK WMI_DELETE_CIPHER_KEY_CMD;

/*
 * WMI_START_SCAN_CMD
 */
typedef enum
{
    WMI_LONG_SCAN = 0,
    WMI_SHORT_SCAN = 1
} WMI_SCAN_TYPE;

typedef PREPACK struct
{
    boolean forceFgScan FIELD_PACKED;
    boolean isLegacy FIELD_PACKED;           /* For Legacy Cisco AP compatibility */
    uint32_t homeDwellTime FIELD_PACKED;     /* Maximum duration in the home channel(milliseconds) */
    uint32_t forceScanInterval FIELD_PACKED; /* Time interval between scans (milliseconds)*/
    uint8_t scanType FIELD_PACKED;           /* WMI_SCAN_TYPE */
    uint8_t numChannels FIELD_PACKED;        /* how many channels follow */
    uint16_t channelList[1] FIELD_PACKED;    /* channels in Mhz */
} POSTPACK WMI_START_SCAN_CMD;

/*
 * WMI_SET_SCAN_PARAMS_CMDID
 */
#define WMI_SHORTSCANRATIO_DEFAULT 3
/*
 *  Warning: ScanCtrlFlag value of 0xFF is used to disable all flags in WMI_SCAN_PARAMS_CMD
 *  Do not add any more flags to WMI_SCAN_CTRL_FLAG_BITS
 */
typedef enum
{
    CONNECT_SCAN_CTRL_FLAGS = 0x01,   /* set if can scan in the Connect cmd */
    SCAN_CONNECTED_CTRL_FLAGS = 0x02, /* set if scan for the SSID it is */
                                      /* already connected to */
    ACTIVE_SCAN_CTRL_FLAGS = 0x04,    /* set if enable active scan */
    ROAM_SCAN_CTRL_FLAGS = 0x08,      /* set if enable roam scan when bmiss and lowrssi */
    REPORT_BSSINFO_CTRL_FLAGS = 0x10, /* set if follows customer BSSINFO reporting rule */
    ENABLE_AUTO_CTRL_FLAGS = 0x20,    /* if disabled, target doesn't
                                        scan after a disconnect event  */
    ENABLE_SCAN_ABORT_EVENT =
        0x40 /* Scan complete event with canceled status will be generated when a scan is prempted before it gets
                completed */
} WMI_SCAN_CTRL_FLAGS_BITS;

#define CAN_SCAN_IN_CONNECT(flags) (flags & CONNECT_SCAN_CTRL_FLAGS)
#define CAN_SCAN_CONNECTED(flags) (flags & SCAN_CONNECTED_CTRL_FLAGS)
#define ENABLE_ACTIVE_SCAN(flags) (flags & ACTIVE_SCAN_CTRL_FLAGS)
#define ENABLE_ROAM_SCAN(flags) (flags & ROAM_SCAN_CTRL_FLAGS)
#define CONFIG_REPORT_BSSINFO(flags) (flags & REPORT_BSSINFO_CTRL_FLAGS)
#define IS_AUTO_SCAN_ENABLED(flags) (flags & ENABLE_AUTO_CTRL_FLAGS)
#define SCAN_ABORT_EVENT_ENABLED(flags) (flags & ENABLE_SCAN_ABORT_EVENT)

#define DEFAULT_SCAN_CTRL_FLAGS                                                                            \
    (CONNECT_SCAN_CTRL_FLAGS | SCAN_CONNECTED_CTRL_FLAGS | ACTIVE_SCAN_CTRL_FLAGS | ROAM_SCAN_CTRL_FLAGS | \
     ENABLE_AUTO_CTRL_FLAGS)

typedef PREPACK struct
{
    uint16_t fg_start_period FIELD_PACKED;     /* seconds */
    uint16_t fg_end_period FIELD_PACKED;       /* seconds */
    uint16_t bg_period FIELD_PACKED;           /* seconds */
    uint16_t maxact_chdwell_time FIELD_PACKED; /* msec */
    uint16_t pas_chdwell_time FIELD_PACKED;    /* msec */
    uint8_t shortScanRatio FIELD_PACKED;       /* how many shorts scan for one long */
    uint8_t scanCtrlFlags FIELD_PACKED;
    uint16_t minact_chdwell_time FIELD_PACKED;  /* msec */
    uint16_t maxact_scan_per_ssid FIELD_PACKED; /* max active scans per ssid */
    uint32_t max_dfsch_act_time FIELD_PACKED;   /* msecs */
} POSTPACK WMI_SCAN_PARAMS_CMD;

/*
 * WMI_SET_BSS_FILTER_CMDID
 */
typedef enum
{
    NONE_BSS_FILTER = 0x0,  /* no beacons forwarded */
    ALL_BSS_FILTER,         /* all beacons forwarded */
    PROFILE_FILTER,         /* only beacons matching profile */
    ALL_BUT_PROFILE_FILTER, /* all but beacons matching profile */
    CURRENT_BSS_FILTER,     /* only beacons matching current BSS */
    ALL_BUT_BSS_FILTER,     /* all but beacons matching BSS */
    PROBED_SSID_FILTER,     /* beacons matching probed ssid */
    LAST_BSS_FILTER         /* marker only */
} WMI_BSS_FILTER;

typedef PREPACK struct
{
    uint8_t bssFilter FIELD_PACKED;  /* see WMI_BSS_FILTER */
    uint8_t reserved1 FIELD_PACKED;  /* For alignment */
    uint16_t reserved2 FIELD_PACKED; /* For alignment */
    uint32_t ieMask FIELD_PACKED;
} POSTPACK WMI_BSS_FILTER_CMD;

/*
 * WMI_SET_PROBED_SSID_CMDID
 */
#define MAX_PROBED_SSID_INDEX 9

typedef enum
{
    DISABLE_SSID_FLAG = 0,     /* disables entry */
    SPECIFIC_SSID_FLAG = 0x01, /* probes specified ssid */
    ANY_SSID_FLAG = 0x02       /* probes for any ssid */
} WMI_SSID_FLAG;

typedef PREPACK struct
{
    uint8_t entryIndex FIELD_PACKED; /* 0 to MAX_PROBED_SSID_INDEX */
    uint8_t flag FIELD_PACKED;       /* WMI_SSID_FLG */
    uint8_t ssidLength FIELD_PACKED;
    uint8_t ssid[32] FIELD_PACKED;
} POSTPACK WMI_PROBED_SSID_CMD;

/*
 * WMI_SET_LISTEN_INT_CMDID
 * The Listen interval is between 15 and 3000 TUs
 */
#define MIN_LISTEN_INTERVAL 15
#define MAX_LISTEN_INTERVAL 5000
#define MIN_LISTEN_BEACONS 1
#define MAX_LISTEN_BEACONS 50

typedef PREPACK struct
{
    uint16_t listenInterval FIELD_PACKED;
    uint16_t numBeacons FIELD_PACKED;
} POSTPACK WMI_LISTEN_INT_CMD;

/*
 * WMI_SET_BMISS_TIME_CMDID
 * valid values are between 1000 and 5000 TUs
 */

#define MIN_BMISS_TIME 1000
#define MAX_BMISS_TIME 5000
#define MIN_BMISS_BEACONS 1
#define MAX_BMISS_BEACONS 50

typedef PREPACK struct
{
    uint16_t bmissTime FIELD_PACKED;
    uint16_t numBeacons FIELD_PACKED;
} POSTPACK WMI_BMISS_TIME_CMD;

/*
 * WMI_SET_POWER_MODE_CMDID
 */
typedef enum
{
    REC_POWER = 0x01,
    MAX_PERF_POWER
} WMI_POWER_MODE;

typedef PREPACK struct
{
    uint8_t powerMode FIELD_PACKED; /* WMI_POWER_MODE */
} POSTPACK WMI_POWER_MODE_CMD;

typedef PREPACK struct
{
    int8_t status FIELD_PACKED; /* WMI_SET_PARAMS_REPLY */
} POSTPACK WMI_SET_PARAMS_REPLY;

/*
 * WMI_SET_POWER_PARAMS_CMDID
 */
typedef enum
{
    IGNORE_DTIM = 0x01,
    NORMAL_DTIM = 0x02,
    STICK_DTIM = 0x03,
    AUTO_DTIM = 0x04
} WMI_DTIM_POLICY;

/* Policy to determnine whether TX should wakeup WLAN if sleeping */
typedef enum
{
    TX_WAKEUP_UPON_SLEEP = 1,
    TX_DONT_WAKEUP_UPON_SLEEP = 2
} WMI_TX_WAKEUP_POLICY_UPON_SLEEP;

/*
 * Policy to determnine whether power save failure event should be sent to
 * host during scanning
 */
typedef enum
{
    SEND_POWER_SAVE_FAIL_EVENT_ALWAYS = 1,
    IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN = 2
} POWER_SAVE_FAIL_EVENT_POLICY;

typedef PREPACK struct
{
    uint16_t idle_period FIELD_PACKED; /* msec */
    uint16_t pspoll_number FIELD_PACKED;
    uint16_t dtim_policy FIELD_PACKED;
    uint16_t tx_wakeup_policy FIELD_PACKED;
    uint16_t num_tx_to_wakeup FIELD_PACKED;
    uint16_t ps_fail_event_policy FIELD_PACKED;
} POSTPACK WMI_POWER_PARAMS_CMD;

/* Adhoc power save types */
typedef enum
{
    ADHOC_PS_DISABLE = 1,
    ADHOC_PS_ATH = 2,
    ADHOC_PS_IEEE = 3,
    ADHOC_PS_OTHER = 4
} WMI_ADHOC_PS_TYPE;

/*
 * WMI_SET_DISC_TIMEOUT_CMDID
 */
typedef PREPACK struct
{
    uint8_t disconnectTimeout FIELD_PACKED; /* seconds */
} POSTPACK WMI_DISC_TIMEOUT_CMD;

typedef enum
{
    UPLINK_TRAFFIC = 0,
    DNLINK_TRAFFIC = 1,
    BIDIR_TRAFFIC = 2
} DIR_TYPE;

typedef enum
{
    DISABLE_FOR_THIS_AC = 0,
    ENABLE_FOR_THIS_AC = 1,
    ENABLE_FOR_ALL_AC = 2
} VOICEPS_CAP_TYPE;

typedef enum
{
    TRAFFIC_TYPE_APERIODIC = 0,
    TRAFFIC_TYPE_PERIODIC = 1
} TRAFFIC_TYPE;

/*
 * WMI_SYNCHRONIZE_CMDID
 */
typedef PREPACK struct
{
    uint8_t dataSyncMap FIELD_PACKED;
} POSTPACK WMI_SYNC_CMD;

/*
 * WMI_CREATE_PSTREAM_CMDID
 */
typedef PREPACK struct
{
    uint32_t minServiceInt FIELD_PACKED; /* in milli-sec */
    uint32_t maxServiceInt FIELD_PACKED; /* in milli-sec */
    uint32_t inactivityInt FIELD_PACKED; /* in milli-sec */
    uint32_t suspensionInt FIELD_PACKED; /* in milli-sec */
    uint32_t serviceStartTime FIELD_PACKED;
    uint32_t minDataRate FIELD_PACKED;  /* in bps */
    uint32_t meanDataRate FIELD_PACKED; /* in bps */
    uint32_t peakDataRate FIELD_PACKED; /* in bps */
    uint32_t maxBurstSize FIELD_PACKED;
    uint32_t delayBound FIELD_PACKED;
    uint32_t minPhyRate FIELD_PACKED; /* in bps */
    uint32_t sba FIELD_PACKED;
    uint32_t mediumTime FIELD_PACKED;
    uint16_t nominalMSDU FIELD_PACKED; /* in octects */
    uint16_t maxMSDU FIELD_PACKED;     /* in octects */
    uint8_t trafficClass FIELD_PACKED;
    uint8_t trafficDirection FIELD_PACKED; /* DIR_TYPE */
    uint8_t rxQueueNum FIELD_PACKED;
    uint8_t trafficType FIELD_PACKED;       /* TRAFFIC_TYPE */
    uint8_t voicePSCapability FIELD_PACKED; /* VOICEPS_CAP_TYPE */
    uint8_t tsid FIELD_PACKED;
    uint8_t userPriority FIELD_PACKED; /* 802.1D user priority */
    uint8_t nominalPHY FIELD_PACKED;   /* nominal phy rate */
} POSTPACK WMI_CREATE_PSTREAM_CMD;

/*
 * WMI_DELETE_PSTREAM_CMDID
 */
typedef PREPACK struct
{
    uint8_t txQueueNumber FIELD_PACKED;
    uint8_t rxQueueNumber FIELD_PACKED;
    uint8_t trafficDirection FIELD_PACKED;
    uint8_t trafficClass FIELD_PACKED;
    uint8_t tsid FIELD_PACKED;
} POSTPACK WMI_DELETE_PSTREAM_CMD;

/*
 * WMI_SET_CHANNEL_PARAMS_CMDID
 */
typedef enum
{
    WMI_11A_MODE = 0x1,
    WMI_11G_MODE = 0x2,
    WMI_11AG_MODE = 0x3,
    WMI_11B_MODE = 0x4,
    WMI_11GONLY_MODE = 0x5
} WMI_PHY_MODE;

#define WMI_MAX_CHANNELS 32

typedef PREPACK struct
{
    uint8_t reserved1 FIELD_PACKED;
    uint8_t scanParam FIELD_PACKED;       /* set if enable scan */
    uint8_t phyMode FIELD_PACKED;         /* see WMI_PHY_MODE */
    uint8_t numChannels FIELD_PACKED;     /* how many channels follow */
    uint16_t channelList[1] FIELD_PACKED; /* channels in Mhz */
} POSTPACK WMI_CHANNEL_PARAMS_CMD;

typedef enum
{
    WMI_LPREAMBLE_DISABLED = 0,
    WMI_LPREAMBLE_ENABLED
} WMI_LPREAMBLE_STATUS;

typedef enum
{
    WMI_IGNORE_BARKER_IN_ERP = 0,
    WMI_DONOT_IGNORE_BARKER_IN_ERP
} WMI_PREAMBLE_POLICY;

typedef PREPACK struct
{
    uint16_t threshold FIELD_PACKED;
} POSTPACK WMI_SET_RTS_CMD;

/*
 *  WMI_TARGET_ERROR_REPORT_BITMASK_CMDID
 *  Sets the error reporting event bitmask in target. Target clears it
 *  upon an error. Subsequent errors are counted, but not reported
 *  via event, unless the bitmask is set again.
 */
typedef PREPACK struct
{
    uint32_t bitmask FIELD_PACKED;
} POSTPACK WMI_TARGET_ERROR_REPORT_BITMASK;

/*
 * WMI_SET_TX_PWR_CMDID
 */
typedef PREPACK struct
{
    uint8_t dbM FIELD_PACKED; /* in dbM units */
} POSTPACK WMI_SET_TX_PWR_CMD, WMI_TX_PWR_REPLY;

/*
 * WMI_GET_TX_PWR_CMDID does not take any parameters
 */

/*
 * WMI_SET_ACCESS_PARAMS_CMDID
 */
#define WMI_DEFAULT_TXOP_ACPARAM 0    /* implies one MSDU */
#define WMI_DEFAULT_ECWMIN_ACPARAM 4  /* corresponds to CWmin of 15 */
#define WMI_DEFAULT_ECWMAX_ACPARAM 10 /* corresponds to CWmax of 1023 */
#define WMI_MAX_CW_ACPARAM 15         /* maximum eCWmin or eCWmax */
#define WMI_DEFAULT_AIFSN_ACPARAM 2
#define WMI_MAX_AIFSN_ACPARAM 15
typedef PREPACK struct
{
    uint16_t txop FIELD_PACKED; /* in units of 32 usec */
    uint8_t eCWmin FIELD_PACKED;
    uint8_t eCWmax FIELD_PACKED;
    uint8_t aifsn FIELD_PACKED;
    uint8_t ac FIELD_PACKED;
} POSTPACK WMI_SET_ACCESS_PARAMS_CMD;

typedef PREPACK struct
{
    uint32_t sleepState FIELD_PACKED;
} WMI_REPORT_SLEEP_STATE_EVENT;

/*
 * Command Replies
 */

/*
 * WMI_GET_CHANNEL_LIST_CMDID reply
 */
typedef PREPACK struct
{
    uint8_t reserved1 FIELD_PACKED;
    uint8_t numChannels FIELD_PACKED;     /* number of channels in reply */
    uint16_t channelList[1] FIELD_PACKED; /* channel in Mhz */
} POSTPACK WMI_CHANNEL_LIST_REPLY;

typedef enum
{
    A_SUCCEEDED = A_OK,
    A_FAILED_DELETE_STREAM_DOESNOT_EXIST = 250,
    A_SUCCEEDED_MODIFY_STREAM = 251,
    A_FAILED_INVALID_STREAM = 252,
    A_FAILED_MAX_THINSTREAMS = 253,
    A_FAILED_CREATE_REMOVE_PSTREAM_FIRST = 254
} PSTREAM_REPLY_STATUS;

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED; /* PSTREAM_REPLY_STATUS */
    uint8_t txQueueNumber FIELD_PACKED;
    uint8_t rxQueueNumber FIELD_PACKED;
    uint8_t trafficClass FIELD_PACKED;
    uint8_t trafficDirection FIELD_PACKED; /* DIR_TYPE */
} POSTPACK WMI_CRE_PRIORITY_STREAM_REPLY;

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED; /* PSTREAM_REPLY_STATUS */
    uint8_t txQueueNumber FIELD_PACKED;
    uint8_t rxQueueNumber FIELD_PACKED;
    uint8_t trafficDirection FIELD_PACKED; /* DIR_TYPE */
    uint8_t trafficClass FIELD_PACKED;
} POSTPACK WMI_DEL_PRIORITY_STREAM_REPLY;

typedef PREPACK struct
{
    uint32_t tempDegree;
    uint8_t tempRegVal;
} POSTPACK WMI_GET_TEMPERATURE_REPLY;

typedef PREPACK struct
{
    uint32_t  status;
} POSTPACK WMI_WLAN_WPS_INIT_KEY_REPLY;

typedef PREPACK struct
{
    uint8_t country_code[3];
} POSTPACK WMI_GET_COUNTRY_CODE_REPLY;

typedef PREPACK struct
{
    uint8_t bssid[ATH_MAC_LEN];
    int8_t bias;
} POSTPACK WMI_BSS_BIAS;

typedef PREPACK struct
{
    uint8_t numBss;
    WMI_BSS_BIAS bssBias[1];
} POSTPACK WMI_BSS_BIAS_INFO;

typedef PREPACK struct WMI_LOWRSSI_SCAN_PARAMS
{
    uint16_t lowrssi_scan_period;
    int16_t lowrssi_scan_threshold;
    int16_t lowrssi_roam_threshold;
    uint8_t roam_rssi_floor;
    uint8_t reserved[1]; /* For alignment */
} POSTPACK WMI_LOWRSSI_SCAN_PARAMS;

typedef PREPACK struct
{
    PREPACK union
    {
        uint8_t bssid[ATH_MAC_LEN];    /* WMI_FORCE_ROAM */
        uint8_t roamMode;              /* WMI_SET_ROAM_MODE  */
        WMI_BSS_BIAS_INFO bssBiasInfo; /* WMI_SET_HOST_BIAS */
        uint8_t bias5G;                /* WMI_SET_HOST_5G_BIAS */
        WMI_LOWRSSI_SCAN_PARAMS lrScanParams;
    } POSTPACK info;
    uint8_t roamCtrlType;
} POSTPACK WMI_SET_ROAM_CTRL_CMD;
/*
 * List of Events (target to host)
 */
typedef enum
{
    WMI_READY_EVENTID = 0x1001,
    WMI_CONNECT_EVENTID = 0x1002,
    WMI_DISCONNECT_EVENTID = 0x1003,
    WMI_BSSINFO_EVENTID = 0x1004,
    WMI_CMDERROR_EVENTID = 0x1005,
    WMI_REGDOMAIN_EVENTID = 0x1006,
    WMI_PSTREAM_TIMEOUT_EVENTID = 0x1007,
    WMI_NEIGHBOR_REPORT_EVENTID = 0x1008,
    WMI_TKIP_MICERR_EVENTID = 0x1009,
    WMI_SCAN_COMPLETE_EVENTID = 0x100a, /* 0x100a */
    WMI_REPORT_STATISTICS_EVENTID = 0x100b,
    WMI_RSSI_THRESHOLD_EVENTID = 0x100c,
    WMI_ERROR_REPORT_EVENTID = 0x100d,
    WMI_REPORT_ROAM_TBL_EVENTID = 0x100f,
    WMI_EXTENSION_EVENTID = 0x1010,
    WMI_SNR_THRESHOLD_EVENTID = 0x1012,
    WMI_LQ_THRESHOLD_EVENTID = 0x1013,
    WMI_TX_RETRY_ERR_EVENTID = 0x1014, /* 0x1014 */
    WMI_REPORT_ROAM_DATA_EVENTID = 0x1015,
#if MANUFACTURING_SUPPORT
    WMI_TEST_EVENTID = 0x1016,
#endif
    WMI_APLIST_EVENTID = 0x1017,
    WMI_GET_PMKID_LIST_EVENTID = 0x1019,
    WMI_CHANNEL_CHANGE_EVENTID = 0x101a,
    WMI_PEER_NODE_EVENTID = 0x101b,
    WMI_DTIMEXPIRY_EVENTID = 0x101d,
    WMI_WLAN_VERSION_EVENTID = 0x101e,
    WMI_SET_PARAMS_REPLY_EVENTID = 0x101f,
    WMI_ADDBA_REQ_EVENTID = 0x1020, /*0x1020 */
    WMI_ADDBA_RESP_EVENTID = 0x1021,
    WMI_DELBA_REQ_EVENTID = 0x1022,
    WMI_REPORT_SLEEP_STATE_EVENTID = 0x1026,
    WMI_GET_PMK_EVENTID = 0x102a,
    WMI_SET_CHANNEL_EVENTID = 0x9000,
    WMI_RX_PROBE_REQ_EVENTID = 0x900a,

#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2
    WMI_STORERECALL_STORE_EVENTID = 0x9004,
    WMI_WPS_GET_STATUS_EVENTID = 0x9005,
    WMI_WPS_PROFILE_EVENTID = 0x9006,
    WMI_SOCKET_RESPONSE_EVENTID = 0x9007,

    /* P2P Events */
    WMI_P2P_GO_NEG_RESULT_EVENTID = 0x1036, /* 1035 */
    WMI_P2P_INVITE_REQ_EVENTID = 0x103C,
    WMI_P2P_INVITE_RCVD_RESULT_EVENTID = 0x103d,
    WMI_P2P_INVITE_SENT_RESULT_EVENTID = 0x103e,
    WMI_P2P_PROV_DISC_RESP_EVENTID = 0x103f,
    WMI_P2P_PROV_DISC_REQ_EVENTID = 0x1040,
    WMI_P2P_START_SDPD_EVENTID = 0x1045,
    WMI_P2P_SDPD_RX_EVENTID = 0x1046,
    WMI_P2P_NODE_LIST_EVENTID = 0x901a,
    WMI_P2P_REQ_TO_AUTH_EVENTID = 0x901b,
#elif ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
    WMI_STORERECALL_STORE_EVENTID = 0x9003,
    WMI_WPS_GET_STATUS_EVENTID = 0x900e,
    WMI_WPS_PROFILE_EVENTID = 0x900f,
    WMI_SOCKET_RESPONSE_EVENTID = 0x9016,
    WMI_P2P_GO_NEG_RESULT_EVENTID = 0x1036,
    WMI_P2P_INVITE_REQ_EVENTID = 0x103E,
    WMI_P2P_INVITE_RCVD_RESULT_EVENTID = 0x103F,
    WMI_P2P_INVITE_SENT_RESULT_EVENTID = 0x1040,
    WMI_P2P_PROV_DISC_RESP_EVENTID = 0x1041,
    WMI_P2P_PROV_DISC_REQ_EVENTID = 0x1042,
    WMI_P2P_START_SDPD_EVENTID = 0x1045,
    WMI_P2P_SDPD_RX_EVENTID = 0x1046,
    WMI_P2P_NODE_LIST_EVENTID = 0x901a,
    WMI_P2P_REQ_TO_AUTH_EVENTID = 0x901b,
    WMI_P2P_LIST_PERSISTENT_NETWORK_EVENTID = 0x9030,

#endif
    WMI_ARGOS_EVENTID = 0x901f,
    WMI_HOST_DSET_STORE_EVENTID = 0x9031,
    WMI_HOST_DSET_READ_EVENTID = 0x9032,
    WMI_STORERECALL_START_EVENTID = 0x9033,
    WMI_HOST_DSET_CREATE_EVENTID = 0x9038,
    WMI_HOST_DSET_LARGE_WRITE_EVENTID = 0x9039,
    WMI_HOST_DSET_LARGE_READ_EVENTID = 0x903A,
    WMI_HOST_DSET_SYNC_EVENTID = 0x903B,

    WMI_DSET_OP_EVENTID = 0x903C,
    WMI_GET_TEMPERATURE_REPLY_EVENTID = 0x903D,
    WMI_PARAM_SET_REPLY_EVENTID = 0x903E,
    WMI_GET_COUNTRY_CODE_REPLY_EVENTID = 0x9040,
    WMI_WLAN_WPS_INIT_KEY_EVENTID = 0x9041,
} WMI_EVENT_ID;

#if ENABLE_P2P_MODE
#define P2P_EVT_BUF_SIZE 512
typedef PREPACK struct
{
    uint32_t EVENT_ID FIELD_PACKED;
    uint8_t data[P2P_EVT_BUF_SIZE] FIELD_PACKED;
} POSTPACK WMI_EVT_TO_APP;
#endif

typedef struct
{
    uint8_t pwr_module;
    boolean pwr_mode;
} POWER_MODE;

typedef enum
{
    WMI_11A_CAPABILITY = 1,
    WMI_11G_CAPABILITY = 2,
    WMI_11AG_CAPABILITY = 3,
    WMI_11NA_CAPABILITY = 4,
    WMI_11NG_CAPABILITY = 5,
    WMI_11NAG_CAPABILITY = 6,
    // END CAPABILITY
    WMI_11N_CAPABILITY_OFFSET = (WMI_11NA_CAPABILITY - WMI_11A_CAPABILITY)
} WMI_PHY_CAPABILITY;

typedef PREPACK struct
{
    uint8_t macaddr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t phyCapability FIELD_PACKED; /* WMI_PHY_CAPABILITY */
} POSTPACK WMI_READY_EVENT_1;

typedef PREPACK struct
{
    uint32_t sw_version FIELD_PACKED;
    uint32_t abi_version FIELD_PACKED;
    uint8_t macaddr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t phyCapability FIELD_PACKED; /* WMI_PHY_CAPABILITY */
} POSTPACK WMI_READY_EVENT_2;

#define WMI_READY_EVENT WMI_READY_EVENT_2 /* host code */

typedef PREPACK struct
{
    uint32_t freq;
    uint16_t len;
    uint8_t data[1];
} POSTPACK WMI_P2P_RX_PROBE_REQ_EVENT_STRUCT;

/*
 * Connect Event
 */
typedef PREPACK struct
{
    uint16_t channel FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t listenInterval FIELD_PACKED;
    uint16_t beaconInterval FIELD_PACKED;
    uint32_t networkType FIELD_PACKED;
    uint8_t beaconIeLen FIELD_PACKED;
    uint8_t assocReqLen FIELD_PACKED;
    uint8_t assocRespLen FIELD_PACKED;
    uint8_t assocInfo[1] FIELD_PACKED;
} POSTPACK WMI_CONNECT_EVENT;

/*
 * Disconnect Event
 */
typedef enum
{
    NO_NETWORK_AVAIL = 0x01,
    LOST_LINK = 0x02, /* bmiss */
    DISCONNECT_CMD = 0x03,
    BSS_DISCONNECTED = 0x04,
    AUTH_FAILED = 0x05,
    ASSOC_FAILED = 0x06,
    NO_RESOURCES_AVAIL = 0x07,
    CSERV_DISCONNECT = 0x08,
    INVALID_PROFILE = 0x0a,
    DOT11H_CHANNEL_SWITCH = 0x0b,
    PROFILE_MISMATCH = 0x0c,
    CONNECTION_EVICTED = 0x0d,
    IBSS_MERGE = 0xe
} WMI_DISCONNECT_REASON;

typedef PREPACK struct
{
    uint16_t protocolReasonStatus FIELD_PACKED; /* reason code, see 802.11 spec. */
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;    /* set if known */
    uint8_t disconnectReason FIELD_PACKED;      /* see WMI_DISCONNECT_REASON */
    uint8_t assocRespLen FIELD_PACKED;
    uint8_t assocInfo[1] FIELD_PACKED;
} POSTPACK WMI_DISCONNECT_EVENT;

/*
 * BSS Info Event.
 * Mechanism used to inform host of the presence and characteristic of
 * wireless networks present.  Consists of bss info header followed by
 * the beacon or probe-response frame body.  The 802.11 header is not included.
 */
typedef enum
{
    BEACON_FTYPE = 0x1,
    PROBERESP_FTYPE,
    ACTION_MGMT_FTYPE,
    PROBEREQ_FTYPE
} WMI_BI_FTYPE;

enum
{
    BSS_ELEMID_CHANSWITCH = 0x01,
    BSS_ELEMID_ATHEROS = 0x02
};

typedef PREPACK struct
{
    uint16_t channel FIELD_PACKED;
    uint8_t frameType FIELD_PACKED; /* see WMI_BI_FTYPE */
    uint8_t snr FIELD_PACKED;
    int16_t rssi FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint32_t ieMask FIELD_PACKED;
} POSTPACK WMI_BSS_INFO_HDR;

/*
 * BSS INFO HDR version 2.0
 * With 6 bytes HTC header and 6 bytes of WMI header
 * WMI_BSS_INFO_HDR cannot be accomodated in the removed 802.11 management
 * header space.
 * - Reduce the ieMask to 2 bytes as only two bit flags are used
 * - Remove rssi and compute it on the host. rssi = snr - 95
 */
typedef PREPACK struct
{
    uint16_t channel FIELD_PACKED;
    uint8_t frameType FIELD_PACKED; /* see WMI_BI_FTYPE */
    uint8_t snr FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t ieMask FIELD_PACKED;
} POSTPACK WMI_BSS_INFO_HDR2;

/*
 * Command Error Event
 */
typedef enum
{
    INVALID_PARAM = 0x01,
    ILLEGAL_STATE = 0x02,
    INTERNAL_ERROR = 0x03,
    STACK_ERROR = 0x04
} WMI_ERROR_CODE;

typedef PREPACK struct
{
    uint16_t commandId FIELD_PACKED;
    uint8_t errorCode FIELD_PACKED;
} POSTPACK WMI_CMD_ERROR_EVENT;

/*
 * New Regulatory Domain Event
 */
typedef PREPACK struct
{
    uint32_t regDomain FIELD_PACKED;
} POSTPACK WMI_REG_DOMAIN_EVENT;

typedef PREPACK struct
{
    uint8_t txQueueNumber FIELD_PACKED;
    uint8_t rxQueueNumber FIELD_PACKED;
    uint8_t trafficDirection FIELD_PACKED;
    uint8_t trafficClass FIELD_PACKED;
} POSTPACK WMI_PSTREAM_TIMEOUT_EVENT;

/*
 * TKIP MIC Error Event
 */
typedef PREPACK struct
{
    uint8_t keyid FIELD_PACKED;
    uint8_t ismcast FIELD_PACKED;
} POSTPACK WMI_TKIP_MICERR_EVENT;

/*
 * WMI_SCAN_COMPLETE_EVENTID - no parameters (old), staus parameter (new)
 */
typedef PREPACK struct
{
    int32_t status FIELD_PACKED;
} POSTPACK WMI_SCAN_COMPLETE_EVENT;

#define MAX_OPT_DATA_LEN 1400

/*
 * Reporting statistics.
 */
typedef PREPACK struct
{
    uint32_t tx_packets FIELD_PACKED;
    uint32_t tx_bytes FIELD_PACKED;
    uint32_t tx_unicast_pkts FIELD_PACKED;
    uint32_t tx_unicast_bytes FIELD_PACKED;
    uint32_t tx_multicast_pkts FIELD_PACKED;
    uint32_t tx_multicast_bytes FIELD_PACKED;
    uint32_t tx_broadcast_pkts FIELD_PACKED;
    uint32_t tx_broadcast_bytes FIELD_PACKED;
    uint32_t tx_rts_success_cnt FIELD_PACKED;
    uint32_t tx_packet_per_ac[4] FIELD_PACKED;
    uint32_t tx_errors_per_ac[4] FIELD_PACKED;

    uint32_t tx_errors FIELD_PACKED;
    uint32_t tx_failed_cnt FIELD_PACKED;
    uint32_t tx_retry_cnt FIELD_PACKED;
    uint32_t tx_mult_retry_cnt FIELD_PACKED;
    uint32_t tx_rts_fail_cnt FIELD_PACKED;
    int32_t tx_unicast_rate FIELD_PACKED;
} POSTPACK tx_stats_t;

typedef PREPACK struct
{
    uint32_t rx_packets FIELD_PACKED;
    uint32_t rx_bytes FIELD_PACKED;
    uint32_t rx_unicast_pkts FIELD_PACKED;
    uint32_t rx_unicast_bytes FIELD_PACKED;
    uint32_t rx_multicast_pkts FIELD_PACKED;
    uint32_t rx_multicast_bytes FIELD_PACKED;
    uint32_t rx_broadcast_pkts FIELD_PACKED;
    uint32_t rx_broadcast_bytes FIELD_PACKED;
    uint32_t rx_fragment_pkt FIELD_PACKED;

    uint32_t rx_errors FIELD_PACKED;
    uint32_t rx_crcerr FIELD_PACKED;
    uint32_t rx_key_cache_miss FIELD_PACKED;
    uint32_t rx_decrypt_err FIELD_PACKED;
    uint32_t rx_duplicate_frames FIELD_PACKED;
    int32_t rx_unicast_rate FIELD_PACKED;
} POSTPACK rx_stats_t;

typedef PREPACK struct
{
    uint32_t tkip_local_mic_failure FIELD_PACKED;
    uint32_t tkip_counter_measures_invoked FIELD_PACKED;
    uint32_t tkip_replays FIELD_PACKED;
    uint32_t tkip_format_errors FIELD_PACKED;
    uint32_t ccmp_format_errors FIELD_PACKED;
    uint32_t ccmp_replays FIELD_PACKED;
} POSTPACK tkip_ccmp_stats_t FIELD_PACKED;

typedef PREPACK struct
{
    uint32_t power_save_failure_cnt FIELD_PACKED;
    uint16_t stop_tx_failure_cnt FIELD_PACKED;
    uint16_t atim_tx_failure_cnt FIELD_PACKED;
    uint16_t atim_rx_failure_cnt FIELD_PACKED;
    uint16_t bcn_rx_failure_cnt FIELD_PACKED;
} POSTPACK pm_stats_t;

typedef PREPACK struct
{
    uint32_t cs_bmiss_cnt FIELD_PACKED;
    uint32_t cs_lowRssi_cnt FIELD_PACKED;
    uint16_t cs_connect_cnt FIELD_PACKED;
    uint16_t cs_disconnect_cnt FIELD_PACKED;
    int16_t cs_aveBeacon_rssi FIELD_PACKED;
    uint16_t cs_roam_count FIELD_PACKED;
    int16_t cs_rssi FIELD_PACKED;
    uint8_t cs_snr FIELD_PACKED;
    uint8_t cs_aveBeacon_snr FIELD_PACKED;
    uint8_t cs_lastRoam_msec FIELD_PACKED;
} POSTPACK cserv_stats_t;

typedef PREPACK struct
{
    tx_stats_t tx_stats FIELD_PACKED;
    rx_stats_t rx_stats FIELD_PACKED;
    tkip_ccmp_stats_t tkipCcmpStats FIELD_PACKED;
} POSTPACK wlan_net_stats_t;

typedef PREPACK struct
{
    uint32_t arp_received FIELD_PACKED;
    uint32_t arp_matched FIELD_PACKED;
    uint32_t arp_replied FIELD_PACKED;
} POSTPACK arp_stats_t;

typedef PREPACK struct
{
    uint32_t wow_num_pkts_dropped FIELD_PACKED;
    uint16_t wow_num_events_discarded FIELD_PACKED;
    uint8_t wow_num_host_pkt_wakeups FIELD_PACKED;
    uint8_t wow_num_host_event_wakeups FIELD_PACKED;
} POSTPACK wlan_wow_stats_t;

typedef PREPACK struct
{
    uint32_t lqVal FIELD_PACKED;
    int32_t noise_floor_calibation FIELD_PACKED;
    pm_stats_t pmStats FIELD_PACKED;
    wlan_net_stats_t txrxStats FIELD_PACKED;
    wlan_wow_stats_t wowStats FIELD_PACKED;
    arp_stats_t arpStats FIELD_PACKED;
    cserv_stats_t cservStats FIELD_PACKED;
} POSTPACK WMI_TARGET_STATS;

/*
 *  WMI_ERROR_REPORT_EVENTID
 */
typedef enum
{
    WMI_TARGET_PM_ERR_FAIL = 0x00000001,
    WMI_TARGET_KEY_NOT_FOUND = 0x00000002,
    WMI_TARGET_DECRYPTION_ERR = 0x00000004,
    WMI_TARGET_BMISS = 0x00000008,
    WMI_PSDISABLE_NODE_JOIN = 0x00000010,
    WMI_TARGET_COM_ERR = 0x00000020,
    WMI_TARGET_FATAL_ERR = 0x00000040
} WMI_TARGET_ERROR_VAL;

typedef PREPACK struct
{
    uint32_t errorVal FIELD_PACKED;
} POSTPACK WMI_TARGET_ERROR_REPORT_EVENT;

typedef PREPACK struct
{
    uint8_t retrys FIELD_PACKED;
} POSTPACK WMI_TX_RETRY_ERR_EVENT;

/*
 * developer commands
 */

#if 1
static const int32_t wmi_rateTable[][2] = {
    //{W/O SGI, with SGI}
    {1000, 1000},   {2000, 2000},     {5500, 5500},     {11000, 11000},  {6000, 6000},   {9000, 9000},
    {12000, 12000}, {18000, 18000},   {24000, 24000},   {36000, 36000},  {48000, 48000}, {54000, 54000},
    {6500, 7200},   {13000, 14400},   {19500, 21700},   {26000, 28900},  {39000, 43300}, {52000, 57800},
    {58500, 65000}, {65000, 72200},   {13500, 15000},   {27500, 30000},  {40500, 45000}, {54000, 60000},
    {81500, 90000}, {108000, 120000}, {121500, 135000}, {135000, 150000}};
#endif

/*
 * WMI_SET_BITRATE_CMDID
 *
 * Get bit rate cmd uses same definition as set bit rate cmd
 */
typedef enum
{
    RATE_AUTO = -1,
    RATE_1Mb = 0,
    RATE_2Mb = 1,
    RATE_5_5Mb = 2,
    RATE_11Mb = 3,
    RATE_6Mb = 4,
    RATE_9Mb = 5,
    RATE_12Mb = 6,
    RATE_18Mb = 7,
    RATE_24Mb = 8,
    RATE_36Mb = 9,
    RATE_48Mb = 10,
    RATE_54Mb = 11,
    RATE_MCS_0_20 = 12,
    RATE_MCS_1_20 = 13,
    RATE_MCS_2_20 = 14,
    RATE_MCS_3_20 = 15,
    RATE_MCS_4_20 = 16,
    RATE_MCS_5_20 = 17,
    RATE_MCS_6_20 = 18,
    RATE_MCS_7_20 = 19,
    RATE_MCS_0_40 = 20,
    RATE_MCS_1_40 = 21,
    RATE_MCS_2_40 = 22,
    RATE_MCS_3_40 = 23,
    RATE_MCS_4_40 = 24,
    RATE_MCS_5_40 = 25,
    RATE_MCS_6_40 = 26,
    RATE_MCS_7_40 = 27
} WMI_BIT_RATE;

typedef PREPACK struct
{
    int8_t rateIndex FIELD_PACKED; /* see WMI_BIT_RATE */
    int8_t mgmtRateIndex FIELD_PACKED;
    int8_t ctlRateIndex FIELD_PACKED;
} POSTPACK WMI_BIT_RATE_CMD;

typedef PREPACK struct
{
    int8_t rateIndex FIELD_PACKED; /* see WMI_BIT_RATE */
} POSTPACK WMI_BIT_RATE_REPLY;

/*
 * WMI_SET_FIXRATES_CMDID
 *
 * Get fix rates cmd uses same definition as set fix rates cmd
 */
#define FIX_RATE_1Mb ((uint32_t)0x1)
#define FIX_RATE_2Mb ((uint32_t)0x2)
#define FIX_RATE_5_5Mb ((uint32_t)0x4)
#define FIX_RATE_11Mb ((uint32_t)0x8)
#define FIX_RATE_6Mb ((uint32_t)0x10)
#define FIX_RATE_9Mb ((uint32_t)0x20)
#define FIX_RATE_12Mb ((uint32_t)0x40)
#define FIX_RATE_18Mb ((uint32_t)0x80)
#define FIX_RATE_24Mb ((uint32_t)0x100)
#define FIX_RATE_36Mb ((uint32_t)0x200)
#define FIX_RATE_48Mb ((uint32_t)0x400)
#define FIX_RATE_54Mb ((uint32_t)0x800)
#define FIX_RATE_MCS_0_20 ((uint32_t)0x1000)
#define FIX_RATE_MCS_1_20 ((uint32_t)0x2000)
#define FIX_RATE_MCS_2_20 ((uint32_t)0x4000)
#define FIX_RATE_MCS_3_20 ((uint32_t)0x8000)
#define FIX_RATE_MCS_4_20 ((uint32_t)0x10000)
#define FIX_RATE_MCS_5_20 ((uint32_t)0x20000)
#define FIX_RATE_MCS_6_20 ((uint32_t)0x40000)
#define FIX_RATE_MCS_7_20 ((uint32_t)0x80000)
#define FIX_RATE_MCS_0_40 ((uint32_t)0x100000)
#define FIX_RATE_MCS_1_40 ((uint32_t)0x200000)
#define FIX_RATE_MCS_2_40 ((uint32_t)0x400000)
#define FIX_RATE_MCS_3_40 ((uint32_t)0x800000)
#define FIX_RATE_MCS_4_40 ((uint32_t)0x1000000)
#define FIX_RATE_MCS_5_40 ((uint32_t)0x2000000)
#define FIX_RATE_MCS_6_40 ((uint32_t)0x4000000)
#define FIX_RATE_MCS_7_40 ((uint32_t)0x8000000)

typedef enum
{
    WMI_WMM_DISABLED = 0,
    WMI_WMM_ENABLED
} WMI_WMM_STATUS;

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED;
} POSTPACK WMI_SET_WMM_CMD;

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED;
} POSTPACK WMI_SET_QOS_SUPP_CMD;

typedef enum
{
    WMI_TXOP_DISABLED = 0,
    WMI_TXOP_ENABLED
} WMI_TXOP_CFG;

typedef PREPACK struct
{
    uint8_t txopEnable FIELD_PACKED;
} POSTPACK WMI_SET_WMM_TXOP_CMD;

typedef PREPACK struct
{
    uint8_t keepaliveInterval FIELD_PACKED;
} POSTPACK WMI_SET_KEEPALIVE_CMD;

typedef PREPACK struct
{
    boolean configured FIELD_PACKED;
    uint8_t keepaliveInterval FIELD_PACKED;
} POSTPACK WMI_GET_KEEPALIVE_CMD;

/*
 * Add Application specified IE to a management frame
 */
#define WMI_MAX_IE_LEN  64

typedef PREPACK struct {
    uint8_t mgmtFrmType;  /* one of WMI_MGMT_FRAME_TYPE */
    uint8_t ieLen;    /* Length  of the IE that should be added to the MGMT frame */
    uint8_t ieInfo[1];
} POSTPACK WMI_SET_APPIE_CMD;

typedef PREPACK struct
{
    uint16_t oldChannel FIELD_PACKED;
    uint32_t newChannel FIELD_PACKED;
} POSTPACK WMI_CHANNEL_CHANGE_EVENT;

typedef PREPACK struct
{
    uint32_t version FIELD_PACKED;
} POSTPACK WMI_WLAN_VERSION_EVENT;

/* WMI_ADDBA_REQ_EVENTID */
typedef PREPACK struct
{
    uint8_t tid FIELD_PACKED;
    uint8_t win_sz FIELD_PACKED;
    uint16_t st_seq_no FIELD_PACKED;
    uint8_t status FIELD_PACKED; /* f/w response for ADDBA Req; OK(0) or failure(!=0) */
} POSTPACK WMI_ADDBA_REQ_EVENT;

/* WMI_ADDBA_RESP_EVENTID */
typedef PREPACK struct
{
    uint8_t tid FIELD_PACKED;
    uint8_t status FIELD_PACKED;    /* OK(0), failure (!=0) */
    uint16_t amsdu_sz FIELD_PACKED; /* Three values: Not supported(0), 3839, 8k */
} POSTPACK WMI_ADDBA_RESP_EVENT;

/* WMI_DELBA_EVENTID
 * f/w received a DELBA for peer and processed it.
 * Host is notified of this
 */
typedef PREPACK struct
{
    uint8_t tid FIELD_PACKED;
    uint8_t is_peer_initiator FIELD_PACKED;
    uint16_t reason_code FIELD_PACKED;
} POSTPACK WMI_DELBA_EVENT;

/* WMI_ALLOW_AGGR_CMDID
 * Configures tid's to allow ADDBA negotiations
 * on each tid, in each direction
 */
typedef PREPACK struct
{
    uint16_t tx_allow_aggr FIELD_PACKED; /* 16-bit mask to allow uplink ADDBA negotiation - bit position indicates tid*/
    uint16_t rx_allow_aggr
        FIELD_PACKED; /* 16-bit mask to allow donwlink ADDBA negotiation - bit position indicates tid*/
} POSTPACK WMI_ALLOW_AGGR_CMD;

/* WMI_ADDBA_REQ_CMDID
 * f/w starts performing ADDBA negotiations with peer
 * on the given tid
 */
typedef PREPACK struct
{
    uint8_t tid FIELD_PACKED;
} POSTPACK WMI_ADDBA_REQ_CMD;

/* WMI_DELBA_REQ_CMDID
 * f/w would teardown BA with peer.
 * is_send_initiator indicates if it's or tx or rx side
 */
typedef PREPACK struct
{
    uint8_t tid FIELD_PACKED;
    uint8_t is_sender_initiator FIELD_PACKED;

} POSTPACK WMI_DELBA_REQ_CMD;

#define PEER_NODE_JOIN_EVENT 0x00
#define PEER_NODE_LEAVE_EVENT 0x01
#define PEER_FIRST_NODE_JOIN_EVENT 0x10
#define PEER_LAST_NODE_LEAVE_EVENT 0x11
typedef PREPACK struct
{
    uint8_t eventCode FIELD_PACKED;
    uint8_t peerMacAddr[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_PEER_NODE_EVENT;

#define IEEE80211_FRAME_TYPE_MGT 0x00
#define IEEE80211_FRAME_TYPE_CTL 0x04

typedef PREPACK struct
{
    uint8_t band FIELD_PACKED;   /* specifies which band to apply these values */
    uint8_t enable FIELD_PACKED; /* allows 11n to be disabled on a per band basis */
    uint8_t chan_width_40M_supported FIELD_PACKED;
    uint8_t short_GI_20MHz FIELD_PACKED;
    uint8_t short_GI_40MHz FIELD_PACKED;
    uint8_t intolerance_40MHz FIELD_PACKED;
    uint8_t max_ampdu_len_exp FIELD_PACKED;
} POSTPACK WMI_SET_HT_CAP_CMD;

typedef PREPACK struct
{
    uint8_t sta_chan_width FIELD_PACKED;
} POSTPACK WMI_SET_HT_OP_CMD;

typedef PREPACK struct
{
    uint8_t metaVersion FIELD_PACKED; /* version of meta data for rx packets <0 = default> (0-7 = valid) */
    uint8_t dot11Hdr FIELD_PACKED;    /* 1 == leave .11 header intact , 0 == replace .11 header with .3 <default> */
    uint8_t defragOnHost
        FIELD_PACKED; /* 1 == defragmentation is performed by host, 0 == performed by target <default> */
    uint8_t reserved[1] FIELD_PACKED; /* alignment */
} POSTPACK WMI_RX_FRAME_FORMAT_CMD;

/* STORE / RECALL Commands AND Events DEFINITION START */

#define DSET_STORE_RECALL_START 0x00000900
#define DSET_STORE_RECALL_END 0x000009FF

#define IS_STRRCL_DSET(a) (((a) >= DSET_STORE_RECALL_START) && ((a) <= DSET_STORE_RECALL_END))

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED;
#define STRRCL_RECIPIENT_HOST 1
    uint8_t recipient FIELD_PACKED;
} POSTPACK WMI_STORERECALL_CONFIGURE_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* number of bytes of data to follow */
    uint32_t length FIELD_PACKED;  /* start of data */
} POSTPACK WMI_STORERECALL_RECALL_DSET;

typedef PREPACK struct
{
    uint32_t length FIELD_PACKED; /* number of bytes of data to follow */
    uint8_t data[1] FIELD_PACKED; /* start of data */
} POSTPACK WMI_STORERECALL_RECALL_CMD;

typedef PREPACK struct
{
    uint32_t sleep_msec FIELD_PACKED;
    uint8_t store_after_tx_empty FIELD_PACKED;
    uint8_t store_after_fresh_beacon_rx FIELD_PACKED;
} POSTPACK WMI_STORERECALL_HOST_READY_CMD;

typedef PREPACK struct
{
    uint32_t msec_sleep FIELD_PACKED; /* time between power off/on */
    uint32_t length FIELD_PACKED;     /* length of following data */
    uint8_t data[1] FIELD_PACKED;     /* start of data */
} POSTPACK WMI_STORERECALL_STORE_EVENT;

typedef PREPACK struct
{
    uint32_t length FIELD_PACKED; /* length of following data */
    uint8_t data[1] FIELD_PACKED; /* start of data */
} POSTPACK WMI_HOST_DSET_STORE_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* length of following data */
    uint32_t length FIELD_PACKED;  /* length of following data */
} POSTPACK WMI_HOST_DSET_CREATE_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* length of following data */
    uint32_t offset FIELD_PACKED;  /* length of following data */
    uint32_t length FIELD_PACKED;  /* length of following data */
    uint8_t data[1] FIELD_PACKED;  /* start of data */
} POSTPACK WMI_HOST_DSET_WRITE_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* length of following data */
    uint32_t offset FIELD_PACKED;  /* length of following data */
    uint32_t length FIELD_PACKED;  /* length of following data */
} POSTPACK WMI_HOST_DSET_READBACK_EVENT;

typedef PREPACK struct
{
    uint32_t dset_count FIELD_PACKED; /* length of following data */
} POSTPACK WMI_HOST_DSET_SYNC_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* dset_id */
    uint32_t status FIELD_PACKED;  /* status */
} POSTPACK WMI_DSET_OP_CREATE_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* dset_id */
    uint32_t status FIELD_PACKED;  /* status */
} POSTPACK WMI_DSET_OP_OPEN_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* dset_id */
    uint32_t status FIELD_PACKED;  /* status */
    uint32_t offset FIELD_PACKED;  /* status */
    uint32_t length FIELD_PACKED;  /* status */
    uint8_t data[1];
} POSTPACK WMI_DSET_OP_READ_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* dset_id */
    uint32_t status FIELD_PACKED;  /* status */
} POSTPACK WMI_DSET_OP_WRITE_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* dset_id */
    uint32_t status FIELD_PACKED;  /* status */
} POSTPACK WMI_DSET_OP_COMMIT_EVENT;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* dset_id */
    uint32_t status FIELD_PACKED;  /* status */
} POSTPACK WMI_DSET_OP_CLOSE_EVENT;

typedef PREPACK struct
{
    uint32_t length FIELD_PACKED; /* number of bytes of data to follow */
    uint8_t data[1] FIELD_PACKED; /* start of data */
} POSTPACK WMI_STORERECALL_READ_CMD;

typedef PREPACK struct
{
    uint32_t dset_count FIELD_PACKED; /* number of bytes of data to follow */
    uint8_t data[1] FIELD_PACKED;     /* start of data */
} POSTPACK WMI_HOST_DSET_SYNC_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* length of following data */
    uint32_t length FIELD_PACKED;  /* length of following data */
    uint32_t offset FIELD_PACKED;  /* length of following data */
    uint8_t data[1] FIELD_PACKED;  /* start of data */
} POSTPACK WMI_HOST_DSET_READBACK_CMD;

typedef PREPACK struct
{
    uint32_t flags FIELD_PACKED; /* number of bytes of data to follow */
} POSTPACK WMI_HOST_DSET_RESP_CREATE_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED; /* length of following data */
    uint32_t offset FIELD_PACKED;  /* length of following data */
    uint32_t length FIELD_PACKED;  /* length of following data */
} POSTPACK WMI_HOST_DSET_RESP_WRITE_CMD;

typedef PREPACK struct
{
    uint32_t msec_sleep FIELD_PACKED; /* time between power off/on */
} POSTPACK WMI_STORERECALL_START_EVENT;

/* STORE / RECALL Commands AND Events DEFINITION END */
/* WPS Commands AND Events DEFINITION START */

typedef PREPACK struct
{
    uint8_t ssid[WMI_MAX_SSID_LEN] FIELD_PACKED;
    uint8_t macaddress[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t channel FIELD_PACKED;
    uint8_t ssid_len FIELD_PACKED;
} POSTPACK WPS_SCAN_LIST_ENTRY;

typedef enum
{
    WPS_REGISTRAR_ROLE = 0x1,
    WPS_ENROLLEE_ROLE = 0x2,
    WPS_AP_ENROLLEE_ROLE = 0x3
} WPS_OPER_MODE;

typedef enum
{
    WPS_DO_CONNECT_AFTER_CRED_RECVD = 0x1
} WPS_START_CTRL_FLAG;

#define WPS_PIN_LEN (8)

typedef PREPACK struct
{
    uint8_t pin[WPS_PIN_LEN + 1] FIELD_PACKED;
    uint8_t pin_length FIELD_PACKED;
} POSTPACK WPS_PIN;

typedef enum
{
    WPS_PIN_MODE = 0x1,
    WPS_PBC_MODE = 0x2
} WPS_MODE;

typedef PREPACK struct
{
    WPS_SCAN_LIST_ENTRY ssid_info FIELD_PACKED;
    uint8_t config_mode FIELD_PACKED; /* WPS_MODE */
    WPS_PIN wps_pin FIELD_PACKED;
    uint8_t timeout FIELD_PACKED;  /* in Seconds */
    uint8_t role FIELD_PACKED;     /* WPS_OPER_MOD */
    uint8_t ctl_flag FIELD_PACKED; /* WPS_START_CTRL_FLAG */
} POSTPACK WMI_WPS_START_CMD;

typedef enum
{
    WPS_STATUS_SUCCESS = 0x0,
    WPS_STATUS_FAILURE = 0x1,
    WPS_STATUS_IDLE = 0x2,
    WPS_STATUS_IN_PROGRESS = 0x3
} WPS_STATUS;

typedef PREPACK struct
{
    uint8_t wps_status FIELD_PACKED; /* WPS_STATUS */
    uint8_t wps_state FIELD_PACKED;
} POSTPACK WMI_WPS_GET_STATUS_EVENT;

typedef enum
{
    WPS_ERROR_INVALID_START_INFO = 0x1,
    WPS_ERROR_MULTIPLE_PBC_SESSIONS,
    WPS_ERROR_WALKTIMER_TIMEOUT,
    WPS_ERROR_M2D_RCVD,
    WPS_ERROR_PWD_AUTH_FAIL,
    WPS_ERROR_CANCELLED,
    WPS_ERROR_INVALID_PIN
} WPS_ERROR_CODE;

/* Authentication Type Flags */
#define WPS_CRED_AUTH_OPEN 0x0001
#define WPS_CRED_AUTH_WPAPSK 0x0002
#define WPS_CRED_AUTH_SHARED 0x0004
#define WPS_CRED_AUTH_WPA 0x0008
#define WPS_CRED_AUTH_WPA2 0x0010
#define WPS_CRED_AUTH_WPA2PSK 0x0020

/* Encryption Type Flags */
#define WPS_CRED_ENCR_NONE 0x0001
#define WPS_CRED_ENCR_WEP 0x0002
#define WPS_CRED_ENCR_TKIP 0x0004
#define WPS_CRED_ENCR_AES 0x0008

typedef PREPACK struct
{
    uint16_t ap_channel FIELD_PACKED;
    uint8_t ssid[WMI_MAX_SSID_LEN] FIELD_PACKED;
    uint8_t ssid_len FIELD_PACKED;
    uint16_t auth_type FIELD_PACKED;
    uint16_t encr_type FIELD_PACKED;
    uint8_t key_idx FIELD_PACKED;
    uint8_t key[64] FIELD_PACKED;
    uint8_t key_len FIELD_PACKED;
    uint8_t mac_addr[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WPS_CREDENTIAL;

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED;     /* WPS_STATUS */
    uint8_t error_code FIELD_PACKED; /* WPS_ERROR_CODE */
    WPS_CREDENTIAL credential FIELD_PACKED;
    uint8_t peer_dev_addr[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_WPS_PROFILE_EVENT;
/* WPS Commands AND Events DEFINITION END */

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED; /* 1 == device operates in promiscuous mode , 0 == normal mode <default> */
#define WMI_PROM_FILTER_SRC_ADDR 0x01
#define WMI_PROM_FILTER_DST_ADDR 0x02
    uint8_t filters FIELD_PACKED;
    uint8_t srcAddr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t dstAddr[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_SET_FILTERED_PROMISCUOUS_MODE_CMD;

typedef PREPACK struct
{
    uint16_t channel FIELD_PACKED; /* frequency in Mhz */
} POSTPACK WMI_SET_CHANNEL_CMD;

typedef PREPACK struct
{
    uint32_t cmd_type FIELD_PACKED; /*Socket command type*/
    uint32_t length FIELD_PACKED;   /* number of bytes of data to follow */
    // uint8_t  data[1] FIELD_PACKED;  /* start of data */
} POSTPACK WMI_SOCKET_CMD;

typedef PREPACK struct
{
    uint32_t resp_type FIELD_PACKED;   /*Socket command response type*/
    uint32_t sock_handle FIELD_PACKED; /*Socket handle*/
    uint32_t error FIELD_PACKED;       /*Return value*/
    uint8_t data[1] FIELD_PACKED;      /*data for socket command e.g. select FDs*/
} POSTPACK WMI_SOCK_RESPONSE_EVENT;

#if ENABLE_P2P_MODE

typedef PREPACK struct
{
    uint8_t flag FIELD_PACKED;
} POSTPACK WMI_P2P_SET_CONCURRENT_MODE;

typedef PREPACK struct
{
    uint8_t value FIELD_PACKED;
} POSTPACK WMI_P2P_SET_GO_INTENT;

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED;
} POSTPACK WMI_P2P_SET_CCK_RATES;

typedef PREPACK struct
{
    uint8_t go_intent FIELD_PACKED;
    uint8_t reserved[3] FIELD_PACKED; /* Deprecated 'country' field */
    uint8_t reg_class FIELD_PACKED;
    uint8_t listen_channel FIELD_PACKED;
    uint8_t op_reg_class FIELD_PACKED;
    uint8_t op_channel FIELD_PACKED;
    uint32_t node_age_to FIELD_PACKED;
    uint8_t max_node_count FIELD_PACKED;
} POSTPACK WMI_P2P_FW_SET_CONFIG_CMD;

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED;
} POSTPACK WMI_P2P_SET_PROFILE_CMD;

typedef PREPACK struct
{
    uint16_t categ FIELD_PACKED;
    uint16_t sub_categ FIELD_PACKED;
} POSTPACK device_type_tuple;

typedef PREPACK struct
{
    device_type_tuple pri_dev_type FIELD_PACKED;
    uint8_t pri_device_type[8] FIELD_PACKED;
#define MAX_P2P_SEC_DEVICE_TYPES 5
    device_type_tuple sec_dev_type[MAX_P2P_SEC_DEVICE_TYPES] FIELD_PACKED;
#define WPS_UUID_LEN 16
    uint8_t uuid[WPS_UUID_LEN] FIELD_PACKED;
#define WPS_MAX_DEVNAME_LEN 32
    uint8_t device_name[WPS_MAX_DEVNAME_LEN] FIELD_PACKED;
    uint8_t dev_name_len FIELD_PACKED;
} POSTPACK WMI_WPS_SET_CONFIG_CMD;

typedef PREPACK struct
{
    device_type_tuple pri_dev_type FIELD_PACKED;
    device_type_tuple sec_dev_type[MAX_P2P_SEC_DEVICE_TYPES] FIELD_PACKED;
    uint8_t device_addr[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_SET_REQ_DEV_ATTR_CMD;

#define MAX_LIST_COUNT 8
#define MAX_PASS_LEN 32

typedef PREPACK struct
{
    uint8_t data[1];
} POSTPACK WMI_P2P_PERSISTENT_LIST_NETWORK_EVENT;

typedef PREPACK struct
{
    uint8_t role;
    uint8_t macaddr[ATH_MAC_LEN];
    uint8_t ssid[WMI_MAX_SSID_LEN];
    uint8_t passphrase[MAX_PASS_LEN];
} POSTPACK WMI_PERSISTENT_MAC_LIST;

typedef PREPACK struct
{
    uint8_t ssidLength FIELD_PACKED;
    uint8_t ssid[WMI_MAX_SSID_LEN] FIELD_PACKED;
} POSTPACK P2P_SSID;

typedef enum wmi_p2p_discovery_type
{
    WMI_P2P_FIND_START_WITH_FULL,
    WMI_P2P_FIND_ONLY_SOCIAL,
    WMI_P2P_FIND_PROGRESSIVE
} WMI_P2P_DISC_TYPE;

typedef PREPACK struct
{
    uint32_t timeout FIELD_PACKED;
    uint8_t type FIELD_PACKED;
} POSTPACK WMI_P2P_FIND_CMD;

typedef PREPACK struct
{
    uint16_t listen_freq FIELD_PACKED;
    uint16_t force_freq FIELD_PACKED;
    uint16_t go_oper_freq FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t own_interface_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t member_in_go_dev[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t go_dev_dialog_token FIELD_PACKED;
    P2P_SSID peer_go_ssid FIELD_PACKED;
    uint8_t wps_method FIELD_PACKED;
    uint8_t dev_capab FIELD_PACKED;
    int8_t go_intent FIELD_PACKED;
    uint8_t persistent_grp FIELD_PACKED;
} POSTPACK WMI_P2P_GO_NEG_START_CMD;

typedef PREPACK struct
{
    uint8_t persistent_group FIELD_PACKED;
    uint8_t group_formation FIELD_PACKED;
} POSTPACK WMI_P2P_GRP_INIT_CMD;

typedef PREPACK struct
{
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t grp_formation_status FIELD_PACKED;
} POSTPACK WMI_P2P_GRP_FORMATION_DONE_CMD;

typedef PREPACK struct
{
    uint32_t timeout FIELD_PACKED;
} POSTPACK WMI_P2P_LISTEN_CMD;

typedef PREPACK struct
{
    uint16_t listen_freq FIELD_PACKED;
    uint16_t force_freq FIELD_PACKED;
    uint8_t status FIELD_PACKED;
    int8_t go_intent FIELD_PACKED;
    uint8_t wps_buf[512] FIELD_PACKED;
    uint16_t wps_buflen FIELD_PACKED;
    uint8_t p2p_buf[512] FIELD_PACKED;
    uint16_t p2p_buflen FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t wps_method FIELD_PACKED;
    uint8_t persistent_grp FIELD_PACKED;
} POSTPACK WMI_P2P_GO_NEG_REQ_RSP_CMD;

typedef enum
{
    WMI_P2P_INVITE_ROLE_GO,
    WMI_P2P_INVITE_ROLE_ACTIVE_GO,
    WMI_P2P_INVITE_ROLE_CLIENT
} WMI_P2P_INVITE_ROLE;

typedef PREPACK struct
{
    uint8_t role FIELD_PACKED;
    uint16_t listen_freq FIELD_PACKED;
    uint16_t force_freq FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t go_dev_addr[ATH_MAC_LEN] FIELD_PACKED;
    P2P_SSID ssid FIELD_PACKED;
    uint8_t is_persistent FIELD_PACKED;
    uint8_t wps_method FIELD_PACKED;
} POSTPACK WMI_P2P_INVITE_CMD;

typedef PREPACK struct
{
    uint16_t force_freq FIELD_PACKED;
    uint8_t status FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    //    uint8_t p2p_buf[512];
    uint16_t p2p_buflen FIELD_PACKED;
    uint8_t is_go FIELD_PACKED;
    uint8_t group_bssid[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_P2P_INVITE_REQ_RSP_CMD;

typedef PREPACK struct
{
    uint16_t force_freq FIELD_PACKED;
    uint8_t status FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t is_go FIELD_PACKED;
    uint8_t group_bssid[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_P2P_FW_INVITE_REQ_RSP_CMD;

typedef PREPACK struct
{
    uint16_t wps_method FIELD_PACKED;
    uint16_t listen_freq FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t peer[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t go_dev_addr[ATH_MAC_LEN] FIELD_PACKED;
    P2P_SSID go_oper_ssid FIELD_PACKED;
} POSTPACK WMI_P2P_PROV_DISC_REQ_CMD;

typedef enum
{
    WMI_P2P_CONFID_LISTEN_CHANNEL = 1,
    WMI_P2P_CONFID_CROSS_CONNECT = 2,
    WMI_P2P_CONFID_SSID_POSTFIX = 3,
    WMI_P2P_CONFID_INTRA_BSS = 4,
    WMI_P2P_CONFID_CONCURRENT_MODE = 5,
    WMI_P2P_CONFID_GO_INTENT = 6,
    WMI_P2P_CONFID_DEV_NAME = 7,
    WMI_P2P_CONFID_P2P_OPMODE = 8,
    WMI_P2P_CONFID_CCK_RATES = 9
} WMI_P2P_CONF_ID;

typedef PREPACK struct
{
    uint8_t reg_class FIELD_PACKED;
    uint8_t listen_channel FIELD_PACKED;
} POSTPACK WMI_P2P_LISTEN_CHANNEL;

typedef PREPACK struct
{
    uint8_t flag FIELD_PACKED;
} POSTPACK WMI_P2P_SET_CROSS_CONNECT;

typedef PREPACK struct
{
    uint8_t ssid_postfix[WMI_MAX_SSID_LEN - 9] FIELD_PACKED;
    uint8_t ssid_postfix_len FIELD_PACKED;
} POSTPACK WMI_P2P_SET_SSID_POSTFIX;

typedef PREPACK struct
{
    uint8_t flag FIELD_PACKED;
} POSTPACK WMI_P2P_SET_INTRA_BSS;

#define P2P_DEV (1 << 0)
#define P2P_CLIENT (1 << 1)
#define P2P_GO (1 << 2)
#define P2P_PERSISTENT_FLAG 0x80
typedef PREPACK struct
{
    uint8_t p2pmode FIELD_PACKED;
} POSTPACK WMI_P2P_SET_MODE;

#define RATECTRL_MODE_DEFAULT 0
#define RATECTRL_MODE_PERONLY 1

typedef PREPACK struct
{
    uint32_t mode FIELD_PACKED;
} POSTPACK WMI_SET_RATECTRL_PARM_CMD;

#define WMI_P2P_MAX_TLV_LEN 1024
typedef enum
{
    WMI_P2P_SD_TYPE_GAS_INITIAL_REQ = 0x1,
    WMI_P2P_SD_TYPE_GAS_INITIAL_RESP = 0x2,
    WMI_P2P_SD_TYPE_GAS_COMEBACK_REQ = 0x3,
    WMI_P2P_SD_TYPE_GAS_COMEBACK_RESP = 0x4,
    WMI_P2P_PD_TYPE_RESP = 0x5,
    WMI_P2P_SD_TYPE_STATUS_IND = 0x6
} WMI_P2P_SDPD_TYPE;

typedef enum
{
    WMI_P2P_SDPD_TRANSACTION_PENDING = 0x1,
    WMI_P2P_SDPD_TRANSACTION_COMP = 0x2
} WMI_P2P_SDPD_TRANSACTION_STATUS;

typedef PREPACK struct
{
    uint8_t type FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t frag_id FIELD_PACKED;
    uint8_t reserved1 FIELD_PACKED; /* alignment */
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t freq FIELD_PACKED;
    uint16_t status_code FIELD_PACKED;
    uint16_t comeback_delay FIELD_PACKED;
    uint16_t tlv_length FIELD_PACKED;
    uint16_t update_indic FIELD_PACKED;
    uint16_t total_length FIELD_PACKED;
    uint16_t reserved2 FIELD_PACKED; /* future */
    uint8_t tlv[WMI_P2P_MAX_TLV_LEN] FIELD_PACKED;
} POSTPACK WMI_P2P_SDPD_TX_CMD;

typedef PREPACK struct
{
    uint16_t go_oper_freq FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t own_interface_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t go_dev_dialog_token FIELD_PACKED;
    P2P_SSID peer_go_ssid FIELD_PACKED;
    uint8_t wps_method FIELD_PACKED;
    uint8_t dev_capab FIELD_PACKED;
    uint8_t dev_auth FIELD_PACKED;
    uint8_t go_intent FIELD_PACKED;
} POSTPACK WMI_P2P_FW_CONNECT_CMD_STRUCT;

typedef PREPACK struct
{
    uint16_t wps_method FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t peer[ATH_MAC_LEN] FIELD_PACKED;
} POSTPACK WMI_P2P_FW_PROV_DISC_REQ_CMD;

/* P2P module events */

typedef PREPACK struct
{
    uint8_t num_p2p_dev FIELD_PACKED;
    uint8_t data[1] FIELD_PACKED;
} POSTPACK WMI_P2P_NODE_LIST_EVENT;

typedef PREPACK struct
{
    uint8_t sa[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint16_t dev_password_id FIELD_PACKED;
} POSTPACK WMI_P2P_REQ_TO_AUTH_EVENT;

typedef PREPACK struct
{
    uint8_t sa[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t wps_buf[512] FIELD_PACKED;
    uint16_t wps_buflen FIELD_PACKED;
    uint8_t p2p_buf[512] FIELD_PACKED;
    uint16_t p2p_buflen FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
} POSTPACK WMI_P2P_GO_NEG_REQ_EVENT;

typedef PREPACK struct
{
    uint16_t freq FIELD_PACKED;
    int8_t status FIELD_PACKED;
    uint8_t role_go FIELD_PACKED;
    uint8_t ssid[WMI_MAX_SSID_LEN] FIELD_PACKED;
    uint8_t ssid_len FIELD_PACKED;
#define WMI_MAX_PASSPHRASE_LEN 9
    char pass_phrase[WMI_MAX_PASSPHRASE_LEN] FIELD_PACKED;
    uint8_t peer_device_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t peer_interface_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t wps_method FIELD_PACKED;
    uint8_t persistent_grp FIELD_PACKED;
} POSTPACK WMI_P2P_GO_NEG_RESULT_EVENT;

typedef PREPACK struct
{
    uint8_t sa[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t go_dev_addr[ATH_MAC_LEN] FIELD_PACKED;
    P2P_SSID ssid FIELD_PACKED;
    uint8_t is_persistent FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
} POSTPACK WMI_P2P_FW_INVITE_REQ_EVENT;

typedef PREPACK struct
{
    uint16_t oper_freq FIELD_PACKED;
    uint8_t sa[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t is_bssid_valid FIELD_PACKED;
    uint8_t go_dev_addr[ATH_MAC_LEN] FIELD_PACKED;
    P2P_SSID ssid FIELD_PACKED;
    uint8_t status FIELD_PACKED;
} POSTPACK WMI_P2P_INVITE_RCVD_RESULT_EVENT;

typedef PREPACK struct
{
    uint8_t status FIELD_PACKED;
    uint8_t bssid[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t is_bssid_valid FIELD_PACKED;
} POSTPACK WMI_P2P_INVITE_SENT_RESULT_EVENT;

typedef PREPACK struct
{
    uint8_t sa[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t wps_config_method FIELD_PACKED;
    uint8_t dev_addr[ATH_MAC_LEN] FIELD_PACKED;
#define WPS_DEV_TYPE_LEN 8
#define WPS_MAX_DEVNAME_LEN 32
    uint8_t pri_dev_type[WPS_DEV_TYPE_LEN] FIELD_PACKED;
    uint8_t device_name[WPS_MAX_DEVNAME_LEN] FIELD_PACKED;
    uint8_t dev_name_len FIELD_PACKED;
    uint16_t dev_config_methods FIELD_PACKED;
    uint8_t device_capab FIELD_PACKED;
    uint8_t group_capab FIELD_PACKED;
} POSTPACK WMI_P2P_PROV_DISC_REQ_EVENT;

typedef PREPACK struct
{
    uint8_t peer[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t config_methods FIELD_PACKED;
} POSTPACK WMI_P2P_PROV_DISC_RESP_EVENT;

typedef PREPACK struct
{
    uint8_t type FIELD_PACKED;
    uint8_t transaction_status FIELD_PACKED;
    uint8_t dialog_token FIELD_PACKED;
    uint8_t frag_id FIELD_PACKED;
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint16_t freq FIELD_PACKED;
    uint16_t status_code FIELD_PACKED;
    uint16_t comeback_delay FIELD_PACKED;
    uint16_t tlv_length FIELD_PACKED;
    uint16_t update_indic FIELD_PACKED;
    //  Variable length TLV will be placed after the event
} POSTPACK WMI_P2P_SDPD_RX_EVENT;

typedef PREPACK struct
{
    char wps_pin[WPS_PIN_LEN] FIELD_PACKED;
    uint8_t peer_addr[ATH_MAC_LEN] FIELD_PACKED;
    uint8_t wps_role FIELD_PACKED;
} POSTPACK WMI_P2P_PROV_INFO;

typedef PREPACK struct
{
    WPS_CREDENTIAL credential FIELD_PACKED;
} POSTPACK WMI_P2P_PERSISTENT_PROFILE_CMD;

#endif

#if ENABLE_P2P_MODE

typedef PREPACK struct
{
    uint8_t dev_name[WPS_MAX_DEVNAME_LEN] FIELD_PACKED;
    uint8_t dev_name_len FIELD_PACKED;
} POSTPACK WMI_P2P_SET_DEV_NAME;

typedef PREPACK struct
{
    uint8_t config_id; /* set to one of WMI_P2P_CONF_ID */
    PREPACK union
    {
        WMI_P2P_LISTEN_CHANNEL listen_ch;
        WMI_P2P_SET_CROSS_CONNECT cross_conn;
        WMI_P2P_SET_SSID_POSTFIX ssid_postfix;
        WMI_P2P_SET_INTRA_BSS intra_bss;
        WMI_P2P_SET_CONCURRENT_MODE concurrent_mode;
        WMI_P2P_SET_GO_INTENT go_intent;
        WMI_P2P_SET_DEV_NAME device_name;
        WMI_P2P_SET_MODE mode;
        WMI_P2P_SET_CCK_RATES cck_rates;
    } POSTPACK val;
} POSTPACK WMI_P2P_SET_CMD;

typedef PREPACK struct
{
    uint32_t duration FIELD_PACKED;
    uint32_t interval FIELD_PACKED;
    uint32_t start_or_offset FIELD_PACKED;
    uint8_t count_or_type FIELD_PACKED;
} POSTPACK P2P_NOA_DESCRIPTOR;

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED;
    uint8_t count FIELD_PACKED;
    uint8_t noas[1] FIELD_PACKED;
} POSTPACK WMI_NOA_INFO_STRUCT;

typedef PREPACK struct
{
    uint8_t enable FIELD_PACKED;
    uint8_t ctwin FIELD_PACKED;
} POSTPACK WMI_OPPPS_INFO_STRUCT;

#endif

typedef PREPACK struct
{
    boolean enable FIELD_PACKED;
    uint8_t nextProbeCount FIELD_PACKED;
    uint8_t maxBackOff FIELD_PACKED;
    uint8_t minGtxRssi FIELD_PACKED;
    uint8_t forceBackOff FIELD_PACKED;
} POSTPACK WMI_GREENTX_PARAMS_CMD;

/* LPL commands */
typedef PREPACK struct
{
    uint8_t lplPolicy FIELD_PACKED;       /*0 - force off, 1 force on, 2 dynamic*/
    uint8_t noBlockerDetect FIELD_PACKED; /*don't do blocker detection if lpl policy is set
                                           to dynamic*/
    uint8_t noRfbDetect FIELD_PACKED;     /*don't do rate fall back  detection if lpl policy is set
                                           to dynamic*/
    uint8_t rsvd FIELD_PACKED;
} POSTPACK WMI_LPL_FORCE_ENABLE_CMD;

/*
 * Extended WMI commands are those that are needed during wireless
 * operation, but which are not really wireless commands.  This allows,
 * for instance, platform-specific commands.  Extended WMI commands are
 * embedded in a WMI command message with WMI_COMMAND_ID=WMI_EXTENSION_CMDID.
 * Extended WMI events are similarly embedded in a WMI event message with
 * WMI_EVENT_ID=WMI_EXTENSION_EVENTID.
 */
typedef PREPACK struct
{
    uint32_t commandId FIELD_PACKED;
} POSTPACK WMIX_CMD_HDR;

#define WMIX_GPIO_OUTPUT_SET_CMDID 0x2003
#define WMIX_GPIO_INPUT_GET_CMDID 0x2004
#define WMIX_GPIO_REGISTER_SET_CMDID 0x2005
#define WMIX_GPIO_REGISTER_GET_CMDID 0x2006
#define WMIX_HB_CHALLENGE_RESP_CMDID 0x2008
#define WMIX_DBGLOG_CFG_MODULE_CMDID 0x2009
#define WMIX_GPIO_DATA_EVENTID 0x3005
#define WMIX_HB_CHALLENGE_RESP_EVENTID 0x3007
#define WMIX_DBGLOG_EVENTID 0x3008
#define WMIX_PFM_DATA_EVENTID 0x300C
#define WMIX_PFM_DATA_DONE_EVENTID 0x300D

/*
 * =============GPIO support=================
 * NB: Some of the WMIX APIs use a 32-bit mask.  On Targets that support
 * more than 32 GPIO pins, those APIs only support the first 32 GPIO pins.
 */

/*
 * Set GPIO pin output state.
 * In order for output to be driven, a pin must be enabled for output.
 * This can be done during initialization through the GPIO Configuration
 * DataSet, or during operation with the enable_mask.
 *
 * If a request is made to simultaneously set/clear or set/disable or
 * clear/disable or disable/enable, results are undefined.
 */
typedef PREPACK struct
{
    uint32_t set_mask;     /* pins to set */
    uint32_t clear_mask;   /* pins to clear */
    uint32_t enable_mask;  /* pins to enable for output */
    uint32_t disable_mask; /* pins to disable/tristate */
} POSTPACK WMIX_GPIO_OUTPUT_SET_CMD;

/*
 * Set a GPIO register.  For debug/exceptional cases.
 * Values for gpioreg_id are GPIO_ID_*, defined in a
 * platform-dependent header, gpio.h.
 */
typedef PREPACK struct
{
    uint32_t gpioreg_id; /* GPIO register ID */
    uint32_t value;      /* value to write */
} POSTPACK WMIX_GPIO_REGISTER_SET_CMD;

/* Get a GPIO register.  For debug/exceptional cases. */
typedef PREPACK struct
{
    uint32_t gpioreg_id; /* GPIO register to read */
} POSTPACK WMIX_GPIO_REGISTER_GET_CMD;

/*
 * Host acknowledges and re-arms GPIO interrupts.  A single
 * message should be used to acknowledge all interrupts that
 * were delivered in an earlier WMIX_GPIO_INTR_EVENT message.
 */
typedef PREPACK struct
{
    uint32_t ack_mask; /* interrupts to acknowledge */
} POSTPACK WMIX_GPIO_INTR_ACK_CMD;

/*
 * Target informs Host of GPIO interrupts that have ocurred since the
 * last WMIX_GIPO_INTR_ACK_CMD was received.  Additional information --
 * the current GPIO input values is provided -- in order to support
 * use of a GPIO interrupt as a Data Valid signal for other GPIO pins.
 */
typedef PREPACK struct
{
    uint32_t intr_mask;    /* pending GPIO interrupts */
    uint32_t input_values; /* recent GPIO input values */
} POSTPACK WMIX_GPIO_INTR_EVENT;

/*
 * WMIX_HB_CHALLENGE_RESP_CMDID
 * Heartbeat Challenge Response command
 */
typedef PREPACK struct {
    uint32_t cookie;
    uint32_t source;
} POSTPACK WMIX_HB_CHALLENGE_RESP_CMD;

#define WMIX_HB_CHALLENGE_RESP_EVENT WMIX_HB_CHALLENGE_RESP_CMD

/*
 * Target responds to Host's earlier WMIX_GPIO_INPUT_GET_CMDID request
 * using a GPIO_DATA_EVENT with
 *   value set to the mask of GPIO pin inputs and
 *   reg_id set to GPIO_ID_NONE
 *
 *
 * Target responds to Hosts's earlier WMIX_GPIO_REGISTER_GET_CMDID request
 * using a GPIO_DATA_EVENT with
 *   value set to the value of the requested register and
 *   reg_id identifying the register (reflects the original request)
 * NB: reg_id supports the future possibility of unsolicited
 * WMIX_GPIO_DATA_EVENTs (for polling GPIO input), and it may
 * simplify Host GPIO support.
 */
typedef PREPACK struct
{
    uint32_t value;
    uint32_t reg_id;
} POSTPACK WMIX_GPIO_DATA_EVENT;

/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/

typedef PREPACK struct
{
    uint8_t max_queued_buffers FIELD_PACKED;  /*don't do blocker detection if lpl policy is set to dynamic*/
    uint8_t endpoint_mapping[8] FIELD_PACKED; /*don't do rate fall back  detection if lpl policy is set to dynamic*/
    uint8_t start_credits_per_queue[8] FIELD_PACKED;
    uint8_t num_credit_queues FIELD_PACKED; /*0 - force off, 1 force on, 2 dynamic*/
} POSTPACK WMI_INIT_REVERSE_CREDIT_CMD;

typedef PREPACK struct
{
    uint8_t offset FIELD_PACKED;
    uint8_t shift FIELD_PACKED;
    uint32_t mask FIELD_PACKED;
    uint8_t count FIELD_PACKED;
    uint32_t category_mapping_data[8] FIELD_PACKED;
    uint8_t ep_mapping_data[8] FIELD_PACKED;

} POSTPACK WMI_INIT_RCV_DATA_CLASSIFIER_CMD;

typedef PREPACK struct
{
    uint32_t log_time;
    uint16_t event_id;
    uint16_t unused;
} POSTPACK WMIX_PFM_DATA_EVENT;

typedef PREPACK struct
{
    boolean enable FIELD_PACKED;
} POSTPACK WMID_EVENT_SET_CMD;

typedef struct ath_pmu_param
{
    uint32_t options;
    uint32_t wake_msec;
} ATH_PMU_PARAM;

typedef struct ath_pfm_param
{
    uint32_t options;
} ATH_PFM_PARAM;

typedef struct ath_dset_param
{
    uint32_t dset_id;
    uint32_t offset;
    uint32_t length;

} ATH_DSET_PARAM;

#define MAX_TIMESTAMP 32

typedef PREPACK struct
{
    uint8_t eventID[MAX_TIMESTAMP] FIELD_PACKED;
    uint32_t timestamp[MAX_TIMESTAMP] FIELD_PACKED;
} POSTPACK WMI_PFM_REPORT_EVENT;

typedef struct
{
    uint16_t dset_id;
    uint16_t len;
    uint32_t offset;
} POSTPACK WMI_DSET_HOST_CFG_ITEM;

typedef struct
{
    uint32_t length;
    WMI_DSET_HOST_CFG_ITEM dset[2];
} POSTPACK WMI_DSET_HOST_CFG_CMD;

/*
 * Extended WMIX_DSET commands are those that are needed for host dset
 * operation
 */
typedef PREPACK struct
{
    uint32_t commandId FIELD_PACKED;
} POSTPACK WMIX_DSET_CMD_HDR;

#define WMIX_DSET_CREATE_CMDID 0x1001
#define WMIX_DSET_OPEN_CMDID 0x1002
#define WMIX_DSET_READ_CMDID 0x1003
#define WMIX_DSET_WRITE_CMDID 0x1004
#define WMIX_DSET_COMMIT_CMDID 0x1005
#define WMIX_DSET_CLOSE_CMDID 0x1006
#define WMIX_DSET_SIZE_CMDID 0x1007
#define WMIX_DSET_DELETE_CMDID 0x1008

typedef PREPACK struct
{
    WMIX_DSET_CMD_HDR hdr;
    uint8_t data[1] FIELD_PACKED; /* start of data */
} POSTPACK WMI_DSET_OP_EVENT;

/*
 */
typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
    uint32_t flags FIELD_PACKED;
    uint32_t length FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_CREATE_PARAM_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
    uint32_t flags FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_OPEN_PARAM_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_COMMIT_PARAM_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_CLOSE_PARAM_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
    uint32_t flags FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_DELETE_PARAM_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
    uint32_t flags FIELD_PACKED;
    uint32_t offset FIELD_PACKED;
    uint32_t length FIELD_PACKED;
    uint8_t data[1] FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_WRITE_PARAM_CMD;

typedef PREPACK struct
{
    uint32_t dset_id FIELD_PACKED;
    uint32_t offset FIELD_PACKED;
    uint32_t length FIELD_PACKED;
} POSTPACK WMIX_DSET_OP_READ_PARAM_CMD;

/*Define all  SET/GET Param Groups here  */
#define QCOM_GROUPID_OFFSET 16

#define QCOM_GROUPID_MASK 0xFFFF

#define QCOM_PARAMID_MASK 0xFFFF

#define QCOM_PARAM_MAKE_ID(grpid, paramid) (grpid << QCOM_GROUPID_OFFSET | paramid)

#define QCOM_PARAM_GET_GROUP_ID(which_param) (which_param >> QCOM_GROUPID_OFFSET && QCOM_GROUPID_MASK)

#define QCOM_PARAM_GET_PARAM_ID(which_param) (which_param && QCOM_PARAMID_MASK)

/* WMI_SYS_SET_PARAM_REPLY */

typedef PREPACK struct
{
    A_STATUS status;
} POSTPACK WMI_PARAM_SET_REPLY;

/* WMI_SYS_SET_PARAMS */

typedef PREPACK struct
{
    uint32_t length;
    uint32_t which_param;
    uint8_t data_buffer[1];
} POSTPACK WMI_PARAM_SET_CMD;

#ifdef __cplusplus
}
#endif

#endif /* _WMI_H_ */
