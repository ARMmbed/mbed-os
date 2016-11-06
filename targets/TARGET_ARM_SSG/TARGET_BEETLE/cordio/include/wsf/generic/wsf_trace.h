/*************************************************************************************************/
/*!
 *  \file   wsf_trace.h
 *
 *  \brief  Trace message interface.
 *
 *          $Date: 2016-02-27 09:05:32 -0800 (Sat, 27 Feb 2016) $
 *          $Revision: 6074 $
 *
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef WSF_TRACE_H
#define WSF_TRACE_H

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Token event handler. */
typedef void (*WsfTokenHandler_t)(void);

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

void WsfTrace(const char *pStr, ...);
void WsfToken(uint32_t tok, uint32_t var);
void WsfTraceEnable(bool_t enable);

/* Token management. */
bool_t WsfTokenService(void);
uint8_t WsfTokenIOWrite(uint8_t *pBuf, uint8_t len);

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifdef TOKEN_GENERATION

#define WSF_TOKEN(subsys, stat, msg)                    \
  __WSF_TOKEN_DEFINE__(                                 \
    /* token:   */     MODULE_ID, __LINE__,             \
    /* origin:  */     __FILE__, subsys,                \
    /* message: */     stat, msg)

#define WSF_TRACE0(subsys, stat, msg)                   WSF_TOKEN(subsys, stat, msg)
#define WSF_TRACE1(subsys, stat, msg, var1)             WSF_TOKEN(subsys, stat, msg)
#define WSF_TRACE2(subsys, stat, msg, var1, var2)       WSF_TOKEN(subsys, stat, msg)
#define WSF_TRACE3(subsys, stat, msg, var1, var2, var3) WSF_TOKEN(subsys, stat, msg)

#elif WSF_TRACE_ENABLED == TRUE

#define WSF_TRACE0(subsys, stat, msg)                   WsfTrace(msg)
#define WSF_TRACE1(subsys, stat, msg, var1)             WsfTrace(msg, var1)
#define WSF_TRACE2(subsys, stat, msg, var1, var2)       WsfTrace(msg, var1, var2)
#define WSF_TRACE3(subsys, stat, msg, var1, var2, var3) WsfTrace(msg, var1, var2, var3)

#elif WSF_TOKEN_ENABLED == TRUE

#define WSF_TRACE0(subsys, stat, msg)                   \
  WsfToken(((__LINE__ & 0xFFF) << 16) | MODULE_ID, 0)
#define WSF_TRACE1(subsys, stat, msg, var1)             \
  WsfToken(((__LINE__ & 0xFFF) << 16) | MODULE_ID, (uint32_t)(var1))
#define WSF_TRACE2(subsys, stat, msg, var1, var2)       \
  WsfToken(((__LINE__ & 0xFFF) << 16) | MODULE_ID, (uint32_t)(((var2) << 16) | ((var1) & 0xFFFF)))
#define WSF_TRACE3(subsys, stat, msg, var1, var2, var3) \
  WsfToken(((__LINE__ & 0xFFF) << 16) | MODULE_ID, (uint32_t)((((var3) & 0xFFFF) << 16) | (((var2) & 0xFF) << 8) | ((var1) & 0xFF)))

#else

#define WSF_TRACE0(subsys, stat, msg)
#define WSF_TRACE1(subsys, stat, msg, var1)
#define WSF_TRACE2(subsys, stat, msg, var1, var2)
#define WSF_TRACE3(subsys, stat, msg, var1, var2, var3)

#endif

#define WSF_TRACE_INFO0(msg)
#define WSF_TRACE_INFO1(msg, var1)
#define WSF_TRACE_INFO2(msg, var1, var2)
#define WSF_TRACE_INFO3(msg, var1, var2, var3)
#define WSF_TRACE_WARN0(msg)                        WSF_TRACE0("WSF", "WARN", msg)
#define WSF_TRACE_WARN1(msg, var1)                  WSF_TRACE1("WSF", "WARN", msg, var1)
#define WSF_TRACE_WARN2(msg, var1, var2)            WSF_TRACE2("WSF", "WARN", msg, var1, var2)
#define WSF_TRACE_WARN3(msg, var1, var2, var3)      WSF_TRACE3("WSF", "WARN", msg, var1, var2, var3)
#define WSF_TRACE_ERR0(msg)                         WSF_TRACE0("WSF", "ERR",  msg)
#define WSF_TRACE_ERR1(msg, var1)                   WSF_TRACE1("WSF", "ERR",  msg, var1)
#define WSF_TRACE_ERR2(msg, var1, var2)             WSF_TRACE2("WSF", "ERR",  msg, var1, var2)
#define WSF_TRACE_ERR3(msg, var1, var2, var3)       WSF_TRACE3("WSF", "ERR",  msg, var1, var2, var3)
#define WSF_TRACE_ALLOC0(msg)
#define WSF_TRACE_ALLOC1(msg, var1)
#define WSF_TRACE_ALLOC2(msg, var1, var2)
#define WSF_TRACE_ALLOC3(msg, var1, var2, var3)
#define WSF_TRACE_FREE0(msg)
#define WSF_TRACE_FREE1(msg, var1)
#define WSF_TRACE_FREE2(msg, var1, var2)
#define WSF_TRACE_FREE3(msg, var1, var2, var3)
#define WSF_TRACE_MSG0(msg)
#define WSF_TRACE_MSG1(msg, var1)
#define WSF_TRACE_MSG2(msg, var1, var2)
#define WSF_TRACE_MSG3(msg, var1, var2, var3)

