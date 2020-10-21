/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "pwmout_api.h"

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "pwmout_def.h"
#include "s5js100_pinconfig.h"

/************PWM***************/
const PinMap *pwmout_pinmap()
{
	return PinMap_PWM;
}

static uint32_t pwmout_get_pulsewidth(uint32_t period, float dutycycle)
{
	uint32_t width = (uint32_t) ((period * dutycycle) / 100);
	return width;
}

static void pwmout_set_period(struct pwmout_s *priv, uint32_t period_us)
{
	uint32_t period_tick = S5JS100_USENONDS_TO_TICK(period_us);
	putreg32(period_tick, priv->pwm_name + S5JS100_PWM_TCNTB_OFFSET);
}

static void pwmout_set_pulsewidth(struct pwmout_s *priv, uint32_t width_us)
{
	uint32_t width_tick = S5JS100_USENONDS_TO_TICK(width_us);
	putreg32(width_tick, priv->pwm_name + S5JS100_PWM_TCMPB_OFFSET);
}

static void pwmout_set_start(struct pwmout_s *priv, int value)
{
	int tmp = getreg32(S5JS100_PWM_TCON);
	int offset_bit = S5JS100_PWM_TCON_OFFSET_TIMERn(priv->id) +
						S5JS100_PWM_TCON_OFFSET_START;
	uint32_t result = 0;

	if (value)
		result = tmp | (1 << offset_bit);
	else
		result = tmp & (~(1 << offset_bit));

	putreg32(result, S5JS100_PWM_TCON);
}

static void pwmout_set_manualupdate(struct pwmout_s *priv, int value)
{
	uint32_t tmp = getreg32(S5JS100_PWM_TCON);
	int offset_bit = S5JS100_PWM_TCON_OFFSET_TIMERn(priv->id) +
						S5JS100_PWM_TCON_OFFSET_UPDATE;
	uint32_t result = 0;

	if (value)
		result = tmp | (1 << offset_bit);
	else
		result = tmp & (~(1 << offset_bit));

	putreg32(result, S5JS100_PWM_TCON);
}

static void pwmout_set_inverter(struct pwmout_s *priv, int value)
{
	uint32_t tmp = getreg32(S5JS100_PWM_TCON);
	int offset_bit = S5JS100_PWM_TCON_OFFSET_TIMERn(priv->id) +
						S5JS100_PWM_TCON_OFFSET_INVERTER;
	uint32_t result = 0;

	if (value)
		result = tmp | (1 << offset_bit);
	else
		result = tmp & (~(1 << offset_bit));

	putreg32(result, S5JS100_PWM_TCON);
}

static void pwmout_set_autoload(struct pwmout_s *priv, int value)
{
	uint32_t tmp = getreg32(S5JS100_PWM_TCON);
	int offset_bit = S5JS100_PWM_TCON_OFFSET_TIMERn(priv->id) +
						S5JS100_PWM_TCON_OFFSET_AUTOLOAD;
	uint32_t result = 0;

	if (value)
		result = tmp | (1 << offset_bit);
	else
		result = tmp & (~(1 << offset_bit));

	putreg32(result, S5JS100_PWM_TCON);
}

static void pwmout_timer_config(struct pwmout_s *priv, uint32_t period_us, uint32_t width_us)
{
	if (width_us >= 0)
		pwmout_set_pulsewidth(priv, width_us);
	if (period_us >= 0)
		pwmout_set_period(priv, period_us);

	pwmout_set_manualupdate(priv, S5JS100_PWM_MANULUPDATE_ENABLE);
	pwmout_set_autoload(priv, S5JS100_PWM_AUTORELOAD_ENABLE);
	pwmout_set_manualupdate(priv, S5JS100_PWM_MANULUPDATE_DISABLE);
}

