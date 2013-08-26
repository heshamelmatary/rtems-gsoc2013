/* Init
 *
 * This routine is the initialization task for this test program.
 */

/*
 * Copyright (c) 2012-2013 Hesham Al-Matary.
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */
#define __RTEMS_VIOLATE_KERNEL_VISIBILITY__
#define CONFIGURE_INIT
#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/score/mm.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  uint32_t *a1;
  uint32_t *a2;
  uint32_t a;
  size_t size = 0x00100000;

  uint32_t *region1 = (uint32_t *) 0x0C000000U;
  uint32_t *region2 = (uint32_t *) 0x0C200000U;

  puts( "\n\n*** Start of mmtest2 ***\n" );

  printf("Test 1: set Region1  as read only\n");
  _Memory_management_Set_attributes( region1, size, RTEMS_MM_REGION_READ);

  printf("Test 2: set Region2  as write enabled\n");
  _Memory_management_Set_attributes( region2, size, RTEMS_MM_REGION_WRITE);

  // TODO: Make fatal tests withing exception handler */
  /* FIXME: make addresses target-independent */

  a1 = region2;
  printf("Test 3: Write to write enabled block\n");
  *a1 = 0xCC;

  a1 = (char*)0xffffffffU;
  printf("Checking MMU exception 1: Read from Unmapped block\n");
  a = *a1++;

  a1 = 0xffffffffU;
  printf("Checking MMU exception 2: Write to Unmapped block\n");
  *a1++ = 0xCC;

  // this one isn't an exception.
  a2 = (char *) region1;
  printf("Checking MMU exception 4: Read from readonly block\n");
  a = *a2++;

  printf("Checking MMU exception 5: Write to readonly block  \n");
  *a2++ = 0xCC;

  printf(  "\n\n*** End of mmtest2 ***\n" );
  exit( 0 );
}
