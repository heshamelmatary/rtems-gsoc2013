/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 * 
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <bsp/mm_config_table.h>
#include <bsp/linker-symbols.h>

void bsp_memory_management_Initialize(void);

BSP_START_TEXT_SECTION void bsp_memory_management_Initialize(void)
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
