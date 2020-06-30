//*****************************************************************************
//
//  am_reg_jedec.h
//! @file
//!
//! @brief Register macros for the ARM JEDEC module
//
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
#ifndef AM_REG_JEDEC_H
#define AM_REG_JEDEC_H

//*****************************************************************************
//
// JEDEC
// Instance finder. (1 instance(s) available)
//
//*****************************************************************************
#define AM_REG_JEDEC_NUM_MODULES                     1
#define AM_REG_JEDECn(n) \
    (REG_JEDEC_BASEADDR + 0x00000000 * n)

/* ========================================  Start of section using anonymous unions  ======================================== */
#if defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language = extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
  #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
  #pragma clang diagnostic ignored "-Wnested-anon-types"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/**
  \brief  Structure type to access the Apollo CM4 JEDEC registers.
 */
typedef struct
{
  uint32_t RESERVED0[52U];              /* 0xF00 - 0xFCF */

  union
  {
    __IM  uint32_t PID4;                /*!< 0xF0000FD0 (R/ )  PID4 Register */

    struct
    {
      __IM uint32_t JEPCONT     : 4;    /* [3..0] Contains the JEP Continuation bits. */
    } PID4_b;
  };

  union
  {
    __IM  uint32_t PID5;                /*!< 0xF0000FD4 (R/ )  PID5 Register */

    struct
    {
      __IM uint32_t VALUE       : 32;   /* [31..0] Contains the value of 0x00000000. */
    } PID5_b;
  };

  union
  {
    __IM  uint32_t PID6;                /*!< 0xF0000FD8 (R/ )  PID6 Register */

    struct
    {
      __IM uint32_t VALUE       : 32;   /* [31..0] Contains the value of 0x00000000. */
    } PID6_b;
  };

  union
  {
    __IM  uint32_t PID7;                /*!< 0xF0000FDC (R/ )  PID7 Register */

    struct
    {
      __IM uint32_t VALUE       : 32;   /* [31..0] Contains the value of 0x00000000. */
    } PID7_b;
  };

  union
  {
    __IM  uint32_t PID0;                /*!< 0xF0000FE0 (R/ )  PID0 Register */

    struct
    {
      __IM uint32_t PNL8        : 8;    /* [7..0] Contains the low 8 bits of the Ambiq Micro device part number. */
    } PID0_b;
  };

  union
  {
    __IM  uint32_t PID1;                /*!< 0xF0000FE4 (R/ )  PID1 Register */

    struct
    {
      __IM uint32_t PNH4        : 4;    /* [3..0] Contains the high 4 bits of the Ambiq Micro device part number. */
      __IM uint32_t JEPIDL      : 4;    /* [7..4] Contains the low 4 bits of the Ambiq Micro JEDEC JEP-106 ID. The full JEPID is therefore 0x9B. */
    } PID1_b;
  };

  union
  {
    __IM  uint32_t PID2;                /*!< 0xF0000FE8 (R/ )  PID2 Register */

    struct
    {
      __IM uint32_t JEPIDH      : 4;    /* [3..0] Contains the high 3 bits of the Ambiq Micro JEPID. Note that bit3 of this field is hard-coded to 1. The full JEPID is therefore 0x9B. */
      __IM uint32_t CHIPREVH4   : 4;    /* [7..4] Contains the high 4 bits of the Ambiq Micro CHIPREV (see also MCUCTRL.CHIPREV). Note that this field will change with each revision of the chip. */
    } PID2_b;
  };

  union
  {
    __IM  uint32_t PID3;                /*!< 0xF0000FEC (R/ )  PID3 Register */

    struct
    {
      __IM uint32_t ZERO        : 4;    /* [3..0] This field is hard-coded to 0x0. */
      __IM uint32_t CHIPREVL4   : 4;    /* [7..0] Contains the low 4 bits of the Ambiq Micro CHIPREV (see also MCUCTRL.CHIPREV). Note that this field will change with each revision of the chip. */
    } PID3_b;
  };

  union
  {
    __IM  uint32_t CID0;                /*!< 0xF0000FE0 (R/ )  CID0 Register */

    struct
    {
      __IM uint32_t CID         : 8;    /* [7..0] Coresight ROM Table, CID0. */
    } CID0_b;
  };

  union
  {
    __IM  uint32_t CID1;                /*!< 0xF0000FE4 (R/ )  CID1 Register */

    struct
    {
      __IM uint32_t CID         : 8;    /* [7..0] Coresight ROM Table, CID1. */
    } CID1_b;
  };

  union
  {
    __IM  uint32_t CID2;                /*!< 0xF0000FE8 (R/ )  CID2 Register */

    struct
    {
      __IM uint32_t CID         : 8;    /* [7..0] Coresight ROM Table, CID2. */
    } CID2_b;
  };

  union
  {
    __IM  uint32_t CID3;                /*!< 0xF0000FEC (R/ )  CID3 Register */

    struct
    {
      __IM uint32_t CID         : 8;    /* [7..0] Coresight ROM Table, CID3. */
    } CID3_b;
  };
} JEDEC_Type;


