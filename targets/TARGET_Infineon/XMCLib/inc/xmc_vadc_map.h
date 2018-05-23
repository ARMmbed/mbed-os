/**
 * @file xmc_vadc_map.h
 * @date 2016-11-17
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-15:
 *     - Initial version
 *
 * 2015-12-01:
 *     - Added:
 *     - XMC4300 device supported
 *
 *     - Fixed: 
 *     - Wrong MACRO name corrected for XMC4200/4100 devices.
 *       XMC_VADC_G3_SAMPLE renamed to XMC_VADC_G1_SAMPLE
 *
 * 2016-11-17:
 *      - Fixed: Add missing support for XMC47000
 *      - Fixed: Renamed XMC_CCU_41_ST2 to  XMC_CCU_41_ST3
 *      - Added: New macros equivalent to th existing ones but with better naming. 
 *               Old macros are kept for backward compatibility but they deprecated.
 *      - Added: ECAT support for XMC48/43
 *
 * @endcond 
 *
 */

#ifndef XMC_ADC_MAP_H
#define XMC_ADC_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#if ( UC_FAMILY == XMC1 )

/*  Group request source Gating input connection mappings */
#define XMC_CCU_40_ST3          		XMC_VADC_REQ_GT_A /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_ST3 */
#define XMC_CCU_40_ST2          		XMC_VADC_REQ_GT_B /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_ST2 */
#define XMC_CCU_40_ST1          		XMC_VADC_REQ_GT_C /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_ST1 */
#define XMC_CCU_40_ST0          		XMC_VADC_REQ_GT_D /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_ST0 */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_CCU_80_ST3_A        		XMC_VADC_REQ_GT_E /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_ST3A */
#define XMC_CCU_81_ST3          		XMC_VADC_REQ_GT_F /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_ST3 */
#endif
#if (UC_SERIES != XMC13 && UC_SERIES != XMC11)
#define XMC_LEDTS0_FN           		XMC_VADC_REQ_GT_I /**< @deprecated use instead  XMC_VADC_REQ_GT_LEDTS0_FN */
#define XMC_LEDTS1_FN           		XMC_VADC_REQ_GT_J /**< @deprecated use instead  XMC_VADC_REQ_GT_LEDTS1_FN */
#endif
#define XMC_ERU_0_PDOUT2        		XMC_VADC_REQ_GT_K /**< @deprecated use instead  XMC_VADC_REQ_GT_ERU0_PDOUT2 */
#define XMC_ERU_0_PDOUT3        		XMC_VADC_REQ_GT_L /**< @deprecated use instead  XMC_VADC_REQ_GT_ERU0_PDOUT3 */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_CCU_80_ST0          		XMC_VADC_REQ_GT_M /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_ST0 */
#define XMC_CCU_80_ST1          		XMC_VADC_REQ_GT_N /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_ST1 */
#endif
#define XMC_ERU_0_PDOUT0        		XMC_VADC_REQ_GT_O /**< @deprecated use instead  XMC_VADC_REQ_GT_ERU0_PDOUT0 */
#define XMC_ERU_0_PDOUT1        		XMC_VADC_REQ_GT_P /**< @deprecated use instead  XMC_VADC_REQ_GT_ERU0_PDOUT1 */

/* Group request source Trigger input connection mappings */
#define XMC_CCU_40_SR2          		XMC_VADC_REQ_TR_A /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU40_SR2 */
#define XMC_CCU_40_SR3          		XMC_VADC_REQ_TR_B /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU40_SR3 */
#if (UC_SERIES != XMC11)
#define XMC_BCCU0_TRIGOUT       		XMC_VADC_REQ_TR_F /**< @deprecated use instead  XMC_VADC_REQ_TR_BCCU0_TRIGOUT0, XMC_VADC_REQ_TR_G0_BCCU0_TRIGOUT0 or XMC_VADC_REQ_TR_G1_BCCU0_TRIGOUT1 */
#endif
#define XMC_ERU_0_IOUT2         		XMC_VADC_REQ_TR_G /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU0_IOUT2 */
#define XMC_ERU_0_IOUT3         		XMC_VADC_REQ_TR_H /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU0_IOUT3 */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_CCU_80_SR2          		XMC_VADC_REQ_TR_I /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU80_SR2 */
#define XMC_CCU_80_SR3          		XMC_VADC_REQ_TR_J /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU80_SR3 */
#endif
#define XMC_ERU_0_IOUT0         		XMC_VADC_REQ_TR_M /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU0_IOUT0 */
#define XMC_ERU_0_IOUT1         		XMC_VADC_REQ_TR_N /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU0_IOUT1 */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_POSIF_0_SR1         		XMC_VADC_REQ_TR_O /**< @deprecated use instead  XMC_VADC_REQ_TR_POSIF0_SR1 */
#endif
#define XMC_REQ_GT_SEL          		XMC_VADC_REQ_TR_P /**< @deprecated use instead  XMC_VADC_REQ_TR_REQ_GT_SEL */

