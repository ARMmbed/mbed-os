/*
 * Copyright (c) 2018 ARM Limited
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
 * \file arm_mps2_io_drv.h
 * \brief Generic driver for ARM MPS2 IO.
 */

#ifndef __ARM_MPS2_IO_DRV_H__
#define __ARM_MPS2_IO_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ARM MPS2 IO enumeration types */
enum arm_mps2_io_access_t {
    ARM_MPS2_IO_ACCESS_PIN = 0,  /*!< Pin access to MPS2 IO */
    ARM_MPS2_IO_ACCESS_PORT      /*!< Port access to MPS2 IO */
};

enum arm_mps2_io_type_t {
    ARM_MPS2_IO_TYPE_SCC = 0,  /*!< Use the SCC IO device */
    ARM_MPS2_IO_TYPE_FPGAIO    /*!< Use the FPGA IO device */
};

/* ARM MPS2 IO device configuration structure */
struct arm_mps2_io_dev_cfg_t {
    const uint32_t base;                 /*!< MPS2 IO base address */
    const enum arm_mps2_io_type_t type;  /*!< SCC or FPGAIO */
};

/* ARM MPS2 IO device structure */
struct arm_mps2_io_dev_t {
    const struct arm_mps2_io_dev_cfg_t* const cfg; /*!< MPS2 IO configuration */
};

/**
 * \brief  Writes to output LEDs.
 *
 * \param[in] dev      MPS2 IO device where to write \ref arm_mps2_io_dev_t
 * \param[in] access   Access type \ref arm_mps2_io_access_t
 * \param[in] pin_num  Pin number.
 * \param[in] value    Value(s) to set.
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_mps2_io_write_leds(struct arm_mps2_io_dev_t* dev,
                            enum arm_mps2_io_access_t access,
                            uint8_t pin_num,
                            uint32_t value);

/**
 * \brief  Writes corresponding pin in FPGA IO MISC register.
 *
 * \param[in] dev      MPS2 IO device where to write \ref arm_mps2_io_dev_t
 * \param[in] pin_num  Pin number.
 * \param[in] value    Value to set.
 *
 * \note This function doesn't check if dev is NULL.
 * \note This function doesn't support port access.
 */
void arm_mps2_io_write_misc(struct arm_mps2_io_dev_t* dev,
                            enum arm_mps2_io_access_t access,
                            uint8_t pin_num,
                            uint32_t value);

/**
 * \brief Reads the buttons status.
 *
 * \param[in] dev      MPS2 IO device where to read \ref arm_mps2_io_dev_t
 * \param[in] access   Access type \ref arm_mps2_io_access_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns bit value for Pin access or port value for port access.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps2_io_read_buttons(struct arm_mps2_io_dev_t* dev,
                                  enum arm_mps2_io_access_t access,
                                  uint8_t pin_num);

/**
 * \brief Reads the LED status.
 *
 * \param[in] dev      MPS2 IO device where to read \ref arm_mps2_io_dev_t
 * \param[in] access   Access type \ref arm_mps2_io_access_t
 * \param[in] pin_num  Pin number.
 *
 * \return Returns bit value for Pin access or port value for port access.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_mps2_io_read_leds(struct arm_mps2_io_dev_t* dev,
                               enum arm_mps2_io_access_t access,
                               uint8_t pin_num);

#ifdef __cplusplus
}
#endif

#endif /* __ARM_MPS2_IO_DRV_H__ */
