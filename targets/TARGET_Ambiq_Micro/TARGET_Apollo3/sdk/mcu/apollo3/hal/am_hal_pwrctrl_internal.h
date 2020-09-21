//*****************************************************************************
//
//  am_hal_pwrctrl_internal.h
//! @file
//!
//! @brief Internal definitions for Power Control
//!
//! @addtogroup pwrctrl3 Power Control
//! @ingroup apollo3hal
//! @{

//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#ifndef AM_HAL_PWRCTRL_INTERNAL_H
#define AM_HAL_PWRCTRL_INTERNAL_H


//*****************************************************************************
//
// Peripheral enable bits for am_hal_pwrctrl_periph_enable/disable()
//
//*****************************************************************************
#define AM_HAL_PWRCTRL_IOS      (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOS,   PWRCTRL_DEVPWREN_PWRIOS_EN))
#define AM_HAL_PWRCTRL_IOM0     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM0,  PWRCTRL_DEVPWREN_PWRIOM0_EN))
#define AM_HAL_PWRCTRL_IOM1     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM1,  PWRCTRL_DEVPWREN_PWRIOM1_EN))
#define AM_HAL_PWRCTRL_IOM2     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM2,  PWRCTRL_DEVPWREN_PWRIOM2_EN))
#define AM_HAL_PWRCTRL_IOM3     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM3,  PWRCTRL_DEVPWREN_PWRIOM3_EN))
#define AM_HAL_PWRCTRL_IOM4     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM4,  PWRCTRL_DEVPWREN_PWRIOM4_EN))
#define AM_HAL_PWRCTRL_IOM5     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM5,  PWRCTRL_DEVPWREN_PWRIOM5_EN))
#define AM_HAL_PWRCTRL_UART0    (_VAL2FLD(PWRCTRL_DEVPWREN_PWRUART0, PWRCTRL_DEVPWREN_PWRUART0_EN))
#define AM_HAL_PWRCTRL_UART1    (_VAL2FLD(PWRCTRL_DEVPWREN_PWRUART1, PWRCTRL_DEVPWREN_PWRUART1_EN))
#define AM_HAL_PWRCTRL_ADC      (_VAL2FLD(PWRCTRL_DEVPWREN_PWRADC,   PWRCTRL_DEVPWREN_PWRADC_EN))
#define AM_HAL_PWRCTRL_SCARD    (_VAL2FLD(PWRCTRL_DEVPWREN_PWRSCARD, PWRCTRL_DEVPWREN_PWRSCARD_EN))
#define AM_HAL_PWRCTRL_MSPI     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRMSPI,  PWRCTRL_DEVPWREN_PWRMSPI_EN))
#define AM_HAL_PWRCTRL_PDM      (_VAL2FLD(PWRCTRL_DEVPWREN_PWRPDM,   PWRCTRL_DEVPWREN_PWRPDM_EN))
#define AM_HAL_PWRCTRL_BLEL     (_VAL2FLD(PWRCTRL_DEVPWREN_PWRBLEL,  PWRCTRL_DEVPWREN_PWRBLEL_EN))

#define AM_HAL_PWRCTRL_DEVPWREN_MASK    0x00003FFF
#define AM_HAL_PWRCTRL_DEVPWRSTATUS_MASK 0x000003FC

//*****************************************************************************
//
// Memory enable values for all defined memory configurations.
//
//*****************************************************************************
#define AM_HAL_PWRCTRL_MEMEN_SRAM_8K_DTCM        (_VAL2FLD(PWRCTRL_MEMPWREN_DTCM, PWRCTRL_MEMPWREN_DTCM_GROUP0DTCM0))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_32K_DTCM       (_VAL2FLD(PWRCTRL_MEMPWREN_DTCM, PWRCTRL_MEMPWREN_DTCM_GROUP0))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_64K_DTCM       (_VAL2FLD(PWRCTRL_MEMPWREN_DTCM, PWRCTRL_MEMPWREN_DTCM_ALL))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_96K                               \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_64K_DTCM     |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP0))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_128K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_96K          |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP1))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_160K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_128K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP2))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_192K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_160K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP3))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_224K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_192K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP4))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_256K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_224K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP5))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_288K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_256K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP6))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_320K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_288K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP7))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_352K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_320K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP8))
#define AM_HAL_PWRCTRL_MEMEN_SRAM_384K                              \
        (AM_HAL_PWRCTRL_MEMEN_SRAM_352K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWREN_SRAM, PWRCTRL_MEMPWREN_SRAM_GROUP9))