//*****************************************************************************
//
// JEDEC_PID4 - JEP Continuation Register
//
//*****************************************************************************
// Contains the JEP Continuation bits.
#define JEDEC_PID4_JEPCONT_Pos                0U
#define JEDEC_PID4_JEPCONT_Msk                (0x0000000FUL)

//*****************************************************************************
//
// JEDEC_PID5 - JEP reserved Register
//
//*****************************************************************************
// Contains the value of 0x00000000.
#define JEDEC_PID5_VALUE_Pos                0U
#define JEDEC_PID5_VALUE_Msk                (0xFFFFFFFFUL)

//*****************************************************************************
//
// JEDEC_PID6 - JEP reserved Register
//
//*****************************************************************************
// Contains the value of 0x00000000.
#define JEDEC_PID6_VALUE_Pos                0U
#define JEDEC_PID6_VALUE_Msk                (0xFFFFFFFFUL)

//*****************************************************************************
//
// JEDEC_PID7 - JEP reserved Register
//
//*****************************************************************************
// Contains the value of 0x00000000.
#define JEDEC_PID7_VALUE_Pos                0U
#define JEDEC_PID7_VALUE_Msk                (0xFFFFFFFFUL)

//*****************************************************************************
//
// JEDEC_PID0 - Ambiq Partnum low byte
//
//*****************************************************************************
// Contains the low 8 bits of the Ambiq Micro device part number.
#define JEDEC_PID0_PNL8_Pos                 0U
#define JEDEC_PID0_PNL8_Msk                 (0x000000FFUL)

//*****************************************************************************
//
// JEDEC_PID1 - Ambiq part number high-nibble, JEPID low-nibble.
//
//*****************************************************************************
// Contains the low 4 bits of the Ambiq Micro JEDEC JEP-106 ID. The full JEPID
// is therefore 0x9B.
#define JEDEC_PID1_JEPIDL_Pos               4U
#define JEDEC_PID1_JEPIDL_Msk               (0x000000F0UL)

// Contains the high 4 bits of the Ambiq Micro device part number.
#define JEDEC_PID1_PNH4_Pos                 0U
#define JEDEC_PID1_PNH4_Msk                 (0x0000000FUL)

//*****************************************************************************
//
// JEDEC_PID2 - Ambiq chip revision low-nibble, JEPID high-nibble
//
//*****************************************************************************
// Contains the high 4 bits of the Ambiq Micro CHIPREV (see also
// MCUCTRL.CHIPREV). Note that this field will change with each revision of the
// chip.
#define JEDEC_PID2_CHIPREVH4_Pos            4U
#define JEDEC_PID2_CHIPREVH4_Msk            (0x000000F0UL)

// Contains the high 3 bits of the Ambiq Micro JEPID. Note that bit3 of this
// field is hard-coded to 1. The full JEPID is therefore 0x9B.
#define JEDEC_PID2_JEPIDH_Pos               0U
#define JEDEC_PID2_JEPIDH_Msk               (0x0000000FUL)

//*****************************************************************************
//
// JEDEC_PID3 - Ambiq chip revision high-nibble.
//
//*****************************************************************************
// Contains the low 4 bits of the Ambiq Micro CHIPREV (see also
// MCUCTRL.CHIPREV). Note that this field will change with each revision of the
// chip.
#define JEDEC_PID3_CHIPREVL4_Pos            4U
#define JEDEC_PID3_CHIPREVL4_Msk            (0x000000F0UL)

// This field is hard-coded to 0x0.
#define JEDEC_PID3_ZERO_Pos                 0U
#define JEDEC_PID3_ZERO_Msk                 (0x0000000FUL)

//*****************************************************************************
//
// JEDEC_CID0 - Coresight ROM Table.
//
//*****************************************************************************
// Coresight ROM Table, CID0.
#define JEDEC_CID0_CID_Pos                  0U
#define JEDEC_CID0_CID_Msk                  (0x000000FFUL)

//*****************************************************************************
//
// JEDEC_CID1 - Coresight ROM Table.
//
//*****************************************************************************
// Coresight ROM Table, CID1.
#define JEDEC_CID1_CID_Pos                  0U
#define JEDEC_CID1_CID_Msk                  (0x000000FFUL)

//*****************************************************************************
//
// JEDEC_CID2 - Coresight ROM Table.
//
//*****************************************************************************
// Coresight ROM Table, CID2.
#define JEDEC_CID2_CID_Pos                  0U
#define JEDEC_CID2_CID_Msk                  (0x000000FFUL)

//*****************************************************************************
//
// JEDEC_CID3 - Coresight ROM Table.
//
//*****************************************************************************
// Coresight ROM Table, CID3.
#define JEDEC_CID3_CID_Pos                  0U
#define JEDEC_CID3_CID_Msk                  (0x000000FFUL)




#define JEDEC_BASE  (0xF0000F00UL)                            /*!< JEDEC Base Address */

#define JEDEC   ((JEDEC_Type       *)     JEDEC_BASE      )   /*!< JEDEC configuration struct */


/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#endif



#endif // AM_REG_JEDEC_H
