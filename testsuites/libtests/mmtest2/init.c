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
#include <rtems/score/heapimpl.h>
#include <rtems/score/wkspace.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  unsigned char* a1;
  unsigned char* a2;
  char a;
  size_t size = 0x001000;

  printf( "\n\n*** Start of mmtest2 ***\n" );

  uintptr_t *region1 = _Workspace_Allocate(size);
  uintptr_t *region2 = _Workspace_Allocate(size);

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
    region1);
  } else 
  {
    printf("Failed to allocate memory for Region 2 from Work Space ! \n");
    exit(0);
  }

  printf("Test 1: set Region1  as read only\n");
  _Memory_management_Set_attributes( region1, size, RTEMS_MM_REGION_PROTECTION_READ_ONLY);

  printf("Test 2: set Region2  as write enabled\n");
  _Memory_management_Set_attributes( region2, size, RTEMS_MM_REGION_PROTECTION_WRITE);

  // TODO: Make fatal tests withing exception handler */
  /* FIXME: make addresses target-independent */

  a1 = (char *) region2;
  printf("Test 3: Write to write enabled block\n");
  //*a1 = 0xCC;

  a1 = (char*)0xffffffffU;
  printf("Checking MMU exception 1: Read from Unmapped block\n");
  a = *a1++;

  a1 = (char*)0xffffffffU + 0x2000U;
  printf("Checking MMU exception 2: Write to Unmapped block\n");
  *a1++ = 0xCC;

  // this one isn't an exception.
  a2 = (char *) region1;
  printf("Checking MMU exception 4: Read from readonly block\n");
  a = *a2++;

  printf("Checking MMU exception 5: Write to readonly block  \n");
  *a2++ = 0xCC;

  printf("Test 6: set region2 with no access attributes \n");
  _Memory_management_Set_attributes(region2, size, RTEMS_MM_REGION_NO_ACCESS);
  
  a1 = (char *) region2;
  printf("Checking MMU exception 6: Write to uninstalled block \n");
  *a1 = 0xCC;

  printf(  "\n\n*** End of mmtest2 ***\n" );
  exit( 0 );
}
