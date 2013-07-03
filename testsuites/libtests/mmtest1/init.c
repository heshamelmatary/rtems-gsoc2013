/* Init
 *
 * This routine is the initialization task for this test program.
 *
 */

/*
 * Copyright (c) 2013 Gedare Bloom.
 * Copyright (c) 2012 Hesham Al-Matary.
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
  Memory_management_Entry mme1 = {
    .name = "Valid Entry-1",
    .base = 0x00100000,
    .size = 0x200000,
    .installed = false,
    .bsp_mme = NULL
  };
  Memory_management_Entry mme2 = {
    .name = "Valid Entry-2",
    .base = 0x00400000,
    .size = 0x100000,
    .installed = false,
    .bsp_mme = NULL
  };

  puts( "\n\n*** Start of mmtest1 ***\n" );

  puts( "initialize the memory management manager\n");
  //_Memory_management_Initialize ( );

  printf("Test 1: Installing Entry-1 \
and set it as Read only\n");
  //_Memory_management_Install_entry( &mme1, RTEMS_MM_REGION_PROTECTION_READ_ONLY);

  printf("Test 2 : Installing Entry-2\
and set it as Write enabled\n");
  //_Memory_management_Install_entry( &mme2, RTEMS_MM_REGION_PROTECTION_WRITE);

  printf( "\n\n*** End of mmtest1 ***\n" );

  exit( 0 );
}
