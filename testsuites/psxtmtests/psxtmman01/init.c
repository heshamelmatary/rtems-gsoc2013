/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <timesys.h>
#include <rtems/timerdrv.h>
#include "test_support.h"
#include <pthread.h>
#include <rtems/posix/mman.h>

/* forward declarations to avoid warnings */
void *POSIX_Init(void *argument);

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
}

uint32_t *region1 = (uint32_t *) 0x0C000000U;
size_t size = 0x00100000;
uint32_t *a1;

void *POSIX_Init(
  void *argument
)
{
  puts( "\n\n*** POSIX mprotect test01 ***" );

  puts( "Test1 : call mprotect and apply read attributes at a region \n");
  _Memory_management_Set_attributes( region1, size, PROT_READ);
  a1 = (char *) region1;
  puts("Checking Memory Violation : Write to readonly block  \n");
  *a1++ = 0xCC;

  puts( "*** END OF POSIX mprotect test01  ***" );

  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS     1
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT

#define CONFIGURE_INITIAL_EXTENSIONS { .fatal = fatal_extension }

#define CONFIGURE_MEMORY_OVERHEAD 1024 * 3 

#include <rtems/confdefs.h>
/* end of file */
