/**
 *******************************************************************************
 * @file    tmpm066_intifsd.c
 * @brief   This file provides API functions for INTIFSD driver 
 * @version V2.0.2.1    
 * @date    2015/10/19
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm066_intifsd.h"

#if defined(__TMPM066_INTIFSD_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @defgroup INTIFSD
  * @brief INTIFSD driver modules
  * @{
  */

/** @defgroup INTIFSD_Private_Defines
  * @{
  */
#define INTIFSD_NMIFLG_MASK                       ((uint32_t)0xFFF8FFFF)

#define INTIFSD_INTFLAG4_MASK                     ((uint32_t)0xE0000000)
#define INTIFSD_INTFLAG5_MASK                     ((uint32_t)0xFFFFFC00)

/** @} */
/* End of group INTIFSD_Private_Defines */

/** @defgroup INTIFSD_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group INTIFSD_Private_FunctionPrototypes */

/** @defgroup INTIFSD_Private_Functions
  * @{
  */

/** @} */
/* End of group INTIFSD_Private_Functions */

/** @defgroup INTIFSD_Exported_Functions
  * @{
  */

/**
  * @brief  Get the NMI flag that shows who triggered NMI.
  * @param  None
  * @retval NMI flag
  */
INTIFSD_NMIFactor INTIFSD_GetNMIFlag(void)
{
    INTIFSD_NMIFactor intifsd_nmi_factor = { 0U };

    intifsd_nmi_factor.All = TSB_INTIFSD->FLAG0 & (~INTIFSD_NMIFLG_MASK);

    return intifsd_nmi_factor;
}

/**
  * @brief  Get the INTFLAG3.
  * @param  None
  * @retval INTFLAG3 flag
  */
INTIFSD_IntFlag3Factor INTIFSD_GetIntFlag3(void)
{
    INTIFSD_IntFlag3Factor intifsd_intflag3_factor = { 0U };

    intifsd_intflag3_factor.All = TSB_INTIFSD->FLAG3;

    return intifsd_intflag3_factor;
}

/**
  * @brief  Get the INTFLAG4.
  * @param  None
  * @retval INTFLAG4 flag
  */
INTIFSD_IntFlag4Factor INTIFSD_GetIntFlag4(void)
{
    INTIFSD_IntFlag4Factor intifsd_intflag4_factor = { 0U };

    intifsd_intflag4_factor.All = TSB_INTIFSD->FLAG4 & (~INTIFSD_INTFLAG4_MASK);

    return intifsd_intflag4_factor;
}

/**
  * @brief  Get the INTFLAG5.
  * @param  None
  * @retval INTFLAG5 flag
  */
INTIFSD_IntFlag5Factor INTIFSD_GetIntFlag5(void)
{
    INTIFSD_IntFlag5Factor intifsd_intflag5_factor = { 0U };

    intifsd_intflag5_factor.All = TSB_INTIFSD->FLAG5 & (~INTIFSD_INTFLAG5_MASK);

    return intifsd_intflag5_factor;
}

