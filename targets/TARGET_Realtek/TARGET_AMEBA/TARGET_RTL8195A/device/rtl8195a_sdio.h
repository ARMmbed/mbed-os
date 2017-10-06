/*
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#ifndef MBED_RTL8195A_SDIO_H
#define MBED_RTL8195A_SDIO_H

__BUILD_CCTRL_MACRO(SDIOD, REG_PESOC_HCI_CLK_CTRL0)
__BUILD_CCTRL_MACRO(SDIOH, REG_PESOC_HCI_CLK_CTRL0)

#define __SDIOD_Enable()                                            \
  do {                                                              \
    __RTK_PERI_SETBIT(REG_SOC_HCI_COM_FUNC_EN, BIT_SOC_HCI_SDIOD_ON_EN);  \
    __RTK_PERI_SETBIT(REG_SOC_HCI_COM_FUNC_EN, BIT_SOC_HCI_SDIOD_OFF_EN); \
  } while (0)

#define __SDIOH_Enable()                                            \
  do {                                                              \
    __RTK_PERI_SETBIT(REG_SOC_HCI_COM_FUNC_EN, BIT_SOC_HCI_SDIOH_EN);  \
  } while (0)

#define __SDIOD_Disable()                                           \
  do {                                                              \
    __RTK_READ32(SDIO_DEVICE_REG_BASE, 0);                                    \
    __RTK_PERI_CLRBIT(REG_SOC_HCI_COM_FUNC_EN, BIT_SOC_HCI_SDIOD_ON_EN);  \
    __RTK_PERI_CLRBIT(REG_SOC_HCI_COM_FUNC_EN, BIT_SOC_HCI_SDIOD_OFF_EN); \
  } while (0)

#define __SDIOH_Disable()                                           \
  do {                                                              \
    __RTK_READ32(SDIO_HOST_REG_BASE, 0);                                    \
    __RTK_PERI_CLRBIT(REG_SOC_HCI_COM_FUNC_EN, BIT_SOC_HCI_SDIOH_EN);  \
  } while (0)

// PERI_MCTRL_HCI
#define __SDIOD_PINMUX_Enable()  __RTK_PERI_SETBIT(REG_HCI_PINMUX_CTRL, BIT_HCI_SDIOD_PIN_EN)
#define __SDIOH_PINMUX_Enable()  __RTK_PERI_SETBIT(REG_HCI_PINMUX_CTRL, BIT_HCI_SDIOH_PIN_EN)
#define __SDIOD_PINMUX_Disable() __RTK_PERI_CLRBIT(REG_HCI_PINMUX_CTRL, BIT_HCI_SDIOD_PIN_EN)
#define __SDIOH_PINMUX_Disable() __RTK_PERI_CLRBIT(REG_HCI_PINMUX_CTRL, BIT_HCI_SDIOH_PIN_EN)
#define __MII_PINMUX_Enable()    __RTK_PERI_SETBIT(REG_HCI_PINMUX_CTRL, BIT_HCI_MII_PIN_EN)
#define __MII_PINMUX_Disable()   __RTK_PERI_CLRBIT(REG_HCI_PINMUX_CTRL, BIT_HCI_MII_PIN_EN)

// Interface for HAL functions
extern void SDIO_HST_Disable(void);
extern void SDIO_DEV_Disable(void);

#endif
