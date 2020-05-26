/*==============================================================================
 Copyright (c) 2016-2018, The Linux Foundation.
 Copyright (c) 2018-2019, Laurence Lundblade.
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

#ifndef __QCBOR__qcbor_encode_tests__
#define __QCBOR__qcbor_encode_tests__

#include "qcbor.h"


/*
 Notes:

 - All the functions in qcbor.h are called once in the aggregation of all the tests below.

 - All the types that are supported are given as input and parsed by these tests

 - There is some hostile input such as invalid lengths and CBOR too complex
   and types this parser doesn't handle

 */


/*
 Most basic test.
 */
int BasicEncodeTest(void);


/*
 Encode lots of integer values, particularly around the boundary and make sure they
 Match the expected binary output. Primarily an encoding test.
 */
int IntegerValuesTest1(void);



/*
 Create nested arrays to the max depth allowed and make sure it succeeds.
 This is an encoding test.
 */
int ArrayNestingTest1(void);


/*
 Create nested arrays to one more than the meax depth and make sure it fails.
 This is an encoding test.
 */
int ArrayNestingTest2(void);


/*
 Encoding test.
 Create arrays to max depth and close one extra time and look for correct error code
 */
int ArrayNestingTest3(void);


/*
 This tests the QCBOREncode_AddRaw() function by adding two chunks or RAWCBOR to an
 array and comparing with expected values. This is an encoding test.
 */
int EncodeRawTest(void);


/*
 This creates a somewhat complicated CBOR MAP and verifies it against expected
 data. This is an encoding test.
 */
int MapEncodeTest(void);



/*
 Encodes a goodly number of floats and doubles and checks encoding is right
 */
int FloatValuesTest1(void);


/*
 Encodes true, false and the like
 */
int SimpleValuesTest1(void);


/*
 Encodes most data formats that are supported */
int EncodeDateTest(void);


/*
 Encodes particular data structure that a particular app will need...
 */
int RTICResultsTest(void);


/*
 Calls all public encode methods in qcbor.h once.
 */
int AllAddMethodsTest(void);

/*
 The binary string wrapping of maps and arrays used by COSE
 */
int  BstrWrapTest(void);

int BstrWrapErrorTest(void);

int BstrWrapNestTest(void);

int CoseSign1TBSTest(void);

int EncodeErrorTests(void);



#endif /* defined(__QCBOR__qcbor_encode_tests__) */
