/*  Application_task
 *
 *  This routine is trys to dynamically change shared resource
 *  that affects memory protection area attributes withing a SMP
 *  system.
 *
 *  COPYRIGHT (c) 2013 Hesham AL-Matary.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/score/mm.h>

#define DEBUG 1

rtems_task Libmm_Task(
  rtems_task_argument task_index
)
{
  int               cpu_num;
  char              name[5];
  char             *p;

  Memory_management_Entry mme1 = {
    .name = "Valid Entry-1",
    .base = 0x0C000000U,
    .size = 0x1000U,
    .installed = false,
    .bsp_mme = NULL
  };

  /* Get the task name */
  p = rtems_object_get_name( RTEMS_SELF, 5, name );
  rtems_test_assert( p != NULL );

   /* Get the CPU Number */
  cpu_num = bsp_smp_processor_id();

  /* Print that the task is up and running. */
  Loop();
  printf(" CPU %d running Task %s\n", cpu_num, name);

  /* Set the flag that the task is up and running */
  TaskRan[cpu_num] = true;


  /* Drop into a loop which will keep this task on
   * running on the cpu.
   */
  while(1);
}
