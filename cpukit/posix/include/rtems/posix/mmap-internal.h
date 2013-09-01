/*
 *  mmap() - POSIX 1003.1b 6.3.1 - map pages of memory
 *
 *  COPYRIGHT (c) 2010.
 *  Chris Johns (chrisj@rtems.org)
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#ifndef MMAP_INTERNAL_H
#define MMAP_INTERNAL_H
#include <stdint.h>

#include <rtems/score/mm.h>

/*Posix API manipulate the  ALUT in their own way,
 * so please never use the libmmu and mmap at the same time 
 */

bool mmap_mappings_lock_obtain( void );
bool mmap_mappings_lock_release( void );

#endif
