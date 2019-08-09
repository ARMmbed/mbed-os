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

#ifndef __BMI_MSG_H__
#define __BMI_MSG_H__

/*
 * Bootloader Messaging Interface (BMI)
 *
 * BMI is a very simple messaging interface used during initialization
 * to read memory, write memory, execute code, and to define an
 * application entry PC.
 *
 * It is used to download an application to AR6K, to provide
 * patches to code that is already resident on AR6K, and generally
 * to examine and modify state.  The Host has an opportunity to use
 * BMI only once during bootup.  Once the Host issues a BMI_DONE
 * command, this opportunity ends.
 *
 * The Host writes BMI requests to mailbox0, and reads BMI responses
 * from mailbox0.   BMI requests all begin with a command
 * (see below for specific commands), and are followed by
 * command-specific data.
 *
 * Flow control:
 * The Host can only issue a command once the Target gives it a
 * "BMI Command Credit", using AR6K Counter #4.  As soon as the
 * Target has completed a command, it issues another BMI Command
 * Credit (so the Host can issue the next command).
 *
 * BMI handles all required Target-side cache flushing.
 */

/* Maximum data size used for BMI transfers */
#define BMI_DATASZ_MAX 256

/* BMI Commands */

#define BMI_NO_COMMAND 0

#define BMI_DONE 1
/*
 * Semantics: Host is done using BMI
 * Request format:
 *    uint32_t      command (BMI_DONE)
 * Response format: none
 */

#define BMI_READ_MEMORY 2
/*
 * Semantics: Host reads AR6K memory
 * Request format:
 *    uint32_t      command (BMI_READ_MEMORY)
 *    uint32_t      address
 *    uint32_t      length, at most BMI_DATASZ_MAX
 * Response format:
 *    uint8_t       data[length]
 */

#define BMI_WRITE_MEMORY 3
/*
 * Semantics: Host writes AR6K memory
 * Request format:
 *    uint32_t       command (BMI_WRITE_MEMORY)
 *    uint32_t      address
 *    uint32_t      length, at most BMI_DATASZ_MAX
 *    uint8_t       data[length]
 * Response format: none
 */

#define BMI_EXECUTE 4
/*
 * Semantics: Causes AR6K to execute code
 * Request format:
 *    uint32_t      command (BMI_EXECUTE)
 *    uint32_t      address
 *    uint32_t      parameter
 * Response format:
 *    uint32_t      return value
 */

#define BMI_SET_APP_START 5
/*
 * Semantics: Set Target application starting address
 * Request format:
 *    uint32_t      command (BMI_SET_APP_START)
 *    uint32_t      address
 * Response format: none
 */

#define BMI_READ_SOC_REGISTER 6
/*
 * Semantics: Read a 32-bit Target SOC register.
 * Request format:
 *    uint32_t      command (BMI_READ_REGISTER)
 *    uint32_t      address
 * Response format:
 *    uint32_t      value
 */

#define BMI_WRITE_SOC_REGISTER 7
/*
 * Semantics: Write a 32-bit Target SOC register.
 * Request format:
 *    uint32_t      command (BMI_WRITE_REGISTER)
 *    uint32_t      address
 *    uint32_t      value
 *
 * Response format: none
 */

#define BMI_GET_TARGET_ID 8
#define BMI_GET_TARGET_INFO 8
/*
 * Semantics: Fetch the 4-byte Target information
 * Request format:
 *    uint32_t      command (BMI_GET_TARGET_ID/INFO)
 * Response format1 (old firmware):
 *    uint32_t      TargetVersionID
 * Response format2 (newer firmware):
 *    uint32_t      TARGET_VERSION_SENTINAL
 *    struct bmi_target_info;
 */

struct bmi_target_info
{
    uint32_t target_info_byte_count; /* size of this structure */
    uint32_t target_ver;             /* Target Version ID */
    uint32_t target_type;            /* Target type */
};

#define TARGET_VERSION_SENTINAL 0xffffffff
#define TARGET_TYPE_AR4100 3
#define TARGET_TYPE_AR400X 6

