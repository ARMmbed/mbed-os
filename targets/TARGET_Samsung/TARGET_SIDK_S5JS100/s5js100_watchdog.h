/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S5JS100_WATCHDOG_H
#define __S5JS100_WATCHDOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifndef CONFIG_S5JS100_WATCHDOG
void s5js100_watchdog_disable(void);

#else /* CONFIG_S5JS100_WATCHDOG */
struct s5js100_watchdog_priv_s;

void s5js100_watchdog_disable(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_enable(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_reset_disable(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_reset_enable(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_irq_disable(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_irq_enable(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_clk_set(unsigned int prescaler, unsigned int divider);
void s5js100_watchdog_set_reload_val(FAR struct s5js100_watchdog_priv_s *priv, unsigned int time_ms);
void s5js100_watchdog_set_load_val_diff(FAR struct s5js100_watchdog_priv_s *priv, unsigned int time_ms);
unsigned int s5js100_watchdog_get_curr(FAR struct s5js100_watchdog_priv_s *priv);
void s5js100_watchdog_set_curr(FAR struct s5js100_watchdog_priv_s *priv, unsigned int curr_val);
void s5js100_watchdog_clear_int(FAR struct s5js100_watchdog_priv_s *priv);
FAR struct s5js100_watchdog_priv_s *s5js100_watchdog_init(void);

#endif

#ifdef CONFIG_WATCHDOG
int s5js100_watchdog_initialize(FAR const char *devpath);
void s5js100_watchdog_autoping(void *arg);
FAR struct watchdog_lowerhalf_s *s5js100_watchdog_lowerhalf(void);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __S5JS100_WATCHDOG_H */
