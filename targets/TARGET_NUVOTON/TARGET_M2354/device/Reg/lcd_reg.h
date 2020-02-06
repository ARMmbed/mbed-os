/**************************************************************************//**
 * @file     lcd_reg.h
 * @version  V1.00
 * @brief    LCD register definition header file
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __LCD_REG_H__
#define __LCD_REG_H__


/** @addtogroup REGISTER Control Register

  @{

*/


/*---------------------- Liquid-Crystal Display -------------------------*/
/**
    @addtogroup LCD Liquid-Crystal Display(LCD)
    Memory Mapped Structure for LCD Controller
@{ */
 
typedef struct
{


/**
 * @var LCD_T::CFG
 * Offset: 0x00  LCD Configuration Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[1:0]   |BIAS      |Bias Voltage Level Selection
 * |        |          |This field is used to select the bias voltage level of LCD controller.
 * |        |          |0 = Reserved.
 * |        |          |1 = 1/2 bias.
 * |        |          |2 = 1/3 bias.
 * |        |          |3 = 1/4 Bias.
 * |[3]     |TYPE      |LCD Waveform Type Selection
 * |        |          |This field is used to select the driving waveform type of LCD controller.
 * |        |          |0 = Type A.
 * |        |          |1 = Type B.
 * |[6:4]   |DUTY      |COM Duty Ratio Selection
 * |        |          |This field is used to select the LCD operating duty ratio.
 * |        |          |0 = 1/1 duty.
 * |        |          |1 = 1/2 duty.
 * |        |          |2 = 1/3 duty.
 * |        |          |3 = 1/4 duty.
 * |        |          |4 = 1/5 duty.
 * |        |          |5 = 1/6 duty.
 * |        |          |6 = 1/7 duty.
 * |        |          |7 = 1/8 duty.
 * |        |          |Note: When 1/3 duty is selected, only COM0 to COM2 are for LCD driving; if 1/8 duty is selected, COM0 to COM7 are for LCD driving.
 * |[17:8]  |FREQDIV   |LCD Operating Frequency Divider
 * |        |          |The FREQDIV of LCD controller is used to divide the LCD_CLK to generate LCD operating frequency.
 * |        |          |LCD operating frequency = LCD_CLK / ((FRRQDIV + 1) * 2)).
 * |        |          |Note 1: The FREQDIV can be selected from 0, 1, 2u2026 to 1023, therefore the fastest operating frequency is (LCD_CLK / 2) Hz and lowest operating frequency is (LCD_CLK / 2048) Hz.
 * |        |          |For example:
 * |        |          |If LCD_CLK is 32 kHz,
 * |        |          |the fastest LCD operating frequency is 32 kHz / ((0+1) * 2) = 16 kHz;.
 * |        |          |the lowest LCD operating frequency is 32 kHz / ((1023+1) * 2) = 16 Hz.
 * |        |          |Note 2: LCD frame rate can be calculated by (LCD operating frequency) * (COM duty ratio).
 * |        |          |For example:
 * |        |          |If LCD operating frequency is 16 kHz, COM duty ratio is 1/4,
 * |        |          |then the LCD frame rate is (16 kHz * (1/4)) = 4 kHz.
 * @var LCD_T::CTL
 * Offset: 0x04  LCD Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |EN        |LCD Display Enable Bit
 * |        |          |0 = LCD display function Disabled.
 * |        |          |1 = LCD display function Enabled.
 * |        |          |Note: User should read SYNC (LCD_CTL[31]) bit to check if to enable/disable LCD display is completed or not.
 * |[1]     |BLINK     |LCD Blinking Enable Bit
 * |        |          |0 = LCD blinking function Disabled.
 * |        |          |1 = LCD blinking function Enabled.
 * |        |          |Note 1: Blinking frequency is determined by the frame counting event.
 * |        |          |Note 2: LCD blinking function can work normal even if MCU is in Power-down mode.
 * |[17:8]  |FCV       |Frame Counting Value
 * |        |          |This field indicates the frame counting value to genetate the frame counting event.
 * |        |          |The target frame counting event period is (FCV + 1) frames.
 * |        |          |Note: The FCV is rom 0, 1, 2u2026 to 1023, thus the frame counting event period can be selected from 1, 2, 3u2026 to 1024 frames.
 * |[31]    |SYNC      |LCD Enable Control SYNC Flag Indicator (Read Only)
 * |        |          |If user executes enable/disable EN (LCD_CTL[0]), this flag can be indicated enable/disable EN function is completed or not.
 * |        |          |0 = Set EN bit 0 or 1 is completed.
 * |        |          |1 = Set EN bit is synchronizing and not become active yet.
 * |        |          |Note: Set LCD EN bit 1 to trun on the LCD display needs 2 * LCD_CLK period to become active
 * |        |          |Set LCD EN bit 0 to turn off the LCD display, the LCD display cannot be turned off immediately until the end of frame is reached
 * |        |          |Therefore, user should read SYNC bit to check if setting LCD EN bit is completed or not.
 * @var LCD_T::SDCTL0
 * Offset: 0x08  LCD Segment Display Control Register 0
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL1
 * Offset: 0x0C  LCD Segment Display Control Register 1
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL2
 * Offset: 0x10  LCD Segment Display Control Register 2
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL3
 * Offset: 0x14  LCD Segment Display Control Register 3
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL4
 * Offset: 0x18  LCD Segment Display Control Register 4
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL5
 * Offset: 0x1C  LCD Segment Display Control Register 5
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL6
 * Offset: 0x20  LCD Segment Display Control Register 6
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL7
 * Offset: 0x24  LCD Segment Display Control Register 7
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL8
 * Offset: 0x28  LCD Segment Display Control Register 8
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL9
 * Offset: 0x2C  LCD Segment Display Control Register 9
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::SDCTL10
 * Offset: 0x30  LCD Segment Display Control Register 10
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |DATA0     |Segment Display on Data Region 0
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 0), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG0 display data; x = 1 means to set SEG4 display data.
 * |        |          |Note 2: DATA0[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[15:8]  |DATA1     |Segment Display on Data Region 1
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 1), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG1 display data; x = 1 means to set SEG5 display data.
 * |        |          |Note 2: DATA1[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[23:16] |DATA2     |Segment Display on Data Region 2
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 2), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG2 display data; x = 1 means to set SEG6 display data.
 * |        |          |Note 2: DATA2[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * |[31:24] |DATA3     |Segment Display on Data Region 3
 * |        |          |This field is used to set specify segment display u201CSelectedu201D or u201CDeselectedu201D on COM0 to COM7.
 * |        |          |0 = Correspond segment display is Deselected.
 * |        |          |1 = Correspond segment display is Selected.
 * |        |          |Note 1: The selected segment number is ((4 * x) + 3), x = 0 ~ 10.
 * |        |          |For example, if x = 0 means to set the SEG3 display data; x = 1 means to set SEG7 display data.
 * |        |          |Note 2: DATA3[n] indicates the SEGx display on COMn, n = 0 ~ 7.
 * |        |          |For example, if n = 0 is used to set SEGx u201CDeselectedu201D or u201DSelectedu201D on COM0; if n = 7 is used to set SEGx u201CDeselectedu201D or u201CSelectedu201D on COM7.
 * @var LCD_T::INTEN
 * Offset: 0x40  LCD Interrupt Enable Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |FCIEN     |Frame Counting Interrupt Enable Bit
 * |        |          |This bit is used to enable frame counting interrupt.
 * |        |          |0 = Frame Counting Interrupt Disable.
 * |        |          |1 = Frame Counting Interrupt Enable.
 * |[1]     |FEIEN     |Frame End Interrupt Enable Bit
 * |        |          |This bit is used to enable frame end interrupt.
 * |        |          |0 = Frame End Interrupt Disable.
 * |        |          |1 = Frame End Interrupt Enable.
 * @var LCD_T::INTSTS
 * Offset: 0x44  LCD Interrupt Status Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |FCIF      |Frame Counting Interrupt Flag
 * |        |          |This flag indicates that current frame counter value has reached to FCV (LCD_CTL[17:8] Frame Counting Value).
 * |        |          |0 = Frame counting event did not occur.
 * |        |          |1 = Frame counting event occurred.
 * |        |          |Note: This bit can be cleared by writing 1 to it.
 * |[1]     |FEIF      |Frame End Interrupt Flag
 * |        |          |This flag indicates that current frame display has completed.
 * |        |          |0 = Frame end event did not occur.
 * |        |          |1 = Frame end event occurred.
 * |        |          |Note: This bit can be cleared by writing 1 to it.
 */
    __IO uint32_t CFG;                   /*!< [0x0000] LCD Configuration Register                                       */
    __IO uint32_t CTL;                   /*!< [0x0004] LCD Control Register                                             */
    __IO uint32_t SDCTL[11];              /*!< [0x0008] ~ [0x0030] LCD Segment Display Control Register 0 ~ 11           */
    __I  uint32_t RESERVE0[3];
    __IO uint32_t INTEN;                 /*!< [0x0040] LCD Interrupt Enable Control Register                            */
    __IO uint32_t INTSTS;                /*!< [0x0044] LCD Interrupt Status Register                                    */

} LCD_T;

