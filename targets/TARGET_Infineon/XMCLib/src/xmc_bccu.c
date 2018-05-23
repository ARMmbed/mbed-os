/**
 * @file xmc_bccu.c
 * @date 2015-06-20
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
 * 2015-02-19:
 *     - Initial draft <br>
 *
 * 2015-05-08:
 *     - Minor bug fixes in following APIs: XMC_BCCU_ConcurrentStartDimming(), XMC_BCCU_ConcurrentAbortDimming(),
 *               XMC_BCCU_SetGlobalDimmingLevel() <br>
 *     
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * <b>Detailed description of file:</b><br>
 * APIs for the functional blocks of BCCU have been defined:<br>
 * -- GLOBAL configuration <br>
 * -- Clock configuration, Function/Event configuration, Interrupt configuration <br>
 *
 * @endcond
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_bccu.h>

#if defined(BCCU0)
#include <xmc_scu.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_BCCU_NO_OF_CHANNELS    (9U)
#define XMC_BCCU_CHANNEL_MASK      ((0x1 << XMC_BCCU_NO_OF_CHANNELS)-1)
#define XMC_BCCU_NO_OF_DIM_ENGINE  (3U)
#define XMC_BCCU_DIM_ENGINE_MASK   (((0x1 << XMC_BCCU_NO_OF_DIM_ENGINE)-1))

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL/UTILITY ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/*
 * API to initialise the global resources of a BCCU module
 */
void XMC_BCCU_GlobalInit(XMC_BCCU_t *const bccu, const XMC_BCCU_GLOBAL_CONFIG_t *const config)
{
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_BCCU0);

  bccu->GLOBCON = config->globcon;
  
  bccu->GLOBCLK = config->globclk;
  bccu->GLOBDIM = config->global_dimlevel;  

}

/*
 * API to configure the global trigger mode & delay of a BCCU module
 */
void XMC_BCCU_ConfigGlobalTrigger(XMC_BCCU_t *const bccu, XMC_BCCU_TRIGMODE_t mode, XMC_BCCU_TRIGDELAY_t delay)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TM_Msk | BCCU_GLOBCON_TRDEL_Msk);
  bccu->GLOBCON |= ((uint32_t)mode | ((uint32_t)delay << BCCU_GLOBCON_TRDEL_Pos));
}

/*
 * API to configure the trap input selection of a BCCU module
 */
void XMC_BCCU_SelectTrapInput (XMC_BCCU_t *const bccu, XMC_BCCU_CH_TRAP_IN_t input)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TRAPIS_Msk);
  bccu->GLOBCON |= ((uint32_t)input << BCCU_GLOBCON_TRAPIS_Pos);
}

/*
 * API to configure the trap edge selection of a BCCU module
 */
void XMC_BCCU_SetTrapEdge (XMC_BCCU_t *const bccu, XMC_BCCU_CH_TRAP_EDGE_t edge)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TRAPED_Msk);
  bccu->GLOBCON |= ((uint32_t)edge << BCCU_GLOBCON_TRAPED_Pos);
}

/*
 * API to configure the suspend mode of a BCCU module
 */
void XMC_BCCU_ConfigSuspendMode (XMC_BCCU_t *const bccu, XMC_BCCU_SUSPEND_MODE_t mode)
{
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_SUSCFG_Msk);
  bccu->GLOBCON |= ((uint32_t)mode << BCCU_GLOBCON_SUSCFG_Pos);
}

/*
 * API to configure number of consecutive zeroes allowed at modulator output (flicker watch-dog number)
 */
void XMC_BCCU_SetFlickerWDThreshold (XMC_BCCU_t *const bccu, uint32_t threshold_no)
{
  XMC_ASSERT("XMC_BCCU_SetFlickerWDThreshold: Invalid threshold no", (threshold_no <= BCCU_GLOBCON_WDMBN_Msk));
  
  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_WDMBN_Msk);
  bccu->GLOBCON |= (uint32_t)(threshold_no << BCCU_GLOBCON_WDMBN_Pos);
}

/*
 * API to configure the fast clock prescaler factor of a BCCU module
 */
