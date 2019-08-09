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

#ifndef __HTC_H__
#define __HTC_H__

#define A_OFFSETOF(type, field) (int)(&(((type *)0)->field))

#define ASSEMBLE_UNALIGNED_UINT16(p, highbyte, lowbyte) \
    (((uint16_t)(((uint8_t *)(p))[(highbyte)])) << 8 | (uint16_t)(((uint8_t *)(p))[(lowbyte)]))

/* alignment independent macros (little-endian) to fetch UINT16s or UINT8s from a
 * structure using only the type and field name.
 * Use these macros if there is the potential for unaligned buffer accesses. */
#define A_GET_UINT16_FIELD(p, type, field) \
    ASSEMBLE_UNALIGNED_UINT16(p, A_OFFSETOF(type, field) + 1, A_OFFSETOF(type, field))

#define A_SET_UINT16_FIELD(p, type, field, value)                                \
    \
{                                                                         \
        ((uint8_t *)(p))[A_OFFSETOF(type, field)] = (uint8_t)(value);            \
        ((uint8_t *)(p))[A_OFFSETOF(type, field) + 1] = (uint8_t)((value) >> 8); \
    \
}

#define A_GET_UINT8_FIELD(p, type, field) ((uint8_t *)(p))[A_OFFSETOF(type, field)]

#define A_SET_UINT8_FIELD(p, type, field, value) ((uint8_t *)(p))[A_OFFSETOF(type, field)] = (value)

/****** DANGER DANGER ***************
 *
 *   The frame header length and message formats defined herein were
 *   selected to accommodate optimal alignment for target processing.  This reduces code
 *   size and improves performance.
 *
 *   Any changes to the header length may alter the alignment and cause exceptions
 *   on the target. When adding to the message structures insure that fields are
 *   properly aligned.
 *
 */

/* HTC frame header */
typedef PREPACK struct _HTC_FRAME_HDR
{
    /* do not remove or re-arrange these fields, these are minimally required
     * to take advantage of 4-byte lookaheads in some hardware implementations */
    uint8_t EndpointID FIELD_PACKED;
    uint8_t Flags FIELD_PACKED;
    uint16_t PayloadLen FIELD_PACKED; /* length of data (including trailer) that follows the header */

    /***** end of 4-byte lookahead ****/

    uint8_t ControlBytes[2] FIELD_PACKED;

    /* message payload starts after the header */

} POSTPACK HTC_FRAME_HDR;

/* frame header flags */

/* send direction */
#define HTC_FLAGS_NEED_CREDIT_UPDATE (1 << 0)
#define HTC_FLAGS_SEND_BUNDLE (1 << 1)  /* start or part of bundle */
#define HTC_FLAGS_SEND_TRAILER (1 << 2) /* send trailer data */

/* receive direction */
#define HTC_FLAGS_RECV_UNUSED_0 (1 << 0)      /* bit 0 unused */
#define HTC_FLAGS_RECV_TRAILER (1 << 1)       /* bit 1 trailer data present */
#define HTC_FLAGS_RECV_UNUSED_2 (1 << 0)      /* bit 2 unused */
#define HTC_FLAGS_RECV_UNUSED_3 (1 << 0)      /* bit 3 unused */
#define HTC_FLAGS_RECV_BUNDLE_CNT_MASK (0xF0) /* bits 7..4  */
#define HTC_FLAGS_RECV_BUNDLE_CNT_SHIFT 4

#define HTC_HDR_LENGTH (sizeof(HTC_FRAME_HDR))
#define HTC_MAX_TRAILER_LENGTH 255
#define HTC_MAX_PAYLOAD_LENGTH (4096 - sizeof(HTC_FRAME_HDR))

/* HTC control message IDs */

#define HTC_MSG_READY_ID 1
#define HTC_MSG_CONNECT_SERVICE_ID 2
#define HTC_MSG_CONNECT_SERVICE_RESPONSE_ID 3
#define HTC_MSG_SETUP_COMPLETE_ID 4
#define HTC_MSG_SETUP_COMPLETE_EX_ID 5

#define HTC_MAX_CONTROL_MESSAGE_LENGTH 256

/* base message ID header */
typedef PREPACK struct
{
    uint16_t MessageID FIELD_PACKED;
} POSTPACK HTC_UNKNOWN_MSG;

/* HTC ready message
 * direction : target-to-host  */
typedef PREPACK struct
{
    uint16_t MessageID FIELD_PACKED;   /* ID */
    uint16_t CreditCount FIELD_PACKED; /* number of credits the target can offer */
    uint16_t CreditSize FIELD_PACKED;  /* size of each credit */
    uint8_t MaxEndpoints FIELD_PACKED; /* maximum number of endpoints the target has resources for */
    uint8_t _Pad1 FIELD_PACKED;
} POSTPACK HTC_READY_MSG;