/*  Group request source Gating input connection mappings */
#define XMC_VADC_REQ_GT_CCU40_ST3  			XMC_VADC_REQ_GT_A /**< VADC Gating input A */
#define XMC_VADC_REQ_GT_CCU40_ST2   		XMC_VADC_REQ_GT_B /**< VADC Gating input B */
#define XMC_VADC_REQ_GT_CCU40_ST1   		XMC_VADC_REQ_GT_C /**< VADC Gating input C */
#define XMC_VADC_REQ_GT_CCU40_ST0   		XMC_VADC_REQ_GT_D /**< VADC Gating input D */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_VADC_REQ_GT_CCU80_ST3A 			XMC_VADC_REQ_GT_E /**< VADC Gating input E */
#define XMC_VADC_REQ_GT_CCU80_ST3   		XMC_VADC_REQ_GT_F /**< VADC Gating input F */
#endif
#if (UC_SERIES != XMC13 && UC_SERIES != XMC11)
#define XMC_VADC_REQ_GT_LEDTS0_FN   		XMC_VADC_REQ_GT_I /**< VADC Gating input I */
#define XMC_VADC_REQ_GT_LEDTS1_FN   		XMC_VADC_REQ_GT_J /**< VADC Gating input J */
#endif
#define XMC_VADC_REQ_GT_ERU0_PDOUT2 		XMC_VADC_REQ_GT_K /**< VADC Gating input K */
#define XMC_VADC_REQ_GT_ERU0_PDOUT3 		XMC_VADC_REQ_GT_L /**< VADC Gating input L */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_VADC_REQ_GT_CCU80_ST0   		XMC_VADC_REQ_GT_M /**< VADC Gating input M */
#define XMC_VADC_REQ_GT_CCU80_ST1   		XMC_VADC_REQ_GT_N /**< VADC Gating input N */
#endif
#define XMC_VADC_REQ_GT_ERU0_PDOUT0 		XMC_VADC_REQ_GT_O /**< VADC Gating input O */
#define XMC_VADC_REQ_GT_ERU0_PDOUT1 		XMC_VADC_REQ_GT_P /**< VADC Gating input P */

/* Group request source Trigger input connection mappings */
#define XMC_VADC_REQ_TR_CCU40_SR2   		XMC_VADC_REQ_TR_A /**< VADC Trigger input A */
#define XMC_VADC_REQ_TR_CCU40_SR3   		XMC_VADC_REQ_TR_B /**< VADC Trigger input B */
#if (UC_SERIES != XMC11)
#define XMC_VADC_REQ_TR_BCCU0_TRIGOUT0 		XMC_VADC_REQ_TR_F /**< VADC Global Background Source Trigger input F */
#define XMC_VADC_REQ_TR_G0_BCCU0_TRIGOUT0 	XMC_VADC_REQ_TR_F /**< VADC Group 0 Trigger input F */
#define XMC_VADC_REQ_TR_G1_BCCU0_TRIGOUT1 	XMC_VADC_REQ_TR_F /**< VADC Group1 Trigger input F */
#endif
#define XMC_VADC_REQ_TR_ERU0_IOUT2      	XMC_VADC_REQ_TR_G /**< VADC Trigger input G */
#define XMC_VADC_REQ_TR_ERU0_IOUT3      	XMC_VADC_REQ_TR_H /**< VADC Trigger input H */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_VADC_REQ_TR_CCU80_SR2       	XMC_VADC_REQ_TR_I /**< VADC Trigger input I */
#define XMC_VADC_REQ_TR_CCU80_SR3       	XMC_VADC_REQ_TR_J /**< VADC Trigger input J */
#endif
#define XMC_VADC_REQ_TR_ERU0_IOUT0      	XMC_VADC_REQ_TR_M /**< VADC Trigger input M */
#define XMC_VADC_REQ_TR_ERU0_IOUT1      	XMC_VADC_REQ_TR_N /**< VADC Trigger input N */
#if (UC_SERIES != XMC12 && UC_SERIES != XMC11)
#define XMC_VADC_REQ_TR_POSIF0_SR1      	XMC_VADC_REQ_TR_O /**< VADC Trigger input O */
#endif
#define XMC_VADC_REQ_TR_REQ_GT_SEL      	XMC_VADC_REQ_TR_P /**< VADC Trigger input P */