/**
    @addtogroup LCD_CONST LCD Bit Field Definition
    Constant Definitions for LCD Controller
@{ */

#define LCD_CFG_BIAS_Pos                 (0)                                               /*!< LCD_T::CFG: BIAS Position              */
#define LCD_CFG_BIAS_Msk                 (0x3ul << LCD_CFG_BIAS_Pos)                       /*!< LCD_T::CFG: BIAS Mask                  */

#define LCD_CFG_TYPE_Pos                 (3)                                               /*!< LCD_T::CFG: TYPE Position              */
#define LCD_CFG_TYPE_Msk                 (0x1ul << LCD_CFG_TYPE_Pos)                       /*!< LCD_T::CFG: TYPE Mask                  */

#define LCD_CFG_DUTY_Pos                 (4)                                               /*!< LCD_T::CFG: DUTY Position              */
#define LCD_CFG_DUTY_Msk                 (0x7ul << LCD_CFG_DUTY_Pos)                       /*!< LCD_T::CFG: DUTY Mask                  */

#define LCD_CFG_FREQDIV_Pos              (8)                                               /*!< LCD_T::CFG: FREQDIV Position           */
#define LCD_CFG_FREQDIV_Msk              (0x3fful << LCD_CFG_FREQDIV_Pos)                  /*!< LCD_T::CFG: FREQDIV Mask               */