/* extended HTC ready message */
typedef PREPACK struct
{
    HTC_READY_MSG Version2_0_Info FIELD_PACKED; /* legacy version 2.0 information at the front... */
    /* extended information */
    uint8_t HTCVersion FIELD_PACKED;
    uint8_t MaxMsgsPerHTCBundle FIELD_PACKED;
} POSTPACK HTC_READY_EX_MSG;

#define HTC_VERSION_2P0 0x00
#define HTC_VERSION_2P1 0x01 /* HTC 2.1 */

#define HTC_SERVICE_META_DATA_MAX_LENGTH 128

/* connect service
 * direction : host-to-target */
typedef PREPACK struct
{
    uint16_t MessageID FIELD_PACKED;
    uint16_t ServiceID FIELD_PACKED;       /* service ID of the service to connect to */
    uint16_t ConnectionFlags FIELD_PACKED; /* connection flags */

#define HTC_CONNECT_FLAGS_REDUCE_CREDIT_DRIBBLE \
    (1 << 2) /* reduce credit dribbling when    \
                 the host needs credits */
#define HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK (0x3)
#define HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_ONE_FOURTH 0x0
#define HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_ONE_HALF 0x1
#define HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_THREE_FOURTHS 0x2
#define HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_UNITY 0x3

    uint8_t ServiceMetaLength; /* length of meta data that follows */
    uint8_t _Pad1;

    /* service-specific meta data starts after the header */

} POSTPACK HTC_CONNECT_SERVICE_MSG;

/* connect response
 * direction : target-to-host */
typedef PREPACK struct
{
    uint16_t MessageID FIELD_PACKED;
    uint16_t ServiceID FIELD_PACKED;        /* service ID that the connection request was made */
    uint8_t Status FIELD_PACKED;            /* service connection status */
    uint8_t EndpointID FIELD_PACKED;        /* assigned endpoint ID */
    uint16_t MaxMsgSize FIELD_PACKED;       /* maximum expected message size on this endpoint */
    uint8_t ServiceMetaLength FIELD_PACKED; /* length of meta data that follows */
    uint8_t _Pad1 FIELD_PACKED;

    /* service-specific meta data starts after the header */

} POSTPACK HTC_CONNECT_SERVICE_RESPONSE_MSG;

typedef PREPACK struct
{
    uint16_t MessageID FIELD_PACKED;
    /* currently, no other fields */
} POSTPACK HTC_SETUP_COMPLETE_MSG;

/* extended setup completion message */
typedef PREPACK struct
{
    uint16_t MessageID FIELD_PACKED;
    uint32_t SetupFlags FIELD_PACKED;
    uint8_t MaxMsgsPerBundledRecv FIELD_PACKED;
    uint8_t Rsvd[3] FIELD_PACKED;
} POSTPACK HTC_SETUP_COMPLETE_EX_MSG;

#define HTC_SETUP_COMPLETE_FLAGS_ENABLE_BUNDLE_RECV (1 << 0)

/* connect response status codes */
#define HTC_SERVICE_SUCCESS 0      /* success */
#define HTC_SERVICE_NOT_FOUND 1    /* service could not be found */
#define HTC_SERVICE_FAILED 2       /* specific service failed the connect */
#define HTC_SERVICE_NO_RESOURCES 3 /* no resources (i.e. no more endpoints) */
#define HTC_SERVICE_NO_MORE_EP                     \
    4 /* specific service is not allowing any more \
         endpoints */

/* report record IDs */

#define HTC_RECORD_NULL 0
#define HTC_RECORD_CREDITS 1
#define HTC_RECORD_LOOKAHEAD 2
#define HTC_RECORD_LOOKAHEAD_BUNDLE 3

typedef PREPACK struct
{
    uint8_t RecordID FIELD_PACKED; /* Record ID */
    uint8_t Length FIELD_PACKED;   /* Length of record */
} POSTPACK HTC_RECORD_HDR;

typedef PREPACK struct
{
    uint8_t EndpointID FIELD_PACKED; /* Endpoint that owns these credits */
    uint8_t Credits FIELD_PACKED;    /* credits to report since last report */
} POSTPACK HTC_CREDIT_REPORT;

typedef PREPACK struct
{
    uint8_t PreValid FIELD_PACKED;     /* pre valid guard */
    uint8_t LookAhead[4] FIELD_PACKED; /* 4 byte lookahead */
    uint8_t PostValid FIELD_PACKED;    /* post valid guard */

    /* NOTE: the LookAhead array is guarded by a PreValid and Post Valid guard bytes.
     * The PreValid bytes must equal the inverse of the PostValid byte */

} POSTPACK HTC_LOOKAHEAD_REPORT;

typedef PREPACK struct
{
    uint8_t LookAhead[4] FIELD_PACKED; /* 4 byte lookahead */
} POSTPACK HTC_BUNDLED_LOOKAHEAD_REPORT;

#endif /* __HTC_H__ */
