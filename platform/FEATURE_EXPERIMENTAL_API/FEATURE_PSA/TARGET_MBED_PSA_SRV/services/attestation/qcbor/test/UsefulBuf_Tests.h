/*==============================================================================
 Copyright (c) 2016-2018, The Linux Foundation.
 Copyright (c) 2018, Laurence Lundblade.
 All rights reserved.
 SPDX-License-Identifier: BSD-3-Clause

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors, nor the name "Laurence Lundblade" may be used to
      endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================*/

#ifndef UsefulBuf_UsefulBuf_Tests_h
#define UsefulBuf_UsefulBuf_Tests_h

const char * UOBTest_NonAdversarial(void);

const char *  TestBasicSanity(void);

const char *  UOBTest_BoundaryConditionsTest(void);

const char *  UBMacroConversionsTest(void);

const char *  UBUtilTests(void);

const char *  UIBTest_IntegerFormat(void);

const char *  UBUTest_CopyUtil(void);

#endif