#endif

#if ( UC_FAMILY == XMC4 )

/*  Group request source Gating input connection mappings */
#define XMC_CCU_40_ST3          	   XMC_VADC_REQ_GT_A /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_ST3 */
#define XMC_CCU_41_ST3          	   XMC_VADC_REQ_GT_B /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU41_ST3 */
#define XMC_CCU_40_SR0          	   XMC_VADC_REQ_GT_C /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_SR0 */
#define XMC_CCU_41_SR1          	   XMC_VADC_REQ_GT_D /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU41_SR1 */
#define XMC_CCU_80_ST3_A        	   XMC_VADC_REQ_GT_E /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_ST3A */
#define XMC_CCU_80_ST3_B        	   XMC_VADC_REQ_GT_F /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_ST3B */

#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_CCU_81_ST3_A        	   XMC_VADC_REQ_GT_G /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU81_ST3A */
#define XMC_CCU_81_ST3_B        	   XMC_VADC_REQ_GT_H /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU81_ST3B */
#endif

#define XMC_DAC_0_SGN           	   XMC_VADC_REQ_GT_I /**< @deprecated use instead  XMC_VADC_REQ_GT_DAC0_SGN, XMC_VADC_REQ_GT_G0_DAC0_SGN or XMC_VADC_REQ_GT_G2_DAC0_SGN */
#define XMC_DAC_1_SGN           	   XMC_VADC_REQ_GT_I /**< @deprecated use instead  XMC_VADC_REQ_GT_DAC0_SGN, XMC_VADC_REQ_GT_G1_DAC1_SGN or XMC_VADC_REQ_GT_G3_DAC1_SGN */
#define XMC_LEDTS_FN            	   XMC_VADC_REQ_GT_J /**< @deprecated use instead  XMC_VADC_REQ_GT_LEDTS_FN */
#define XMC_VADC_G0_BLOUT0      	   XMC_VADC_REQ_GT_K /**< @deprecated use instead  XMC_VADC_REQ_GT_VADC_G1BFLOUT0 or XMC_VADC_REQ_GT_G0_VADC_G1BFLOUT0 */
#define XMC_VADC_G1_BLOUT0      	   XMC_VADC_REQ_GT_K /**< @deprecated use instead  XMC_VADC_REQ_GT_VADC_G1BFLOUT0 or XMC_VADC_REQ_GT_G1_VADC_G0BFLOUT0 */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_G2_BLOUT0      	   XMC_VADC_REQ_GT_K /**< @deprecated use instead  XMC_VADC_REQ_GT_VADC_G1BFLOUT0 or XMC_VADC_REQ_GT_G2_VADC_G3BFLOUT0 */
#define XMC_VADC_G3_BLOUT0      	   XMC_VADC_REQ_GT_K /**< @deprecated use instead  XMC_VADC_REQ_GT_VADC_G1BFLOUT0 or XMC_VADC_REQ_GT_G3_VADC_G2BFLOUT0 */
#endif
#define XMC_VADC_G0_SAMPLE      	   XMC_VADC_REQ_GT_L /**< @deprecated use instead  XMC_VADC_REQ_GT_G0_VADC_G3SAMPLE */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_G1_SAMPLE      	   XMC_VADC_REQ_GT_L /**< @deprecated use instead  XMC_VADC_REQ_GT_G1_VADC_G0SAMPLE */
#define XMC_VADC_G2_SAMPLE      	   XMC_VADC_REQ_GT_L /**< @deprecated use instead  XMC_VADC_REQ_GT_G2_VADC_G1SAMPLE */
#define XMC_VADC_G3_SAMPLE      	   XMC_VADC_REQ_GT_L /**< @deprecated use instead  XMC_VADC_REQ_GT_G3_VADC_G2SAMPLE */
#endif	
#define XMC_CCU_80_SR0          	   XMC_VADC_REQ_GT_M /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_SR0 */
#define XMC_CCU_80_SR1          	   XMC_VADC_REQ_GT_N /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU80_SR1 */
#define XMC_ERU_1_PDOUT0        	   XMC_VADC_REQ_GT_O /**< @deprecated use instead  XMC_VADC_REQ_GT_ERU1_PDOUT0 */
#define XMC_ERU_1_PDOUT1 		       XMC_VADC_REQ_GT_P /**< @deprecated use instead  XMC_VADC_REQ_GT_ERU1_PDOUT1 */