void pwmout_init_direct(pwmout_t *obj, const PinMap *pinmap)
{
	// Get the peripheral name from the pin and assign it to the object
	struct pwmout_s *priv = (struct pwmout_s *)obj;

	if (!obj || !pinmap)
		return;

	PinName pin = (PinName)pinmap->pin;
	int pwm = (int)pinmap->peripheral;
	int function = (int)pinmap->function;
	int id = 0;
	uint32_t pin_addr = get_tmux_addr(pin);

	if (getreg32(pin_addr) != 0x7100)
	{
		putreg32(0x7100, pin_addr);
	}

	if (pinmap->function)
		s5js100_configgpio(pinmap->function);

	switch (pin) {
		case PWM_TOUT0:
			id = 0;
			break;
		case PWM_TOUT1:
			id = 1;
			break;
		case PWM_TOUT2:
			id = 2;
			break;
		case PWM_TOUT3:
			id = 3;
			break;
		default:
			break;
	}

	priv->id = id;
	priv->pwm_name = pwm;
	priv->period_us = S5JS100_PWM_DEFAULT_PERIOD;
	priv->width_us = S5JS100_PWM_DEFAULT_PULSEWIDTH;

	pwmout_timer_config(priv, priv->period_us, priv->width_us);
	pwmout_set_start(priv, S5JS100_PWM_TIMER_START);
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;

	PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
	int function = (int)pinmap_find_function(pin, PinMap_PWM);

	const PinMap static_pinmap = {pin, pwm, function};

	pwmout_init_direct(obj, &static_pinmap);
}

void pwmout_free(pwmout_t *obj)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;

	if (!obj)
		return;

	pwmout_set_start(priv, S5JS100_PWM_TIMER_STOP);
}

void pwmout_write(pwmout_t *obj, float value)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	float dutycycle = value * 100;

	if (!obj || value < 0)
		return;

	priv->width_us = pwmout_get_pulsewidth(priv->period_us, dutycycle);
	pwmout_timer_config(priv, priv->period_us, priv->width_us);
	// wait(1);
}

float pwmout_read(pwmout_t *obj)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	float dutycycle = 0;

	if (!obj || priv->period_us <= 0)
		return 0;

	dutycycle = ((float)priv->width_us / priv->period_us);
	return dutycycle;
}

void pwmout_period(pwmout_t *obj, float seconds)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	float dutycycle = 0;

	if (!priv)
		return;

	dutycycle = ((float)priv->width_us / (float)priv->period_us) * 100;
	priv->period_us = (uint32_t)(seconds * S5JS100_SENDS_TO_USENONDS);
	priv->width_us = pwmout_get_pulsewidth(priv->period_us, dutycycle);

	pwmout_timer_config(priv, priv->period_us, priv->width_us);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	float dutycycle = 0;

	if (!priv)
		return;

	dutycycle = ((float)priv->width_us / (float)priv->period_us) * 100;
	priv->period_us = (uint32_t)(ms * S5JS100_MSENDS_TO_USENONDS);
	priv->width_us = pwmout_get_pulsewidth(priv->period_us, dutycycle);

	pwmout_timer_config(priv, priv->period_us, priv->width_us);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	float dutycycle = 0;

	if (!priv)
		return;

	dutycycle = ((float)priv->width_us / (float)priv->period_us) * 100;
	priv->period_us = (uint32_t)us;
	priv->width_us = pwmout_get_pulsewidth(priv->period_us, dutycycle);

	pwmout_timer_config(priv, priv->period_us, priv->width_us);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;

	if (!priv)
		return;

	priv->width_us = (uint32_t)(seconds * S5JS100_SENDS_TO_USENONDS);
	pwmout_timer_config(priv, priv->period_us, priv->width_us);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;

	if (!priv)
		return;

	priv->width_us = (uint32_t)(ms * S5JS100_MSENDS_TO_USENONDS);
	pwmout_timer_config(priv, priv->period_us, priv->width_us);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;

	if (!priv)
		return;

	priv->width_us = (uint32_t)us;
	pwmout_timer_config(priv, priv->period_us, priv->width_us);
}

int pwmout_read_period_us(pwmout_t *obj)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	return priv->period_us;
}

int pwmout_read_pulsewidth_us(pwmout_t *obj)
{
	struct pwmout_s *priv = (struct pwmout_s *)obj;
	return priv->width_us;
}

#endif