#if ATH_FIRMWARE_TARGET == TARGET_AR4100_REV2
#define TARGET_TYPE TARGET_TYPE_AR4100
#elif ATH_FIRMWARE_TARGET == TARGET_AR400X_REV1
#define TARGET_TYPE TARGET_TYPE_AR400X
#endif

#define BMI_ROMPATCH_INSTALL 9
/*
 * Semantics: Install a ROM Patch.
 * Request format:
 *    uint32_t      command (BMI_ROMPATCH_INSTALL)
 *    uint32_t      Target ROM Address
 *    uint32_t      Target RAM Address or Value (depending on Target Type)
 *    uint32_t      Size, in bytes
 *    uint32_t      Activate? 1-->activate;
 *                            0-->install but do not activate
 * Response format:
 *    uint32_t      PatchID
 */

#define BMI_ROMPATCH_UNINSTALL 10
/*
 * Semantics: Uninstall a previously-installed ROM Patch,
 * automatically deactivating, if necessary.
 * Request format:
 *    uint32_t      command (BMI_ROMPATCH_UNINSTALL)
 *    uint32_t      PatchID
 *
 * Response format: none
 */

#define BMI_ROMPATCH_ACTIVATE 11
/*
 * Semantics: Activate a list of previously-installed ROM Patches.
 * Request format:
 *    uint32_t      command (BMI_ROMPATCH_ACTIVATE)
 *    uint32_t      rompatch_count
 *    uint32_t      PatchID[rompatch_count]
 *
 * Response format: none
 */

#define BMI_ROMPATCH_DEACTIVATE 12
/*
 * Semantics: Deactivate a list of active ROM Patches.
 * Request format:
 *    uint32_t      command (BMI_ROMPATCH_DEACTIVATE)
 *    uint32_t      rompatch_count
 *    uint32_t      PatchID[rompatch_count]
 *
 * Response format: none
 */

#define BMI_LZ_STREAM_START 13
/*
 * Semantics: Begin an LZ-compressed stream of input
 * which is to be uncompressed by the Target to an
 * output buffer at address.  The output buffer must
 * be sufficiently large to hold the uncompressed
 * output from the compressed input stream.  This BMI
 * command should be followed by a series of 1 or more
 * BMI_LZ_DATA commands.
 *    uint32_t      command (BMI_LZ_STREAM_START)
 *    uint32_t      address
 * Note: Not supported on all versions of ROM firmware.
 */

#define BMI_LZ_DATA 14
/*
 * Semantics: Host writes AR6K memory with LZ-compressed
 * data which is uncompressed by the Target.  This command
 * must be preceded by a BMI_LZ_STREAM_START command. A series
 * of BMI_LZ_DATA commands are considered part of a single
 * input stream until another BMI_LZ_STREAM_START is issued.
 * Request format:
 *    uint32_t      command (BMI_LZ_DATA)
 *    uint32_t      length (of compressed data),
 *                  at most BMI_DATASZ_MAX
 *    uint8_t       CompressedData[length]
 * Response format: none
 * Note: Not supported on all versions of ROM firmware.
 */

#define BMI_NVRAM_PROCESS 15
#define BMI_NVRAM_SEG_NAME_SZ 16
/*
 * Semantics: Cause Target to search NVRAM (if any) for a
 * segment with the specified name and process it according
 * to NVRAM metadata.
 * Request format:
 *    uint32_t      command (BMI_NVRAM_PROCESS)
 *    uint8_t       name[BMI_NVRAM_SEG_NAME_SZ] name (LE format)
 * Response format:
 *    uint32_t      0, if nothing was executed;
 *                  otherwise the value returned from the
 *                  last NVRAM segment that was executed
 */

#define BMI_SEGMENTED_WRITE_ADDR 0x1234
/*
 * Semantics: This is not a command but just the write memory address
 * that should be used by hosted systems while downloading segmented
 * firmware binary file to the target while target is in BMI mode.
 * Target in BMI mode interprets stream and loads the various segments
 * being downloaded onto appropriate target RAM locations.
 * Request format:
 *    uint32_t       command (BMI_WRITE_MEMORY)
 *    uint32_t      address
 *    uint32_t      length, at most BMI_DATASZ_MAX
 *    uint8_t       data[length]
 * Response format: none
 */
#endif /* __BMI_MSG_H__ */