#define LCD_CTL_EN_Pos                   (0)                                               /*!< LCD_T::CTL: EN Position                */
#define LCD_CTL_EN_Msk                   (0x1ul << LCD_CTL_EN_Pos)                         /*!< LCD_T::CTL: EN Mask                    */

#define LCD_CTL_BLINK_Pos                (1)                                               /*!< LCD_T::CTL: BLINK Position             */
#define LCD_CTL_BLINK_Msk                (0x1ul << LCD_CTL_BLINK_Pos)                      /*!< LCD_T::CTL: BLINK Mask                 */

#define LCD_CTL_WVREV_Pos                (2)                                               /*!< LCD_T::CTL: WVREV Position             */
#define LCD_CTL_WVREV_Msk                (0x1ul << LCD_CTL_WVREV_Pos)                      /*!< LCD_T::CTL: WVREV Mask                 */

#define LCD_CTL_FCV_Pos                  (8)                                               /*!< LCD_T::CTL: FCV Position               */
#define LCD_CTL_FCV_Msk                  (0x3fful << LCD_CTL_FCV_Pos)                      /*!< LCD_T::CTL: FCV Mask                   */

#define LCD_CTL_SYNC_Pos                 (31)                                              /*!< LCD_T::CTL: SYNC Position              */
#define LCD_CTL_SYNC_Msk                 (0x1ul << LCD_CTL_SYNC_Pos)                       /*!< LCD_T::CTL: SYNC Mask                  */

#define LCD_SDCTL0_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL0: DATA0 Position          */
#define LCD_SDCTL0_DATA0_Msk             (0xfful << LCD_SDCTL0_DATA0_Pos)                  /*!< LCD_T::SDCTL0: DATA0 Mask              */

#define LCD_SDCTL0_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL0: DATA1 Position          */
#define LCD_SDCTL0_DATA1_Msk             (0xfful << LCD_SDCTL0_DATA1_Pos)                  /*!< LCD_T::SDCTL0: DATA1 Mask              */

#define LCD_SDCTL0_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL0: DATA2 Position          */
#define LCD_SDCTL0_DATA2_Msk             (0xfful << LCD_SDCTL0_DATA2_Pos)                  /*!< LCD_T::SDCTL0: DATA2 Mask              */

