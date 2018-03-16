/*
 * Copyright (c) 2016-2018 ARM Limited
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
 * \file arm_gpio_drv.h
 * \brief Generic driver for ARM GPIO.
 */

#ifndef __ARM_GPIO_DRV_H__
#define __ARM_GPIO_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_PORT_MASK         0xFFFF  /* Default port mask */

/* GPIO flags */
#define ARM_GPIO_PIN_DISABLE      (1 << 0)
#define ARM_GPIO_PIN_ENABLE       (1 << 1)
#define ARM_GPIO_OUTPUT           (1 << 2)
#define ARM_GPIO_INPUT            (1 << 3)
#define ARM_GPIO_IRQ              (1 << 4)
#define ARM_GPIO_IRQ_EDGE         (1 << 5)
#define ARM_GPIO_IRQ_LEVEL        (1 << 6)
#define ARM_GPIO_IRQ_ACTIVE_LOW   (1 << 7)
#define ARM_GPIO_IRQ_ACTIVE_HIGH  (1 << 8)

/* ARM GPIO enumeration types */
enum arm_gpio_access_t {
    ARM_GPIO_ACCESS_PIN = 0,  /*!< Pin access to GPIO */
    ARM_GPIO_ACCESS_PORT      /*!< Port access to GPIO */
};

enum arm_gpio_irq_status_t {
    ARM_GPIO_IRQ_DISABLE = 0,  /*!< Disable interruptions */
    ARM_GPIO_IRQ_ENABLE        /*!< Enable interruptions */
};

enum arm_gpio_error_t {
    ARM_GPIO_ERR_NONE = 0,      /*!< No error */
    ARM_GPIO_ERR_INVALID_ARG,   /*!< Error invalid input argument */
    ARM_GPIO_ERR_PORT_NOT_INIT  /*!< Error GPIO port not initialized */
};

/* ARM GPIO device configuration structure */
struct arm_gpio_dev_cfg_t {
    const uint32_t base;  /*!< GPIO base address */
};

/* ARM GPIO device data structure */
struct arm_gpio_dev_data_t {
    uint32_t state;      /*!< Indicates if the gpio driver
                              is initialized and enabled */
    uint32_t port_mask;  /*!< Port mask used for any port access */
};

/* ARM GPIO device structure */
struct arm_gpio_dev_t {
    const struct arm_gpio_dev_cfg_t* const cfg;  /*!< GPIO configuration */
    struct arm_gpio_dev_data_t* const data;      /*!< GPIO data */
};

/* ARM GPIO pin structure */
struct arm_gpio_pin_t {
    uint32_t                number;       /*!< Pin number */
    enum arm_gpio_access_t  access_type;  /*!< Type of access in the
                                               GPIO block */
};

/**
 * \brief Initializes GPIO port.
 *
 * \param[in] dev  GPIO port to initalize \ref arm_gpio_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_gpio_init(struct arm_gpio_dev_t* dev);

/**
 * \brief Configurates pin or port.
 *
 * \param[in] dev      GPIO port to initalize \ref arm_gpio_dev_t
 * \param[in] access   Access type \ref arm_gpio_access_t
 * \param[in] pin_num  Pin number.
 * \param[in] flags    Pin flags \ref arm_gpio_flags_t
 *
 * \return Returns error code as specified in \ref arm_gpio_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_gpio_error_t arm_gpio_config(struct arm_gpio_dev_t* dev,
                                      enum arm_gpio_access_t access,
                                      uint8_t pin_num,
                                      uint32_t flags);

/**
 * \brief  Writes to output pin or port.
 *
 * \param[in] dev      GPIO port to initalize \ref arm_gpio_dev_t
 * \param[in] access   Access type \ref arm_gpio_access_t
 * \param[in] pin_num  Pin number.
 * \param[in] value    Value(s) to set.
 *
 * \return Returns error code as specified in \ref arm_gpio_error_t
 *
 * \note This function doesn't check if dev is NULL.
 * \note As ARM is a read-modify-write architecture, before writing a
 *       value on a GPIO pin it is required to disable the
 *       interrupts to prevent problems in a multitasking
 *       environment.
 */
enum arm_gpio_error_t arm_gpio_write(struct arm_gpio_dev_t* dev,
                                     enum arm_gpio_access_t access,
                                     uint8_t pin_num,
                                     uint32_t value);

/**
 * \brief Reads the pin or port status.
 *
 * \param[in] dev      GPIO port to initalize \ref arm_gpio_dev_t
 * \param[in] access   Access type \ref arm_gpio_access_t
 * \param[in] pin_num  Pin number.
 * \param[in] value    Value of input pin(s).
 *
 * \return Returns bit value for Pin access or port value for port access.
 *  Negative value for error.
 * \note This function doesn't check if dev is NULL.
 */
int32_t arm_gpio_read(struct arm_gpio_dev_t* dev, enum arm_gpio_access_t access,
                      uint8_t pin_num);

/**
 * \brief Sets interrupt status for the given pin or port.
 *
 * \param[in] dev      GPIO port to initalize \ref arm_gpio_dev_t
 * \param[in] access   Access type \ref arm_gpio_access_t
 * \param[in] pin_num  Pin number.
 * \param[in] status   Interrupt status \ref arm_gpio_irq_status
 *
 * \return Returns error code as specified in \ref arm_gpio_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_gpio_error_t arm_gpio_set_interrupt(struct arm_gpio_dev_t* dev,
                                             enum arm_gpio_access_t access,
                                             uint8_t pin_num,
                                             enum arm_gpio_irq_status_t status);

/**
 * \brief Gets interrupt status for the given pin or port.
 *
 * \param[in]  dev      GPIO port to initalize \ref arm_gpio_dev_t
 * \param[in]  access   Access type \ref arm_gpio_access_t
 * \param[in]  pin_num  Pin number.
 * \param[out] status   Interrupt status values. If the access is by pin, then
 *                      the status will be 0 or 1.
 *
 * \return Returns error code as specified in \ref arm_gpio_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_gpio_error_t arm_gpio_get_irq_status(struct arm_gpio_dev_t* dev,
                                              enum arm_gpio_access_t access,
                                              uint8_t pin_num,
                                              uint32_t* status);

/**
 * \brief Clears gpio interrupt.
 *
 * \param[in] dev      GPIO port to initalize \ref arm_gpio_dev_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns error code as specified in \ref arm_gpio_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_gpio_error_t arm_gpio_clear_interrupt(struct arm_gpio_dev_t* dev,
                                               uint8_t pin_num);

/**
 * \brief Sets gpio mask for port access.
 *
 * \param[in] dev        GPIO port \ref arm_gpio_dev_t
 * \param[in] port_mask  New port mask to set, only the 16 LSb are taken into
 *                       account
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_gpio_set_port_mask(struct arm_gpio_dev_t* dev, uint32_t port_mask);

/**
 * \brief Gets gpio mask for port access.
 *
 * \param[in] dev  GPIO port \ref arm_gpio_dev_t
 *
 * \return Returns the current port mask
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_gpio_get_port_mask(struct arm_gpio_dev_t* dev);

#ifdef __cplusplus
}
#endif
#endif /* __ARM_GPIO_DRV_H__ */
