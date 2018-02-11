
#ifndef __RTX_OS__
#define __RTX_OS__

#include "inttypes.h"

typedef struct osRtxSemaphore_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    reserved;
  const char                    *name;  ///< Object Name
  uint16_t                     tokens;  ///< Current number of tokens
  uint16_t                 max_tokens;  ///< Maximum number of tokens
} osRtxSemaphore_t;

#endif 