#define LCD_SDCTL0_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL0: DATA3 Position          */
#define LCD_SDCTL0_DATA3_Msk             (0xfful << LCD_SDCTL0_DATA3_Pos)                  /*!< LCD_T::SDCTL0: DATA3 Mask              */

#define LCD_SDCTL1_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL1: DATA0 Position          */
#define LCD_SDCTL1_DATA0_Msk             (0xfful << LCD_SDCTL1_DATA0_Pos)                  /*!< LCD_T::SDCTL1: DATA0 Mask              */

#define LCD_SDCTL1_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL1: DATA1 Position          */
#define LCD_SDCTL1_DATA1_Msk             (0xfful << LCD_SDCTL1_DATA1_Pos)                  /*!< LCD_T::SDCTL1: DATA1 Mask              */

#define LCD_SDCTL1_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL1: DATA2 Position          */
#define LCD_SDCTL1_DATA2_Msk             (0xfful << LCD_SDCTL1_DATA2_Pos)                  /*!< LCD_T::SDCTL1: DATA2 Mask              */

#define LCD_SDCTL1_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL1: DATA3 Position          */
#define LCD_SDCTL1_DATA3_Msk             (0xfful << LCD_SDCTL1_DATA3_Pos)                  /*!< LCD_T::SDCTL1: DATA3 Mask              */

#define LCD_SDCTL2_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL2: DATA0 Position          */
#define LCD_SDCTL2_DATA0_Msk             (0xfful << LCD_SDCTL2_DATA0_Pos)                  /*!< LCD_T::SDCTL2: DATA0 Mask              */

#define LCD_SDCTL2_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL2: DATA1 Position          */
#define LCD_SDCTL2_DATA1_Msk             (0xfful << LCD_SDCTL2_DATA1_Pos)                  /*!< LCD_T::SDCTL2: DATA1 Mask              */

#define LCD_SDCTL2_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL2: DATA2 Position          */
#define LCD_SDCTL2_DATA2_Msk             (0xfful << LCD_SDCTL2_DATA2_Pos)                  /*!< LCD_T::SDCTL2: DATA2 Mask              */

#define LCD_SDCTL2_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL2: DATA3 Position          */
#define LCD_SDCTL2_DATA3_Msk             (0xfful << LCD_SDCTL2_DATA3_Pos)                  /*!< LCD_T::SDCTL2: DATA3 Mask              */

#define LCD_SDCTL3_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL3: DATA0 Position          */
#define LCD_SDCTL3_DATA0_Msk             (0xfful << LCD_SDCTL3_DATA0_Pos)                  /*!< LCD_T::SDCTL3: DATA0 Mask              */

#define LCD_SDCTL3_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL3: DATA1 Position          */
#define LCD_SDCTL3_DATA1_Msk             (0xfful << LCD_SDCTL3_DATA1_Pos)                  /*!< LCD_T::SDCTL3: DATA1 Mask              */

#define LCD_SDCTL3_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL3: DATA2 Position          */
#define LCD_SDCTL3_DATA2_Msk             (0xfful << LCD_SDCTL3_DATA2_Pos)                  /*!< LCD_T::SDCTL3: DATA2 Mask              */

#define LCD_SDCTL3_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL3: DATA3 Position          */
#define LCD_SDCTL3_DATA3_Msk             (0xfful << LCD_SDCTL3_DATA3_Pos)                  /*!< LCD_T::SDCTL3: DATA3 Mask              */

#define LCD_SDCTL4_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL4: DATA0 Position          */
#define LCD_SDCTL4_DATA0_Msk             (0xfful << LCD_SDCTL4_DATA0_Pos)                  /*!< LCD_T::SDCTL4: DATA0 Mask              */

#define LCD_SDCTL4_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL4: DATA1 Position          */
#define LCD_SDCTL4_DATA1_Msk             (0xfful << LCD_SDCTL4_DATA1_Pos)                  /*!< LCD_T::SDCTL4: DATA1 Mask              */

#define LCD_SDCTL4_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL4: DATA2 Position          */
#define LCD_SDCTL4_DATA2_Msk             (0xfful << LCD_SDCTL4_DATA2_Pos)                  /*!< LCD_T::SDCTL4: DATA2 Mask              */

