/**
 * @file
 *
 * @brief Inlined Routines from the Memory Management Manager
 */

/*
 * Copyright (c) 2013 Hesham AL-Matary
 * Copyright (c) 2013 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_MM_H
# error "Never use <rtems/score/mm.inl> directly; include <rtems/score/mm.h> instead."
#endif

#ifndef _RTEMS_SCORE_MM_INL
#define _RTEMS_SCORE_MM_INL

#include <stdlib.h>

/**
 * @addtogroup SuperCoreMM
 */
/**@{**/

/**
 * @brief Calls _CPU_Memory_management_Initialize.
 */
void _Memory_management_Initialize( void )
{
#ifdef RTEMS_SMP   
  _SMP_lock_Initialize( &mm_lock );
#endif 

  _CPU_Memory_management_Initialize();
}

/**
 * @brief Calls _CPU_Memory_management_Install_entry.
 */
void _Memory_management_Install_entry(
  uintptr_t base,
  size_t size,
  uint32_t attr
)
{
#ifdef RTEMS_SMP   
  _SMP_lock_Acquire( &mm_lock );
#endif

  _CPU_Memory_management_Install_entry(base, size, attr);

#ifdef RTEMS_SMP    
  _SMP_lock_Release( &mm_lock );
#endif
}

/**
 * @brief Calls _CPU_Memory_management_Uninstall_entry.
 */
void _Memory_management_Uninstall_entry(
  uintptr_t base,
  size_t size
)
{
#ifdef RTEMS_SMP   
  _SMP_lock_Acquire( &mm_lock );
#endif

  _CPU_Memory_management_Uninstall_entry(base, size);

#ifdef RTEMS_SMP    
  _SMP_lock_Release( &mm_lock );
#endif

}

/** @}*/

#endif
