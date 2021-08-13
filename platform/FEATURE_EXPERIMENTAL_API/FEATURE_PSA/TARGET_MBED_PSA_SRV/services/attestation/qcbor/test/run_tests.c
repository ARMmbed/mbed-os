/*==============================================================================
 run_tests.c -- test aggregator and results reporting

 Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 Created on 9/30/18
 ==============================================================================*/

#include "run_tests.h"
#include "UsefulBuf.h"
#include <stdbool.h>

#include "float_tests.h"
#include "qcbor_decode_tests.h"
#include "qcbor_encode_tests.h"
#include "UsefulBuf_Tests.h"



// Used to test RunTests
int fail_test()
{
    return -44;
}




/*
 Convert a number up to 999999999 to a string. This is so sprintf doesn't
 have to be linked in so as to minimized dependencies even in test code.
  */
const char *NumToString(int32_t nNum, UsefulBuf StringMem)
{
    const int32_t nMax = 1000000000;

    UsefulOutBuf OutBuf;
    UsefulOutBuf_Init(&OutBuf, StringMem);

    if(nNum < 0) {
        UsefulOutBuf_AppendByte(&OutBuf, '-');
        nNum = -nNum;
    }
    if(nNum > nMax-1) {
        return "XXX";
    }

    bool bDidSomeOutput = false;
    for(int n = nMax; n > 0; n/=10) {
        int x = nNum/n;
        if(x || bDidSomeOutput){
            bDidSomeOutput = true;
            UsefulOutBuf_AppendByte(&OutBuf, '0' + x);
            nNum -= x * n;
        }
    }
    if(!bDidSomeOutput){
        UsefulOutBuf_AppendByte(&OutBuf, '0');
    }
    UsefulOutBuf_AppendByte(&OutBuf, '\0');

    return UsefulOutBuf_GetError(&OutBuf) ? "" : StringMem.ptr;
}




typedef int (test_fun_t)(void);
typedef const char * (test_fun2_t)(void);


#define TEST_ENTRY(test_name)  {#test_name, test_name, true}
#define TEST_ENTRY_DISABLED(test_name)  {#test_name, test_name, false}

typedef struct {
    const char  *szTestName;
    test_fun_t  *test_fun;
    bool         bEnabled;
} test_entry;

typedef struct {
    const char *szTestName;
    test_fun2_t  *test_fun;
    bool         bEnabled;
} test_entry2;

test_entry2 s_tests2[] = {
    TEST_ENTRY(UBUTest_CopyUtil),
    TEST_ENTRY(UOBTest_NonAdversarial),
    TEST_ENTRY(TestBasicSanity),
    TEST_ENTRY(UOBTest_BoundaryConditionsTest),
    TEST_ENTRY(UBMacroConversionsTest),
    TEST_ENTRY(UBUtilTests),
    TEST_ENTRY(UIBTest_IntegerFormat)
};


test_entry s_tests[] = {
    TEST_ENTRY(ParseMapAsArrayTest),
    TEST_ENTRY_DISABLED(AllocAllStringsTest),
    TEST_ENTRY(IndefiniteLengthNestTest),
    TEST_ENTRY(NestedMapTestIndefLen),
    TEST_ENTRY(ParseSimpleTest),
    TEST_ENTRY(EncodeRawTest),
    TEST_ENTRY(RTICResultsTest),
    TEST_ENTRY(MapEncodeTest),
    TEST_ENTRY(ArrayNestingTest1),
    TEST_ENTRY(ArrayNestingTest2),
    TEST_ENTRY(ArrayNestingTest3),
    TEST_ENTRY(EncodeDateTest),
    TEST_ENTRY(SimpleValuesTest1),
    TEST_ENTRY(IntegerValuesTest1),
    TEST_ENTRY(AllAddMethodsTest),
    TEST_ENTRY(ParseTooDeepArrayTest),
    TEST_ENTRY(ComprehensiveInputTest),
    TEST_ENTRY(ParseMapTest),
    TEST_ENTRY_DISABLED(IndefiniteLengthArrayMapTest),
    TEST_ENTRY(BasicEncodeTest),
    TEST_ENTRY(NestedMapTest),
    TEST_ENTRY(BignumParseTest),
    TEST_ENTRY(OptTagParseTest),
    TEST_ENTRY(DateParseTest),
    TEST_ENTRY(ShortBufferParseTest2),
    TEST_ENTRY(ShortBufferParseTest),
    TEST_ENTRY(ParseDeepArrayTest),
    TEST_ENTRY(SimpleArrayTest),
    TEST_ENTRY(IntegerValuesParseTest),
    TEST_ENTRY_DISABLED(MemPoolTest),
    TEST_ENTRY_DISABLED(IndefiniteLengthStringTest),
    TEST_ENTRY(HalfPrecisionDecodeBasicTests),
    TEST_ENTRY(DoubleAsSmallestTest),
    TEST_ENTRY(HalfPrecisionAgainstRFCCodeTest),
    TEST_ENTRY(BstrWrapTest),
    TEST_ENTRY(BstrWrapErrorTest),
    TEST_ENTRY(BstrWrapNestTest),
    TEST_ENTRY(CoseSign1TBSTest),
    TEST_ENTRY(StringDecoderModeFailTest),
    TEST_ENTRY_DISABLED(BigComprehensiveInputTest),
    TEST_ENTRY(EncodeErrorTests),
    //TEST_ENTRY(fail_test),
};


