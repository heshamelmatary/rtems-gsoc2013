/* Init
 *
 * This routine is the initialization task for this test program.
 *
 */

/*
 * Copyright (c) 2012-2013 Hesham Al-Matary.
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#define CONFIGURE_INIT
#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/score/mm.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  size_t size = 0x00100000; 
  uintptr_t *region1 = _Workspace_Allocate(size);
  uintptr_t *region2 = _Workspace_Allocate(size);

  puts( "\n\n*** Start of mmtest1 ***\n" );

  if (region1 != NULL)
  {
    printf("Region 1 successfully allocated from Work Space at address 0x%x \n", \
region1);
  } else 
  {
    printf("Failed to allocate memory for Region 1 from Work Space ! \n");
    exit(0);
  }

  if (region2 != NULL)
  {
    printf("Region 2 successfully allocated from Work Space at address 0x%x \n", \
 region2);
  } else
  {
    printf("Failed to allocate memory for Region 2 from Work Space ! \n");
    exit(0);
  }

  printf("Test 1 : Set Region2 as write enabled\n");
  _Memory_management_Set_attributes( region2, size, RTEMS_MM_REGION_WRITE);

  printf("Test 2: Set Region1 as read only\n");
  _Memory_management_Set_attributes( region1, size, RTEMS_MM_REGION_READ);

  printf( "\n\n*** End of mmtest1 ***\n" );

  exit( 0 );
}
