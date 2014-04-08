/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_ftm_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_init(uint8_t instance, ftm_config_t *config)
{
  switch(config->mode)
  {
  case kFtmInputCapture:
    break;
  case kFtmOutputCompare:
     break;
  case kFtmEdgeAlignedPWM:
  case kFtmCenterAlignedPWM:
  case kFtmCombinedPWM:
    ftm_hal_enable_pwm_mode(instance, config);
     break;
  case kFtmDualEdgeCapture:
    break;
  case kFtmQuadCapture:
    break;
  default:
    assert(0);
    break;
  }
}

/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_enable_pwm_mode(uint8_t instance, ftm_config_t *config)
{
  ftm_hal_enable_dual_capture(instance, config->channel, false);
  ftm_hal_set_channel_edge_level(instance, config->channel,config->edge_mode.ftm_pwm_edge_mode? 1:2);
  switch(config->mode)
  {
  case kFtmEdgeAlignedPWM:
    ftm_hal_enable_dual_channel_combine(instance, config->channel, false);
    ftm_hal_set_cpwms(instance, 0);
    ftm_hal_set_channel_MSnBA_mode(instance, config->channel, 2);
     break;
  case kFtmCenterAlignedPWM:
    ftm_hal_enable_dual_channel_combine(instance, config->channel, false);
    ftm_hal_set_cpwms(instance, 1);
     break;
  case kFtmCombinedPWM:
    ftm_hal_set_cpwms(instance, 0);
    ftm_hal_enable_dual_channel_combine(instance, config->channel, true);
     break;
  default:
    assert(0);
    break;
  }
}

/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_disable_pwm_mode(uint8_t instance, ftm_config_t *config)
{

  ftm_hal_set_channel_count_value(instance, config->channel, 0);
  switch(config->mode)
  {
  case kFtmEdgeAlignedPWM:
    ftm_hal_set_channel_MSnBA_mode(instance, config->channel, 1);
     break;
  case kFtmCenterAlignedPWM:
    ftm_hal_set_cpwms(instance, 0);
     break;
  case kFtmCombinedPWM:
    ftm_hal_enable_dual_channel_combine(instance, config->channel, false);
     break;
  default:
    assert(0);
    break;
  }
}
/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_reset(uint8_t instance)
{
  uint8_t chan = ((instance == 1) || (instance == 2)) ? 2 : HW_FTM_CHANNEL_COUNT;
  ftm_hal_set_clock_source(instance,kClock_source_FTM_None);
  HW_FTM_SC_WR(instance, 0);
  HW_FTM_CNT_WR(instance, 0);
  HW_FTM_MOD_WR(instance, 0);

  /*instance 1 and 2 only has two channels,0 and 1*/
  for(int i = 0; i<chan; i++)
  {
    HW_FTM_CnSC_WR(instance, i, 0);
    HW_FTM_CnV_WR(instance, i, 0);
  }
  HW_FTM_CNTIN_WR(instance, 0);
  HW_FTM_STATUS_WR(instance, 0);
  HW_FTM_MODE_WR(instance, 0x00000004);
  HW_FTM_SYNC_WR(instance, 0);
  HW_FTM_OUTINIT_WR(instance, 0);
  HW_FTM_OUTMASK_WR(instance, 0);
  HW_FTM_COMBINE_WR(instance, 0);
  HW_FTM_DEADTIME_WR(instance, 0);
  HW_FTM_EXTTRIG_WR(instance, 0);
  HW_FTM_POL_WR(instance, 0);
  HW_FTM_FMS_WR(instance, 0);
  HW_FTM_FILTER_WR(instance, 0);
  HW_FTM_FLTCTRL_WR(instance, 0);
  /*HW_FTM_QDCTRL_WR(instance, 0);*/
  HW_FTM_CONF_WR(instance, 0);
  HW_FTM_FLTPOL_WR(instance, 0);
  HW_FTM_SYNCONF_WR(instance, 0);
  HW_FTM_INVCTRL_WR(instance, 0);
  HW_FTM_SWOCTRL_WR(instance, 0);
  HW_FTM_PWMLOAD_WR(instance, 0);
}
/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_set_hardware_trigger(uint8_t instance, uint8_t trigger_num, bool enable)
{
   assert(instance <HW_FTM_INSTANCE_COUNT);
   switch(trigger_num)
   {
      case 0:
        BW_FTM_SYNC_TRIG0(instance, enable?1:0);
        break;

      case 1:
        BW_FTM_SYNC_TRIG1(instance, enable?1:0);
        break;

      case 2:
        BW_FTM_SYNC_TRIG2(instance, enable?1:0);
        break;

      default:
         assert(0);
         break;
   }
}

/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_enable_channel_trigger(uint8_t instance, uint8_t channel, bool val)
{
      uint8_t bit = val? 1:0;
      uint32_t value = (channel > 1U)? (uint8_t)(bit<<(channel-2U)) : (uint8_t)(bit<<(channel+4U));
      assert(instance <HW_FTM_INSTANCE_COUNT && channel < (HW_FTM_CHANNEL_COUNT - 2));
      BW_FTM_EXTTRIG_INITTRIGEN(instance, val);
      val?   HW_FTM_EXTTRIG_SET(instance, value)
           : HW_FTM_EXTTRIG_CLR(instance, value);
}

/*see fsl_ftm_hal.h for documentation of this function*/
void ftm_hal_set_channel_input_capture_filter(uint8_t instance, uint8_t channel, uint8_t val)
{
   assert(instance < HW_FTM_INSTANCE_COUNT  && channel < HW_CHAN4);
   switch(channel)
   {
      case 0:
        BW_FTM_FILTER_CH0FVAL(instance, val);
        break;

      case 1:
        BW_FTM_FILTER_CH1FVAL(instance, val);
        break;

      case 2:
        BW_FTM_FILTER_CH2FVAL(instance, val);
        break;

      case 3:
        BW_FTM_FILTER_CH3FVAL(instance, val);
        break;

      default:
         assert(0);
         break;
   }
}

/*see fsl_ftm_hal.h for documentation of this function          */
/*void ftm_hal_set_deadtime(uint8_t instance, uint_32 us)*/