/* Group request source Trigger input connection mappings */
#define XMC_CCU_40_SR2          	   XMC_VADC_REQ_TR_A /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU40_SR2 */
#define XMC_CCU_40_SR3                 XMC_VADC_REQ_TR_B /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU40_SR3 */
#define XMC_CCU_41_SR2                 XMC_VADC_REQ_TR_C /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU41_SR2 */
#define XMC_CCU_41_SR3                 XMC_VADC_REQ_TR_D /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU41_SR3 */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_CCU_42_SR3                 XMC_VADC_REQ_TR_E /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU42_SR3 */
#define XMC_CCU_43_SR3                 XMC_VADC_REQ_TR_F /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU43_SR3 */
#endif	
#define XMC_CCU_80_SR2                 XMC_VADC_REQ_TR_I /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU80_SR2 */
#define XMC_CCU_80_SR3                 XMC_VADC_REQ_TR_J /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU80_SR3 */
#define XMC_CCU_81_SR2                 XMC_VADC_REQ_TR_K /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU81_SR2 */
#define XMC_CCU_81_SR3                 XMC_VADC_REQ_TR_L /**< @deprecated use instead  XMC_VADC_REQ_TR_CCU81_SR3 */
#define XMC_ERU_1_IOUT0                XMC_VADC_REQ_TR_M /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU1_IOUT0 */
#define XMC_ERU_1_IOUT1                XMC_VADC_REQ_TR_N /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU1_IOUT1, XMC_VADC_REQ_TR_G0_ERU1_IOUT1 or XMC_VADC_REQ_TR_G1_ERU1_IOUT1 */
#define XMC_ERU_1_IOUT2                XMC_VADC_REQ_TR_N /**< @deprecated use instead  XMC_VADC_REQ_TR_ERU1_IOUT1, XMC_VADC_REQ_TR_G2_ERU1_IOUT2 or XMC_VADC_REQ_TR_G3_ERU1_IOUT1 */
#if ( (UC_SERIES != XMC43) )
#define XMC_POSIF_0_SR1                XMC_VADC_REQ_TR_O /**< @deprecated use instead  XMC_VADC_REQ_TR_POSIF0_SR1, XMC_VADC_REQ_TR_G0_POSIF0_SR1 or XMC_VADC_REQ_TR_G2_POSIF0_SR1 */
#endif
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_POSIF_1_SR1                XMC_VADC_REQ_TR_O /**< @deprecated use instead  XMC_VADC_REQ_TR_POSIF0_SR1, XMC_VADC_REQ_TR_G1_POSIF0_SR1 or XMC_VADC_REQ_TR_G3_POSIF0_SR1  */
#endif
#define XMC_REQ_GT_SEL                 XMC_VADC_REQ_TR_P /**< @deprecated use instead  XMC_VADC_REQ_GT_CCU40_ST3 */

