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
 * \file gpio_cmsdk_drv.h
 * \brief Generic driver for ARM GPIO.
 */

#ifndef __GPIO_CMSDK_DRV_H__
#define __GPIO_CMSDK_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_CMSDK_MAX_PIN_NUM      16U
#define GPIO_CMSDK_MAX_PORT_MASK    ((1U << GPIO_CMSDK_MAX_PIN_NUM) - 1U)

/* GPIO enumeration types */
enum gpio_cmsdk_direction_t {
    GPIO_CMSDK_INPUT = 0,      /*!< GPIO is input */
    GPIO_CMSDK_OUTPUT          /*!< GPIO is output */
};

enum gpio_cmsdk_altfunc_t {
    GPIO_CMSDK_MAIN_FUNC = 0,  /*!< Alternate function is not enabled */
    GPIO_CMSDK_ALT_FUNC        /*!< Alternate function is enabled */
};

enum gpio_cmsdk_irq_status_t {
    GPIO_CMSDK_IRQ_DISABLE = 0,  /*!< Disable interruptions */
    GPIO_CMSDK_IRQ_ENABLE        /*!< Enable interruptions */
};

enum gpio_cmsdk_irq_type_t {
    GPIO_CMSDK_IRQ_LEVEL = 0,  /*!< Level Interrupt */
    GPIO_CMSDK_IRQ_EDGE        /*!< Edge Interrupt */
};

enum gpio_cmsdk_irq_polarity_t {
    GPIO_CMSDK_IRQ_LOW_OR_FALLING_EDGE = 0,  /*!< Interrupt active low or
                                                  falling edge */
    GPIO_CMSDK_IRQ_HIGH_OR_RISING_EDGE       /*!< Interrupt active high or
                                                  rising edge */
};

enum gpio_cmsdk_error_t {
    GPIO_CMSDK_ERR_NONE = 0,      /*!< No error */
    GPIO_CMSDK_ERR_INVALID_ARG,   /*!< Error invalid input argument */
    GPIO_CMSDK_ALTFUNC_EERROR,    /*!< Alternate function returned error */
};

/* CMSDK GPIO device configuration structure */
struct gpio_cmsdk_dev_cfg_t {
    const uint32_t base;     /*!< GPIO base address */
};

/* CMSDK GPIO device structure */
struct gpio_cmsdk_dev_t {
    const struct gpio_cmsdk_dev_cfg_t* const cfg;  /*!< GPIO configuration */
};

/**
 * \brief Initializes GPIO port.
 *
 * \param[in] dev  GPIO device to initalize \ref gpio_cmsdk_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void gpio_cmsdk_init(struct gpio_cmsdk_dev_t* dev);

/**
 * \brief Configures pin.
 *
 * \param[in] dev             GPIO device to configure \ref gpio_cmsdk_dev_t
 * \param[in] pin_num         Pin number for pin access
 * \param[in] direction       Input or output \ref gpio_cmsdk_direction_t
 * \param[in] altfunc_flags   Alternate function \ref gpio_cmsdk_altfunc_t
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_flags_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_pin_config(struct gpio_cmsdk_dev_t* dev, uint32_t pin_num,
                    enum gpio_cmsdk_direction_t direction,
                    enum gpio_cmsdk_altfunc_t altfunc_flags);

/**
 * \brief Configures port.
 *
 * \param[in] dev             GPIO device to configure \ref gpio_cmsdk_dev_t
 * \param[in] pin_mask        Bitmask of the selected pins
 * \param[in] direction       Input or output \ref gpio_cmsdk_direction_t
 * \param[in] altfunc_flags   Alternate function \ref gpio_cmsdk_altfunc_t
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_port_config(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                     enum gpio_cmsdk_direction_t direction,
                     enum gpio_cmsdk_altfunc_t altfunc_flags);


/**
 * \brief Configures interrupt type
 *
 * \param[in] dev         GPIO device to initalize \ref gpio_cmsdk_dev_t
 * \param[in] pin_mask    Bitmask of the selected pins
 * \param[in] irq_type    Interrupt type \ref gpio_cmsdk_irq_type_t
 * \param[in] irq_pol     Interrupt polarity \ref gpio_cmsdk_irq_polarity_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void gpio_cmsdk_config_irq(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                         enum gpio_cmsdk_irq_type_t irq_type,
                         enum gpio_cmsdk_irq_polarity_t irq_pol);

/**
 * \brief  Sets state of the output pin.
 *
 * \param[in] dev        GPIO device to use for the pin \ref gpio_cmsdk_dev_t
 * \param[in] pin_num    Pin number for pin access
 * \param[in] value      Value(s) to set.
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 * \note GPIO data output register is a read-modify-write register,
 *       so before writing a value on a GPIO pin it is required to disable
 *       the interrupts to prevent concurrency problems.
 */