void XMC_BCCU_SetFastClockPrescaler (XMC_BCCU_t *const bccu, uint32_t div)
{
  XMC_ASSERT("XMC_BCCU_SetFastClockPrescaler: Invalid divider value", (div <= BCCU_GLOBCLK_FCLK_PS_Msk));
  
  bccu->GLOBCLK &= ~(uint32_t)(BCCU_GLOBCLK_FCLK_PS_Msk);
  bccu->GLOBCLK |= div;
  
}

/*
 * API to configure the dimmer clock prescaler factor of a BCCU module
 */
void XMC_BCCU_SetDimClockPrescaler (XMC_BCCU_t *const bccu, uint32_t div)
{
  XMC_ASSERT("XMC_BCCU_SetDimClockPrescaler: Invalid divider value", (div <= BCCU_GLOBCLK_DCLK_PS_Msk));

  bccu->GLOBCLK &= ~(uint32_t)(BCCU_GLOBCLK_DCLK_PS_Msk);
  bccu->GLOBCLK |= (uint32_t)(div << BCCU_GLOBCLK_DCLK_PS_Pos);
  
}

/*
 * API to configure the modulator output (bit-time) clock prescaler factor of a BCCU module
 */
void XMC_BCCU_SelectBitClock (XMC_BCCU_t *const bccu, XMC_BCCU_BCLK_MODE_t div)
{
  bccu->GLOBCLK &= ~(uint32_t)(BCCU_GLOBCLK_BCS_Msk);
  bccu->GLOBCLK |= ((uint32_t)div << BCCU_GLOBCLK_BCS_Pos);
}

/*
 * API to enable the channels at the same time
 */
void XMC_BCCU_ConcurrentEnableChannels (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentEnableChannels: Invalid channel mask", (mask <= XMC_BCCU_CHANNEL_MASK));

  bccu->CHEN |= mask;
}

/*
 * API to disable the channels at the same time
 */
void XMC_BCCU_ConcurrentDisableChannels (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentDisableChannels: Invalid channel mask", (mask <= XMC_BCCU_CHANNEL_MASK));
  bccu->CHEN &= ~(uint32_t)(mask);
}

/*
 * API to set the channel's output passive levels at the same time
 */
void XMC_BCCU_ConcurrentSetOutputPassiveLevel(XMC_BCCU_t *const bccu, uint32_t chan_mask, XMC_BCCU_CH_ACTIVE_LEVEL_t level)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentSetOutputPassiveLevel: Invalid channel mask", (chan_mask <= XMC_BCCU_CHANNEL_MASK));
  
  bccu->CHOCON &= ~(uint32_t)(chan_mask);
  bccu->CHOCON |= (chan_mask * (uint32_t)level);
}

/*
 * API to enable the various types of traps at the same time
 */
void XMC_BCCU_ConcurrentEnableTrap (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentEnableTrap: Invalid channel mask", (mask <= XMC_BCCU_CHANNEL_MASK));

  bccu->CHOCON |= (uint32_t)(mask << BCCU_CHOCON_CH0TPE_Pos);
}

/*
 * API to disable the various types of traps at the same time
 */
void XMC_BCCU_ConcurrentDisableTrap (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentDisableTrap: Invalid channel mask", (mask <= XMC_BCCU_CHANNEL_MASK));

  bccu->CHOCON &= ~(uint32_t)(mask << BCCU_CHOCON_CH0TPE_Pos);
}

/*
 * API to configure trigger mode and trigger delay at the same time, and also configure the channel enable
 */
void XMC_BCCU_ConcurrentConfigTrigger (XMC_BCCU_t *const bccu, XMC_BCCU_TRIG_CONFIG_t *trig)
{
  uint32_t reg;
  
  XMC_ASSERT("XMC_BCCU_ConcurrentConfigTrigger: Invalid channel mask", (trig->mask_chans <= XMC_BCCU_CHANNEL_MASK));

  bccu->GLOBCON &= ~(uint32_t)(BCCU_GLOBCON_TM_Msk | BCCU_GLOBCON_TRDEL_Msk);
  bccu->GLOBCON |= ((uint32_t)trig->mode | ((uint32_t)trig->delay << BCCU_GLOBCON_TRDEL_Pos));
  reg = 0U;
  reg |= trig->mask_chans;
  reg |= ((uint32_t)trig->mask_trig_lines << BCCU_CHTRIG_TOS0_Pos);
  bccu->CHTRIG = reg;
}