#define LCD_SDCTL4_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL4: DATA3 Position          */
#define LCD_SDCTL4_DATA3_Msk             (0xfful << LCD_SDCTL4_DATA3_Pos)                  /*!< LCD_T::SDCTL4: DATA3 Mask              */

#define LCD_SDCTL5_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL5: DATA0 Position          */
#define LCD_SDCTL5_DATA0_Msk             (0xfful << LCD_SDCTL5_DATA0_Pos)                  /*!< LCD_T::SDCTL5: DATA0 Mask              */

#define LCD_SDCTL5_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL5: DATA1 Position          */
#define LCD_SDCTL5_DATA1_Msk             (0xfful << LCD_SDCTL5_DATA1_Pos)                  /*!< LCD_T::SDCTL5: DATA1 Mask              */

#define LCD_SDCTL5_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL5: DATA2 Position          */
#define LCD_SDCTL5_DATA2_Msk             (0xfful << LCD_SDCTL5_DATA2_Pos)                  /*!< LCD_T::SDCTL5: DATA2 Mask              */

#define LCD_SDCTL5_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL5: DATA3 Position          */
#define LCD_SDCTL5_DATA3_Msk             (0xfful << LCD_SDCTL5_DATA3_Pos)                  /*!< LCD_T::SDCTL5: DATA3 Mask              */

#define LCD_SDCTL6_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL6: DATA0 Position          */
#define LCD_SDCTL6_DATA0_Msk             (0xfful << LCD_SDCTL6_DATA0_Pos)                  /*!< LCD_T::SDCTL6: DATA0 Mask              */

#define LCD_SDCTL6_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL6: DATA1 Position          */
#define LCD_SDCTL6_DATA1_Msk             (0xfful << LCD_SDCTL6_DATA1_Pos)                  /*!< LCD_T::SDCTL6: DATA1 Mask              */

#define LCD_SDCTL6_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL6: DATA2 Position          */
#define LCD_SDCTL6_DATA2_Msk             (0xfful << LCD_SDCTL6_DATA2_Pos)                  /*!< LCD_T::SDCTL6: DATA2 Mask              */

#define LCD_SDCTL6_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL6: DATA3 Position          */
#define LCD_SDCTL6_DATA3_Msk             (0xfful << LCD_SDCTL6_DATA3_Pos)                  /*!< LCD_T::SDCTL6: DATA3 Mask              */

#define LCD_SDCTL7_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL7: DATA0 Position          */
#define LCD_SDCTL7_DATA0_Msk             (0xfful << LCD_SDCTL7_DATA0_Pos)                  /*!< LCD_T::SDCTL7: DATA0 Mask              */

#define LCD_SDCTL7_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL7: DATA1 Position          */
#define LCD_SDCTL7_DATA1_Msk             (0xfful << LCD_SDCTL7_DATA1_Pos)                  /*!< LCD_T::SDCTL7: DATA1 Mask              */

#define LCD_SDCTL7_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL7: DATA2 Position          */
#define LCD_SDCTL7_DATA2_Msk             (0xfful << LCD_SDCTL7_DATA2_Pos)                  /*!< LCD_T::SDCTL7: DATA2 Mask              */

#define LCD_SDCTL7_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL7: DATA3 Position          */
#define LCD_SDCTL7_DATA3_Msk             (0xfful << LCD_SDCTL7_DATA3_Pos)                  /*!< LCD_T::SDCTL7: DATA3 Mask              */

#define LCD_SDCTL8_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL8: DATA0 Position          */
#define LCD_SDCTL8_DATA0_Msk             (0xfful << LCD_SDCTL8_DATA0_Pos)                  /*!< LCD_T::SDCTL8: DATA0 Mask              */

#define LCD_SDCTL8_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL8: DATA1 Position          */
#define LCD_SDCTL8_DATA1_Msk             (0xfful << LCD_SDCTL8_DATA1_Pos)                  /*!< LCD_T::SDCTL8: DATA1 Mask              */

#define LCD_SDCTL8_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL8: DATA2 Position          */
#define LCD_SDCTL8_DATA2_Msk             (0xfful << LCD_SDCTL8_DATA2_Pos)                  /*!< LCD_T::SDCTL8: DATA2 Mask              */