#define HCI_TRACE_INFO0(msg)                        WSF_TRACE0("HCI", "INFO", msg)
#define HCI_TRACE_INFO1(msg, var1)                  WSF_TRACE1("HCI", "INFO", msg, var1)
#define HCI_TRACE_INFO2(msg, var1, var2)            WSF_TRACE2("HCI", "INFO", msg, var1, var2)
#define HCI_TRACE_INFO3(msg, var1, var2, var3)      WSF_TRACE3("HCI", "INFO", msg, var1, var2, var3)
#define HCI_TRACE_WARN0(msg)                        WSF_TRACE0("HCI", "WARN", msg)
#define HCI_TRACE_WARN1(msg, var1)                  WSF_TRACE1("HCI", "WARN", msg, var1)
#define HCI_TRACE_WARN2(msg, var1, var2)            WSF_TRACE2("HCI", "WARN", msg, var1, var2)
#define HCI_TRACE_WARN3(msg, var1, var2, var3)      WSF_TRACE3("HCI", "WARN", msg, var1, var2, var3)
#define HCI_TRACE_ERR0(msg)                         WSF_TRACE0("HCI", "ERR",  msg)
#define HCI_TRACE_ERR1(msg, var1)                   WSF_TRACE1("HCI", "ERR",  msg, var1)
#define HCI_TRACE_ERR2(msg, var1, var2)             WSF_TRACE2("HCI", "ERR",  msg, var1, var2)
#define HCI_TRACE_ERR3(msg, var1, var2, var3)       WSF_TRACE3("HCI", "ERR",  msg, var1, var2, var3)

#define HCI_PDUMP_CMD(len, pBuf)
#define HCI_PDUMP_EVT(len, pBuf)
#define HCI_PDUMP_TX_ACL(len, pBuf)
#define HCI_PDUMP_RX_ACL(len, pBuf)

#define DM_TRACE_INFO0(msg)                         WSF_TRACE0("DM", "INFO", msg)
#define DM_TRACE_INFO1(msg, var1)                   WSF_TRACE1("DM", "INFO", msg, var1)
#define DM_TRACE_INFO2(msg, var1, var2)             WSF_TRACE2("DM", "INFO", msg, var1, var2)
#define DM_TRACE_INFO3(msg, var1, var2, var3)       WSF_TRACE3("DM", "INFO", msg, var1, var2, var3)
#define DM_TRACE_WARN0(msg)                         WSF_TRACE0("DM", "WARN", msg)
#define DM_TRACE_WARN1(msg, var1)                   WSF_TRACE1("DM", "WARN", msg, var1)
#define DM_TRACE_WARN2(msg, var1, var2)             WSF_TRACE2("DM", "WARN", msg, var1, var2)
#define DM_TRACE_WARN3(msg, var1, var2, var3)       WSF_TRACE3("DM", "WARN", msg, var1, var2, var3)
#define DM_TRACE_ERR0(msg)                          WSF_TRACE0("DM", "ERR",  msg)
#define DM_TRACE_ERR1(msg, var1)                    WSF_TRACE1("DM", "ERR",  msg, var1)
#define DM_TRACE_ERR2(msg, var1, var2)              WSF_TRACE2("DM", "ERR",  msg, var1, var2)
#define DM_TRACE_ERR3(msg, var1, var2, var3)        WSF_TRACE3("DM", "ERR",  msg, var1, var2, var3)
#define DM_TRACE_ALLOC0(msg)                        WSF_TRACE0("DM", "ALLOC", msg)
#define DM_TRACE_ALLOC1(msg, var1)                  WSF_TRACE1("DM", "ALLOC", msg, var1)
#define DM_TRACE_ALLOC2(msg, var1, var2)            WSF_TRACE2("DM", "ALLOC", msg, var1, var2)
#define DM_TRACE_ALLOC3(msg, var1, var2, var3)      WSF_TRACE3("DM", "ALLOC", msg, var1, var2, var3)
#define DM_TRACE_FREE0(msg)                         WSF_TRACE0("DM", "FREE", msg)
#define DM_TRACE_FREE1(msg, var1)                   WSF_TRACE1("DM", "FREE", msg, var1)
#define DM_TRACE_FREE2(msg, var1, var2)             WSF_TRACE2("DM", "FREE", msg, var1, var2)
#define DM_TRACE_FREE3(msg, var1, var2, var3)       WSF_TRACE3("DM", "FREE", msg, var1, var2, var3)

