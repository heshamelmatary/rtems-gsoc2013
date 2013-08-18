/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 * Copyright (c) 2013 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <info@embedded-brains.de>
 * 
 * Hesham AL-Matary Jul 2, 2013. 
 * This file is copied from bspstarthooks.c with minor modifications
 * to meet libmm design 
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */
#include <libcpu/arm-cp15.h>

#include <libcpu/mm.h>
#include <bsp/start.h>
#include <libcpu/arm-cp15-start.h>
#include <bsp/linker-symbols.h>
#include <rtems/score/mm.h>
#include <libcpu/arm_cp15_print_fsr.h>
//#include <rtems/score/armv7m.h>
//#include <bsp/irq.h>
//#include <rtems/score/cpu.h>
//

#ifdef __cplusplus
extern "C" {
#endif
#ifdef RTEMS_SMP
  #define MMU_DATA_READ_WRITE ARMV7_MMU_DATA_READ_WRITE_SHAREABLE
#else
  #define MMU_DATA_READ_WRITE ARMV7_MMU_DATA_READ_WRITE_CACHED
#endif

#define DEBUG 1

extern _ARMV4_Exception_data_abort_default;

uint32_t translation_table[] = 
{ 
  MMU_DATA_READ_WRITE,
  ARMV7_MMU_READ_ONLY,
  MMU_DATA_READ_WRITE,
  0U /* fault section */
};

typedef struct {
  uint32_t register_r0;
  uint32_t register_r1;
  uint32_t register_r2;
  uint32_t register_r3;
  uint32_t register_r12;
  void *register_lr;
  void *register_pc;
  uint32_t register_xpsr;
} ARMV7M_Exception_frame;

BSP_START_DATA_SECTION static const arm_cp15_start_section_config
rvpbxa9_mmu_config_table[] = {
  {
    .begin = (uint32_t) bsp_section_fast_text_begin,
    .end = (uint32_t) bsp_section_fast_text_end,
    .flags = ARMV7_MMU_CODE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_fast_data_begin,
    .end = (uint32_t) bsp_section_fast_data_end,
    .flags = MMU_DATA_READ_WRITE
  }, {
    .begin = (uint32_t) bsp_section_start_begin,
    .end = (uint32_t) bsp_section_start_end,
    .flags = ARMV7_MMU_CODE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_vector_begin,
    .end = (uint32_t) bsp_section_vector_end,
    .flags = MMU_DATA_READ_WRITE
  }, {
    .begin = (uint32_t) bsp_section_text_begin,
    .end = (uint32_t) bsp_section_text_end,
    .flags = ARMV7_MMU_CODE_CACHED
  }, {
    .begin = (uint32_t) bsp_section_rodata_begin,
    .end = (uint32_t) bsp_section_rodata_end,
    .flags = ARMV7_MMU_DATA_READ_ONLY_CACHED
  }, {
    .begin = (uint32_t) bsp_section_data_begin,
    .end = (uint32_t) bsp_section_data_end,
    .flags = MMU_DATA_READ_WRITE
  }, {
    .begin = (uint32_t) bsp_section_bss_begin,
    .end = (uint32_t) bsp_section_bss_end,
    .flags = MMU_DATA_READ_WRITE
  }, {
    .begin = (uint32_t) bsp_section_work_begin,
    .end = (uint32_t) bsp_section_work_end,
    .flags = MMU_DATA_READ_WRITE
  }, {
    .begin = (uint32_t) bsp_section_stack_begin,
    .end = (uint32_t) bsp_section_stack_end,
    .flags = MMU_DATA_READ_WRITE
  }, {
    .begin = 0x10000000U,
    .end = 0x10020000U,
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = 0x1f000000U,
    .end = 0x20000000U,
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = 0x0C000000U,
    .end = 0x0CFFFFFFU,
    .flags = MMU_DATA_READ_WRITE
  }, {
    // Map page table itself
    .begin = (uint32_t) bsp_translation_table_base,
    // 16 KB L1 section entries
    .end = (uint32_t) bsp_translation_table_base + 0x4000,
    .flags = MMU_DATA_READ_WRITE
  }
};

static void translate_attributes(
  uint32_t high_level_attr,
  uint32_t *ARM_CPU_ATTR
)
{

  /* Clear flags attributes */
  *ARM_CPU_ATTR = 0;

  /* No protection */
  if ( high_level_attr == 0 )
    *ARM_CPU_ATTR = MMU_DATA_READ_WRITE;

  if( high_level_attr & 0x1 )
    *ARM_CPU_ATTR |= ARMV7_MMU_READ_ONLY;

  /* Write access */
  if ( high_level_attr & 0x2 )
    *ARM_CPU_ATTR |= MMU_DATA_READ_WRITE;
} 

void _CPU_Memory_management_Initialize(void)
{	

  uint32_t ctrl = arm_cp15_start_setup_mmu_and_cache(
    		  0,
    		  ARM_CP15_CTRL_AFE | ARM_CP15_CTRL_Z
  		 );

  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
  ctrl,
  (uint32_t *) bsp_translation_table_base,
  ARM_MMU_DEFAULT_CLIENT_DOMAIN,
  &rvpbxa9_mmu_config_table[0],
  RTEMS_ARRAY_SIZE(rvpbxa9_mmu_config_table)
  );
}

void _CPU_Memory_management_Set_attributes(uintptr_t base,size_t size, uint32_t attr)
{

  /* required as arm_cp15_set_translation_table_entries needs a pointer to end not value */
  uint32_t end = (uint32_t)base + (uint32_t)size;
  uint32_t section_flags;
  
  /* translate flags from high-level to ARM specific MMU flags */
  if ( attr != RTEMS_MM_REGION_NO_ACCESS )
  {
    //translate_attributes(attr, &section_flags);
    section_flags = translation_table[attr];

#if DEBUG
    printf("section flags = 0x%x \n", section_flags);
#endif

    arm_cp15_set_translation_table_entries(base, end, section_flags);
  } else 
  {
     arm_cp15_unset_translation_table_entries(base, end);
  }
};

static void print_data(ARMV7M_Exception_frame *frame)
{
  printk("fault at address 0x%x \n",frame->register_lr);
}

void __attribute__((naked)) dummy_data_abort_exception_handler(void)
{

#if DEBUG
  printk("Entered exception handler \n");

 uint32_t address_fault = (uint32_t)arm_cp15_get_fault_address();
 printk("Data fault address at  0x%x\n",address_fault);

 uint32_t fsr = (uint32_t) arm_cp15_get_data_fault_status();
 arm_cp15_print_fault_status_description(fsr);
#endif

//TODO: Call rtems_fatal
exit(0);
}


#ifdef __cplusplus
}
#endif
