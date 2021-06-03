/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Byte order utilities
 *
 * This file by default provides proper behavior on little-endian architectures.
 * On big-endian architectures, IL_BIGENDIAN should be defined.
 */

#ifndef INCLUDED_WHD_ENDIAN_H
#define INCLUDED_WHD_ENDIAN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Reverse the bytes in a 16-bit value */
#define SWAP16(val) \
    ( (uint16_t)( ( ( (uint16_t)(val) & (uint16_t)0x00ffU ) << 8 ) | \
                  ( ( (uint16_t)(val) & (uint16_t)0xff00U ) >> 8 ) ) )

/* Reverse the bytes in a 32-bit value */
#define SWAP32(val) \
    ( (uint32_t)( ( ( (uint32_t)(val) & (uint32_t)0x000000ffU ) << 24 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0x0000ff00U ) <<  8 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0x00ff0000U ) >>  8 ) | \
                  ( ( (uint32_t)(val) & (uint32_t)0xff000000U ) >> 24 ) ) )

#ifdef IL_BIGENDIAN
#define htod32(i) SWAP32(i)
#define htod16(i) SWAP16(i)
#define dtoh32(i) SWAP32(i)
#define dtoh16(i) SWAP16(i)
#define hton16(i) (i)
#define hton32(i) (i)
#define ntoh16(i) (i)
#define ntoh32(i) (i)
#else /* IL_BIGENDIAN */
#define htod32(i) (i)
#define htod16(i) (i)
#define dtoh32(i) (i)
#define dtoh16(i) (i)
#define hton16(i) SWAP16(i)
#define hton32(i) SWAP32(i)
#define ntoh16(i) SWAP16(i)
#define ntoh32(i) SWAP32(i)
#endif /* IL_BIGENDIAN */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INCLUDED_WHD_ENDIAN_H */