#define L2C_TRACE_INFO0(msg)                        WSF_TRACE0("L2C", "INFO", msg)
#define L2C_TRACE_INFO1(msg, var1)                  WSF_TRACE1("L2C", "INFO", msg, var1)
#define L2C_TRACE_INFO2(msg, var1, var2)            WSF_TRACE2("L2C", "INFO", msg, var1, var2)
#define L2C_TRACE_INFO3(msg, var1, var2, var3)      WSF_TRACE3("L2C", "INFO", msg, var1, var2, var3)
#define L2C_TRACE_WARN0(msg)                        WSF_TRACE0("L2C", "WARN", msg)
#define L2C_TRACE_WARN1(msg, var1)                  WSF_TRACE1("L2C", "WARN", msg, var1)
#define L2C_TRACE_WARN2(msg, var1, var2)            WSF_TRACE2("L2C", "WARN", msg, var1, var2)
#define L2C_TRACE_WARN3(msg, var1, var2, var3)      WSF_TRACE3("L2C", "WARN", msg, var1, var2, var3)
#define L2C_TRACE_ERR0(msg)                         WSF_TRACE0("L2C", "ERR",  msg)
#define L2C_TRACE_ERR1(msg, var1)                   WSF_TRACE1("L2C", "ERR",  msg, var1)
#define L2C_TRACE_ERR2(msg, var1, var2)             WSF_TRACE2("L2C", "ERR",  msg, var1, var2)
#define L2C_TRACE_ERR3(msg, var1, var2, var3)       WSF_TRACE3("L2C", "ERR",  msg, var1, var2, var3)

#define ATT_TRACE_INFO0(msg)                        WSF_TRACE0("ATT", "INFO", msg)
#define ATT_TRACE_INFO1(msg, var1)                  WSF_TRACE1("ATT", "INFO", msg, var1)
#define ATT_TRACE_INFO2(msg, var1, var2)            WSF_TRACE2("ATT", "INFO", msg, var1, var2)
#define ATT_TRACE_INFO3(msg, var1, var2, var3)      WSF_TRACE3("ATT", "INFO", msg, var1, var2, var3)
#define ATT_TRACE_WARN0(msg)                        WSF_TRACE0("ATT", "WARN", msg)
#define ATT_TRACE_WARN1(msg, var1)                  WSF_TRACE1("ATT", "WARN", msg, var1)
#define ATT_TRACE_WARN2(msg, var1, var2)            WSF_TRACE2("ATT", "WARN", msg, var1, var2)
#define ATT_TRACE_WARN3(msg, var1, var2, var3)      WSF_TRACE3("ATT", "WARN", msg, var1, var2, var3)
#define ATT_TRACE_ERR0(msg)                         WSF_TRACE0("ATT", "ERR",  msg)
#define ATT_TRACE_ERR1(msg, var1)                   WSF_TRACE1("ATT", "ERR",  msg, var1)
#define ATT_TRACE_ERR2(msg, var1, var2)             WSF_TRACE2("ATT", "ERR",  msg, var1, var2)
#define ATT_TRACE_ERR3(msg, var1, var2, var3)       WSF_TRACE3("ATT", "ERR",  msg, var1, var2, var3)

