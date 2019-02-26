/*
 * Copyright (c) 2017-2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef __MUSCA_A1_SCC_DRV_H__
#define __MUSCA_A1_SCC_DRV_H__

#include <stdint.h>
#include <stddef.h>

/**
 * \brief Enum to store alternate function values.
 *        They are used as shift operand, must be unsigned.
 */
enum gpio_altfunc_t {
    GPIO_MAIN_FUNC = 0UL,
    GPIO_ALTFUNC_1,
    GPIO_ALTFUNC_2,
    GPIO_ALTFUNC_3,
    GPIO_ALTFUNC_MAX
};

#define GPIO_ALTFUNC_ALL_MASK ((1U << GPIO_ALTFUNC_MAX) - 1)

/**
* \brief Enum to store alternate function mask values.
*/
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

enum pinmode_select_t {
    PINMODE_NONE,
    PINMODE_PULL_DOWN,
    PINMODE_PULL_UP
};

/* MUSCA SCC device configuration structure */
struct musca_a1_scc_dev_cfg_t {
    const uint32_t base;  /*!< SCC base address */
};

/* MUSCA SCC device structure */
struct musca_a1_scc_dev_t {
    const struct musca_a1_scc_dev_cfg_t* const cfg;  /*!< SCC configuration */
};

/**
 * \brief Sets selected alternate functions for selected pins
 *
 * \param[in] dev        SCC device pointer \ref musca_a1_scc_dev_t
 * \param[in] altfunc    Alternate function to set \ref gpio_altfunc_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 *
 * \note This function doesn't check if scc dev is NULL.
 * \note If no alternate function is selected then the function won't do anything
 */
void musca_a1_scc_set_alt_func(struct musca_a1_scc_dev_t* dev,
                          enum gpio_altfunc_t altfunc, uint32_t pin_mask);

/**
 * \brief Sets pinmode for the given pins
 *
 * \param[in] dev        SCC device pointer \ref musca_a1_scc_dev_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 * \param[in] mode       Pin mode to set \ref pinmode_select_t
 *
 * \note This function doesn't check if scc dev is NULL.
 */
void musca_a1_scc_set_pinmode(struct musca_a1_scc_dev_t* dev, uint32_t pin_mask,
                         enum pinmode_select_t mode);

/**
 * \brief Sets default input values for the selected pins
 *
 * \param[in] dev               SCC device pointer \ref musca_a1_scc_dev_t
 * \param[in] altfunc           The selected alternate function that is set the
 *                              specified default in value \ref gpio_altfunc_t
 * \param[in] default_in_mask   Pin mask for selecting pins
 * \param[in] default_in_value  Pin values for the selected pins
 *
 * \note This function doesn't check if scc_base is NULL.
 * \note If no alternate function is selected, the function won't do anything
 */
void musca_a1_scc_set_default_in(struct musca_a1_scc_dev_t* dev,
                            enum gpio_altfunc_t altfunc,
                            uint32_t default_in_mask,
                            uint32_t default_in_value);

#endif /* __MUSCA_A1_SCC_DRV_H__ */
