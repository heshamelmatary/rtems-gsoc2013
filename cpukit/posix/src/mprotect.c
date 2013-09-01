/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include <rtems.h>
#include <stdlib.h>
#include <rtems/score/mm.h>

//#include <rtems/posix/mmap-internal.h>
#include <unistd.h>
#include <errno.h>
#include <rtems/posix/mman.h>
/*
 *  12.2.3 Change Memory Protection, P1003.1b-1996, p. 277.
 *
 *  This is not a functional version but the SPARC backend for at least
 *  gcc 2.8.1 plus gnat 3.13p and gcc 3.0.1 require it to be there and
 *  return 0.
 *
 *  As of gcc 4.2.2, the gcc SPARC backend doesn't appear to have a
 *  way to call this for RTEMS anymore but it doesn't hurt to leave it.
 */

#define PROT_NONE       0x00    /* no permissions */
#define PROT_READ	RTEMS_MM_REGION_READ
#define PROT_WRITE      RTEMS_MM_REGION_WRITE
#define PROT_EXEC       RTEMS_MM_REGION_EXECUTE
#define MAP_SHARED	RTEMS_MM_REGION_SHARED

int mprotect(const void *addr, size_t len, int prot)
{
  size_t  ps;

  /*
   * Clear errno.
   */
  errno = 0;
  /*
   * Length cannot be 0.
   */
  if ( len <= 0 ) {
    errno = EINVAL;
    return -1;
  }

  ps = sysconf( _SC_PAGESIZE ) ;
  /*addr is not a mutiple of the page size*/
  if( ((long)addr % ps != 0) || (len % ps != 0)){
    errno = EINVAL;
    return -1;
  }

  /*
   * Obtain the mmap lock. Sets errno on failure.
   */
  /*if ( !mmap_mappings_lock_obtain( ))
    return -1;
  */

  /* for the private mmap, this won't happen
   * I don't consider much about the file access right
   */
  
  if( 0 )
    return EACCES;

  /* I do not quite understand about this error signal*/
 
  if( 0 )
    return  EAGAIN;
      
  _Memory_management_Set_attributes(addr, len, prot);

  return 0;
}
