/*
 * Copyright (c) 2020 Arm Limited
 *
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

enum image_attributes {
    RE_SECURE_IMAGE_OFFSET = (0x0),
    RE_SECURE_IMAGE_MAX_SIZE = (0x50000),
    RE_NON_SECURE_IMAGE_OFFSET = ((0x0) + (0x50000)),
    RE_NON_SECURE_IMAGE_MAX_SIZE = (0x90000),
    RE_SIGN_BIN_SIZE = ((0x90000)),
};
