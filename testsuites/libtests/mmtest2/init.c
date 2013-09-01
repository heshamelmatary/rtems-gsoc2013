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
#define CONFIGURE_INIT

#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/score/mm.h>
#include <assert.h>

#define BLOCK_SIZE  1024 * 1024 /* 1MB */
#define BLOCK_COUNT 3

char memory_pool [BLOCK_SIZE * BLOCK_COUNT];
char *a1;
char *a2;
char a;
size_t size = BLOCK_SIZE;

char *region1 = &memory_pool;
char *region2 = (((char *) &memory_pool) + (BLOCK_SIZE));

static void fatal_extension(
  rtems_fatal_source source,
  bool is_internal,
  rtems_fatal_code error
)
{
  if (source == RTEMS_FATAL_SOURCE_EXCEPTION)
     {
	printk("Internal Exception Occured \n");
     }

  exit(0);
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  puts( "\n\n*** Start of mmtest2 ***\n" );

  printf("Region1 starts at 0x%x \n Region 2 starts at 0x%x\n",region1, region2);
  printf("Test 1: set Region1  as write enabled\n");
  _Memory_management_Set_attributes(region1, size, RTEMS_MM_REGION_WRITE);

  printf("Test 2: set Region2  as read only\n");
  _Memory_management_Set_attributes(region2, size, RTEMS_MM_REGION_READ);

  /* FIXME: make addresses target-independent */

  a1 = region1;
  printf("Test 3: Write to write enabled block\n");
  *a1 = 0xCC;

  a1 = (char*)0xffffffffU;
  printf("Checking MMU exception 1: Read from Unmapped block\n");
  //a = *a1++;

  a1 = 0xffffffffU;
  printf("Checking MMU exception 2: Write to Unmapped block\n");
  //*a1++ = 0xCC;

  // this one isn't an exception.
  a2 = (char *) region2;
  printf("Checking MMU exception 4: Read from readonly block\n");
  a = *a2++;

  printf("Checking MMU exception 5: Write to readonly block  \n");
  *a2++ = 0xCC;

  printf(  "\n\n*** End of mmtest2 ***\n" );
  exit( 0 );
}
