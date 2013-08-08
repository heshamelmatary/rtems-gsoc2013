/* 
 * Copyright (c) 2013 Hesham AL-Matary.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <libcpu/arm-cp15.h>
#include <libcpu/mm.h>
#include <bsp/start.h>
#include <bsp/linker-symbols.h>
#include <libcpu/arm_cp15_print_fsr.h>
//#include <stdlib.h>

#define DEBUG 1

static void translate_attributes(
  uint32_t high_level_attr,
  uint32_t *ARM_CPU_ATTR
)
{

  /* Clear flags attributes */
  *ARM_CPU_ATTR = 0;

  if( high_level_attr & 0x1 )
    *ARM_CPU_ATTR |= ARM_MMU_SECT_AP_1 | ARM_MMU_SECT_AP_0 | ARM_MMU_SECT_AP_2 ;

  /* Write access */
  if ( high_level_attr & 0x2 )
	{
    *ARM_CPU_ATTR |= ARM_MMU_SECT_AP_1 | ARM_MMU_SECT_AP_0;
    *ARM_CPU_ATTR &= ~(ARM_MMU_SECT_AP_2);
	}
} 

void _CPU_Memory_management_Initialize(void)
{	
  uint32_t ctrl = 0;
  uint32_t client_domain = 0;
  uint32_t *ttb;
  ctrl = arm_cp15_get_control();

  uint32_t dac = ARM_CP15_DAC_DOMAIN(client_domain, ARM_CP15_DAC_CLIENT);
  size_t i;

  arm_cp15_set_domain_access_control(dac);

  arm_cp15_set_translation_table_base((uint32_t *) bsp_translation_table_base);

  ttb = arm_cp15_get_translation_table_base();

  for (i = 0; i < ARM_MMU_TRANSLATION_TABLE_ENTRY_COUNT; ++i) {
    /* Set default first-level sections' attributes to no protection, thus,
       RW enabled for all memory map */
    ttb [i] = ((i << ARM_MMU_SECT_BASE_SHIFT) | ARM_MMU_SECT_AP_1 | ARM_MMU_SECT_AP_0 | 2 ) ;
  }

  /* Enable MMU, dCache, iCache, Force protection bit in CP15 control register */
  ctrl |= ARM_CP15_CTRL_AFE | ARM_CP15_CTRL_S | ARM_CP15_CTRL_I | ARM_CP15_CTRL_C | ARM_CP15_CTRL_M | ARM_CP15_CTRL_XP | ARM_CP15_CTRL_R;

  arm_cp15_tlb_invalidate();

  arm_cp15_set_control(ctrl);

  /*
  arm_cp15_start_setup_translation_table_and_enable_mmu_and_cache(
    ctrl,
    (uint32_t *) bsp_translation_table_base,
    ARM_MMU_DEFAULT_CLIENT_DOMAIN,
    &rvpbxa9_mmu_config_table[0],
    RTEMS_ARRAY_SIZE(rvpbxa9_mmu_config_table)
  );*/
}

