/*
 * Copyright (c) 2016-2019 Arm Limited. All rights reserved.
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

#include "Driver_PPC.h"

#include "cmsis_driver_config.h"
#include "RTE_Device.h"

/* Driver version */
#define ARM_PPC_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_PPC_API_VERSION,
    ARM_PPC_DRV_VERSION
};

static ARM_DRIVER_VERSION ARM_PPC_GetVersion(void)
{
    return DriverVersion;
}

#if (RTE_AHB_PPC0)
/* AHB PPC0 Driver wrapper functions */
static int32_t AHB_PPC0_Initialize(void)
{
    ppc_sse200_init(&AHB_PPC0_DEV, AHB_PPC0);

    return ARM_DRIVER_OK;
}

static int32_t AHB_PPC0_Uninitialize(void)
{
    /* Nothing to be done*/
    return ARM_DRIVER_OK;
}

static int32_t AHB_PPC0_ConfigPeriph(uint8_t periph, ARM_PPC_SecAttr sec_attr,
                                     ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&AHB_PPC0_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);
    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t AHB_PPC0_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&AHB_PPC0_DEV, periph);
}

static uint32_t AHB_PPC0_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&AHB_PPC0_DEV, periph);
}

static int32_t AHB_PPC0_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&AHB_PPC0_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void AHB_PPC0_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&AHB_PPC0_DEV);
}

static void AHB_PPC0_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&AHB_PPC0_DEV);
}

static uint32_t AHB_PPC0_InterruptState(void)
{
    return ppc_sse200_irq_state(&AHB_PPC0_DEV);
}

/* AHB PPC0 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_AHB_PPC0;
ARM_DRIVER_PPC Driver_AHB_PPC0 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = AHB_PPC0_Initialize,
    .Uninitialize      = AHB_PPC0_Uninitialize,
    .ConfigPeriph      = AHB_PPC0_ConfigPeriph,
    .IsPeriphSecure    = AHB_PPC0_IsPeriphSecure,
    .IsPeriphPrivOnly  = AHB_PPC0_IsPeriphPrivOnly,
    .EnableInterrupt   = AHB_PPC0_EnableInterrupt,
    .DisableInterrupt  = AHB_PPC0_DisableInterrupt,
    .ClearInterrupt    = AHB_PPC0_ClearInterrupt,
    .InterruptState    = AHB_PPC0_InterruptState
};
#endif /* RTE_AHB_PPC0 */

#if (RTE_AHB_PPCEXP0)
/* AHB PPCEXP0 Driver wrapper functions */
static int32_t AHB_PPCEXP0_Initialize(void)
{
    ppc_sse200_init(&AHB_PPCEXP0_DEV, AHB_PPC_EXP0);

    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP0_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP0_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&AHB_PPCEXP0_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t AHB_PPCEXP0_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&AHB_PPCEXP0_DEV, periph);
}

static uint32_t AHB_PPCEXP0_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&AHB_PPCEXP0_DEV, periph);
}

static int32_t AHB_PPCEXP0_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&AHB_PPCEXP0_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void AHB_PPCEXP0_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&AHB_PPCEXP0_DEV);
}

static void AHB_PPCEXP0_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&AHB_PPCEXP0_DEV);
}

static uint32_t AHB_PPCEXP0_InterruptState(void)
{
    return ppc_sse200_irq_state(&AHB_PPCEXP0_DEV);
}

/* AHB PPCEXP0 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_AHB_PPCEXP0;
ARM_DRIVER_PPC Driver_AHB_PPCEXP0 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = AHB_PPCEXP0_Initialize,
    .Uninitialize      = AHB_PPCEXP0_Uninitialize,
    .ConfigPeriph      = AHB_PPCEXP0_ConfigPeriph,
    .IsPeriphSecure    = AHB_PPCEXP0_IsPeriphSecure,
    .IsPeriphPrivOnly  = AHB_PPCEXP0_IsPeriphPrivOnly,
    .EnableInterrupt   = AHB_PPCEXP0_EnableInterrupt,
    .DisableInterrupt  = AHB_PPCEXP0_DisableInterrupt,
    .ClearInterrupt    = AHB_PPCEXP0_ClearInterrupt,
    .InterruptState    = AHB_PPCEXP0_InterruptState
};
#endif /* RTE_AHB_PPCEXP0 */

