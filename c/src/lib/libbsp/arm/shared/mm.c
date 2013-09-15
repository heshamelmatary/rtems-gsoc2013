/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <bsp/mm.h>
#include <libcpu/arm-cp15.h>
#include <rtems/score/mm.h>

#define LIBMM_REGION_IS_CACHE_ENABLED(attr) \
  (((attr) >> RTEMS_MM_REGION_BIT_CACHE) & 1U)
#define LIBMM_REGION_IS_PROTECTION_WRITE_ENABLED(attr) \
  (((attr) >> RTEMS_MM_REGION_BIT_WRITE) & 1U)
#define LIBMM_REGION_IS_DEVICE_TYPE(attr) \
  (((attr) >> RTEMS_MM_REGION_BIT_DEVICE) & 1U)
#define LIBMM_REGION_IS_SHARED_MEMORY(attr) \
  (((attr) >> RTEMS_MM_REGION_BIT_SHARED) & 1U)

#define ARM_MMU_BIND_SECTION_TO_CLIENT_DOMAIN \
  (ARM_MMU_DEFAULT_CLIENT_DOMAIN << ARM_MMU_SECT_DOMAIN_SHIFT)

#define TRANSLATE_ATTRIBUTES_TO_CPU(attr) \
  ((ARM_MMU_BIND_SECTION_TO_CLIENT_DOMAIN) \
  | (ARM_MMU_SECT_AP_0) \
  | (LIBMM_REGION_IS_PROTECTION_WRITE_ENABLED(attr) ? \
  0U : (ARM_MMU_SECT_AP_2)) \
  | (LIBMM_REGION_IS_CACHE_ENABLED(attr) ? \
  (ARM_MMU_SECT_TEX_0|ARM_MMU_SECT_C|ARM_MMU_SECT_B) : 0U) \
  | (LIBMM_REGION_IS_DEVICE_TYPE(attr) ? ARM_MMU_SECT_B : 0U) \
  | (LIBMM_REGION_IS_SHARED_MEMORY(attr) ? ARM_MMU_SECT_S : 0U) \
  | (ARM_MMU_SECT_DEFAULT))

void bsp_memory_management_set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
  uintptr_t end = base + size;
  uint32_t section_flags = TRANSLATE_ATTRIBUTES_TO_CPU(attr);

  arm_cp15_set_translation_table_entries(base, end, section_flags);
}