void _CPU_Memory_management_Install_entry(uintptr_t base,size_t size, uint32_t attr)
{

  /* required as arm_cp15_set_translation_table_entries needs a pointer to end not value */
  uint32_t end = (uint32_t)base + (uint32_t)size;
  uint32_t section_flags;
  
  /* translate flags from high-level to ARM specific MMU flags */
  translate_attributes(attr, &section_flags);

  rtems_interrupt_level level;
  rtems_interrupt_disable(level);
		
  /****************/
	uint32_t cl_size = arm_cp15_get_min_cache_line_size();
	uint32_t i = ARM_MMU_SECT_GET_INDEX(base);
        uint32_t iend = ARM_MMU_SECT_GET_INDEX(ARM_MMU_SECT_MVA_ALIGN_UP(end));
	uint32_t ctrl;
	uint32_t cpsr = 0;
	uint32_t *ttb = arm_cp15_get_translation_table_base();
	uint32_t *table = (uint32_t *) bsp_section_vector_begin;
	
	//ctrl = arm_cp15_mmu_disable(cl_size);
	ctrl = arm_cp15_get_control();
	arm_cp15_set_control(ctrl & ~(ARM_CP15_CTRL_M | ARM_CP15_CTRL_C | ARM_CP15_CTRL_I));
#if DEBUG
	printf("Control Register is 0x%x \n",arm_cp15_get_control());
	printf("the start index is %i and the end index is %i \n",i,iend);
	printf("Domain access control register 0x%x \n",arm_cp15_get_domain_access_control());
	printf("ttb is 0x%x \n",(uint32_t) arm_cp15_get_translation_table_base());
	printf("interrupt vector start at is 0x%x \n",(uint32_t) table);
#endif	
while (i < iend) {
    uint32_t addr = i << ARM_MMU_SECT_BASE_SHIFT;

#if DEBUG
	printf("PTE before setting attributes is 0x%x \n",ttb[i]);
	printf("Section Flags to be set are 0x%x \n",section_flags);
#endif
    ttb [i] |= addr | section_flags;
#if DEBUG
	printf("PTE after setting attributes is 0x%x \n",ttb[i]);
#endif	

    ++i;
  }

#if DEBUG
	printf("Finished installing PTEs \n");
#endif	
	//arm_cp15_tlb_invalidate();
	arm_cp15_set_control(ctrl);

  /****************/

	rtems_interrupt_enable(level);
#if DEBUG
printf("Control Register after enable MMU is 0x%x \n",arm_cp15_get_control());
#endif	
    __asm__ volatile (
    ARM_SWITCH_TO_ARM
    "mrs %[cpsr], cpsr\n"
    ARM_SWITCH_BACK
    : [cpsr] "=&r" (cpsr)
    );
#if DEBUG
printf("CPSR after enable MMU is 0x%x \n",cpsr);
#endif	
/*
    // set A bit for abort Enable 
    cpsr |= 1U << 8;
    __asm__ volatile (
    ARM_SWITCH_TO_ARM
    "msr cpsr, %[cpsr]\n"
    ARM_SWITCH_BACK
    : [cpsr] "=&r" (cpsr)
    );
*/
#if DEBUG
printf("Control Register after enable MMU is 0x%x \n",arm_cp15_get_control());
#endif	
    __asm__ volatile (
    ARM_SWITCH_TO_ARM
    "mrs %[cpsr], cpsr\n"
    ARM_SWITCH_BACK
    : [cpsr] "=&r" (cpsr)
    );
#if DEBUG
printf("CPSR after setting A bit 0x%x \n",cpsr);
#endif	
  //arm_cp15_set_translation_table_entries(base, end, section_flags);
}

void _CPU_Memory_management_Uninstall_entry(uintptr_t base, size_t size)
{
  /* required as arm_cp15_set_translation_table_entries needs a pointer to end not value */
  uint32_t end = (uint32_t)base + (uint32_t)size;
  uint32_t section_flags;

  rtems_interrupt_level level;
	rtems_interrupt_disable(level);
		
  /****************/
	uint32_t cl_size = arm_cp15_get_min_cache_line_size();
	uint32_t i = ARM_MMU_SECT_GET_INDEX(base);
  uint32_t iend = ARM_MMU_SECT_GET_INDEX(ARM_MMU_SECT_MVA_ALIGN_UP(end));
	uint32_t ctrl;
	uint32_t *ttb = arm_cp15_get_translation_table_base();

	
	ctrl = arm_cp15_mmu_disable(cl_size);
#if DEBUG
	printf("the start index is %i and the end index is %i \n",i,iend);
#endif	
while (i < iend) {
    uint32_t addr = i << ARM_MMU_SECT_BASE_SHIFT;

		// Return back to default attributes
    ttb [i] = ((i << ARM_MMU_SECT_BASE_SHIFT) | ARM_MMU_SECT_AP_1 | ARM_MMU_SECT_AP_0 | 2 ) ;

    ++i;
  }
	//arm_cp15_tlb_invalidate();
	arm_cp15_set_control(ctrl);

  /****************/

	rtems_interrupt_enable(level);
}

void __attribute__((naked)) dummy_data_abort_exception_handler(void)
{
  uint32_t cpsr;
#if DEBUG
  printk("Entered exception handler \n");

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

