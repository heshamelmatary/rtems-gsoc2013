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

#ifndef _RTEMS_SCORE_MMIMPL_H
#define _RTEMS_SCORE_MMIMPL_H

#ifdef RTEMS_SMP
#include <rtems/score/smplock.h>
#endif

#include <rtems/score/mm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief _Memory_management_Region Flags defs
 *  
 */
#define RTEMS_MM_REGION_PROTECTION_READ_ONLY   0x1
#define RTEMS_MM_REGION_PROTECTION_WRITE  0x2
//#define RTEMS_MM_REGION_PROTECTION_EXEC   0x4

#ifdef RTEMS_SMP
SMP_lock_Control mm_lock;
#endif

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
RTEMS_INLINE_ROUTINE void _Memory_management_Uninstall_entry(
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

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