#if (RTE_AHB_PPCEXP1)
/* AHB PPCEXP1 Driver wrapper functions */
static int32_t AHB_PPCEXP1_Initialize(void)
{
    ppc_sse200_init(&AHB_PPCEXP1_DEV, AHB_PPC_EXP1);

    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP1_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP1_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&AHB_PPCEXP1_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t AHB_PPCEXP1_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&AHB_PPCEXP1_DEV, periph);
}

static uint32_t AHB_PPCEXP1_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&AHB_PPCEXP1_DEV, periph);
}

static int32_t AHB_PPCEXP1_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&AHB_PPCEXP1_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void AHB_PPCEXP1_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&AHB_PPCEXP1_DEV);
}

static void AHB_PPCEXP1_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&AHB_PPCEXP1_DEV);
}

static uint32_t AHB_PPCEXP1_InterruptState(void)
{
    return ppc_sse200_irq_state(&AHB_PPCEXP1_DEV);
}

/* AHB PPCEXP1 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_AHB_PPCEXP1;
ARM_DRIVER_PPC Driver_AHB_PPCEXP1 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = AHB_PPCEXP1_Initialize,
    .Uninitialize      = AHB_PPCEXP1_Uninitialize,
    .ConfigPeriph      = AHB_PPCEXP1_ConfigPeriph,
    .IsPeriphSecure    = AHB_PPCEXP1_IsPeriphSecure,
    .IsPeriphPrivOnly  = AHB_PPCEXP1_IsPeriphPrivOnly,
    .EnableInterrupt   = AHB_PPCEXP1_EnableInterrupt,
    .DisableInterrupt  = AHB_PPCEXP1_DisableInterrupt,
    .ClearInterrupt    = AHB_PPCEXP1_ClearInterrupt,
    .InterruptState    = AHB_PPCEXP1_InterruptState
};
#endif /* RTE_AHB_PPCEXP1 */

#if (RTE_AHB_PPCEXP2)
/* AHB PPCEXP2 Driver wrapper functions */
static int32_t AHB_PPCEXP2_Initialize(void)
{
    ppc_sse200_init(&AHB_PPCEXP2_DEV, AHB_PPC_EXP2);

    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP2_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP2_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&AHB_PPCEXP2_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t AHB_PPCEXP2_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&AHB_PPCEXP2_DEV, periph);
}

static uint32_t AHB_PPCEXP2_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&AHB_PPCEXP2_DEV, periph);
}

static int32_t AHB_PPCEXP2_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&AHB_PPCEXP2_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void AHB_PPCEXP2_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&AHB_PPCEXP2_DEV);
}

static void AHB_PPCEXP2_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&AHB_PPCEXP2_DEV);
}

static uint32_t AHB_PPCEXP2_InterruptState(void)
{
    return ppc_sse200_irq_state(&AHB_PPCEXP2_DEV);
}

/* AHB PPCEXP2 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_AHB_PPCEXP2;
ARM_DRIVER_PPC Driver_AHB_PPCEXP2 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = AHB_PPCEXP2_Initialize,
    .Uninitialize      = AHB_PPCEXP2_Uninitialize,
    .ConfigPeriph      = AHB_PPCEXP2_ConfigPeriph,
    .IsPeriphSecure    = AHB_PPCEXP2_IsPeriphSecure,
    .IsPeriphPrivOnly  = AHB_PPCEXP2_IsPeriphPrivOnly,
    .EnableInterrupt   = AHB_PPCEXP2_EnableInterrupt,
    .DisableInterrupt  = AHB_PPCEXP2_DisableInterrupt,
    .ClearInterrupt    = AHB_PPCEXP2_ClearInterrupt,
    .InterruptState    = AHB_PPCEXP2_InterruptState
};
#endif /* RTE_AHB_PPCEXP2 */

#if (RTE_AHB_PPCEXP3)
/* AHB PPCEXP3 Driver wrapper functions */
static int32_t AHB_PPCEXP3_Initialize(void)
{
    ppc_sse200_init(&AHB_PPCEXP3_DEV, AHB_PPC_EXP3);

    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP3_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t AHB_PPCEXP3_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&AHB_PPCEXP3_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t AHB_PPCEXP3_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&AHB_PPCEXP3_DEV, periph);
}

static uint32_t AHB_PPCEXP3_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&AHB_PPCEXP3_DEV, periph);
}

static int32_t AHB_PPCEXP3_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&AHB_PPCEXP3_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void AHB_PPCEXP3_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&AHB_PPCEXP3_DEV);
}

static void AHB_PPCEXP3_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&AHB_PPCEXP3_DEV);
}

