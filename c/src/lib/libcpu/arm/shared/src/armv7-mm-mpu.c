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

_CPU_Memory_management_Install_entry()
{

}

_CPU_Memory_management_Uninstall_entry()
{
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBCPU_ARM_MPU_LIBMM */
