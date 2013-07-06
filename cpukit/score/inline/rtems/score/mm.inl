/**
 * @file
 *
 * @brief Inlined Routines from the Memory Management Manager
 */

/*
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

/**
 * @addtogroup SuperCoreMM
 */
/**@{**/

/**
 * @brief Calls _CPU_Memory_management_Initialize.
 */
RTEMS_INLINE_ROUTINE void _Memory_management_Initialize( void )
{
#ifdef RTEMS_SMP   
   _SMP_lock_Initialize( &mm_lock );
#endif 

  _CPU_Memory_management_Initialize();
}

/**
 * @brief Calls _CPU_Memory_management_Install_entry.
 */
RTEMS_INLINE_ROUTINE void _Memory_management_Install_entry(
  Memory_management_Entry *mme,
  uint32_t attr
)
{
#ifdef RTEMS_SMP   
  _SMP_lock_Acquire( &mm_lock );
#endif

  _CPU_Memory_management_Install_entry(mme, attr);

#ifdef RTEMS_SMP    
  _SMP_lock_Release( &mm_lock );
#endif
}

/**
 * @brief Calls _CPU_Memory_management_Uninstall_entry.
 */
RTEMS_INLINE_ROUTINE void _Memory_management_Uninstall_entry(
  Memory_management_Entry *mme
)
{
#ifdef RTEMS_SMP   
  _SMP_lock_Acquire( &mm_lock );
#endif

  _CPU_Memory_management_Uninstall_entry(mme);

#ifdef RTEMS_SMP    
  _SMP_lock_Release( &mm_lock );
#endif

}

/** @}*/

#endif
