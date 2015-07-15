#ifndef _SAMR21_
#define _SAMR21_

/**
 * \defgroup SAMR21_definitions SAMR21 Device Definitions
 * \brief SAMR21 CMSIS Definitions.
 */

#if defined(__SAMR21E16A__) || defined(__ATSAMR21E16A__)
#include "samr21e16a.h"
#elif defined(__SAMR21E17A__) || defined(__ATSAMR21E17A__)
#include "samr21e17a.h"
#elif defined(__SAMR21E18A__) || defined(__ATSAMR21E18A__)
#include "samr21e18a.h"
#elif defined(__SAMR21G16A__) || defined(__ATSAMR21G16A__)
#include "samr21g16a.h"
#elif defined(__SAMR21G17A__) || defined(__ATSAMR21G17A__)
#include "samr21g17a.h"
#elif defined(__SAMR21G18A__) || defined(__ATSAMR21G18A__)
#include "samr21g18a.h"
#else
#error Library does not support the specified device.
#endif

#endif /* _SAMR21_ */