/*
 * API to start the linear walk of the channels to change towards target intensity at the same time
 */
void XMC_BCCU_ConcurrentStartLinearWalk (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentStartLinearWalk: Invalid channel mask", (mask <= XMC_BCCU_CHANNEL_MASK));

  bccu->CHSTRCON |= (uint32_t)(mask);
}

/*
 * API to abort the linear walk of the channels at the same time
 */
void XMC_BCCU_ConcurrentAbortLinearWalk (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentAbortLinearWalk: Invalid channel mask", (mask <= XMC_BCCU_CHANNEL_MASK));

  bccu->CHSTRCON |= (uint32_t)(mask << BCCU_CHSTRCON_CH0A_Pos);
}

/*
 * API to enable the dimming engines at the same time
 */
void XMC_BCCU_ConcurrentEnableDimmingEngine (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentEnableDimmingEngine: Invalid dimming engine mask", (mask <= XMC_BCCU_DIM_ENGINE_MASK));

  bccu->DEEN = (uint32_t)(mask);
}

/*
 * API to enable the dimming engines at the same time
 */
void XMC_BCCU_ConcurrentDisableDimmingEngine (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentDisableDimmingEngine: Invalid dimming engine mask", (mask <= XMC_BCCU_DIM_ENGINE_MASK));

  bccu->DEEN &= ~(uint32_t)(mask);
}

/*
 * API to start the dimming engines at the same time to change towards target dim level
 */
void XMC_BCCU_ConcurrentStartDimming (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentStartDimming: Invalid dimming engine mask", (mask <= XMC_BCCU_DIM_ENGINE_MASK));

  bccu->DESTRCON = (uint32_t)(mask);
}

/*
 * API to abort the dimming engines at the same time
 */
void XMC_BCCU_ConcurrentAbortDimming (XMC_BCCU_t *const bccu, uint32_t mask)
{
  XMC_ASSERT("XMC_BCCU_ConcurrentAbortDimming: Invalid dimming engine mask", (mask <= XMC_BCCU_DIM_ENGINE_MASK));

  bccu->DESTRCON = (uint32_t)(mask << BCCU_DESTRCON_DE0A_Pos);
}

/*
 * API to configure the dim level of a dimming engine
 */
void  XMC_BCCU_SetGlobalDimmingLevel (XMC_BCCU_t *const bccu, uint32_t level)
{
  XMC_ASSERT("XMC_BCCU_SetGlobalDimmingLevel: Invalid global dimming level", (level <= BCCU_GLOBDIM_GLOBDIM_Msk));

  bccu->GLOBDIM = level;
}

/*
 * API to enable a specific channel
 */
