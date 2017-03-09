#include "W7500x.h"
#include "W7500x_exti.h"
#include "W7500x_gpio.h"


/**
  * @brief  Deinitializes the EXTI peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void EXTI_DeInit(void)
{
    uint32_t i, loop =16;
    for(i=0; i<loop; i++)
    {
        EXTI_PA->Port[i] = 0x00;
        EXTI_PB->Port[i] = 0x00;
        EXTI_PC->Port[i] = 0x00;
    }
    for(i=0; i<5; i++)
    {
        EXTI_PD->Port[i] = 0x00;
    }
}

/**
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the EXTI_InitStruct.
  * @param  EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure
  *         that contains the configuration information for the EXTI peripheral.
  * @retval None
  */
void EXTI_Init(PAD_Type Px, EXTI_InitTypeDef* EXTI_InitStruct)
{
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00, loop = 16;
    P_Port_Def *px_exti;

    assert_param(IS_PAD_TYPE(Px));

    if      (Px == PAD_PA)        px_exti  = EXTI_PA;
    else if (Px == PAD_PB)        px_exti  = EXTI_PB;
    else if (Px == PAD_PC)        px_exti  = EXTI_PC;
    else
    {        
        px_exti  = (P_Port_Def*)EXTI_PD;
        loop = 5;
    }

    for(pinpos = 0x00; pinpos < loop; pinpos++)
    {
        pos = ((uint32_t)0x01) << pinpos;

        currentpin = (EXTI_InitStruct->EXTI_Line) & pos;
        if(currentpin == pos)
        {   
            px_exti->Port[pinpos] |= EXTI_Px_INTEN_ENABLE;

            if(EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising)
                px_exti->Port[pinpos] |=  EXTI_Px_INTPOR_RISING_EDGE;
            else
                px_exti->Port[pinpos] |= EXTI_Px_INTPOR_FALLING_EDGE;
        }
    }

}

void EXTI_Polarity_Set(PAD_Type Px, uint16_t GPIO_Pin, uint16_t Polarity )
{
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00, loop = 16;
    P_Port_Def *px_exti;

    assert_param(IS_PAD_TYPE(Px));

    if      (Px == PAD_PA)        px_exti  = EXTI_PA;
    else if (Px == PAD_PB)        px_exti  = EXTI_PB;
    else if (Px == PAD_PC)        px_exti  = EXTI_PC;
    else
    {        
        px_exti  = (P_Port_Def*)EXTI_PD;
        loop = 5;
    }

    for(pinpos = 0x00; pinpos < loop; pinpos++)
    {
        pos = ((uint32_t)0x01) << pinpos;

        currentpin = GPIO_Pin & pos;
        if(currentpin == pos)
        {   
            if(Polarity == EXTI_Trigger_Rising)
                px_exti->Port[pinpos] |= EXTI_Px_INTPOR_RISING_EDGE;
            else
                px_exti->Port[pinpos] |= EXTI_Px_INTPOR_FALLING_EDGE;
        }
    }

}

/**
  * @brief  Fills each EXTI_InitStruct member with its reset value.
  * @param  EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = 0xFF;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
}

uint16_t EXTI_Px_GetEXTEN(PAD_Type Px)
{
    uint32_t i, loop = 16;
	  uint16_t ret=0;
    P_Port_Def *px_exti;

    assert_param(IS_PAD_TYPE(Px));

    if      (Px == PAD_PA)        px_exti  = EXTI_PA;
    else if (Px == PAD_PB)        px_exti  = EXTI_PB;
    else if (Px == PAD_PC)        px_exti  = EXTI_PC;
    else
    {        
        px_exti  = (P_Port_Def*)EXTI_PD;
        loop = 5;
    }

    for(i = 0x00; i < loop; i++)
    {
             ret |= (((px_exti->Port[i]&0x2)>>1)<<i);
		}
		return ret;
}

uint16_t EXTI_Px_GetEXTINTPOL(PAD_Type Px)
{
    uint32_t i, loop = 16;
	uint16_t ret=0;
    P_Port_Def *px_exti;

    assert_param(IS_PAD_TYPE(Px));

    if      (Px == PAD_PA)        px_exti  = EXTI_PA;
    else if (Px == PAD_PB)        px_exti  = EXTI_PB;
    else if (Px == PAD_PC)        px_exti  = EXTI_PC;
    else
    {        
        px_exti  = (P_Port_Def*)EXTI_PD;
        loop = 5;
    }

    for(i = 0x00; i < loop; i++)
    {
             ret |= ((px_exti->Port[i]&0x1)<<i);
		}
		return ret;
}
