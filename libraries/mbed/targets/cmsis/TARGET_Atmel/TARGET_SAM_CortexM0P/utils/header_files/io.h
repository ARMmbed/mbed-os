#ifndef _SAM_IO_
#define _SAM_IO_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/* SAM D20 family */
#if (SAMD20)
#  include "samd20.h"
#endif

#if (SAMD21)
#  include "samd21.h"
#endif

#if (SAMR21)
#  include "samr21.h"
#endif

#if (SAMD10)
#  include "samd10.h"
#endif

#if (SAMD11)
#  include "samd11.h"
#endif

#if (SAML21)
#  include "saml21.h"
#endif

#endif /* _SAM_IO_ */
