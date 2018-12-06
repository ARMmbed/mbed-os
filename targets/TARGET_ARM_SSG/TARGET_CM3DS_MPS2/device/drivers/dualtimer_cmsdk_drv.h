/*
 * Copyright (c) 2016-2018 Arm Limited
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
 * \file dualtimer_cmsdk_drv.h
 * \brief Generic driver for ARM CMSDK Dualtimers.
 */

#ifndef __DUALTIMER_CMSDK_DRV_H__
#define __DUALTIMER_CMSDK_DRV_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dualtimer default reload value */
#define DUALTIMER_CMSDK_DEF_32BIT_RELOAD  UINT32_MAX
#define DUALTIMER_CMSDK_DEF_16BIT_RELOAD  UINT16_MAX

/* Dualtimer device configuration structure */
struct dualtimer_cmsdk_dev_cfg_t {
    const uint32_t base;  /*!< Dualtimer base address */
};

/* Dualtimer device data structure */
struct dualtimer_cmsdk_dev_data_t {
    uint32_t is_initialized;  /*!< Indicates if the timer is initialized */
};

/* Dualtimer device structure */
struct dualtimer_cmsdk_dev_t {
    const struct dualtimer_cmsdk_dev_cfg_t* const cfg;  /*!< Dualtimer
                                                           configuration */
    struct dualtimer_cmsdk_dev_data_t* const data;      /*!< Dualtimer data */
};

/* Dualtimer enumeration types */
enum dualtimer_cmsdk_error_t {
    DUALTIMER_CMSDK_ERR_NONE = 0u,    /*!< No error */
    DUALTIMER_CMSDK_ERR_NOT_INIT,    /*!< Error dualtimer is not initialized */
    DUALTIMER_CMSDK_ERR_NOT_ENABLE,  /*!< Error timer is not enabled */
    DUALTIMER_CMSDK_ERR_OVERFLOW
};

enum dualtimer_cmsdk_size_t {
    DUALTIMER_CMSDK_SIZE_16BIT = 0u,  /*!< Timer is set as 16 Bit */
    DUALTIMER_CMSDK_SIZE_32BIT = 1u     /*!< Timer is set as 32 Bit */
};

enum dualtimer_cmsdk_prescale_t {
    DUALTIMER_CMSDK_CLOCK_DIV1 = 0u,      /*!< Timer pre-scale is clock/1  */
    DUALTIMER_CMSDK_CLOCK_DIV16 = 1u,     /*!< Timer pre-scale is clock/16 */
    DUALTIMER_CMSDK_CLOCK_DIV256 = 2u,    /*!< Timer pre-scale is clock/256 */
};

enum dualtimer_cmsdk_oneshot_t {
    DUALTIMER_CMSDK_WRAPPING = 0u, /*!< Timer  wraps around */
    DUALTIMER_CMSDK_ONE_SHOT = 1u   /*!< Timer will halt when reaching 0 */
};

enum dualtimer_cmsdk_mode_t {
    DUALTIMER_CMSDK_FREE_RUNNING = 0u, /*!< Timer counts from UINT32_MAX */
    DUALTIMER_CMSDK_PERIODIC = 1u     /*!< Timer counts from load value */
};

/**
 * \brief Initializes a dualtimer hardware.
 *
 * \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 * \note Init should be called prior to any other process.
 */
void dualtimer_cmsdk_init(const struct dualtimer_cmsdk_dev_t* dev);

/**
 * \brief Frees up a dualtimer hardware, so it could be reinitalized.
 *
 * \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 * \note Init should be called prior to any other process.
 */
void dualtimer_cmsdk_free(const struct dualtimer_cmsdk_dev_t* dev);

/**
 * \brief Enables single timer 1 included in dualtimer hardware.
 *
 * \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
 *
 * \note This function doesn't check if dev is NULL.
  * \note This function doesn't check if dualtimer has been initialized.
 */
 void dualtimer_cmsdk_enable_timer1(const struct dualtimer_cmsdk_dev_t* dev);

 /**
  * \brief Enables single timer 2 included in dualtimer hardware.
  *
  * \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
  *
  * \note This function doesn't check if dev is NULL.
  * \note This function doesn't check if dualtimer has been initialized.
  */
 void dualtimer_cmsdk_enable_timer2(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Enables both timer 1&2 included in dualtimer hardware.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_enable_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev);