enum gpio_cmsdk_error_t gpio_cmsdk_pin_write(struct gpio_cmsdk_dev_t* dev,
                                         uint32_t pin_num,
                                         uint32_t value);

/**
 * \brief  Sets state of the output port.
 *
 * \param[in] dev        GPIO device to use for the pins \ref gpio_cmsdk_dev_t
 * \param[in] pin_mask   Bitmask of the selected pins
 * \param[in] value      Bitmask of pins states to set
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 * \note GPIO data output register is a read-modify-write register,
 *       so before writing a value on a GPIO pin it is required to disable
 *       the interrupts to prevent concurrency problems.
 */
enum gpio_cmsdk_error_t gpio_cmsdk_port_write(struct gpio_cmsdk_dev_t* dev,
                                          uint32_t pin_mask,
                                          uint32_t value);

/**
 * \brief Reads the pin status.
 *
 * \param[in]  dev        GPIO device to use for the pin \ref gpio_cmsdk_dev_t
 * \param[in]  pin_num    Pin number for pin access
 * \param[out] data       Bit value read from the IO pin
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_pin_read(struct gpio_cmsdk_dev_t* dev, uint32_t pin_num, uint32_t *data);

/**
 * \brief Reads the port status.
 *
 * \param[in]  dev        GPIO device to use for the pins \ref gpio_cmsdk_dev_t
 * \param[in]  pin_mask   Bitmask of the selected pins
 * \param[out] data       Bit values for the mask read from the IO pin
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_port_read(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                   uint32_t *data);

/**
 * \brief Enables/disables interrupt for the given pin.
 *
 * \param[in] dev        GPIO device to initalize \ref gpio_cmsdk_dev_t
 * \param[in] pin_num    Pin number to configure
 * \param[in] status     Interrupt status \ref gpio_cmsdk_irq_status
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_set_pin_irq_cfg(struct gpio_cmsdk_dev_t* dev, uint32_t pin_num,
                         enum gpio_cmsdk_irq_status_t status);

/**
 * \brief Enables/disables interrupt for the given pins.
 *
 * \param[in] dev        GPIO device to use for the pins \ref gpio_cmsdk_dev_t
 * \param[in] pin_mask   Bitmask of the pins to configure
 * \param[in] status     Interrupt status \ref gpio_cmsdk_irq_status
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_set_port_irq_cfg(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                          enum gpio_cmsdk_irq_status_t status);

/**
 * \brief Get interrupt status for the given pin.
 *
 * \param[in]  dev       GPIO device to use for the pin \ref gpio_cmsdk_dev_t
 * \param[in]  pin_num   Pin number for the access
 * \param[out] status    Interrupt status values. If the access is by pin, then
 *                       the status will be 0 or 1.
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_get_pin_irq_status(struct gpio_cmsdk_dev_t* dev,
                            uint32_t pin_num, uint32_t* status);

/**
 * \brief Get interrupt status for the given port.
 *
 * \param[in]  dev        GPIO device to use for the pins \ref gpio_cmsdk_dev_t
 * \param[in]  pin_mask   Bitmask of the pins to configure
 * \param[out] status     Interrupt status values. If the access is by pin,
 *                        then the status will be 0 or 1.
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t
gpio_cmsdk_get_port_irq_status(struct gpio_cmsdk_dev_t* dev,
                             uint32_t pin_mask, uint32_t* status);

/**
 * \brief Clears gpio interrupt.
 *
 * \param[in] dev      GPIO device to initalize \ref gpio_cmsdk_dev_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns error code as specified in \ref gpio_cmsdk_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum gpio_cmsdk_error_t gpio_cmsdk_clear_irq(struct gpio_cmsdk_dev_t* dev,
                                         uint8_t pin_num);

#ifdef __cplusplus
}
#endif
#endif /* __GPIO_CMSDK_DRV_H__ */
