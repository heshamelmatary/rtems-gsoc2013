/**
 * @file
 *
 * @ingroup ScoreCPUARMCP15
 *
 * @brief ARM co-processor 15 (CP15) API.
 */

/*
 * Copyright (c) 2013 Hesham AL-Matary
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef LICPU_SHARED_ARM_CP15_DATA_FAULT_INFO
#define LICPU_SHARED_ARM_CP15_DATA_FAULT_INFO
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Define mask for ARM CP15 fault status register */
#define ARM_CP15_FAULT_STATUS_MASK 0x040F

/* Error States for ARM CP15 fault status register */
#define ARM_CP15_ALIGNMENT_FAULT   0x00000001
#define ARM_CP15_BACKGROUND_FAULT  0x0000
#define ARM_CP15_ACCESS_PERMISSION_FAULT 0x000D
#define ARM_CP15_PRECISE_EXTERNAL_ABORT_FAULT 0x0008 
#define ARM_CP15_IMPRECISE_EXTERNAL_ABORT_FAULT 0x0406
#define ARM_CP15_PRECISE_PARITY_ERROR_EXCEPTION 0x0006
#define ARM_CP15_IMPRECISE_PARITY_ERROR_EXCEPTION 0x0408
#define ARM_CP15_DEBUG_EVENT 0x0002

/* print error description */

void arm_cp15_print_fault_status_description(uint32_t fsr)
{

  uint32_t error = fsr & ARM_CP15_FAULT_STATUS_MASK;
  //printk("fsr before mask is 0x%x and after mask is 0x%x\n",fsr,error);
  switch(error)
  {
    case ARM_CP15_ALIGNMENT_FAULT:
      printk("Fault Status : Alignment fault !\n");
      break;

    case ARM_CP15_BACKGROUND_FAULT:
      printk("Fault Status : Background fault !\n");
      break;

    case ARM_CP15_ACCESS_PERMISSION_FAULT:
      printk("Fault Status : Memory Access Permission fault !\n");
      break;
   case ARM_CP15_PRECISE_EXTERNAL_ABORT_FAULT:
      printk("Fault Status : Precise external abort !\n");
      break;

   case ARM_CP15_IMPRECISE_EXTERNAL_ABORT_FAULT:
      printk("Fault Status : Imprecise external abort !\n");
      break;
   
   case ARM_CP15_PRECISE_PARITY_ERROR_EXCEPTION:
      printk("Fault Status : Precise parity error excpetion !\n");
      break;

   case ARM_CP15_IMPRECISE_PARITY_ERROR_EXCEPTION:
      printk("Fault Status : Imprecise parity error excpetion !\n");
      break;

   case ARM_CP15_DEBUG_EVENT:
      printk("Fault Status : Debug event !\n");
      break;

   default:
      printk("Unknown Error !\n");
  }
}

#ifdef __cplusplus
#endif /* LICPU_SHARED_ARM_CP15_DATA_FAULT_INFO */
