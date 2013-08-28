/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <bsp/arm-cp15-start.h>
#include <libcpu/arm-cp15.h>
#include <libcpu/mm.h>
#include <bsp/start.h>
#include <bsp/linker-symbols.h>
#include <bsp/mm_config_table.h>

BSP_START_TEXT_SECTION void _CPU_Memory_management_Initialize(void)
{	
  uint32_t ctrl = arm_cp15_get_control();

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
  ctrl,
  (uint32_t *) bsp_translation_table_base,
  ARM_MMU_DEFAULT_CLIENT_DOMAIN,
  &_cpu_mmu_config_table[0],
  RTEMS_ARRAY_SIZE(&_cpu_mmu_config_table)
  );
}

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
