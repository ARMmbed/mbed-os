/*
 * Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform_devices.h"
#include "SMM_MPS2.h"

/* ARM CMSDK Timer driver structures */
#ifdef ARM_CMSDK_TIMER0
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER0_DEV_CFG = {
    .base = CMSDK_TIMER0_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER0_DEV_DATA = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV = {&(CMSDK_TIMER0_DEV_CFG),
                                           &(CMSDK_TIMER0_DEV_DATA)};
#endif

#ifdef ARM_CMSDK_TIMER1
static const struct timer_cmsdk_dev_cfg_t CMSDK_TIMER1_DEV_CFG = {
    .base = CMSDK_TIMER1_BASE};
static struct timer_cmsdk_dev_data_t CMSDK_TIMER1_DEV_DATA = {
    .is_initialized = 0};
struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV = {&(CMSDK_TIMER1_DEV_CFG),
                                           &(CMSDK_TIMER1_DEV_DATA)};
#endif

/* ARM GPIO driver structures */
#ifdef ARM_GPIO0
static const struct arm_gpio_dev_cfg_t ARM_GPIO0_DEV_CFG = {
  .base = CMSDK_GPIO0_BASE};
static struct arm_gpio_dev_data_t ARM_GPIO0_DEV_DATA = {
    .state = 0,
    .port_mask = DEFAULT_PORT_MASK};
struct arm_gpio_dev_t ARM_GPIO0_DEV = {&(ARM_GPIO0_DEV_CFG),
                                       &(ARM_GPIO0_DEV_DATA)};
#endif /* ARM_GPIO0 */

#ifdef ARM_GPIO1
static const struct arm_gpio_dev_cfg_t ARM_GPIO1_DEV_CFG = {
  .base = CMSDK_GPIO1_BASE};
static struct arm_gpio_dev_data_t ARM_GPIO1_DEV_DATA = {
    .state = 0,
    .port_mask = DEFAULT_PORT_MASK};
struct arm_gpio_dev_t ARM_GPIO1_DEV = {&(ARM_GPIO1_DEV_CFG),
                                       &(ARM_GPIO1_DEV_DATA)};
#endif /* ARM_GPIO1 */

#ifdef ARM_GPIO2
static const struct arm_gpio_dev_cfg_t ARM_GPIO2_DEV_CFG = {
  .base = CMSDK_GPIO2_BASE};
static struct arm_gpio_dev_data_t ARM_GPIO2_DEV_DATA = {
    .state = 0,
    .port_mask = DEFAULT_PORT_MASK};
struct arm_gpio_dev_t ARM_GPIO2_DEV = {&(ARM_GPIO2_DEV_CFG),
                                       &(ARM_GPIO2_DEV_DATA)};
#endif /* ARM_GPIO2 */

#ifdef ARM_GPIO3
static const struct arm_gpio_dev_cfg_t ARM_GPIO3_DEV_CFG = {
  .base = CMSDK_GPIO3_BASE};
static struct arm_gpio_dev_data_t ARM_GPIO3_DEV_DATA = {
    .state = 0,
    .port_mask = DEFAULT_PORT_MASK};
struct arm_gpio_dev_t ARM_GPIO3_DEV = {&(ARM_GPIO3_DEV_CFG),
                                       &(ARM_GPIO3_DEV_DATA)};
#endif /* ARM_GPIO3 */

/* ARM MPS2 IO FPGAIO driver structures */
#ifdef ARM_MPS2_IO_FPGAIO
static const struct arm_mps2_io_dev_cfg_t ARM_MPS2_IO_FPGAIO_DEV_CFG = {
  .base = MPS2_FPGAIO_BASE,
  .type = ARM_MPS2_IO_TYPE_FPGAIO};
struct arm_mps2_io_dev_t ARM_MPS2_IO_FPGAIO_DEV =
                                                {&(ARM_MPS2_IO_FPGAIO_DEV_CFG)};
#endif /* ARM_MPS2_IO_FPGAIO */

/* ARM MPS2 IO SCC driver structures */
#ifdef ARM_MPS2_IO_SCC
static const struct arm_mps2_io_dev_cfg_t ARM_MPS2_IO_SCC_DEV_CFG = {
  /*
   * MPS2 IO SCC and FPGAIO registers have similar structure
   * with 4 byte offset addresses.
   */
  .base = MPS2_SCC_BASE + 4,
  .type = ARM_MPS2_IO_TYPE_SCC};
struct arm_mps2_io_dev_t ARM_MPS2_IO_SCC_DEV = {&(ARM_MPS2_IO_SCC_DEV_CFG)};
#endif /* ARM_MPS2_IO_SCC */