#define AM_HAL_PWRCTRL_MEMEN_SRAM_ALL   (AM_HAL_PWRCTRL_MEMEN_SRAM_384K)
#define AM_HAL_PWRCTRL_MEMEN_FLASH_512K  PWRCTRL_MEMPWREN_FLASH0_Msk
#define AM_HAL_PWRCTRL_MEMEN_FLASH_1M                               \
        (PWRCTRL_MEMPWREN_FLASH0_Msk | PWRCTRL_MEMPWREN_FLASH1_Msk)
#define AM_HAL_PWRCTRL_MEMEN_CACHE                                  \
        (PWRCTRL_MEMPWREN_CACHEB0_Msk | PWRCTRL_MEMPWREN_CACHEB2_Msk)
#define AM_HAL_PWRCTRL_MEMEN_CACHE_DIS  (~AM_HAL_PWRCTRL_MEMEN_CACHE)

//
// Power up all available memory devices (this is the default power up state)
//
#define AM_HAL_PWRCTRL_MEMEN_ALL                                            \
    (_VAL2FLD(PWRCTRL_MEMPWREN_DTCM,    PWRCTRL_MEMPWREN_DTCM_ALL)      |   \
     _VAL2FLD(PWRCTRL_MEMPWREN_SRAM,    PWRCTRL_MEMPWREN_SRAM_ALL)      |   \
     _VAL2FLD(PWRCTRL_MEMPWREN_FLASH0,  PWRCTRL_MEMPWREN_FLASH0_EN)     |   \
     _VAL2FLD(PWRCTRL_MEMPWREN_FLASH1,  PWRCTRL_MEMPWREN_FLASH1_EN)     |   \
     _VAL2FLD(PWRCTRL_MEMPWREN_CACHEB0, PWRCTRL_MEMPWREN_CACHEB0_EN)    |   \
     _VAL2FLD(PWRCTRL_MEMPWREN_CACHEB2, PWRCTRL_MEMPWREN_CACHEB2_EN))

//*****************************************************************************
//
// Memory deepsleep powerdown values for all defined memory configurations.
//
//*****************************************************************************
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_8K_DTCM        (_VAL2FLD(PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP, PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP_GROUP0DTCM0))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_32K_DTCM       (_VAL2FLD(PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP, PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP_GROUP0))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_64K_DTCM       (_VAL2FLD(PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP, PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP_ALL))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_96K                               \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_64K_DTCM     |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP0))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_128K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_96K          |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP1))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_160K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_128K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP2))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_192K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_160K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP3))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_224K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_192K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP4))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_256K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_224K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP5))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_288K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_256K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP6))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_320K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_288K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP7))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_352K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_320K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP8))
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_384K                              \
        (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_352K         |                   \
         _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP, PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_GROUP9))

#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_ALL   (AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_384K)
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_FLASH_512K  PWRCTRL_MEMPWDINSLEEP_FLASH0PWDSLP_Msk
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_FLASH_1M                               \
        (PWRCTRL_MEMPWDINSLEEP_FLASH0PWDSLP_Msk | PWRCTRL_MEMPWDINSLEEP_FLASH1PWDSLP_Msk)
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_CACHE      (PWRCTRL_MEMPWDINSLEEP_CACHEPWDSLP_Msk)
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_CACHE_DIS  (~AM_HAL_PWRCTRL_MEMPWDINSLEEP_CACHE)

//
// Power down all available memory devices
//
#define AM_HAL_PWRCTRL_MEMPWDINSLEEP_ALL                                            \
    (_VAL2FLD(PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP,    PWRCTRL_MEMPWDINSLEEP_DTCMPWDSLP_ALL)      |   \
     _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP,    PWRCTRL_MEMPWDINSLEEP_SRAMPWDSLP_ALL)      |   \
     _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_FLASH0PWDSLP,  PWRCTRL_MEMPWDINSLEEP_FLASH0PWDSLP_EN)     |   \
     _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_FLASH1PWDSLP,  PWRCTRL_MEMPWDINSLEEP_FLASH1PWDSLP_EN)     |   \
     _VAL2FLD(PWRCTRL_MEMPWDINSLEEP_CACHEPWDSLP, PWRCTRL_MEMPWDINSLEEP_CACHEPWDSLP_EN))

