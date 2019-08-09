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
#ifndef _HIF_INTERNAL_H_
#define _HIF_INTERNAL_H_

#define HIF_ACTIVE_MBOX_INDEX (0)

#define HIF_MBOX_BLOCK_SIZE 32
#define HIF_MBOX_BASE_ADDR 0x800
#define HIF_MBOX_WIDTH 0x800
#define HIF_MBOX0_BLOCK_SIZE HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX1_BLOCK_SIZE HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX2_BLOCK_SIZE HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX3_BLOCK_SIZE HIF_MBOX_BLOCK_SIZE

#define SPI_CLOCK_FREQUENCY_DEFAULT 24000000
#define SPI_CLOCK_FREQUENCY_REDUCED 12000000

#define HIF_MBOX_START_ADDR(mbox) HIF_MBOX_BASE_ADDR + mbox *HIF_MBOX_WIDTH

#define HIF_MBOX_END_ADDR(mbox) HIF_MBOX_START_ADDR(mbox) + HIF_MBOX_WIDTH - 1

#endif /* _HIF_INTERNAL_H_ */
