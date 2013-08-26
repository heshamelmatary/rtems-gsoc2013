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
#include <bsp/generic-fatal.h>

inline void default_data_abort_exception_handler(void);

BSP_START_TEXT_SECTION void _CPU_Memory_management_Initialize(void)
{	
  uint32_t ctrl = 0;
  uint32_t *ttb;
  ctrl = arm_cp15_get_control();

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
  ctrl,
  (uint32_t *) bsp_translation_table_base,
  ARM_MMU_DEFAULT_CLIENT_DOMAIN,
  &_cpu_mmu_config_table[0],
  RTEMS_ARRAY_SIZE(&_cpu_mmu_config_table)
  );

  arm_cp15_set_exception_handler(ARM_EXCEPTION_DATA_ABORT,
    default_data_abort_exception_handler
  );
}

void _CPU_Memory_management_Set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
  uint32_t end = (uint32_t)base + (uint32_t)size;
  uint32_t cl_size = arm_cp15_get_min_cache_line_size();
  uint32_t i = ARM_MMU_SECT_GET_INDEX(base);
  uint32_t iend = ARM_MMU_SECT_GET_INDEX(ARM_MMU_SECT_MVA_ALIGN_UP(end));
  uint32_t ctrl;
  uint32_t cpsr = 0;
  uint32_t *ttb = arm_cp15_get_translation_table_base();
  uint32_t *table = (uint32_t *) bsp_section_vector_begin;
  uint32_t *vend = (uint32_t *) bsp_section_vector_end;

  arm_cp15_set_translation_table_entries(base, end, translate_attributes(attr));
}

inline void default_data_abort_exception_handler(void)
{
  bsp_generic_fatal(RTEMS_FATAL_SOURCE_BSP_GENERIC);
}
