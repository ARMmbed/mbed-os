/*==============================================================================
 run_tests.h -- test aggregator and results reporting

 Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 Created 9/30/18
 ==============================================================================*/

/**
 @file run_tests.h
*/

/**
 @brief Type for function to output a text string

 @param[in] szString   The string to output
 @param[in] pOutCtx    A context pointer; NULL if not needed
 @param[in] bNewline   If non-zero, output a newline after the string

 This is a prototype of a function to be passed to RunTests() to
 output text strings.

 This can be implemented with stdio (if available) using a straight
 call to fputs() where the FILE * is passed as the pOutCtx as shown in
 the example code below.  This code is for Linux where the newline is
 a \\n. Windows usually prefers \\r\\n.

 @code
    static void fputs_wrapper(const char *szString, void *pOutCtx, int bNewLine)
    {
        fputs(szString, (FILE *)pOutCtx);
        if(bNewLine) {
            fputs("\n", pOutCtx);
        }
     }
 @endcode
*/
typedef void (*OutputStringCB)(const char *szString, void *pOutCtx, int bNewline);


/**
 @brief Runs the QCBOR tests.

 @param[in]  szTestNames    An argv-style list of test names to run. If
                            empty, all are run.
 @param[in]  pfOutput       Function that is called to output text strings.
 @param[in]  pOutCtx        Context pointer passed to output function.
 @param[out] pNumTestsRun   Returns the number of tests run. May be NULL.

 @return The number of tests that failed. Zero means overall success.
 */
int RunTests(const char *szTestNames[], OutputStringCB pfOutput, void *pOutCtx, int *pNumTestsRun);


/**
 @brief Print sizes of encoder / decoder contexts.

 @param[in] pfOutput     Function that is called to output text strings.
 @param[in] pOutCtx      Context pointer passed to output function.
 */
void PrintSizes(OutputStringCB pfOutput, void *pOutCtx);