static uint32_t AHB_PPCEXP3_InterruptState(void)
{
    return ppc_sse200_irq_state(&AHB_PPCEXP3_DEV);
}

/* AHB PPCEXP3 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_AHB_PPCEXP3;
ARM_DRIVER_PPC Driver_AHB_PPCEXP3 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = AHB_PPCEXP3_Initialize,
    .Uninitialize      = AHB_PPCEXP3_Uninitialize,
    .ConfigPeriph      = AHB_PPCEXP3_ConfigPeriph,
    .IsPeriphSecure    = AHB_PPCEXP3_IsPeriphSecure,
    .IsPeriphPrivOnly  = AHB_PPCEXP3_IsPeriphPrivOnly,
    .EnableInterrupt   = AHB_PPCEXP3_EnableInterrupt,
    .DisableInterrupt  = AHB_PPCEXP3_DisableInterrupt,
    .ClearInterrupt    = AHB_PPCEXP3_ClearInterrupt,
    .InterruptState    = AHB_PPCEXP3_InterruptState
};
#endif /* RTE_AHB_PPCEXP3 */

#if (RTE_APB_PPC0)
/* APB PPC0 Driver wrapper functions */
static int32_t APB_PPC0_Initialize(void)
{
    ppc_sse200_init(&APB_PPC0_DEV, APB_PPC0);

    return ARM_DRIVER_OK;
}

static int32_t APB_PPC0_Uninitialize(void)
{
    /* Nothing to be done*/
    return ARM_DRIVER_OK;
}

static int32_t APB_PPC0_ConfigPeriph(uint8_t periph, ARM_PPC_SecAttr sec_attr,
                                     ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&APB_PPC0_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);
    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t APB_PPC0_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&APB_PPC0_DEV, periph);
}

static uint32_t APB_PPC0_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&APB_PPC0_DEV, periph);
}

static int32_t APB_PPC0_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&APB_PPC0_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void APB_PPC0_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&APB_PPC0_DEV);
}

static void APB_PPC0_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&APB_PPC0_DEV);
}

static uint32_t APB_PPC0_InterruptState(void)
{
    return ppc_sse200_irq_state(&APB_PPC0_DEV);
}

/* APB PPC0 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_APB_PPC0;
ARM_DRIVER_PPC Driver_APB_PPC0 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = APB_PPC0_Initialize,
    .Uninitialize      = APB_PPC0_Uninitialize,
    .ConfigPeriph      = APB_PPC0_ConfigPeriph,
    .IsPeriphSecure    = APB_PPC0_IsPeriphSecure,
    .IsPeriphPrivOnly  = APB_PPC0_IsPeriphPrivOnly,
    .EnableInterrupt   = APB_PPC0_EnableInterrupt,
    .DisableInterrupt  = APB_PPC0_DisableInterrupt,
    .ClearInterrupt    = APB_PPC0_ClearInterrupt,
    .InterruptState    = APB_PPC0_InterruptState
};
#endif /* RTE_APB_PPC0 */

#if (RTE_APB_PPC1)
/* APB PPC1 Driver wrapper functions */
static int32_t APB_PPC1_Initialize(void)
{
    ppc_sse200_init(&APB_PPC1_DEV, APB_PPC1);

    return ARM_DRIVER_OK;
}

static int32_t APB_PPC1_Uninitialize(void)
{
    /* Nothing to be done*/
    return ARM_DRIVER_OK;
}

static int32_t APB_PPC1_ConfigPeriph(uint8_t periph, ARM_PPC_SecAttr sec_attr,
                                     ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&APB_PPC1_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);
    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t APB_PPC1_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&APB_PPC1_DEV, periph);
}

static uint32_t APB_PPC1_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&APB_PPC1_DEV, periph);
}
static int32_t APB_PPC1_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&APB_PPC1_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void APB_PPC1_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&APB_PPC1_DEV);
}

static void APB_PPC1_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&APB_PPC1_DEV);
}

static uint32_t APB_PPC1_InterruptState(void)
{
    return ppc_sse200_irq_state(&APB_PPC1_DEV);
}

