
/**
 * @file xmc_hrpwm_map.h
 * @date 2015-06-20
 *
 * @cond
 **********************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without           
 * modification,are permitted provided that the following conditions are met:   
 *                                                                              
 *   Redistributions of source code must retain the above copyright notice,      
 *   this list of conditions and the following disclaimer.                        
 * 
 *   Redistributions in binary form must reproduce the above copyright notice,   
 *   this list of conditions and the following disclaimer in the documentation    
 *   and/or other materials provided with the distribution.                       
 * 
 *   Neither the name of the copyright holders nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   
 * ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   
 * LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      
 * CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   
 * POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share        
 * modifications, enhancements or bug fixes with Infineon Technologies AG       
 * dave@infineon.com).                                                          
 **********************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-06-20: 
 *     - Updated copyright and change history section.
 *
 * @endcond 
 *
 */
 
/**
 *
 * @brief HRPWM mapping for XMC4 microcontroller family. <br>
 *
 */
 
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_hrpwm.h"

#ifndef XMC_HRPWM_MAP_H
#define XMC_HRPWM_MAP_H

#if ((UC_DEVICE == XMC4400) || (UC_DEVICE == XMC4200) || (UC_DEVICE == XMC4100))
/* CSG0 - General input to control Blanking and Switch of the Comparator */
#define XMC_HRPWM_CSG0_BL_P1_4        XMC_HRPWM_CSG_INPUT_SEL_IA
#define XMC_HRPWM_CSG0_BL_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG0_BL_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG0_BL_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG0_BL_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG0_BL_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG0_BL_CCU40_ST0   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG0_BL_CCU41_ST0   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG0_BL_HRPWM_QOUT0 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG0_BL_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG0_BL_CCU40_SR0   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG0_BL_CCU41_SR0   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG0_BL_HRPWM_SR2   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG0_BL_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG0_BL_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG0_BL_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG0 - General input to control start/stop/trigger for Slope Control Logic */
#define XMC_HRPWM_CSG0_SC_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG0_SC_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG0_SC_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG0_SC_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG0_SC_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG0_SC_CCU40_ST0   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG0_SC_CCU41_ST0   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG0_SC_HRPWM_QOUT0 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG0_SC_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG0_SC_CCU40_SR0   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG0_SC_CCU41_SR0   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG0_SC_HRPWM_C0O   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG0_SC_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG0_SC_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG0_SC_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG1 - General input to control Blanking and Switch of the Comparator */
#define XMC_HRPWM_CSG1_BL_P2_4        XMC_HRPWM_CSG_INPUT_SEL_IA
#define XMC_HRPWM_CSG1_BL_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG1_BL_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG1_BL_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG1_BL_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG1_BL_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG1_BL_CCU40_ST1   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG1_BL_CCU41_ST1   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG1_BL_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG1_BL_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG1_BL_CCU40_SR1   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG1_BL_CCU41_SR1   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG1_BL_HRPWM_SR2   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG1_BL_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG1_BL_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG1_BL_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG1 - General input to control start/stop/trigger for Slope Control Logic */
#define XMC_HRPWM_CSG1_SC_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG1_SC_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG1_SC_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG1_SC_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG1_SC_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG1_SC_CCU40_ST1   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG1_SC_CCU41_ST1   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG1_SC_HRPWM_QOUT1 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG1_SC_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG1_SC_CCU40_SR1   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG1_SC_CCU41_SR1   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG1_SC_HRPWM_C1O   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG1_SC_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG1_SC_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG1_SC_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG2 - General input to control Blanking and Switch of the Comparator */
#define XMC_HRPWM_CSG2_BL_P2_5        XMC_HRPWM_CSG_INPUT_SEL_IA
#define XMC_HRPWM_CSG2_BL_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG2_BL_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG2_BL_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG2_BL_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG2_BL_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG2_BL_CCU40_ST2   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG2_BL_CCU41_ST2   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG2_BL_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG2_BL_HRPWM_QOUT3 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG2_BL_CCU40_SR2   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG2_BL_CCU41_SR2   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG2_BL_HRPWM_SR2   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG2_BL_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG2_BL_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG2_BL_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

/* CSG2 - General input to control start/stop/trigger for Slope Control Logic */
#define XMC_HRPWM_CSG2_SC_SCU_GSHR0   XMC_HRPWM_CSG_INPUT_SEL_IB
#define XMC_HRPWM_CSG2_SC_CCU80_ST0   XMC_HRPWM_CSG_INPUT_SEL_IC
#define XMC_HRPWM_CSG2_SC_CCU80_ST1   XMC_HRPWM_CSG_INPUT_SEL_ID
#define XMC_HRPWM_CSG2_SC_CCU80_ST2   XMC_HRPWM_CSG_INPUT_SEL_IE
#define XMC_HRPWM_CSG2_SC_CCU80_ST3   XMC_HRPWM_CSG_INPUT_SEL_IF
#define XMC_HRPWM_CSG2_SC_CCU40_ST2   XMC_HRPWM_CSG_INPUT_SEL_IG
#define XMC_HRPWM_CSG2_SC_CCU41_ST2   XMC_HRPWM_CSG_INPUT_SEL_IH
#define XMC_HRPWM_CSG2_SC_HRPWM_QOUT2 XMC_HRPWM_CSG_INPUT_SEL_II
#define XMC_HRPWM_CSG2_SC_HRPWM_QOUT3 XMC_HRPWM_CSG_INPUT_SEL_IJ
#define XMC_HRPWM_CSG2_SC_CCU40_SR2   XMC_HRPWM_CSG_INPUT_SEL_IK
#define XMC_HRPWM_CSG2_SC_CCU41_SR2   XMC_HRPWM_CSG_INPUT_SEL_IL
#define XMC_HRPWM_CSG2_SC_HRPWM_C2O   XMC_HRPWM_CSG_INPUT_SEL_IM
#define XMC_HRPWM_CSG2_SC_HRPWM_SR3   XMC_HRPWM_CSG_INPUT_SEL_IN
#define XMC_HRPWM_CSG2_SC_ERU1_IOUT0  XMC_HRPWM_CSG_INPUT_SEL_IO
#define XMC_HRPWM_CSG2_SC_ERU1_IOUT1  XMC_HRPWM_CSG_INPUT_SEL_IP

#endif

#endif  /* XMC_HRPWM_MAP_H */
