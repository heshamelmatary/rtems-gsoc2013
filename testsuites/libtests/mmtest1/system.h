/* system.h
 *
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#include <rtems.h>

/* functions */

rtems_task Init(
  rtems_task_argument argument
);

/* configuration information */

#include <bsp.h> /* for device driver prototypes */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            9

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

#define CONFIGURE_MEMORY_OVERHEAD 1024 * 3 

#include <rtems/confdefs.h>
/* end of include file */