/**
  * @brief  Clears the input INT request.
  * @param  INTSource: Select the release INT source
  *   This parameter can be one of the following values:
  *   INTIFSD_INT_SRC_LVD_PSFD, INTIFSD_INT_SRC_LVD_PRD, INTIFSD_INT_SRC_WDT, INTIFSD_INT_SRC_DMAC_0,
  *   INTIFSD_INT_SRC_DMAC_1, INTIFSD_INT_SRC_DMAC_2, INTIFSD_INT_SRC_DMAC_3, INTIFSD_INT_SRC_DMAC_4,
  *   INTIFSD_INT_SRC_DMAC_5, INTIFSD_INT_SRC_DMAC_6, INTIFSD_INT_SRC_DMAC_7, INTIFSD_INT_SRC_DMAC_8,
  *   INTIFSD_INT_SRC_DMAC_9, INTIFSD_INT_SRC_DMAC_10, INTIFSD_INT_SRC_DMAC_11, INTIFSD_INT_SRC_DMAC_12,
  *   INTIFSD_INT_SRC_DMAC_13, INTIFSD_INT_SRC_DMAC_14, INTIFSD_INT_SRC_DMAC_15, INTIFSD_INT_SRC_DMAC_16,
  *   INTIFSD_INT_SRC_DMAC_17, INTIFSD_INT_SRC_DMAC_18, INTIFSD_INT_SRC_DMAC_19, INTIFSD_INT_SRC_DMAC_20,
  *   INTIFSD_INT_SRC_DMAC_21, INTIFSD_INT_SRC_DMAC_22, INTIFSD_INT_SRC_DMAC_23, INTIFSD_INT_SRC_DMAC_24,
  *   INTIFSD_INT_SRC_DMAC_25, INTIFSD_INT_SRC_DMAC_26, INTIFSD_INT_SRC_DMAC_27, INTIFSD_INT_SRC_DMAC_28,
  *   INTIFSD_INT_SRC_DMAC_29, INTIFSD_INT_SRC_DMAC_30, INTIFSD_INT_SRC_DMAC_31, INTIFSD_INT_SRC_DMAC_ERR,
  *   INTIFSD_INT_SRC_TMRB_0_MDOVF, INTIFSD_INT_SRC_TMRB_0_CAP0, INTIFSD_INT_SRC_TMRB_0_CAP1, INTIFSD_INT_SRC_TMRB_1_MDOVF,
  *   INTIFSD_INT_SRC_TMRB_1_CAP0, INTIFSD_INT_SRC_TMRB_1_CAP1, INTIFSD_INT_SRC_TMRB_2_MDOVF, INTIFSD_INT_SRC_TMRB_2_CAP0,
  *   INTIFSD_INT_SRC_TMRB_2_CAP1, INTIFSD_INT_SRC_TMRB_3_MDOVF, INTIFSD_INT_SRC_TMRB_3_CAP0, INTIFSD_INT_SRC_TMRB_3_CAP1,
  *   INTIFSD_INT_SRC_TMRB_4_MDOVF,INTIFSD_INT_SRC_TMRB_4_CAP0, INTIFSD_INT_SRC_TMRB_4_CAP1, INTIFSD_INT_SRC_TMRB_5_MDOVF,
  *   INTIFSD_INT_SRC_TMRB_5_CAP0, INTIFSD_INT_SRC_TMRB_5_CAP1, INTIFSD_INT_SRC_TMRB_6_MDOVF, INTIFSD_INT_SRC_TMRB_6_CAP0,
  *   INTIFSD_INT_SRC_TMRB_6_CAP1, INTIFSD_INT_SRC_TMRB_7_MDOVF, INTIFSD_INT_SRC_TMRB_7_CAP0, INTIFSD_INT_SRC_TMRB_7_CAP1,
  *   INTIFSD_INT_SRC_TMRD_00, INTIFSD_INT_SRC_TMRD_01, INTIFSD_INT_SRC_TMRD_02, INTIFSD_INT_SRC_TMRD_03,
  *   INTIFSD_INT_SRC_TMRD_04, INTIFSD_INT_SRC_TMRD_10, INTIFSD_INT_SRC_TMRD_11, INTIFSD_INT_SRC_TMRD_12,
  *   INTIFSD_INT_SRC_TMRD_13, INTIFSD_INT_SRC_TMRD_14.
  * @retval None
  */
