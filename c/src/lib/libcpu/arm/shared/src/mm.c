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
#include <libcpu/arm_cp15_print_fsr.h>
#include <bsp/mm_config_table.h>

void _CPU_Memory_management_Initialize(void)
{	
  uint32_t ctrl = 0;
  uint32_t client_domain = 15;
  uint32_t *ttb;
  uint32_t config_count = RTEMS_ARRAY_SIZE(_cpu_mmu_config_table);
  ctrl = arm_cp15_get_control();

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
  ctrl,
  (uint32_t *) bsp_translation_table_base,
  ARM_MMU_DEFAULT_CLIENT_DOMAIN,
  &_cpu_mmu_config_table[0],
  RTEMS_ARRAY_SIZE(&_cpu_mmu_config_table)
  );

  arm_cp15_set_exception_handler(ARM_EXCEPTION_DATA_ABORT,
    dummy_data_abort_exception_handler
  );
}

void _CPU_Memory_management_Set_attributes(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
  uint32_t end = (uint32_t)base + (uint32_t)size;
  uint32_t section_flags;
  uint32_t cl_size = arm_cp15_get_min_cache_line_size();
  uint32_t i = ARM_MMU_SECT_GET_INDEX(base);
  uint32_t iend = ARM_MMU_SECT_GET_INDEX(ARM_MMU_SECT_MVA_ALIGN_UP(end));
  uint32_t ctrl;
  uint32_t cpsr = 0;
  uint32_t *ttb = arm_cp15_get_translation_table_base();
  uint32_t *table = (uint32_t *) bsp_section_vector_begin;
  uint32_t *vend = (uint32_t *) bsp_section_vector_end;

  section_flags = translation_table[attr];
  arm_cp15_set_translation_table_entries(base, end, section_flags);
}

void dummy_data_abort_exception_handler(void)
{
  uint32_t cpsr;
#if DEBUG
  printf("Entered exception handler \n");

  __asm__ volatile (
  ARM_SWITCH_TO_ARM
  "mrs %[cpsr], cpsr\n"
  ARM_SWITCH_BACK
  : [cpsr] "=&r" (cpsr)
  );

  printf("CPSR after enable MMU is 0x%x \n",cpsr);

  uint32_t address_fault = (uint32_t)arm_cp15_get_fault_address();
  printk("Data fault address at  0x%x\n",address_fault);

  uint32_t fsr = (uint32_t) arm_cp15_get_data_fault_status();
  arm_cp15_print_fault_status_description(fsr);
#endif

  //TODO: Call rtems_fatal
  exit(0);
}

