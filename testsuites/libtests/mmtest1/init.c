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

#define BLOCK_SIZE  1024 * 1024 /* 1MB */
#define BLOCK_COUNT 3

char memory_pool [BLOCK_SIZE * BLOCK_COUNT];
char *a1;
char *a2;
char a;
size_t size = BLOCK_SIZE;

char *region1 = &memory_pool;
char *region2 = (((char *) &memory_pool) + (BLOCK_SIZE));

rtems_task Init(
  rtems_task_argument ignored
)
{
  puts( "\n\n*** Start of mmtest1 ***\n" );

  printf("Test 1 : Set Region1 as write enabled\n");
  _Memory_management_Set_attributes( region1, size, RTEMS_MM_REGION_WRITE);

  printf("Test 2: Set Region2 as read only\n");
  _Memory_management_Set_attributes( region2, size, RTEMS_MM_REGION_READ);

  printf( "\n\n*** End of mmtest1 ***\n" );

  exit( 0 );
}