void XMC_BCCU_EnableChannel (XMC_BCCU_t *const bccu, uint32_t chan_no)
{
  XMC_ASSERT("XMC_BCCU_EnableChannel: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHEN |= (uint32_t)(BCCU_CHEN_ECH0_Msk << chan_no);
}

/*
 * API to disable a specific channel
 */
void XMC_BCCU_DisableChannel (XMC_BCCU_t *const bccu, uint32_t chan_no)
{
  XMC_ASSERT("XMC_BCCU_DisableChannel: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHEN &= ~(uint32_t)(BCCU_CHEN_ECH0_Msk << chan_no);
}

/*
 * API to set the specific channel's passive level
 */
void XMC_BCCU_SetOutputPassiveLevel(XMC_BCCU_t *const bccu, uint32_t chan_no, XMC_BCCU_CH_ACTIVE_LEVEL_t level)
{
  XMC_ASSERT("XMC_BCCU_SetOutputPassiveLevel: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHOCON |= ((uint32_t)level << chan_no);
}

/*
 * API to enable the specific channel trap
 */
void XMC_BCCU_EnableTrap (XMC_BCCU_t *const bccu, uint32_t chan_no)
{
  XMC_ASSERT("XMC_BCCU_EnableTrap: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHOCON |= (uint32_t)(BCCU_CHOCON_CH0TPE_Msk << chan_no);
}

/*
 * API to disable the specific channel trap
 */
void XMC_BCCU_DisableTrap (XMC_BCCU_t *const bccu, uint32_t chan_no)
{
  XMC_ASSERT("XMC_BCCU_DisableTrap: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHOCON &= ~(uint32_t)(BCCU_CHOCON_CH0TPE_Msk << chan_no);
}

/*
 * API to configure specific channel trigger enable and trigger line.
 */
void XMC_BCCU_EnableChannelTrigger (XMC_BCCU_t *const bccu, uint32_t chan_no, XMC_BCCU_CH_TRIGOUT_t trig_line)
{
  uint32_t reg;
  XMC_ASSERT("XMC_BCCU_EnableChannelTrigger: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHTRIG &= ~(uint32_t)(BCCU_CHTRIG_TOS0_Msk << chan_no);
  reg = (uint32_t)(BCCU_CHTRIG_ET0_Msk << chan_no);
  reg |= ((uint32_t)trig_line << (BCCU_CHTRIG_TOS0_Pos + chan_no));
  bccu->CHTRIG |= reg;
}

/*
 * API to disable specific channel
 */
void XMC_BCCU_DisableChannelTrigger (XMC_BCCU_t *const bccu, uint32_t chan_no)
{
  XMC_ASSERT("XMC_BCCU_DisableChannelTrigger: Invalid channel number", (chan_no <= (XMC_BCCU_NO_OF_CHANNELS-1)));

  bccu->CHTRIG &= ~(uint32_t)(BCCU_CHTRIG_ET0_Msk << chan_no);
}

/*
 * API to initialise the channel of a BCCU module
 */
void XMC_BCCU_CH_Init (XMC_BCCU_CH_t *const channel, const XMC_BCCU_CH_CONFIG_t *const config)
{
  channel->CHCONFIG = config->chconfig;
 
  channel->PKCMP = config->pkcmp;
 
  channel->PKCNTR = config->pkcntr;
}

/*
 * API to configure channel trigger edge and force trigger edge
 */
void XMC_BCCU_CH_ConfigTrigger (XMC_BCCU_CH_t *const channel, XMC_BCCU_CH_TRIG_EDGE_t edge, uint32_t force_trig_en)
{
  uint32_t reg; 
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_TRED_Msk | BCCU_CH_CHCONFIG_ENFT_Msk);
  
  reg = ((uint32_t)edge << BCCU_CH_CHCONFIG_TRED_Pos);
  reg |= (uint32_t)(force_trig_en << BCCU_CH_CHCONFIG_ENFT_Pos);
  channel->CHCONFIG |= reg;
}

/*
 * API to configure the linear walker clock prescaler factor of a BCCU channel
 */
void XMC_BCCU_CH_SetLinearWalkPrescaler (XMC_BCCU_CH_t *const channel, uint32_t clk_div)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_LINPRES_Msk);
  channel->CHCONFIG |= (uint32_t)(clk_div << BCCU_CH_CHCONFIG_LINPRES_Pos);
}

/*
 * API to set channel target intensity
 */
void XMC_BCCU_CH_SetTargetIntensity (XMC_BCCU_CH_t *const channel, uint32_t ch_int)
{
  channel->INTS = ch_int;
}

/*
 * API to retrieve the channel actual intensity
 */
uint32_t XMC_BCCU_CH_ReadIntensity (XMC_BCCU_CH_t *const channel)
{
  return (uint32_t)(channel->INT & BCCU_CH_INT_CHINT_Msk);
}

/*
 * API to enable packer. Also configures packer threshold, off-time and on-time compare levels
 */
void XMC_BCCU_CH_EnablePacker (XMC_BCCU_CH_t *const channel, uint32_t thresh, uint32_t off_comp, uint32_t on_comp)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_PEN_Msk | BCCU_CH_CHCONFIG_PKTH_Msk);
  channel->CHCONFIG |= thresh;
  channel->PKCMP = (off_comp | (uint32_t)(on_comp << BCCU_CH_PKCMP_ONCMP_Pos));
  channel->CHCONFIG |= (uint32_t)BCCU_CH_CHCONFIG_PEN_Msk;
}

/*
 * API to configure packer threshold
 */
void XMC_BCCU_CH_SetPackerThreshold (XMC_BCCU_CH_t *const channel, uint32_t val)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_PKTH_Msk);
  channel->CHCONFIG |= val;
}

/*
 * API to configure packer off-time compare level
 */
void XMC_BCCU_CH_SetPackerOffCompare (XMC_BCCU_CH_t *const channel, uint32_t level)
{
  channel->PKCMP &= ~(uint32_t)(BCCU_CH_PKCMP_OFFCMP_Msk);
  channel->PKCMP |= level;
}