#define LCD_SDCTL8_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL8: DATA3 Position          */
#define LCD_SDCTL8_DATA3_Msk             (0xfful << LCD_SDCTL8_DATA3_Pos)                  /*!< LCD_T::SDCTL8: DATA3 Mask              */

#define LCD_SDCTL9_DATA0_Pos             (0)                                               /*!< LCD_T::SDCTL9: DATA0 Position          */
#define LCD_SDCTL9_DATA0_Msk             (0xfful << LCD_SDCTL9_DATA0_Pos)                  /*!< LCD_T::SDCTL9: DATA0 Mask              */

#define LCD_SDCTL9_DATA1_Pos             (8)                                               /*!< LCD_T::SDCTL9: DATA1 Position          */
#define LCD_SDCTL9_DATA1_Msk             (0xfful << LCD_SDCTL9_DATA1_Pos)                  /*!< LCD_T::SDCTL9: DATA1 Mask              */

#define LCD_SDCTL9_DATA2_Pos             (16)                                              /*!< LCD_T::SDCTL9: DATA2 Position          */
#define LCD_SDCTL9_DATA2_Msk             (0xfful << LCD_SDCTL9_DATA2_Pos)                  /*!< LCD_T::SDCTL9: DATA2 Mask              */

#define LCD_SDCTL9_DATA3_Pos             (24)                                              /*!< LCD_T::SDCTL9: DATA3 Position          */
#define LCD_SDCTL9_DATA3_Msk             (0xfful << LCD_SDCTL9_DATA3_Pos)                  /*!< LCD_T::SDCTL9: DATA3 Mask              */

#define LCD_SDCTL10_DATA0_Pos            (0)                                               /*!< LCD_T::SDCTL10: DATA0 Position         */
#define LCD_SDCTL10_DATA0_Msk            (0xfful << LCD_SDCTL10_DATA0_Pos)                 /*!< LCD_T::SDCTL10: DATA0 Mask             */

#define LCD_SDCTL10_DATA1_Pos            (8)                                               /*!< LCD_T::SDCTL10: DATA1 Position         */
#define LCD_SDCTL10_DATA1_Msk            (0xfful << LCD_SDCTL10_DATA1_Pos)                 /*!< LCD_T::SDCTL10: DATA1 Mask             */

#define LCD_SDCTL10_DATA2_Pos            (16)                                              /*!< LCD_T::SDCTL10: DATA2 Position         */
#define LCD_SDCTL10_DATA2_Msk            (0xfful << LCD_SDCTL10_DATA2_Pos)                 /*!< LCD_T::SDCTL10: DATA2 Mask             */

#define LCD_SDCTL10_DATA3_Pos            (24)                                              /*!< LCD_T::SDCTL10: DATA3 Position         */
#define LCD_SDCTL10_DATA3_Msk            (0xfful << LCD_SDCTL10_DATA3_Pos)                 /*!< LCD_T::SDCTL10: DATA3 Mask             */

#define LCD_INTEN_FCIEN_Pos              (0)                                               /*!< LCD_T::INTEN: FCIEN Position           */
#define LCD_INTEN_FCIEN_Msk              (0x1ul << LCD_INTEN_FCIEN_Pos)                    /*!< LCD_T::INTEN: FCIEN Mask               */

#define LCD_INTEN_FEIEN_Pos              (1)                                               /*!< LCD_T::INTEN: FEIEN Position           */
#define LCD_INTEN_FEIEN_Msk              (0x1ul << LCD_INTEN_FEIEN_Pos)                    /*!< LCD_T::INTEN: FEIEN Mask               */

#define LCD_INTSTS_FCIF_Pos              (0)                                               /*!< LCD_T::INTSTS: FCIF Position           */
#define LCD_INTSTS_FCIF_Msk              (0x1ul << LCD_INTSTS_FCIF_Pos)                    /*!< LCD_T::INTSTS: FCIF Mask               */

#define LCD_INTSTS_FEIF_Pos              (1)                                               /*!< LCD_T::INTSTS: FEIF Position           */
#define LCD_INTSTS_FEIF_Msk              (0x1ul << LCD_INTSTS_FEIF_Pos)                    /*!< LCD_T::INTSTS: FEIF Mask               */

/**@}*/ /* LCD_CONST */
/**@}*/ /* end of LCD register group */
/**@}*/ /* end of REGISTER group */


#endif /* __LCD_REG_H__ */
