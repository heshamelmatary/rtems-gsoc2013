/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 * 
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */
#include <bsp/arm-cp15-start.h>
#include <bsp/linker-symbols.h>
#include <bsp/mm.h>
#include <bsp/start.h>

extern const arm_cp15_start_section_config bsp_mm_config_table[];
extern const size_t bsp_mm_config_table_size;

BSP_START_TEXT_SECTION void bsp_memory_management_initialize(void)
{
  uint32_t ctrl = arm_cp15_get_control();

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
    ctrl,
    (uint32_t *) bsp_translation_table_base,
    ARM_MMU_DEFAULT_CLIENT_DOMAIN,
    &bsp_mm_config_table[0],
    bsp_mm_config_table_size
  );
}
