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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "util/terminal.h"
#include "util/print.h"

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define TERMINAL_IS_SPACE(c)  ((c == '\n') || (c == '\t') || (c == '\r') || (c == ' ') || (c == '\v') || (c == '\f'))
#define TERMINAL_IS_PRINT(c)  ((c >= 0x20) && (c != 0x7F))

/*! \brief  Terminal events. */
enum
{
  TERMINAL_EVENT_COMMAND_RX = (1 << 0)
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief   Control block for terminal. */
typedef struct
{
  wsfHandlerId_t        handlerId;                              /*!< Handler ID for TerminalHandler(). */
  terminalCommand_t     *pFirstCommand;                         /*!< Pointer to first command. */
  char                  buf[TERMINAL_MAX_COMMAND_LEN + 1];      /*!< Command buffer. */
  bool_t                isExecuting;                            /*!< TRUE if command in buffer is executing. */
  bool_t                doEcho;                                 /*!< TRUE if input should be echoed. */
  uint32_t              bufOffset;                              /*!< Offset within buffer. */
  terminalUartTx_t      terminalTx;                             /*!< Function to transmit via UART. */
} terminalCtrlBlk_t;

/**************************************************************************************************
  Local Function Prototypes
**************************************************************************************************/

/*! \brief   Help Command Handler */
static uint8_t terminalCommandHelpHandler(uint32_t argc, char **argv);

/*! \brief   Echo Command Handler */
static uint8_t terminalCommandEchoHandler(uint32_t argc, char **argv);

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! \brief   Control block for terminal. */
static terminalCtrlBlk_t terminalCb;

/*! \brief    Help command. */
static terminalCommand_t terminalCommandHelp = { NULL, "help", "help", terminalCommandHelpHandler };

/*! \brief    Echo command. */
static terminalCommand_t terminalCommandEcho = { NULL, "echo", "echo <on|off>", terminalCommandEchoHandler };

/*************************************************************************************************/
/*!
 *  \brief  Initialize terminal.
 *
 *  \param  handlerId   Handler ID for TerminalHandler().
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("terminal: init");

  terminalCb.handlerId     = handlerId;
  terminalCb.pFirstCommand = NULL;
  terminalCb.isExecuting   = FALSE;
  terminalCb.doEcho        = TRUE;
  terminalCb.bufOffset     = 0;

  TerminalRegisterCommand(&terminalCommandHelp);
  TerminalRegisterCommand(&terminalCommandEcho);
}

/*************************************************************************************************/
/*!
 *  \brief  Register the UART Tx Function for the platform.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalRegisterUartTxFunc(terminalUartTx_t uartTxFunc)
{
  terminalCb.terminalTx = uartTxFunc;
}

/*************************************************************************************************/
/*!
 *  \brief  Register command with terminal.
 *
 *  \param  pCommand    Command.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalRegisterCommand(terminalCommand_t *pCommand)
{
  terminalCommand_t *pCommandTemp = terminalCb.pFirstCommand;

  if (pCommandTemp == NULL)
  {
    terminalCb.pFirstCommand = pCommand;
  }
  else
  {
    while (pCommandTemp->pNext != NULL)
    {
      pCommandTemp = pCommandTemp->pNext;
    }

    pCommandTemp->pNext = pCommand;
  }

  pCommand->pNext = NULL;
}

/*************************************************************************************************/
/*!
 *  \brief    Execute a received command.
 *
 *  \param    pBuf      Command string.
 *
 *  \return   None.
 */
/*************************************************************************************************/
static void terminalExecute(char *pBuf)
{
  uint32_t argc = 0;
  char    *argv[TERMINAL_MAX_ARGC + 1];
  uint32_t length;
  char    *pBufCur;
  int      state;

  enum
  {
    STATE_OUTSIDE_OF_ARG,
    STATE_JUST_GOT_QUOTE,
    STATE_INSIDE_OF_ARG,
    STATE_INSIDE_OF_ARG_IN_QUOTES
  };

  /* Parse arguments in command */
  state  = STATE_OUTSIDE_OF_ARG;
  length = strlen(pBuf);
  for (pBufCur = pBuf; pBufCur < pBuf + length; pBufCur++)
  {
    switch (state)
    {
      case STATE_OUTSIDE_OF_ARG:
      {
        if (*pBufCur == '\"')
        {
          state = STATE_JUST_GOT_QUOTE;
        }
        else if (!TERMINAL_IS_SPACE(*pBufCur))
        {
          state = STATE_INSIDE_OF_ARG;
          if (argc < TERMINAL_MAX_ARGC)
          {
            argv[argc] = pBufCur;
          }
          argc++;
        }
        break;
      }
      case STATE_JUST_GOT_QUOTE:
      {
        if (argc < TERMINAL_MAX_ARGC)
        {
          argv[state] = pBufCur;
        }
        argc++;
        if (*pBufCur == '\"')
        {
          state = STATE_OUTSIDE_OF_ARG;
          *pBufCur = '\0';
        }
        else
        {
          state = STATE_INSIDE_OF_ARG_IN_QUOTES;
        }
        break;
      }
      case STATE_INSIDE_OF_ARG:
      {
        if (TERMINAL_IS_SPACE(*pBufCur))
        {
          state = STATE_OUTSIDE_OF_ARG;
          *pBufCur = '\0';
        }
        else if (*pBufCur == '\"')
        {
          state = STATE_JUST_GOT_QUOTE;
          *pBufCur = '\0';
        }
        break;
      }
      case STATE_INSIDE_OF_ARG_IN_QUOTES:
      {
        if (*pBufCur == '\"')
        {
          state = STATE_OUTSIDE_OF_ARG;
          *pBufCur = '\0';
        }
        break;
      }
    }
  }

  /* Find & invoke command. */
  if (argc > TERMINAL_MAX_ARGC)
  {
    TerminalTxStr(TERMINAL_STRING_ERROR "too many arguments" TERMINAL_STRING_NEW_LINE);
  }
  else if (argc > 0)
  {
    terminalCommand_t *pCommand = terminalCb.pFirstCommand;

    while (pCommand != NULL)
    {
      if (strcmp(pCommand->pName, argv[0]) == 0)
      {
        break;
      }
      pCommand = pCommand->pNext;
    }

    if (pCommand == NULL)
    {
      TerminalTxStr(TERMINAL_STRING_ERROR "unrecognized command \"");
      TerminalTxStr(argv[0]);
      TerminalTxStr("\"" TERMINAL_STRING_NEW_LINE);
    }
    else
    {
      uint8_t r = pCommand->handler(argc, argv);
      switch (r)
      {
        case TERMINAL_ERROR_EXEC:
        case TERMINAL_ERROR_OK:
          break;
        case TERMINAL_ERROR_BAD_ARGUMENTS:
          TerminalTxStr(TERMINAL_STRING_ERROR "Invalid argument(s)" TERMINAL_STRING_NEW_LINE);
          break;
        case TERMINAL_ERROR_TOO_FEW_ARGUMENTS:
          TerminalTxStr(TERMINAL_STRING_ERROR "Too few arguments" TERMINAL_STRING_NEW_LINE);
          break;
        case TERMINAL_ERROR_TOO_MANY_ARGUMENTS:
          TerminalTxStr(TERMINAL_STRING_ERROR "Too many arguments" TERMINAL_STRING_NEW_LINE);
          break;
        default:
          TerminalTxStr(TERMINAL_STRING_ERROR "Unknown error" TERMINAL_STRING_NEW_LINE);
          break;
      }
    }
  }
}

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
void TerminalHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  /* Unused parameters */
  (void)pMsg;

