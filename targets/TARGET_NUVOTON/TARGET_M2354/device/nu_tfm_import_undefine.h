/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
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

#ifndef __NU_TFM_IMPORT_UNDEFINE_H__
#define __NU_TFM_IMPORT_UNDEFINE_H__

/* Avoid polluting name space, esp. BL2 */
#undef BL2
#undef MCUBOOT_IMAGE_NUMBER
#undef NU_UPDATE_STAGE_SDH
#undef NU_UPDATE_STAGE_FLASH

#endif  /* __NU_TFM_IMPORT_UNDEFINE_H__ */