#define SMP_TRACE_INFO0(msg)                        WSF_TRACE0("SMP", "INFO", msg)
#define SMP_TRACE_INFO1(msg, var1)                  WSF_TRACE1("SMP", "INFO", msg, var1)
#define SMP_TRACE_INFO2(msg, var1, var2)            WSF_TRACE2("SMP", "INFO", msg, var1, var2)
#define SMP_TRACE_INFO3(msg, var1, var2, var3)      WSF_TRACE3("SMP", "INFO", msg, var1, var2, var3)
#define SMP_TRACE_WARN0(msg)                        WSF_TRACE0("SMP", "WARN", msg)
#define SMP_TRACE_WARN1(msg, var1)                  WSF_TRACE1("SMP", "WARN", msg, var1)
#define SMP_TRACE_WARN2(msg, var1, var2)            WSF_TRACE2("SMP", "WARN", msg, var1, var2)
#define SMP_TRACE_WARN3(msg, var1, var2, var3)      WSF_TRACE3("SMP", "WARN", msg, var1, var2, var3)
#define SMP_TRACE_ERR0(msg)                         WSF_TRACE0("SMP", "ERR",  msg)
#define SMP_TRACE_ERR1(msg, var1)                   WSF_TRACE1("SMP", "ERR",  msg, var1)
#define SMP_TRACE_ERR2(msg, var1, var2)             WSF_TRACE2("SMP", "ERR",  msg, var1, var2)
#define SMP_TRACE_ERR3(msg, var1, var2, var3)       WSF_TRACE3("SMP", "ERR",  msg, var1, var2, var3)

#define APP_TRACE_INFO0(msg)                        WSF_TRACE0("APP", "INFO", msg)
#define APP_TRACE_INFO1(msg, var1)                  WSF_TRACE1("APP", "INFO", msg, var1)
#define APP_TRACE_INFO2(msg, var1, var2)            WSF_TRACE2("APP", "INFO", msg, var1, var2)
#define APP_TRACE_INFO3(msg, var1, var2, var3)      WSF_TRACE3("APP", "INFO", msg, var1, var2, var3)
#define APP_TRACE_WARN0(msg)                        WSF_TRACE0("APP", "WARN", msg)
#define APP_TRACE_WARN1(msg, var1)                  WSF_TRACE1("APP", "WARN", msg, var1)
#define APP_TRACE_WARN2(msg, var1, var2)            WSF_TRACE2("APP", "WARN", msg, var1, var2)
#define APP_TRACE_WARN3(msg, var1, var2, var3)      WSF_TRACE3("APP", "WARN", msg, var1, var2, var3)
#define APP_TRACE_ERR0(msg)                         WSF_TRACE0("APP", "ERR",  msg)
#define APP_TRACE_ERR1(msg, var1)                   WSF_TRACE1("APP", "ERR",   msg, var1)
#define APP_TRACE_ERR2(msg, var1, var2)             WSF_TRACE2("APP", "ERR",   msg, var1, var2)
#define APP_TRACE_ERR3(msg, var1, var2, var3)       WSF_TRACE3("APP", "ERR",   msg, var1, var2, var3)

#define LL_TRACE_INFO0(msg)                         WSF_TRACE0("LL", "INFO", msg)
#define LL_TRACE_INFO1(msg, var1)                   WSF_TRACE1("LL", "INFO", msg, var1)
#define LL_TRACE_INFO2(msg, var1, var2)             WSF_TRACE2("LL", "INFO", msg, var1, var2)
#define LL_TRACE_INFO3(msg, var1, var2, var3)       WSF_TRACE3("LL", "INFO", msg, var1, var2, var3)
#define LL_TRACE_WARN0(msg)                         WSF_TRACE0("LL", "WARN", msg)
#define LL_TRACE_WARN1(msg, var1)                   WSF_TRACE1("LL", "WARN", msg, var1)
#define LL_TRACE_WARN2(msg, var1, var2)             WSF_TRACE2("LL", "WARN", msg, var1, var2)
#define LL_TRACE_WARN3(msg, var1, var2, var3)       WSF_TRACE3("LL", "WARN", msg, var1, var2, var3)
#define LL_TRACE_ERR0(msg)                          WSF_TRACE0("LL", "ERR",  msg)
#define LL_TRACE_ERR1(msg, var1)                    WSF_TRACE1("LL", "ERR",  msg, var1)
#define LL_TRACE_ERR2(msg, var1, var2)              WSF_TRACE2("LL", "ERR",  msg, var1, var2)
#define LL_TRACE_ERR3(msg, var1, var2, var3)        WSF_TRACE3("LL", "ERR",  msg, var1, var2, var3)

#if (WSF_TRACE_ENABLED == TRUE) || (WSF_TOKEN_ENABLED == TRUE)
#define LL_TRACE_ENABLE(ena)                        WsfTraceEnable(ena)
#else
#define LL_TRACE_ENABLE(ena)
#endif

#endif /* WSF_TRACE_H */
