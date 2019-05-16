/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Terminal handler.
 */
/*************************************************************************************************/

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdarg.h>

#include "wsf_types.h"
#include "wsf_os.h"

/*! \addtogroup WSF_UTIL_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef TERMINAL_MAX_ARGC
#define TERMINAL_MAX_ARGC           8u          /*!< \brief Maximum number of arguments to any command. */
#endif
#ifndef TERMINAL_MAX_COMMAND_LEN
#define TERMINAL_MAX_COMMAND_LEN    100u        /*!< \brief Maximum length of command line. */
#endif
#define TERMINAL_PRINTF_MAX_LEN     256u        /*!< \brief Maximum length of any printed output. */
#define TERMINAL_STRING_PROMPT      "> "        /*!< \brief Prompt string. */
#define TERMINAL_STRING_ERROR       "ERROR: "   /*!< \brief Error prefix. */
#define TERMINAL_STRING_USAGE       "USAGE: "   /*!< \brief Usage prefix. */
#define TERMINAL_STRING_NEW_LINE    "\r\n"      /*!< \brief New line string. */

/*! \brief    Terminal command error codes. */
enum
{
  TERMINAL_ERROR_OK                 = 0,  /*!< \brief Command completed. */
  TERMINAL_ERROR_BAD_ARGUMENTS      = 1,  /*!< \brief ERROR: Invalid argument(s) */
  TERMINAL_ERROR_TOO_FEW_ARGUMENTS  = 2,  /*!< \brief ERROR: Too few arguments */
  TERMINAL_ERROR_TOO_MANY_ARGUMENTS = 3,  /*!< \brief ERROR: Too many arguments */
  TERMINAL_ERROR_EXEC               = 4   /*!< \brief Command completed with execution error. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Handler for a terminal command.
 *
 *  \param  argc      The number of arguments passed to the command.
 *  \param  argv      The array of arguments; the 0th argument is the command.
 *
 *  \return Error code.
 */
/*************************************************************************************************/
typedef uint8_t (*terminalHandler_t)(uint32_t argc, char **argv);

/*************************************************************************************************/
/*!
 *  \brief  Handler for transmit.
 *
 *  \param  pBuf      Buffer to transmit.
 *  \param  len       Number of bytes to transmit.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef bool_t (*terminalUartTx_t)(const uint8_t *pBuf, uint32_t len);

/*! \brief  Terminal command. */
typedef struct terminalCommand_tag
{
  struct terminalCommand_tag   *pNext;     /*!< \brief Pointer to next command in list. */
  const char                   *pName;     /*!< \brief Name of command. */
  const char                   *pHelpStr;  /*!< \brief Help String for command. */
  terminalHandler_t            handler;    /*!< \brief Handler for command. */
} terminalCommand_t;

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize terminal.
 *
 *  \param  handlerId   Handler ID for TerminalHandler().
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalInit(wsfHandlerId_t handlerId);

/*************************************************************************************************/
/*!
 *  \brief  Register the UART Tx Function for the platform.
 *
 *  \param  uartTxFunc    UART Tx callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalRegisterUartTxFunc(terminalUartTx_t uartTxFunc);

/*************************************************************************************************/
/*!
 *  \brief  Register command with terminal.
 *
 *  \param  pCommand    Command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalRegisterCommand(terminalCommand_t *pCommand);

/*************************************************************************************************/
/*!
 *  \brief  Handler for terminal messages.
 *
 *  \param  event       WSF event mask.
 *  \param  pMsg        WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Called by application when a data byte is received.
 *
 *  \param  dataByte    received byte
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalRx(uint8_t dataByte);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to transmit string.
 *
 *  \param  pStr      String.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTxStr(const char *pStr);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to transmit character.
 *
 *  \param  c         Character.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTxChar(char c);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to print formatted data.
 *
 *  \param  pStr      Message format string
 *  \param  ...       Additional arguments, printf-style
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTxPrint(const char *pStr, ...);

/*************************************************************************************************/
/*!
 *  \brief  Application function to transmit data..
 *
 *  \param  pData     Data.
 *  \param  len       Length of data, in bytes.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTx(const uint8_t *pData, uint16_t len);

/*! \} */    /* WSF_UTIL_API */

#endif /* TERMINAL_H */