/* APB PPC1 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_APB_PPC1;
ARM_DRIVER_PPC Driver_APB_PPC1 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = APB_PPC1_Initialize,
    .Uninitialize      = APB_PPC1_Uninitialize,
    .ConfigPeriph      = APB_PPC1_ConfigPeriph,
    .IsPeriphSecure    = APB_PPC1_IsPeriphSecure,
    .IsPeriphPrivOnly  = APB_PPC1_IsPeriphPrivOnly,
    .EnableInterrupt   = APB_PPC1_EnableInterrupt,
    .DisableInterrupt  = APB_PPC1_DisableInterrupt,
    .ClearInterrupt    = APB_PPC1_ClearInterrupt,
    .InterruptState    = APB_PPC1_InterruptState
};
#endif /* RTE_APB_PPC1 */

#if (RTE_APB_PPCEXP0)
/* APB PPCEXP0 Driver wrapper functions */
static int32_t APB_PPCEXP0_Initialize(void)
{
    ppc_sse200_init(&APB_PPCEXP0_DEV, APB_PPC_EXP0);

    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP0_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP0_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&APB_PPCEXP0_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);
    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t APB_PPCEXP0_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&APB_PPCEXP0_DEV, periph);
}

static uint32_t APB_PPCEXP0_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&APB_PPCEXP0_DEV, periph);
}

static int32_t APB_PPCEXP0_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&APB_PPCEXP0_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void APB_PPCEXP0_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&APB_PPCEXP0_DEV);
}

static void APB_PPCEXP0_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&APB_PPCEXP0_DEV);
}

static uint32_t APB_PPCEXP0_InterruptState(void)
{
    return ppc_sse200_irq_state(&APB_PPCEXP0_DEV);
}

/* APB PPCEXP0 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_APB_PPCEXP0;
ARM_DRIVER_PPC Driver_APB_PPCEXP0 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = APB_PPCEXP0_Initialize,
    .Uninitialize      = APB_PPCEXP0_Uninitialize,
    .ConfigPeriph      = APB_PPCEXP0_ConfigPeriph,
    .IsPeriphSecure    = APB_PPCEXP0_IsPeriphSecure,
    .IsPeriphPrivOnly  = APB_PPCEXP0_IsPeriphPrivOnly,
    .EnableInterrupt   = APB_PPCEXP0_EnableInterrupt,
    .DisableInterrupt  = APB_PPCEXP0_DisableInterrupt,
    .ClearInterrupt    = APB_PPCEXP0_ClearInterrupt,
    .InterruptState    = APB_PPCEXP0_InterruptState
};
#endif /* RTE_APB_PPCEXP0 */

#if (RTE_APB_PPCEXP1)
/* APB PPCEXP1 Driver wrapper functions */
static int32_t APB_PPCEXP1_Initialize(void)
{
    ppc_sse200_init(&APB_PPCEXP1_DEV, APB_PPC_EXP1);

    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP1_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP1_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&APB_PPCEXP1_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);
    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t APB_PPCEXP1_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&APB_PPCEXP1_DEV, periph);
}

static uint32_t APB_PPCEXP1_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&APB_PPCEXP1_DEV, periph);
}

static int32_t APB_PPCEXP1_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&APB_PPCEXP1_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void APB_PPCEXP1_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&APB_PPCEXP1_DEV);
}

static void APB_PPCEXP1_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&APB_PPCEXP1_DEV);
}

static uint32_t APB_PPCEXP1_InterruptState(void)
{
    return ppc_sse200_irq_state(&APB_PPCEXP1_DEV);
}

/* APB PPCEXP1 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_APB_PPCEXP1;
ARM_DRIVER_PPC Driver_APB_PPCEXP1 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = APB_PPCEXP1_Initialize,
    .Uninitialize      = APB_PPCEXP1_Uninitialize,
    .ConfigPeriph      = APB_PPCEXP1_ConfigPeriph,
    .IsPeriphSecure    = APB_PPCEXP1_IsPeriphSecure,
    .IsPeriphPrivOnly  = APB_PPCEXP1_IsPeriphPrivOnly,
    .EnableInterrupt   = APB_PPCEXP1_EnableInterrupt,
    .DisableInterrupt  = APB_PPCEXP1_DisableInterrupt,
    .ClearInterrupt    = APB_PPCEXP1_ClearInterrupt,
    .InterruptState    = APB_PPCEXP1_InterruptState
};
#endif /* RTE_APB_PPCEXP1 */

#if (RTE_APB_PPCEXP2)
/* APB PPCEXP2 Driver wrapper functions */
static int32_t APB_PPCEXP2_Initialize(void)
{
    ppc_sse200_init(&APB_PPCEXP2_DEV, APB_PPC_EXP2);

    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP2_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP2_ConfigPeriph(uint8_t periph,
                                        ARM_PPC_SecAttr sec_attr,
                                        ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&APB_PPCEXP2_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t APB_PPCEXP2_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&APB_PPCEXP2_DEV, periph);
}