  if ((event & TERMINAL_EVENT_COMMAND_RX) != 0)
  {
    terminalExecute(terminalCb.buf);
    TerminalTxStr(TERMINAL_STRING_PROMPT);
    terminalCb.bufOffset   = 0;
    terminalCb.isExecuting = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Called by application when a data byte is received.
 *
 *  \param  dataByte  Received byte.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalRx(uint8_t dataByte)
{
  /* Hands off buf if command is executing. */
  if (!terminalCb.isExecuting)
  {
    /* If this is the end of a line, signal task. */
    if ((dataByte == '\n') || (dataByte == '\r'))
    {
      TerminalTxStr(TERMINAL_STRING_NEW_LINE);
      terminalCb.buf[terminalCb.bufOffset] = '\0';
      WsfSetEvent(terminalCb.handlerId, TERMINAL_EVENT_COMMAND_RX);
      terminalCb.isExecuting = TRUE;
    }

    /* Check for delete. */
    else if ((dataByte == 0x7F) || (dataByte == '\b'))
    {
      if (terminalCb.bufOffset > 0)
      {
        terminalCb.bufOffset--;
        if (terminalCb.doEcho)
        {
          TerminalTxStr("\b \b");
        }
      }
    }

    /* If we still have room in the buf, put it in buf.  Othewise ignore it. */
    else if (terminalCb.bufOffset < TERMINAL_MAX_COMMAND_LEN)
    {
      /* Ignore non-printable characters. */
      if (TERMINAL_IS_PRINT(dataByte))
      {
        terminalCb.buf[terminalCb.bufOffset] = dataByte;
        terminalCb.bufOffset++;
        if (terminalCb.doEcho)
        {
          TerminalTxChar(dataByte);
        }
      }
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Called by application to transmit string.
 *
 *  \param  pStr      String.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTxStr(const char *pStr)
{
  TerminalTx((const uint8_t *)pStr, (uint16_t)strlen(pStr));
}

/*************************************************************************************************/
/*!
 *  \brief  Called by application to transmit character.
 *
 *  \param  c         Character.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTxChar(char c)
{
  TerminalTx((const uint8_t *)&c, 1);
}

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
void TerminalTxPrint(const char *pStr, ...)
{
  uint32_t len;
  char     buf[TERMINAL_PRINTF_MAX_LEN];
  va_list  args;

  va_start(args, pStr);
  len = PrintVsn(buf, TERMINAL_PRINTF_MAX_LEN, pStr, args);
  va_end(args);

  TerminalTx((uint8_t *)buf, (uint16_t)len);
}

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
static uint8_t terminalCommandHelpHandler(uint32_t argc, char **argv)
{
  terminalCommand_t *pCommand = terminalCb.pFirstCommand;

  /* Unused parameters */
  (void)argv;

  if (argc > 1)
  {
    return TERMINAL_ERROR_TOO_MANY_ARGUMENTS;
  }

  while (pCommand != NULL)
  {
    TerminalTxStr(pCommand->pHelpStr);
    TerminalTxStr(TERMINAL_STRING_NEW_LINE);

    pCommand = pCommand->pNext;
  }

  TerminalTxStr(TERMINAL_STRING_NEW_LINE);
  return TERMINAL_ERROR_OK;
}


/*************************************************************************************************/
/*!
 *  \brief  Transmit buffer on platform UART.
 *
 *  \param  pBuf    Buffer to transmit.
 *  \param  len     Length of buffer in octets.
 *
 *  \return None.
 */
/*************************************************************************************************/
void TerminalTx(const uint8_t *pData, uint16_t len)
{
  if (terminalCb.terminalTx)
  {
    (*terminalCb.terminalTx)(pData, len);
  }
}

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
static uint8_t terminalCommandEchoHandler(uint32_t argc, char **argv)
{
  if (argc < 2)
  {
    return TERMINAL_ERROR_TOO_FEW_ARGUMENTS;
  }
  else if (argc == 2)
  {
    if (strcmp(argv[1], "on") == 0)
    {
      terminalCb.doEcho = TRUE;
      TerminalTxStr("echo on" TERMINAL_STRING_NEW_LINE);
    }
    else if (strcmp(argv[1], "off") == 0)
    {
      terminalCb.doEcho = FALSE;
      TerminalTxStr("echo off" TERMINAL_STRING_NEW_LINE);
    }
    else
    {
      return TERMINAL_ERROR_BAD_ARGUMENTS;
    }
  }
  else
  {
    return TERMINAL_ERROR_TOO_MANY_ARGUMENTS;
  }

  return TERMINAL_ERROR_OK;
}