int RunTests(const char *szTestNames[], OutputStringCB pfOutput, void *poutCtx, int *pNumTestsRun)
{
    int nTestsFailed = 0;
    int nTestsRun = 0;
    UsefulBuf_MAKE_STACK_UB(StringStorage, 5);

    test_entry2 *t2;
    const test_entry2 *s_tests2_end = s_tests2 + sizeof(s_tests2)/sizeof(test_entry2);

    for(t2 = s_tests2; t2 < s_tests2_end; t2++) {
        if(szTestNames[0]) {
            // Some tests have been named
            const char **szRequestedNames;
            for(szRequestedNames = szTestNames; *szRequestedNames;  szRequestedNames++) {
                if(!strcmp(t2->szTestName, *szRequestedNames)) {
                    break; // Name matched
                }
            }
            if(*szRequestedNames == NULL) {
                // Didn't match this test
                continue;
            }
        } else {
            // no tests named, but don't run "disabled" tests
            if(!t2->bEnabled) {
                // Don't run disabled tests when all tests are being run
                // as indicated by no specific test names being given
                continue;
            }
        }

        const char * szTestResult = (t2->test_fun)();
        nTestsRun++;
        if(pfOutput) {
            (*pfOutput)(t2->szTestName, poutCtx, 0);
        }

        if(szTestResult) {
            if(pfOutput) {
                (*pfOutput)(" FAILED (returned ", poutCtx, 0);
                (*pfOutput)(szTestResult, poutCtx, 0);
                (*pfOutput)(")", poutCtx, 1);
            }
            nTestsFailed++;
        } else {
            if(pfOutput) {
                (*pfOutput)( " PASSED", poutCtx, 1);
            }
        }
    }


    test_entry *t;
    const test_entry *s_tests_end = s_tests + sizeof(s_tests)/sizeof(test_entry);

    for(t = s_tests; t < s_tests_end; t++) {
        if(szTestNames[0]) {
            // Some tests have been named
            const char **szRequestedNames;
            for(szRequestedNames = szTestNames; *szRequestedNames;  szRequestedNames++) {
                if(!strcmp(t->szTestName, *szRequestedNames)) {
                    break; // Name matched
                }
            }
            if(*szRequestedNames == NULL) {
                // Didn't match this test
                continue;
            }
        } else {
            // no tests named, but don't run "disabled" tests
            if(!t->bEnabled) {
                // Don't run disabled tests when all tests are being run
                // as indicated by no specific test names being given
                continue;
            }
        }

        int nTestResult = (t->test_fun)();
        nTestsRun++;
        if(pfOutput) {
            (*pfOutput)(t->szTestName, poutCtx, 0);
        }

        if(nTestResult) {
            if(pfOutput) {
                (*pfOutput)(" FAILED (returned ", poutCtx, 0);
                (*pfOutput)(NumToString(nTestResult, StringStorage), poutCtx, 0);
                (*pfOutput)(")", poutCtx, 1);
            }
            nTestsFailed++;
        } else {
            if(pfOutput) {
                (*pfOutput)( " PASSED", poutCtx, 1);
            }
        }
    }

    if(pNumTestsRun) {
        *pNumTestsRun = nTestsRun;
    }

    if(pfOutput) {
        (*pfOutput)( "SUMMARY: ", poutCtx, 0);
        (*pfOutput)( NumToString(nTestsRun, StringStorage), poutCtx, 0);
        (*pfOutput)( " tests run; ", poutCtx, 0);
        (*pfOutput)( NumToString(nTestsFailed, StringStorage), poutCtx, 0);
        (*pfOutput)( " tests failed", poutCtx, 1);
    }

    return nTestsFailed;
}




static void PrintSize(const char *szWhat, uint32_t uSize, OutputStringCB pfOutput, void *pOutCtx)
{
   UsefulBuf_MAKE_STACK_UB(buffer, 20);

   (*pfOutput)(szWhat, pOutCtx, 0);
   (*pfOutput)(" ", pOutCtx, 0);
   (*pfOutput)(NumToString(uSize, buffer), pOutCtx, 0);
   (*pfOutput)("", pOutCtx, 1);
}

void PrintSizes(OutputStringCB pfOutput, void *pOutCtx)
{
   // Type and size of return from sizeof() varies. These will never be large so cast is safe
   PrintSize("sizeof(QCBORTrackNesting)",   (uint32_t)sizeof(QCBORTrackNesting), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBOREncodeContext)",  (uint32_t)sizeof(QCBOREncodeContext), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBORDecodeNesting)",  (uint32_t)sizeof(QCBORDecodeNesting), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBORDecodeContext)",  (uint32_t)sizeof(QCBORDecodeContext), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBORItem)",           (uint32_t)sizeof(QCBORItem), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBORStringAllocator)",(uint32_t)sizeof(QCBORStringAllocator), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBORTagListIn)",      (uint32_t)sizeof(QCBORTagListIn), pfOutput, pOutCtx);
   PrintSize("sizeof(QCBORTagListOut)",     (uint32_t)sizeof(QCBORTagListOut), pfOutput, pOutCtx);
   (*pfOutput)("", pOutCtx, 1);
}
