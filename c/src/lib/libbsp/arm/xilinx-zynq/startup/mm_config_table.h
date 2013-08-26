/*
 * Copyright (c) 2013 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <info@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <bsp/arm-cp15-start.h>
#include <libcpu/arm-cp15.h>

#ifdef RTEMS_SMP
  #define MMU_DATA_READ_WRITE ARMV7_MMU_DATA_READ_WRITE_SHAREABLE
#else
  #define MMU_DATA_READ_WRITE ARMV7_MMU_DATA_READ_WRITE_CACHED
#endif

#define translate_attributes(attr) \
  ((ARM_MMU_DEFAULT_CLIENT_DOMAIN << ARM_MMU_SECT_DOMAIN_SHIFT) \
  | (ARM_MMU_SECT_AP_0) \
  | (((attr) >> RTEMS_MM_REGION_BIT_WRITE) ? 0U : (ARM_MMU_SECT_AP_2)) \
  | (((attr) >> RTEMS_MM_REGION_BIT_CACHE) ? (ARM_MMU_SECT_TEX_0|ARM_MMU_SECT_C|ARM_MMU_SECT_B) : 0U) \
  | (((attr) >> RTEMS_MM_REGION_BIT_DEVICE) ? ARM_MMU_SECT_B : 0U) \
  | (((attr) >> RTEMS_MM_REGION_BIT_SHARED) ? ARM_MMU_SECT_S : 0U) \
  | (ARM_MMU_SECT_DEFAULT))

BSP_START_DATA_SECTION static const arm_cp15_start_section_config
_cpu_mmu_config_table[] = {
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
    .begin = 0xe0000000U,
    .end = 0xe0200000U,
    .flags = ARMV7_MMU_DEVICE
  }, {
    .begin = 0xf8000000U,
    .end = 0xf9000000U,
    .flags = ARMV7_MMU_DEVICE
  }
};

const static uint32_t translation_table[] =
{
  ARMV7_MMU_DATA_READ_WRITE,
  ARMV7_MMU_READ_ONLY,
  ARMV7_MMU_DATA_READ_WRITE,
  0U
};
