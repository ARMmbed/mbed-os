/*
 * Copyright (c) 2018-2019 Arm Limited. All rights reserved.
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
#ifndef __MUSCA_B1_SCC_DRV_H__
#define __MUSCA_B1_SCC_DRV_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 38 pins can be controlled in total - including the reserved ones */
#define GPIO_ALTFUNC_MAX_PINS     38U

/** Allowed error codes. */
enum musca_b1_scc_error_t {
    SCC_ERR_NONE = 0U,
    SCC_INVALID_ARG
};

/**
 * Enum to store alternate function values.
 * They are used as shift operand, must be unsigned.
 */
enum gpio_altfunc_t {
    GPIO_MAIN_FUNC = 0UL,
    GPIO_ALTFUNC_1,
    GPIO_ALTFUNC_2,
    GPIO_ALTFUNC_3,
    GPIO_ALTFUNC_MAX
};

#define GPIO_ALTFUNC_ALL_MASK ((1U << GPIO_ALTFUNC_MAX) - 1)

/** Enum to store alternate function mask values.*/
enum gpio_altfunc_mask_t {
    GPIO_ALTFUNC_NONE       = 0,
    GPIO_MAIN_FUNC_MASK     = (1UL << GPIO_MAIN_FUNC),
    GPIO_ALTFUNC_1_MASK     = (1UL << GPIO_ALTFUNC_1),
    GPIO_ALTFUNC_2_MASK     = (1UL << GPIO_ALTFUNC_2),
    GPIO_ALTFUNC_3_MASK     = (1UL << GPIO_ALTFUNC_3),
    GPIO_MAIN_FUNC_NEG_MASK = (~GPIO_MAIN_FUNC_MASK & GPIO_ALTFUNC_ALL_MASK),
    GPIO_ALTFUNC_1_NEG_MASK = (~GPIO_ALTFUNC_1_MASK & GPIO_ALTFUNC_ALL_MASK),
    GPIO_ALTFUNC_2_NEG_MASK = (~GPIO_ALTFUNC_2_MASK & GPIO_ALTFUNC_ALL_MASK),
    GPIO_ALTFUNC_3_NEG_MASK = (~GPIO_ALTFUNC_3_MASK & GPIO_ALTFUNC_ALL_MASK)
};

/** Supported pin modes */
enum pinmode_select_t {
    PINMODE_NONE = 0U,
    PINMODE_PULL_DOWN,
    PINMODE_PULL_UP,
    PINMODE_MAX
};

/** Musca SCC device configuration structure */
struct musca_b1_scc_dev_cfg_t {
    const uint32_t base;  /*!< SCC base address */
};

/** Musca SCC device structure */
struct musca_b1_scc_dev_t {
    const struct musca_b1_scc_dev_cfg_t* const cfg;  /*!< SCC configuration */
};

/**
 * \brief Sets selected alternate functions for selected pins
 *
 * \param[in] dev        SCC device pointer \ref musca_b1_scc_dev_t
 * \param[in] altfunc    Alternate function to set \ref gpio_altfunc_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 *
 * \return   Returns error code.     \ref musca_b1_scc_error_t
 *
 * \note This function doesn't check if scc dev is NULL.
 * \note If no alternate function is selected then this API won't do anything
 */
enum musca_b1_scc_error_t
musca_b1_scc_set_alt_func(struct musca_b1_scc_dev_t* dev,
                          enum gpio_altfunc_t altfunc, uint64_t pin_mask);

/**
 * \brief Sets pinmode for the given pins
 *
 * \param[in] dev        SCC device pointer \ref musca_b1_scc_dev_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 * \param[in] mode       Pin mode to set \ref pinmode_select_t
 *
 * \return   Returns error code.     \ref musca_b1_scc_error_t
 *
 * \note This function doesn't check if scc dev is NULL.
 */
enum musca_b1_scc_error_t
musca_b1_scc_set_pinmode(struct musca_b1_scc_dev_t* dev, uint64_t pin_mask,
                         enum pinmode_select_t mode);

/**
 * \brief Sets default input values for the selected pins
 *
 * \param[in] dev               SCC device pointer \ref musca_b1_scc_dev_t
 * \param[in] altfunc           The selected alternate function that is set the
 *                              specified default in value \ref gpio_altfunc_t
 * \param[in] pin_num           Pin number
 * \param[in] default_in_value  True if the in value needs to be set to 1,
 *                              false if it needs to be 0
 *
 * \return   Returns error code.     \ref musca_b1_scc_error_t
 *
 * \note This function doesn't check if scc_base is NULL.
 * \note If no alternate function is selected, the function won't do anything
 */
enum musca_b1_scc_error_t
musca_b1_scc_set_default_in(struct musca_b1_scc_dev_t* dev,
                            enum gpio_altfunc_t altfunc,
                            uint32_t pin_num,
                            bool default_in_value);
#ifdef __cplusplus
}
#endif

#endif /* __MUSCA_B1_SCC_DRV_H__ */