/*  Group request source Gating input connection mappings */
#define XMC_VADC_REQ_GT_CCU40_ST3 	   		XMC_VADC_REQ_GT_A  /**< VADC Gating input A */
#define XMC_VADC_REQ_GT_CCU41_ST3      		XMC_VADC_REQ_GT_B  /**< VADC Gating input B */
#define XMC_VADC_REQ_GT_CCU40_SR0      		XMC_VADC_REQ_GT_C  /**< VADC Gating input C */
#define XMC_VADC_REQ_GT_CCU41_SR1      		XMC_VADC_REQ_GT_D  /**< VADC Gating input D */
#define XMC_VADC_REQ_GT_CCU80_ST3A     		XMC_VADC_REQ_GT_E  /**< VADC Gating input E */
#define XMC_VADC_REQ_GT_CCU80_ST3B     		XMC_VADC_REQ_GT_F  /**< VADC Gating input F */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_REQ_GT_CCU81_ST3A     		XMC_VADC_REQ_GT_G  /**< VADC Gating input G */
#define XMC_VADC_REQ_GT_CCU81_ST3B     		XMC_VADC_REQ_GT_H  /**< VADC Gating input H */
#endif
#define XMC_VADC_REQ_GT_DAC0_SGN       		XMC_VADC_REQ_GT_I  /**< VADC Global Background Source Gating input I */
#define XMC_VADC_REQ_GT_G0_DAC0_SGN    		XMC_VADC_REQ_GT_I  /**< VADC Group 0 Gating input I */
#define XMC_VADC_REQ_GT_G1_DAC1_SGN    		XMC_VADC_REQ_GT_I  /**< VADC Group 1 Gating input I */
#define XMC_VADC_REQ_GT_G2_DAC0_SGN    		XMC_VADC_REQ_GT_I  /**< VADC Group 2 Gating input I */
#define XMC_VADC_REQ_GT_G3_DAC1_SGN    		XMC_VADC_REQ_GT_I  /**< VADC Group 3 Gating input I */
#define XMC_VADC_REQ_GT_LEDTS_FN       		XMC_VADC_REQ_GT_J  /**< VADC Gating input J */
#define XMC_VADC_REQ_GT_VADC_G1BFLOUT0   	XMC_VADC_REQ_GT_K  /**< VADC Global Background Source Gating input K */
#define XMC_VADC_REQ_GT_G0_VADC_G1BFLOUT0   XMC_VADC_REQ_GT_K  /**< VADC Group 0 Gating input K */
#define XMC_VADC_REQ_GT_G1_VADC_G0BFLOUT0   XMC_VADC_REQ_GT_K  /**< VADC Group 1 Gating input K */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_REQ_GT_G2_VADC_G3BFLOUT0   XMC_VADC_REQ_GT_K  /**< VADC Group 2 Gating input K */
#define XMC_VADC_REQ_GT_G3_VADC_G2BFLOUT0   XMC_VADC_REQ_GT_K  /**< VADC Group 3 Gating input K */
#endif
#define XMC_VADC_REQ_GT_G0_VADC_G3SAMPLE    XMC_VADC_REQ_GT_L  /**< VADC Group 0 Gating input L */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_REQ_GT_G1_VADC_G0SAMPLE    XMC_VADC_REQ_GT_L  /**< VADC Group 1 Gating input L */
#define XMC_VADC_REQ_GT_G2_VADC_G1SAMPLE    XMC_VADC_REQ_GT_L  /**< VADC Group 2 Gating input L */
#define XMC_VADC_REQ_GT_G3_VADC_G2SAMPLE    XMC_VADC_REQ_GT_L  /**< VADC Group 3 Gating input L */
#endif	
#define XMC_VADC_REQ_GT_CCU80_SR0      		XMC_VADC_REQ_GT_M  /**< VADC Gating input M */
#define XMC_VADC_REQ_GT_CCU80_SR1      		XMC_VADC_REQ_GT_N  /**< VADC Gating input N */
#define XMC_VADC_REQ_GT_ERU1_PDOUT0    		XMC_VADC_REQ_GT_O  /**< VADC Gating input O */
#define XMC_VADC_REQ_GT_ERU1_PDOUT1    		XMC_VADC_REQ_GT_P  /**< VADC Gating input P */