/**
* \brief Disables timer 1 included in dualtimer hardware.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_disable_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Disables timer 2 included in dualtimer hardware.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_disable_timer2(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Disables both timers 1&2 included in dualtimer hardware.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_disable_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Sets one shot count for timer 1 included in dualtimer hardware.
*
* When timer reaches zero if one-shot is set it will wait there until user
* does one of the following:
* - Write a new value to load register.
* - Clear one shot setting the timers to WRAPPING mode.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] mode    enum dualtimer_cmsdk_oneshot_t WRAPPING/ONE_SHOT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_oneshot_count_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_oneshot_t mode);

/**
* \brief Sets one shot count for timer 2 included in dualtimer hardware.
*
* When timer reaches zero if one-shot is set it will wait there until user
* does one of the following:
* - Write a new value to load register.
* - Clear one shot setting the timers to WRAPPING mode.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] mode    enum dualtimer_cmsdk_oneshot_t WRAPING/ONE_SHOT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_oneshot_count_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_oneshot_t mode);

/**
* \brief Sets one shot count for timers 1&2 included in dualtimer hardware.
*
* When timer reaches zero if one-shot is set it will wait there until user
* does one of the following:
* - Write a new value to load register.
* - Clear one shot setting the timers to WRAPPING mode.
*
* \param[in] dev     Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] mode    enum dualtimer_cmsdk_oneshot_t WRAPING/ONE_SHOT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_oneshot_count_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_oneshot_t mode);

/**
* \brief Sets the size of timer 1 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] bit_size  enum dualtimer_cmsdk_oneshot_t SIZE_16BIT/SIZE_32BIT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_size_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_size_t bit_size);

/**
* \brief Sets the size of timer 2 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] bit_size  enum dualtimer_cmsdk_oneshot_t SIZE_16BIT/SIZE_32BIT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_size_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_size_t bit_size);

/**
* \brief Sets the size of timers 1&2 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] bit_size  enum dualtimer_cmsdk_oneshot_t SIZE_16BIT/SIZE_32BIT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_size_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_size_t bit_size);

/**
* \brief Gets the size of timer 1 included in dualtimer hardware.
*
* \param[in] dev    Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return           enum dualtimer_cmsdk_oneshot_t SIZE_16BIT/SIZE_32BIT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
enum dualtimer_cmsdk_size_t dualtimer_cmsdk_get_size_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Gets the size of timer 2 included in dualtimer hardware.
*
* \param[in] dev    Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return           enum dualtimer_cmsdk_oneshot_t SIZE_16BIT/SIZE_32BIT
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
enum dualtimer_cmsdk_size_t dualtimer_cmsdk_get_size_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Sets the clock divider for timer 1 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] enum dualtimer_cmsdk_prescale_t prescale CLOCK_DIV1/16/256
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_prescale_timer1(
                                const struct dualtimer_cmsdk_dev_t* dev,
                                enum dualtimer_cmsdk_prescale_t prescale);

/**
* \brief Sets the clock divider for timer 2 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] enum dualtimer_cmsdk_prescale_t prescale CLOCK_DIV1/16/256
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_prescale_timer2(
                                const struct dualtimer_cmsdk_dev_t* dev,
                                enum dualtimer_cmsdk_prescale_t prescale);

/**
* \brief Sets the clock divider for timers 1&2 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] enum dualtimer_cmsdk_prescale_t prescale CLOCK_DIV1/16/256
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_prescale_both_timers(
                                const struct dualtimer_cmsdk_dev_t* dev,
                                enum dualtimer_cmsdk_prescale_t prescale);

/**
* \brief Gets the current set prescale value for timer 1.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return enum dualtimer_cmsdk_prescale_t prescale CLOCK_DIV1/16/256
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
enum dualtimer_cmsdk_prescale_t dualtimer_cmsdk_get_prescale_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);


/**
* \brief Gets the current set prescale value for timer 2.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return enum dualtimer_cmsdk_prescale_t prescale CLOCK_DIV1/16/256
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
enum dualtimer_cmsdk_prescale_t dualtimer_cmsdk_get_prescale_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Sets the run mode for timer1 1 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] enum      dualtimer_cmsdk_mode_t mode FREE_RUNNING/PERIODIC
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_mode_timer1(const struct dualtimer_cmsdk_dev_t* dev,
                                     enum dualtimer_cmsdk_mode_t mode);

/**
* \brief Sets the run mode for timer 2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] enum      dualtimer_cmsdk_mode_t mode FREE_RUNNING/PERIODIC
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_mode_timer2(const struct dualtimer_cmsdk_dev_t* dev,
                                     enum dualtimer_cmsdk_mode_t mode);

/**
* \brief Sets the run mode for timers 1&2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] enum      dualtimer_cmsdk_mode_t mode FREE_RUNNING/PERIODIC
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_set_mode_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        enum dualtimer_cmsdk_mode_t mode);

/**
* \brief Gets the run mode for timer 1 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return enum      dualtimer_cmsdk_mode_t mode FREE_RUNNING/PERIODIC
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
enum dualtimer_cmsdk_mode_t dualtimer_cmsdk_get_mode_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Gets the run mode for timer 2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return enum      dualtimer_cmsdk_mode_t mode FREE_RUNNING/PERIODIC
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
enum dualtimer_cmsdk_mode_t dualtimer_cmsdk_get_mode_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Enables the interrupt for timer 1 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function only enables the hardware irq, use NVIC for handler setup.
*/
void dualtimer_cmsdk_enable_interrupt_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Enables the interrupt for timer 2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function only enables the hardware irq, use NVIC for handler setup.
*/
void dualtimer_cmsdk_enable_interrupt_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Enables the interrupt for timer 1&2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function only enables the hardware irq, use NVIC for handler setup.
*/
void dualtimer_cmsdk_enable_interrupt_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Disables the interrupt for timer 1 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_disable_interrupt_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Disables the interrupt for timer 2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_disable_interrupt_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Disables the interrupt for timers 1&2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_disable_interrupt_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Clears the interrupt for timer 1 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_clear_interrupt_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Clears the interrupt for timer 2 included in dualtimer hardware.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_clear_interrupt_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Set the reload value for timer 1 included in dualtimer hardware.
*
* Writing to the reload register while timer is active will reset the
* current value register to the reload value
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] reload    uint32_t value that the timer counts down from
*
* \return dualtimer_cmsdk_error_t NONE/ERR_OVERFLOW
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function will detect if users sets a uint32_t value to 16Bit timer
*/
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_reload_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload);