//*****************************************************************************
//
// Memory status values for all defined memory configurations
//
//*****************************************************************************
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_8K_DTCM             \
        (PWRCTRL_MEMPWRSTATUS_DTCM00_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_32K_DTCM            \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_8K_DTCM    |       \
         PWRCTRL_MEMPWRSTATUS_DTCM01_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_64K_DTCM            \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_32K_DTCM   |       \
         PWRCTRL_MEMPWRSTATUS_DTCM1_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_96K                 \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_64K_DTCM   |       \
         PWRCTRL_MEMPWRSTATUS_SRAM0_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_128K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_96K        |       \
         PWRCTRL_MEMPWRSTATUS_SRAM1_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_160K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_128K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM2_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_192K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_160K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM3_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_224K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_192K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM4_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_256K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_224K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM5_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_288K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_256K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM6_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_320K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_288K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM7_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_352K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_320K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM8_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_384K                \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_352K       |       \
         PWRCTRL_MEMPWRSTATUS_SRAM9_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_ALL                 \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_384K)
#define AM_HAL_PWRCTRL_PWRONSTATUS_FLASH_512K               \
        (PWRCTRL_MEMPWRSTATUS_FLASH0_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_FLASH_1M                 \
        (AM_HAL_PWRCTRL_PWRONSTATUS_FLASH_512K      |       \
         PWRCTRL_MEMPWRSTATUS_FLASH1_Msk)
#define AM_HAL_PWRCTRL_PWRONSTATUS_ALL                      \
        (AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_384K       |       \
          AM_HAL_PWRCTRL_PWRONSTATUS_FLASH_1M)

//*****************************************************************************
//
// Memory event values for all defined memory configurations
//
//*****************************************************************************
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_8K_DTCM                       \
        (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_DTCMEN,                         \
                  PWRCTRL_MEMPWREVENTEN_DTCMEN_GROUP0DTCM0EN))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_32K_DTCM                      \
        (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_DTCMEN,                         \
                  PWRCTRL_MEMPWREVENTEN_DTCMEN_GROUP0EN))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_64K_DTCM                      \
        (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_DTCMEN,                         \
                  PWRCTRL_MEMPWREVENTEN_DTCMEN_ALL))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_96K                           \
        ((_VAL2FLD(PWRCTRL_MEMPWREVENTEN_DTCMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_DTCMEN_ALL))       |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP0EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_128K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_96K              |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP1EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_160K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_128K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP2EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_192K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_160K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP3EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_224K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_192K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP4EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_256K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_224K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP5EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_288K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_256K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP6EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_320K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_288K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP7EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_352K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_320K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP8EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_384K                          \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_352K             |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_SRAMEN,                        \
                   PWRCTRL_MEMPWREVENTEN_SRAMEN_GROUP9EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_FLASH_512K                         \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_FLASH0EN,                      \
                    PWRCTRL_MEMPWREVENTEN_FLASH0EN_EN))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_FLASH_1M                           \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_FLASH_512K            |           \
         (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_FLASH1EN,                      \
                   PWRCTRL_MEMPWREVENTEN_FLASH1EN_EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_CACHE                              \
         ((_VAL2FLD(PWRCTRL_MEMPWREVENTEN_CACHEB0EN,                    \
                    PWRCTRL_MEMPWREVENTEN_CACHEB0EN_EN))    |           \
          (_VAL2FLD(PWRCTRL_MEMPWREVENTEN_CACHEB2EN,                    \
                    PWRCTRL_MEMPWREVENTEN_CACHEB2EN_EN)))
#define AM_HAL_PWRCTRL_MEMPWREVENTEN_ALL                                \
        (AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_384K             |           \
         AM_HAL_PWRCTRL_MEMPWREVENTEN_FLASH_1M              |           \
         AM_HAL_PWRCTRL_MEMPWREVENTEN_CACHE)

//*****************************************************************************
//
// Memory region mask values for all defined memory configurations
//
//*****************************************************************************
#define AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK             AM_HAL_PWRCTRL_MEMEN_SRAM_ALL
#define AM_HAL_PWRCTRL_MEM_REGION_FLASH_MASK            AM_HAL_PWRCTRL_MEMEN_FLASH_1M
#define AM_HAL_PWRCTRL_MEM_REGION_CACHE_MASK            AM_HAL_PWRCTRL_MEMEN_CACHE
#define AM_HAL_PWRCTRL_MEM_REGION_ALT_CACHE_MASK        AM_HAL_PWRCTRL_PWRONSTATUS_CACHE
#define AM_HAL_PWRCTRL_MEM_REGION_ALL_MASK              AM_HAL_PWRCTRL_MEMEN_ALL
#define AM_HAL_PWRCTRL_MEM_REGION_ALT_ALL_MASK          AM_HAL_PWRCTRL_PWRONSTATUS_ALL


#endif // AM_HAL_PWRCTRL_INTERNAL_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
