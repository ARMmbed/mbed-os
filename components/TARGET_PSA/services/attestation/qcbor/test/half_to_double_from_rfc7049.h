/*==============================================================================
 half_to_double_from_rfc7049.h -- interface to IETF float conversion code.

 Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 Created on 9/23/18
  ==============================================================================*/

#ifndef half_to_double_from_rfc7049_h
#define half_to_double_from_rfc7049_h

double decode_half(unsigned char *halfp);

#endif /* half_to_double_from_rfc7049_h */