/**
* \brief Set the reload value for timer 2 included in dualtimer hardware.
*
* Writing to the reload register while timer is active will reset the
* current value register to the reload value
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] reload    uint32_t value that the timer counts down from
*
* \return dualtimer_cmsdk_error_t NONE/ERR_OVERFLOW
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function will detect if users sets a uint32_t value to 16Bit timer
*/
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_reload_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload);

/**
* \brief Set the reload value for timers 1&2 included in dualtimer hardware.
*
* Writing to the reload register while timer is active will reset the
* current value register to the reload value
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] reload    uint32_t value that the timer counts down from
*
* \return dualtimer_cmsdk_error_t NONE/ERR_OVERFLOW
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function will detect if users sets a uint32_t value to 16Bit timer
*/
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_reload_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload);

/**
* \brief Gets the reload value for timer 1 included in dualtimer hardware.
*
* \param[in] dev        Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return uint32_t     Reload value of timer 1
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
uint32_t dualtimer_cmsdk_get_reload_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Gets the reload value for timer 2 included in dualtimer hardware.
*
* \param[in] dev        Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return uint32_t     Reload value of timer 2
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
uint32_t dualtimer_cmsdk_get_reload_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Set the background-reload value for timer 1 of dualtimer hw.
*
* Unlike normal reload, writing to this register will not affect current
* value of timer, and the change become effective after the timer counts to zero
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] reload    uint32_t value that the timer counts down from
*
* \return dualtimer_cmsdk_error_t NONE/ERR_OVERFLOW
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function will detect if users sets a uint32_t value to 16Bit timer
*/
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_bgreload_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload);

