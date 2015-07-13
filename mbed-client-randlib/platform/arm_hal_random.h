/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */

#ifndef ARM_HAL_RANDOM_H_
#define ARM_HAL_RANDOM_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \brief This function performs Random number driver init.
 */
extern void arm_random_module_init(void);
/**
 * \brief Get random library seed value.
 */
extern uint32_t arm_random_seed_get(void);
#ifdef __cplusplus
}
#endif
#endif /* ARM_HAL_RANDOM_H_ */