/* Group request source Trigger input connection mappings */
#define XMC_VADC_REQ_TR_CCU40_SR2      		XMC_VADC_REQ_TR_A  /**< VADC Trigger input A */
#define XMC_VADC_REQ_TR_CCU40_SR3      		XMC_VADC_REQ_TR_B  /**< VADC Trigger input B */
#define XMC_VADC_REQ_TR_CCU41_SR2      		XMC_VADC_REQ_TR_C  /**< VADC Trigger input C */
#define XMC_VADC_REQ_TR_CCU41_SR3      		XMC_VADC_REQ_TR_D  /**< VADC Trigger input D */
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_REQ_TR_CCU42_SR3      		XMC_VADC_REQ_TR_E  /**< VADC Trigger input E */
#define XMC_VADC_REQ_TR_CCU43_SR3      		XMC_VADC_REQ_TR_F  /**< VADC Trigger input F */
#endif	
#if ((UC_SERIES == XMC48) || (UC_SERIES == XMC43))
#define XMC_VADC_REQ_TR_ECAT_SYNC0     		XMC_VADC_REQ_TR_G  /**< VADC Trigger input G */
#define XMC_VADC_REQ_TR_ECAT_SYNC1     		XMC_VADC_REQ_TR_H  /**< VADC Trigger input H */
#endif
#define XMC_VADC_REQ_TR_CCU80_SR2      		XMC_VADC_REQ_TR_I  /**< VADC Trigger input I */
#define XMC_VADC_REQ_TR_CCU80_SR3      		XMC_VADC_REQ_TR_J  /**< VADC Trigger input J */
#define XMC_VADC_REQ_TR_CCU81_SR2      		XMC_VADC_REQ_TR_K  /**< VADC Trigger input K */
#define XMC_VADC_REQ_TR_CCU81_SR3      		XMC_VADC_REQ_TR_L  /**< VADC Trigger input L */
#define XMC_VADC_REQ_TR_ERU1_IOUT0     		XMC_VADC_REQ_TR_M  /**< VADC Trigger input M */
#define XMC_VADC_REQ_TR_ERU1_IOUT1     		XMC_VADC_REQ_TR_M  /**< VADC Global Background Source Trigger input N */
#define XMC_VADC_REQ_TR_G0_ERU1_IOUT1     	XMC_VADC_REQ_TR_N  /**< VADC Group 0 Trigger input N */
#define XMC_VADC_REQ_TR_G1_ERU1_IOUT1     	XMC_VADC_REQ_TR_N  /**< VADC Group 1 Trigger input N */
#define XMC_VADC_REQ_TR_G2_ERU1_IOUT2     	XMC_VADC_REQ_TR_N  /**< VADC Group 2 Trigger input N */
#define XMC_VADC_REQ_TR_G3_ERU1_IOUT2     	XMC_VADC_REQ_TR_N  /**< VADC Group 3 Trigger input N */
#if ( (UC_SERIES != XMC43) )
#define XMC_VADC_REQ_TR_POSIF0_SR1     		XMC_VADC_REQ_TR_O  /**< VADC Global Background Source Trigger input O */
#define XMC_VADC_REQ_TR_G0_POSIF0_SR1     	XMC_VADC_REQ_TR_O  /**< VADC Group 0 Trigger input O */
#define XMC_VADC_REQ_TR_G1_POSIF1_SR1     	XMC_VADC_REQ_TR_O  /**< VADC Group 1 Trigger input O */
#endif
#if ( (UC_SERIES != XMC43) && (UC_SERIES != XMC42) && (UC_SERIES != XMC41))
#define XMC_VADC_REQ_TR_G2_POSIF0_SR1     	XMC_VADC_REQ_TR_O  /**< VADC Group 2 Trigger input O */
#define XMC_VADC_REQ_TR_G3_POSIF1_SR1     	XMC_VADC_REQ_TR_O  /**< VADC Group 3 Trigger input O */
#endif
#define XMC_VADC_REQ_TR_REQ_GT_SEL     		XMC_VADC_REQ_TR_P  /**< VADC Trigger input P */

#endif

#ifdef __cplusplus
}
#endif

#endif
