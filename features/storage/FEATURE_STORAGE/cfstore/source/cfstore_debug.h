/** @file cfstore_debug.h
 *
 * component debug header file.
 */


#ifndef __CFSTORE_DEBUG
#define __CFSTORE_DEBUG

#include <stdint.h>
#include <assert.h>
#include <stdio.h>


/* Debug Support */

#define CFSTORE_LOG_NONE        0
#define CFSTORE_LOG_ERR         1
#define CFSTORE_LOG_WARN        2
#define CFSTORE_LOG_NOTICE      3
#define CFSTORE_LOG_INFO        4
#define CFSTORE_LOG_DEBUG       5
#define CFSTORE_LOG_FENTRY      6

#define CFSTORE_LOG(_fmt, ...)                          \
  do                                                    \
  {                                                     \
        printf(_fmt, __VA_ARGS__);                      \
  }while(0);

#define noCFSTORE_DEBUG
#ifdef CFSTORE_DEBUG

extern uint32_t cfstore_optDebug_g;
extern uint32_t cfstore_optLogLevel_g;
extern uint32_t cfstore_optLogTracepoint_g;


/* uncomment for asserts to work */
/* #undef NDEBUG */
// todo: port to mbedOSV3++ #include <core-util/assert.h>

#define CFSTORE_INLINE
// todo: port to mbedOSV3++ #define CFSTORE_ASSERT  CORE_UTIL_ASSERT
#define CFSTORE_ASSERT(...)

#define CFSTORE_DBGLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(cfstore_optDebug_g && (cfstore_optLogLevel_g >= CFSTORE_LOG_DEBUG))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


#define CFSTORE_ERRLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(cfstore_optDebug_g && (cfstore_optLogLevel_g >= CFSTORE_LOG_ERR))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


#define CFSTORE_FENTRYLOG(_fmt, ...)                       \
  do                                                    \
  {                                                     \
    if(cfstore_optDebug_g && (cfstore_optLogLevel_g >= CFSTORE_LOG_FENTRY))  \
    {                                                   \
        printf(_fmt, __VA_ARGS__);                      \
    }                                                   \
  }while(0);


/* tracepoints */
#define CFSTORE_TP_NONE         0x0
#define CFSTORE_TP_CLOSE        (1<<0)
#define CFSTORE_TP_CREATE       (1<<1)
#define CFSTORE_TP_DELETE       (1<<2)
#define CFSTORE_TP_FILE         (1<<3)
#define CFSTORE_TP_FIND         (1<<4)
#define CFSTORE_TP_FLUSH        (1<<5)
#define CFSTORE_TP_FSM          (1<<6)
#define CFSTORE_TP_INIT         (1<<7)
#define CFSTORE_TP_MEM          (1<<8)
#define CFSTORE_TP_OPEN         (1<<9)
#define CFSTORE_TP_READ         (1<<10)
#define CFSTORE_TP_WRITE        (1<<11)
#define CFSTORE_TP_VERBOSE1     (1<<12)
#define CFSTORE_TP_VERBOSE2     (1<<13)
#define CFSTORE_TP_VERBOSE3     (1<<14)
#define CFSTORE_TP_CALLBACK     (1<<15)
#define CFSTORE_TP_FENTRY       (1<<31)

#define CFSTORE_TP(_tp, _fmt, ...)                                              \
do                                                                              \
{                                                                               \
    if(cfstore_optDebug_g && (cfstore_optLogLevel_g >= CFSTORE_LOG_DEBUG))      \
    {                                                                           \
        if((cfstore_optLogTracepoint_g & (_tp)) == (uint32_t)(_tp))             \
        {                                                                       \
            printf(_fmt, __VA_ARGS__);                                          \
        }                                                                       \
    }                                                                           \
}while(0);


#else
#define CFSTORE_ASSERT(_x)                   do { } while(0)
#define CFSTORE_INLINE                       inline
#define CFSTORE_DBGLOG(_fmt, ...)            do { } while(0)
#define CFSTORE_ERRLOG(_fmt, ...)            do { } while(0)
#define CFSTORE_FENTRYLOG(_fmt, ...)         do { } while(0)
#define CFSTORE_TP(_tp, _fmt, ...)           do { } while(0)
#endif /* CFSTORE_DEBUG */


#endif /*__CFSTORE_DEBUG*/
