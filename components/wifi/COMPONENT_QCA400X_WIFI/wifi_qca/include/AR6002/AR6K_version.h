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

#define __VER_MAJOR_ 3 // corresponds to release X._._._._
#define __VER_MINOR_ 3 // corresponds to release _.X._._._
#define __VER_PATCH_ 0 // corresponds to release _._.X._.
#define __VER_REV_ 0   // corresponds to release _._._.X._   <-- Increment This field manually for every build

/* The makear6ksdk script (used for release builds) modifies the following line. */
#define __BUILD_NUMBER_ 0

/* Format of the version number. */
#define VER_MAJOR_BIT_OFFSET 28
#define VER_MINOR_BIT_OFFSET 24
#define VER_PATCH_BIT_OFFSET 16
#define VER_BUILD_NUM_BIT_OFFSET 0

/*
 * The version has the following format:
 * Bits 28-31: Major version
 * Bits 24-27: Minor version
 * Bite 18-23: Patch number
 * Bits 8-17: Revision number (Increment this every build)
 * Bits 0-7:  build number which is automatically updated
 *
 * DO NOT split the following macro into multiple lines as this may confuse the build scripts.
 */
#define AR6K_SW_VERSION                                                                \
    ((__VER_MAJOR_ << VER_MAJOR_BIT_OFFSET) + (__VER_MINOR_ << VER_MINOR_BIT_OFFSET) + \
     ((__VER_PATCH_ & 0xFF) << VER_PATCH_BIT_OFFSET) + (((__BUILD_NUMBER_)&0xFFFF) << VER_BUILD_NUM_BIT_OFFSET))

/* ABI Version. Reflects the version of binary interface exposed by AR6K target firmware. Needs to be incremented by 1
 * for any change in the firmware that requires upgrade of the driver on the host side for the change to work correctly
 */
#define ABI_VERSION_MAJOR (1)
#define ABI_VERSION_MINOR (0)
#define ABI_MAJOR_OFFSET (16)
#define ABI_MAJOR_MASK (0xffff0000)
#define ABI_MINOR_OFFSET (0)
#define ABI_MINOR_MASK (0x0000ffff)
#define AR6K_ABI_VERSION ((ABI_VERSION_MAJOR << ABI_MAJOR_OFFSET) | (ABI_VERSION_MINOR << ABI_MINOR_OFFSET))
