/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <libcpu/mm.h>
#include <libcpu/arm-cp15.h>

#define translate_attributes(attr) \
  ((ARM_MMU_DEFAULT_CLIENT_DOMAIN << ARM_MMU_SECT_DOMAIN_SHIFT) \
  | (ARM_MMU_SECT_AP_0) \
  | ((((attr) >> RTEMS_MM_REGION_BIT_WRITE) & 1U) ? 0U : (ARM_MMU_SECT_AP_2)) \
  | ((((attr) >> RTEMS_MM_REGION_BIT_CACHE) & 1U) ? \
  (ARM_MMU_SECT_TEX_0|ARM_MMU_SECT_C|ARM_MMU_SECT_B) : 0U) \
  | ((((attr) >> RTEMS_MM_REGION_BIT_DEVICE) & 1U) ? ARM_MMU_SECT_B : 0U) \
  | ((((attr) >> RTEMS_MM_REGION_BIT_SHARED) & 1U) ? ARM_MMU_SECT_S : 0U) \
  | (ARM_MMU_SECT_DEFAULT))

void _CPU_Memory_management_Set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
  uint32_t end = (uint32_t)base + (uint32_t)size;
  uint32_t section_flags = translate_attributes(attr);

  arm_cp15_set_translation_table_entries(base, end, section_flags);
}
