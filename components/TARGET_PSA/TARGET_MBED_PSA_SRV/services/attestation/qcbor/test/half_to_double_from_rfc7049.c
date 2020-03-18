/*
 SPDX-License-Identifier: BSD-2-Clause

 Copyright (c) 2013 IETF Trust and the persons identified as the
 document authors.  All rights reserved.

 This document is subject to BCP 78 and the IETF Trust's Legal
 Provisions Relating to IETF Documents
 (http://trustee.ietf.org/license-info) in effect on the date of
 publication of this document.  Please review these documents
 carefully, as they describe your rights and restrictions with respect
 to this document.  Code Components extracted from this document must
 include Simplified BSD License text as described in Section 4.e of
 the Trust Legal Provisions and are provided without warranty as
 described in the Simplified BSD License.

 */

/*
 This code is from RFC 7049. It is not used in the main implementation
 because:
   a) it adds a dependency on <math.h> and ldexp().
   b) the license may be an issue

 QCBOR does support half-precision, but rather than using
 floating point math like this, it does it with bit shifting
 and masking.

 This code is here to test that code.

 */

#include "half_to_double_from_rfc7049.h"

#include <math.h>

double decode_half(unsigned char *halfp) {
    int half = (halfp[0] << 8) + halfp[1];
    int exp = (half >> 10) & 0x1f;
    int mant = half & 0x3ff;
    double val;
    if (exp == 0) val = ldexp(mant, -24);
    else if (exp != 31) val = ldexp(mant + 1024, exp - 25);
    else val = mant == 0 ? INFINITY : NAN;
    return half & 0x8000 ? -val : val;
}
