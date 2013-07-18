/**
 *  @file
 *  
 *  @ingroup libmm
 * 
 *  @brief Wrapper for ARMv7 MPU API.
 */

/*
 *  Copyright (c) 2013 Hesham AL-Matary.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */
 
#ifndef LIBCPU_ARM_MPU_LIBMM
#define LIBCPU_ARM_MPU_LIBMM

#include <rtems/score/armv7m.h>
#include <libcpu/mm.h>
#include <bsp/start-config.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
 
#define ALIGN_REGION_START_ADDRESS(addr,size) \
  addr &= (2<<size)

static void translate_attributes(
  uint32_t high_level_attr,
  uint32_t *ARM_CPU_ATTR
)
{
#ifdef ARM_MULTILIB_ARCH_V7M
  /* Clear flags attributes */
  *ARM_CPU_ATTR = 0;

  if( high_level_attr & 0x1 )
    *ARM_CPU_ATTR |= ARMV7M_MPU_AP_PRIV_RO_USER_RO;

  /* Write access */
  if ( high_level_attr & 0x2 )
    *ARM_CPU_ATTR |= ARMV7M_MPU_AP_PRIV_RW_USER_RW;
#endif /* ARM_MULTILIB_ARCH_V7M */
}


void _CPU_Memory_management_Initialize(void)
{
#ifdef ARM_MULTILIB_ARCH_V7M
  volatile ARMV7M_MPU *mpu = _ARMV7M_MPU;
  size_t region_count = arm_start_config_mpu_region_count;
  size_t i = 0;

  for (i = 0; i < region_count; ++i) {
    mpu->rbar = arm_start_config_mpu_region [i].rbar;
    mpu->rasr = arm_start_config_mpu_region [i].rasr;
  }

  if (region_count > 0) {
    mpu->ctrl = ARMV7M_MPU_CTRL_ENABLE;
  }
#endif /* ARM_MULTILIB_ARCH_V7M */
}

_CPU_Memory_management_Install_entry(Memory_management_Entry *mme, uint32_t attr)
{
#ifdef ARM_MULTILIB_ARCH_V7M
  volatile ARMV7M_MPU *mpu = _ARMV7M_MPU;
  rtems_interrupt_level level;

  uint32_t mpu_attr;

  translate_attributes(attr, &mpu_attr);


  /* Disable MPU and interrupts */
  rtems_interrupt_disable(level);
  mpu-> = 0;

  ARMV7M_MPU_Region region =
    ARMV7M_MPU_REGION_INITIALIZER(
          0,
          mme->base,
          mme->size,
          mpu_attr
    );
  
  mpu->rbar = region.basr;
  mpu->rasr = region.rasr;

  /* Enable MPU and interrupts */
  mpu->ctrl = ARMV7M_MPU_CTRL_ENABLE;
  rtems_interrupt_enable(level);
#endif /* ARM_MULTILIB_ARCH_V7M */
}

_CPU_Memory_management_Uninstall_entry()
{
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBCPU_ARM_MPU_LIBMM */