static uint32_t APB_PPCEXP2_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&APB_PPCEXP2_DEV, periph);
}

static int32_t APB_PPCEXP2_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&APB_PPCEXP2_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void APB_PPCEXP2_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&APB_PPCEXP2_DEV);
}

static void APB_PPCEXP2_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&APB_PPCEXP2_DEV);
}

static uint32_t APB_PPCEXP2_InterruptState(void)
{
    return ppc_sse200_irq_state(&APB_PPCEXP2_DEV);
}

/* APB PPCEXP2 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_APB_PPCEXP2;
ARM_DRIVER_PPC Driver_APB_PPCEXP2 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = APB_PPCEXP2_Initialize,
    .Uninitialize      = APB_PPCEXP2_Uninitialize,
    .ConfigPeriph      = APB_PPCEXP2_ConfigPeriph,
    .IsPeriphSecure    = APB_PPCEXP2_IsPeriphSecure,
    .IsPeriphPrivOnly  = APB_PPCEXP2_IsPeriphPrivOnly,
    .EnableInterrupt   = APB_PPCEXP2_EnableInterrupt,
    .DisableInterrupt  = APB_PPCEXP2_DisableInterrupt,
    .ClearInterrupt    = APB_PPCEXP2_ClearInterrupt,
    .InterruptState    = APB_PPCEXP2_InterruptState
};
#endif /* RTE_APB_PPCEXP2 */

#if (RTE_APB_PPCEXP3)
/* APB PPCEXP3 Driver wrapper functions */
static int32_t APB_PPCEXP3_Initialize(void)
{
    ppc_sse200_init(&APB_PPCEXP3_DEV, APB_PPC_EXP3);

    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP3_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t APB_PPCEXP3_ConfigPeriph(uint8_t periph, ARM_PPC_SecAttr sec_attr,
                                       ARM_PPC_PrivAttr priv_attr)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_config_peripheral(&APB_PPCEXP3_DEV, periph,
                                       (enum ppc_sse200_sec_attr_t)sec_attr,
                                       (enum ppc_sse200_priv_attr_t)priv_attr);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static uint32_t APB_PPCEXP3_IsPeriphSecure(uint8_t periph)
{
    return ppc_sse200_is_periph_secure(&APB_PPCEXP3_DEV, periph);
}

static uint32_t APB_PPCEXP3_IsPeriphPrivOnly(uint8_t periph)
{
    return ppc_sse200_is_periph_priv_only(&APB_PPCEXP3_DEV, periph);
}

static int32_t APB_PPCEXP3_EnableInterrupt(void)
{
    enum ppc_sse200_error_t ret;

    ret = ppc_sse200_irq_enable(&APB_PPCEXP3_DEV);

    if (ret != PPC_SSE200_ERR_NONE) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static void APB_PPCEXP3_DisableInterrupt(void)
{
    ppc_sse200_irq_disable(&APB_PPCEXP3_DEV);
}

static void APB_PPCEXP3_ClearInterrupt(void)
{
    ppc_sse200_clear_irq(&APB_PPCEXP3_DEV);
}

static uint32_t APB_PPCEXP3_InterruptState(void)
{
    return ppc_sse200_irq_state(&APB_PPCEXP3_DEV);
}

/* APB PPCEXP3 Driver CMSIS access structure */
extern ARM_DRIVER_PPC Driver_APB_PPCEXP3;
ARM_DRIVER_PPC Driver_APB_PPCEXP3 = {
    .GetVersion        = ARM_PPC_GetVersion,
    .Initialize        = APB_PPCEXP3_Initialize,
    .Uninitialize      = APB_PPCEXP3_Uninitialize,
    .ConfigPeriph      = APB_PPCEXP3_ConfigPeriph,
    .IsPeriphSecure    = APB_PPCEXP3_IsPeriphSecure,
    .IsPeriphPrivOnly  = APB_PPCEXP3_IsPeriphPrivOnly,
    .EnableInterrupt   = APB_PPCEXP3_EnableInterrupt,
    .DisableInterrupt  = APB_PPCEXP3_DisableInterrupt,
    .ClearInterrupt    = APB_PPCEXP3_ClearInterrupt,
    .InterruptState    = APB_PPCEXP3_InterruptState
};
#endif /* RTE_APB_PPCEXP3 */
