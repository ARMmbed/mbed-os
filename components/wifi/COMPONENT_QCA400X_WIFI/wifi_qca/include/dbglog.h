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
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef _DBGLOG_H_
#define _DBGLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DBGLOG_TIMESTAMP_OFFSET 0
#define DBGLOG_TIMESTAMP_MASK            \
    0x0000FFFF /* Bit 0-15. Contains bit \
                  8-23 of the LF0 timer */
#define DBGLOG_DBGID_OFFSET 16
#define DBGLOG_DBGID_MASK 0x03FF0000 /* Bit 16-25 */
#define DBGLOG_DBGID_NUM_MAX 256     /* Upper limit is width of mask */

#define DBGLOG_MODULEID_OFFSET 26
#define DBGLOG_MODULEID_MASK 0x3C000000 /* Bit 26-29 */
#define DBGLOG_MODULEID_NUM_MAX 16      /* Upper limit is width of mask */

/*
 * Please ensure that the definition of any new module intrduced is captured
 * between the DBGLOG_MODULEID_START and DBGLOG_MODULEID_END defines. The
 * structure is required for the parser to correctly pick up the values for
 * different modules.
 */
#define DBGLOG_MODULEID_START
#define DBGLOG_MODULEID_INF 0
#define DBGLOG_MODULEID_WMI 1
#define DBGLOG_MODULEID_MISC 2
#define DBGLOG_MODULEID_PM 3
#define DBGLOG_MODULEID_TXRX_MGMTBUF 4
#define DBGLOG_MODULEID_TXRX_TXBUF 5
#define DBGLOG_MODULEID_TXRX_RXBUF 6
#define DBGLOG_MODULEID_WOW 7
#define DBGLOG_MODULEID_WHAL 8
#define DBGLOG_MODULEID_DC 9
#define DBGLOG_MODULEID_CO 10
#define DBGLOG_MODULEID_RO 11
#define DBGLOG_MODULEID_CM 12
#define DBGLOG_MODULEID_MGMT 13
#define DBGLOG_MODULEID_TMR 14
#define DBGLOG_MODULEID_BTCOEX 15
#define DBGLOG_MODULEID_TLPM 2
#define DBGLOG_MODULEID_END

#define DBGLOG_NUM_ARGS_OFFSET 30
#define DBGLOG_NUM_ARGS_MASK 0xC0000000 /* Bit 30-31 */
#define DBGLOG_NUM_ARGS_MAX 2           /* Upper limit is width of mask */

#define DBGLOG_MODULE_LOG_ENABLE_OFFSET 0
#define DBGLOG_MODULE_LOG_ENABLE_MASK 0x0000FFFF

#define DBGLOG_REPORTING_ENABLED_OFFSET 16
#define DBGLOG_REPORTING_ENABLED_MASK 0x00010000

#define DBGLOG_TIMESTAMP_RESOLUTION_OFFSET 17
#define DBGLOG_TIMESTAMP_RESOLUTION_MASK 0x000E0000

#define DBGLOG_REPORT_SIZE_OFFSET 20
#define DBGLOG_REPORT_SIZE_MASK 0x3FF00000

#define DBGLOG_LOG_BUFFER_SIZE 1500
#define DBGLOG_DBGID_DEFINITION_LEN_MAX 90

struct dbglog_buf_s
{
    struct dbglog_buf_s *next;
    uint8_t *buffer;
    uint32_t bufsize;
    uint32_t length;
    uint32_t count;
    uint32_t free;
};

struct dbglog_hdr_s
{
    struct dbglog_buf_s *dbuf;
    uint32_t dropped;
};

struct dbglog_config_s
{
    uint32_t cfgvalid; /* Mask with valid config bits */
    union
    {
        /* TODO: Take care of endianness */
        struct
        {
            uint32_t mmask : 16; /* Mask of modules with logging on */
            uint32_t rep : 1;    /* Reporting enabled or not */
            uint32_t tsr : 3;    /* Time stamp resolution. Def: 1 ms */
            uint32_t size : 10;  /* Report size in number of messages */
            uint32_t reserved : 2;
        } dbglog_config;

        uint32_t value;
    } u;
};

#define cfgmmask u.dbglog_config.mmask
#define cfgrep u.dbglog_config.rep
#define cfgtsr u.dbglog_config.tsr
#define cfgsize u.dbglog_config.size
#define cfgvalue u.value

#ifdef __cplusplus
}
#endif

#endif /* _DBGLOG_H_ */
