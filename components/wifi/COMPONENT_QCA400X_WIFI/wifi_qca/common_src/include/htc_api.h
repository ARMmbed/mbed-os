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

#ifndef _HTC_API_H_
#define _HTC_API_H_

#include <a_config.h>

typedef uint16_t HTC_SERVICE_ID;

#define HTC_MAILBOX_NUM_MAX 4

/* ------ Endpoint IDS ------ */
typedef enum
{
    ENDPOINT_UNUSED = -1,
    ENDPOINT_0 = 0,
    ENDPOINT_1,
    ENDPOINT_2,
    ENDPOINT_MANAGED_MAX,
    ENDPOINT_3 = ENDPOINT_2 + 1,
    ENDPOINT_4,
    ENDPOINT_5,
#if WLAN_CONFIG_TRUNCATED_ENDPOINTS
    ENDPOINT_MAX
#else
    ENDPOINT_6,
    ENDPOINT_7,
    ENDPOINT_8,
    ENDPOINT_MAX
#endif
} HTC_ENDPOINT_ID;

/* service connection information */
typedef struct _HTC_SERVICE_CONNECT_REQ
{
    HTC_SERVICE_ID ServiceID;      /* service ID to connect to */
    uint16_t ConnectionFlags;      /* connection flags, see htc protocol definition */
    uint8_t *pMetaData;            /* ptr to optional service-specific meta-data */
    uint8_t MetaDataLength;        /* optional meta data length */
    int32_t MaxSendQueueDepth;     /* maximum depth of any send queue */
    uint32_t LocalConnectionFlags; /* HTC flags for the host-side (local) connection */
    uint32_t MaxSendMsgSize;       /* override max message size in send direction */
} HTC_SERVICE_CONNECT_REQ;

#define HTC_LOCAL_CONN_FLAGS_ENABLE_SEND_BUNDLE_PADDING (1 << 0) /* enable send bundle padding for this endpoint */

/* service connection response information */
typedef struct _HTC_SERVICE_CONNECT_RESP
{
    uint8_t *pMetaData;       /* caller supplied buffer to optional meta-data */
    uint8_t BufferLength;     /* length of caller supplied buffer */
    uint8_t ActualLength;     /* actual length of meta data */
    HTC_ENDPOINT_ID Endpoint; /* endpoint to communicate over */
    uint32_t MaxMsgLength;    /* max length of all messages over this endpoint */
    uint8_t ConnectRespCode;  /* connect response code from target */
} HTC_SERVICE_CONNECT_RESP;

#endif /* _HTC_API_H_ */