/*
 * API to configure packer on-time compare level.
 */
void XMC_BCCU_CH_SetPackerOnCompare (XMC_BCCU_CH_t *const channel, uint32_t level)
{
  channel->PKCMP &= ~(uint32_t)(BCCU_CH_PKCMP_ONCMP_Msk);
  channel->PKCMP |= (level << BCCU_CH_PKCMP_ONCMP_Pos);
}

/*
 * API to disable a packer.
 */
void XMC_BCCU_CH_DisablePacker (XMC_BCCU_CH_t *const channel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_PEN_Msk);
}

/*
 * API to set packer off-time counter value
 */
void XMC_BCCU_CH_SetPackerOffCounter (XMC_BCCU_CH_t *const channel, uint32_t cnt_val)
{
  channel->PKCNTR &= ~(uint32_t)(BCCU_CH_PKCNTR_OFFCNTVAL_Msk);
  channel->PKCNTR |= cnt_val;
}

/*
 * API to set packer on-time counter value
 */
void XMC_BCCU_CH_SetPackerOnCounter (XMC_BCCU_CH_t *const channel, uint32_t cnt_val)
{
  channel->PKCNTR &= ~(uint32_t)(BCCU_CH_PKCNTR_ONCNTVAL_Msk);
  channel->PKCNTR |= (uint32_t)(cnt_val << BCCU_CH_PKCNTR_ONCNTVAL_Pos);
}

/*
 * API to select the dimming engine of a channel
 */
void XMC_BCCU_CH_SelectDimEngine (XMC_BCCU_CH_t *const channel, XMC_BCCU_CH_DIMMING_SOURCE_t sel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_DSEL_Msk);
  channel->CHCONFIG |= ((uint32_t)sel << BCCU_CH_CHCONFIG_DSEL_Pos);
}

/*
 * API to bypass the dimming engine. And the brightness of channel is depending only on
 * intensity of the channel.
 */
void XMC_BCCU_CH_EnableDimmingBypass (XMC_BCCU_CH_t *const channel)
{
  channel->CHCONFIG |= (uint32_t)(BCCU_CH_CHCONFIG_DBP_Msk);
}

/*
 * API to disable the bypass of dimming engine. And the brightness of channel is depending
 * on intensity of channel and dimming level of dimming engine.
 */
void XMC_BCCU_CH_DisableDimmingBypass (XMC_BCCU_CH_t *const channel)
{
  channel->CHCONFIG &= ~(uint32_t)(BCCU_CH_CHCONFIG_DBP_Msk);
}

/*
 * API to initialise a specific dimming engine of a BCCU module
 */
void XMC_BCCU_DIM_Init (XMC_BCCU_DIM_t *const dim_engine, const XMC_BCCU_DIM_CONFIG_t *const config)
{
  dim_engine->DTT = config->dtt;
}

/*
 * API to set dimming engine target dim level
 */
void XMC_BCCU_DIM_SetTargetDimmingLevel (XMC_BCCU_DIM_t *const dim_engine, uint32_t level)
{
  dim_engine->DLS = level;
}

/*
 * API to configure the dimming clock prescaler factor of a dimming engine
 */
void XMC_BCCU_DIM_SetDimDivider (XMC_BCCU_DIM_t *const dim_engine, uint32_t div)
{
  dim_engine->DTT &= ~(uint32_t)(BCCU_DE_DTT_DIMDIV_Msk);
  dim_engine->DTT |= div;
}

/*
 * API to configure the dimming curve
 */
void XMC_BCCU_DIM_ConfigDimCurve (XMC_BCCU_DIM_t *const dim_engine, uint32_t dither_en, XMC_BCCU_DIM_CURVE_t sel)
{
  uint32_t reg;
  dim_engine->DTT &= ~(uint32_t)(BCCU_DE_DTT_DTEN_Msk | BCCU_DE_DTT_CSEL_Msk);
  reg = (uint32_t)(dither_en << BCCU_DE_DTT_DTEN_Pos);
  reg |= ((uint32_t)sel << BCCU_DE_DTT_CSEL_Pos);
  dim_engine->DTT |= reg;
}

#endif /* BCCU0 */