/**
* \brief Set the background-reload value for timers 2 of dualtimer hw.
*
* Unlike normal reload, writing to this register will not affect current
* value of timer, and the change become effective after the timer counts to zero
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] reload    uint32_t value that the timer counts down from
*
* \return dualtimer_cmsdk_error_t NONE/ERR_OVERFLOW
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function will detect if users sets a uint32_t value to 16Bit timer
*/
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_bgreload_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload);

/**
* \brief Set the background-reload value for timers 1&2 of dualtimer hw.
*
* Unlike normal reload, writing to this register will not affect current
* value of timer, and the change become effective after the timer counts to zero
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
* \param[in] reload    uint32_t value that the timer counts down from
*
* \return dualtimer_cmsdk_error_t NONE/ERR_OVERFLOW
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
* \note This function will detect if users sets a uint32_t value to 16Bit timer
*/
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_bgreload_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload);

/**
* \brief Gets the background-reload value for timer 1 of dualtimer hw.
*
* \param[in] dev        Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return uint32_t     Reload value of timer 1
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
uint32_t dualtimer_cmsdk_get_bgreload_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Gets the background-reload value for timer 2 of dualtimer hw.
*
* \param[in] dev        Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return uint32_t     Reload value of timer 2
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
uint32_t dualtimer_cmsdk_get_bgreload_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Gets the current value for timer 1 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    uint32_t  Copy of current value of timer.
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
uint32_t dualtimer_cmsdk_get_current_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Gets the current value for timer 2 included in dualtimer hardware.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    uint32_t  Copy of current value of timer.
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
uint32_t dualtimer_cmsdk_get_current_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks if timer 1 included in dualtimer hardware, is enabled.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool     true if timer 1 is enabled.
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_enabled_timer1(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks if timer 2 included in dualtimer hardware, is enabled.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 2 is enabled.
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_enabled_timer2(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks if timer 1 included in dualtimer hardware, has the irq enabled.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 1 irq is enabled.
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_interrupt_enabled_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks if timer 2 included in dualtimer hardware, has the irq enabled.
*
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 2 irq is enabled.
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_interrupt_enabled_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks the masked interrupt status of timer 1 included in dualtimer hw.
*
* This function is checking against the masked interrupt register which is the
* output of timerx enabled AND timerX raw interrupt.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 1's masked irq is active
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_interrupt_active_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks the masked interrupt status of timer 1 included in dualtimer hw.
*
* This function is checking against the masked interrupt register which is the
* output of timerx enabled AND timerX raw interrupt.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 2's masked irq is active
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_interrupt_active_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks the raw interrupt status of timer 1 included in dualtimer hw.
*
* This function is checking for the raw interrupt produced when timer reaches 0
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 1's masked irq is active
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_raw_interrupt_active_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks the raw interrupt status of timer 1 included in dualtimer hw.
*
* This function is checking for the raw interrupt produced when timer reaches 0
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if timer 2's masked irq is active
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
bool dualtimer_cmsdk_is_raw_interrupt_active_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Checks if dualtimer hardware has been initialized.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return    bool      true if dualtimer has been initialized.
*
* \note This function doesn't check if dev is NULL.
*/
bool dualtimer_cmsdk_is_initialized(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Reads the number of ticks elapsed in the current cycle of timer 1.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return Get elapsed number of ticks since last reload was set.
*         Elapsed = (Reload value - Current value)
*/
uint32_t dualtimer_cmsdk_get_elapsed_value_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Reads the number of ticks elapsed in the current cycle of timer 2.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \return Get elapsed number of ticks since last reload was set.
*         Elapsed = (Reload value - Current value)
*/
uint32_t dualtimer_cmsdk_get_elapsed_value_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Resets the timer 1 count value to the reload value.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_reset_timer1(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Resets the timer 2 count value to the reload value.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_reset_timer2(const struct dualtimer_cmsdk_dev_t* dev);

/**
* \brief Resets both timers' count values to their reload values.
*
* \param[in] dev       Dualtimer device structure \ref dualtimer_cmsdk_dev_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dualtimer has been initialized.
*/
void dualtimer_cmsdk_reset_both_timers(const struct dualtimer_cmsdk_dev_t* dev);


#ifdef __cplusplus
}
#endif
#endif /* __DUALTIMER_CMSDK_DRV_H__ */
