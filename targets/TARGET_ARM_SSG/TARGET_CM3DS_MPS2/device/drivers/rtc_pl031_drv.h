/*
 * Copyright (c) 2018 Arm Limited
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

/**
 * \file rtc_pl031_drv.h
 * \brief Declarations for the PL031 Real Time Clock (RTC) native driver.
 */

#ifndef __RTC_PL031_DRV_H__
#define __RTC_PL031_DRV_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief RTC PL031 device configuration structure.
 */
struct rtc_pl031_dev_cfg_t {
    const uintptr_t base;             /*!< RTC PL031 base address */
};

/**
 * \brief RTC PL031 device structure.
 */
struct rtc_pl031_dev_t {
    const struct rtc_pl031_dev_cfg_t* const cfg;
            /*!< RTC driver configuration */
};

/**
 * \brief Initializes the RTC PL031 device.
 *
 * \param[in] dev      RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_init(struct rtc_pl031_dev_t* dev);

/**
 * \brief Enables RTC PL031 device.
 *
 * \param[in] dev      RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_dev_enable(struct rtc_pl031_dev_t* dev);

/**
 * \brief Disables RTC PL031 device.
 *
 * \param[in] dev      RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_dev_disable(struct rtc_pl031_dev_t* dev);

/**
 * \brief Reads current time from RTC PL031 device.
 *
 * \param[in] dev        RTC device structure \ref rtc_pl031_dev_t.
 * \param[out] seconds   Current time in seconds.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_read_current_time(struct rtc_pl031_dev_t* dev,
                                 uint32_t *seconds);

/**
 * \brief Writes current time to RTC PL031 device.
 *
 * \param[in] dev        RTC device structure \ref rtc_pl031_dev_t.
 * \param[in] seconds    Current time to be set in seconds.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_write_current_time(struct rtc_pl031_dev_t* dev,
                                  uint32_t seconds);

/**
 * \brief Clears interrupt mask of RTC PL031.
 *
 * \param[in] dev        RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_enable_interrupt(struct rtc_pl031_dev_t* dev);

/**
 * \brief Sets interrupt mask of RTC PL031.
 *
 * \param[in] dev        RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_disable_interrupt(struct rtc_pl031_dev_t* dev);

/**
 * \brief Check if RTC PL031 interrupt is masked.
 *
 * \param[in] dev              RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that RTC PL031 interrupt is masked.
 *
 * \note    This function does not check if dev is NULL.
 */
bool rtc_pl031_is_interrupt_masked(struct rtc_pl031_dev_t* dev);

/**
 * \brief Gets raw interrupt pending status of RTC PL031.
 *
 * \param[in] dev              RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that RTC PL031 raw interrupt
 *          status is pending.
 *
 * \note    This function does not check if dev is NULL.
 */
bool rtc_pl031_is_raw_interrupt_pending(struct rtc_pl031_dev_t* dev);

/**
 * \brief Gets masked interrupt pending status of RTC PL031.
 *
 * \param[in] dev              RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that RTC PL031 masked interrupt
 *          status is pending.
 *
 * \note    This function does not check if dev is NULL.
 */
bool rtc_pl031_is_masked_interrupt_pending(struct rtc_pl031_dev_t* dev);

/**
 * \brief Writes match value to RTC PL031 device.
 *
 * \param[in] dev        RTC device structure \ref rtc_pl031_dev_t.
 * \param[in] seconds    Match value to be set in seconds.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_write_match_value(struct rtc_pl031_dev_t* dev, uint32_t seconds);

/**
 * \brief Clear interrupt status bit of RTC PL031.
 *
 * \param[in] dev        RTC device structure \ref rtc_pl031_dev_t.
 *
 * \return  Return true indicates that the function executed successfully,
 *          otherwise an error occurred.
 */
bool rtc_pl031_clear_interrupt(struct rtc_pl031_dev_t* dev);

#ifdef __cplusplus
}
#endif
#endif /* __RTC_PL031_DRV_H__ */
