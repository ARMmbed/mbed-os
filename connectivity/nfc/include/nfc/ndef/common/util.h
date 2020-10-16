/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef FEATURES_NFC_SOURCE_NFC_COMMON_UTIL_H_
#define FEATURES_NFC_SOURCE_NFC_COMMON_UTIL_H_

#include "platform/Span.h"

namespace mbed {
namespace nfc {
namespace ndef {
namespace common {

/**
 * @addtogroup nfc
 * @{
 */

/**
 * Convert a C-string into a Span<const uint8_t>.
 *
 * @note The NULL charactere is not present in the resulting object.
 *
 * @param cstr The cstr to convert into a Span.
 *
 * @return A Span that views cstr but doesn't include the NULL terminator
 * character.
 */
template<size_t N>
Span < const uint8_t, N - 1 > span_from_cstr(const char (&cstr)[N])
{
    return Span < const uint8_t, N - 1 > ((const uint8_t *)cstr, N - 1);
}

/**
 * Convert a C-string into a Span<const uint8_t>.
 *
 * @note The NULL charactere is not present in the resulting object.
 *
 * @param cstr The cstr to convert into a Span.
 *
 * @return A Span that views cstr but doesn't include the NULL terminator
 * character.
 */
Span<const uint8_t> span_from_cstr(const char *cstr);

/**
 * @}
 */

} // namespace common
} // namespace ndef
} // namespace nfc
} // namespace mbed

#endif /* FEATURES_NFC_SOURCE_NFC_COMMON_UTIL_H_ */