void INTIFSD_ClearINTReq(INTIFSD_INTSrc INTSource)
{
    uint8_t num = 0U;

    /* Check the parameters */
    assert_param(IS_INTIFSD_INT_SRC(INTSource));

    num = (uint32_t) INTSource;
    switch (num) {
    case 0U:                   /* STOP1INT_016 */
        TSB_INTIFSD->STOP1INT_016 |= 0xC0;
        break;
    case 1U:                   /* STOP1INT_017 */
        TSB_INTIFSD->STOP1INT_017 |= 0xC0;
        break;
    case 2U:                   /* IDLEINT_018 */
        TSB_INTIFSD->IDLEINT_018 |= 0xC0;
        break;
    case 3U:                   /* IDLEINT_096 */
        TSB_INTIFSD->IDLEINT_096 |= 0xC0;
        break;
    case 4U:                   /* IDLEINT_097 */
        TSB_INTIFSD->IDLEINT_097 |= 0xC0;
        break;
    case 5U:                   /* IDLEINT_098 */
        TSB_INTIFSD->IDLEINT_098 |= 0xC0;
        break;
    case 6U:                   /* IDLEINT_099 */
        TSB_INTIFSD->IDLEINT_099 |= 0xC0;
        break;
    case 7U:                   /* IDLEINT_100 */
        TSB_INTIFSD->IDLEINT_100 |= 0xC0;
        break;
    case 8U:                   /* IDLEINT_101 */
        TSB_INTIFSD->IDLEINT_101 |= 0xC0;
        break;
    case 9U:                   /* IDLEINT_102 */
        TSB_INTIFSD->IDLEINT_102 |= 0xC0;
        break;
    case 10U:                  /* IDLEINT_103 */
        TSB_INTIFSD->IDLEINT_103 |= 0xC0;
        break;
    case 11U:                  /* IDLEINT_104 */
        TSB_INTIFSD->IDLEINT_104 |= 0xC0;
        break;
    case 12U:                  /* IDLEINT_105 */
        TSB_INTIFSD->IDLEINT_105 |= 0xC0;
        break;
    case 13U:                  /* IDLEINT_106 */
        TSB_INTIFSD->IDLEINT_106 |= 0xC0;
        break;
    case 14U:                  /* IDLEINT_107 */
        TSB_INTIFSD->IDLEINT_107 |= 0xC0;
        break;
    case 15U:                  /* IDLEINT_108 */
        TSB_INTIFSD->IDLEINT_108 |= 0xC0;
        break;
    case 16U:                  /* IDLEINT_109 */
        TSB_INTIFSD->IDLEINT_109 |= 0xC0;
        break;
    case 17U:                  /* IDLEINT_110 */
        TSB_INTIFSD->IDLEINT_110 |= 0xC0;
        break;
    case 18U:                  /* IDLEINT_111 */
        TSB_INTIFSD->IDLEINT_111 |= 0xC0;
        break;
    case 19U:                  /* IDLEINT_112 */
        TSB_INTIFSD->IDLEINT_112 |= 0xC0;
        break;
    case 20U:                  /* IDLEINT_113 */
        TSB_INTIFSD->IDLEINT_113 |= 0xC0;
        break;
    case 21U:                  /* IDLEINT_114 */
        TSB_INTIFSD->IDLEINT_114 |= 0xC0;
        break;
    case 22U:                  /* IDLEINT_115 */
        TSB_INTIFSD->IDLEINT_115 |= 0xC0;
        break;
    case 23U:                  /* IDLEINT_116 */
        TSB_INTIFSD->IDLEINT_116 |= 0xC0;
        break;
    case 24U:                  /* IDLEINT_117 */
        TSB_INTIFSD->IDLEINT_117 |= 0xC0;
        break;
    case 25U:                  /* IDLEINT_118 */
        TSB_INTIFSD->IDLEINT_118 |= 0xC0;
        break;
    case 26U:                  /* IDLEINT_119 */
        TSB_INTIFSD->IDLEINT_119 |= 0xC0;
        break;
    case 27U:                  /* IDLEINT_120 */
        TSB_INTIFSD->IDLEINT_120 |= 0xC0;
        break;
    case 28U:                  /* IDLEINT_121 */
        TSB_INTIFSD->IDLEINT_121 |= 0xC0;
        break;
    case 29U:                  /* IDLEINT_122 */
        TSB_INTIFSD->IDLEINT_122 |= 0xC0;
        break;
    case 30U:                  /* IDLEINT_123 */
        TSB_INTIFSD->IDLEINT_123 |= 0xC0;
        break;
    case 31U:                  /* IDLEINT_124 */
        TSB_INTIFSD->IDLEINT_124 |= 0xC0;
        break;
    case 32U:                  /* IDLEINT_125 */
        TSB_INTIFSD->IDLEINT_125 |= 0xC0;
        break;
    case 33U:                  /* IDLEINT_126 */
        TSB_INTIFSD->IDLEINT_126 |= 0xC0;
        break;
    case 34U:                  /* IDLEINT_127 */
        TSB_INTIFSD->IDLEINT_127 |= 0xC0;
        break;
    case 35U:                  /* IDLEINT_128 */
        TSB_INTIFSD->IDLEINT_128 |= 0xC0;
        break;
    case 36U:                  /* IDLEINT_129 */
        TSB_INTIFSD->IDLEINT_129 |= 0xC0;
        break;
    case 37U:                  /* IDLEINT_130 */
        TSB_INTIFSD->IDLEINT_130 |= 0xC0;
        break;
    case 38U:                  /* IDLEINT_131 */
        TSB_INTIFSD->IDLEINT_131 |= 0xC0;
        break;
    case 39U:                  /* IDLEINT_132 */
        TSB_INTIFSD->IDLEINT_132 |= 0xC0;
        break;
    case 40U:                  /* IDLEINT_133 */
        TSB_INTIFSD->IDLEINT_133 |= 0xC0;
        break;
    case 41U:                  /* IDLEINT_134 */
        TSB_INTIFSD->IDLEINT_134 |= 0xC0;
        break;
    case 42U:                  /* IDLEINT_135 */
        TSB_INTIFSD->IDLEINT_135 |= 0xC0;
        break;
    case 43U:                  /* IDLEINT_136 */
        TSB_INTIFSD->IDLEINT_136 |= 0xC0;
        break;
    case 44U:                  /* IDLEINT_137 */
        TSB_INTIFSD->IDLEINT_137 |= 0xC0;
        break;
    case 45U:                  /* IDLEINT_138 */
        TSB_INTIFSD->IDLEINT_138 |= 0xC0;
        break;
    case 46U:                  /* IDLEINT_139 */
        TSB_INTIFSD->IDLEINT_139 |= 0xC0;
        break;
    case 47U:                  /* IDLEINT_140 */
        TSB_INTIFSD->IDLEINT_140 |= 0xC0;
        break;
    case 48U:                  /* IDLEINT_141 */
        TSB_INTIFSD->IDLEINT_141 |= 0xC0;
        break;
    case 49U:                  /* IDLEINT_142 */
        TSB_INTIFSD->IDLEINT_142 |= 0xC0;
        break;
    case 50U:                  /* IDLEINT_143 */
        TSB_INTIFSD->IDLEINT_143 |= 0xC0;
        break;
    case 51U:                  /* IDLEINT_144 */
        TSB_INTIFSD->IDLEINT_144 |= 0xC0;
        break;
    case 52U:                  /* IDLEINT_145 */
        TSB_INTIFSD->IDLEINT_145 |= 0xC0;
        break;
    case 53U:                  /* IDLEINT_146 */
        TSB_INTIFSD->IDLEINT_146 |= 0xC0;
        break;
    case 54U:                  /* IDLEINT_147 */
        TSB_INTIFSD->IDLEINT_147 |= 0xC0;
        break;
    case 55U:                  /* IDLEINT_148 */
        TSB_INTIFSD->IDLEINT_148 |= 0xC0;
        break;
    case 56U:                  /* IDLEINT_149 */
        TSB_INTIFSD->IDLEINT_149 |= 0xC0;
        break;
    case 57U:                  /* IDLEINT_150 */
        TSB_INTIFSD->IDLEINT_150 |= 0xC0;
        break;
    case 58U:                  /* IDLEINT_151 */
        TSB_INTIFSD->IDLEINT_151 |= 0xC0;
        break;
    case 59U:                  /* IDLEINT_152 */
        TSB_INTIFSD->IDLEINT_152 |= 0xC0;
        break;
    case 60U:                  /* IDLEINT_160 */
        TSB_INTIFSD->IDLEINT_160 |= 0xC0;
        break;
    case 61U:                  /* IDLEINT_161 */
        TSB_INTIFSD->IDLEINT_161 |= 0xC0;
        break;
    case 62U:                  /* IDLEINT_162 */
        TSB_INTIFSD->IDLEINT_162 |= 0xC0;
        break;
    case 63U:                  /* IDLEINT_163 */
        TSB_INTIFSD->IDLEINT_163 |= 0xC0;
        break;
    case 64U:                  /* IDLEINT_164 */
        TSB_INTIFSD->IDLEINT_164 |= 0xC0;
        break;
    case 65U:                  /* IDLEINT_165 */
        TSB_INTIFSD->IDLEINT_165 |= 0xC0;
        break;
    case 66U:                  /* IDLEINT_166 */
        TSB_INTIFSD->IDLEINT_166 |= 0xC0;
        break;
    case 67U:                  /* IDLEINT_167 */
        TSB_INTIFSD->IDLEINT_167 |= 0xC0;
        break;
    case 68U:                  /* IDLEINT_168 */
        TSB_INTIFSD->IDLEINT_168 |= 0xC0;
        break;
    case 69U:                  /* IDLEINT_169 */
        TSB_INTIFSD->IDLEINT_169 |= 0xC0;
        break;
    default:
        /* Do nothing */
        break;
    }
}

/** @} */
/* End of group INTIFSD_Exported_Functions */

/** @} */
/* End of group INTIFSD */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* defined(__TMPM066_INTIFSD_H) */
