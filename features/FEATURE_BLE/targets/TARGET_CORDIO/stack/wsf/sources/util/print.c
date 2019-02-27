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
 *  \brief Utility functions.
 */
/*************************************************************************************************/

#include <string.h>

#include "util/print.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief    Detect hexadecimal digits. */
#define PRINT_IS_XDIGIT(c)     (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || \
                                ((c >= 'A') && (c <= 'F')))

/*! \brief    Convert hexadecimal digit to integer. */
#define PRINT_XDIGIT_TO_INT(c)  (((c >= '0') && (c <= '9')) ? (uint8_t)(c - '0') : \
                                ((c >= 'a') && (c <= 'f')) ? (uint8_t)(c - 'a' + 10u) : \
                                ((c >= 'A') && (c <= 'F')) ? (uint8_t)(c - 'A' + 10u) : 0u)

/*************************************************************************************************/
/*!
 *  \brief  Format an integer.
 *
 *  \param  pStr    Storage for formatted integer.
 *  \param  maxLen  Maximum number of characters to store.
 *  \param  i       Integer to format.
 *  \param  base    Integer base.
 *  \param  sign    TRUE if sign should be printed.
 *  \param  width   Width of field.
 *
 *  \return Number of characters stored.
 */
/*************************************************************************************************/
static int printFmtInt(char *pStr, int maxLen, int i, uint8_t base, bool_t sign, int width)
{
  char *s, *p = pStr;
  unsigned int u = (unsigned int) i;
  uint8_t use_width;
  int t;

  use_width = width;

  if (i == 0)
  {
    s = "0\0";
    width--;
    goto almost;
  }

  if (sign && base == 10 && i < 0)
  {
    *pStr++ = '-';
    u = (unsigned int)-i;
    width--;
  }
  s = pStr + maxLen - 1;
  *s = '\0';

  while (u && (!use_width || (width > 0)))
  {
    t = (unsigned int)u % base;
    if (t >= 10)
    {
      t += 'A' - '0' - 10;
    }
    *--s = (char)(t + '0');
    u /= base;
    width--;
  }

almost:
  while (width > 0)
  {
    *pStr++ = '0';
    width--;
  }
  strcpy(pStr, s);

  return strlen(p);
}

/*************************************************************************************************/
/*!
 *  \brief  Parse an integer from a string.
 *
 *  \param  pStr      String to parse.
 *  \param  pInt      Storage for parsed integer.
 *  \param  base      Integer base.
 *
 *  \return Number of characters consumed.
 */
/*************************************************************************************************/
static int32_t printParseInt(const char *pStr, uint32_t *pInt, uint32_t base)
{
  int32_t r = 0;

  if (base == 0)
  {
    if (*pStr == '0')
    {
      if (((*(pStr + 1) == 'x') || (*(pStr + 1) == 'X')) && PRINT_IS_XDIGIT(*(pStr + 2)))
      {
        r    += 2;
        pStr += 2;
        base  = 16;
      }
      else
      {
        base = 8;
      }
    }
    else
    {
      base = 10;
    }
  }
  else if (base == 16)
  {
    if (*pStr == '0')
    {
      if (((*(pStr + 1) == 'x') || (*(pStr + 1) == 'X')) && PRINT_IS_XDIGIT(*(pStr + 2)))
      {
        r    += 2;
        pStr += 2;
      }
    }
  }

  /* One character is required; all characters must be consumed. */
  *pInt = 0;
  do
  {
    char    c;
    uint8_t t;

    c = *pStr++;
    if (!PRINT_IS_XDIGIT(c))
    {
      break;
    }

    t = PRINT_XDIGIT_TO_INT(c);
    if (t >= base)
    {
      break;
    }

    *pInt *= base;
    *pInt += t;
    r++;
  } while (*pStr != '\0');

  return r;
}

/*************************************************************************************************/
/*!
 *  \brief  Print a trace message.
 *
 *  \param  pStr      Storage for formatted string.
 *  \param  size      Maximum number of characters to store.
 *  \param  pFmt      Format string.
 *  \param  ap        Arguments.
 *
 *  \return Number of characters stored.
 */
/*************************************************************************************************/
uint32_t PrintVsn(char *pStr, uint32_t size, const char *pFmt, va_list ap)
{
    size_t len = 0;

    *pStr = 0;
    size--; /* Ensure we null-terminate within our buffer */

    while ((*pFmt != '\0') && (len < size))
    {
      uint32_t width = 0;

      /* normal */
      if (*pFmt != '%')
      {
        *pStr++ = *pFmt++;
        len++;
        continue;
      }

      pFmt++;
      if (*pFmt == '%')
      {
        *pStr++ = '%';
        len++;
        pFmt++;
        continue;
      }

      /* width */
      if (*pFmt == '0')
      {
        pFmt += printParseInt(pFmt, &width, 10u);
      }

      /* long (ignore) */
      if (*pFmt == 'l')
      {
        pFmt++;
      }

      switch(*pFmt)
      {
        /* character */
        case 'c':
        {
          char tmp = va_arg(ap, int);
          *pStr++  = tmp;
          len++;
          break;
        }
        /* unsigned decimal integer */
        case 'u':
        {
          unsigned int tmp    = va_arg(ap, unsigned int);
          uint8_t      lenTmp = printFmtInt(pStr, size - len, tmp, 10, 0, width);
          pStr += lenTmp;
          len  += lenTmp;
          break;
        }
        /* signed decimal integer */
        case 'd':
        {
          int     tmp    = va_arg(ap, int);
          uint8_t lenTmp = printFmtInt(pStr, size - len, tmp, 10, 1, width);
          pStr += lenTmp;
          len  += lenTmp;
          break;
        }
        /* pointer */
        case 'p':
        {
          unsigned long tmp    = va_arg(ap, unsigned long);
          uint8_t       lenTmp = printFmtInt(pStr, size - len, tmp, 16, 1, 8u);
          pStr += lenTmp;
          len  += lenTmp;
          break;
        }
        /* unsigned hexadecimal integer */
        case 'x':
        case 'X':
        {
          unsigned int tmp    = va_arg(ap, unsigned int);
          uint8_t      lenTmp = printFmtInt(pStr, size - len, tmp, 16, 0, width);
          pStr += lenTmp;
          len  += lenTmp;
          break;
        }
        /* string */
        case 's':
        {
          char *tmp = va_arg(ap, char *);
          while ((tmp != NULL) && (*tmp != '\0') && (len < size))
          {
            *pStr++ = *tmp++;
            len++;
          }
          break;
        }
        default:
          break;
      }
      pFmt++;
    }

    /* Null-terminate output. */
    *pStr = 0;

    if (len > size)
    {
      /* Compensate for -1 earlier. */
      return size + 2;
    }

    return len;
}
