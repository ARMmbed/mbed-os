/*
 * Copyright (c) 2017-2019 Arm Limited. All rights reserved.
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
 * \file ppc_sse200_drv.h
 * \brief Generic driver for ARM SEE 200 Peripheral Protection
 *        Controllers (PPC).
 */

#ifndef __PPC_SSE_200_DRV_H__
#define __PPC_SSE_200_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Secure Privilege Control Block aka SPCTRL */
/* Non-Secure Privilege Control Block aka NSPCTRL */

/* ARM TrustZone PPC device configuration structure */
struct ppc_sse200_dev_cfg_t {
    uint32_t const spctrl_base;  /*!< SPCTRL base address */
    uint32_t const nspctrl_base; /*!< NSPCTRL base address */
};

/* ARM TrustZone PPC device data structure */
struct ppc_sse200_dev_data_t {
    volatile uint32_t* p_ns_ppc;  /*!< Pointer to non-secure register */
    volatile uint32_t* p_sp_ppc;  /*!< Pointer to secure unprivileged
                                   *   register
                                   */
    volatile uint32_t* p_nsp_ppc; /*!< Pointer to non-secure unprivileged
                                   *   register
                                   */
    uint32_t int_bit_mask;        /*!< Interrupt bit mask */
    uint8_t state;                /*!< Indicates if the PPC driver
                                   *  is initialized
                                   */
    uint8_t reserved[3];          /*!< 32 bits alignment */
};

/* ARM PPC device structure */
struct ppc_sse200_dev_t {
    const struct ppc_sse200_dev_cfg_t* const cfg;  /*!< PPC configuration */
    struct ppc_sse200_dev_data_t* const data;      /*!< PPC data */
};

/* Security attribute used to configure the peripheral */
enum ppc_sse200_sec_attr_t {
    PPC_SSE200_SECURE_ONLY,    /*! Secure access */
    PPC_SSE200_NONSECURE_ONLY, /*! Non-secure access */
};

/* Privilege attribute used to configure the peripheral */
enum ppc_sse200_priv_attr_t {
    PPC_SSE200_PRIV_AND_NONPRIV, /*! Privilege and non-Privilege access */
    PPC_SSE200_PRIV_ONLY,        /*! Privilege only access */
};

/* ARM PPC error codes */
enum ppc_sse200_error_t {
    PPC_SSE200_ERR_NONE = 0,  /*!< No error */
    PPC_SSE200_NOT_INIT,      /*!< PPC not initialized */
};

/* ARM PPC names */
enum ppc_sse200_name_t {
    AHB_PPC0 = 0,  /*!< AHB PPC0 */
    AHB_PPC_EXP0,  /*!< Expansion 0 AHB PPC */
    AHB_PPC_EXP1,  /*!< Expansion 1 AHB PPC */
    AHB_PPC_EXP2,  /*!< Expansion 2 AHB PPC */
    AHB_PPC_EXP3,  /*!< Expansion 3 AHB PPC */
    APB_PPC0,      /*!< APB PPC0 */
    APB_PPC1,      /*!< APB PPC1 */
    APB_PPC_EXP0,  /*!< Expansion 0 APB PPC */
    APB_PPC_EXP1,  /*!< Expansion 1 APB PPC */
    APB_PPC_EXP2,  /*!< Expansion 2 APB PPC */
    APB_PPC_EXP3   /*!< Expansion 3 APB PPC */
};

/**
 * \brief Initialize the PPC device.
 *
 * \param[in] dev       PPC device \ref ppc_sse200_dev_t
 * \param[in] ppc_name  PPC name \ref ppc_sse200_name_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void ppc_sse200_init(struct ppc_sse200_dev_t* dev,
                     enum ppc_sse200_name_t ppc_name);

/**
 * \brief Configures the PPC device.
 *
 * \param[in] dev        PPC device \ref ppc_sse200_dev_t
 * \param[in] periph     Peripheral position in the PPC.
 * \param[in] sec_attr   Secure attribute value.
 * \param[in] priv_attr  Privilege attribute value.
 *
 * \return Returns error code as specified in \ref ppc_sse200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum ppc_sse200_error_t ppc_sse200_config_peripheral(
                                         struct ppc_sse200_dev_t* dev,
                                         uint8_t periph,
                                         enum ppc_sse200_sec_attr_t sec_attr,
                                         enum ppc_sse200_priv_attr_t priv_attr);
/**
 * \brief Checks if the peripheral is configured as secure or non-secure.
 *
 * \param[in] dev     PPC device \ref ppc_sse200_dev_t
 * \param[in] periph  Peripheral position in the PPC.
 *
 * \return Returns 1 for secure and 0 for non-secure.
 *         If the driver is not initialized the return value is 1 (secure) as
 *         it is the default system configuration.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t ppc_sse200_is_periph_secure(struct ppc_sse200_dev_t* dev,
                                     uint8_t periph);

/**
 * \brief Checks if the peripheral is configured as Privilege only or
 *        Privilege and non-Privilege access mode.
 *
 * \param[in] dev     PPC device \ref ppc_sse200_dev_t
 * \param[in] periph  Peripheral position in the PPC.
 *
 * \return Returns 1 for Privilege only configuration and 0 for Privilege and
 *         non-Privilege access.
 *         If the driver is not initialized the return of this function is
 *         1 (Privilege only) as it is the default system configuration.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t ppc_sse200_is_periph_priv_only(struct ppc_sse200_dev_t* dev,
                                        uint8_t periph);
/**
 * \brief Enables PPC interrupt.
 *
 * \param[in] dev  PPC device \ref ppc_sse200_dev_t
 *
 * \return Returns error code as specified in \ref ppc_sse200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum ppc_sse200_error_t ppc_sse200_irq_enable(struct ppc_sse200_dev_t* dev);

/**
 * \brief Disables PPC interrupt.
 *
 * \param[in] dev  PPC device \ref ppc_sse200_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void ppc_sse200_irq_disable(struct ppc_sse200_dev_t* dev);

/**
 * \brief Clears PPC interrupt.
 *
 * \param[in] dev  PPC device \ref ppc_sse200_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void ppc_sse200_clear_irq(struct ppc_sse200_dev_t* dev);

/**
 * \brief Returns the PPC interrupt state.
 *
 * \param[in] dev  PPC device \ref ppc_sse200_dev_t
 *
 * \return Returns 1 if the interrupt is active and otherwise 0.
 *         If the driver is not initialized the return of this function is
 *         0 (not active) as it is the default system configuration.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t ppc_sse200_irq_state(struct ppc_sse200_dev_t* dev);

#ifdef __cplusplus
}
#endif

#endif /* __PPC_SSE_200_DRV_H__ */
